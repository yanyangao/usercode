//
// This code does the simple pu reweighting according to the number of vertices
// using the Z->mm events inside peak (+/-15 GeV window)
//

#include "TFile.h"
#include "TArrow.h"
#include "TStyle.h"
#include "TChain.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include "TH2F.h"
#include "TF1.h"
#include "TH3.h"
#include "TMath.h"
#include "Math/LorentzVector.h"
#include "goodrun.cc"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector; 

double nPUScaleFactor(TH1D *fhDPU, int npu);
void calcpuweight()
{
  
  gROOT->ProcessLine(".L goodrun.h++");
  set_goodrun_file("DCSOnly_2012A_160412.jmu");

  // file names of the data/MC smurf ntuples
  TString dataFileName = "data/data.root";
  TString mcFileName =  "data/dyll.root";

  // chain the two files together
  TChain *chain = new TChain("tree");
  chain->Add(dataFileName);
  chain->Add(mcFileName);
  assert(chain);

  TH1D *hnvtx_data = new TH1D("hnvtx_data", "hnvtx_data", 51, -0.5, 50.5);
  TH1D *hnvtx_mc = new TH1D("hnvtx_mc", "hnvtx_mc", 51, -0.5, 50.5);
  
  
  // declare the variables to use
  int nvtx_ = 0;
  int type_ = 0;
  int dstype_ = 0;
  unsigned int run_ = 0;
  unsigned int lumi_ = 0;
  LorentzVector*  dilep_ = 0;

  chain->SetBranchAddress( "nvtx"      , &nvtx_     );   
  chain->SetBranchAddress( "type",  &type_);
  chain->SetBranchAddress( "dstype",  &dstype_);
  chain->SetBranchAddress( "run"      , &run_     );     
  chain->SetBranchAddress( "lumi"      , &lumi_     );  
  chain->SetBranchAddress( "dilep", &dilep_);
  
  for(int ievt = 0; ievt< chain->GetEntries(); ievt++){   
    chain->GetEntry(ievt);  
    if ( abs(dilep_->mass() - 91.1876) > 15 ) continue;
    // apply good run list 
    if ( dstype_ == 0 ) {
      if ( ! goodrun(run_, lumi_) ) continue;
    }
    if ( type_ == 0) {
      if ( dstype_ == 0) 	
	hnvtx_data->Fill(TMath::Min(39, nvtx_));
      else 
	hnvtx_mc->Fill(TMath::Min(39, nvtx_));
    }
  }
  // normalize each histogram to 1.
  hnvtx_data->Scale(1./hnvtx_data->Integral(0,100));
  hnvtx_mc->Scale(1./hnvtx_mc->Integral(0,100));

  
  // Derives weights..
  TH1D *puWeights = (TH1D*)hnvtx_data->Clone("puWeights");
  puWeights->Divide(hnvtx_mc);

  // test if the weights are working
  TH1D *hnvtx_mc_reweighted = new TH1D("hnvtx_mc_reweighted", "hnvtx_mc_reweighted", 51, -0.5, 50.5);
  for(int ievt = 0; ievt< chain->GetEntries(); ievt++){   
    chain->GetEntry(ievt);  
    if ( dstype_ != 0 && type_ == 0) {
      hnvtx_mc_reweighted->Fill(nvtx_, nPUScaleFactor(puWeights, TMath::Min(39, nvtx_)));
    }
      
  }
  hnvtx_mc_reweighted->Scale(1./hnvtx_mc_reweighted->Integral(0,100));
  hnvtx_mc_reweighted->SetLineColor(kRed);
  hnvtx_mc_reweighted->SetMarkerColor(kRed);
  

  TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
  leg->SetFillColor(0);
  leg->AddEntry(hnvtx_data, "Data", "l");
  leg->AddEntry(hnvtx_mc_reweighted, "MC reweighted", "l");


  TCanvas *c1 = new TCanvas("c1", "c1");
  hnvtx_data->Draw("hist");
  hnvtx_mc_reweighted->Draw("samehist");
  leg->Draw("same");
  c1->SaveAs("reweightPU.eps");

  TString weightFileName = "PileUpReweighting_Summer12_2012A.root";
  TFile *weightFile = new TFile(weightFileName, "RECREATE");
  weightFile->cd();
  puWeights->Write();
  hnvtx_data->Write();
  hnvtx_mc->Write();  
  hnvtx_mc_reweighted->Write();
  weightFile->Close();

  delete c1;
  delete leg;
  
}

// steal from 
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Smurf/Analysis/HWWlvlv/factors.h?revision=1.9&view=markup
double nPUScaleFactor(TH1D *fhDPU, int npu){
  double mynpu = TMath::Min((double)npu,39.499);
  Int_t npuxbin = fhDPU->GetXaxis()->FindBin(mynpu);
  return fhDPU->GetBinContent(npuxbin);
}
