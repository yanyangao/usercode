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

//FOR CLUSTERINFO
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
//#include "DataFormats/SiStripCluster/interface/SiStripClusterInfo.h"
#include "DataFormats/Common/interface/DetSetVector.h"

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
  std::vector<SiStripRecHit2D*> getRecHitComponents(const TrackingRecHit* rechit);
  uint16_t ClusterCharge(const SiStripRecHit2D* hit);

  // ----------member data ---------------------------
  edm::InputTag bsSrc;
  edm::InputTag  ctfTrackCollectionTag_;
  edm::InputTag  vertexCollectionTag_;
  edm::InputTag  secTrackCollectionTag_;

  bool debug_;
  bool afterRefitting_;
  bool saveAllClusters_;
  bool runSecTrackColl_;

  // Output
  std::string filename_;     
  TFile* rootFile_;
  TTree* rootTree_;
  
  int glob_runno_;
  int glob_evtno_;
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
  int isValid_pvtx_[nMaxPVs_];
  int isFake_pvtx_[nMaxPVs_];
  double recx_pvtx_[nMaxPVs_];
  double recy_pvtx_[nMaxPVs_];
  double recz_pvtx_[nMaxPVs_];
  double recx_err_pvtx_[nMaxPVs_];
  double recy_err_pvtx_[nMaxPVs_];
  double recz_err_pvtx_[nMaxPVs_];

  // Track
  static const int nMaxCTFtracks_ = 1000;
  int ctf_n_;
  double ctf_pt_[nMaxCTFtracks_];       
  double ctf_eta_[nMaxCTFtracks_];
  double ctf_phi_[nMaxCTFtracks_];
  double ctf_dz_[nMaxCTFtracks_];
  double ctf_dxy_[nMaxCTFtracks_];
  double ctf_dzCorr_[nMaxCTFtracks_];
  double ctf_dxyCorr_[nMaxCTFtracks_];
  double ctf_ptErr_[nMaxCTFtracks_];       
  double ctf_etaErr_[nMaxCTFtracks_];
  double ctf_phiErr_[nMaxCTFtracks_];
  double ctf_dzErr_[nMaxCTFtracks_];
  double ctf_dxyErr_[nMaxCTFtracks_];
  int    ctf_charge_[nMaxCTFtracks_];
  double ctf_qoverp_[nMaxCTFtracks_];
  double ctf_chi2_[nMaxCTFtracks_];
  double ctf_chi2ndof_[nMaxCTFtracks_];
  int    ctf_algo_[nMaxCTFtracks_];
  int    ctf_qualityMask_[nMaxCTFtracks_];
  int    ctf_isLoose_[nMaxCTFtracks_];
  int    ctf_isTight_[nMaxCTFtracks_];
  int    ctf_isHighPurity_[nMaxCTFtracks_];
  double ctf_xPCA_[nMaxCTFtracks_];
  double ctf_yPCA_[nMaxCTFtracks_];
  double ctf_zPCA_[nMaxCTFtracks_];
  int    ctf_nLayers_[nMaxCTFtracks_];
  int    ctf_nPXBLayers_[nMaxCTFtracks_];
  int    ctf_nPXFLayers_[nMaxCTFtracks_];
  double ctf_trkWeightpvtx_[nMaxCTFtracks_]; 
  //Hits
  int ctf_nHit_[nMaxCTFtracks_];            //======= Hit Total ============
  int ctf_nStripHit_[nMaxCTFtracks_];
  int ctf_nPixelHit_[nMaxCTFtracks_];
  int ctf_nTIBhit_[nMaxCTFtracks_];         //======= TIB ==================
  int ctf_nTIB1hit_[nMaxCTFtracks_];
  int ctf_nTIB2hit_[nMaxCTFtracks_];
  int ctf_nTIB3hit_[nMaxCTFtracks_];
  int ctf_nTIB4hit_[nMaxCTFtracks_];
  int ctf_nTIDhit_[nMaxCTFtracks_];         //======= TID ==================
  int ctf_nTID1hit_[nMaxCTFtracks_];
  int ctf_nTID2hit_[nMaxCTFtracks_];
  int ctf_nTID3hit_[nMaxCTFtracks_];
  int ctf_nTOBhit_[nMaxCTFtracks_];         //======= TOB ==================
  int ctf_nTOB1hit_[nMaxCTFtracks_];
  int ctf_nTOB2hit_[nMaxCTFtracks_];
  int ctf_nTOB3hit_[nMaxCTFtracks_];
  int ctf_nTOB4hit_[nMaxCTFtracks_];
  int ctf_nTOB5hit_[nMaxCTFtracks_];
  int ctf_nTOB6hit_[nMaxCTFtracks_];
  int ctf_nTEChit_[nMaxCTFtracks_];         //======= TEC ==================
  int ctf_nTEC1hit_[nMaxCTFtracks_];
  int ctf_nTEC2hit_[nMaxCTFtracks_];
  int ctf_nTEC3hit_[nMaxCTFtracks_];
  int ctf_nTEC4hit_[nMaxCTFtracks_];
  int ctf_nTEC5hit_[nMaxCTFtracks_];
  int ctf_nTEC6hit_[nMaxCTFtracks_];
  int ctf_nTEC7hit_[nMaxCTFtracks_];
  int ctf_nTEC8hit_[nMaxCTFtracks_];
  int ctf_nTEC9hit_[nMaxCTFtracks_];
  int ctf_nPXBhit_[nMaxCTFtracks_];         //======= Pixel ==================
  int ctf_nPXFhit_[nMaxCTFtracks_];
  // Cluster
  double ctf_clusterCharge_all_;
  double ctf_clusterCharge_TIB_;
  double ctf_clusterCharge_TID_;
  double ctf_clusterCharge_TOB_;
  double ctf_clusterCharge_TEC_;
  static const int nMaxCTFclusters_ = 10000;
  int    ctfcluster_n_;
  int    ctfcluster_type_[nMaxCTFclusters_];
  int    ctfcluster_layer_[nMaxCTFclusters_];
  double ctfcluster_charge_[nMaxCTFclusters_];
  double ctfcluster_x_[nMaxCTFclusters_];
  double ctfcluster_y_[nMaxCTFclusters_];
  double ctfcluster_z_[nMaxCTFclusters_];

  // second track Collection
  static const int nMaxSECTRKtracks_ = 1000;
  int sectrk_n_;
  double sectrk_pt_[nMaxSECTRKtracks_];       
  double sectrk_eta_[nMaxSECTRKtracks_];
  double sectrk_phi_[nMaxSECTRKtracks_];
  double sectrk_dz_[nMaxSECTRKtracks_];
  double sectrk_dxy_[nMaxSECTRKtracks_];
  double sectrk_dzCorr_[nMaxSECTRKtracks_];
  double sectrk_dxyCorr_[nMaxSECTRKtracks_];
  double sectrk_ptErr_[nMaxSECTRKtracks_];       
  double sectrk_etaErr_[nMaxSECTRKtracks_];
  double sectrk_phiErr_[nMaxSECTRKtracks_];
  double sectrk_dzErr_[nMaxSECTRKtracks_];
  double sectrk_dxyErr_[nMaxSECTRKtracks_];
  int    sectrk_charge_[nMaxSECTRKtracks_];
  double sectrk_qoverp_[nMaxSECTRKtracks_];
  double sectrk_chi2_[nMaxSECTRKtracks_];
  double sectrk_chi2ndof_[nMaxSECTRKtracks_];
  int    sectrk_algo_[nMaxSECTRKtracks_];
  int    sectrk_qualityMask_[nMaxSECTRKtracks_];
  int    sectrk_isLoose_[nMaxSECTRKtracks_];
  int    sectrk_isTight_[nMaxSECTRKtracks_];
  int    sectrk_isHighPurity_[nMaxSECTRKtracks_];
  double sectrk_xPCA_[nMaxSECTRKtracks_];
  double sectrk_yPCA_[nMaxSECTRKtracks_];
  double sectrk_zPCA_[nMaxSECTRKtracks_]; 
  int    sectrk_nLayers_[nMaxSECTRKtracks_];
  int    sectrk_nPXBLayers_[nMaxSECTRKtracks_];
  int    sectrk_nPXFLayers_[nMaxSECTRKtracks_];
  double sectrk_trkWeightpvtx_[nMaxSECTRKtracks_]; 
  //Hits
  int sectrk_nHit_[nMaxSECTRKtracks_];            //======= Hit Total ============
  int sectrk_nStripHit_[nMaxSECTRKtracks_];
  int sectrk_nPixelHit_[nMaxSECTRKtracks_];
  int sectrk_nTIBhit_[nMaxSECTRKtracks_];         //======= TIB ==================
  int sectrk_nTIB1hit_[nMaxSECTRKtracks_];
  int sectrk_nTIB2hit_[nMaxSECTRKtracks_];
  int sectrk_nTIB3hit_[nMaxSECTRKtracks_];
  int sectrk_nTIB4hit_[nMaxSECTRKtracks_];
  int sectrk_nTIDhit_[nMaxSECTRKtracks_];         //======= TID ==================
  int sectrk_nTID1hit_[nMaxSECTRKtracks_];
  int sectrk_nTID2hit_[nMaxSECTRKtracks_];
  int sectrk_nTID3hit_[nMaxSECTRKtracks_];
  int sectrk_nTOBhit_[nMaxSECTRKtracks_];         //======= TOB ==================
  int sectrk_nTOB1hit_[nMaxSECTRKtracks_];
  int sectrk_nTOB2hit_[nMaxSECTRKtracks_];
  int sectrk_nTOB3hit_[nMaxSECTRKtracks_];
  int sectrk_nTOB4hit_[nMaxSECTRKtracks_];
  int sectrk_nTOB5hit_[nMaxSECTRKtracks_];
  int sectrk_nTOB6hit_[nMaxSECTRKtracks_];
  int sectrk_nTEChit_[nMaxSECTRKtracks_];         //======= TEC ==================
  int sectrk_nTEC1hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC2hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC3hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC4hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC5hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC6hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC7hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC8hit_[nMaxSECTRKtracks_];
  int sectrk_nTEC9hit_[nMaxSECTRKtracks_];
  int sectrk_nPXBhit_[nMaxSECTRKtracks_];   //======= Pixel ==================
  int sectrk_nPXFhit_[nMaxSECTRKtracks_];
  // Cluster
  double sectrk_clusterCharge_all_;
  double sectrk_clusterCharge_TIB_;
  double sectrk_clusterCharge_TID_;
  double sectrk_clusterCharge_TOB_;
  double sectrk_clusterCharge_TEC_;
  static const int nMaxSECTRKclusters_ = 10000;
  int    sectrkcluster_n_;
  int    sectrkcluster_type_[nMaxSECTRKclusters_];
  int    sectrkcluster_layer_[nMaxSECTRKclusters_];
  double sectrkcluster_charge_[nMaxSECTRKclusters_];
  double sectrkcluster_x_[nMaxSECTRKclusters_];
  double sectrkcluster_y_[nMaxSECTRKclusters_];
  double sectrkcluster_z_[nMaxSECTRKclusters_];

  // All Hits Cluster
  int cluster_n_all_;
  int cluster_n_tib_;
  int cluster_n_tid_;
  int cluster_n_tob_;
  int cluster_n_tec_;
  static const int nMaxClusters_ = 5000;
  int cluster_n_;
  int    cluster_type_[nMaxClusters_];
  int    cluster_layer_[nMaxClusters_];
  double cluster_charge_[nMaxClusters_];
  double cluster_x_[nMaxClusters_];
  double cluster_y_[nMaxClusters_];
  double cluster_z_[nMaxClusters_];

};

#endif

