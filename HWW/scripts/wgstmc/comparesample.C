#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TLegend.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
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

void createPlot(std::vector<int> samples, std::vector<TString> files, std::vector<TString> legend);


TFile *output_file = new TFile("histos_wz_nocuts.root", "RECREATE");
ofstream text; 
enum Sample{
  wgstold  = 1UL<<0,
  wgstnew  = 1UL<<1,
};

void comparesample()
{  
  // load macros  
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()"); 

  // load macros  
  std::vector<int> samples;
  std::vector<TString> files;
  std::vector<TString> legend;
  
  samples.push_back(wgstold);
  files.push_back("wz_gentuple_ee_old.root");
  legend.push_back("pp#rightarrow ln #gamma^{*}");

  samples.push_back(wgstnew);
  files.push_back("wz_gentuple_ee_new_50k.root");
  legend.push_back("pp#rightarrow W#gamma^{*}#rightarrow lnll");
  
  
  createPlot(samples, files, legend);
  

}

void createPlot(std::vector<int> samples, std::vector<TString> files, std::vector<TString> legend)
{

  TString y_title = "Number of Entries";
  const int nHist = files.size(); // number of files

  // Declare the histograms to be saved 
  TH1F *hmll_z[nHist];
  TH1F *hleadleppt_z[nHist];
  TH1F *htrailleppt_z[nHist];
  TH1F *hdrll_z[nHist];
  TH1F *hmlnu_w[nHist];
  TH1F *hleppt_w[nHist];
  TH1F *hlepeta_w[nHist];
  TH1F *hwmass[nHist];
  
  
  // Get the histograms from the ntuples
  for (int i=0;i<nHist;i++) {
    TString treeName =  "Events";
    
    TFile *file = TFile::Open(files[i], "READ");
    TTree *chain = (TTree*) file->Get(treeName);
    assert(chain);
  
    // declare histograms  to fill
    Color_t color = kBlack;
    TString sampleName = "";
    setSample(int(samples[i]), color, sampleName);

    // define the histograms to plot
    
    // dilmass 
    hmll_z[i] = new TH1F(Form("mll_z_%s", sampleName.Data()), Form("mll_z_%s", sampleName.Data()), 50, 0, 100.);
    hmll_z[i]->Sumw2();
    hmll_z[i]->SetLineColor(color);
    hmll_z[i]->SetMarkerColor(color);
    
    // dilmass 
    hdrll_z[i] = new TH1F(Form("drll_z_%s", sampleName.Data()), Form("drll_z_%s", sampleName.Data()), 50, 0, 6.);
    hdrll_z[i]->Sumw2();
    hdrll_z[i]->SetLineColor(color);
    hdrll_z[i]->SetMarkerColor(color);

    // leading lepton pT
    hleadleppt_z[i] = new TH1F(Form("leadleppt_z_%s", sampleName.Data()), Form("leadleppt_z_%s", sampleName.Data()), 50, 0, 100.);
    hleadleppt_z[i]->Sumw2();
    hleadleppt_z[i]->SetLineColor(color);
    hleadleppt_z[i]->SetMarkerColor(color);
    
    // trailing lepton pT
    htrailleppt_z[i] = new TH1F(Form("trailleppt_z_%s", sampleName.Data()), Form("trailleppt_z_%s", sampleName.Data()), 50, 0, 20.);
    htrailleppt_z[i]->Sumw2();
    htrailleppt_z[i]->SetLineColor(color);
    htrailleppt_z[i]->SetMarkerColor(color);

    // mlnu from W
    hmlnu_w[i] = new TH1F(Form("mlnu_w_%s", sampleName.Data()), Form("mlnu_w_%s", sampleName.Data()), 50, 0, 100.);
    hmlnu_w[i]->Sumw2();
    hmlnu_w[i]->SetLineColor(color);
    hmlnu_w[i]->SetMarkerColor(color);

    // w mass 
    hwmass[i] = new TH1F(Form("wmass_%s", sampleName.Data()), Form("wmass_%s", sampleName.Data()), 50, 0, 100.);
    hwmass[i]->Sumw2();
    hwmass[i]->SetLineColor(color);
    hwmass[i]->SetMarkerColor(color);

    // pt of the lepton from W
    hleppt_w[i] = new TH1F(Form("leppt_w_%s", sampleName.Data()), Form("leppt_w_%s", sampleName.Data()), 50, 0, 100.);
    hleppt_w[i]->Sumw2();
    hleppt_w[i]->SetLineColor(color);
    hleppt_w[i]->SetMarkerColor(color);

    // pt of the lepton from W
    hlepeta_w[i] = new TH1F(Form("lepeta_w_%s", sampleName.Data()), Form("lepeta_w_%s", sampleName.Data()), 50, -2.5, 2.5);
    hlepeta_w[i]->Sumw2();
    hlepeta_w[i]->SetLineColor(color);
    hlepeta_w[i]->SetMarkerColor(color);
    
    
    std::cout  << "Processing " << chain->GetEntries() << " entries. \n";
    
    chain->Project(Form("mll_z_%s", sampleName.Data()), "TMath::Min(zllmass,99.99)", "weight");
    chain->Project(Form("drll_z_%s", sampleName.Data()), "sqrt(pow(eta_zlepton1-eta_zlepton2, 2)+pow(phi_zlepton1-phi_zlepton2, 2))", "weight");
    chain->Project(Form("leadleppt_z_%s", sampleName.Data()), "TMath::Min(TMath::Max(pt_zlepton1, pt_zlepton2), 99.99)", "weight");
    chain->Project(Form("trailleppt_z_%s", sampleName.Data()), "TMath::Min(TMath::Min(pt_zlepton1, pt_zlepton2), 19.99)", "weight");
    chain->Project(Form("mlnu_w_%s", sampleName.Data()), "TMath::Min(wlnumass, 99.99)", "weight");
    chain->Project(Form("wmass_%s", sampleName.Data()), "TMath::Min(wmass, 99.99)", "weight");
    chain->Project(Form("leppt_w_%s", sampleName.Data()), "TMath::Min(pt_wlepton, 99.99)", "weight");
    chain->Project(Form("lepeta_w_%s", sampleName.Data()), "TMath::Min(TMath::Max(eta_wlepton, -2.499), 2.499)", "weight");
  }
  
  
  
  output_file->cd();
  
  for(int i=0;i<nHist;i++) {
    hmll_z[i]->Write();
    hdrll_z[i]->Write();
    hleadleppt_z[i]->Write();
    htrailleppt_z[i]->Write();
    hmlnu_w[i]->Write();
    hleppt_w[i]->Write();
    hlepeta_w[i]->Write();
    hwmass[i]->Write();
  }
  
  // tidy up
  
  for ( int i = 0; i<nHist;i++) {
    delete hmll_z[i];
    delete hdrll_z[i];
    delete hleadleppt_z[i];
    delete htrailleppt_z[i];
    delete hmlnu_w[i];
    delete hleppt_w[i];
    delete hlepeta_w[i];
    delete hwmass[i];
  }

}

void setSample(int sample, Color_t & color, TString & sampleName)
{
  switch ( sample) {
  case wgstold:
    color =  kBlack;
    sampleName = "old";
    break;
  case wgstnew:
    color =  kBlue;
    sampleName = "new";
    break;
  default:
    color =  kBlack;
    break;
  }
}

