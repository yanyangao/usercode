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
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector; 
double nPUScaleFactor(TH1D *fhDPU, int npu);

void drawnvtx(TString mode = "fall11")
{
  // load macros  
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()"); 
  gROOT->ForceStyle();
  
  TString weightFileName;
  if ( mode == "fall11" ) weightFileName = "PileupReweighting.Fall11DYmm_To_Cert_190456-191859_8TeV_PromptReco_Collisions12_JSON.root";
  if ( mode == "summer12" ) weightFileName = "PileupReweighting.Summer12WW_To_Cert_190456-191859_8TeV_PromptReco_Collisions12_JSON.root";
  
  TFile *fPUFile = TFile::Open(weightFileName);
  TH1D *fhpuWeights = (TH1D*)(fPUFile->Get("puWeights"));
  assert(fhpuWeights);
  fhpuWeights->SetDirectory(0);
  delete fPUFile;

  TH1F *hnvtx_data;
  hnvtx_data = new TH1F("hnvtx_data", "hnvtx_data", 51, -0.5, 50.5);

  TH1F *hnvtx_mc;
  hnvtx_mc = new TH1F("hnvtx_mc", "hnvtx_mc", 51, -0.5, 50.5);
  hnvtx_mc->Sumw2();

  TChain *chain = new TChain("tree");
  chain->Add("/smurf/data/Run2012_Summer12_SmurfV9_51X/mitf-alljets_noweights/data.root");
  if ( mode == "summer12") 
    chain->Add("/smurf/data/Run2012_Summer12_SmurfV9_51X/mitf-alljets_noweights/dyll.root");
  if (mode == "fall11") 
    chain->Add("/smurf/data/Run2011_Fall11_SmurfV9_42X/mitf-alljets/dymm.root");
  assert(chain);
  
  int type_ = 0;
  int dstype_ = 0;
  int nvtx_ = 0;
  int npu_ = 0;
  LorentzVector*  dilep_ = 0;

  chain->SetBranchAddress( "npu"      , &npu_     );      
  chain->SetBranchAddress( "nvtx"     , &nvtx_    );      
  chain->SetBranchAddress( "type"     , &type_    );      
  chain->SetBranchAddress( "dstype"   , &dstype_  );      
  chain->SetBranchAddress( "dilep"    , &dilep_   );      

  for(int ievt = 0; ievt< chain->GetEntries(); ievt++){   
    chain->GetEntry(ievt); 
    if ( type_ == 1 || type_ == 2) continue;
    if ( (dilep_->mass() - 91.1876) > 15. ) continue;
    
    if ( dstype_ == 0 ) {
      hnvtx_data->Fill(nvtx_);
    }
    else {
      float sfWeightPU = nPUScaleFactor(fhpuWeights, npu_);
      hnvtx_mc->Fill(nvtx_, sfWeightPU);
    }
  }
  
  // normalize each histogram to 1
  hnvtx_data->Scale(1./hnvtx_data->Integral(0,1000));
  hnvtx_mc->Scale(1./hnvtx_mc->Integral(0,1000));

  float yMax = hnvtx_data->GetMaximum();
  yMax = yMax > hnvtx_mc->GetMaximum() ? yMax : hnvtx_mc->GetMaximum();
  
  hnvtx_mc->SetLineColor(kBlue);
  hnvtx_mc->SetFillColor(kBlue);
  hnvtx_mc->SetLineWidth(3);
  hnvtx_mc->SetXTitle("Number of Vertices");

  hnvtx_data->SetLineColor(kRed);
  hnvtx_data->SetMarkerColor(kRed);
  hnvtx_data->SetLineWidth(3);
  hnvtx_data->SetXTitle("Number of Vertices");

  TLegend *leg = new TLegend(0.5, 0.6, 0.85, 0.9);
  leg->SetFillColor(0);
  leg->AddEntry(hnvtx_data, "Data", "l");
  leg->AddEntry(hnvtx_mc, "MC", "l");

  TCanvas *c1 = new TCanvas("c1","c1");
  hnvtx_mc->SetMaximum(yMax * 1.1);
  hnvtx_mc->Draw("hist");
  hnvtx_data->Draw("samehist");
  leg->Draw("same");
  c1->SaveAs(Form("pileup_%s.eps", mode.Data()));
  c1->SaveAs(Form("pileup_%s.png", mode.Data()));
  
}

// steal from 
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Smurf/Analysis/HWWlvlv/factors.h?revision=1.9&view=markup
double nPUScaleFactor(TH1D *fhDPU, int npu){
  double mynpu = TMath::Min((double)npu,39.499);
  Int_t npuxbin = fhDPU->GetXaxis()->FindBin(mynpu);
  return fhDPU->GetBinContent(npuxbin);
}
