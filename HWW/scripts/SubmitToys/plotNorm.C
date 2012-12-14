void plotNormSingle(TString proc,TString inj,int jet, int mH, TString dir, TString ana);
void plotNorm() {
  int mH = 125; 
  TString inj = "125";
  TString dir_result = "~/scratch0/ana_PostHCP_2D_19fb/";
  TString ana = "hww";
  for ( int njet = 0; njet < 1; njet ++ ) {
    plotNormSingle("qqWW",inj,njet,mH,dir_result,ana);
    plotNormSingle("ggH" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("WjetsE" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("WjetsM" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Wgamma" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Wg3l" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Top" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("VV" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Ztt" ,inj,njet,mH,dir_result,ana);
  }
}

void plotNormSingle(TString proc,TString inj,int jet, int mH, TString dir, TString ana) {

  gROOT->Reset();
  float input = 0;
  float maxx  = 0;
  float hcp_s = 0;
  float hcp_b = 0;
  
  //THIS IS FOR 0J mH=125
  if (jet==0 && mH==125) {
    // process ZH      WH      qqH     ggH     qqWW       ggWW      VV      Top      Zjets  WjetsE   Wgamma    Wg3l    Ztt    WjetsM
    // rate   0.000   5.447   2.805  224.727  3749.844  197.973  128.573  453.141   0.000  334.868  108.992  178.605  40.347  497.622
    if (proc=="ZH")          { input = 0.0     ; maxx=1.0; }
    else if (proc=="WH")     { input = 5.447   ; maxx=1.0; }
    else if (proc=="qqH")    { input = 2.805   ; maxx=1.0; }
    else if (proc=="ggH")    { input = 224.724 ; maxx=1.0; }
    else if (proc=="qqWW")   { input = 3749.844; maxx=1.0; }
    else if (proc=="ggWW")   { input = 197.973 ; maxx=1.0; }
    else if (proc=="VV")     { input = 128.573 ; maxx=1.0; }
    else if (proc=="Top")    { input = 453.141 ; maxx=1.0; }
    else if (proc=="Zjets")  { input = 0.0     ; maxx=1.0; }
    else if (proc=="WjetsE") { input = 334.868 ; maxx=1.0; }
    else if (proc=="Wgamma") { input = 108.992 ; maxx=1.0; }
    else if (proc=="Wg3l")   { input = 178.605 ; maxx=1.0; }
    else if (proc=="Ztt")    { input = 40.347  ; maxx=1.0; }
    else if (proc=="WjetsM") { input = 492.622 ; maxx=1.0; }
    else return;
  }

  //THIS IS FOR 1J mH=125
  if (jet==1 && mH==125) {
    // process ZH      WH      qqH    ggH      qqWW     ggWW     VV       Top      Zjets  WjetsE    Wgamma Wg3l     Ztt    WjetsM
    // rate   0.000   6.797  10.828  88.974  1128.722  65.711  115.082  1370.905   0.000  158.203  28.190  27.770  69.111  220.397
    if (proc=="ZH")          { input = 0.0     ; maxx=1.0; }
    else if (proc=="WH")     { input = 6.797   ; maxx=1.0; }
    else if (proc=="qqH")    { input = 10.828   ; maxx=1.0; }
    else if (proc=="ggH")    { input = 88.974  ; maxx=1.0; }
    else if (proc=="qqWW")   { input = 1128.722; maxx=1.0; }
    else if (proc=="ggWW")   { input = 65.711  ; maxx=1.0; }
    else if (proc=="VV")     { input = 115.082 ; maxx=1.0; }
    else if (proc=="Top")    { input = 1370.905; maxx=1.0; }
    else if (proc=="Zjets")  { input = 0.0     ; maxx=1.0; }
    else if (proc=="WjetsE") { input = 158.203 ; maxx=1.0; }
    else if (proc=="Wgamma") { input = 28.190  ; maxx=1.0; }
    else if (proc=="Wg3l")   { input = 27.770  ; maxx=1.0; }
    else if (proc=="Ztt")    { input = 69.111  ; maxx=1.0; }
    else if (proc=="WjetsM") { input = 220.397 ; maxx=1.0; }
    else return;
  }

  gSystem->Exec(Form("grep -h %s %s/logsNorm/%i/logNorm_%s_%i_%sof_%ij_shape_8TeV_*.log >& tmp.txt",proc.Data(),dir.Data(),mH,inj.Data(),mH,ana.Data(),jet));
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);

  TCanvas c1;
  TH1F* h_proc_b = new TH1F(proc+"_bkg_fit",proc,50,-1.*maxx,maxx);
  TH1F* h_proc_s = new TH1F(proc+"_sig_fit",proc,50,-1.*maxx,maxx);

  ifstream in;
  in.open("tmp.txt");

  Float_t nbfit,nsfit;
  TString s1,s2;
  Int_t nlines = 0;
  
  cout << "input=" << input << endl;
  while (1) {
    in >> s1 >> s2 >> nsfit >> nbfit;
    if (nlines < 5) printf("s1=%s s2=%s nbfit=%.3f, nsfit=%.3f\n",s1.Data(),s2.Data(),nbfit,nsfit);
    if (!in.good()) break;
    //cout << (nsfit-input)/input << endl;
    h_proc_b->Fill( (nbfit-input)/input );
    h_proc_s->Fill( (nsfit-input)/input );
    nlines++;
  }
  printf(" found %d points\n",nlines);
  
  in.close();
  gSystem->Exec(Form("mkdir -p %s/plots",dir.Data()));
  gSystem->Exec("rm tmp.txt");

  h_proc_b->SetTitle("");
  h_proc_b->GetXaxis()->SetTitle("(N_{fit,b}-N_{in})/N_{in}");
  h_proc_b->GetYaxis()->SetTitle("toys/bin");
  h_proc_b->Draw();
  h_proc_b->Fit("gaus");
  /*
  TLine line_b((hcp_b-input)/input,0,(hcp_b-input)/input,h_proc_b->GetBinContent(h_proc_b->GetMaximumBin()));
  line_b.SetLineColor(kMagenta);
  line_b.SetLineWidth(2);
  line_b.Draw("same");
  */
  c1.SaveAs(Form("%s/plots/norm_inj%s_%ij_%i_bfit_%s_%s.png",dir.Data(),inj.Data(),jet,mH,proc.Data(),ana.Data()));
  c1.SaveAs(Form("%s/plots/norm_inj%s_%ij_%i_bfit_%s_%s.eps",dir.Data(),inj.Data(),jet,mH,proc.Data(),ana.Data()));

  h_proc_s->SetTitle("");
  h_proc_s->GetXaxis()->SetTitle("(N_{fit,s}-N_{in})/N_{in}");
  h_proc_s->GetYaxis()->SetTitle("toys/bin");
  h_proc_s->Draw();
  h_proc_s->Fit("gaus");
  /*
  TLine line_s((hcp_s-input)/input,0,(hcp_s-input)/input,h_proc_s->GetBinContent(h_proc_s->GetMaximumBin()));
  line_s.SetLineColor(kMagenta);
  line_s.SetLineWidth(2);
  line_s.Draw("same");
  */
  gPad->Update();
  // if (gPad->GetFrame()->GetY2()>10000) h_proc_s->GetYaxis()->SetRangeUser(0,1000);
  c1.SaveAs(Form("%s/plots/norm_inj%s_%ij_%i_sfit_%s_%s.png",dir.Data(),inj.Data(),jet,mH,proc.Data(),ana.Data()));
  c1.SaveAs(Form("%s/plots/norm_inj%s_%ij_%i_sfit_%s_%s.eps",dir.Data(),inj.Data(),jet,mH,proc.Data(),ana.Data()));
  
  
}
 
/*
root -b -q plotNorm.C\(\"qqWW\",\"125\",0,125\)
for proc in ggH qqWW ggWW Top Wjets Wgamma; do root -b -q plotNorm.C\(\"${proc}\",\"125\",0,125\); done
for inj in def 125 200; do for proc in ggH qqWW ggWW Top Wjets Wgamma; do root -b -q plotNorm.C\(\"${proc}\",\"${inj}\",0,125\); done; done
for nj in {0,1}; do for proc in ggH qqWW ggWW Top Wjets Wgamma; do root -b -q plotNorm.C\(\"${proc}\",\"125\",${nj},125\); done; done
*/
