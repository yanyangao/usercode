#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "THStack.h"
#include <iostream>
#include <stdlib.h>
#include "TChain.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TArrow.h"


int mH = 130;
bool fromMCFM = true;
// TString cuts = "hww130cuts";
TString cuts = "nocuts";

void drawsimple8vs7()
{

  bool normalize = 1;
  bool doEightTeV = 1;
  
  bool doWW = 0;
  bool doHiggs = 1;
  
  
  TString plotDir;
  TString fileName;

  
  if ( doWW) {
    plotDir = Form("wwmadgraph/%s/", cuts.Data());
    fileName = Form("histos_wwmadgraph_%s.root", cuts.Data());
    drawsingle(fileName, "mt_ww", "Transverse Higgs Mass [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "met_ww", "MET [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "leadleppt_ww", "Leading Lepton pT [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "trailleppt_ww", "Trailing Lepton pT [GeV]", normalize, doEightTeV, plotDir); 
    drawsingle(fileName, "leadlepeta_ww", "Leading Lepton #eta [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "traillepeta_ww", "Trailing Lepton #eta [GeV]", normalize, doEightTeV, plotDir); 
    drawsingle(fileName, "mll_ww", "Dilepton Mass [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "dilpt_ww", "Dilepton pT [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "dPhi_ww", "#Delta#Phi(dileptons) [degrees]", normalize, doEightTeV, plotDir);
    // drawsingle(fileName, "dR_ww", "#Delta R(dileptons)", normalize, doEightTeV, plotDir);
  }

  if ( doHiggs) {
    plotDir = Form("hww%i/%s/", mH, cuts.Data());;
    if ( fromMCFM) 
      plotDir = Form("hww%imcfm/%s/", mH, cuts.Data());;
    fileName = Form("histos_hww%i_%s.root", mH, cuts.Data());
    drawsingle(fileName, "mt_hww", "Transverse Higgs Mass [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "met_hww", "MET [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "leadleppt_hww", "Leading Lepton pT [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "trailleppt_hww", "Trailing Lepton pT [GeV]", normalize, doEightTeV, plotDir); 
    drawsingle(fileName, "leadlepeta_hww", "Leading Lepton #eta [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "traillepeta_hww", "Trailing Lepton #eta [GeV]", normalize, doEightTeV, plotDir); 
    drawsingle(fileName, "mll_hww", "Dilepton Mass [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "dilpt_hww", "Dilepton pT [GeV]", normalize, doEightTeV, plotDir);
    drawsingle(fileName, "dPhi_hww", "#Delta#Phi(dileptons) [degrees]", normalize, doEightTeV, plotDir);
  }
  
}
void drawsingle(TString fileName, TString varName, TString x_title, bool normalize, bool doEightTeV, TString plotDir) 
{
  // load macros  
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()"); 
  gROOT->ForceStyle();
  
  TFile *file = new TFile(fileName);
  
  TH1F *h_8tev = (TH1F*) file->Get(Form("%s_8tev", varName.Data()));
  TH1F *h_7tev = (TH1F*) file->Get(Form("%s_7tev", varName.Data()));
  
  h_7tev->SetLineColor(kBlack);
  h_8tev->SetLineColor(kBlue);

  h_7tev->SetMarkerColor(kBlack);
  h_8tev->SetMarkerColor(kBlue);

  h_7tev->GetYaxis()->SetNdivisions(505);
  h_8tev->GetYaxis()->SetNdivisions(505);
  
  h_7tev->SetLineWidth(3);
  h_8tev->SetLineWidth(3);

  h_7tev->SetMinimum(0.);
  h_8tev->SetMinimum(0.);

  TString y_title = "Number of Events";
  if ( normalize ) y_title = "Number of Events normalized to 1";
  h_7tev->SetXTitle(x_title);
  h_7tev->SetYTitle(y_title);

  if ( normalize) {
    h_8tev->Scale(1./h_8tev->Integral(0,1000));
    h_7tev->Scale(1./h_7tev->Integral(0,1000));
  }

  float yMax  = h_7tev->GetMaximum();
  yMax = yMax > h_8tev->GetMaximum() ? yMax : h_8tev->GetMaximum();

  TLegend *leg = new TLegend(0.6, 0.78, 0.85, 0.89);
  leg->SetFillColor(0);
  leg->SetTextSize(0.035);
  if ( fromMCFM) {
    leg->AddEntry(h_7tev, "7 TeV", "l");
    leg->AddEntry(h_8tev, "8 TeV", "l");
  } else {
    leg->AddEntry(h_7tev, "7 TeV", "lp");
    leg->AddEntry(h_8tev, "8 TeV", "lp");
  }
  // 
  // get the ratio plots
  // 
  
  TH1F *hratio = h_7tev->Clone(Form("%s_7tevTo8tevratio", varName.Data()));
  hratio->Divide(h_8tev);
  
  if ( normalize) 
    hratio->GetYaxis()->SetRangeUser(0.5, 1.5);
  else 
    hratio->GetYaxis()->SetRangeUser(0.5, 1.5);
  hratio->GetYaxis()->SetLabelSize(0.1);
  hratio->GetXaxis()->SetLabelSize(0.1);
  hratio->GetXaxis()->SetTitleSize(0.1);
  hratio->GetXaxis()->SetTitleOffset(1.0);
  hratio->GetYaxis()->SetTitleOffset(0.65);
  hratio->GetYaxis()->SetTitleSize(0.12);
  hratio->GetXaxis()->SetTickLength( hratio->GetXaxis()->GetTickLength() * 3.0 );
  hratio->GetYaxis()->SetNdivisions(505);
  hratio->SetXTitle(x_title);
  hratio->SetYTitle("Ratio to 8 TeV");
  
  TLegend *leg_ratio = new TLegend(0.4, 0.7, 0.55, 0.95);
  leg_ratio->SetFillColor(0);
  leg_ratio->SetTextSize(0.06);
  if ( fromMCFM )
    leg_ratio->AddEntry(hratio, "7 TeV / 8 TeV", "l");
  else 
    leg_ratio->AddEntry(hratio, "7 TeV / 8 TeV", "lp");

  TLine *l0 = new TLine(hratio->GetXaxis()->GetXmin(), 1.0, hratio->GetXaxis()->GetXmax(), 1.0);
  TLine *l1 = new TLine(hratio->GetXaxis()->GetXmin(), 1.10, hratio->GetXaxis()->GetXmax(), 1.10);
  TLine *l2 = new TLine(hratio->GetXaxis()->GetXmin(), 0.90, hratio->GetXaxis()->GetXmax(), 0.90);
  

  l0->SetLineColor(kBlue);
  l0->SetLineStyle(kDashed);
  l0->SetLineWidth(2);

  l1->SetLineColor(kBlue);
  l1->SetLineStyle(kDashed);
  l1->SetLineWidth(2);

  l2->SetLineColor(kBlue);
  l2->SetLineStyle(kDashed);  
  l2->SetLineWidth(2);

  TCanvas *c1 = new TCanvas();
  c1->Divide(1,2,0.01,0.0);
  c1->cd(1);   
  gPad->SetPad( 0.0, 0.35, 1.0, 1.0 );
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->Update();
  h_7tev->SetMaximum(yMax * 1.2);
  if ( fromMCFM) {
    h_7tev->Draw("hist");
    h_8tev->Draw("samehist");
  }
  else {
    h_7tev->Draw("h");
    h_8tev->Draw("sameh");
  }
  leg->Draw("same");
  
  c1->cd(2); 
  gPad->SetPad( 0.0, 0.0, 1.0, 0.35 );
  gPad->SetBottomMargin(0.3);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();
  if ( fromMCFM )
    hratio->Draw("hist");
  else 
    hratio->Draw("histe");
  leg_ratio->Draw("same");
  if ( normalize) {
    //l0->Draw("same");
    l1->Draw("same");
    l2->Draw("same");
  }
  if ( normalize) {
    c1->SaveAs(Form("plots/epsfiles/%s/normalize/%s_norm.eps", plotDir.Data(), varName.Data()));
    c1->SaveAs(Form("plots/pngfiles/%s/normalize/%s_norm.png", plotDir.Data(), varName.Data()));
    
  }   else {
    c1->SaveAs(Form("plots/epsfiles/%s/straight/%s.eps", plotDir.Data(), varName.Data()));
    c1->SaveAs(Form("plots/pngfiles/%s/straight/%s.png", plotDir.Data(), varName.Data()));
  }

  delete c1;
  
  
}
