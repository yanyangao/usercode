//
// This macro draws average of correlation matrix 
// * will generate two correlation plots
//   * correlation_all.png : with all nuisances 
//   * correlation_strip.png : with selected nuisances  
//                             selected nuisance parameters are defined in nuisance_out[nNui] 
// * for now, input directory is hard-coded which can be improved later  
//    TFile *File_tmp = TFile::Open(Form("../../logsNorm/125/mlfit_injm125_m125_0j_id%i.root", i), "READ");  
//
//  Usage : root -b -q correlation_avg.C
//

// number of toys generated  
const int nFiles = 1000; 

//
// List of nuisance parameters to be drawn  
//
const int nNui = 12;
char *nuisance_out[nNui] = {
   "CMS_hww_0j_WW_8TeV", 
   "QCDscale_WW_EXTRAP", 
   "CMS_hww_0j_ttbar_8TeV", 
   "CMS_hww_Wg3l", 
   "CMS_hww_MVAWEBounding", 
   "CMS_hww_MVAWMBounding",
   "FakeRate_e", 
   "FakeRate_m", 
   "QCDscale_ggH", 
   "QCDscale_ggH1in", 
   "QCDscale_ggH_ACCEPT", 
   "r"}; 

//
// Compares a string with list of strings (nuisance_out)
// If nui_histo exists in nui_list[nNui], it returns true.
// If not, it returns false.
//
bool comparenames(char* nui_list[nNui], char* nui_histo) { 

	bool match = false;

	for(int i=0; i<nNui; i++) { 
		TString str_nui_list = nui_list[i] ;
		TString str_nui_histo = nui_histo ;
		//if( str_nui_histo.Contains(str_nui_list) ) match = true; 
		if( str_nui_histo.EqualTo(str_nui_list) ) match = true; 
	}
	return match;
}

//
// B/W color scheme for the color blind
// from Dave who got from Dima
//
void loadColorPalette() {

    Int_t palette[17];
    palette[8] = kWhite;
    for (unsigned int i=0;i<8;++i){
        palette[7-i] = 19-i;
        palette[9+i] = 19-i;
    }
    gStyle->SetPalette(17,palette);

}

//
// Main function
//
TH2F* strip_histo(TH2F* h2in) {
 	
	// Style  
	gStyle->SetPaintTextFormat("6.2f"); 
	
        //  load B/W color scheme
//	loadColorPalette();

	//
	// strip off unnecessary X slices  
	//

	// Axes
	TAxis *axisX = h2in->GetXaxis(); 	 
	TAxis *axisY = h2in->GetYaxis();	
	int nbinsX = axisX->GetNbins(); 
	int nbinsY = axisY->GetNbins();
	
	// output histogram after stripping unnecessary X slices
	TH2F *h2outX 	= new TH2F("h2outX", "h2outX", nNui,0,nNui, nbinsY,0,nbinsY);
	int h2outXbin = 1; // bin index for h1outX
	
	for (int ibinX = 1; ibinX <= nbinsX; ibinX++) { 
		if( !comparenames(nuisance_out, axisX->GetBinLabel(ibinX)) ) continue;
			
		// project an X slice
		TH1D *h1Xslice = h2in->ProjectionY("h1Xslice",ibinX,ibinX);	 
		h2outX->GetXaxis()->SetBinLabel(h2outXbin, axisX->GetBinLabel(ibinX));
		// put them into 2D 
		for (int ibinY = 1; ibinY <= nbinsY; ibinY++) {  
			h2outX->SetBinContent(h2outXbin, ibinY, h1Xslice->GetBinContent(ibinY)); 
			h2outX->GetYaxis()->SetBinLabel(ibinY, axisY->GetBinLabel(ibinY)); 

		}
		h2outXbin++;
	} 
	
	//
	// strip off unnecessary Y slices  
	//
	
	// Axes
	TAxis *axisXoutX = h2outX->GetXaxis(); 	 
	TAxis *axisYoutX = h2outX->GetYaxis(); 	  
	int nbinsXoutX = axisXoutX->GetNbins();	
	int nbinsYoutX = axisYoutX->GetNbins();	 
	
	// output histogram after stripping unnecessary X slices
	TH2F *h2outXY 	= new TH2F("h2outXY", "h2outXY", nNui,0,nNui, nNui,0,nNui);
	int h2outYbin = 1; // bin index for h1outXY
	
	for (int ibinY = 1; ibinY <= nbinsYoutX; ibinY++) { 
		if( !comparenames(nuisance_out, axisYoutX->GetBinLabel(ibinY)) ) continue;
		
		// project an Y slice
		TH1D *h1Yslice = h2outX->ProjectionX("h1Yslice",ibinY,ibinY);	  
		
		h2outXY->GetYaxis()->SetBinLabel(h2outYbin, axisYoutX->GetBinLabel(ibinY));
		// put them into 2D 
		for (int ibinX = 1; ibinX <=nbinsXoutX; ibinX++) {  
			h2outXY->SetBinContent(ibinX, h2outYbin, h1Yslice->GetBinContent(ibinX)); 
			h2outXY->GetXaxis()->SetBinLabel(ibinX, axisXoutX->GetBinLabel(ibinX));
		}
		h2outYbin++;
	}
    
    return h2outXY;	
	
}

void correlation_avg() {

    // Style  
    gStyle->SetPaintTextFormat("6.2f"); 
    
    int nGoodFiles = 0; 
       
    TFile *File = TFile::Open(Form("../../logsNorm/125/mlfit_injm125_m125_0j_id%i.root", 0), "READ");
    RooFitResult *fit_s = (RooFitResult*) File->Get("fit_s");
    TH2F *h2NN   = (TH2F*) fit_s->correlationHist(); 

    nGoodFiles++;

    for(int i=1; i<nFiles; i++) {

        //cout << "... Adding  " << Form("../../logsNorm/125/mlfit_injm125_m125_0j_id%i.root", i)  << endl;
        
        TFile *File_tmp = TFile::Open(Form("../../logsNorm/125/mlfit_injm125_m125_0j_id%i.root", i), "READ");
        RooFitResult *fit_s_tmp = (RooFitResult*) File_tmp->Get("fit_s"); 
        if(!fit_s_tmp)  continue;
        TH2F *h2NN_tmp   = fit_s_tmp->correlationHist(); 

        h2NN->Add(h2NN_tmp); 

        File_tmp->Close();  
        //delete fit_s_tmp;
        //delete h2NN_tmp;
  
        nGoodFiles++;
    }

    cout << "\n\n" << endl; 
    cout << "-----------------------------------------------------" << endl;
    cout << " Number of input files      : " << nFiles << endl;
    cout << " Number of good input files : " << nGoodFiles << endl;
    cout << "-----------------------------------------------------\n\n" << endl;

    h2NN->Scale(1./(float)nGoodFiles); 
    TCanvas* c_all = new TCanvas("c_all","",800,600);
    h2NN->SetStats(0); 
    h2NN->SetTitle("Correlation"); 
    h2NN->Draw("colz"); 
    c_all->SaveAs("correlation_all.png");
    
    //
    // print large value bins
    //
    for(int i=0; i<141; i++) cout << "-";  cout << endl; 
    cout.width(15); cout <<"[ x, y] == ";    
    cout.width(50); cout << "Nuisance (X)" << " : "; 
    cout.width(50); cout << "Nuisance (Y)" << " : ";  
    cout.width(20); cout << "Correlation"  << endl;  
    for(int i=0; i<141; i++) cout << "-";  cout << endl; 

    for(int x=1; x<=h2NN->GetXaxis()->GetNbins(); x++)  {
        for(int y=1; y<=h2NN->GetXaxis()->GetNbins()+1-x; y++)  { 

            if(x+y == h2NN->GetXaxis()->GetNbins()+1) continue;

            if( TMath::Abs(h2NN->GetBinContent(x,y)) > 0.2)  {
	      cout.width(15);
              cout << Form("[ %i, %i] == ", x, y);    
	      cout.width(50);
              cout << h2NN->GetXaxis()->GetBinLabel(x) << " : "; 
	      cout.width(50);
              cout << h2NN->GetYaxis()->GetBinLabel(y) << " : ";  
	      cout.width(20);
              cout << h2NN->GetBinContent(x,y) << "  " 
                   << endl; 
            }
        }
    }
    for(int i=0; i<141; i++) cout << "-";  cout << endl; 
 
    // stripped version 
    TH2F* h2NNstrip = strip_histo(h2NN);

    // 
    // Draw and save histogram
    // 
    TCanvas* c_strip = new TCanvas("c","",800,600);
    c_strip->SetLeftMargin(0.25);
    c_strip->SetBottomMargin(0.2);
    c_strip->SetRightMargin(0.2);
    c_strip->SetGridx();
    c_strip->SetGridy();

    h2NNstrip->SetMinimum(-1.);
    h2NNstrip->SetMaximum(1.);
    h2NNstrip->SetMarkerSize(15./nNui);
    h2NNstrip->SetStats(0); 
    h2NNstrip->SetTitle("Correlation"); 
    h2NNstrip->Draw("colz text"); 

    // cosmetic : I like it 
    TGaxis *axistmp;
    axistmp = new TGaxis(0, nNui, nNui, nNui,0, 2, 50510,"+L");
    axistmp->SetNdivisions(0);
    axistmp->SetLabelFont(0);
    axistmp->Draw();
    
    c_strip->SaveAs("correlation_strip.png");

    delete c_all;
    delete c_strip;
}
