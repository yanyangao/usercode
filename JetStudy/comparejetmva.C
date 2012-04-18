//
// This script makes some simple comparison plots
// 

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


void readZEffsingle(TString zeffFileName, TString mode);

void comparejetmva()
{ 
  readZEffsingle("Zeff_nojetmva.root", "mm");
  readZEffsingle("Zeff_jetmva.root", "mm");
}

void readZEffsingle(TString zeffFileName, TString mode)
	      
{

  TFile *file = TFile::Open(zeffFileName);
  
  // find the bin that corresponds to the jetveto thresold
  // int jetvetobin = 4; // this corresponds to jet veto 30
  int jetvetobin = 4; // this corresponds to jet veto 35 GeV
  
  // do the data / MC ratio
  TH1F   *h_effdata[4];
  double eff_data[4];
  double efferr_data[4];

  TH1F   *h_effpowheg[4];
  double eff_powheg[4];
  double efferr_powheg[4];
  double effratio_powheg_[4];
  double effratio_powheg_err[4];

  
  TH1F   *h_effmadgraph[4];
  double eff_madgraph[4];
  double efferr_madgraph[4];
  double effratio_madgraph_[4];
  double effratio_madgraph_err[4];

  for ( int i = 0; i < 4; i++)
    {
      
      if ( mode == "all" ) {
	h_effdata[i]=(TH1F*) file->Get(Form("jetpteff_data_vtxbin%i",i));
	h_effmadgraph[i]=(TH1F*) file->Get(Form("jetpteff_madgraph_vtxbin%i",i));
      }

      if ( mode == "ee" ) {
	h_effdata[i]=(TH1F*) file->Get(Form("jetpteff_data_vtxbin%i_ee",i));
	h_effmadgraph[i]=(TH1F*) file->Get(Form("jetpteff_madgraph_vtxbin%i_ee",i));
      }

      if ( mode == "mm" ) {
	h_effdata[i]=(TH1F*) file->Get(Form("jetpteff_data_vtxbin%i_mm",i));
	h_effmadgraph[i]=(TH1F*) file->Get(Form("jetpteff_madgraph_vtxbin%i_mm",i));
      }
      

      eff_data[i] = h_effdata[i]->GetBinContent(jetvetobin);
      efferr_data[i] = h_effdata[i]->GetBinError(jetvetobin);
      
      eff_madgraph[i] = h_effmadgraph[i]->GetBinContent(jetvetobin);
      efferr_madgraph[i] = h_effmadgraph[i]->GetBinError(jetvetobin);
      effratio_madgraph_[i] = eff_data[i] / eff_madgraph[i];
      effratio_madgraph_err[i] = effratio_madgraph_[i] * sqrt( pow(efferr_data[i]/eff_data[i], 2) + pow(efferr_madgraph[i]/eff_madgraph[i], 2));

    }
  
  // print out tables
  std::cout << " ------------------ Tables for " << mode << "-----------------------\n"; 
  std::cout << "\\begin{table}[!htb]\n\\begin{center}\n\\begin{tabular}{|c|c|ccc|}\n\\hline\n";
  std::cout << " samples &  all vtx bins & nVtx $<=$ 10 &  10 $<$ nVtx $<$ 20 & nVtx [20-30] \\\\\n";
  std::cout << "\\hline\n";
  std::cout << Form("data & {\\color{red} %4.2f $\\pm$ %4.2f} & %4.2f $\\pm$ %4.2f & %4.2f $\\pm$ %4.2f & {\\color{blue}%4.2f $\\pm$ %4.2f} \\\\\n",
		    eff_data[0], efferr_data[0], eff_data[1], efferr_data[1], eff_data[2], efferr_data[2], eff_data[3], efferr_data[3]);  
  std::cout << Form("madgraph MC & {\\color{red}%4.2f $\\pm$ %4.2f} & %4.2f $\\pm$ %4.2f & %4.2f $\\pm$ %4.2f & {\\color{blue}%4.2f $\\pm$ %4.2f} \\\\\n",
		    eff_madgraph[0], efferr_madgraph[0], eff_madgraph[1], efferr_madgraph[1], eff_madgraph[2], efferr_madgraph[2], eff_madgraph[3], efferr_madgraph[3]);  

  std::cout << "\\hline\n";
  std::cout << Form("data/madgraph MC & {\\color{red}%4.2f $\\pm$ %4.2f} & %4.2f $\\pm$ %4.2f & %4.2f $\\pm$ %4.2f & {\\color{blue}%4.2f $\\pm$ %4.2f} \\\\\n",
		    effratio_madgraph_[0], effratio_madgraph_err[0], effratio_madgraph_[1], effratio_madgraph_err[1], effratio_madgraph_[2], effratio_madgraph_err[2], effratio_madgraph_[3], effratio_madgraph_err[3]);  


  std::cout << "\\hline\n";
  std::cout << "\\end{tabular}\n";
  std::cout << "\\end{center}\n";
  std::cout << "\\end{table}\n";
  std::cout << " ---------------------------------------------------\n";

}

int findBin(float value, TH1F *hist) 
{
  for (int i = 1; i < hist->GetNbinsX() + 1; i++ )
    {
      if ( (hist->GetBinLowEdge(i) + hist->GetBinWidth(i)) == value)
	return i;
      else 
	return 0;
    }
}

