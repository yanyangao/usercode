// Make plots from histograms
#include "TFile.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include <iostream>
#include <stdlib.h>
#include "Math/LorentzVector.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TArrow.h"
#include "goodrun.cc"


typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector; 

//const Double_t jetPtbins[14] = {0,5,15,20,25,30,40,50,60,80,100,120,150,200};
const Double_t jetPtbins[19] = {0,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,100};
const int nBins = 18;

const Double_t vtxbins[15] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 30, 40};
const int nvtxBins = 14;

//const Double_t vtxbins[7] = {0, 5, 10, 15, 20, 25, 30};
//const int nvtxBins = 6;


TFile *output_file = new TFile("Zeff_nojetmva.root", "RECREATE");

void createPlot(std::vector<int> samples, std::vector<TString> names,  std::vector<TString> legends, TString x_title, bool logy, float xMin, float xMax, int rebin);
void getEff(TH1F*  hist, TH1F* hist_eff);
void getEff2Hist(TH1F* hist_num, TH1F* hist_denum, TH1F* hist_eff);
Color_t SetColor(int sample);
double nPUScaleFactor(TH1D *fhDPU, int npu);


// steal from ../../Core/SmurfTree.h
enum Selection {
  BaseLine          = 1UL<<0,  // pt(reco)>20/10, acceptance,!STA muon, mll>12
  ChargeMatch       = 1UL<<1,  // q1*q2<0
  Lep1FullSelection = 1UL<<2,  // full id, isolation, d0, dz etc
  Lep1LooseEleV1    = 1UL<<3,  // electron fakeable object selection is passed V1
  Lep1LooseEleV2    = 1UL<<4,  // electron fakeable object selection is passed V2
  Lep1LooseEleV3    = 1UL<<5,  // electron fakeable object selection is passed V3
  Lep1LooseEleV4    = 1UL<<6,  // electron fakeable object selection is passed V4
  Lep1LooseMuV1     = 1UL<<7,  // muon fakeable object selection (relIso<1.0)
  Lep1LooseMuV2     = 1UL<<8,  // muon fakeable object selection (relIso<0.4)
  Lep2FullSelection = 1UL<<9,  // full id, isolation, d0, dz etc
  Lep2LooseEleV1    = 1UL<<10, // electron fakeable object selection is passed V1
  Lep2LooseEleV2    = 1UL<<11, // electron fakeable object selection is passed V2
  Lep2LooseEleV3    = 1UL<<12, // electron fakeable object selection is passed V3
  Lep2LooseEleV4    = 1UL<<13, // electron fakeable object selection is passed V4
  Lep2LooseMuV1     = 1UL<<14, // muon fakeable object selection (relIso<1.0)
  Lep2LooseMuV2     = 1UL<<15, // muon fakeable object selection (relIso<0.4)
  FullMET           = 1UL<<16, // full met selection
  ZVeto             = 1UL<<17, // event is not in the Z-mass peak for ee/mm final states
  TopTag            = 1UL<<18, // soft muon and b-jet tagging for the whole event regardless of n-jets (non-zero means    tagged)
  TopVeto           = 1UL<<19, // soft muon and b-jet tagging for the whole event regardless of n-jets (zero means tag ged)
  OneBJet           = 1UL<<20, // 1-jet events, where the jet is b-tagged (top control sample with one b-quark missing )
  TopTagNotInJets   = 1UL<<21, // soft muon and b-jet tagging for areas outside primary jets (non-zero means tagged)~
  ExtraLeptonVeto   = 1UL<<22, // extra lepton veto, DR(muon-electron)>=0.3
  Lep3FullSelection = 1UL<<23,  // full id, isolation, d0, dz etc
  Lep3LooseEleV1    = 1UL<<24, // electron fakeable object selection is passed V1
  Lep3LooseEleV2    = 1UL<<25, // electron fakeable object selection is passed V2
  Lep3LooseEleV3    = 1UL<<26, // electron fakeable object selection is passed V3
  Lep3LooseEleV4    = 1UL<<27, // electron fakeable object selection is passed V4
  Lep3LooseMuV1     = 1UL<<28, // muon fakeable object selection (relIso<1.0)
  Lep3LooseMuV2     = 1UL<<29  // muon fakeable object selection (relIso<0.4)
};

// ww-preselection information available through bits (extra cuts are needed)
// http://www.t2.ucsd.edu/tastwiki/bin/view/Smurf/ReferenceSelections#SmurfWW_V6_with_42X
UInt_t ww = BaseLine|ChargeMatch|Lep1FullSelection|Lep2FullSelection|FullMET|ZVeto|TopVeto|ExtraLeptonVeto;
UInt_t ww_baseline = BaseLine|ChargeMatch|Lep1FullSelection|Lep2FullSelection|ExtraLeptonVeto|TopVeto;

enum Samples{
  zdata       = 1UL<<0,
  zpowheg     = 1UL<<1,
  zmadgraph   = 1UL<<2,
};


void getZEff()
{

  // load macros  
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()"); 
  gROOT->ProcessLine(".L goodrun.h++");
  set_goodrun_file("DCSOnly_2012A_160412.jmu");

  std::vector<int> samples;
  std::vector<TString> names;
  std::vector<TString> legends;
  
  samples.push_back(zdata);
  names.push_back("data");
  legends.push_back("2012 Data");

  samples.push_back(zmadgraph);
  names.push_back("madgraph");
  legends.push_back("Madgraph MC");

  std::cout << ww_baseline << "\n";


  float xMin(10);
  float xMax(100);
  int REBIN(1);
  
  createPlot(samples, names, legends, "Max PFJet Pt (GeV)", false, xMin, xMax, REBIN);
  
}

void createPlot(std::vector<int> samples, std::vector<TString> names, std::vector<TString> legends, TString x_title, bool logy, float xMin, float xMax, int rebin)
{

  TString y_title = "Number of Events";
  const int nHist = samples.size();

  // these are histograms w.r.t. jet pT
  TH1F *hist[nHist][4];
  TH1F *eff[nHist][4];
  TH1F *hist_mm[nHist][4];
  TH1F *eff_mm[nHist][4];
  TH1F *hist_ee[nHist][4];
  TH1F *eff_ee[nHist][4];
  
  // these are histograms w.r.t. vtx

  TH1F *hnvtx[nHist];
  TH1F *hnvtx_ee[nHist];
  TH1F *hnvtx_mm[nHist];

  TH1F *hnvtx_0j[nHist];
  TH1F *hnvtx_0j_ee[nHist];
  TH1F *hnvtx_0j_mm[nHist];

  TH1F *effvsnvtx[nHist];
  TH1F *effvsnvtx_ee[nHist];
  TH1F *effvsnvtx_mm[nHist];

  for(int i=0;i<nHist;i++) {
  
    // get the chain
    TChain *chain = new TChain("tree");
    if ( samples[i] & zdata)
      chain->Add(TString("data/data_nojetmva.root"));
    //chain->Add(TString("data/data.root"));
    if ( samples[i] & zpowheg) {
      chain->Add(TString("data/dyee_mg.root"));
      chain->Add(TString("data/dymm_mg.root"));

    }
    if ( samples[i] & zmadgraph) {
      // chain->Add(TString("data/dyll.root"));
      chain->Add(TString("data/dyee_mg_52X_nometcut.root"));
      chain->Add(TString("data/dymm_mg_52X_nometcut.root"));
    }
    assert(chain);
    
    // define the jet pt histogram and efficiencies
    for ( int j = 0; j < 4; j++) {

      hist[i][j] = new TH1F(Form("jetpt_%s_vtxbin%i", names[i].Data(), j), Form("jetpt_%s_vtxbin%i", names[i].Data(), j), nBins, jetPtbins);
      eff[i][j] = new TH1F(Form("jetpteff_%s_vtxbin%i", names[i].Data(), j), Form("jetpteff_%s_vtxbin%i", names[i].Data(), j), nBins, jetPtbins);

      hist_mm[i][j] = new TH1F(Form("jetpt_%s_vtxbin%i_mm", names[i].Data(), j), Form("jetpt_%s_vtxbin%i_mm", names[i].Data(), j), nBins, jetPtbins);
      eff_mm[i][j] = new TH1F(Form("jetpteff_%s_vtxbin%i_mm", names[i].Data(), j), Form("jetpteff_%s_vtxbin%i_mm", names[i].Data(), j), nBins, jetPtbins);

      hist_ee[i][j] = new TH1F(Form("jetpt_%s_vtxbin%i_ee", names[i].Data(), j), Form("jetpt_%s_vtxbin%i_ee", names[i].Data(), j), nBins, jetPtbins);
      eff_ee[i][j] = new TH1F(Form("jetpteff_%s_vtxbin%i_ee", names[i].Data(), j), Form("jetpteff_%s_vtxbin%i_ee", names[i].Data(), j), nBins, jetPtbins);
      
      hist[i][j]->Sumw2();
      hist_mm[i][j]->Sumw2();
      hist_ee[i][j]->Sumw2();
    }


    // histograms for jet veto efficiencies vs nvtx
    hnvtx[i] = new TH1F(Form("nvtx_%s", names[i].Data()), Form("nvtx_%s", names[i].Data()), nvtxBins, vtxbins);
    hnvtx_ee[i] = new TH1F(Form("nvtx_ee_%s", names[i].Data()), Form("nvtx_ee_%s", names[i].Data()), nvtxBins, vtxbins);
    hnvtx_mm[i] = new TH1F(Form("nvtx_mm_%s", names[i].Data()), Form("nvtx_mm_%s", names[i].Data()), nvtxBins, vtxbins);

    hnvtx_0j[i] = new TH1F(Form("nvtx_%s_0j", names[i].Data()), Form("nvtx_%s_0j", names[i].Data()), nvtxBins, vtxbins);
    hnvtx_0j_ee[i] = new TH1F(Form("nvtx_ee_%s_0j", names[i].Data()), Form("nvtx_ee_%s_0j", names[i].Data()), nvtxBins, vtxbins);
    hnvtx_0j_mm[i] = new TH1F(Form("nvtx_mm_%s_0j", names[i].Data()), Form("nvtx_mm_%s_0j", names[i].Data()), nvtxBins, vtxbins);

    effvsnvtx[i] = new TH1F(Form("effvsnvtx_%s", names[i].Data()), Form("effvsnvtx_%s", names[i].Data()), nvtxBins, vtxbins);
    effvsnvtx_ee[i] = new TH1F(Form("effvsnvtx_ee_%s", names[i].Data()), Form("effvsnvtx_ee_%s", names[i].Data()), nvtxBins, vtxbins);
    effvsnvtx_mm[i] = new TH1F(Form("effvsnvtx_mm_%s", names[i].Data()), Form("effvsnvtx_mm_%s", names[i].Data()), nvtxBins, vtxbins);


    // fill the jet pT histogram 
    LorentzVector*  jet1_ = 0;  
    float scale1fb_ = 1.;
    int type_ = 0;
    LorentzVector*  dilep_ = 0;
    LorentzVector*  lep2_ = 0;
    float sfWeightTrig_ = 1.;
    float sfWeightEff_ = 1.;  
    int dstype_ = 0;
    unsigned int cuts_ = 0;
    float pmet_ = 0;
    float pTrackMet_ = 0;
    int nvtx_ = 0;
    int npu_ = 0;
    unsigned int run_ = 0;
    unsigned int lumi_ = 0;
    unsigned int njets_ = 0;

    chain->SetBranchAddress( "jet1", &jet1_);
    chain->SetBranchAddress( "scale1fb",  &scale1fb_);
    chain->SetBranchAddress( "type",  &type_);
    chain->SetBranchAddress( "dilep", &dilep_);
    chain->SetBranchAddress( "lep2", &lep2_);
    chain->SetBranchAddress( "dstype",  &dstype_);
    chain->SetBranchAddress( "cuts"      , &cuts_     );     
    chain->SetBranchAddress( "pmet"      , &pmet_     );     
    chain->SetBranchAddress( "pTrackMet"      , &pTrackMet_     );     
    chain->SetBranchAddress( "nvtx"      , &nvtx_     );     
    chain->SetBranchAddress( "npu"      , &npu_     );     
    if ( chain->GetBranchStatus("sfWeightTrig"))
      chain->SetBranchAddress( "sfWeightTrig",  &sfWeightTrig_); 
    if ( chain->GetBranchStatus("sfWeightEff"))
      chain->SetBranchAddress( "sfWeightEff",  &sfWeightEff_); 
    chain->SetBranchAddress( "run"      , &run_     );     
    chain->SetBranchAddress( "lumi"      , &lumi_     );     
    chain->SetBranchAddress( "njets"      , &njets_     );     

    for(int ievt = 0; ievt< chain->GetEntries(); ievt++){   
      chain->GetEntry(ievt);    
      
      if ((cuts_ & ww_baseline) != ww_baseline) continue;
      if ( fabs(dilep_->mass() - 91.1875 ) >= 7.5 ) continue;
      if ( lep2_->Pt() < 15 ) continue;
      // if ( abs(jet1_->Eta()) > 3.0) continue;

      float weight = 1.0;
      float weight_ee = 1.0;
      float weight_mm = 1.0;
      
      // apply good run list 
      if ( dstype_ == 0 ) {
	if ( ! goodrun(run_, lumi_) ) continue;
	
      } else {
	weight *= scale1fb_;
	weight_ee *= scale1fb_;
	weight_mm *= scale1fb_;
      }
      
      // 
      // fill the jet veto efficiency vs nvtx plots 
      //
      hnvtx[i]->Fill(nvtx_, weight);
      if ( type_ == 0 ) 
	hnvtx_mm[i]->Fill(nvtx_, weight_mm);
      if ( type_ == 3 ) 
	hnvtx_ee[i]->Fill(nvtx_, weight_ee);
      
      if ( njets_ == 0 ) {
	hnvtx_0j[i]->Fill(nvtx_, weight);
	if ( type_ == 0 ) 
	  hnvtx_0j_mm[i]->Fill(nvtx_, weight_mm);
	if ( type_ == 3 ) 
	  hnvtx_0j_ee[i]->Fill(nvtx_, weight_ee);
      }
      
      //
      // fill the jet veto efficiency vs jet pT plots
      // 
      hist[i][0]->Fill(jet1_->Pt(), weight);      
      if ( nvtx_ <= 10 ) {	  
	hist[i][1]->Fill(jet1_->Pt(), weight);
      } else if ( nvtx_ > 10 && nvtx_ < 20 ) {
	hist[i][2]->Fill(jet1_->Pt(), weight);
      } else if ( nvtx_ >= 20 && nvtx_ < 30 ) {
      	hist[i][3]->Fill(jet1_->Pt(), weight);
      }

      // fill the mm plots
      if ( type_ == 0) {
	hist_mm[i][0]->Fill(jet1_->Pt(), weight_mm);      
	if ( nvtx_ <= 10 ) {	  
	  hist_mm[i][1]->Fill(jet1_->Pt(), weight_mm);
	} else if ( nvtx_ > 10 && nvtx_ < 20 ) {
	  hist_mm[i][2]->Fill(jet1_->Pt(), weight_mm);
	} else if ( nvtx_ >= 20 && nvtx_ < 30 ) {
	  hist_mm[i][3]->Fill(jet1_->Pt(), weight_mm);
	}
      }
      
      // fill the ee plots
      if ( type_ == 3) {
	hist_ee[i][0]->Fill(jet1_->Pt(), weight_ee);      
	if ( nvtx_ <= 10 ) {	  
	  hist_ee[i][1]->Fill(jet1_->Pt(), weight_ee);
	} else if ( nvtx_ > 10 && nvtx_ < 20 ) {
	  hist_ee[i][2]->Fill(jet1_->Pt(), weight_ee);
	} else if ( nvtx_ >= 20 && nvtx_ < 30 ) {
	  hist_ee[i][3]->Fill(jet1_->Pt(), weight_ee);
	}
      }
    }
    // tidy up
    delete chain;
  }

      
  // do the efficiency calclations 
  for ( int i = 0; i < nHist; i++) {
    getEff2Hist(hnvtx_0j[i], hnvtx[i], effvsnvtx[i]);
    getEff2Hist(hnvtx_0j_ee[i], hnvtx_ee[i], effvsnvtx_ee[i]);
    getEff2Hist(hnvtx_0j_mm[i], hnvtx_mm[i], effvsnvtx_mm[i]);
    for ( int j = 0 ; j < 4; j++) {
      getEff(hist[i][j], eff[i][j]);
      getEff(hist_mm[i][j], eff_mm[i][j]);
      getEff(hist_ee[i][j], eff_ee[i][j]);
    }
  }
  
  // arrange the plot ranges/qualities
  TLegend *leg = new TLegend(0.5, 0.4, 0.85, 0.55);
  leg->SetFillColor(0);
  leg->SetTextSize(0.04);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetShadowColor(0);
  
  
  // plot control
  for ( int i = 0; i < nHist; i++ ) {  

    effvsnvtx[i]->GetYaxis()->SetRangeUser(0,1.1);
    effvsnvtx_mm[i]->GetYaxis()->SetRangeUser(0,1.1);
    effvsnvtx_ee[i]->GetYaxis()->SetRangeUser(0,1.1);
      
    effvsnvtx[i]->SetXTitle("Number of Vertices");   
    effvsnvtx_ee[i]->SetXTitle("Number of Vertices");
    effvsnvtx_mm[i]->SetXTitle("Number of Vertices");

    effvsnvtx[i]->SetYTitle("Jet Veto efficiency");
    effvsnvtx_ee[i]->SetYTitle("Jet Veto efficiency");
    effvsnvtx_mm[i]->SetYTitle("Jet Veto efficiency");

    effvsnvtx[i]->SetLineColor(SetColor(samples[i]));
    effvsnvtx[i]->SetMarkerColor(SetColor(samples[i]));
    effvsnvtx_ee[i]->SetLineColor(SetColor(samples[i]));
    effvsnvtx_ee[i]->SetMarkerColor(SetColor(samples[i]));
    effvsnvtx_mm[i]->SetLineColor(SetColor(samples[i]));
    effvsnvtx_mm[i]->SetMarkerColor(SetColor(samples[i]));

    hnvtx[i]->SetLineColor(SetColor(samples[i]));
    hnvtx[i]->SetMarkerColor(SetColor(samples[i]));
    hnvtx_ee[i]->SetLineColor(SetColor(samples[i]));
    hnvtx_ee[i]->SetMarkerColor(SetColor(samples[i]));
    hnvtx_mm[i]->SetLineColor(SetColor(samples[i]));
    hnvtx_mm[i]->SetMarkerColor(SetColor(samples[i]));

    hnvtx_0j[i]->SetLineColor(SetColor(samples[i]));
    hnvtx_0j[i]->SetMarkerColor(SetColor(samples[i]));
    hnvtx_0j_ee[i]->SetLineColor(SetColor(samples[i]));
    hnvtx_0j_ee[i]->SetMarkerColor(SetColor(samples[i]));
    hnvtx_0j_mm[i]->SetLineColor(SetColor(samples[i]));
    hnvtx_0j_mm[i]->SetMarkerColor(SetColor(samples[i]));



    for ( int j = 0; j < 4; j++) {
      // set the overflow bin to the last bin content
      int maxbin = hist[i][j]->GetNbinsX();
      hist[i][j]->SetBinContent(maxbin, hist[i][j]->GetBinContent(maxbin)+hist[i][j]->GetBinContent(maxbin+1));
      hist[i][j]->SetBinError(maxbin, sqrt(pow(hist[i][j]->GetBinError(maxbin),2)+pow(hist[i][j]->GetBinError(maxbin+1),2)));
      hist[i][j]->SetBinContent(1, hist[i][j]->GetBinContent(1)+hist[i][j]->GetBinContent(0));
      hist[i][j]->SetBinError(1, sqrt(pow(hist[i][j]->GetBinError(1),2)+pow(hist[i][j]->GetBinError(0),2)));

      hist_mm[i][j]->SetBinContent(maxbin, hist_mm[i][j]->GetBinContent(maxbin)+hist_mm[i][j]->GetBinContent(maxbin+1));
      hist_mm[i][j]->SetBinError(maxbin, sqrt(pow(hist_mm[i][j]->GetBinError(maxbin),2)+pow(hist_mm[i][j]->GetBinError(maxbin+1),2)));
      hist_mm[i][j]->SetBinContent(1, hist_mm[i][j]->GetBinContent(1)+hist_mm[i][j]->GetBinContent(0));
      hist_mm[i][j]->SetBinError(1, sqrt(pow(hist_mm[i][j]->GetBinError(1),2)+pow(hist_mm[i][j]->GetBinError(0),2)));

      hist_ee[i][j]->SetBinContent(maxbin, hist_ee[i][j]->GetBinContent(maxbin)+hist_ee[i][j]->GetBinContent(maxbin+1));
      hist_ee[i][j]->SetBinError(maxbin, sqrt(pow(hist_ee[i][j]->GetBinError(maxbin),2)+pow(hist_ee[i][j]->GetBinError(maxbin+1),2)));
      hist_ee[i][j]->SetBinContent(1, hist_ee[i][j]->GetBinContent(1)+hist_ee[i][j]->GetBinContent(0));
      hist_ee[i][j]->SetBinError(1, sqrt(pow(hist_ee[i][j]->GetBinError(1),2)+pow(hist_ee[i][j]->GetBinError(0),2)));

      // ranges
      eff[i][j]->GetYaxis()->SetRangeUser(0,1.1);
      eff_mm[i][j]->GetYaxis()->SetRangeUser(0,1.1);
      eff_ee[i][j]->GetYaxis()->SetRangeUser(0,1.1);
      
      // titles
      hist[i][j]->GetXaxis()->SetTitle(x_title);
      hist[i][j]->GetYaxis()->SetTitle("Number of Entries");
      hist_mm[i][j]->GetXaxis()->SetTitle(x_title);
      hist_mm[i][j]->GetYaxis()->SetTitle("Number of Entries");
      hist_ee[i][j]->GetXaxis()->SetTitle(x_title);
      hist_ee[i][j]->GetYaxis()->SetTitle("Number of Entries");

      eff[i][j]->GetXaxis()->SetTitle(x_title);
      eff[i][j]->GetYaxis()->SetTitle("Jet Veto Efficiency");
      eff_mm[i][j]->GetXaxis()->SetTitle(x_title);
      eff_mm[i][j]->GetYaxis()->SetTitle("Jet Veto Efficiency");
      eff_ee[i][j]->GetXaxis()->SetTitle(x_title);
      eff_ee[i][j]->GetYaxis()->SetTitle("Jet Veto Efficiency");
      // get the color setup
      hist[i][j]->SetLineColor(SetColor(samples[i]));
      hist[i][j]->SetMarkerColor(SetColor(samples[i]));
      hist_mm[i][j]->SetLineColor(SetColor(samples[i]));
      hist_mm[i][j]->SetMarkerColor(SetColor(samples[i]));
      hist_ee[i][j]->SetLineColor(SetColor(samples[i]));
      hist_ee[i][j]->SetMarkerColor(SetColor(samples[i]));

      eff[i][j]->SetLineColor(SetColor(samples[i]));
      eff[i][j]->SetMarkerColor(SetColor(samples[i]));
      eff_mm[i][j]->SetLineColor(SetColor(samples[i]));
      eff_mm[i][j]->SetMarkerColor(SetColor(samples[i]));
      eff_ee[i][j]->SetLineColor(SetColor(samples[i]));
      eff_ee[i][j]->SetMarkerColor(SetColor(samples[i]));
    }
    leg->AddEntry(hist[i][0], legends[i]);
  }
  
  // do plot 

  // make the histgrams scale to the entries
  float yMax_ee = 0.;
  float yMax_mm = 0.;
  for ( int i = 0; i < nHist ; i++ ) {
    hist_ee[i][0]->Scale(1./hist_ee[i][0]->Integral(0,1000));
    yMax_ee = yMax_ee > hist_ee[i][0]->GetMaximum() ? yMax_ee : hist_ee[i][0]->GetMaximum();
    hist_mm[i][0]->Scale(1./hist_mm[i][0]->Integral(0,1000));
    yMax_mm = yMax_mm > hist_mm[i][0]->GetMaximum() ? yMax_mm : hist_mm[i][0]->GetMaximum();
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  hist_ee[0][0]->SetMaximum(yMax_ee * 1.1);
  hist_ee[0][0]->Draw("h");
  for(int i=1;i<nHist;i++)
    hist_ee[i][0]->Draw("sameh");
  leg->Draw("same");
  c1->Print(TString("epsfiles/ZjetpT_ee.eps"));
  c1->Print(TString("epsfiles/ZjetpT_ee.png"));

  c1->Clear();
  eff_ee[0][0]->Draw("h");
  for(int i=1;i<nHist;i++)
    eff_ee[i][0]->Draw("sameh");
  leg->Draw("same");
  c1->Print(TString("epsfiles/Zjetvetoeff_ee.eps"));
  c1->Print(TString("epsfiles/Zjetvetoeff_ee.png"));

  c1->Clear();
  hist_mm[0][0]->SetMaximum(yMax_mm * 1.1);
  hist_mm[0][0]->SetMaximum(hist_mm[0][0]->GetMaximum()*1.2);
  hist_mm[0][0]->Draw("h");
  for(int i=1;i<nHist;i++)
    hist_mm[i][0]->Draw("sameh");
  leg->Draw("same");
  c1->Print(TString("epsfiles/ZjetpT_mm.eps"));
  c1->Print(TString("epsfiles/ZjetpT_mm.png"));

  
  c1->Clear();
  eff_mm[0][0]->Draw("h");
  for(int i=1;i<nHist;i++)
    eff_mm[i][0]->Draw("sameh");
  leg->Draw("same");
  c1->Print(TString("epsfiles/Zjetvetoeff_mm.eps"));
  c1->Print(TString("epsfiles/Zjetvetoeff_mm.png"));
  
  
  c1->Clear();
  effvsnvtx[0]->Draw("h");
  for(int i=1;i<nHist;i++)
    effvsnvtx[i]->Draw("samee");
  leg->Draw("same");
  c1->Print(TString("epsfiles/Zjetvetoeff_vs_nvtx.eps"));
  c1->Print(TString("epsfiles/Zjetvetoeff_vs_nvtx.png"));

  c1->Clear();
  effvsnvtx_ee[0]->Draw("h");
  for(int i=1;i<nHist;i++)
    effvsnvtx_ee[i]->Draw("samee");
  leg->Draw("same");
  c1->Print(TString("epsfiles/Zjetvetoeff_vs_nvtx_ee.eps"));
  c1->Print(TString("epsfiles/Zjetvetoeff_vs_nvtx_ee.png"));

  c1->Clear();
  effvsnvtx_mm[0]->Draw("h");
  for(int i=1;i<nHist;i++)
    effvsnvtx_mm[i]->Draw("samee");
  leg->Draw("same");
  c1->Print(TString("epsfiles/Zjetvetoeff_vs_nvtx_mm.eps"));
  c1->Print(TString("epsfiles/Zjetvetoeff_vs_nvtx_mm.png"));

  
  // tidy up
  delete c1;
  delete leg;
  // save plots in histogram
  output_file->cd();
  for(int i = 0; i < nHist; i++) {
    for ( int j = 0; j < 4; j++) {
      hist[i][j]->Write();
      eff[i][j]->Write();
      hist_mm[i][j]->Write();
      eff_mm[i][j]->Write();
      hist_ee[i][j]->Write();
      eff_ee[i][j]->Write();
    }
    hnvtx[i]->Write();
    hnvtx_ee[i]->Write();
    hnvtx_mm[i]->Write();
    hnvtx_0j[i]->Write();
    hnvtx_0j_ee[i]->Write();
    hnvtx_0j_mm[i]->Write();
    effvsnvtx[i]->Write();
    effvsnvtx_ee[i]->Write();
    effvsnvtx_mm[i]->Write();
  }
  

  for(int i = 0; i < nHist; i++) {
    delete hnvtx[i];
    delete hnvtx_ee[i];
    delete hnvtx_mm[i];
    delete hnvtx_0j[i];
    delete hnvtx_0j_ee[i];
    delete hnvtx_0j_mm[i];
    delete effvsnvtx[i];
    delete effvsnvtx_ee[i];
    delete effvsnvtx_mm[i];

    for ( int j = 0; j < 4; j++) {
      delete hist[i][j];
      delete eff[i][j];
      delete hist_mm[i][j];
      delete eff_mm[i][j];
      delete hist_ee[i][j];
      delete eff_ee[i][j];
    }
  }
}


Color_t SetColor(int sample)
{
  switch ( sample) {
  case zdata:
    return kRed;
    break;
  case zpowheg:
    return kRed;
    break;
  case zmadgraph:
    return kBlack;
    break;
  default:
    return kBlack;
    break;
  }  
}

void getEff(TH1F*  hist, TH1F* hist_eff) {
  for (int i=1;i<hist->GetNbinsX()+1;i++) {
    Double_t num = hist->Integral(0, i);
    double denum = hist->Integral(0, hist->GetNbinsX()+1);
    Double_t eff = num/denum;
    hist_eff->SetBinContent(i,eff);
    hist_eff->SetBinError( i, sqrt(eff*(1-eff)/num));
  }
}

void getEff2Hist(TH1F* hist_num, TH1F* hist_denum, TH1F* hist_eff) {
  for (int i=1;i<hist_num->GetNbinsX()+1;i++) {
    Double_t num = hist_num->GetBinContent(i);
    double denum = hist_denum->GetBinContent(i);
    Double_t eff = denum > 0 ? num/denum : 0;
    Double_t efferr = num > 0 ? sqrt(eff*(1-eff)/num) : 0.;
    hist_eff->SetBinContent(i,eff);
    hist_eff->SetBinError( i, efferr);
  }
}

// steal from 
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Smurf/Analysis/HWWlvlv/factors.h?revision=1.9&view=markup
double nPUScaleFactor(TH1D *fhDPU, int npu){
  double mynpu = TMath::Min((double)npu,39.499);
  Int_t npuxbin = fhDPU->GetXaxis()->FindBin(mynpu);
  return fhDPU->GetBinContent(npuxbin);
}
