// -*- C++ -*-
//
// Package:    LhcTrackAnalyzer
// Class:      LhcTrackAnalyzer
// 
/**\class LhcTrackAnalyzer LhcTrackAnalyzer.cc MySub/LshcTrackAnalyzer/src/LhcTrackAnalyzer.cc

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
#include <SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h>
#include <SimDataFormats/TrackingHit/interface/PSimHit.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
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
// RecHit
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"

// Vertex Stuff
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"  
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

// BeamSpot 
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

// Trigger
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"

#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsTechTrigRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMaskAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskTechTrigRcd.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMaskVetoAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskVetoTechTrigRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

typedef math::XYZPoint Point;

using namespace std;
using namespace reco;
using namespace edm;



// Constructor

LhcTrackAnalyzer::LhcTrackAnalyzer(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  debug_    = iConfig.getParameter<bool>       ("Debug");  
  ctfTrackCollectionTag_      = iConfig.getParameter<edm::InputTag>("ctfTrackCollectionTag");
  secTrackCollectionTag_      = iConfig.getParameter<edm::InputTag>("secTrackCollectionTag");
  vertexCollectionTag_       = iConfig.getUntrackedParameter<edm::InputTag>("vertexCollection");
  bsSrc = iConfig.getParameter< edm::InputTag >("beamSpot");
  filename_ = iConfig.getParameter<std::string>("OutputFileName");
  afterRefitting_           = iConfig.getParameter<bool> ("afterRefitting");  
  runSecTrackColl_          = iConfig.getParameter<bool> ("runSecTrackColl");
  saveAllClusters_          = iConfig.getParameter<bool> ("saveAllClusters");
  selTechBit_              = iConfig.getParameter<bool> ("selTechBit");
  techBitToSelect_         = iConfig.getParameter<int>("techBitToSelect");
  selNonFakePvtx_          = iConfig.getParameter<bool>("selNonFakePvtx");
  pixelVertexCollectionTag_      = iConfig.getParameter<edm::InputTag>("pixelVertexCollectionTag"); 
}
   
// Destructor
LhcTrackAnalyzer::~LhcTrackAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// Member functions
//

// ------------ method called to for each event  ------------
void
LhcTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //=======================================================
  // Initialize Root-tuple variables
  //=======================================================

  SetRootVar();
  
  glob_runno_ = iEvent.id().run();
  glob_evtno_ = iEvent.id().event();
  glob_ls_   = iEvent.luminosityBlock();
  glob_bx_  = iEvent.bunchCrossing();

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
    cout << "vertexCollection cannot be found -> using empty collection of same type. Run/Event: " << glob_runno_ << "/" << glob_evtno_ << endl;
  } 
       
  //=======================================================
  // GET pixelVertices
  //=======================================================
  static const reco::VertexCollection s_empty_pixelVertexColl;
  const reco::VertexCollection *pixelVertexColl = &s_empty_pixelVertexColl;
  edm::Handle<reco::VertexCollection>  pixelVertexCollectionHandle;
  iEvent.getByLabel(pixelVertexCollectionTag_, pixelVertexCollectionHandle);
  if( iEvent.getByLabel(pixelVertexCollectionTag_, pixelVertexCollectionHandle)) {
    pixelVertexColl = pixelVertexCollectionHandle.product();
  } else {
    cout << "pixelVertexCollection cannot be found. -> using empty collection of same type." <<endl;
  }
  
  //=======================================================
  // Tracker Geometry
  //=======================================================
  edm::ESHandle<TrackerGeometry> tracker;
  iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
  //const TrackerGeometry &tracker_geometry(*tracker);
    
  //=======================================================
  // CTF Track accessors
  //=======================================================
  static const reco::TrackCollection s_empty_ctfTrackColl;
  const reco::TrackCollection *ctfTrackColl = &s_empty_ctfTrackColl;
  edm::Handle<reco::TrackCollection> ctfTrackCollectionHandle;
  iEvent.getByLabel(ctfTrackCollectionTag_, ctfTrackCollectionHandle);
  if( iEvent.getByLabel(ctfTrackCollectionTag_, ctfTrackCollectionHandle)) {
    ctfTrackColl = ctfTrackCollectionHandle.product();
  } else {
    cout << "ctfTrackCollection cannot be found -> using empty collection of same type.Run/Event: " << glob_runno_ << "/" << glob_evtno_ << endl;
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
  // Define the pvtx point as the leading pvtx position if the vertex collection is full, otherwise as default BS
  const Point pvtx = vertexCollectionHandle.isValid() ? Point(vertexColl->begin()->x(),vertexColl->begin()->y(), vertexColl->begin()->z()):beamSpot;

  bool nonFakePvtx = true;
  if(selNonFakePvtx_) nonFakePvtx = false;
  nVertices_ = 0; 
  for(reco::VertexCollection::const_iterator v=vertexColl->begin(); 
      v!=vertexColl->end(); ++v, ++nVertices_) {
    nTracks_pvtx_[nVertices_] = v->tracksSize();
    ndof_pvtx_[nVertices_] = v->ndof();	
    sumptsq_pvtx_[nVertices_] = sumPtSquared(*v);
    isValid_pvtx_[nVertices_] = int(v->isValid());
    isFake_pvtx_[nVertices_] =  int(v->isFake());
    if(!v->isFake()) hasGoodPvtx_ = 1; 
    if(selNonFakePvtx_ && !v->isFake()) nonFakePvtx = true;
    recx_pvtx_[nVertices_] = v->x();
    recy_pvtx_[nVertices_] = v->y();
    recz_pvtx_[nVertices_] = v->z();
    recx_err_pvtx_[nVertices_] = v->xError();
    recy_err_pvtx_[nVertices_] = v->yError();
    recz_err_pvtx_[nVertices_] = v->zError();
  }
  

  //=======================================================
  // Retrieve the PrimaryVertex information
  //======================================================= 

  if(debug_)
    cout<<"LhcTrackAnalyzer::analyze() looping over "<< pixelVertexCollectionHandle->size()<< "pixelVertices." << endl;    
  
  nPixelVertices_ = 0; 
  for(reco::VertexCollection::const_iterator v=pixelVertexColl->begin(); 
      v!=pixelVertexColl->end(); ++v, ++nPixelVertices_) {
    nTracks_pxlpvtx_[nPixelVertices_] = v->tracksSize();
    isFake_pxlpvtx_[nPixelVertices_] =  int(v->isFake());
    if(!v->isFake()) hasGoodPxlPvtx_ = 1;
    recz_pxlpvtx_[nPixelVertices_] = v->z();
    recz_err_pxlpvtx_[nPixelVertices_] = v->zError();
  }
  
  
  //=======================================================
  // Retrieve the Track information: CTF
  //=======================================================
  
  if(debug_)
    cout<<"LhcTrackAnalyzer::analyze() looping over "<< ctfTrackCollectionHandle->size()<< "ctfTracks." << endl;       
  // Loop track by reco::Track
  //for(unsigned int i = 0, TrackCollection::const_iterator track = ctfTrackCollectionHandle->begin(); track!= ctfTrackCollectionHandle->end(); ++track, ++i)
  // Loop track reco::TrackRef
  ctf_n_ = 0;
  ctf_nHighPurity_ = 0;
  for (unsigned int i=0; i<ctfTrackCollectionHandle->size(); i++) {
    TrackRef tkref(ctfTrackCollectionHandle,i); 
    if ( ctf_n_ >= nMaxCTFtracks_ ) {
      std::cout << " LhcTrackAnalyzer::analyze() : Warning - Number of ctfTracks: " 
		<< ctf_n_ << " , greater than " << nMaxCTFtracks_ << std::endl;
      continue;
      }

    // Calculate dzSignificance
    float pvtx_zErr = (vertexCollectionHandle.isValid()&&(!vertexColl->begin()->isFake())) ? vertexColl->begin()->zError():bsSigmaZ_;
    float dzpvtxsign = tkref->dz(pvtx)/sqrt(tkref->dzError()*tkref->dzError()+pvtx_zErr*pvtx_zErr); 

    if(tkref->quality(reco::TrackBase::highPurity) && dzpvtxsign < 10 && tkref->ptError()/tkref->pt()<0.1 ) {
    ctf_pt_[ctf_n_]       = tkref->pt();
    ctf_ptErr_[ctf_n_]    = tkref->ptError();
    ctf_eta_[ctf_n_]      = tkref->eta();
    ctf_etaErr_[ctf_n_]   = tkref->etaError();
    ctf_phi_[ctf_n_]      = tkref->phi();
    ctf_phiErr_[ctf_n_]   = tkref->phiError();
    ctf_dz_[ctf_n_]       = tkref->dz();
    ctf_dzErr_[ctf_n_]       = tkref->dzError();
    ctf_dxy_[ctf_n_]      = tkref->dxy();
    ctf_dxyErr_[ctf_n_]      = tkref->dxyError();
    ctf_dzCorr_[ctf_n_]       = tkref->dz(beamSpot);
    ctf_dxyCorr_[ctf_n_]      = tkref->dxy(beamSpot);
    ctf_dxyCorr_pvtx_[ctf_n_] = tkref->dxy(pvtx);
    ctf_dzCorr_pvtx_[ctf_n_]       = tkref->dz(pvtx);
    ctf_dzCorrErr_pvtx_[ctf_n_]       = sqrt(tkref->dzError()*tkref->dzError()+pvtx_zErr*pvtx_zErr);
    ctf_chi2_[ctf_n_]     = tkref->chi2();
    ctf_chi2ndof_[ctf_n_] = tkref->normalizedChi2();
    ctf_charge_[ctf_n_]   = tkref->charge();
    ctf_qoverp_[ctf_n_]   = tkref->qoverp();
    ctf_algo_[ctf_n_]    = tkref->algo();
    ctf_qualityMask_[ctf_n_] = tkref->qualityMask();
    ctf_xPCA_[ctf_n_]     = tkref->vertex().x();
    ctf_yPCA_[ctf_n_]     = tkref->vertex().y();
    ctf_zPCA_[ctf_n_]     = tkref->vertex().z(); 
    
    ctf_nLayers_[ctf_n_]   = int(tkref->hitPattern().trackerLayersWithMeasurement());
    ctf_nPXBLayers_[ctf_n_] = int(tkref->hitPattern().pixelBarrelLayersWithMeasurement());
    ctf_nPXFLayers_[ctf_n_] = int(tkref->hitPattern().pixelEndcapLayersWithMeasurement());
    ctf_nLostHit_[ctf_n_]   = int(tkref->hitPattern().numberOfLostHits());
    ctf_nLayers3D_[ctf_n_] = int(tkref->hitPattern().pixelLayersWithMeasurement() +
				 tkref->hitPattern().numberOfValidStripLayersWithMonoAndStereo());
    
    
    
    // Loop over all vertexs and fill the trackWeight
    int ivtx = 0; 
    for(reco::VertexCollection::const_iterator v=vertexColl->begin();
	v!=vertexColl->end(); ++v, ++ivtx) {
      // if the given track is used in any pvtx, store the track weight and pvtx no
      if(v->trackWeight(tkref)!=0) {
	ctf_trkWeightpvtx_[ctf_n_] =  v->trackWeight(tkref);
	ctf_pvtx_no_[ctf_n_] = ivtx; 
	if(debug_)
	  cout<<"TrackWeight in PrimaryVertex # "<<ivtx<<": "<< v->trackWeight(tkref)<<endl; 
      }
    }
    
    // TrackQuality
    //bool isloose = tkref->quality(reco::Track::loose); 
    //bool istight = tkref->quality(reco::Track::tight);
    //bool ishighpurity = tkref->quality(reco::Track::highPurity); 
    //bool isconfirmed = tkref->quality(reco::Track::confirmed);
    //bool isgoodIterative = tkref->quality(reco::Track::goodIterative);
      
    //std::string qualityName; 
    //if(isloose) qualityName = tkref->qualityName(reco::Track::loose);
    //if(istight) qualityName = tkref->qualityName(reco::Track::tight);
    //if(ishighpurity) qualityName = tkref->qualityName(reco::Track::highPurity);
    
    ctf_isLoose_[ctf_n_] = int ( (tkref->qualityMask() & 1 ) == 1);
    ctf_isTight_[ctf_n_] = int ( (tkref->qualityMask() & 2 ) == 2);
    ctf_isHighPurity_[ctf_n_] = int ( (tkref->qualityMask() & 4 ) == 4);
    if( (tkref->qualityMask() & 4 ) == 4) ++ctf_nHighPurity_;
    
    
    if(debug_) {
      cout << "ctfTrack "<< i << " : pT = "<< ctf_pt_<<" +/- "<< ctf_ptErr_ 
	   << ";  = "<< ctf_eta_<<" +/- "<< ctf_etaErr_ 
	   << ";  = "<< ctf_phi_<<" +/- "<< ctf_phiErr_ 
	   << ";  = "<< ctf_dxy_<<" +/- "<< ctf_dxyErr_ 
	   << ";  = "<< ctf_dz_<<" +/- "<< ctf_dzErr_ 
	   << endl;
      cout<<"qualityMask() = "<< tkref->qualityMask()<<endl;
      cout<<"algo() = "<<tkref->algo()<<"; tkref->algoName(tkref->algo()) = "
	  << tkref->algoName(tkref->algo()) <<endl;
    }   
    
    //------------------------------
    // Loop over the track recHits :
    //------------------------------
    for ( trackingRecHit_iterator recHit = tkref->recHitsBegin(); recHit != tkref->recHitsEnd(); ++recHit ) {
      if ( !((*recHit)->isValid()) ) continue; 
      
      // PATRICE : pas compris
      //total number of hits belonging to track (matched hits are resolved)
      //if( dynamic_cast<const SiStripRecHit2D*>((*recHit).get()) ) nHit+=1;
      //if( dynamic_cast<const SiStripMatchedRecHit2D*>((*recHit).get()) ) nHit+=2;
      
      ++ctf_nHit_[ctf_n_];
      DetId id((*recHit)->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++ctf_nStripHit_[ctf_n_];
	++ctf_nTIBhit_[ctf_n_];
	TIBDetId useDetId(id.rawId());
	switch ( useDetId.layer() ) {
	case 1: ++ctf_nTIB1hit_[ctf_n_]; break;
	case 2: ++ctf_nTIB2hit_[ctf_n_]; break;
	case 3: ++ctf_nTIB3hit_[ctf_n_]; break;
	case 4: ++ctf_nTIB4hit_[ctf_n_]; break;
	}
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++ctf_nStripHit_[ctf_n_];
	++ctf_nTIDhit_[ctf_n_];
	TIDDetId useDetId(id.rawId());
	switch ( useDetId.wheel() ) {
	case 1: ++ctf_nTID1hit_[ctf_n_]; break;
	case 2: ++ctf_nTID2hit_[ctf_n_]; break;
	case 3: ++ctf_nTID3hit_[ctf_n_]; break;
	}
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++ctf_nStripHit_[ctf_n_];
	++ctf_nTOBhit_[ctf_n_];
	TOBDetId useDetId(id.rawId());
	switch ( useDetId.layer() ) {
	case 1: ++ctf_nTOB1hit_[ctf_n_]; break;
	case 2: ++ctf_nTOB2hit_[ctf_n_]; break;
	case 3: ++ctf_nTOB3hit_[ctf_n_]; break;
	case 4: ++ctf_nTOB4hit_[ctf_n_]; break;
	case 5: ++ctf_nTOB5hit_[ctf_n_]; break;
	case 6: ++ctf_nTOB6hit_[ctf_n_]; break;
	}
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++ctf_nStripHit_[ctf_n_];
	++ctf_nTEChit_[ctf_n_];
	TECDetId useDetId(id.rawId());
	switch ( useDetId.wheel() ) {
	case 1: ++ctf_nTEC1hit_[ctf_n_]; break;
	case 2: ++ctf_nTEC2hit_[ctf_n_]; break;
	case 3: ++ctf_nTEC3hit_[ctf_n_]; break;
	case 4: ++ctf_nTEC4hit_[ctf_n_]; break;
	case 5: ++ctf_nTEC5hit_[ctf_n_]; break;
	case 6: ++ctf_nTEC6hit_[ctf_n_]; break;
	case 7: ++ctf_nTEC7hit_[ctf_n_]; break;
	case 8: ++ctf_nTEC8hit_[ctf_n_]; break;
	case 9: ++ctf_nTEC9hit_[ctf_n_]; break;
	}
      }  else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++ctf_nPixelHit_[ctf_n_];
	++ctf_nPXBhit_[ctf_n_];
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++ctf_nPixelHit_[ctf_n_];
	++ctf_nPXFhit_[ctf_n_];
      } // endif over sub-dets
      
	// Loop over the track SiStripRecHit2D for clusterCharge :
      
      std::vector<SiStripRecHit2D*> output = getRecHitComponents((*recHit).get());
      for(std::vector<SiStripRecHit2D*>::const_iterator rhit = output.begin(); rhit!=output.end(); ++ rhit) {
	
	uint16_t clusterCharge = ClusterCharge(*rhit);
	ctf_clusterCharge_all_ += clusterCharge;
	
	  unsigned int clusterType  = 0;
	  unsigned int clusterLayer = 0;
	  
	  DetId id_tmp((*rhit)->geographicalId());
	  
	  if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TIB ) {
	    clusterType = 1;
	    ctf_clusterCharge_TIB_ += clusterCharge;
	    TIBDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.layer();
	  }
	  else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TID ) {
	    clusterType = 2;
	    ctf_clusterCharge_TID_ += clusterCharge;
	    TIDDetId useDetId(id_tmp.rawId());
	   clusterLayer = useDetId.wheel();
	  }
	  else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TOB ) {
	    clusterType = 3;
	    ctf_clusterCharge_TOB_ += clusterCharge;
	    TOBDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.layer();
	  }
	  else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TEC ) {
	    clusterType = 4;
	    ctf_clusterCharge_TEC_ += clusterCharge;
	    TECDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.wheel();
	  }
	  
	 if ( ctfcluster_n_ < nMaxCTFclusters_ ) {
	   ctfcluster_type_[ctfcluster_n_]   = clusterType;
	   ctfcluster_layer_[ctfcluster_n_]  = clusterLayer;
	   ctfcluster_charge_[ctfcluster_n_] = clusterCharge;
	   // Cluster position available only after track refitting
	   if ( afterRefitting_ ) {	
	     edm::ESHandle<TransientTrackingRecHitBuilder> theTrackerRecHitBuilder;
	     iSetup.get<TransientRecHitRecord>().get("WithTrackAngle",theTrackerRecHitBuilder); 
	     const TransientTrackingRecHitBuilder* theTTRHBuilder = theTrackerRecHitBuilder.product();
	     TransientTrackingRecHit::RecHitPointer tthit = theTTRHBuilder->build((*recHit).get());
	     GlobalPoint recHitGlobalPosition = tthit->globalPosition();
	     ctfcluster_x_[ctfcluster_n_] = recHitGlobalPosition.x();
	     ctfcluster_y_[ctfcluster_n_] = recHitGlobalPosition.y();
	     ctfcluster_z_[ctfcluster_n_] = recHitGlobalPosition.z();
	   }
	   ++ctfcluster_n_;
	 } else {
	   std::cout << " TrackTupleMaker::analyze() : Warning #cluster for CTF tracks: " << ctfcluster_n_ << " , greater than " << nMaxCTFclusters_ << std::endl;
	 }
	} // end loop over track SiStripRecHit2D
      } // end loop over ctf RecHits
      ++ctf_n_;
    }
  } // End of Loop over the ctf tracks
  
  if(ctf_n_>0) {
    ctf_fHighPurity_ =  double(ctf_nHighPurity_)/double(ctf_n_);
    //cout<<"ctf_n ="<<ctf_n_<<"; ctf_nHighPurity = "<<ctf_nHighPurity_<<"; ctf_fHighPurity = "<<ctf_fHighPurity_<<endl;
  }
  
  //=======================================================
  // If run on secTrackCollection
  //=======================================================
  
  if( runSecTrackColl_ ) {
    
    //=======================================================
    // Track accessors
    //=======================================================
    static const reco::TrackCollection s_empty_secTrackColl;
    const reco::TrackCollection *secTrackColl = &s_empty_secTrackColl;
    edm::Handle<reco::TrackCollection> secTrackCollectionHandle;
    iEvent.getByLabel(secTrackCollectionTag_, secTrackCollectionHandle);
    if( iEvent.getByLabel(secTrackCollectionTag_, secTrackCollectionHandle)) {
      secTrackColl = secTrackCollectionHandle.product();
    } else {
      cout << "secTrackCollection cannot be found -> using empty collection of same type.Run/Event: " << glob_runno_ << "/" << glob_evtno_ << endl;
    }
    
    //=======================================================
    // Retrieve the Track information: secTrackCollection
    //=======================================================
    
    if(debug_)
      cout<<"LhcTrackAnalyzer::analyze() looping over "<< secTrackCollectionHandle->size()<< "secTrackCollection Tracks." << endl;       
    // Loop track by reco::Track
    //for(unsigned int i = 0, TrackCollection::const_iterator track = secTrackCollectionHandle->begin(); track!= secTrackCollectionHandle->end(); ++track, ++i)
    // Loop track reco::TrackRef
    sectrk_n_ = 0;
    for (unsigned int i=0; i<secTrackCollectionHandle->size(); i++, sectrk_n_++) {
      TrackRef tkref(secTrackCollectionHandle,i); 
      if ( sectrk_n_ >= nMaxSECTRKtracks_ ) {
	std::cout << " LhcTrackAnalyzer::analyze() : Warning - Number of secTracks: " 
		  << sectrk_n_ << " , greater than " << nMaxSECTRKtracks_ << std::endl;
	continue;
      }
      
      sectrk_pt_[sectrk_n_]       = tkref->pt();
      sectrk_ptErr_[sectrk_n_]    = tkref->ptError();
      sectrk_eta_[sectrk_n_]      = tkref->eta();
      sectrk_etaErr_[sectrk_n_]   = tkref->etaError();
      sectrk_phi_[sectrk_n_]      = tkref->phi();
      sectrk_phiErr_[sectrk_n_]   = tkref->phiError();
      sectrk_dz_[sectrk_n_]       = tkref->dz();
      sectrk_dzErr_[sectrk_n_]       = tkref->dzError();
      sectrk_dxy_[sectrk_n_]      = tkref->dxy();
      sectrk_dxyErr_[sectrk_n_]      = tkref->dxyError();
      sectrk_dzCorr_[sectrk_n_]       = tkref->dz(beamSpot);
      sectrk_dxyCorr_[sectrk_n_]      = tkref->dxy(beamSpot);
      sectrk_dxyCorr_pvtx_[sectrk_n_]      = tkref->dxy(pvtx);
      sectrk_dzCorr_pvtx_[sectrk_n_]       = tkref->dz(pvtx);
      sectrk_chi2_[sectrk_n_]     = tkref->chi2();
      sectrk_chi2ndof_[sectrk_n_] = tkref->normalizedChi2();
      sectrk_charge_[sectrk_n_]   = tkref->charge();
      sectrk_qoverp_[sectrk_n_]   = tkref->qoverp();
      sectrk_algo_[sectrk_n_]    = tkref->algo();
      sectrk_qualityMask_[sectrk_n_] = tkref->qualityMask();
      sectrk_xPCA_[sectrk_n_]     = tkref->vertex().x();
      sectrk_yPCA_[sectrk_n_]     = tkref->vertex().y();
      sectrk_zPCA_[sectrk_n_]     = tkref->vertex().z(); 
     
      
      sectrk_nLayers_[sectrk_n_]   = int(tkref->hitPattern().trackerLayersWithMeasurement());
      sectrk_nPXBLayers_[sectrk_n_] = int(tkref->hitPattern().pixelBarrelLayersWithMeasurement());
      sectrk_nPXFLayers_[sectrk_n_] = int(tkref->hitPattern().pixelEndcapLayersWithMeasurement());
      sectrk_nLostHit_[sectrk_n_]   = int(tkref->hitPattern().numberOfLostHits());
      sectrk_nLayers3D_[sectrk_n_] = int(tkref->hitPattern().pixelLayersWithMeasurement() +
				  tkref->hitPattern().numberOfValidStripLayersWithMonoAndStereo());

      // TrackQuality
      //bool isloose = tkref->quality(reco::Track::loose); 
      //bool istight = tkref->quality(reco::Track::tight);
      //bool ishighpurity = tkref->quality(reco::Track::highPurity); 
      //bool isconfirmed = tkref->quality(reco::Track::confirmed);
      //bool isgoodIterative = tkref->quality(reco::Track::goodIterative);
      
      //std::string qualityName; 
      //if(isloose) qualityName = tkref->qualityName(reco::Track::loose);
      //if(istight) qualityName = tkref->qualityName(reco::Track::tight);
      //if(ishighpurity) qualityName = tkref->qualityName(reco::Track::highPurity);
      
      sectrk_isLoose_[sectrk_n_] = int ( (tkref->qualityMask() & 1 ) == 1);
      sectrk_isTight_[sectrk_n_] = int ( (tkref->qualityMask() & 2 ) == 2);
      sectrk_isHighPurity_[sectrk_n_] = int ( (tkref->qualityMask() & 4 ) == 4);
      
      
      if(debug_) {
	cout << "secTrack "<< i << " : pT = "<< sectrk_pt_<<" +/- "<< sectrk_ptErr_ 
	     << ";  = "<< sectrk_eta_<<" +/- "<< sectrk_etaErr_ 
	     << ";  = "<< sectrk_phi_<<" +/- "<< sectrk_phiErr_ 
	     << ";  = "<< sectrk_dxy_<<" +/- "<< sectrk_dxyErr_ 
	     << ";  = "<< sectrk_dz_<<" +/- "<< sectrk_dzErr_ 
	     << endl;
	cout<<"qualityMask() = "<< tkref->qualityMask()<<endl;
	cout<<"algo() = "<<tkref->algo()<<"; tkref->algoName(tkref->algo()) = "
	    << tkref->algoName(tkref->algo()) <<endl;
      }   
      
      //------------------------------
      // Loop over the track recHits :
      //------------------------------
      for ( trackingRecHit_iterator recHit = tkref->recHitsBegin(); recHit != tkref->recHitsEnd(); ++recHit ) {
	if ( !((*recHit)->isValid()) ) continue; 
	
	// PATRICE : pas compris
	//total number of hits belonging to track (matched hits are resolved)
	//if( dynamic_cast<const SiStripRecHit2D*>((*recHit).get()) ) nHit+=1;
	//if( dynamic_cast<const SiStripMatchedRecHit2D*>((*recHit).get()) ) nHit+=2;
	
	++sectrk_nHit_[sectrk_n_];
	DetId id((*recHit)->geographicalId());
	
	if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	  ++sectrk_nStripHit_[sectrk_n_];
	  ++sectrk_nTIBhit_[sectrk_n_];
	  TIBDetId useDetId(id.rawId());
	  switch ( useDetId.layer() ) {
	  case 1: ++sectrk_nTIB1hit_[sectrk_n_]; break;
	  case 2: ++sectrk_nTIB2hit_[sectrk_n_]; break;
	  case 3: ++sectrk_nTIB3hit_[sectrk_n_]; break;
	  case 4: ++sectrk_nTIB4hit_[sectrk_n_]; break;
	  }
	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	  ++sectrk_nStripHit_[sectrk_n_];
	  ++sectrk_nTIDhit_[sectrk_n_];
	  TIDDetId useDetId(id.rawId());
	  switch ( useDetId.wheel() ) {
	  case 1: ++sectrk_nTID1hit_[sectrk_n_]; break;
	  case 2: ++sectrk_nTID2hit_[sectrk_n_]; break;
	  case 3: ++sectrk_nTID3hit_[sectrk_n_]; break;
	  }
	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	  ++sectrk_nStripHit_[sectrk_n_];
	  ++sectrk_nTOBhit_[sectrk_n_];
	  TOBDetId useDetId(id.rawId());
	  switch ( useDetId.layer() ) {
	  case 1: ++sectrk_nTOB1hit_[sectrk_n_]; break;
	  case 2: ++sectrk_nTOB2hit_[sectrk_n_]; break;
	  case 3: ++sectrk_nTOB3hit_[sectrk_n_]; break;
	  case 4: ++sectrk_nTOB4hit_[sectrk_n_]; break;
	  case 5: ++sectrk_nTOB5hit_[sectrk_n_]; break;
	  case 6: ++sectrk_nTOB6hit_[sectrk_n_]; break;
	  }
	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	  ++sectrk_nStripHit_[sectrk_n_];
	  ++sectrk_nTEChit_[sectrk_n_];
	  TECDetId useDetId(id.rawId());
	  switch ( useDetId.wheel() ) {
	  case 1: ++sectrk_nTEC1hit_[sectrk_n_]; break;
	  case 2: ++sectrk_nTEC2hit_[sectrk_n_]; break;
	  case 3: ++sectrk_nTEC3hit_[sectrk_n_]; break;
	  case 4: ++sectrk_nTEC4hit_[sectrk_n_]; break;
	  case 5: ++sectrk_nTEC5hit_[sectrk_n_]; break;
	  case 6: ++sectrk_nTEC6hit_[sectrk_n_]; break;
	  case 7: ++sectrk_nTEC7hit_[sectrk_n_]; break;
	  case 8: ++sectrk_nTEC8hit_[sectrk_n_]; break;
	  case 9: ++sectrk_nTEC9hit_[sectrk_n_]; break;
	  }
	}  else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	  ++sectrk_nPixelHit_[sectrk_n_];
	  ++sectrk_nPXBhit_[sectrk_n_];
	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	  ++sectrk_nPixelHit_[sectrk_n_];
	  ++sectrk_nPXFhit_[sectrk_n_];
	} // endif over sub-dets
	
	// Loop over the track SiStripRecHit2D for clusterCharge :
	
	std::vector<SiStripRecHit2D*> output = getRecHitComponents((*recHit).get());
	for(std::vector<SiStripRecHit2D*>::const_iterator rhit = output.begin(); rhit!=output.end(); ++ rhit) {
	  
	  uint16_t clusterCharge = ClusterCharge(*rhit);
	  sectrk_clusterCharge_all_ += clusterCharge;
	  
	  unsigned int clusterType  = 0;
	  unsigned int clusterLayer = 0;
	  
	  DetId id_tmp((*rhit)->geographicalId());
	  
	  if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TIB ) {
	    clusterType = 1;
	    sectrk_clusterCharge_TIB_ += clusterCharge;
	    TIBDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.layer();
	  }
	  else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TID ) {
	    clusterType = 2;
	    sectrk_clusterCharge_TID_ += clusterCharge;
	    TIDDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.wheel();
	  }
	  else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TOB ) {
	    clusterType = 3;
	    sectrk_clusterCharge_TOB_ += clusterCharge;
	    TOBDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.layer();
	  }
	  else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TEC ) {
	    clusterType = 4;
	    sectrk_clusterCharge_TEC_ += clusterCharge;
	    TECDetId useDetId(id_tmp.rawId());
	    clusterLayer = useDetId.wheel();
	  }
	  
	  if ( sectrkcluster_n_ < nMaxSECTRKclusters_ ) {
	    sectrkcluster_type_[sectrkcluster_n_]   = clusterType;
	    sectrkcluster_layer_[sectrkcluster_n_]  = clusterLayer;
	    sectrkcluster_charge_[sectrkcluster_n_] = clusterCharge;

	    // Cluster position available only after track refitting
	    if ( afterRefitting_ ) {
	      edm::ESHandle<TransientTrackingRecHitBuilder> theTrackerRecHitBuilder;
	      iSetup.get<TransientRecHitRecord>().get("WithTrackAngle",theTrackerRecHitBuilder); 
	      const TransientTrackingRecHitBuilder* theTTRHBuilder = theTrackerRecHitBuilder.product();
	      TransientTrackingRecHit::RecHitPointer tthit = theTTRHBuilder->build((*recHit).get());
	      GlobalPoint recHitGlobalPosition = tthit->globalPosition();
	      sectrkcluster_x_[sectrkcluster_n_] = recHitGlobalPosition.x();
	      sectrkcluster_y_[sectrkcluster_n_] = recHitGlobalPosition.y();
	      sectrkcluster_z_[sectrkcluster_n_] = recHitGlobalPosition.z();
	    }
	    ++sectrkcluster_n_;
	  } else {
	    std::cout << " TrackTupleMaker::analyze() : Warning #cluster for SECTRK tracks: " << sectrkcluster_n_ << " , greater than " << nMaxSECTRKclusters_ << std::endl;
	  }
	} // end loop over track SiStripRecHit2D
      } // end loop over RecHits
    } // End of Loop over the secTrackColl tracks
  }
  
  //=======================================================
  // All RecHits
  //=======================================================
  
  if ( saveAllClusters_ ) { 
    //=======================================================
    // RecHits accessors: for AllHits
    // WARNING: RECO data for collsion doesn't have "siStripMatchedRecHits"
    //=======================================================
    edm::Handle<SiStripRecHit2DCollection> rphiRecHits;
    iEvent.getByLabel("siStripMatchedRecHits","rphiRecHit",rphiRecHits);
    const SiStripRecHit2DCollection *rphiRecHitCollection = rphiRecHits.product();
    
    edm::Handle<SiStripRecHit2DCollection> stereorecHitHandle;
    iEvent.getByLabel("siStripMatchedRecHits","stereoRecHit",stereorecHitHandle);
    const SiStripRecHit2DCollection *stereoRecHitCollection = stereorecHitHandle.product();
    
    // Loop over Rphi Hits :
    //======================
    
    if ( debug_ ) std::cout <<  " TrackTupleMaker::analyze() : Looping over " << rphiRecHitCollection->size() << " Rphi RecHits" << std::endl;
    
    for(SiStripRecHit2DCollection::DataContainer::const_iterator rhit=rphiRecHitCollection->data().begin(); rhit!=rphiRecHitCollection->data().end(); ++rhit) {
      
      if ( cluster_n_ >= nMaxClusters_ ) {
	std::cout << " TrackTupleMaker::analyze() : Warning #cluster : " << cluster_n_ << " , greater than " << nMaxClusters_ << std::endl;
	continue;
      }
      
      ++cluster_n_all_;
      
      unsigned int clusterType  = 0;
      unsigned int clusterLayer = 0;
      uint16_t clusterCharge = ClusterCharge(&(*rhit));
      
      DetId id_tmp((&(*rhit))->geographicalId());
      if        ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TIB ) {
	clusterType = 1;
	TIBDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.layer();
	++cluster_n_tib_;
      } else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TID ) {
	clusterType = 2;
	TIDDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.wheel();
	++cluster_n_tid_;
      } else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TOB ) {
	clusterType = 3;
	TOBDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.layer();
	++cluster_n_tob_;
      } else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TEC ) {
	clusterType = 4;
	TECDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.wheel();
	++cluster_n_tec_;
      }
      
      cluster_type_[cluster_n_]   = clusterType;
      cluster_layer_[cluster_n_]  = clusterLayer;
      cluster_charge_[cluster_n_] = clusterCharge;
      ++cluster_n_;
      
    } // end loop over all rPhi RecHits
    
    // Loop over Stereo Hits :
    //========================
    
    if ( debug_ ) std::cout <<  " TrackTupleMaker::analyze() : Looping over " << stereoRecHitCollection->size() << " Stereo RecHits" << std::endl;
    
    for(SiStripRecHit2DCollection::DataContainer::const_iterator rhit=stereoRecHitCollection->data().begin(); rhit!=stereoRecHitCollection->data().end(); ++rhit) {
      
      if ( cluster_n_ >= nMaxClusters_ ) {
	std::cout << " TrackTupleMaker::analyze() : Warning #cluster : " << cluster_n_ << " , greater than " << nMaxClusters_ << std::endl;
	continue;
      }
      
      ++cluster_n_all_;
      
      unsigned int clusterType  = 0;
      unsigned int clusterLayer = 0;
      uint16_t clusterCharge = ClusterCharge(&(*rhit));
      
      DetId id_tmp((&(*rhit))->geographicalId());
      if        ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TIB ) {
	clusterType = 1;
	TIBDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.layer();
	++cluster_n_tib_;
      } else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TID ) {
	clusterType = 2;
	TIDDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.wheel();
	++cluster_n_tid_;
      } else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TOB ) {
	clusterType = 3;
	TOBDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.layer();
	++cluster_n_tob_;
      } else if ( (unsigned int)id_tmp.subdetId() == StripSubdetector::TEC ) {
	clusterType = 4;
	TECDetId useDetId(id_tmp.rawId());
	clusterLayer = useDetId.wheel();
	++cluster_n_tec_;
      }
      
      cluster_type_[cluster_n_]   = clusterType;
      cluster_layer_[cluster_n_]  = clusterLayer;
      cluster_charge_[cluster_n_] = clusterCharge;
      ++cluster_n_;
      
    } // end loop over all Stereo RecHits
    
  } // endif save clusters

  // Trigger Bits
  bool techBitPass;
  if(!selTechBit_) techBitPass = true;
  else techBitPass = false;
  
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();
  edm::Handle< L1GlobalTriggerReadoutRecord > gtRecord;
  iEvent.getByLabel( edm::InputTag("gtDigis"), gtRecord);
  
  // Get dWord after masking disabled bits
  DecisionWord dWord = gtRecord->decisionWord();
  if ( ! dWord.empty() ) { // if board not there this is zero
    // loop over dec. bit to get total rate (no overlap)
    for ( int i = 0; i < 128; ++i ) {
      if ( dWord[i] ) {
	//cout<<"physics number "<<i<<"  "<<endl;
	physics_bits_[nphysbits_]=i;
	nphysbits_++;
	//cout<<"physics bits number "<< physics_bits_[i]<<"  "<<endl;
      }
    }
  }
  
  TechnicalTriggerWord tw = gtRecord->technicalTriggerWord();
  if ( ! tw.empty() ) {
    // loop over dec. bit to get total rate (no overlap)
    for ( int i = 0; i < 64; ++i ) {
      if ( tw[i] ) {
	//cout<<"technical number "<<i<<"  "<<endl;
	technical_bits_[ntechbits_]=i;
	if (i == 40 || i == 41) isTechBit40_  = 1; 
	// The beamHalo bits are 36-39
	if (i < 40 && i > 35) isBeamHalo_ = 1; 
	// if BSC activity, (technical bit) is between 32 and 43, set to true
	if (i > 31 && i < 44 ) isBSC_ = 1;
	if( selTechBit_) {
	  if ( i == techBitToSelect_) { 
	    techBitPass = true;
	    if(debug_) cout<<"Selecting Technical Bit "<<i<<endl;
	  }
	}
	ntechbits_++;
      }
    }
  }
  

  if( techBitPass && nonFakePvtx ) // select on the event based on techBit and nonFakePvtx
    rootTree_->Fill();
} 


// ------------ method called once each job before begining the event loop  ------------
void LhcTrackAnalyzer::beginJob()
  {
  edm::LogInfo("beginJob") << "Begin Job" << std::endl;
  // Define TTree for output
  rootFile_ = new TFile(filename_.c_str(),"recreate");
  rootTree_ = new TTree("tree","Lhc Track tree");
  // Global
  rootTree_->Branch("glob_runno",&glob_runno_,"glob_runno/I");
  rootTree_->Branch("glob_evtno",&glob_evtno_,"glob_evtno/I");
  rootTree_->Branch("glob_ls",&glob_ls_,"glob_ls/I");
  rootTree_->Branch("glob_bx",&glob_bx_,"glob_bx/I");
   
 

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
  rootTree_->Branch("ndof_pvtx",&ndof_pvtx_,"ndof_pvtx[nVertices]/D"); 
  rootTree_->Branch("isValid_pvtx",&isValid_pvtx_,"isValid_pvtx[nVertices]/I"); 
  rootTree_->Branch("isFake_pvtx",&isFake_pvtx_,"isFake_pvtx[nVertices]/I"); 
  rootTree_->Branch("recx_pvtx",&recx_pvtx_,"recx_pvtx[nVertices]/D"); 
  rootTree_->Branch("recy_pvtx",&recy_pvtx_,"recy_pvtx[nVertices]/D"); 
  rootTree_->Branch("recz_pvtx",&recz_pvtx_,"recz_pvtx[nVertices]/D");
  rootTree_->Branch("recx_err_pvtx",&recx_err_pvtx_,"recx_err_pvtx[nVertices]/D"); 
  rootTree_->Branch("recy_err_pvtx",&recy_err_pvtx_,"recy_err_pvtx[nVertices]/D"); 
  rootTree_->Branch("recz_err_pvtx",&recz_err_pvtx_,"recz_err_pvtx[nVertices]/D");
  rootTree_->Branch("hasGoodPvtx",&hasGoodPvtx_,"hasGoodPvtx/I");  

  // PixelVertices
  rootTree_->Branch("nPixelVertices",&nPixelVertices_,"nPixelVertices/I"); 
  rootTree_->Branch("nTracks_pxlpvtx",&nTracks_pxlpvtx_,"nTracks_pxlpvtx[nPixelVertices]/I"); 
  rootTree_->Branch("isFake_pxlpvtx",&isFake_pxlpvtx_,"isFake_pxlpvtx[nPixelVertices]/I"); 
  rootTree_->Branch("recz_pxlpvtx",&recz_pxlpvtx_,"recz_pxlpvtx[nPixelVertices]/D");
  rootTree_->Branch("recz_err_pxlpvtx",&recz_err_pxlpvtx_,"recz_err_pxlpvtx[nPixelVertices]/D"); 
  rootTree_->Branch("hasGoodPxlPvtx",&hasGoodPxlPvtx_,"hasGoodPxlPvtx/I"); 
  

  // Trigger bits
  rootTree_->Branch("ntechbits",&ntechbits_,"ntechbits/I");
  rootTree_->Branch("nphysbits",&nphysbits_,"nphysbits/I");
  rootTree_->Branch("technical_bits",&technical_bits_,"technical_bits[ntechbits]/I");
  rootTree_->Branch("physics_bits",&physics_bits_,"physics_bits[nphysbits]/I");
  rootTree_->Branch("isTechBit40",&isTechBit40_,"isTechBit40/I"); 
  rootTree_->Branch("isBSC",&isBSC_,"isBSC/I"); 
  rootTree_->Branch("isBeamHalo",&isBeamHalo_,"isBeamHalo/I"); 
  rootTree_->Branch("belowPtThresold",&belowPtThresold_,"belowPtThresold/I"); 



  // CTF Track
  rootTree_->Branch("ctf_n",&ctf_n_,"ctf_n/I");
  rootTree_->Branch("ctf_nHighPurity",&ctf_nHighPurity_,"ctf_nHighPurity/I"); 
  rootTree_->Branch("ctf_fHighPurity",&ctf_fHighPurity_,"ctf_fHighPurity/D"); 
  rootTree_->Branch("ctf_pt",&ctf_pt_,"ctf_pt[ctf_n]/D");
  rootTree_->Branch("ctf_ptErr",&ctf_ptErr_,"ctf_ptErr[ctf_n]/D");
  rootTree_->Branch("ctf_eta",&ctf_eta_,"ctf_eta[ctf_n]/D");
  rootTree_->Branch("ctf_etaErr",&ctf_etaErr_,"ctf_etaErr[ctf_n]/D");
  rootTree_->Branch("ctf_phi",&ctf_phi_,"ctf_phi[ctf_n]/D");
  rootTree_->Branch("ctf_phiErr",&ctf_phiErr_,"ctf_phiErr[ctf_n]/D");
  rootTree_->Branch("ctf_dz",&ctf_dz_,"ctf_dz[ctf_n]/D");
  rootTree_->Branch("ctf_dzErr",&ctf_dzErr_,"ctf_dzErr[ctf_n]/D");
  rootTree_->Branch("ctf_dxy",&ctf_dxy_,"ctf_dxy[ctf_n]/D"); 
  rootTree_->Branch("ctf_dxyErr",&ctf_dxyErr_,"ctf_dxyErr[ctf_n]/D");
  rootTree_->Branch("ctf_dxyCorr",&ctf_dxyCorr_,"ctf_dxyCorr[ctf_n]/D");
  rootTree_->Branch("ctf_dzCorr",&ctf_dzCorr_,"ctf_dzCorr[ctf_n]/D");
  rootTree_->Branch("ctf_dxyCorr_pvtx",&ctf_dxyCorr_pvtx_,"ctf_dxyCorr_pvtx[ctf_n]/D");
  rootTree_->Branch("ctf_dzCorr_pvtx",&ctf_dzCorr_pvtx_,"ctf_dzCorr_pvtx[ctf_n]/D");
  rootTree_->Branch("ctf_dzCorrErr_pvtx",&ctf_dzCorrErr_pvtx_,"ctf_dzCorrErr_pvtx[ctf_n]/D");
  rootTree_->Branch("ctf_chi2",&ctf_chi2_,"ctf_chi2[ctf_n]/D");
  rootTree_->Branch("ctf_chi2ndof",&ctf_chi2ndof_,"ctf_chi2ndof[ctf_n]/D");
  rootTree_->Branch("ctf_charge",&ctf_charge_,"ctf_charge[ctf_n]/I");
  rootTree_->Branch("ctf_qoverp",&ctf_qoverp_,"ctf_qoverp[ctf_n]/D");
  rootTree_->Branch("ctf_algo",&ctf_algo_,"ctf_algo[ctf_n]/I");
  rootTree_->Branch("ctf_qualityMask",&ctf_qualityMask_,"ctf_qualityMask[ctf_n]/I");
  rootTree_->Branch("ctf_isLoose",&ctf_isLoose_,"ctf_isLoose[ctf_n]/I");
  rootTree_->Branch("ctf_isTight",&ctf_isTight_,"ctf_isTight[ctf_n]/I");
  rootTree_->Branch("ctf_isHighPurity",&ctf_isHighPurity_,"ctf_isHighPurity[ctf_n]/I");
  rootTree_->Branch("ctf_xPCA",&ctf_xPCA_,"ctf_xPCA[ctf_n]/D");
  rootTree_->Branch("ctf_yPCA",&ctf_yPCA_,"ctf_yPCA[ctf_n]/D");
  rootTree_->Branch("ctf_zPCA",&ctf_zPCA_,"ctf_zPCA[ctf_n]/D");
  rootTree_->Branch("ctf_nLayers",&ctf_nLayers_,"ctf_nLayers[ctf_n]/I");
  rootTree_->Branch("ctf_nLayers3D",&ctf_nLayers3D_,"ctf_nLayers3D[ctf_n]/I");
  rootTree_->Branch("ctf_nPXBLayers",&ctf_nPXBLayers_,"ctf_nPXBLayers[ctf_n]/I");
  rootTree_->Branch("ctf_nPXFLayers",&ctf_nPXFLayers_,"ctf_nPXFLayers[ctf_n]/I");
  rootTree_->Branch("ctf_trkWeightpvtx",&ctf_trkWeightpvtx_,"ctf_trkWeightpvtx[ctf_n]/D");
  rootTree_->Branch("ctf_pvtx_no",&ctf_pvtx_no_,"ctf_pvtx_no[ctf_n]/I");

  // CTF Track Hits
  rootTree_->Branch("ctf_nHit",&ctf_nHit_,"ctf_nHit[ctf_n]/I");
  rootTree_->Branch("ctf_nLostHit",&ctf_nLostHit_,"ctf_nLostHit[ctf_n]/I");
  rootTree_->Branch("ctf_nStripHit",&ctf_nStripHit_,"ctf_nStripHit[ctf_n]/I");
  rootTree_->Branch("ctf_nPixelHit",&ctf_nPixelHit_,"ctf_nPixelHit[ctf_n]/I");
  rootTree_->Branch("ctf_nPXBhit",&ctf_nPXBhit_,"ctf_nPXBhit[ctf_n]/I");
  rootTree_->Branch("ctf_nPXFhit",&ctf_nPXFhit_,"ctf_nPXFhit[ctf_n]/I");
  rootTree_->Branch("ctf_nTIBhit",&ctf_nTIBhit_,"ctf_nTIBhit[ctf_n]/I");
  rootTree_->Branch("ctf_nTIB1hit",&ctf_nTIB1hit_,"ctf_nTIB1hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTIB2hit",&ctf_nTIB2hit_,"ctf_nTIB2hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTIB3hit",&ctf_nTIB3hit_,"ctf_nTIB3hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTIB4hit",&ctf_nTIB4hit_,"ctf_nTIB4hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTIDhit",&ctf_nTIDhit_,"ctf_nTIDhit[ctf_n]/I");
  rootTree_->Branch("ctf_nTID1hit",&ctf_nTID1hit_,"ctf_nTID1hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTID2hit",&ctf_nTID2hit_,"ctf_nTID2hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTID3hit",&ctf_nTID3hit_,"ctf_nTID3hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOBhit",&ctf_nTOBhit_,"ctf_nTOBhit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOB1hit",&ctf_nTOB1hit_,"ctf_nTOB1hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOB2hit",&ctf_nTOB2hit_,"ctf_nTOB2hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOB3hit",&ctf_nTOB3hit_,"ctf_nTOB3hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOB4hit",&ctf_nTOB4hit_,"ctf_nTOB4hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOB5hit",&ctf_nTOB5hit_,"ctf_nTOB5hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTOB6hit",&ctf_nTOB6hit_,"ctf_nTOB6hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEChit",&ctf_nTEChit_,"ctf_nTEChit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC1hit",&ctf_nTEC1hit_,"ctf_nTEC1hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC2hit",&ctf_nTEC2hit_,"ctf_nTEC2hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC3hit",&ctf_nTEC3hit_,"ctf_nTEC3hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC4hit",&ctf_nTEC4hit_,"ctf_nTEC4hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC5hit",&ctf_nTEC5hit_,"ctf_nTEC5hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC6hit",&ctf_nTEC6hit_,"ctf_nTEC6hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC7hit",&ctf_nTEC7hit_,"ctf_nTEC7hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC8hit",&ctf_nTEC8hit_,"ctf_nTEC8hit[ctf_n]/I");
  rootTree_->Branch("ctf_nTEC9hit",&ctf_nTEC9hit_,"ctf_nTEC9hit[ctf_n]/I");
  // CTF Cluster
  rootTree_->Branch("ctf_clusterCharge_all",&ctf_clusterCharge_all_,"ctf_clusterCharge_all/D");
  rootTree_->Branch("ctf_clusterCharge_TIB",&ctf_clusterCharge_TIB_,"ctf_clusterCharge_TIB/D");
  rootTree_->Branch("ctf_clusterCharge_TID",&ctf_clusterCharge_TID_,"ctf_clusterCharge_TID/D");
  rootTree_->Branch("ctf_clusterCharge_TOB",&ctf_clusterCharge_TOB_,"ctf_clusterCharge_TOB/D");
  rootTree_->Branch("ctf_clusterCharge_TEC",&ctf_clusterCharge_TEC_,"ctf_clusterCharge_TEC/D");
  rootTree_->Branch("ctfcluster_n",&ctfcluster_n_,"ctfcluster_n/I");
  rootTree_->Branch("ctfcluster_type",&ctfcluster_type_,"ctfcluster_type[ctfcluster_n]/I");
  rootTree_->Branch("ctfcluster_layer",&ctfcluster_layer_,"ctfcluster_layer[ctfcluster_n]/I");
  rootTree_->Branch("ctfcluster_charge",&ctfcluster_charge_,"ctfcluster_charge[ctfcluster_n]/D");
  if ( afterRefitting_ ) {
    rootTree_->Branch("ctfcluster_x",&ctfcluster_x_,"ctfcluster_x[ctfcluster_n]/D");
    rootTree_->Branch("ctfcluster_y",&ctfcluster_y_,"ctfcluster_y[ctfcluster_n]/D");
    rootTree_->Branch("ctfcluster_z",&ctfcluster_z_,"ctfcluster_z[ctfcluster_n]/D");
  }
  // If running on second track Coll
  if( runSecTrackColl_ ) {
    rootTree_->Branch("sectrk_n",&sectrk_n_,"sectrk_n/I");
    rootTree_->Branch("sectrk_pt",&sectrk_pt_,"sectrk_pt[sectrk_n]/D");
    rootTree_->Branch("sectrk_ptErr",&sectrk_ptErr_,"sectrk_ptErr[sectrk_n]/D");
    rootTree_->Branch("sectrk_eta",&sectrk_eta_,"sectrk_eta[sectrk_n]/D");
    rootTree_->Branch("sectrk_etaErr",&sectrk_etaErr_,"sectrk_etaErr[sectrk_n]/D");
    rootTree_->Branch("sectrk_phi",&sectrk_phi_,"sectrk_phi[sectrk_n]/D");
    rootTree_->Branch("sectrk_phiErr",&sectrk_phiErr_,"sectrk_phiErr[sectrk_n]/D");
    rootTree_->Branch("sectrk_dz",&sectrk_dz_,"sectrk_dz[sectrk_n]/D");
    rootTree_->Branch("sectrk_dzErr",&sectrk_dzErr_,"sectrk_dzErr[sectrk_n]/D");
    rootTree_->Branch("sectrk_dxy",&sectrk_dxy_,"sectrk_dxy[sectrk_n]/D"); 
    rootTree_->Branch("sectrk_dxyErr",&sectrk_dxyErr_,"sectrk_dxyErr[sectrk_n]/D");
    rootTree_->Branch("sectrk_dxyCorr",&sectrk_dxyCorr_,"sectrk_dxyCorr[sectrk_n]/D");
    rootTree_->Branch("sectrk_dzCorr",&sectrk_dzCorr_,"sectrk_dzCorr[sectrk_n]/D");
    rootTree_->Branch("sectrk_dxyCorr_pvtx",&sectrk_dxyCorr_pvtx_,"sectrk_dxyCorr_pvtx[sectrk_n]/D");
    rootTree_->Branch("sectrk_dzCorr_pvtx",&sectrk_dzCorr_pvtx_,"sectrk_dzCorr_pvtx[sectrk_n]/D");
    rootTree_->Branch("sectrk_chi2",&sectrk_chi2_,"sectrk_chi2[sectrk_n]/D");
    rootTree_->Branch("sectrk_chi2ndof",&sectrk_chi2ndof_,"sectrk_chi2ndof[sectrk_n]/D");
    rootTree_->Branch("sectrk_charge",&sectrk_charge_,"sectrk_charge[sectrk_n]/I");
    rootTree_->Branch("sectrk_qoverp",&sectrk_qoverp_,"sectrk_qoverp[sectrk_n]/D");
    rootTree_->Branch("sectrk_algo",&sectrk_algo_,"sectrk_algo[sectrk_n]/I");
    rootTree_->Branch("sectrk_qualityMask",&sectrk_qualityMask_,"sectrk_qualityMask[sectrk_n]/I");
    rootTree_->Branch("sectrk_isLoose",&sectrk_isLoose_,"sectrk_isLoose[sectrk_n]/I");
    rootTree_->Branch("sectrk_isTight",&sectrk_isTight_,"sectrk_isTight[sectrk_n]/I");
    rootTree_->Branch("sectrk_isHighPurity",&sectrk_isHighPurity_,"sectrk_isHighPurity[sectrk_n]/I");
    rootTree_->Branch("sectrk_xPCA",&sectrk_xPCA_,"sectrk_xPCA[sectrk_n]/D");
    rootTree_->Branch("sectrk_yPCA",&sectrk_yPCA_,"sectrk_yPCA[sectrk_n]/D");
    rootTree_->Branch("sectrk_zPCA",&sectrk_zPCA_,"sectrk_zPCA[sectrk_n]/D"); 
    rootTree_->Branch("sectrk_nLayers",&sectrk_nLayers_,"sectrk_nLayers[sectrk_n]/I");
    rootTree_->Branch("sectrk_nLayers3D",&sectrk_nLayers3D_,"sectrk_nLayers3D[sectrk_n]/I"); 
    rootTree_->Branch("sectrk_nPXBLayers",&sectrk_nPXBLayers_,"sectrk_nPXBLayers[sectrk_n]/I");
    rootTree_->Branch("sectrk_nPXFLayers",&sectrk_nPXFLayers_,"sectrk_nPXFLayers[sectrk_n]/I");
    
    // SECTRK Track Hits
    rootTree_->Branch("sectrk_nHit",&sectrk_nHit_,"sectrk_nHit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nLostHit",&sectrk_nLostHit_,"sectrk_nLostHit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nStripHit",&sectrk_nStripHit_,"sectrk_nStripHit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nPixelHit",&sectrk_nPixelHit_,"sectrk_nPixelHit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nPXBhit",&sectrk_nPXBhit_,"sectrk_nPXBhit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nPXFhit",&sectrk_nPXFhit_,"sectrk_nPXFhit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTIBhit",&sectrk_nTIBhit_,"sectrk_nTIBhit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTIB1hit",&sectrk_nTIB1hit_,"sectrk_nTIB1hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTIB2hit",&sectrk_nTIB2hit_,"sectrk_nTIB2hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTIB3hit",&sectrk_nTIB3hit_,"sectrk_nTIB3hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTIB4hit",&sectrk_nTIB4hit_,"sectrk_nTIB4hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTIDhit",&sectrk_nTIDhit_,"sectrk_nTIDhit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTID1hit",&sectrk_nTID1hit_,"sectrk_nTID1hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTID2hit",&sectrk_nTID2hit_,"sectrk_nTID2hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTID3hit",&sectrk_nTID3hit_,"sectrk_nTID3hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOBhit",&sectrk_nTOBhit_,"sectrk_nTOBhit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOB1hit",&sectrk_nTOB1hit_,"sectrk_nTOB1hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOB2hit",&sectrk_nTOB2hit_,"sectrk_nTOB2hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOB3hit",&sectrk_nTOB3hit_,"sectrk_nTOB3hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOB4hit",&sectrk_nTOB4hit_,"sectrk_nTOB4hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOB5hit",&sectrk_nTOB5hit_,"sectrk_nTOB5hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTOB6hit",&sectrk_nTOB6hit_,"sectrk_nTOB6hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEChit",&sectrk_nTEChit_,"sectrk_nTEChit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC1hit",&sectrk_nTEC1hit_,"sectrk_nTEC1hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC2hit",&sectrk_nTEC2hit_,"sectrk_nTEC2hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC3hit",&sectrk_nTEC3hit_,"sectrk_nTEC3hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC4hit",&sectrk_nTEC4hit_,"sectrk_nTEC4hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC5hit",&sectrk_nTEC5hit_,"sectrk_nTEC5hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC6hit",&sectrk_nTEC6hit_,"sectrk_nTEC6hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC7hit",&sectrk_nTEC7hit_,"sectrk_nTEC7hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC8hit",&sectrk_nTEC8hit_,"sectrk_nTEC8hit[sectrk_n]/I");
    rootTree_->Branch("sectrk_nTEC9hit",&sectrk_nTEC9hit_,"sectrk_nTEC9hit[sectrk_n]/I");
    // SECTRK Cluster
    rootTree_->Branch("sectrk_clusterCharge_all",&sectrk_clusterCharge_all_,"sectrk_clusterCharge_all/D");
    rootTree_->Branch("sectrk_clusterCharge_TIB",&sectrk_clusterCharge_TIB_,"sectrk_clusterCharge_TIB/D");
    rootTree_->Branch("sectrk_clusterCharge_TID",&sectrk_clusterCharge_TID_,"sectrk_clusterCharge_TID/D");
    rootTree_->Branch("sectrk_clusterCharge_TOB",&sectrk_clusterCharge_TOB_,"sectrk_clusterCharge_TOB/D");
    rootTree_->Branch("sectrk_clusterCharge_TEC",&sectrk_clusterCharge_TEC_,"sectrk_clusterCharge_TEC/D");
    rootTree_->Branch("sectrkcluster_n",&sectrkcluster_n_,"sectrkcluster_n/I");
    rootTree_->Branch("sectrkcluster_type",&sectrkcluster_type_,"sectrkcluster_type[sectrkcluster_n]/I");
    rootTree_->Branch("sectrkcluster_layer",&sectrkcluster_layer_,"sectrkcluster_layer[sectrkcluster_n]/I");
    rootTree_->Branch("sectrkcluster_charge",&sectrkcluster_charge_,"sectrkcluster_charge[sectrkcluster_n]/D");
    if ( afterRefitting_ ) {
    rootTree_->Branch("sectrkcluster_x",&sectrkcluster_x_,"sectrkcluster_x[sectrkcluster_n]/D");
    rootTree_->Branch("sectrkcluster_y",&sectrkcluster_y_,"sectrkcluster_y[sectrkcluster_n]/D");
    rootTree_->Branch("sectrkcluster_z",&sectrkcluster_z_,"sectrkcluster_z[sectrkcluster_n]/D");
    }
  }

  if ( saveAllClusters_ ) { 
    // all clusters
    rootTree_->Branch("cluster_n_all",&cluster_n_all_,"cluster_n_all/I");
    rootTree_->Branch("cluster_n_tib",&cluster_n_tib_,"cluster_n_tib/I");
    rootTree_->Branch("cluster_n_tid",&cluster_n_tid_,"cluster_n_tid/I");
    rootTree_->Branch("cluster_n_tob",&cluster_n_tob_,"cluster_n_tob/I");
    rootTree_->Branch("cluster_n_tec",&cluster_n_tec_,"cluster_n_tec/I");
    rootTree_->Branch("cluster_n",&cluster_n_,"cluster_n/I");
    rootTree_->Branch("cluster_type",&cluster_type_,"cluster_type[cluster_n]/I");
    rootTree_->Branch("cluster_layer",&cluster_layer_,"cluster_layer[cluster_n]/I");
    rootTree_->Branch("cluster_charge",&cluster_charge_,"cluster_charge[cluster_n]/D");
    if ( afterRefitting_ ) {
      rootTree_->Branch("cluster_x",&cluster_x_,"cluster_x[cluster_n]/D");
      rootTree_->Branch("cluster_y",&cluster_y_,"cluster_y[cluster_n]/D");
      rootTree_->Branch("cluster_z",&cluster_z_,"cluster_z[cluster_n]/D");
    }
  }    
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
  
  // Global
  glob_evtno_ = 0;
  glob_runno_ = 0;
  glob_ls_ = 0;
  glob_bx_ = 0;

  //BeamSpot
  bsX0_ = 0;
  bsY0_ = 0;
  bsZ0_ = 0;
  bsDxdz_ = 0;
  bsDydz_ = 0;
  
  // pt
  belowPtThresold_ = 0;
  // Trigger bits
  isTechBit40_=0;
  isBeamHalo_ = 0;
  isBSC_ = 0;
  ntechbits_=0;
  nphysbits_=0;
  for ( int i=0; i<nMaxbits_; ++i ) {
    technical_bits_[i]= 0;
  }
  for ( int i=0; i<nMaxbits_; ++i ) {
    physics_bits_[i]= 0;
  }
  
  // PrimaryVertex
  hasGoodPvtx_ = 0;
  nVertices_ = 0;
  for ( int i=0; i<nMaxPVs_; ++i ) {
    nTracks_pvtx_[i] = 0; // Number of tracks in the pvtx
    ndof_pvtx_[i] = 0;
    sumptsq_pvtx_[i] = 0;
    isValid_pvtx_[i] = 0;
    isFake_pvtx_[i] = 0;
    recx_pvtx_[i] = 0;
    recy_pvtx_[i] = 0;
    recz_pvtx_[i] = 0;
    recx_err_pvtx_[i] = 0;
    recy_err_pvtx_[i] = 0;
    recz_err_pvtx_[i] = 0;
  }
  
  // PixelVertices
  hasGoodPxlPvtx_ = 0;
  nPixelVertices_ = 0;
  for ( int i=0; i<nMaxPixelPVs_; ++i ) { 
    nTracks_pxlpvtx_[i] = 0;
    isFake_pxlpvtx_[i] = 0;
    recz_pxlpvtx_[i] = 0; 
    recz_err_pxlpvtx_[i] = 0; 
  }
  
  // == CTF Track
  ctf_n_ = 0;
  ctf_nHighPurity_=0;
  ctf_fHighPurity_=0;
  for ( int i=0; i<nMaxCTFtracks_; ++i ) {
    ctf_pt_[i]             = 0;
    ctf_eta_[i]            = 0;
    ctf_phi_[i]            = 0;
    ctf_dz_[i]             = 0;
    ctf_dxy_[i]            = 0;
    ctf_dzCorr_[i]         = 0;
    ctf_dxyCorr_[i]        = 0;
    ctf_dzCorr_pvtx_[i]         = 0;
    ctf_dxyCorr_pvtx_[i]        = 0;
    ctf_dzCorrErr_pvtx_[i]         = 0;
    ctf_ptErr_[i]          = 0;
    ctf_etaErr_[i]         = 0;
    ctf_phiErr_[i]         = 0;
    ctf_dzErr_[i]          = 0;
    ctf_dxyErr_[i]         = 0;
    ctf_chi2_[i]           = 0;
    ctf_chi2ndof_[i]       = 0;
    ctf_charge_[i]         = 0;
    ctf_qoverp_[i]         = 0;
    ctf_algo_[i]           = 0;
    ctf_qualityMask_[i]     = 0;
    ctf_isHighPurity_[i]    = 0;
    ctf_isTight_[i]         = 0;
    ctf_isLoose_[i]         = 0;
    ctf_xPCA_[i]        = 0;
    ctf_yPCA_[i]        = 0;
    ctf_zPCA_[i]        = 0;  
    ctf_nLayers_[i]     = 0;
    ctf_nPXBLayers_[i]  = 0;
    ctf_nPXFLayers_[i]  = 0;
    ctf_nLayers3D_[i]   = 0;
    ctf_trkWeightpvtx_[i] = 0;
    ctf_pvtx_no_[i] = -1;  

    // Hits
    ctf_nHit_[i]      = 0;
    ctf_nLostHit_[i]      = 0;
    ctf_nStripHit_[i] = 0;
    ctf_nPixelHit_[i] = 0;
    ctf_nTIBhit_[i]   = 0;
    ctf_nTIB1hit_[i]  = 0;
    ctf_nTIB2hit_[i]  = 0;
    ctf_nTIB3hit_[i]  = 0;
    ctf_nTIB4hit_[i]  = 0;
    ctf_nTIDhit_[i]   = 0;
    ctf_nTID1hit_[i]  = 0;
    ctf_nTID2hit_[i]  = 0;
    ctf_nTID3hit_[i]  = 0;
    ctf_nTOBhit_[i]   = 0;
    ctf_nTOB1hit_[i]  = 0;
    ctf_nTOB2hit_[i]  = 0;
    ctf_nTOB3hit_[i]  = 0;
    ctf_nTOB4hit_[i]  = 0;
    ctf_nTOB5hit_[i]  = 0;
    ctf_nTOB6hit_[i]  = 0;
    ctf_nTEChit_[i]   = 0;
    ctf_nTEC1hit_[i]  = 0;
    ctf_nTEC2hit_[i]  = 0;
    ctf_nTEC3hit_[i]  = 0;
    ctf_nTEC4hit_[i]  = 0;
    ctf_nTEC5hit_[i]  = 0;
    ctf_nTEC6hit_[i]  = 0;
    ctf_nTEC7hit_[i]  = 0;
    ctf_nTEC8hit_[i]  = 0;
    ctf_nTEC9hit_[i]  = 0;
    ctf_nPXBhit_[i]   = 0;
    ctf_nPXFhit_[i]   = 0;
  }
  // ctf Cluster
  ctf_clusterCharge_all_ = 0;
  ctf_clusterCharge_TIB_ = 0;
  ctf_clusterCharge_TID_ = 0;
  ctf_clusterCharge_TOB_ = 0;
  ctf_clusterCharge_TEC_ = 0;
  ctfcluster_n_  = 0;
  for ( int i=0; i<nMaxCTFclusters_; ++i ) {
    ctfcluster_type_[i]   = 0;
    ctfcluster_layer_[i]  = 0;
    ctfcluster_charge_[i] = 0;
    ctfcluster_x_[i] = 0;
    ctfcluster_y_[i] = 0;
    ctfcluster_z_[i] = 0;
  }

  // == track from second track Collection
  sectrk_n_ = 0;
  for ( int i=0; i<nMaxSECTRKtracks_; ++i ) {
    sectrk_pt_[i]             = 0;
    sectrk_eta_[i]            = 0;
    sectrk_phi_[i]            = 0;
    sectrk_dz_[i]             = 0;
    sectrk_dxy_[i]            = 0;
    sectrk_dzCorr_[i]         = 0;
    sectrk_dxyCorr_[i]        = 0;
    sectrk_dzCorr_pvtx_[i]         = 0;
    sectrk_dxyCorr_pvtx_[i]        = 0;
    sectrk_ptErr_[i]          = 0;
    sectrk_etaErr_[i]         = 0;
    sectrk_phiErr_[i]         = 0;
    sectrk_dzErr_[i]          = 0;
    sectrk_dxyErr_[i]         = 0;
    sectrk_chi2_[i]           = 0;
    sectrk_chi2ndof_[i]       = 0;
    sectrk_charge_[i]         = 0;
    sectrk_qoverp_[i]         = 0;
    sectrk_algo_[i]           = 0;
    sectrk_qualityMask_[i]     = 0;
    sectrk_isHighPurity_[i]    = 0;
    sectrk_isTight_[i]         = 0;
    sectrk_isLoose_[i]         = 0;
    sectrk_xPCA_[i]        = 0;
    sectrk_yPCA_[i]        = 0;
    sectrk_zPCA_[i]        = 0; 
    sectrk_nLayers_[i]     = 0;
    sectrk_nPXBLayers_[i]  = 0;
    sectrk_nPXFLayers_[i]  = 0;
    sectrk_nLayers3D_[i]  = 0;

    // Hits
    sectrk_nHit_[i]      = 0;
    sectrk_nLostHit_[i]      = 0;
    sectrk_nStripHit_[i] = 0;
    sectrk_nPixelHit_[i] = 0;
    sectrk_nTIBhit_[i]   = 0;
    sectrk_nTIB1hit_[i]  = 0;
    sectrk_nTIB2hit_[i]  = 0;
    sectrk_nTIB3hit_[i]  = 0;
    sectrk_nTIB4hit_[i]  = 0;
    sectrk_nTIDhit_[i]   = 0;
    sectrk_nTID1hit_[i]  = 0;
    sectrk_nTID2hit_[i]  = 0;
    sectrk_nTID3hit_[i]  = 0;
    sectrk_nTOBhit_[i]   = 0;
    sectrk_nTOB1hit_[i]  = 0;
    sectrk_nTOB2hit_[i]  = 0;
    sectrk_nTOB3hit_[i]  = 0;
    sectrk_nTOB4hit_[i]  = 0;
    sectrk_nTOB5hit_[i]  = 0;
    sectrk_nTOB6hit_[i]  = 0;
    sectrk_nTEChit_[i]   = 0;
    sectrk_nTEC1hit_[i]  = 0;
    sectrk_nTEC2hit_[i]  = 0;
    sectrk_nTEC3hit_[i]  = 0;
    sectrk_nTEC4hit_[i]  = 0;
    sectrk_nTEC5hit_[i]  = 0;
    sectrk_nTEC6hit_[i]  = 0;
    sectrk_nTEC7hit_[i]  = 0;
    sectrk_nTEC8hit_[i]  = 0;
    sectrk_nTEC9hit_[i]  = 0;
    sectrk_nPXBhit_[i]   = 0;
    sectrk_nPXFhit_[i]   = 0;
  }
  // sectrk Cluster
  sectrk_clusterCharge_all_ = 0;
  sectrk_clusterCharge_TIB_ = 0;
  sectrk_clusterCharge_TID_ = 0;
  sectrk_clusterCharge_TOB_ = 0;
  sectrk_clusterCharge_TEC_ = 0;
  sectrkcluster_n_  = 0;
  for ( int i=0; i<nMaxSECTRKclusters_; ++i ) {
    sectrkcluster_type_[i]   = 0;
    sectrkcluster_layer_[i]  = 0;
    sectrkcluster_charge_[i] = 0;
    sectrkcluster_x_[i] = 0;
    sectrkcluster_y_[i] = 0;
    sectrkcluster_z_[i] = 0;
  }

  // All Clusters
  cluster_n_all_ = 0;
  cluster_n_tib_ = 0;
  cluster_n_tid_ = 0;
  cluster_n_tob_ = 0;
  cluster_n_tec_ = 0;
  cluster_n_ = 0;
  for ( int i=0; i<cluster_n_; ++i ) {
    cluster_type_[i]   = 0;
    cluster_layer_[i]  = 0;
    cluster_charge_[i] = 0;
    cluster_x_[i]      = 0;
    cluster_y_[i]      = 0;
    cluster_z_[i]      = 0;
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

std::vector<SiStripRecHit2D*> LhcTrackAnalyzer::getRecHitComponents(const TrackingRecHit* rechit){
   std::vector<SiStripRecHit2D*> output;
   const ProjectedSiStripRecHit2D* phit=dynamic_cast<const ProjectedSiStripRecHit2D*>(rechit);
   const SiStripMatchedRecHit2D* matchedhit=dynamic_cast<const SiStripMatchedRecHit2D*>(rechit);
   const SiStripRecHit2D* hit=dynamic_cast<const SiStripRecHit2D*>(rechit);
   if(phit) hit=&(phit->originalHit());
   if(matchedhit){
      const SiStripRecHit2D* monohit   =  matchedhit->monoHit();
      const SiStripRecHit2D* stereohit =  matchedhit->stereoHit();
      output.push_back(monohit->clone());
      output.push_back(stereohit->clone());
   }
   else if (hit){
      output.push_back(hit->clone());
   }
   return output;
}

uint16_t LhcTrackAnalyzer::ClusterCharge(const SiStripRecHit2D* hit){
  uint16_t charge = 0;
  edm::Ref<edmNew::DetSetVector<SiStripCluster> ,SiStripCluster>
    cluster = hit->cluster();
  std::vector<uint8_t>::const_iterator it;
  for (it = (cluster->amplitudes()).begin(); it != (cluster->amplitudes()).end(); ++it){
    charge += (*it);
  }
  return charge;
}


//define this as a plug-in
//DEFINE_FWK_MODULE(LhcTrackAnalyzer);
