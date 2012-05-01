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

TString cuts = "nocuts";

void drawcomp()
{
  
  bool normalize = true;

  TString plotDir = Form("%s/", cuts.Data());
  TString fileName = Form("histos_wz_%s.root", cuts.Data());

  drawsingle(fileName, "mll_z", "dilepton mass from #gamma* [GeV]", normalize, plotDir, true);
  drawsingle(fileName, "drll_z", "#Delta R(dileptons from #gamma*) [radian]", normalize, plotDir, true);
  drawsingle(fileName, "leadleppt_z", "Leading lepton pT from #gamma* [GeV]", normalize, plotDir);
  drawsingle(fileName, "trailleppt_z", "Trailing lepton pT from #gamma* [GeV]", normalize, plotDir);
  drawsingle(fileName, "mlnu_w", "m(l#nu) from W [GeV]", normalize, plotDir);
  drawsingle(fileName, "wmass", "W generated mass [GeV]", normalize, plotDir);
  drawsingle(fileName, "leppt_w", "lepton pT from W [GeV]", normalize, plotDir);
  drawsingle(fileName, "lepeta_w", "lepton #eta from W", normalize, plotDir);

}
void drawsingle(TString fileName, TString varName, TString x_title, bool normalize, TString plotDir, bool logy=false) 
{
  // load macros  
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()"); 
  gROOT->ForceStyle();
  
  TFile *file = new TFile(fileName);
  
  TH1F *h_new = (TH1F*) file->Get(Form("%s_new", varName.Data()));
  TH1F *h_old = (TH1F*) file->Get(Form("%s_old", varName.Data()));
  
  h_old->SetLineColor(kBlack);
  h_new->SetLineColor(kBlue);

  h_old->SetMarkerColor(kBlack);
  h_new->SetMarkerColor(kBlue);

  h_old->GetYaxis()->SetNdivisions(505);
  h_new->GetYaxis()->SetNdivisions(505);
  
  h_old->SetLineWidth(3);
  h_new->SetLineWidth(3);

  h_old->SetMinimum(0.);
  h_new->SetMinimum(0.);

  TString y_title = "Number of Events";
  if ( normalize ) y_title = "Number of Events normalized to 1";
  h_old->SetXTitle(x_title);
  h_old->SetYTitle(y_title);

  if ( normalize) {
    h_new->Scale(1./h_new->Integral(0,1000));
    h_old->Scale(1./h_old->Integral(0,1000));
  }

  float yMax  = h_old->GetMaximum();
  yMax = yMax > h_new->GetMaximum() ? yMax : h_new->GetMaximum();

  TLegend *leg = new TLegend(0.6, 0.7, 0.85, 0.89);
  leg->SetFillColor(0);
  leg->SetTextSize(0.05);
  leg->AddEntry(h_new, "New: pp#rightarrow ee W", "lp");
  leg->AddEntry(h_old, "Old: pp#rightarrow ee l#nu", "lp");

  // 
  // get the ratio plots
  // 
  
  TH1F *hratio = h_new->Clone(Form("%s_newTooldratio", varName.Data()));
  hratio->Divide(h_old);
  
  if ( normalize) 
    hratio->GetYaxis()->SetRangeUser(0.5, 1.5);
  else 
    hratio->GetYaxis()->SetRangeUser(0.5, 2.5);
  hratio->GetYaxis()->SetLabelSize(0.1);
  hratio->GetXaxis()->SetLabelSize(0.1);
  hratio->GetXaxis()->SetTitleSize(0.1);
  hratio->GetXaxis()->SetTitleOffset(1.0);
  hratio->GetYaxis()->SetTitleOffset(0.65);
  hratio->GetYaxis()->SetTitleSize(0.12);
  hratio->GetXaxis()->SetTickLength( hratio->GetXaxis()->GetTickLength() * 3.0 );
  hratio->GetYaxis()->SetNdivisions(505);
  hratio->SetXTitle(x_title);
  hratio->SetYTitle("Ratio to old config");
  
  TLegend *leg_ratio = new TLegend(0.4, 0.7, 0.55, 0.95);
  leg_ratio->SetFillColor(0);
  leg_ratio->SetTextSize(0.1);
  leg_ratio->AddEntry(hratio, "new / old", "lp");

  TCanvas *c1 = new TCanvas();
  c1->Divide(1,2,0.01,0.0);
  c1->cd(1);   
  if ( logy) 
    gPad->SetLogy();
  gPad->SetPad( 0.0, 0.35, 1.0, 1.0 );
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->Update();
  h_old->SetMaximum(yMax * 1.2);
  h_old->Draw("h");
  h_new->Draw("sameh");
  leg->Draw("same");
  
  c1->cd(2); 
  gPad->SetLogy(0);
  gPad->SetPad( 0.0, 0.0, 1.0, 0.35 );
  gPad->SetBottomMargin(0.3);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();
  hratio->Draw("histe");
  leg_ratio->Draw("same");
  if ( normalize) {
    c1->SaveAs(Form("plots/epsfiles/%s/normalize/%s_norm.eps", plotDir.Data(), varName.Data()));
    c1->SaveAs(Form("plots/pngfiles/%s/normalize/%s_norm.png", plotDir.Data(), varName.Data()));
    
  }   else {
    c1->SaveAs(Form("plots/epsfiles/%s/straight/%s.eps", plotDir.Data(), varName.Data()));
    c1->SaveAs(Form("plots/pngfiles/%s/straight/%s.png", plotDir.Data(), varName.Data()));
  }

  delete c1;
  
  
}
