// -*- C++ -*-
//
// Package:    LhcTackAnalyzer
// Class:      LhcTrackAnalyzer
// 
/**\class LhcTrackAnalyzer LhcTrackAnalyzer.cc MySub/LhcTrackAnalyzer/src/LhcTrackAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//

// updated to 25/2/2009 5.30 pm

//
//

// system include files
#include <memory>


// user include files
#include "UserCode/LhcTrackAnalyzer/interface/LhcTrackAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/ESHandle.h>
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include <SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h>
#include <SimDataFormats/TrackingHit/interface/PSimHit.h>

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TChain.h"
#include "TNtuple.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include <Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h>
#include <DataFormats/GeometrySurface/interface/Surface.h>
#include <DataFormats/GeometrySurface/interface/GloballyPositioned.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h" 

// Vertex Stuff
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"  
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

// BeamSpot 
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
typedef math::XYZTLorentzVectorF LorentzVector;
typedef math::XYZPoint Point;

using namespace std;
using namespace reco;
using namespace edm;



// Constructor

LhcTrackAnalyzer::LhcTrackAnalyzer(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  debug_    = iConfig.getParameter<bool>       ("Debug");  
  trackCollectionTag_      = iConfig.getParameter<edm::InputTag>("TrackCollectionTag");
  vertexCollectionTag_       = iConfig.getUntrackedParameter<edm::InputTag>("vertexCollection");
  bsSrc = iConfig.getParameter< edm::InputTag >("beamSpot");
  filename_ = iConfig.getParameter<std::string>("OutputFileName");
}
   
// Destructor
LhcTrackAnalyzer::~LhcTrackAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
LhcTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //=======================================================
  // Initialize Root-tuple variables
  //=======================================================

  SetRootVar();
 
  //=======================================================
  // BeamSpot accessors
  //=======================================================

  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByLabel(bsSrc,recoBeamSpotHandle);
  reco::BeamSpot bs = *recoBeamSpotHandle;     
  
  const Point beamSpot = recoBeamSpotHandle.isValid() ? Point(recoBeamSpotHandle->x0(), recoBeamSpotHandle->y0(), recoBeamSpotHandle->z0()) : Point(0, 0, 0);
  
  //=======================================================
  // PrimaryVertex accessors
  //=======================================================
  static const reco::VertexCollection s_empty_vertexColl;
  const reco::VertexCollection *vertexColl = &s_empty_vertexColl;
  edm::Handle<reco::VertexCollection> vertexCollectionHandle;
  iEvent.getByLabel(vertexCollectionTag_, vertexCollectionHandle);
  if( iEvent.getByLabel(vertexCollectionTag_, vertexCollectionHandle)) {
    vertexColl = vertexCollectionHandle.product();
  } else {
    cout << "vertexCollection cannot be found -> using empty collection of same type." <<endl;
  } 
   
  //=======================================================
  // Track accessors
  //=======================================================
  static const reco::TrackCollection s_empty_trackColl;
  const reco::TrackCollection *trackColl = &s_empty_trackColl;
  edm::Handle<reco::TrackCollection> trackCollectionHandle;
  iEvent.getByLabel(trackCollectionTag_, trackCollectionHandle);
  if( iEvent.getByLabel(trackCollectionTag_, trackCollectionHandle)) {
    trackColl = trackCollectionHandle.product();
  } else {
    cout << "trackCollection cannot be found -> using empty collection of same type." <<endl;
  }

  //=======================================================
  // Retrieve the BeamSpot information
  //======================================================= 
  
  if(recoBeamSpotHandle.isValid()) {
    bsX0_ = bs.x0();
    bsY0_ = bs.y0();
    bsZ0_ = bs.z0();
    bsSigmaZ_ = bs.sigmaZ();
    bsDxdz_ = bs.dxdz();
    bsDydz_ = bs.dydz();

    if(debug_)
      cout<<"beamSpot: (x,y,z) = ("<< bsX0_<<", "<<bsY0_<<", "<<bsZ0_
	  <<"); sigmaZ = "<<bsSigmaZ_<<"; dxdz = "<<bsDxdz_<<"; dydz = "<<bsDydz_<<endl;
    
  }

  //=======================================================
  // Retrieve the PrimaryVertex information
  //======================================================= 

  if(debug_)
    cout<<"LhcTrackAnalyzer::analyze() looping over "<< vertexCollectionHandle->size()<< "primaryVertices." << endl;    
  nVertices_ = 0; 
  for(reco::VertexCollection::const_iterator v=vertexColl->begin(); 
      v!=vertexColl->end(); ++v, ++nVertices_) {
    if(!v->isFake() && v->isValid()) {
      nTracks_pvtx_[nVertices_] = v->tracksSize();	
      sumptsq_pvtx_[nVertices_] = sumPtSquared(*v); 
      isValid_pvtx_[nVertices_] = v->isValid();
      isFake_pvtx_[nVertices_] = v->isFake();	
      recx_pvtx_[nVertices_] = v->x();
      recy_pvtx_[nVertices_] = v->y();
      recz_pvtx_[nVertices_] = v->z();
      recx_err_pvtx_[nVertices_] = v->xError();
      recy_err_pvtx_[nVertices_] = v->yError();
      recz_err_pvtx_[nVertices_] = v->zError();
    }
  }

  //=======================================================
  // Retrieve the Track information
  //=======================================================
  if(debug_)
    cout<<"LhcTrackAnalyzer::analyze() looping over "<< trackCollectionHandle->size()<< "tracks." << endl;       
  // Loop track by reco::Track
  //for(unsigned int i = 0, TrackCollection::const_iterator track = trackCollectionHandle->begin(); track!= trackCollectionHandle->end(); ++track, ++i)
  // Loop track reco::TrackRef
  nTracks_ = 0;
  for (unsigned int i=0; i<trackCollectionHandle->size(); i++, nTracks_++) {
    TrackRef tkref(trackCollectionHandle,i); 
    if ( nTracks_ >= nMaxtracks_ ) {
      std::cout << " LhcTrackAnalyzer::analyze() : Warning - Number of tracks: " 
		<< nTracks_ << " , greater than " << nMaxtracks_ << std::endl;
      continue;
      }
    
    pt_[nTracks_]       = tkref->pt();
    ptErr_[nTracks_]    = tkref->ptError();
    eta_[nTracks_]      = tkref->eta();
    etaErr_[nTracks_]   = tkref->etaError();
    phi_[nTracks_]      = tkref->phi();
    phiErr_[nTracks_]   = tkref->phiError();
    dz_[nTracks_]       = tkref->dz();
    dzErr_[nTracks_]       = tkref->dzError();
    dxy_[nTracks_]      = tkref->dxy();
    dxyErr_[nTracks_]      = tkref->dxyError();

    
    dzCorr_[nTracks_]       = tkref->dz(beamSpot);
    dxyCorr_[nTracks_]      = tkref->dxy(beamSpot);
            
    chi2_[nTracks_]     = tkref->chi2();
    chi2ndof_[nTracks_] = tkref->normalizedChi2();
    charge_[nTracks_]   = tkref->charge();
    qoverp_[nTracks_]   = tkref->qoverp();

    algo_[nTracks_]    = tkref->algo();
    qualityMask_[nTracks_] = tkref->qualityMask();
    

    xPCA_[nTracks_]     = tkref->vertex().x();
    yPCA_[nTracks_]     = tkref->vertex().y();
    zPCA_[nTracks_]     = tkref->vertex().z(); 
    

    if(vertexColl->size()>0 && !vertexColl->begin()->isFake()) {                                                                       
      if(debug_)
	cout<<"TrackWeight in PrimaryVertexColl = "<< vertexColl->begin()->trackWeight(tkref)<<endl; 
      trkWeightpvtx_[nTracks_] =  vertexColl->begin()->trackWeight(tkref);
    }
    
    // TrackQuality
    bool isloose = tkref->quality(reco::Track::loose); 
    bool istight = tkref->quality(reco::Track::tight);
    bool ishighpurity = tkref->quality(reco::Track::highPurity); 
    bool isconfirmed = tkref->quality(reco::Track::confirmed);
    bool isgoodIterative = tkref->quality(reco::Track::goodIterative);
    
    std::string qualityName; 
    if(isloose) qualityName = tkref->qualityName(reco::Track::loose);
    if(istight) qualityName = tkref->qualityName(reco::Track::tight);
    if(ishighpurity) qualityName = tkref->qualityName(reco::Track::highPurity);
    
    isLoose_[nTracks_] = int ( (tkref->qualityMask() & 1 ) == 1);
    isTight_[nTracks_] = int ( (tkref->qualityMask() & 2 ) == 2);
    isHighPurity_[nTracks_] = int ( (tkref->qualityMask() & 4 ) == 4);
    
    if(debug_) {
      cout<< "Track "<<i<<" : pT = "<<tkref->pt()<<endl;
      cout<<"tkref->qualityMask() = "<< tkref->qualityMask()<<endl;
      cout<<"qualityName = "<<qualityName<<"; isloose = "<<isloose
	  <<"; istight = "<<istight
	  <<"; ishighpurity = "<<ishighpurity
	    <<"; isconfirmed = "<<isconfirmed
	  <<"; isgoodIterative = "<<isgoodIterative
	  <<endl;
      cout<<"tkref->algo() = "<<tkref->algo()<<"; tkref->algoName(tkref->algo()) = "
	  << tkref->algoName(tkref->algo()) <<endl;
    }
    // End of TrackQuality
  }
  
 
  rootTree_->Fill();
} 


// ------------ method called once each job before begining the event loop  ------------
void LhcTrackAnalyzer::beginJob(const edm::EventSetup&)
{
  edm::LogInfo("beginJob") << "Begin Job" << std::endl;
  // Define TTree for output
  rootFile_ = new TFile(filename_.c_str(),"recreate");
  rootTree_ = new TTree("tree","Lhc Track tree");
  
  // BeamSpot
  rootTree_->Branch("bsX0",&bsX0_,"bsX0/D");
  rootTree_->Branch("bsY0",&bsY0_,"bsY0/D");
  rootTree_->Branch("bsZ0",&bsZ0_,"bsZ0/D");
  rootTree_->Branch("bsSigmaZ",&bsSigmaZ_,"bsSigmaZ/D");
  rootTree_->Branch("bsDxdz",&bsDxdz_,"bsDxdz/D");
  rootTree_->Branch("bsDydz",&bsDydz_,"bsDydz/D");

  // PrimaryVertex
  rootTree_->Branch("nVertices",&nVertices_,"nVertices/I");  
  rootTree_->Branch("nTracks_pvtx",&nTracks_pvtx_,"nTracks_pvtx[nVertices]/I"); 
  rootTree_->Branch("isValid_pvtx",&isValid_pvtx_,"isValid_pvtx[nVertices]/I"); 
  rootTree_->Branch("isFake_pvtx",&isFake_pvtx_,"isFake_pvtx[nVertices]/I"); 
  rootTree_->Branch("recx_pvtx",&recx_pvtx_,"recx_pvtx[nVertices]/D"); 
  rootTree_->Branch("recy_pvtx",&recy_pvtx_,"recy_pvtx[nVertices]/D"); 
  rootTree_->Branch("recz_pvtx",&recz_pvtx_,"recz_pvtx[nVertices]/D");
  rootTree_->Branch("recx_err_pvtx",&recx_err_pvtx_,"recx_err_pvtx[nVertices]/D"); 
  rootTree_->Branch("recy_err_pvtx",&recy_err_pvtx_,"recy_err_pvtx[nVertices]/D"); 
  rootTree_->Branch("recz_err_pvtx",&recz_err_pvtx_,"recz_err_pvtx[nVertices]/D");

  // Track
  rootTree_->Branch("nTracks",&nTracks_,"nTracks/I");
  rootTree_->Branch("pt",&pt_,"pt[nTracks]/D");
  rootTree_->Branch("ptErr",&ptErr_,"ptErr[nTracks]/D");
  rootTree_->Branch("eta",&eta_,"eta[nTracks]/D");
  rootTree_->Branch("etaErr",&etaErr_,"etaErr[nTracks]/D");
  rootTree_->Branch("phi",&phi_,"phi[nTracks]/D");
  rootTree_->Branch("phiErr",&phiErr_,"phiErr[nTracks]/D");
  rootTree_->Branch("dz",&dz_,"dz[nTracks]/D");
  rootTree_->Branch("dzErr",&dzErr_,"dzErr[nTracks]/D");
  rootTree_->Branch("dxy",&dxy_,"dxy[nTracks]/D");
  rootTree_->Branch("dxyErr",&dxyErr_,"dxyErr[nTracks]/D");
  rootTree_->Branch("dzCorr",&dzCorr_,"dzCorr[nTracks]/D");
  rootTree_->Branch("dxyErr",&dxyErr_,"dxyErr[nTracks]/D");
  rootTree_->Branch("chi2",&chi2_,"chi2[nTracks]/D");
  rootTree_->Branch("chi2ndof",&chi2ndof_,"chi2ndof[nTracks]/D");
  rootTree_->Branch("charge",&charge_,"charge[nTracks]/I");
  rootTree_->Branch("qoverp",&qoverp_,"qoverp[nTracks]/D");
  rootTree_->Branch("algo",&algo_,"algo[nTracks]/I");
  rootTree_->Branch("qualityMask",&qualityMask_,"qualityMask[nTracks]/I");
  rootTree_->Branch("isLoose",&isLoose_,"isLoose[nTracks]/I");
  rootTree_->Branch("isTight",&isTight_,"isTight[nTracks]/I");
  rootTree_->Branch("isHighPurity",&isHighPurity_,"isHighPurity[nTracks]/I");
  rootTree_->Branch("xPCA",&xPCA_,"xPCA[nTracks]/D");
  rootTree_->Branch("yPCA",&yPCA_,"yPCA[nTracks]/D");
  rootTree_->Branch("zPCA",&zPCA_,"zPCA[nTracks]/D");
  rootTree_->Branch("trkWeightpvtx",&trkWeightpvtx_,"trkWeightpvtx[nTracks]/D");


 
  
}

// ------------ method called once each job just after ending the event loop  ------------
void LhcTrackAnalyzer::endJob() 
{
   if ( rootFile_ ) {
     rootFile_->Write();
     rootFile_->Close();
   }



}

void LhcTrackAnalyzer::SetRootVar() {
  
  //BeamSpot
  bsX0_ = 0;
  bsY0_ = 0;
  bsZ0_ = 0;
  bsDxdz_ = 0;
  bsDydz_ = 0;

  //Track
  nTracks_ = 0;
  for ( int i=0; i<nMaxtracks_; ++i ) {
    pt_[i]             = -999;
    eta_[i]            = -999;
    phi_[i]            = -999;
    dz_[i]             = -999;
    dxy_[i]            = -999;
    dzCorr_[i]         = -999;
    dxyCorr_[i]        = -999;
    ptErr_[i]          = -999;
    etaErr_[i]         = -999;
    phiErr_[i]         = -999;
    dzErr_[i]          = -999;
    dxyErr_[i]         = -999;
    chi2_[i]           = -999;
    chi2ndof_[i]       = -999;
    charge_[i]         = -999;
    qoverp_[i]         = -999;
    algo_[i]           = -999;
    qualityMask_[i]     = -999;
    isHighPurity_[i]    = 0;
    isTight_[i]         = 0;
    isLoose_[i]         = 0;
    xPCA_[i]        = -999;
    yPCA_[i]        = -999;
    zPCA_[i]        = -999;
    trkWeightpvtx_[i] = -999;
  }

  //PrimaryVertex
  nVertices_ = 0;
  for ( int i=0; i<nMaxPVs_; ++i ) {
    nTracks_pvtx_[nMaxPVs_] = -999; // Number of tracks in the pvtx  
    sumptsq_pvtx_[nMaxPVs_] = -999;
    isValid_pvtx_[nMaxPVs_] = -999;
    isFake_pvtx_[nMaxPVs_] = -999;
    recx_pvtx_[nMaxPVs_] = -999;
    recy_pvtx_[nMaxPVs_] = -999;
    recz_pvtx_[nMaxPVs_] = -999;
    recx_err_pvtx_[nMaxPVs_] = -999;
    recy_err_pvtx_[nMaxPVs_] = -999;
    recz_err_pvtx_[nMaxPVs_] = -999;
  }
}

double LhcTrackAnalyzer::sumPtSquared(const reco::Vertex & v)  {
  double sum = 0.;
  double pT;
  for (reco::Vertex::trackRef_iterator it = v.tracks_begin(); it != v.tracks_end(); it++) {
    pT = (**it).pt();
    sum += pT*pT;
  }
  return sum;
}



//define this as a plug-in
//DEFINE_FWK_MODULE(LhcTrackAnalyzer);
