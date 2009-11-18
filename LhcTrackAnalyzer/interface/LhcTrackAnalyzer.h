#ifndef LhcTrackAnalyzer_h
#define LhcTrackAnalyzer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TTree.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloud.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloudCollection.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"

#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

//FOR CLUSTERINFO
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
//#include "DataFormats/SiStripCluster/interface/SiStripClusterInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "DataFormats/Common/interface/EDProduct.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

// Vertex Stuff
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"  
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

// BeamSpot 
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

// system include files
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <map>

//
// class decleration
//

class LhcTrackAnalyzer : public edm::EDAnalyzer {

 public:
  explicit LhcTrackAnalyzer(const edm::ParameterSet&);
  ~LhcTrackAnalyzer();
  
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  double sumPtSquared(const reco::Vertex& );

  // ----------member data ---------------------------
  edm::InputTag bsSrc;
  edm::InputTag  trackCollectionTag_;
  edm::InputTag  vertexCollectionTag_;
  bool debug_;
  
  // Output
  std::string filename_;     
  TFile* rootFile_;
  TTree* rootTree_;
  
  // Root-Tuple variables :
  //=======================
  void SetRootVar();  

  // BeamSpot 
  double bsX0_;
  double bsY0_;
  double bsZ0_;
  double bsSigmaZ_;
  double bsDxdz_;
  double bsDydz_;

  // PrimaryVertices
  static const int nMaxPVs_ = 50;
  int nVertices_;
  int nTracks_pvtx_[nMaxPVs_]; // Number of tracks in the pvtx  
  double sumptsq_pvtx_[nMaxPVs_];
  bool isValid_pvtx_[nMaxPVs_];
  bool isFake_pvtx_[nMaxPVs_];
  double recx_pvtx_[nMaxPVs_];
  double recy_pvtx_[nMaxPVs_];
  double recz_pvtx_[nMaxPVs_];
  double recx_err_pvtx_[nMaxPVs_];
  double recy_err_pvtx_[nMaxPVs_];
  double recz_err_pvtx_[nMaxPVs_];

  // Track
  static const int nMaxtracks_ = 1000;
  int nTracks_;
  double pt_[nMaxtracks_];       
  double eta_[nMaxtracks_];
  double phi_[nMaxtracks_];
  double dz_[nMaxtracks_];
  double dxy_[nMaxtracks_];
  double dzCorr_[nMaxtracks_];
  double dxyCorr_[nMaxtracks_];
  double ptErr_[nMaxtracks_];       
  double etaErr_[nMaxtracks_];
  double phiErr_[nMaxtracks_];
  double dzErr_[nMaxtracks_];
  double dxyErr_[nMaxtracks_];
  int    charge_[nMaxtracks_];
  double qoverp_[nMaxtracks_];
  double chi2_[nMaxtracks_];
  double chi2ndof_[nMaxtracks_];
  int    algo_[nMaxtracks_];
  int    qualityMask_[nMaxtracks_];
  int    isLoose_[nMaxtracks_];
  int    isTight_[nMaxtracks_];
  int    isHighPurity_[nMaxtracks_];
  double xPCA_[nMaxtracks_];
  double yPCA_[nMaxtracks_];
  double zPCA_[nMaxtracks_];
  double trkWeightpvtx_[nMaxtracks_]; 

};

#endif
