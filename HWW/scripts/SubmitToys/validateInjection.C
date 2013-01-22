// 
// originally from 
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/GCerati/HWWScripts/validateInjection.C
// 

void validateInjection() {
  
  TString inputcarddir = "../../";
  TString toydatadir="../../";

  for ( int njet = 0 ; njet < 2 ; njet ++ ) {
    validateInjectionSingle(inputcarddir, toydatadir, "hww", njet);
    validateInjectionSingle(inputcarddir, toydatadir, "xww", njet);
  }
}
void validateInjectionSingle(TString inputcarddir = "../../", TString toydatadir="../../", TString ana="hww", int njet = 0)
{

  gROOT->Reset();
  gStyle->SetOptStat(0);

  int mH = 125;

  TCanvas *c1 = new TCanvas();

  // 
  // Get the input carddir
  // 
  TFile *_file_ref = TFile::Open(Form("%s/%i/%sof_%ij.input_8TeV.root", inputcarddir.Data(), mH, ana.Data(), njet));
  THStack hs("hs","processes");

  TH1F* ZH = (TH1F*) _file_ref->Get("histo_ZH");
  TH1F* WH = (TH1F*) _file_ref->Get("histo_WH");
  TH1F* ggH = (TH1F*) _file_ref->Get("histo_ggH");
  TH1F* qqH = (TH1F*) _file_ref->Get("histo_qqH");
  TH1F* ggWW = (TH1F*) _file_ref->Get("histo_ggWW");
  TH1F* qqWW = (TH1F*) _file_ref->Get("histo_qqWW");
  TH1F* VV = (TH1F*) _file_ref->Get("histo_VV");
  TH1F* Top = (TH1F*) _file_ref->Get("histo_Top");
  TH1F* WjetsE = (TH1F*) _file_ref->Get("histo_WjetsE");
  TH1F* WjetsM = (TH1F*) _file_ref->Get("histo_WjetsM");
  TH1F* Zjets = (TH1F*) _file_ref->Get("histo_Zjets");
  TH1F* Ztt = (TH1F*) _file_ref->Get("histo_Ztt");
  TH1F* Wgamma = (TH1F*) _file_ref->Get("histo_Wgamma");
  TH1F* Wg3l = (TH1F*) _file_ref->Get("histo_Wg3l");


  ZH->SetLineColor(kRed);
  WH->SetLineColor(kRed);
  ggH->SetLineColor(kRed);
  qqH->SetLineColor(kRed);
  ggWW->SetLineColor(kAzure-9);
  qqWW->SetLineColor(kAzure-9);
  VV->SetLineColor(kAzure-2);
  Top->SetLineColor(kYellow);
  WjetsE->SetLineColor(kGray+1);
  WjetsM->SetLineColor(kGray+1);
  Zjets->SetLineColor(kGreen+2);
  Ztt->SetLineColor(kGreen+2);
  Wgamma->SetLineColor(kGray+1);
  Wg3l->SetLineColor(kGray+1);
  ZH->SetFillColor(kRed);
  WH->SetFillColor(kRed);
  ggH->SetFillColor(kRed);
  qqH->SetFillColor(kRed);
  ggWW->SetFillColor(kAzure-9);
  qqWW->SetFillColor(kAzure-9);
  VV->SetFillColor(kAzure-2);
  Top->SetFillColor(kYellow);
  WjetsE->SetFillColor(kGray+1);
  WjetsM->SetFillColor(kGray+1);
  Zjets->SetFillColor(kGreen+2);
  Ztt->SetFillColor(kGreen+2);
  Wgamma->SetFillColor(kGray+1);
  Wg3l->SetFillColor(kGray+1);
  ZH->SetFillStyle(1001);
  WH->SetFillStyle(1001);
  ggH->SetFillStyle(1001);
  qqH->SetFillStyle(1001);
  ggWW->SetFillStyle(1001);
  qqWW->SetFillStyle(1001);
  VV->SetFillStyle(1001);
  Top->SetFillStyle(1001);
  WjetsE->SetFillStyle(1001);
  WjetsM->SetFillStyle(1001);
  Zjets->SetFillStyle(1001);
  Ztt->SetFillStyle(1001);
  Wgamma->SetFillStyle(1001);
  Wg3l->SetFillStyle(1001);

  hs.Add(ggWW);
  hs.Add(qqWW);
  hs.Add(VV);
  hs.Add(Top);
  hs.Add(WjetsE);
  hs.Add(WjetsM);
  hs.Add(Wgamma);
  hs.Add(Wg3l);
  hs.Add(Zjets);
  hs.Add(Ztt);
  hs.Add(ZH);
  hs.Add(WH);
  hs.Add(ggH);
  hs.Add(qqH);
  // hs.Draw("hist");
  
  TH1F* hinj = new TH1F("hinj","hinj", ggH->GetNbinsX(), ggH->GetXaxis()->GetXmin(), ggH->GetXaxis()->GetXmax()); 
  for (int i=0;i<1000;++i) {
    TFile *_file_inj = TFile::Open(Form("%s/125/%sof_%ij_shape_8TeV_PseudoData_sb_seed12344.root", toydatadir.Data(), ana.Data(), njet));
    TH1F* h = (TH1F*) _file_inj->Get(Form("j%iof_%i",njet,i));
    hinj->Add(h);
    _file_inj->Close();
  }
  hinj->Scale(1./1000.);
  hinj->SetLineWidth(2);
  hinj->SetTitle();
  hinj->GetXaxis()->SetTitle("unrolled");
  hinj->GetYaxis()->SetTitle("events");
  hinj->GetYaxis()->SetRangeUser(0, TMath::Max(hinj->GetMaximum(), hs.GetMaximum())*1.1);
  hinj->Draw();

  hs.Draw("samehist");
  hinj->Draw("same");
  
  c1->SaveAs(Form("toys_%sof_%ij_injmH%i.eps", ana.Data(), njet, mH));
  c1->SaveAs(Form("toys_%sof_%ij_injmH%i.png", ana.Data(), njet, mH));

}
