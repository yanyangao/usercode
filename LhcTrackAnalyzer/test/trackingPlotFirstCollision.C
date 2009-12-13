void SAMPLE()
{
  //  gROOT->ProcessLine(".L /home/yygao/tdrStyle.C");
  // setTDRStyle();
  
  std::string fileName = "ROOTDIR/SAMPLE_GLOBALTAG_SEQUENCE.root";
  TFile *file = new TFile(fileName.c_str());

  TText* te;
  TCanvas *canvas = new TCanvas("",""); // has to add ("","") to avoid creating a new canvas?

  bool runSecTrackColl = RUNSECTRACKCOLL;
  int normScale = NORMSCALE; // == 0: do nothing; 1: normalizeByEntries; 2: normalizeByIntegral 

  
  // Plots filled per event
  createPlot(canvas, file, "perEvt", "n", 200, 0, 200, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perEvt", "nVertices", 20, 0, 20, false, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perEvt", "nPixelVertices", 20, 0, 20, false, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perEvt", "hasGoodPvtx", 2, 0, 2, false, te, "UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perEvt", "isBSC", 2, 0, 2, false, te, "UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perEvt", "isTechBit40", 2, 0, 2, false, te, "UU",0.55,0.70,false,false,false,normScale);
  //createPlot(canvas, file, "perEvt", "glob_ls", 20, 0, 20, false, te, "UU",0.55,0.70,false,false,false,normScale);  
  createPlot(canvas, file, "perEvt", "glob_bx", 100, 0, 10000, false, te, "UU",0.55,0.70,false,false,false,normScale);  
  createPlot(canvas, file, "perEvt", "bsX0", 10, -10, 10, false, te, "UU",0.55,0.70,false,false,false,normScale);  
  createPlot(canvas, file, "perEvt", "bsY0", 10, -10, 10, false, te, "UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perEvt", "bsZ0", 10, -10, 10, false, te, "UU",0.55,0.70,false,false,false,normScale);

  // Plots filled per track
  createPlot(canvas, file, "perTrk", "nHit", 40, 0, 40, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nLostHit", 10, 0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nPXBhit", 20, 0, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nPXFhit", 20, 0, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nTIBhit", 20, 0, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nTOBhit", 20, 0, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nTIDhit", 20, 0, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nTEChit", 20, 0, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nLayers", 30, 0, 30, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nLayers3D", 30, 0, 30, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);   
  createPlot(canvas, file, "perTrk", "nPXBLayers", 10, 0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nPXFLayers", 10, 0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "nPixelHit", 10, 0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);   
  createPlot(canvas, file, "perTrk", "eta", 100, -4, 4, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "pt", 100, 0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "phi", 100, -3.5, 3.5, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "dxy", 100, -1, 1, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "dz", 100, -20, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);

  createPlot(canvas, file, "perTrk", "dxyCorr", 100, -1, 1, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale); 
  createPlot(canvas, file, "perTrk", "dzCorr", 100, -20, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "xPCA", 100, -1, 1, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "yPCA", 100, -1, 1, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "zPCA", 100, -20, 20, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perTrk", "chi2ndof", 100, 0.0, 10, runSecTrackColl, te,"UU",0.55,0.70,false,false,true,normScale);
  createPlot(canvas, file, "perTrk", "algo", 20, -1, 19, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale); 
  createPlot(canvas, file, "perTrk", "isHighPurity", 2, 0, 2, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);  

  // Plots filled per SiStripHit
  createPlot(canvas, file, "perSiHit","cluster_charge", 100, 0, 1000, runSecTrackColl, te,"UU",0.55,0.70,false,false,false,normScale);

  // Plot filled per Valid Vertex
  createPlot(canvas, file, "perGoodPvtx", "recx_pvtx", 100, 0.1, 0.3, false, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perGoodPvtx", "recy_pvtx", 100, 0, 0.3, false, te,"UU",0.55,0.70,false,false,false,normScale);
  createPlot(canvas, file, "perGoodPvtx", "recz_pvtx", 100, -20, 20, false, te,"UU",0.55,0.70,false,false,false,normScale);
  // Plot filled per vertex
  createPlot(canvas, file, "perVtx", "isFake_pvtx", 2, 0, 2, false, te,"UU",0.55,0.70,false,false,false,normScale);
  
  createPlot(canvas, file, "perVtx", "recz_pxlpvtx", 100, -20, 20, false, te,"UU",0.55,0.70,false,false,false,normScale);
  
}

void createPlot(TCanvas *canvas, TFile *file,  TString type, TString name, int nbins, double xMin, double xMax, 
		bool runSecTrackColl, TText* te, char * option, 
		double startingY, double startingX = .1, bool fit = false, 
		bool logx=false, bool logy=false, int normScale){
  TTree *tree;
  tree = file->GetObject("tree",tree);
  
  TH1F *h1; // for ctf trk
  h1 = new TH1F("h1", "h1", nbins, xMin, xMax);
  
  TH1F *h2; // for sectrk
  h2 = new TH1F("h2","h2",nbins,xMin,xMax);
  

  TString x_title = "";
  TString y_title = "Number of Tracks";
  
  // If the histogram is filled per track, cut on the event and track.
  // Otherwise, cut only on the event level

  TString basecut="isTechBit40&&!isBeamHalo&&hasGoodPvtx==1";
  TString evtcutstring=basecut;
  evtcutstring.Append("&&EVTSELECTION");
  TString trkcutstring=basecut;
  trkcutstring.Append("&&EVTSELECTION&&TRKSELECTION");
  
  TString ctf_name =  "ctf_";

  if( name.Contains("cluster_charge",TString::kExact) ) {
    ctf_name = "ctf"; 
    ctf_name.Append(name);   
    tree->Project("h1",ctf_name, evtcutstring);
  }
  else if (name == "recz_pxlpvtx") {
    tree->Project("h1",name, evtcutstring);
    y_title="Number of pixelVertices";
  }
  else if( name.Contains("pvtx")) {
    if(name.Contains("rec")) {  
      y_title="Number of Real PrimaryVertices";
      TString vtxcutstring=evtcutstring;
      tree->Project("h1",name, vtxcutstring.Append("&&isFake_pvtx==0")); 
    }
    else { 
      y_title="Number of Valid PrimaryVertices";
      tree->Project("h1",name, evtcutstring);
    }
  }
  else if ( name.Contains("nVertices",TString::kExact) 
	    || name.Contains("hasGoodPvtx",TString::kExact) 
	    || name.Contains("nPixelVertices",TString::kExact) 
	    || name.Contains("isTechBit40",TString::kExact)
            || name.Contains("isBSC",TString::kExact)  
	    || name.Contains("bsX0",TString::kExact)
	    || name.Contains("bsY0",TString::kExact)
	    || name.Contains("bsZ0",TString::kExact)
	    || name.Contains("glob_ls",TString::kExact) 
	    || name.Contains("glob_bx",TString::kExact)   
	    ) {
    tree->Project("h1",name, evtcutstring);
    y_title = "Number of Events";
  }
  else { 
    ctf_name.Append(name); 
    tree->Project("h1",ctf_name, trkcutstring);
  }

  if(runSecTrackColl && ctf_name.Contains("ctf")) {
    TString sectrk_name = ctf_name.ReplaceAll("ctf","sectrk");
    tree->Project("h2",sectrk_name,evtcutstring);
  }
 
  // Set the x_tile 
  if( name == "n" ) {
    x_title = "Tracks per Event" ;
    y_title = "Number of Events";
  }
  
  // All Clusters
  if( name.Contains("clusterCharge_all",TString::kExact) )  x_title = "All Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TIB",TString::kExact) )  x_title = "All TIB Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TID",TString::kExact) )  x_title = "All TID Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TOB",TString::kExact) )  x_title = "All TOB Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TEC",TString::kExact) )  x_title = "All TEC Cluster Charge (ADC Counts)";
  
  if( name.Contains("cluster_charge",TString::kExact) ) {
    x_title = " Strip Cluster Charge (ADC Counts)";
    y_title = "Number of Clusters";
  }
    
  // Histograms filled per Track
  if( name.Contains("nHit",TString::kExact) )  x_title = "Number of Valid Hits per Track" ;
  if( name.Contains("nLostHit",TString::kExact) )  x_title = "Number of Lost Hits per Track" ; 
  if( name.Contains("nPXBhit",TString::kExact) )  x_title = "Number of PixelBarrel Hits per Track" ;
  if( name.Contains("nPXFhit",TString::kExact) )  x_title = "Number of PixEndcap Hits per Track" ;
  if( name.Contains("nTIBhit",TString::kExact) )  x_title = "Number of TIB Hits per Track" ;
  if( name.Contains("nTOBhit",TString::kExact) )  x_title = "Number of TOB Hits per Track" ;
  if( name.Contains("nTIDhit",TString::kExact) )  x_title = "Number of TID Hits per Track" ;
  if( name.Contains("nTEChit",TString::kExact) )  x_title = "Number of TEC Hits per Track" ;
  if( name.Contains("nPixelHit",TString::kExact) )  x_title = "Number of Pixel Hits per Track" ;

  if( name.Contains("nLayers",TString::kExact) )  x_title = "Number of Layers per Track" ;
  if( name.Contains("nLayers3D",TString::kExact) )  x_title = "Number of 3D Layers per Track" ;
  if( name.Contains("nPXBLayers",TString::kExact) )  x_title = "Number of PixelBarrel Layers per Track" ;
  if( name.Contains("nPXFLayers",TString::kExact) )  x_title = "Number of PixEndcap Layers per Track" ;
  if( name.Contains("eta",TString::kExact) )  x_title = "Track Pseudorapidity" ;
  if( name.Contains("pt",TString::kExact) )  x_title = "p_{T} (GeV)" ;
  if( name.Contains("phi",TString::kExact) )  x_title = "Track Azimuthal Angle" ;
  if( name.Contains("dxy",TString::kExact) )  x_title = "Track dxy wrt (0,0,0) (cm)" ;
  if( name.Contains("dz",TString::kExact) )  x_title = "Track dz wrt (0,0,0) (cm)" ;
  if( name.Contains("dxyCorr",TString::kExact) )  x_title = "Track dxy wrt BS (cm)" ;  
  if( name.Contains("dzCorr",TString::kExact) )  x_title = "Track dz wrt BS (cm)" ;  
  if( name.Contains("xPCA",TString::kExact) )  x_title = "x PCA (cm)" ;
  if( name.Contains("yPCA",TString::kExact) )  x_title = "y PCA (cm)" ;
  if( name.Contains("zPCA",TString::kExact) )  x_title = "z PCA (cm)" ;
  if( name.Contains("chi2ndof",TString::kExact) )  x_title = "#chi^{2}/ndf" ;
  if( name.Contains("algo",TString::kExact) )  x_title = "Track Algorithm" ;  
  if( name.Contains("isHighPurity",TString::kExact) )  x_title = "isHighPurity" ;  

  // pvtx stuff
  if( name.Contains("recx_pvtx",TString::kExact) )  x_title = "PrimaryVertex Position X (cm)" ;
  if( name.Contains("recy_pvtx",TString::kExact) )  x_title = "PrimaryVertex Position Y (cm)" ;
  if( name.Contains("recz_pvtx",TString::kExact) )  x_title = "PrimaryVertex Position Z (cm)" ;
  if( name.Contains("isFake_pvtx",TString::kExact) )  x_title = "PrimaryVertex isFake()";  
  if( name.Contains("recz_pxlpvtx",TString::kExact) )  x_title = "pixelVertices Position Z (cm)" ;

  //beamspot
  if( name.Contains("bsX0",TString::kExact) )  x_title = "BeamSpot X (cm)" ;
  if( name.Contains("bsY0",TString::kExact) )  x_title = "BeamSpot Y (cm)" ;
  if( name.Contains("bsZ0",TString::kExact) )  x_title = "BeamSpot Z (cm)" ;
   
  

  if( name.Contains("nVertices",TString::kExact) )  x_title = "Number of PrimaryVertices";
  if( name.Contains("nPixelVertices",TString::kExact) )  x_title = "Number of pixelVertices";
  if( name.Contains("hasGoodPvtx",TString::kExact) )  x_title = "hasRealPrimaryVertex";
  if( name.Contains("isTechBit40",TString::kExact) )  x_title = "isTechBit40";
  if( name.Contains("isBSC",TString::kExact) )  x_title = "isBSC";
  if( name.Contains("glob_bx",TString::kExact) )  x_title = "bunchCrossing";

  if(logx) gPad->SetLogx();
  else gPad->SetLogx(0);
  if(logy) gPad->SetLogy();
  else gPad->SetLogy(0);
  
  h1->SetLineColor(4);
  h1->SetMarkerColor(4);
  h1->SetMarkerStyle(21);
  h1->SetMarkerSize(0.7);
  //h1->StatOverflows(kTRUE); 
  h1->SetLineWidth(3);
  h1->GetXaxis()->SetTitle(x_title); 
  h1->GetYaxis()->SetTitle(y_title);
  
  if(runSecTrackColl) {
    h2->SetLineColor(2);
    h2->SetMarkerColor(2);
    h2->SetMarkerStyle(21);
    h2->SetMarkerSize(0.7);
    //h2->StatOverflows(kTRUE); 
    h2->SetLineWidth(3);
    h2->GetXaxis()->SetTitle(x_title);  
    h2->GetYaxis()->SetTitle(y_title);
  }

  if(runSecTrackColl) {
    setStats(h1, h2, startingY,  startingX, fit);
    NormalizeHistograms(h1, h2, normScale);
    if(!logy && !logx)
      fixRangeY(h1,h2); //I don't know why, with logy or logx, this line creates trouble in overlaying two plots if there is an Y=0 bin
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
  if(type == "perTrk" || name == "n" || name == "cluster_charge") 
    leg->Draw("SAME");
  
  TString pngdirname ="PNGDIR/SAMPLE/CUTSTRING/";
  TString pngfilename = pngdirname;
  pngfilename.Append(name);
  pngfilename.Append(".png");
  canvas->Print(pngfilename);

  TString epsdirname ="EPSDIR/SAMPLE/CUTSTRING/";
  TString epsfilename = epsdirname;
  epsfilename.Append(name);
  epsfilename.Append(".eps");
  canvas->Print(epsfilename);

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
