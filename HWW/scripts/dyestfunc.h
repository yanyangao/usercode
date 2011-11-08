#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include <iostream>
#include <fstream>
#include "TH2F.h"
#include "TH1F.h"
#include "TString.h"
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
#include "TRint.h"
#include "TChain.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TAxis.h"
#include "TMath.h"
#include "TCut.h"

TCut c_zeemc("((lep1MotherMcId==23&&lep2MotherMcId==23&&dstype!=46)||dstype==46)");
TCut c_zmmmc("((lep1MotherMcId==23&&lep2MotherMcId==23&&dstype!=45)||dstype==45)");
TCut c_zwindow("c_zwindow", "abs(dilep.M()-91.1876)<7.5");
TCut c_higgsextra("c_higgsextra", "(jet1.Pt()<15||dPhiDiLepJet1<165*TMath::Pi()/180.)");

// declare the R histograms
const int nbins= 4;  
const float bins [5] = {20, 25, 30, 37, 50};

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
  TopTag            = 1UL<<18, // soft muon and b-jet tagging for the whole event regardless of n-jets (non-zero means tagged)
  TopVeto           = 1UL<<19, // soft muon and b-jet tagging for the whole event regardless of n-jets (zero means tagged)
  OneBJet           = 1UL<<20, // 1-jet events, where the jet is b-tagged (top control sample with one b-quark missing)
  TopTagNotInJets   = 1UL<<21, // soft muon and b-jet tagging for areas outside primary jets (non-zero means tagged)
  ExtraLeptonVeto   = 1UL<<22, // extra lepton veto, DR(muon-electron)>=0.3
  Lep3FullSelection = 1UL<<23,  // full id, isolation, d0, dz etc
  Lep3LooseEleV1    = 1UL<<24, // electron fakeable object selection is passed V1
  Lep3LooseEleV2    = 1UL<<25, // electron fakeable object selection is passed V2
  Lep3LooseEleV3    = 1UL<<26, // electron fakeable object selection is passed V3
  Lep3LooseEleV4    = 1UL<<27, // electron fakeable object selection is passed V4
  Lep3LooseMuV1     = 1UL<<28, // muon fakeable object selection (relIso<1.0)
  Lep3LooseMuV2     = 1UL<<29  // muon fakeable object selection (relIso<0.4)
};

UInt_t ww = BaseLine|ChargeMatch|Lep1FullSelection|Lep2FullSelection|FullMET|ZVeto|TopVeto|ExtraLeptonVeto;
UInt_t ww_nomet = BaseLine|ChargeMatch|Lep1FullSelection|Lep2FullSelection|ZVeto|TopVeto|ExtraLeptonVeto;
UInt_t ww_nozveto = BaseLine|ChargeMatch|Lep1FullSelection|Lep2FullSelection|FullMET|TopVeto|ExtraLeptonVeto;
UInt_t ww_nozveto_nomet = BaseLine|ChargeMatch|Lep1FullSelection|Lep2FullSelection|TopVeto|ExtraLeptonVeto;

bool higgsptcut(int mH, float pt1, float pt2)
{
  switch (mH) {
  case 115: 
  case 120: 
    return   (pt1>20 && pt2>10);
    break;
  case 130: 
    return   (pt1>25 && pt2>10);
    break;
  case 140: 
    return   (pt1>25 && pt2>15);
    break;
  case 150: 
    return   (pt1>27 && pt2>25);
    break;
  case 160: 
    return   (pt1>30 && pt2>25);
    break;
  case 170: 
    return   (pt1>34 && pt2>25);
    break;
  case 180: 
    return   (pt1>36 && pt2>25);
    break;
  case 190: 
    return   (pt1>38 && pt2>25);
    break;
  case 200: 
    return   (pt1>40 && pt2>25);
    break;
  case 250: 
    return   (pt1>55 && pt2>25);
    break;
  case 300: 
    return   (pt1>70 && pt2>25);
    break;
  case 350: 
    return   (pt1>80 && pt2>25);
    break;
  case 400: 
    return   (pt1>90 && pt2>25);
    break;
  case 450: 
    return   (pt1>110 && pt2>25);
    break;
  case 500: 
    return   (pt1>120 && pt2>25);
    break;
  case 550: 
    return   (pt1>130 && pt2>25);
    break;
  case 600: 
    return   (pt1>140 && pt2>25);
    break;
  default:
    return  (pt1>20 && pt2>10);
    break;
  }
}

bool higgsdphicut(int mH, float dphi)
{
  switch (mH) {
  case 115: 
  case 120: 
    return   (dphi<115.*TMath::Pi()/180.);
    break;
  case 130: 
  case 140: 
  case 150: 
    return   (dphi<90.*TMath::Pi()/180.);
    break;
 case 160: 
  case 170: 
    return   (dphi<60.*TMath::Pi()/180.);
    break;
  case 180: 
    return   (dphi<70.*TMath::Pi()/180.);
    break;
  case 190: 
    return  (dphi<90.*TMath::Pi()/180.);
    break;
  case 200: 
    return  (dphi<100.*TMath::Pi()/180.);
    break;
  case 250: 
    return  (dphi<140.*TMath::Pi()/180.);
    break;
  case 300: 
  case 350: 
  case 400:   
  case 450:
  case 500:    
  case 550:
  case 600:      
    return  (dphi<175.*TMath::Pi()/180.);
    break;
  default:
    return true;
    break;
  }
}

bool higgsprecut(int mH, float pt1, float pt2, float dphi)
{
  return higgsptcut(mH,pt1,pt2) && higgsdphicut(mH,dphi);
}

bool higgsmllcut(int mH, float mll)
{
  if (TMath::Abs(mll - 91.1876) < 15 ) return false;
  
  switch (mH) {
  case 115: 
  case 120: 
    return mll < 40;
    break;
  case 130: 
  case 140: 
    return mll < 45;
    break;
  case 150: 
  case 160: 
  case 170:
    return mll < 50;
    break;
  case 180: 
    return mll < 60;
    break;
  case 190: 
    return mll < 80;
    break;
  case 200: 
    return mll < 90;
    break;
  case 250: 
    return mll < 150;
    break;
  case 300: 
    return mll < 200;
    break;
  case 350: 
    return mll < 250;
    break;
  case 400: 
    return mll < 300;
    break;
  case 450: 
    return mll < 350;
    break;
  case 500: 
    return mll < 400;
    break;
  case 550: 
    return mll < 450;
    break;
  case 600: 
    return mll < 500;
    break;
  default:
    return TMath::Abs(mll - 91.1876) > 15;  
    break;
  }
  return true;
}


bool higgsmtcut(int mH, float mt)
{
  switch (mH) {
  case 115: 
    return (mt < 110 && mt > 70);
    break;
  case 120: 
    return (mt < 120 && mt > 70);
    break;
  case 130: 
    return (mt < 125 && mt > 74);
    break;
  case 140: 
    return (mt < 130 && mt > 80);
    break;
  case 150: 
    return (mt < 150 && mt > 80);
    break;
  case 160: 
    return (mt < 160 && mt > 90);
    break;
  case 170: 
    return (mt < 170 && mt > 110);
    break;
  case 180: 
    return (mt < 180 && mt > 120);
    break;
  case 190: 
    return (mt < 190 && mt > 120);
    break;
  case 200: 
    return (mt < 200 && mt > 120);
    break;
  case 250: 
    return (mt < 250 && mt > 120);
    break;
  case 300: 
  case 350:
  case 400:  
  case 450:
  case 500:    
  case 550:
  case 600:        
    return (mt < mH && mt > 120);
    break;
  default:
    return true;
    break;
  }
}

void mcEstimation(TChain *& chMC, float lumi, int type, TCut cut, double& yield, double & yieldError) {
  // EE
  TH1F *mass_ee = new TH1F("mass_ee", "mass_ee", 10, 0, 200);
  mass_ee->Sumw2();
  chMC->Project("mass_ee", "dilep.mass()", Form("scale1fb*sfWeightPU*sfWeightEff*sfWeightTrig*%f*(type==%i&&%s)", lumi, type, cut.GetTitle()));
  yield = mass_ee->IntegralAndError(0,100, yieldError);
  delete mass_ee;
}
 
void combll(double nee, double neeE, double nmm, double nmmE, double & n, double & nE) 
{
  n = nee + nmm;
  nE = sqrt( neeE*neeE + nmmE*nmmE);
}


void ofsubtraction (double Nll, double NllE, double Nem, double NemE, double kll, double kllE, double & Nll_subt, double & Nll_subtE) {
  Nll_subt = Nll - 0.5 * Nem * kll; 
  Nll_subtE = sqrt( pow(NllE,2)  + pow( 0.5 * NemE *kll, 2) +pow( 0.5 * Nem * kllE, 2) ) ;
}

void ofsubt_single(TChain *& chData, TCut cut,  ofstream &text, double k_ee, double k_eeE,
		   double & nee_subt, double &nee_subtE, double &nmm_subt, double &nmm_subtE, 
		   double &ntot_subt, double &ntot_subtE, bool forRatio = false, bool verbose=false)
{  
  // get the event counts with signal selections
  double ndata[4]={0,0,0,0};
  double ndataerr[4]={0,0,0,0};
  for (int i=0; i < 4 ; i++) {
    ndata[i] = chData->GetEntries(Form("(type==%i&&%s)", i, cut.GetTitle()));
    ndataerr[i] = sqrt(ndata[i]);
    if (verbose) {
      printf("%.0f\t", ndata[i]);      
      text << Form("%.0f\t", ndata[i]);      
    }
  }
  if (verbose) {
    printf("\n");
    text << "\n";
  }

  // do the opposite flavor subtraction
  // fill the needed information
  double nem = ndata[1] + ndata[2];
  double nemE = sqrt(pow(ndataerr[1],2) + pow(ndataerr[2],2));
  double nee = ndata[3];
  double neeE = ndataerr[3];
  double nmm = ndata[0];
  double nmmE = ndataerr[0];
  double k_mm = 1./k_ee;
  double k_mmE = k_mm * k_eeE / k_ee;

  // for ee 
  ofsubtraction (nee, neeE, nem, nemE, k_ee, k_eeE, nee_subt, nee_subtE);
  // for mm
  ofsubtraction (nmm, nmmE, nem, nemE, k_mm, k_mmE, nmm_subt, nmm_subtE);

  // now do the sum assuming 0 uncertainty on k
  if (forRatio) {
    ntot_subt = nee + k_ee*k_ee*nmm - k_ee*nem;
    ntot_subtE = sqrt( neeE*neeE  + pow(k_ee*k_ee*nmmE,2) + pow(k_ee*nemE, 2));
  }
  else {
    ntot_subt = nee + nmm - 0.5 * nem * (k_ee + 1./k_ee);
    ntot_subtE = sqrt( neeE*neeE  + nmmE*nmmE + pow(0.5*(k_ee+1./k_ee)*nemE,2));
  }
}

void ratio_syst(TH1F* & ratio_vs_met, double & R, double & RE_stat, double & RE_syst)
{
  int nbinsX = ratio_vs_met->GetNbinsX();

  // take the R from the bin before the signal region
  R = ratio_vs_met->GetBinContent(nbinsX-1);
  RE_stat = ratio_vs_met->GetBinError(nbinsX-1);
  RE_syst = 0.0;
  
  for(int i=1;i<=nbinsX;i++) {
    // do not consider the last bin if the error is > 40%
    if ( i== nbinsX && ratio_vs_met->GetBinContent(i) > 0) {
      if ( ratio_vs_met->GetBinError(i) / ratio_vs_met->GetBinContent(i) > 0.4)  continue;
    }
    RE_syst = RE_syst > TMath::Abs(ratio_vs_met->GetBinContent(i) - R) ? RE_syst : TMath::Abs(ratio_vs_met->GetBinContent(i) - R); 
  }
}


void lookupR(int njet, TString fileName, TString suffix,
	     double & R_ee, double & R_eeE, double & R_eeE_syst,
	     double & R_mm, double & R_mmE, double & R_mmE_syst,
	     double & R, double & RE, double & RE_syst)
{

  TFile *file = TFile::Open(fileName,"READ");
  if (file == 0x0) 
    std::cout << "CANNOT Find " << fileName << "...! You need to fill the Rout/in..\n";
  //assert(file);
  gROOT->cd();
  TH1F *hRee = (TH1F*) file->Get(Form("Ree_vs_met_%s_%iJet", suffix.Data(), njet)); 
  ratio_syst(hRee, R_ee, R_eeE, R_eeE_syst);
  
  TH1F *hRmm = (TH1F*) file->Get(Form("Rmm_vs_met_%s_%iJet", suffix.Data(), njet));
  ratio_syst(hRmm, R_mm, R_mmE, R_mmE_syst);

  TH1F *hR = (TH1F*) file->Get(Form("R_vs_met_%s_%iJet", suffix.Data(), njet));
  ratio_syst(hR, R, RE, RE_syst);
  
  file->Close();
}




void getEstimates(double Di_subt, double  Di_subtE, double R, double RE, double RE_syst,double & pred, double & predE, double & predE_syst)
{
  pred	= R*Di_subt;
  predE = sqrt(pow(R*Di_subtE,2) +pow(RE*Di_subt,2));
  predE_syst = pred * (RE_syst / R);
}


// calculate the k
void fillkee(TChain *chData, float & k_ee, float & k_eeE, int njet) 
{ 
  float Ninee = chData->GetEntries(Form("njets==%i&&((cuts&%i)==%i)&&type==3&&abs(dilep.mass()-91.1876)<15&&min(pmet,pTrackMet)>20&&((cuts&4719111)==4719111)", njet, ww_nozveto_nomet,ww_nozveto_nomet));
  float NineeE = sqrt(Ninee);
  float Ninmm = chData->GetEntries(Form("njets==%i&&((cuts&%i)==%i)&&type==0&&abs(dilep.mass()-91.1876)<15&&min(pmet,pTrackMet)>20&&((cuts&4719111)==4719111)", njet, ww_nozveto_nomet,ww_nozveto_nomet));
  float NinmmE = sqrt(Ninmm);

  k_ee = sqrt(Ninee/Ninmm);
  k_eeE = sqrt(pow(NineeE/Ninmm,2) + pow(NinmmE*Ninee/(Ninmm*Ninmm),2))/2.;
}

// calculate the R from out/in
void calcR(double Nout, double NoutE, double Nin, double NinE, double & R, double & RE) {
  R  = Nin > 0.0 ? Nout/Nin : 0.0;
  RE = Nin > 0.0 ? R*sqrt(pow(NoutE/Nout,2) + pow(NinE/Nin,2)) : 0.0;
}

// fill R ratio in MC
void fillratioMC(  TChain* & chMC, TFile *& ratioFile, int njet, float lumi, 
		   TCut & c_wwloosecut, TCut& c_higgsprecut, TCut & c_higgsmllcut, ofstream &text, 
		   double sigmetcut)
  
{
  ratioFile->cd();
  // declare the R histograms
  TH1F *Rmm_mc = new TH1F("Rmm_mc", "Rmm_mc", nbins, bins);
  Rmm_mc->SetName(Form("Rmm_vs_met_mc_%iJet", njet));
		
  TH1F *Ree_mc = new TH1F("Ree_mc", "Ree_mc", nbins, bins);
  Ree_mc->SetName(Form("Ree_vs_met_mc_%iJet", njet));

  TH1F *R_mc = new TH1F("R_mc", "R_mc", nbins, bins);
  R_mc->SetName(Form("R_vs_met_mc_%iJet", njet));
  
  // fill the R for each slice of the MET cut defined in the R_mc histogram
  for (int i=1;i < nbins +1 ; i++ ) {
    // define the MET cut
    float metcut_low = R_mc->GetBinLowEdge(i);
    float metcut_high = R_mc->GetBinLowEdge(i) + R_mc->GetBinWidth(i);
    TCut c_metcut("c_metcut", Form("min(pmet,pTrackMet)>%f&&min(pmet,pTrackMet)<%f", metcut_low, metcut_high));
    
    if(i==nbins && metcut_low >= sigmetcut) {
      c_metcut = TCut("c_metcut", Form("min(pmet,pTrackMet)>%f", sigmetcut));
      std::cout << Form("***Calculating R for Met Cut (> %.0f):\n", metcut_low);
      text << Form("***Calculating R for Met Cut (> %.0f):\n", metcut_low);
    }
    else {
      std::cout << Form("***Calculating R for Met Cut (%.0f-%.0f):\n", metcut_low, metcut_high);
      text << Form("***Calculating R for Met Cut (%.0f-%.0f):\n", metcut_low, metcut_high);
    }
    
    // Calculate R in MM
    double Nin_mm(0.), Nin_mmE(0.0), Nout_mm(0.0), Nout_mmE(0.0), Rmm(0.), RmmE(0.);
    mcEstimation(chMC, lumi, 0, c_zmmmc+c_wwloosecut+c_higgsprecut+c_zwindow+c_metcut, Nin_mm, Nin_mmE);
    mcEstimation(chMC, lumi, 0, c_zmmmc+c_wwloosecut+c_higgsprecut+c_higgsmllcut+c_metcut, Nout_mm, Nout_mmE);
    calcR(Nout_mm, Nout_mmE, Nin_mm, Nin_mmE, Rmm, RmmE);

    // Calculate R in EE
    double Nin_ee(0.), Nin_eeE(0.0), Nout_ee(0.0), Nout_eeE(0.0), Ree(0.), ReeE(0.);
    mcEstimation(chMC, lumi, 3, c_zeemc+c_wwloosecut+c_higgsprecut+c_zwindow+c_metcut, Nin_ee, Nin_eeE);
    mcEstimation(chMC, lumi, 3, c_zeemc+c_wwloosecut+c_higgsprecut+c_higgsmllcut+c_metcut, Nout_ee, Nout_eeE);
    calcR(Nout_ee, Nout_eeE, Nin_ee, Nin_eeE, Ree, ReeE);
    
    // Calculate R in EE+MM
    double Nin(0.), NinE(0.0), Nout(0.0), NoutE(0.0), R(0.), RE(0.0);
    Nin = Nin_ee + Nin_mm;
    NinE = sqrt(Nin_eeE*Nin_eeE + Nin_mmE*Nin_mmE);
    Nout = Nout_ee + Nout_mm;
    NoutE = sqrt(Nout_eeE*Nout_eeE + Nout_mmE*Nout_mmE);
    calcR(Nout, NoutE, Nin, NinE, R, RE);
  

    // Save histograms
    Rmm_mc->SetBinContent(i, Rmm);
    Rmm_mc->SetBinError(i, RmmE);
    Ree_mc->SetBinContent(i, Ree);
    Ree_mc->SetBinError(i, ReeE);
    R_mc->SetBinContent(i, R);
    R_mc->SetBinError(i, RE);
    
    // Print out numsbers
    printf("MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin_mm, Nin_mmE, Nout_mm, Nout_mmE, Rmm, RmmE);
    text << Form("MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin_mm, Nin_mmE, Nout_mm, Nout_mmE, Rmm, RmmE);
    printf("EE: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin_ee, Nin_eeE, Nout_ee, Nout_eeE, Ree, ReeE);
    text << Form("EE: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin_ee, Nin_eeE, Nout_ee, Nout_eeE, Ree, ReeE);
    printf("EE+MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin, NinE, Nout, NoutE, R, RE);
    text << Form("EE+MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin, NinE, Nout, NoutE, R, RE);
  }
  
  Rmm_mc->Write();
  Ree_mc->Write();
  R_mc->Write();
}

  
// fill R ratio in Data
void fillratioData(  TChain* & chData, TFile *& ratioFile, int njet, 
		     const float  k_ee, const float  k_eeE, 
		     TCut &c_wwloosecut, TCut& c_higgsprecut, TCut & c_higgsmllcut, ofstream &text, double sigmetcut)
  
{
  ratioFile->cd();
  
  TH1F *Rmm_data = new TH1F("Rmm_data", "Rmm_data", nbins, bins);
  Rmm_data->SetName(Form("Rmm_vs_met_data_%iJet", njet));
		
  TH1F *Ree_data = new TH1F("Ree_data", "Ree_data", nbins, bins);
  Ree_data->SetName(Form("Ree_vs_met_data_%iJet", njet));

  TH1F *R_data = new TH1F("R_data", "R_data", nbins, bins);
  R_data->SetName(Form("R_vs_met_data_%iJet", njet));
  
  // fill the R for each slice of the MET cut defined in the R_data histogram
  for (int i=1;i <nbins+1; i++ ) {
    // define the MET cut
    float metcut_low = R_data->GetBinLowEdge(i);
    //if (metcut_low >= sigmetcut) continue; // avoid looking at the signal box
    float metcut_high = R_data->GetBinLowEdge(i) + R_data->GetBinWidth(i);
    TCut c_metcut("c_metcut", Form("min(pmet,pTrackMet)>%f&&min(pmet,pTrackMet)<%f", metcut_low, metcut_high));
    
    if(i==nbins && metcut_low >= sigmetcut) {
      c_metcut = TCut("c_metcut", Form("min(pmet,pTrackMet)>%f", sigmetcut));
      std::cout << Form("***Calculating R for Met Cut (> %.0f):\n", metcut_low);
      text << Form("***Calculating R for Met Cut (> %.0f):\n", metcut_low);
    }
    else {
      std::cout << Form("***Calculating R for Met Cut (%.0f-%.0f):\n", metcut_low, metcut_high);
      text << Form("***Calculating R for Met Cut (%.0f-%.0f):\n", metcut_low, metcut_high);
    }
    double Nin_mm(0.), Nin_mmE(0.0), Nin_ee(0.), Nin_eeE(0.0), Nin(0.0), NinE(0.0);
    double Nout_mm(0.), Nout_mmE(0.0), Nout_ee(0.), Nout_eeE(0.0), Nout(0.0), NoutE(0.0);
    ofsubt_single(chData, c_wwloosecut+c_higgsprecut+c_zwindow+c_metcut, text, k_ee, k_eeE, Nin_ee, Nin_eeE, Nin_mm, Nin_mmE, Nin, NinE, true);
    ofsubt_single(chData, c_wwloosecut+c_higgsprecut+c_higgsmllcut+c_metcut, text, k_ee, k_eeE, Nout_ee, Nout_eeE, Nout_mm, Nout_mmE, Nout, NoutE,true);
    
    double Rmm(0.), RmmE(0.), Ree(0.), ReeE(0.), R(0.), RE(0.);
    calcR(Nout_mm, Nout_mmE, Nin_mm, Nin_mmE, Rmm, RmmE);
    calcR(Nout_ee, Nout_eeE, Nin_ee, Nin_eeE, Ree, ReeE);
    calcR(Nout, NoutE, Nin, NinE, R, RE);
    
    printf("MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin_mm, Nin_mmE, Nout_mm, Nout_mmE, Rmm, RmmE);
    text << Form("MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin_mm, Nin_mmE, Nout_mm, Nout_mmE, Rmm, RmmE);
    
    // save the values in the histograms
    Rmm_data->SetBinContent(i, Rmm);
    Rmm_data->SetBinError(i, RmmE);
    Ree_data->SetBinContent(i, Ree);
    Ree_data->SetBinError(i, ReeE);
    R_data->SetBinContent(i, R);
    R_data->SetBinError(i, RE); 

    // Print out numbers
    printf("MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin_mm, Nin_mmE, Nout_mm, Nout_mmE, Rmm, RmmE);
    text << Form("MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin_mm, Nin_mmE, Nout_mm, Nout_mmE, Rmm, RmmE);
    printf("EE: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin_ee, Nin_eeE, Nout_ee, Nout_eeE, Ree, ReeE);
    text << Form("EE: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin_ee, Nin_eeE, Nout_ee, Nout_eeE, Ree, ReeE);
    printf("EE+MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n",  Nin, NinE, Nout, NoutE, R, RE);
    text << Form("EE+MM: Nin = %.1f +/- %.1f, Nout = %.1f +/- %.1f, R = %.3f +/- %.3f\n", Nin, NinE, Nout, NoutE, R, RE);
  }
  
  Rmm_data->Write();
  Ree_data->Write();
  R_data->Write();
}

