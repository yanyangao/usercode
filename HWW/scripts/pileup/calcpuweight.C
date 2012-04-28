#include "TFile.h"
#include "TArrow.h"
#include "TStyle.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include "TH2F.h"
#include "TF1.h"
#include "TH3.h"
#include "TMath.h"


void calcpuweight(TString mode = "fall11")
{
  
  // 
  // data pileup targets
  //
  
  // 1- read the "true" data target obtained from pileupCalc.py
  TString dataFileName = "pileup_Cert_190456-191859_8TeV_PromptReco_Collisions12_JSON.root";
  // TString dataFileName = "/smurf/sixie/Pileup/PUTarget.Full2011.160404-180252.root";
  TFile *dataFile = TFile::Open(dataFileName);
  assert(dataFile);
  TH1D *hpu_data = (TH1D*) dataFile->Get("pileup");
  hpu_data->SetDirectory(0);
  delete dataFile;

  // 2 - make the observed distributions
  TH1D *hpu_data_obs = new TH1D("hpu_data_obs", "hpu_data_obs", 51, -0.5, 50.5);
  TH3D *hpu_data_3d = new TH3D("hpu_data_3d", "hpu_data_3d", 51, -0.5, 50.5, 51, -0.5, 50.5, 51, -0.5, 50.5);
  
  for ( int i_rand = 0; i_rand < hpu_data->GetEntries() ; i_rand ++) {
    int mean = hpu_data->GetRandom();
    double x = gRandom->Poisson(mean);
    double y = gRandom->Poisson(mean);
    double z = gRandom->Poisson(mean);
    hpu_data_3d->Fill(x,y,z);
    hpu_data_obs->Fill(x);
  }

  //
  // read the mc pu files 
  // 
  
  bool existPUHist = false;
  TString mcFileName;
  TH1D *hpu_mc = new TH1D("hpu_mc", "hpu_mc", 51, -0.5, 50.5);
  TH3D *hpu_mc_3d =  new TH3D("hpu_mc_3d", "hpu_mc_3d", 51, -0.5, 50.5, 51, -0.5, 50.5, 51, -0.5, 50.5);
  
  // 
  // Take the input from an existing file
  // 
  if ( existPUHist) {
    if ( mode == "fall11" ) mcFileName = "/smurf/sixie/Pileup/3DReweighting/histo_f11-zmmm20-powheg-v14b-pu_all_noskim.root";
    TFile *mcFile = TFile::Open(mcFileName);
    hpu_mc_3d = (TH3*) mcFile->Get("hNPU50ns");
    hpu_mc_3d->SetDirectory(0);
    hpu_mc = (TH1D*) mcFile->Get("hNPU");
    hpu_mc->SetDirectory(0);
    delete mcFile;
  } else {
    if ( mode == "fall11" ) mcFileName = "/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/dymm.root";
    if ( mode == "summer12") mcFileName = "/smurf/data/Run2012_Summer12_SmurfV9_51X/mitf-alljets_noweights/dyll.root";
    
    TFile *mcFile = TFile::Open(mcFileName, "READ");
    TTree *tree = (TTree*) mcFile->Get("tree");
    unsigned int npu_ = 0;
    unsigned int npuPlusOne_ = 0;
    unsigned int npuMinusOne_ = 0;
       
    tree->SetBranchAddress( "npu", &npu_);
    tree->SetBranchAddress( "npuPlusOne", &npuPlusOne_);
    tree->SetBranchAddress( "npuMinusOne", &npuMinusOne_);
    
    for(int ievt = 0; ievt< tree->GetEntries(); ievt++){   
      tree->GetEntry(ievt);    
      hpu_mc->Fill(npu_);
      hpu_mc_3d->Fill(npu_, npuPlusOne_, npuMinusOne_);
    }
    delete mcFile;
  }
  
  // normalize each histogram to 1.
  hpu_data->Scale(1./hpu_data->Integral(0,100));
  hpu_mc->Scale(1./hpu_mc->Integral(0,100));
  hpu_data_3d->Scale(1./hpu_data_3d->Integral(0, 100));
  hpu_mc_3d->Scale(1./hpu_mc_3d->Integral(0, 100));
  hpu_data_obs->Scale(1./hpu_data_obs->Integral(0,100));
  
  // calculate weights
  TH1D *puWeights = hpu_data_obs->Clone("puWeights");
  puWeights->Divide(hpu_mc);

  // calculate the 3d pu weights
  TH3D *puWeights3D = hpu_data_3d->Clone("puWeights3D");
  puWeights3D->Divide(hpu_mc_3d);
    
  TString weightFileName;
  if ( mode == "fall11" ) weightFileName = "PileupReweighting.Fall11DYmm_To_Cert_190456-191859_8TeV_PromptReco_Collisions12_JSON.root";
  if ( mode == "summer12" ) weightFileName = "PileupReweighting.Summer12WW_To_Cert_190456-191859_8TeV_PromptReco_Collisions12_JSON.root";
   
  TFile *weightFile = new TFile(weightFileName, "RECREATE");
  weightFile->cd();
  puWeights->Write();
  hpu_data_3d->Write();
  puWeights3D->Write();
  hpu_data_obs->Write();
  weightFile->Close();

}
