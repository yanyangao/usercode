void compCut()
{
  std::string fileName = "FILE";
  TFile *file = new TFile(fileName.c_str());

  TText* te;
  TCanvas *canvas = new TCanvas("",""); // has to add ("","") to avoid creating a new canvas?
  int do_CTF_SecTrk = DOCFTSECTRK; // 1: ctf; 2: sectrk 
  
  int normScale = NORMSCALE; // == 0: do nothing; 1: normalizeByEntries; 2: normalizeByIntegral 

  // Plots filled per event:
  createPlot(canvas, file, "perEvt", "n", 200, 0, 200, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perEvt", "nVertices", 10, 0, 10, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perEvt", "nPixelVertices", 100, 0, 100, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);

  // Plots filled per track
  createPlot(canvas, file, "perTrk",  "nHit", 40, 0, 40, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nLostHit", 10, 0, 10, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nPXBhit", 20, 0, 20, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nPXFhit", 20, 0, 20, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nTIBhit", 20, 0, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nTOBhit", 20, 0, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nTIDhit", 20, 0, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nTEChit", 20, 0, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nLayers", 30, 0, 30, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nPXBLayers", 10, 0, 10, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nPXFLayers", 10, 0, 10, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nPixelHit", 10, 0, 10, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "nLayers3D", 30, 0, 30, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
    
  createPlot(canvas, file, "perTrk",  "eta", 100, -4, 4, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "pt", 100, 0, 10, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "phi", 100, -3.5, 3.5, te,"UU",0.15,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "dxyCorr", 100, -0.5, 0.5, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "dz", 100, -20, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);

  createPlot(canvas, file, "perTrk",  "xPCA", 100, -0.5, 0.5, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "yPCA", 100, -0.5, 0.5, te,"UU",0.55,0.70,false,false,true,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "zPCA", 100, -20, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "chi2ndof", 100, 0.0, 10, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "algo", 10, 0.0, 10, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perTrk",  "isHighPurity", 2, 0.0, 2, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale); 

  // Plots filled per Real Vertex  
  createPlot(canvas, file, "perGoodPvtx",  "recx_pvtx", 100, -1, 1, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale); 
  createPlot(canvas, file, "perGoodPvtx",  "recy_pvtx", 100, -1, 1, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  createPlot(canvas, file, "perGoodPvtx",  "recz_pvtx", 100, -20, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);

  // Plots filled per Vertex
  createPlot(canvas, file, "perVtx",  "isFake_pvtx", 3, -1, 2, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  //createPlot(canvas, file, "perVtx",  "recz_pxlpvtx", 100, -20, 20, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);

  // Plots filled per SiStripHit
  createPlot(canvas, file, "perSiHit",  "cluster_charge", 100, 0, 1000, te,"UU",0.55,0.70,false,false,false,do_CTF_SecTrk, normScale);
  
  
}

void createPlot(TCanvas *canvas, TFile *file, TString type, TString name, int nbins, double xMin, double xMax, 
	        TText* te, char * option, 
		double startingY, double startingX = .1, bool fit = false, 
		bool logx=false, bool logy=false, int algo, int normScale){

  TTree *tree;
  tree = file->GetObject("tree",tree);
  
  TString hist_name;
  TH1F *h1 = new TH1F("h1", "h1", nbins, xMin, xMax);
  TH1F *h2 = new TH1F("h2", "h2", nbins, xMin, xMax);

  TString x_title = "";                   
  TString y_title = "Number of Tracks";  

  // If the histogram is filled per track, cut on the event and track.
  // Otherwise, cut only on the event level

  TString basecut="isTechBit40&&!isBeamHalo&&hasGoodPvtx==1";
  // Cut1 
  TString evtcutstring1=basecut;
  evtcutstring1.Append("&&EVTSELECTION1");
  TString trkcutstring1=basecut;
  trkcutstring1.Append("&&EVTSELECTION1&&TRKSELECTION1");
  // Cut2
  TString evtcutstring2=basecut;
  evtcutstring2.Append("&&EVTSELECTION2");
  TString trkcutstring2=basecut;
  trkcutstring2.Append("&&EVTSELECTION2&&TRKSELECTION2");

  if( name.Contains("cluster_charge",TString::kExact) ) {
    if(algo == 1)  hist_name = "ctf"; 
    if(algo == 2)  hist_name = "sectrk";
    hist_name.Append(name);   
    tree->Project("h1",hist_name, evtcutstring1);
    tree->Project("h2",hist_name, evtcutstring2);
    x_title = " Strip Cluster Charge (ADC Counts)";
    y_title = "Number of Clusters";
  }  
  // Number of Tracks
 else if( name == "n" ) {
    if(algo == 1)  hist_name = "ctf_"; 
    if(algo == 2)  hist_name = "sectrk_";
    hist_name.Append(name);   
    tree->Project("h1",hist_name, evtcutstring1);
    tree->Project("h2",hist_name, evtcutstring2);
    x_title = "Number of Tracks";
    y_title = "Number of Events";
  }

  // Filled per pxlpvtx
  else if (name == "recz_pxlpvtx") {
    tree->Project("h1",name, evtcutstring1);
    tree->Project("h2",name, evtcutstring2);
    y_title="Number of pixelVertices";
  }
  // Filled per pvtx
  else if( name.Contains("pvtx")) {
    if(name.Contains("rec")) {  
      y_title="Number of Real PrimaryVertices";
      TString vtxcutstring1=evtcutstring1; 
      TString vtxcutstring2=evtcutstring2;
      tree->Project("h1",name, vtxcutstring1.Append("&&isFake_pvtx==0"));
      tree->Project("h2",name, vtxcutstring2.Append("&&isFake_pvtx==0"));
    }
    else { 
      y_title="Number of Valid PrimaryVertices";
      tree->Project("h1",name, evtcutstring1);
      tree->Project("h2",name, evtcutstring2);
    }
  }
  // Filled per event other variables
  else if ( name.Contains("nVertices",TString::kExact) 
	    || name.Contains("hasGoodPvtx",TString::kExact) 
	    || name.Contains("nPixelVertices",TString::kExact) 
	    || name.Contains("isTechBit40",TString::kExact) 
	    || name.Contains("bsX0",TString::kExact)
	    || name.Contains("bsY0",TString::kExact)
	    || name.Contains("bsZ0",TString::kExact)
	    || name.Contains("bsSigmaZ",TString::kExact)
            || name.Contains("glob_ls",TString::kExact)
	    || name.Contains("glob_bx",TString::kExact)  
	    ) {
    tree->Project("h1",name, evtcutstring1);
    tree->Project("h2",name, evtcutstring2);
    y_title = "Number of Events";
  }
  // Filled per track
  else { 
    if(algo == 1)  hist_name = "ctf_"; 
    if(algo == 2)  hist_name = "sectrk_";
    hist_name.Append(name);   
    tree->Project("h1",hist_name, trkcutstring1);
    tree->Project("h2",hist_name, trkcutstring2);
  }


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
  if( name.Contains("nPXBLayers",TString::kExact) )  x_title = "Number of PixelBarrel Layers per Track" ;
  if( name.Contains("nPXFLayers",TString::kExact) )  x_title = "Number of PixEndcap Layers per Track" ;
  if( name.Contains("nLayers3D",TString::kExact) )  x_title = "Number of 3D Layers per Track" ;
  if( name.Contains("nPixelLayers",TString::kExact) )  x_title = "Number of Pixel Layers per Track" ;
  
  if( name.Contains("eta",TString::kExact) )  x_title = "Track Pseudorapidity" ;
  if( name.Contains("pt",TString::kExact) )  x_title = "p_{T} (GeV)" ;
  if( name.Contains("phi",TString::kExact) )  x_title = "Track Azimuthal Angle" ;
  if( name.Contains("dxy",TString::kExact) )  x_title = "Track dxy wrt (0,0,0) (cm)" ;
  if( name.Contains("dxyCorr",TString::kExact) )  x_title = "Track dxy wrt offline BS (cm)" ; 
  if( name.Contains("dz",TString::kExact) )  x_title = "Track dz wrt (0,0,0) (cm)" ;
  if( name.Contains("xPCA",TString::kExact) )  x_title = "x PCA (cm)" ;
  if( name.Contains("yPCA",TString::kExact) )  x_title = "y PCA (cm)" ;
  if( name.Contains("zPCA",TString::kExact) )  x_title = "z PCA (cm)" ;
  if( name.Contains("chi2ndof",TString::kExact) )  x_title = "#chi^{2}/ndf" ;
  if( name.Contains("algo",TString::kExact) )  x_title = "Track Algorithm" ;  
  if( name.Contains("isHighPurity",TString::kExact) )  x_title = "isHighPurity";
  
  // all cluster charge
  if( name.Contains("clusterCharge_all",TString::kExact) )  x_title = "Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TIB",TString::kExact) )  x_title = "TIB Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TID",TString::kExact) )  x_title = "TID Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TOB",TString::kExact) )  x_title = "TOB Cluster Charge (ADC Counts)";
  if( name.Contains("clusterCharge_TEC",TString::kExact) )  x_title = "TEC Cluster Charge (ADC Counts)";

  // pvtx stuff
  if( name.Contains("recx_pvtx",TString::kExact) )  x_title = "PrimaryVertex Position X (cm)" ;
  if( name.Contains("recy_pvtx",TString::kExact) )  x_title = "PrimaryVertex Position Y (cm)" ;
  if( name.Contains("recz_pvtx",TString::kExact) )  x_title = "PrimaryVertex Position Z (cm)" ;
  if( name.Contains("isFake_pvtx",TString::kExact) )  x_title = "PrimaryVertex isFake()";  
  if( name.Contains("nVertices",TString::kExact) )  x_title = "Number of PrimaryVertices";
  if( name.Contains("nPixelVertices",TString::kExact) )  x_title = "Number of pixelVertices";
  if( name.Contains("hasGoodPvtx",TString::kExact) )  x_title = "hasRealPrimaryVertex";
  
  //beamspot
  if( name.Contains("bsX0",TString::kExact) )  x_title = "BeamSpot X (cm)" ;
  if( name.Contains("bsY0",TString::kExact) )  x_title = "BeamSpot Y (cm)" ;
  if( name.Contains("bsZ0",TString::kExact) )  x_title = "BeamSpot Z (cm)" ;
  if( name.Contains("bsSigmaZ",TString::kExact) )  x_title = "BeamSpot SigmaZ (cm)" ;

  // TriggerBits
  if( name.Contains("isTechBit40",TString::kExact) )  x_title = "isTechBit40";
  if( name.Contains("isBSC",TString::kExact) )  x_title = "isBSC";
  if( name == "n" )  x_title = "Number of Tracks";


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
  
  h2->SetLineColor(2);
  h2->SetMarkerColor(2);
  h2->SetMarkerStyle(21);
  h2->SetMarkerSize(0.7);
  //h2->StatOverflows(kTRUE); 
  h2->SetLineWidth(3);
  h2->GetXaxis()->SetTitle(x_title);  
  h2->GetYaxis()->SetTitle(y_title);
  
  setStats(h1, h2, startingY,  startingX, fit);
  NormalizeHistograms(h1, h2, normScale);
  if(!logy&&!logx)
    fixRangeY(h1,h2);

  gPad->Update();
  h1->Draw();
  h2->Draw("sames");
  canvas->Update();
  
  TLegend *leg = new TLegend(0.16,0.93,0.60,0.995);
  leg->SetTextSize(0.03);
  leg->SetTextFont(42);
  leg->SetFillColor(0); 
  leg->SetBorderSize(1);
  leg->AddEntry(h1, "NEWLABEL" , "L" );
  leg->AddEntry(h2, "REFLABEL" , "L" );
  leg->Draw("SAME");

  TString pngdirname ="PNGDIR";
  TString pngfilename = pngdirname;
  pngfilename.Append(name);
  pngfilename.Append(".png");
  canvas->Print(pngfilename);

  TString epsdirname ="EPSDIR";
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

// Always normalize the high statistics sample to low statistic sample
void NormalizeHistograms(TH1F* h1, TH1F* h2, int SetScale) {

  if(SetScale == 0  || h1==0 || h2==0 )  return; // Do nothing

  // NormalizeByEntries
  else if (SetScale==1) { 
    if ( h1->GetEntries() != 0 && h2->GetEntries() != 0 ){
      //h1->Sumw2();
      //h2->Sumw2();
      if(h1->GetEntries()<h2->GetEntries())
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
