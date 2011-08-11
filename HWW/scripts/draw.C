#include "TH1F.h"
#include "histTool.C"

void draw(int mH = 0, int njet=0, float lumi = 1.092, TString dysample="dy" ) {

  gROOT->ProcessLine(".L tdrStyle.C");
  gROOT->ProcessLine("setTDRStyle()");
  
  gStyle->SetOptFit(1);

  lumi = lumi*1000.0; 
  TFile *file = TFile::Open(Form("Routin_%iJet_mH%i_%.0fpb.root", njet, mH, lumi ), "READ");
  assert(file);
  gROOT->cd();

  TH1F *Ree_vs_met_mc = (TH1F*) file->Get(Form("Ree_vs_met_mc_%iJet", njet));
  TH1F *Rmm_vs_met_mc = (TH1F*) file->Get(Form("Rmm_vs_met_mc_%iJet", njet));
  TH1F *R_vs_met_mc = (TH1F*) file->Get(Form("R_vs_met_mc_%iJet", njet));
  
  setStyle(Ree_vs_met_mc, 1, false, kBlue, 21, "min(proj_{tk-MET}, proj_{pfMET}) (GeV)", "R_{out/in}", 0, 50);
  setStyle(Rmm_vs_met_mc, 1, false, kRed, 21, "min(proj_{tk-MET}, proj_{pfMET}) (GeV)", "R_{out/in}", 0, 50);
  setStyle(R_vs_met_mc, 1, false, kRed, 24, "min(proj_{tk-MET}, proj_{pfMET}) (GeV)", "R_{out/in}", 0, 50);

  R_vs_met_mc->SetMarkerSize(1.5);
  
  TH1F *Ree_vs_met_data = (TH1F*) file->Get(Form("Ree_vs_met_data_%iJet", njet));
  TH1F *Rmm_vs_met_data = (TH1F*) file->Get(Form("Rmm_vs_met_data_%iJet", njet));
  TH1F *R_vs_met_data = (TH1F*) file->Get(Form("R_vs_met_data_%iJet", njet));

  setStyle(Ree_vs_met_data, 1, false, kBlue, 20, "min(proj_{tk-MET}, proj_{pfMET}) (GeV)", "R_{out/in}", 0, 50);
  setStyle(Rmm_vs_met_data, 1, false, kRed, 20, "min(proj_{tk-MET}, proj_{pfMET}) (GeV)", "R_{out/in}", 0, 50);
  setStyle(R_vs_met_data, 1, false, kBlack, 20, "min(proj_{tk-MET}, proj_{pfMET}) (GeV)", "R_{out/in}", 0, 50);
  R_vs_met_data->SetMarkerSize(1.5);

  float yMax = 1.0;
  if (njet == 1) {
    if (mH <= 400) yMax = 1.0;
    else yMax = 5.0;
  }
  
  if (njet == 0) {
    switch (mH) {
    case 115:
    case 120:
    case 130:
    case 140:
      yMax = 3.0;
      break;
    case 150:
    case 160:
    case 170:
    case 180:
      yMax = 5.0;
      break;
    case 190:
    case 200:
    case 250:
    case 300:
      yMax = 1.0;
      break;
    case 350:
    case 400:
    case 450:
    case 500:
    case 550:
    case 600:
      yMax = 5.0;
      break;
    default:
      yMax = 1.0;
      break;
    }
  }
  
  
  Ree_vs_met_mc->GetYaxis()->SetRangeUser(0,yMax);
  Rmm_vs_met_mc->GetYaxis()->SetRangeUser(0,yMax);
  R_vs_met_mc->GetYaxis()->SetRangeUser(0,yMax);

  Rmm_vs_met_data->GetYaxis()->SetRangeUser(0,yMax);
  Ree_vs_met_data->GetYaxis()->SetRangeUser(0,yMax);
  R_vs_met_data->GetYaxis()->SetRangeUser(0,yMax);


  TCanvas *c1 = new TCanvas();
  c1->cd();  
  
  TLegend *leg;
  if (mH == 0)
    leg = new TLegend(0.2, 0.75, 0.6, 0.90, Form("R_{out/in} with WW Selection",mH), "brNDC");
  else
    leg = new TLegend(0.2, 0.75, 0.6, 0.90, Form("R_{out/in} with mH(%i) Selection",mH), "brNDC");
  
  leg->SetFillColor(0);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetShadowColor(0);

  // do basic fit to a straight line
  //R_vs_met_data->Fit("pol0");
  //R_vs_met_data->SetStats(1);
  R_vs_met_data->Draw("HISTE1FUNC");
  R_vs_met_mc->Draw("SAMEHISTE1");
  
  TLine *metsig = new TLine(40, 0., 40, yMax * 0.7 );
  metsig->SetLineStyle(kDashed);
  metsig->SetLineWidth(2);
  metsig->Draw("same");

  /*
  TArrow *metcutarr = new TArrow(40, 0.35*yMax, 50, yMax*0.35 , 0.08, "|>");
  metcutarr->SetLineStyle(kDashed);
  metcutarr->SetLineWidth(2);
  metcutarr->SetAngle(25.);
  //metcutarr->Draw("same");
  */
  


  //leg->AddEntry(Rmm_vs_met_data, "R(#mu#mu): OF-Subt in data", "lp");
  //leg->AddEntry(Ree_vs_met_data, "R(ee): OF-Subt in data", "lp");
  leg->AddEntry(R_vs_met_data, "OF-Subt in data", "lp");
  if (dysample == "dysoup") 
    leg->AddEntry(R_vs_met_mc, "Z/ZZ/WZ MC Prediction", "lp");
  if (dysample == "dy")
    leg->AddEntry(R_vs_met_mc, "Drell-Yan MC Prediction", "lp");
  if (dysample == "vz")
    leg->AddEntry(R_vs_met_mc, "ZZ/WZ MC Prediction", "lp");

  leg->Draw();
  c1->Print(Form("Routin_%iJet_mH%i_%.0fpb_%s.eps", njet, mH,lumi,dysample.Data()));
  c1->Print(Form("Routin_%iJet_mH%i_%.0fpb_%s.png", njet, mH,lumi,dysample.Data()));
  



  delete c1;
  delete Ree_vs_met_data;
  delete Rmm_vs_met_data;
  delete R_vs_met_data;
  delete Ree_vs_met_mc;
  delete Rmm_vs_met_mc;
  delete R_vs_met_mc;
  delete metsig;
  
  file->Close();
  
}
