void SAMPLE()
{
  gROOT->ProcessLine(".L ~/tdrStyle.C");
  setTDRStyle();
  
  std::string fileName = "ROOTDIR/SAMPLE_GLOBALTAG_RELEASE.root";
  TFile *file = new TFile(fileName.c_str());

  TText* te;
  TCanvas *canvas = new TCanvas("",""); // has to add ("","") to avoid creating a new canvas?

  bool runSecTrackColl = RUNSECTRACKCOLL;
  int normScale = 0; // == 0: do nothing; 1: normalizeByEntries; 2: normalizeByIntegral 

  createPlot(canvas, file, "n", 301, -0.5, 300.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nHit", 40, -0.5, 39.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nLostHit", 10, -0.5, 9.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nPXBhit", 21, -0.5, 20.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "nPXFhit", 21, -0.5, 20.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "nTIBhit", 21, -0.5, 20.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nTOBhit", 21, -0.5, 20.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nTIDhit", 21, -0.5, 20.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nTEChit", 21, -0.5, 20.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);

  createPlot(canvas, file, "nLayers", 30, -0.5, 29.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "nPXBLayers", 10, -0.5, 9.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "nPXFLayers", 10, -0.5, 9.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  
  createPlot(canvas, file, "clusterCharge_all", 100, 0, 500000, runSecTrackColl, te,"UU",0.55,0.70,false,true,false,normScale);
  createPlot(canvas, file, "clusterCharge_TIB", 100, 0, 300000, runSecTrackColl, te,"UU",0.55,0.70,false,true,false,normScale);
  createPlot(canvas, file, "clusterCharge_TID", 100, 0, 300000, runSecTrackColl, te,"UU",0.55,0.70,false,true,false,normScale);
  createPlot(canvas, file, "clusterCharge_TOB", 100, 0, 300000, runSecTrackColl, te,"UU",0.55,0.70,false,true,false,normScale);
  createPlot(canvas, file, "clusterCharge_TEC", 100, 0, 300000, runSecTrackColl, te,"UU",0.55,0.70,false,true,false,normScale);

  createPlot(canvas, file, "eta", 100, -4, 4, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "pt", 100, 0, 100, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "phi", 100, -3.5, 3.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "dxy", 100, -10, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "dz", 100, -25, 25, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);

  createPlot(canvas, file, "xPCA", 100, -10, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "yPCA", 100, -10, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "zPCA", 100, -25, 25, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "chi2ndof", 100, 0.0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
}

void createPlot(TCanvas *canvas, TFile *file,  TString name, int nbins, double xMin, double xMax, 
		bool runSecTrackColl, TText* te, char * option, 
		double startingY, double startingX = .1, bool fit = false, 
		bool logx=false, bool logy=false, int normScale){
  TTree *tree;
  tree = file->GetObject("tree",tree);
  
  TH1F *h1; 
  h1 = new TH1F("h1", "h1", nbins, xMin, xMax);
  TString ctf_name = "ctf_";
  ctf_name.Append(name);
  tree->Project("h1",ctf_name);
  
  TH1F *h2;
  h2 = new TH1F("h2","h2",nbins,xMin,xMax);
  
  if(runSecTrackColl) {
    TString sectrk_name = "sectrk_";
    sectrk_name.Append(name);
    tree->Project("h2",sectrk_name);
  }
  TString x_title = "";
  TString y_title = "Number of Tracks";
  
  if( name=="n" ) {
    x_title = "Tracks per Event" ;
    y_title = "Number of Events";
  }
  
  if( name.Contains("nHit",TString::kExact) )  x_title = "Number of Valid Hits per Track" ;
  if( name.Contains("nLostHit",TString::kExact) )  x_title = "Number of Lost Hits per Track" ; 
  if( name.Contains("nPXBhit",TString::kExact) )  x_title = "Number of PixelBarrel Hits per Track" ;
  if( name.Contains("nPXFhit",TString::kExact) )  x_title = "Number of PixEndcap Hits per Track" ;
  if( name.Contains("nTIBhit",TString::kExact) )  x_title = "Number of TIB Hits per Track" ;
  if( name.Contains("nTOBhit",TString::kExact) )  x_title = "Number of TOB Hits per Track" ;
  if( name.Contains("nTIDhit",TString::kExact) )  x_title = "Number of TID Hits per Track" ;
  if( name.Contains("nTEChit",TString::kExact) )  x_title = "Number of TEC Hits per Track" ;

  if( name.Contains("nLayers",TString::kExact) )  x_title = "Number of Layers per Track" ;
  if( name.Contains("nPXBLayers",TString::kExact) )  x_title = "Number of PixelBarrel Layers per Track" ;
  if( name.Contains("nPXFLayers",TString::kExact) )  x_title = "Number of PixEndcap Layers per Track" ;


  if( name.Contains("clusterCharge_all",TString::kExact) )  x_title = "Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TIB",TString::kExact) )  x_title = "TIB Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TID",TString::kExact) )  x_title = "TID Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TOB",TString::kExact) )  x_title = "TOB Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TEC",TString::kExact) )  x_title = "TEC Cluster Charge (ADC Counts)";

  if( name.Contains("eta",TString::kExact) )  x_title = "Track Pseudorapidity" ;
  if( name.Contains("pt",TString::kExact) )  x_title = "p_{T} (GeV)" ;
  if( name.Contains("phi",TString::kExact) )  x_title = "Track Azimuthal Angle" ;
  if( name.Contains("dxy",TString::kExact) )  x_title = "Track dxy wrt (0,0,0) (cm)" ;
  if( name.Contains("dz",TString::kExact) )  x_title = "Track dz wrt (0,0,0) (cm)" ;
  if( name.Contains("xPCA",TString::kExact) )  x_title = "x PCA (cm)" ;
  if( name.Contains("yPCA",TString::kExact) )  x_title = "y PCA (cm)" ;
  if( name.Contains("zPCA",TString::kExact) )  x_title = "z PCA (cm)" ;
  if( name.Contains("chi2ndof",TString::kExact) )  x_title = "#chi^{2}/ndf" ;

  if(logx) gPad->SetLogx();
  else gPad->SetLogx(0);
  if(logy) gPad->SetLogy();
  else gPad->SetLogy(0);

  h1->SetLineColor(1);
  h1->SetMarkerColor(1);
  h1->SetMarkerStyle(21);
  h1->SetMarkerSize(0.7);
  h1->StatOverflows(kTRUE); 
  h1->SetLineWidth(3);
  h1->GetXaxis()->SetTitle(x_title); 
  h1->GetYaxis()->SetTitle(y_title);
  
  if(runSecTrackColl) {
    h2->SetLineColor(2);
    h2->SetMarkerColor(2);
    h2->SetMarkerStyle(21);
    h2->SetMarkerSize(0.7);
    h2->StatOverflows(kTRUE); 
    h2->SetLineWidth(3);
    h2->GetXaxis()->SetTitle(x_title);  
    h2->GetYaxis()->SetTitle(y_title);
  }

  if(runSecTrackColl) {
    setStats(h1, h2, startingY,  startingX, fit);
    NormalizeHistograms(h1, h2, normScale);
    fixRangeY(h1,h2);
  }
  else {
    setStats(h1, startingY,  startingX, fit);
  }
    
  gPad->Update();
  h1->Draw();
  if(runSecTrackColl) h2->Draw("sames");
  canvas->Update();
  
  TLegend *leg = new TLegend(0.16,0.93,0.60,0.995);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->SetFillColor(0); 
  leg->SetBorderSize(1);
  leg->AddEntry(h1, "TRACKCOLLECTION" , "L" );
  if(runSecTrackColl)
    leg->AddEntry(h2, "SECTRKCOLLECTION" , "L" );
  leg->Draw("SAME");

  TString dirname ="PNGDIR/SAMPLE/";
  TString filename = dirname;
  filename.Append(name);
  filename.Append(".png");
  canvas->Print(filename);

  filename.ReplaceAll("png","eps");
  canvas->Print(filename);




  delete h1;
  delete h2; 
}



void setStats(TH1* h1, double startingY, double startingX = .1,bool fit){
  if (startingY<0){
    h1->SetStats(0);
  } else {
    gStyle->SetOptStat(111110);
    //gStyle->SetOptStat(1110);

    if (fit){
      h1->Fit("gaus");
      TF1* f1 = (TF1*) h1->GetListOfFunctions()->FindObject("gaus");
      f1->SetLineColor(1);
      f1->SetLineWidth(1);
    }
    h1->Draw();
    gPad->Update(); 
    TPaveStats* st1 = (TPaveStats*) h1->GetListOfFunctions()->FindObject("stats");
    if (fit) {st1->SetOptFit(0010);    st1->SetOptStat(1001);}
    st1->SetX1NDC(startingX);
    st1->SetX2NDC(startingX+0.29);
    st1->SetY1NDC(startingY+0.22);
    st1->SetY2NDC(startingY+0.44);
    st1->SetTextColor(1);
    st1->SetTextSize(0.04);
  }
}

void NormalizeHistograms(TH1F* h1, TH1F* h2, int SetScale) {

  if(SetScale == 0  || h1==0 || h2==0 )  return; // Do nothing

  // NormalizeByEntries
  else if (SetScale==1) { 
    if ( h1->GetEntries() != 0 && h2->GetEntries() != 0 ){
      //h1->Sumw2();
      //h2->Sumw2();
      if(h1->GetEntries()>h2->GetEntries())
	h2->Scale(float(h1->GetEntries()/h2->GetEntries()));
      else
	h1->Scale(float(h2->GetEntries()/h1->GetEntries()));
    }
  }
  // NormalizeByIntegrals
  else if  (SetScale==2) {
    if ( h1->Integral() != 0 && h2->Integral() != 0 ){
      //h1->Sumw2();
      //h2->Sumw2();
      if(h1->Integral()>h2->Integral())
	h2->Scale(float(h1->Integral()/h2->Integral()));
      else
	h1->Scale(float(h2->Integral()/h1->Integral()));
    }
  }
}


void setStats(TH1* s,TH1* r, double startingY, double startingX = .1,bool fit){
  if (startingY<0){
    h1->SetStats(0);
    h2->SetStats(0);
  } else {
    gStyle->SetOptStat(111110);
    //gStyle->SetOptStat(1110);

    if (fit){
      h1->Fit("gaus");
      TF1* f1 = (TF1*) h1->GetListOfFunctions()->FindObject("gaus");
      f1->SetLineColor(2);
      f1->SetLineWidth(1);
    }
    h1->Draw();
    gPad->Update(); 
    TPaveStats* st1 = (TPaveStats*) h1->GetListOfFunctions()->FindObject("stats");
    if (fit) {st1->SetOptFit(0010);    st1->SetOptStat(1001);}
    st1->SetX1NDC(startingX);
    st1->SetX2NDC(startingX+0.29);
    st1->SetY1NDC(startingY+0.22);
    st1->SetY2NDC(startingY+0.44);
    st1->SetTextColor(4);
    st1->SetTextSize(0.04);
    if (fit) {
      h2->Fit("gaus");
      TF1* f2 = (TF1*) h2->GetListOfFunctions()->FindObject("gaus");
      f2->SetLineColor(4);
      f2->SetLineWidth(1);    
    }
    h2->Draw();
    gPad->Update(); 
    TPaveStats* st2 = (TPaveStats*) h2->GetListOfFunctions()->FindObject("stats");
    if (fit) {st2->SetOptFit(0010);    st2->SetOptStat(1001);}
    st2->SetX1NDC(startingX);
    st2->SetX2NDC(startingX+0.29);
    st2->SetY1NDC(startingY);
    st2->SetY2NDC(startingY+0.22);
    st2->SetTextColor(2);
    st2->SetTextSize(0.04);
  }
}


void fixRangeY(TH1* r,TH1* s){
  double ymin = (r->GetBinContent(r->GetMinimumBin()) < s->GetBinContent(s->GetMinimumBin())) ? 
    r->GetBinContent(r->GetMinimumBin()) : s->GetBinContent(s->GetMinimumBin());
  double ymax = (r->GetBinContent(r->GetMaximumBin()) > s->GetBinContent(s->GetMaximumBin())) ?
    r->GetBinContent(r->GetMaximumBin()) : s->GetBinContent(s->GetMaximumBin());
  r->GetYaxis()->SetRangeUser(ymin*0.9,ymax*1.1);
  s->GetYaxis()->SetRangeUser(ymin*0.9,ymax*1.1);
}
