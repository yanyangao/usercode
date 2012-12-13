void plotNormSingle(TString proc,TString inj,int jet, int mH, TString dir, TString ana);
void plotNorm() {
  int mH = 125; 
  TString inj = "125";
  TString dir_result = "~/scratch0/HCP_Injection/";
  TString ana = "hww";
  plotNormSingle("qqWW",inj,0,mH,dir_result,ana);
  plotNormSingle("ggH" ,inj,0,mH,dir_result,ana);
  plotNormSingle("Wjets" ,inj,0,mH,dir_result,ana);
  plotNormSingle("Wgamma" ,inj,0,mH,dir_result,ana);
  plotNormSingle("Wg3l" ,inj,0,mH,dir_result,ana);
  plotNormSingle("Top" ,inj,0,mH,dir_result,ana);
  plotNormSingle("VV" ,inj,0,mH,dir_result,ana);
  plotNormSingle("Ztt" ,inj,0,mH,dir_result,ana);
}

void plotNormSingle(TString proc,TString inj,int jet, int mH, TString dir, TString ana) {

  // process                            ZH    WH   qqH   ggH      qqWW    ggWW      VV    Top     Zjets Wjets    Wgamma Wg3l	Ztt	
  // rate                             1.471 5.123 2.622 210.631 3616.214 190.711 120.761 390.807 0.000 831.409 100.719 164.744 39.078

  gROOT->Reset();
  float input = 0;
  float maxx  = 0;
  float hcp_s = 0;
  float hcp_b = 0;
  
  //THIS IS FOR 0J mH=125
  if (jet==0 && mH==125) {
    if (proc=="ZH")          { input = 1.471   ; maxx=1.0; }
    else if (proc=="WH")     { input = 5.123   ; maxx=1.0; }
    else if (proc=="qqH")    { input = 2.622   ; maxx=1.0; }
    else if (proc=="ggH")    { input = 210.631 ; maxx=3.0; }
    else if (proc=="qqWW")   { input = 3616.214; maxx=1.0; }
    else if (proc=="ggWW")   { input = 190.711 ; maxx=1.0; }
    else if (proc=="VV")     { input = 120.761 ; maxx=1.0; }
    else if (proc=="Top")    { input = 390.807 ; maxx=1.0; }
    else if (proc=="Zjets")  { input = 0.0     ; maxx=1.0; }
    else if (proc=="Wjets")  { input = 831.409 ; maxx=1.0; }
    else if (proc=="Wgamma") { input = 100.719 ; maxx=1.0; }
    else if (proc=="Wg3l")   { input = 164.744 ; maxx=1.0; }
    else if (proc=="Ztt")    { input = 39.078  ; maxx=1.0; }
    else return;
  }

  gSystem->Exec(Form("grep -h %s %s/logsNorm/%i/logNorm_%s_%i_%sof_%ij_shape_8TeV_*.log >& tmp.txt",proc.Data(),dir.Data(),mH,inj.Data(),mH,ana.Data(),jet));
  std::cout << Form("grep -h %s %s/logsNorm/%i/logNorm_%s_%i_%sof_%ij_shape_8TeV_*.log >& tmp.txt",proc.Data(),dir.Data(),mH,inj.Data(),mH,ana.Data(),jet) << "\n";

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
  if (gPad->GetFrame()->GetY2()>1000) h_proc_s->GetYaxis()->SetRangeUser(0,1000);
  c1.SaveAs(Form("%s/plots/norm_inj%s_%ij_%i_sfit_%s_%s.png",dir.Data(),inj.Data(),jet,mH,proc.Data(),ana.Data()));
  c1.SaveAs(Form("%s/plots/norm_inj%s_%ij_%i_sfit_%s_%s.eps",dir.Data(),inj.Data(),jet,mH,proc.Data(),ana.Data()));
  
  
}
 
/*
root -b -q plotNorm.C\(\"qqWW\",\"125\",0,125\)
for proc in ggH qqWW ggWW Top Wjets Wgamma; do root -b -q plotNorm.C\(\"${proc}\",\"125\",0,125\); done
for inj in def 125 200; do for proc in ggH qqWW ggWW Top Wjets Wgamma; do root -b -q plotNorm.C\(\"${proc}\",\"${inj}\",0,125\); done; done
for nj in {0,1}; do for proc in ggH qqWW ggWW Top Wjets Wgamma; do root -b -q plotNorm.C\(\"${proc}\",\"125\",${nj},125\); done; done
*/
