void plotNormSingle(TString proc,TString inj,int jet, int mH, TString dir, TString ana);
void plotNorm() {
  int mH = 125; 
  TString inj = "125";
  TString dir_result = "/afs/cern.ch/user/y/yygao/scratch0/hwwjcp_19fb_nosiginstshapesyst/";
  TString ana = "xww";
  for ( int njet = 0; njet < 1; njet ++ ) {
    plotNormSingle("qqWW",inj,njet,mH,dir_result,ana);
    plotNormSingle("ggWW",inj,njet,mH,dir_result,ana);
    plotNormSingle("ggH" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("WjetsE" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("WjetsM" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Wgamma" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Wg3l" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Top" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("VV" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("Ztt" ,inj,njet,mH,dir_result,ana);
    plotNormSingle("PullMu",     inj, njet, mH, dir_result, ana);
  }
}

void plotNormSingle(TString proc,TString inj,int jet, int mH, TString dir, TString ana) {

  gROOT->Reset();
  float input = 0;
  float maxx  = 0;
  float hcp_s = 0;
  float hcp_b = 0;

  float yield_ZH(0.), yield_WH(0.), yield_qqH(0.), yield_ggH(0.);
  float yield_qqWW(0.), yield_ggWW(0.), yield_VV(0.), yield_Top(0.0), yield_Zjets(0.0), yield_WjetsE(0.), yield_Wgamma(0.0), yield_Wg3l(0.0), yield_Ztt(0.), yield_WjetsM(0.); 
  
  TString dir_cards="../../";
  TString datacardName = Form("%s/%i/%sof_%ij_shape_8TeV.txt",dir_cards.Data(),mH,ana.Data(),jet);
  std::cout << datacardName << "\n";
  ifstream incard (datacardName);
  string line;
  
  if (incard.is_open()) {
    while ( incard.good() ) {
      getline (incard,line);
      size_t found=line.find("rate");
      if (found!=string::npos) {
	std::cout << line << std::endl;
	stringstream stream(line);
	string rate;
	stream >> rate ;
	stream >> yield_ZH; 
	stream >> yield_WH;
	stream >> yield_qqH;
	stream >> yield_ggH;
	stream >> yield_qqWW;
	stream >> yield_ggWW;
	stream >> yield_VV;    
	stream >> yield_Top;
	stream >> yield_Zjets;
	stream >> yield_WjetsE;
	stream >> yield_Wgamma;
	stream >> yield_Wg3l;
	stream >> yield_Ztt;
	stream >> yield_WjetsM;
      }
    }
    incard.close();
  }
  
  if (proc=="ZH")            { input = yield_ZH     ; maxx=1.0; }
    else if (proc=="WH")     { input = yield_WH     ; maxx=1.0; }
    else if (proc=="qqH")    { input = yield_qqH    ; maxx=1.0; }
    else if (proc=="ggH")    { input = yield_ggH    ; maxx=1.0; }
    else if (proc=="qqWW")   { input = yield_qqWW   ; maxx=1.0; }
    else if (proc=="ggWW")   { input = yield_ggWW   ; maxx=1.0; }
    else if (proc=="VV")     { input = yield_VV     ; maxx=1.0; }
    else if (proc=="Top")    { input = yield_Top    ; maxx=1.0; }
    else if (proc=="Zjets")  { input = yield_Zjets  ; maxx=1.0; }
    else if (proc=="WjetsE") { input = yield_WjetsE ; maxx=1.0; }
    else if (proc=="Wgamma") { input = yield_Wgamma ; maxx=1.0; }
    else if (proc=="Wg3l")   { input = yield_Wg3l   ; maxx=1.0; }
    else if (proc=="Ztt")    { input = yield_Ztt    ; maxx=1.0; }
    else if (proc=="WjetsM") { input = yield_WjetsM ; maxx=1.0; }
    else if (proc=="PullMu") { input = yield_ZH+yield_WH+yield_qqH+yield_ggH; maxx=5.0; }
    else return;
  

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


  //Pull for Mu
  if(proc=="PullMu") { 
    for(int i=1; i<1000; i++) {
      TFile *File = TFile::Open(Form("%s/logsNorm/%i/mlfit_injm%s_m%i_%sof_%ij_id%i.root", dir.Data(), mH, inj.Data(), mH, ana.Data(), jet, i), "READ");
      RooFitResult *fit_s = (RooFitResult*) File->Get("fit_s");
      if(!fit_s)  { File->Close(); continue; }
      if(fit_s->status() != 0) { File->Close(); continue; } // fit status == 0 : requires fit quality
      RooRealVar *r = (RooRealVar*) fit_s->floatParsFinal().find("r");
      h_proc_s->Fill( ( r->getVal() - 1. ) / r->getError() ); 
      //cout <<  r->getVal() << " +/- " <<  r->getError() << " ==> " <<  ( r->getVal() - 1. ) / r->getError() << endl;  
      File->Close();
    }
  }

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
  if(proc != "PullMu") h_proc_s->GetXaxis()->SetTitle("(N_{fit,s}-N_{in})/N_{in}");
  else h_proc_s->GetXaxis()->SetTitle("(fitted #mu - 1)/#sigma_{fitted #mu}");
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
