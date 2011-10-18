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
#include "dyestfunc.h"
#include "goodrun.cc"

ofstream text;



typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector; 

void dycalc(int mH = 0, int njet = 0, float metcut = 40.0, float lumiinpb = 1545, 
	    TString dataDir = "/Users/yanyan/CMS/SnT/WW/DYEst/LP2011/data/",
	    TString useMCRValue = true,  bool fillRoutin=true)
{
  gROOT->ProcessLine(".L goodrun.h+");    
  gROOT->ProcessLine(".L dyestfunc.h+");
  
  float lumi = lumiinpb / 1000.;
    // set json
  //set_goodrun_file("LP2011_json_1545pb.txt");
  set_goodrun_file("2011a_2121pb.txt");
  TCut c_goodrun("dstype != 0 || goodrun(run,lumi)");
  
  // define the cuts used later
  TCut c_wwloosecut(Form("njets==%i&&((cuts&%i)==%i)",njet,ww_nozveto_nomet,ww_nozveto_nomet) + c_goodrun);
  TCut c_higgsprecut(Form("higgsprecut(%i,lep1.Pt(), lep2.Pt(), dPhi)", mH)+c_higgsextra); // this extra cut is the dPhiDiLepJet1
  TCut c_higgsmllcut(Form("higgsmllcut(%i,dilep.M())", mH));
  TCut c_higgsmtcut(Form("higgsmtcut(%i,mt)", mH));
  TCut c_higgsinZ = Form("njets==%i&&((cuts&%i)==%i)&&min(pmet,pTrackMet)>%f", njet, ww_nozveto, ww_nozveto, metcut)+c_higgsprecut+c_zwindow+c_higgsmtcut+c_goodrun;
  TCut c_higgsfincut = Form("njets==%i&&((cuts&%i)==%i)&&min(pmet,pTrackMet)>%f", njet, ww, ww, metcut)+c_higgsprecut+c_higgsmllcut+c_higgsmtcut+c_goodrun;
  
  // load relevant data files
  TChain *chMC = new TChain("tree");
  chMC->Add(dataDir + "dyee.root");
  chMC->Add(dataDir + "dymm.root");
  assert(chMC);

  TChain *chMCVZ = new TChain("tree");
  chMCVZ->Add(dataDir + "wz.root");
  chMCVZ->Add(dataDir + "zz.root");
   
    
  // load the data file
  TChain *chData = new TChain("tree");
  chData->Add(dataDir + "data.root");
  assert(chData);
  
  // log files
  text.open(Form("dyest_%iJet_Metcut%.0f_mH%i_%.0fpb.txt", njet, metcut,mH,lumi*1000));
    
  // ======================================================
  // Calcualte the data-driven estimates
  // ======================================================

  // ======================================================
  // == 1. Get the data-driven electron/muon efficiency ratio 
  // ======================================================
  float k_ee(0.0), k_eeE(0.0);
  fillkee(chData, k_ee, k_eeE, njet);
  std::cout << "----------------------------------------------------------------------------\n";
  std::cout<< Form("Electron to muon efficiency is %.3f +/- %.3f\n", k_ee, k_eeE);
  
  text << "----------------------------------------------------------------------------\n";
  text << Form("Electron to muon efficiency is %.3f +/- %.3f\n", k_ee, k_eeE);

  // set error on k_eeE to 0
  k_eeE = 0.0;

  // do opposite flavor subtraction in the signal box.
  double Do_ee_subt(0.), Do_ee_subtE(0.), Do_mm_subt(0.), Do_mm_subtE(0.), Do_subt(0.), Do_subtE(0.);
  ofsubt_single(chData, c_higgsfincut, text, k_ee, k_eeE,  Do_ee_subt, Do_ee_subtE, Do_mm_subt, Do_mm_subtE, Do_subt, Do_subtE, false,true);
  std::cout << Form("OF subtracted yields in signal region:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM) \n", 
		    Do_mm_subt, Do_mm_subtE, Do_ee_subt, Do_ee_subtE, Do_subt, Do_subtE);
  text << Form("OF subtracted yields in signal region:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM) \n", 
	       Do_mm_subt, Do_mm_subtE, Do_ee_subt, Do_ee_subtE, Do_subt, Do_subtE); 
  
  text << "----------------------------------------------------------------------------\n";

  // ======================================================
  // == 2. Fill the Rout/in from data and MC  
  // ======================================================
  TString ratioFileName = Form("Routin_%iJet_mH%i_%.0fpb.root", njet,mH,lumi*1000);
  
  if (fillRoutin) {
    TFile *ratioFile = new TFile(ratioFileName, "RECREATE");
    text << "----------------------------------------------------------------------------\n";
    text << "Filling R in MC....\n";  
    fillratioMC(chMC, ratioFile, njet, lumi, c_wwloosecut, c_higgsprecut, c_higgsmllcut, text, metcut);
    text << "----------------------------------------------------------------------------\n";
    text << "Filling R in Data using OF subtraction....\n";  
    fillratioData(chData, ratioFile, njet, k_ee, k_eeE, c_wwloosecut, c_higgsprecut, c_higgsmllcut, text, metcut);
    ratioFile->Close();
  }
  

  
  // ======================================================
  // == 3. Count the number events in the Z window and do OF subtraction
  // ======================================================
  double Di_ee_subt(0.), Di_ee_subtE(0.), Di_mm_subt(0.), Di_mm_subtE(0.), Di_subt(0.), Di_subtE(0.);
  std::cout << "----------------------------------------------------------------------------\n";
  std::cout << "Number of events inside Z window for " << njet << "-jet bin (mm/me/em/ee): \n";
  text << "----------------------------------------------------------------------------\n";
  text << "Number of events inside Z window for " << njet << "-jet bin (mm/me/em/ee): \n";
  ofsubt_single(chData, c_higgsinZ, text, k_ee, k_eeE,  Di_ee_subt, Di_ee_subtE, Di_mm_subt, Di_mm_subtE, Di_subt, Di_subtE, false,true);
  
  std::cout << Form("OF subtracted yields in Z window:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM) \n", 
	     Di_mm_subt, Di_mm_subtE, Di_ee_subt, Di_ee_subtE, Di_subt, Di_subtE);
  text << Form("OF subtracted yields in Z window:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM) \n", 
	       Di_mm_subt, Di_mm_subtE, Di_ee_subt, Di_ee_subtE, Di_subt, Di_subtE);
  
  // ======================================================
  // == 4. Subtract the WZ/ZZ contribution using MC
  //       We need to subtract the WZ/ZZ contribution in this case
  //       put an uncertainty of 10% on the WZ/ZZ syst error.
  
  double Di_ee_vz(0.), Di_ee_vzE(0.), Di_mm_vz(0.), Di_mm_vzE(0.), Di_vz(0.), Di_vzE(0.);
  mcEstimation(chMCVZ, lumi, 0, c_higgsinZ+c_zmmmc, Di_mm_vz, Di_mm_vzE);
  mcEstimation(chMCVZ, lumi, 3, c_higgsinZ+c_zeemc, Di_ee_vz, Di_ee_vzE);
  combll(Di_mm_vz, Di_mm_vzE, Di_ee_vz, Di_ee_vzE, Di_vz, Di_vzE);
  double vzsyst = 0.1; // the uncertainty to the MC cross-section on WZ/ZZ
  double Di_ee_vzE_syst = Di_ee_vz * vzsyst;
  double Di_mm_vzE_syst = Di_mm_vz * vzsyst;
  double Di_vzE_syst = Di_vz * vzsyst;
  
  printf("WZ/ZZ contribution in Z window: \n %.2f +/- %.2f +/- %.2f (MM), %.2f +/- %.2f +/- %.2f (EE), %.2f +/- %.2f +/- %.2f (EE+MM)\n", 
	 Di_mm_vz, Di_mm_vzE, Di_mm_vzE_syst, Di_ee_vz, Di_ee_vzE, Di_mm_vzE_syst, Di_vz, Di_vzE, Di_vzE_syst);
  text << Form("WZ/ZZ contribution in Z window: \n %.2f +/- %.2f +/- %.2f (MM), %.2f +/- %.2f +/- %.2f (EE), %.2f +/- %.2f +/- %.2f (EE+MM)\n", 
	 Di_mm_vz, Di_mm_vzE, Di_mm_vzE_syst, Di_ee_vz, Di_ee_vzE, Di_ee_vzE_syst, Di_vz, Di_vzE, Di_vzE_syst);

  double Do_ee_vz(0.), Do_ee_vzE(0.), Do_mm_vz(0.), Do_mm_vzE(0.), Do_vz(0.), Do_vzE(0.);
  mcEstimation(chMCVZ, lumi, 0, c_higgsfincut+c_zmmmc, Do_mm_vz, Do_mm_vzE);
  mcEstimation(chMCVZ, lumi, 3, c_higgsfincut+c_zeemc, Do_ee_vz, Do_ee_vzE);
  combll(Do_mm_vz, Do_mm_vzE, Do_ee_vz, Do_ee_vzE, Do_vz, Do_vzE);
  printf("WZ/ZZ contribution in signal region: \n %.2f +/- %.2f (MM), %.2f +/- %.2f (EE), %.2f +/- %.2f (EE+MM)\n", 
	 Do_mm_vz, Do_mm_vzE, Do_ee_vz, Do_ee_vzE, Do_vz, Do_vzE);
  text << Form("WZ/ZZ contribution in signal region: \n %.2f +/- %.2f (MM), %.2f +/- %.2f (EE), %.2f +/- %.2f (EE+MM)\n", 
       Do_mm_vz, Do_mm_vzE, Do_ee_vz, Do_ee_vzE, Do_vz, Do_vzE);

  Di_ee_subt = Di_ee_subt - Di_ee_vz;
  Di_ee_subtE = sqrt( pow(Di_ee_subtE,2) + pow(Di_ee_vzE,2) + pow(Di_ee_vzE_syst,2));

  Di_mm_subt = Di_mm_subt - Di_mm_vz;
  Di_mm_subtE = sqrt( pow(Di_mm_subtE,2) + pow(Di_mm_vzE,2) + pow(Di_mm_vzE_syst,2));
  
  Di_subt = Di_subt - Di_vz;
  Di_subtE = sqrt( pow(Di_subtE,2) + pow(Di_vzE,2) + pow(Di_vzE_syst,2));

  // if over-subtracting, fluctuate the event count to 1
  if (Di_subt <= 0.0)
    Di_subt = 1.0;

  std::cout << Form("OF/VZ subtracted yields in Z window:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM) \n", 
	     Di_mm_subt, Di_mm_subtE, Di_ee_subt, Di_ee_subtE, Di_subt, Di_subtE);
  text << Form("OF/VZ subtracted yields in Z window:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM) \n", 
	       Di_mm_subt, Di_mm_subtE, Di_ee_subt, Di_ee_subtE, Di_subt, Di_subtE);

  // == Z window regiom
  double nmmInMC(0.), nmmInMCE(0.0), neeInMC(0.), neeInMCE(0.0), nInMC(0.), nInMCE(0.0);
  mcEstimation(chMC, lumi, 0, c_higgsinZ+c_zmmmc, nmmInMC, nmmInMCE);
  mcEstimation(chMC, lumi, 3, c_higgsinZ+c_zeemc, neeInMC, neeInMCE);
  combll(nmmInMC, nmmInMCE, neeInMC, neeInMCE, nInMC, nInMCE);
  text << "----------------------------------------------------------------------------\n";
 printf("MC estimation in Z window:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM)\n", nmmInMC , nmmInMCE, neeInMC, neeInMCE, nInMC, nInMCE);
   text << Form("MC estimation in Z window:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM)\n", nmmInMC , nmmInMCE, neeInMC, neeInMCE, nInMC, nInMCE);
   
  double sfIn_ee = Di_ee_subt / neeInMC;
  double sfIn_eeE = sfIn_ee * sqrt( pow(Di_ee_subtE/Di_ee_subt, 2) + pow(neeInMCE/neeInMC,2));
  
  double sfIn_mm = Di_mm_subt / nmmInMC;
  double sfIn_mmE = sfIn_mm * sqrt( pow(Di_mm_subtE/Di_mm_subt, 2) + pow(nmmInMCE/nmmInMC,2));
  
  double sfIn = Di_subt / nInMC;
  double sfInE = sfIn * sqrt( pow(Di_subtE/Di_subt, 2) + pow(nInMCE/nInMC,2));
  
  printf("data/MC scale factor from Z-window yield:\n %.2f +/- %.2f (MM)\t  %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM)\n",
	 sfIn_mm, sfIn_mmE, sfIn_ee, sfIn_eeE, sfIn, sfInE);
  text << Form("data/MC scale factor from Z-window yield:\n %.2f +/- %.2f (MM)\t  %.2f +/- %.2f(EE)\t %.2f +/- %.2f (EE+MM)\n",
	       sfIn_mm, sfIn_mmE, sfIn_ee, sfIn_eeE, sfIn, sfInE);
  
  // ======================================================
  // == 4. Do the data-driven estimate
  // ======================================================
  // Read the Rout file and get the nominal value

  std::cout << "Looking up R from " << ratioFileName << std::endl;
  double R_ee(0.), R_eeE(0.), R_eeE_syst(0.0),  R_mm(0.), R_mmE(0.), R_mmE_syst(0.), R(0.), RE(0.), RE_syst(0.);
  if (useMCRValue)
    lookupR(njet, ratioFileName, "mc", R_ee, R_eeE, R_eeE_syst, R_mm, R_mmE, R_mmE_syst, R, RE, RE_syst);
  else 
    lookupR(njet, ratioFileName, "data", R_ee, R_eeE, R_eeE_syst, R_mm, R_mmE, R_mmE_syst, R, RE, RE_syst);
  
  printf("Choose the nominal R as...\n ");
  printf("R(EE) = %.2f +/- %.2f +/- %.2f\n", R_ee, R_eeE, R_eeE_syst);
  printf("R(MM) = %.2f +/- %.2f +/- %.2f\n", R_mm, R_mmE, R_mmE_syst);
  printf("R(EE+MM) = %.2f +/- %.2f +/- %.2f\n", R, RE, RE_syst);
  text << "----------------------------------------------------------------------------\n";  
  text << Form("Choose the nominal R as..\n");
  text << Form("R(EE) = %.2f +/- %.2f +/- %.2f\n", R_ee, R_eeE, R_eeE_syst);
  text << Form("R(MM) = %.2f +/- %.2f +/- %.2f\n", R_mm, R_mmE, R_mmE_syst);
  text << Form("R(EE+MM) = %.2f +/- %.2f +/- %.2f\n", R, RE, RE_syst);

  double pred_ee(0.), pred_eeE(0.), pred_eeE_syst(0.), pred_mm(0.), pred_mmE(0.), pred_mmE_syst(0.), pred(0.), predE(0.), predE_syst(0.);
  getEstimates( Di_ee_subt, Di_ee_subtE, R_ee, R_eeE, R_eeE_syst, pred_ee, pred_eeE, pred_eeE_syst);
  getEstimates( Di_mm_subt, Di_mm_subtE, R_mm, R_mmE, R_mmE_syst, pred_mm, pred_mmE, pred_mmE_syst);
  getEstimates( Di_subt, Di_subtE, R, RE, RE_syst, pred, predE, predE_syst);

  text << "----------------------------------------------------------------------------\n";
  printf("data-driven estimate:\n %.2f +/- %.2f +/- %.2f(MM)\t %.2f +/- %.2f +/- %.2f (EE)\t %.2f +/- %.2f +/- %.2f (EE+MM)\n", 
	       pred_mm, pred_mmE, pred_mmE_syst, pred_ee, pred_eeE, pred_eeE_syst, pred, predE, predE_syst);
  text << Form("data-driven estimate:\n %.2f +/- %.2f +/- %.2f(MM)\t %.2f +/- %.2f +/- %.2f (EE)\t %.2f +/- %.2f +/- %.2f (EE+MM)\n", 
	       pred_mm, pred_mmE, pred_mmE_syst, pred_ee, pred_eeE, pred_eeE_syst, pred, predE, predE_syst);
  
  // ======================================================
  // == 5. Check MC based predictions and data/MC scale-factors
  // ======================================================
  
  // == Signal region
  double nmmMC(0.), nmmMCE(0.0), neeMC(0.), neeMCE(0.0), nMC(0.0), nMCE(0.);
  mcEstimation(chMC, lumi, 0, c_higgsfincut+c_zmmmc, nmmMC, nmmMCE);
  mcEstimation(chMC, lumi, 3, c_higgsfincut, neeMC, neeMCE);
  combll(nmmMC, nmmMCE, neeMC, neeMCE, nMC, nMCE);

  text << "----------------------------------------------------------------------------\n";
  printf("MC estimation in signal region %i-jet bin:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t  %.2f +/- %.2f (EE+MM)\n", 
	 njet, nmmMC, nmmMCE, neeMC, neeMCE, nMC, nMCE);
  text << Form("MC estimation in signal region %i-jet bin:\n %.2f +/- %.2f(MM)\t %.2f +/- %.2f(EE)\t  %.2f +/- %.2f (EE+MM)\n", 
	       njet, nmmMC, nmmMCE, neeMC, neeMCE, nMC, nMCE);
  
  // Be careful, if using the MC for the scale factor, do not count the N(out MC)
  // scalefactor = N(in) data / N(in) MC if using the MC
  // (5B) from the out-peak
  double sf_ee = pred_ee / neeMC;
  double sf_eeE_syst = sf_ee * pred_eeE_syst/pred_ee;
  double sf_eeE = sf_ee * sqrt( pow(pred_eeE/pred_ee,2) + pow(neeMCE/neeMC,2));
  if (useMCRValue)
    sf_eeE = sf_ee * (sfIn_eeE/sfIn_ee);
  
  double sf_mm = pred_mm / nmmMC;
  double sf_mmE_syst = sf_mm * pred_mmE_syst/pred_mm;
  double sf_mmE = sf_mm * sqrt( pow(pred_mmE/pred_mm,2) + pow(nmmMCE/nmmMC,2));
  if (useMCRValue)
    sf_mmE = sf_mm * (sfIn_mmE/sfIn_mm);
    

  double sf = pred / nMC;
  double sfE = sf * sqrt( pow(predE/pred,2) + pow(nMCE/nMC,2));
  double sfE_syst = sf * predE_syst/pred;
  if (useMCRValue)
    sfE = sf * (sfInE/sfIn);
  
  printf("data/MC scale factor from Rout/in method:\n %.2f +/- %.2f +/- %.2f (MM)\t  %.2f +/- %.2f +/- %.2f (EE)\t %.2f +/- %.2f +/- %.2f (EE+MM)\n",
	 sf_mm, sf_mmE, sf_mmE_syst, sf_ee, sf_eeE, sf_eeE_syst, sf, sfE, sfE_syst);
  text << Form("data/MC scale factor from Rout/in method:\n %.2f +/- %.2f (MM)\t  %.2f +/- %.2f (EE)\t %.2f +/- %.2f (EE+MM)\n",
	       sf_mm, sqrt(sf_mmE*sf_mmE + sf_mmE_syst*sf_mmE_syst), sf_ee, sqrt(sf_eeE*sf_eeE + sf_eeE_syst*sf_eeE_syst), sf, sqrt(sfE*sfE + sfE_syst*sfE_syst));
  text << "----------------------------------------------------------------------------\n";

 // Tempoary to calculate scale factors based on the ww selection level.
  
  double sf_ww(0.), sf_wwE_syst(0.), sf_wwE_stat(0.);
  if (njet == 0) {
    sf_ww = 3.02;
    sf_wwE_stat = 0;
    sf_wwE_syst = 1.85;
  }

  if (njet == 1) {
    sf_ww = 2.81;
    sf_wwE_stat = 0.;
    sf_wwE_syst = 1.40;
  }
  
  if (njet == 2) {
    sf_ww = 4.84;
    sf_wwE_stat = 0.40;
    sf_wwE_syst = 1.83;
  }

  double pred_b = nMC * sf_ww;
  double pred_bE = pred_b * sqrt(pow(sf_wwE_stat/sf_ww,2) + pow(nMCE/nMC,2));
  double pred_bE_syst = pred_b * sf_wwE_syst/sf_ww;

  printf("data-driven estimate using ww selection SF:\n %.2f +/- %.2f +/- %.2f (EE+MM)\n", pred_b, pred_bE, pred_bE_syst);
  text << Form("data-driven estimate using ww selection SF:\n %.2f +/- %.2f +/- %.2f (EE+MM)\n", pred_b, pred_bE, pred_bE_syst);
  text << "----------------------------------------------------------------------------\n";
}


