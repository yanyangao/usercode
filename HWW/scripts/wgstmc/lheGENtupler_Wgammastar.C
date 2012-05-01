
//
// 
// 

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TLorentzVector.h>
#endif

#include "GenStructDefs.hh"

TGenWGammastarData data;

bool verbose = 1;

//====================================================================================================

void lheGENtupler_Wgammastar(Int_t Option = 0) {

  gBenchmark->Start("lheGENtupler");
  
  vector<string> fnamev;
  string outfname; 
  if (Option == 0) {
    outfname = "wz_gentuple_ee_old_test.root";
    fnamev.push_back("test_old.lhe");
    /*
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_1_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_2_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_3_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_4_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_5_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_6_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_7_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_8_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_9_unweighted_events.lhe");
    fnamev.push_back("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE/gridpack_wgammastarToEE8TeV_10_unweighted_events.lhe");
    
    int nfiles = 76;
    for (int nfile = 1; nfile < nfiles; nfile++) 
      fnamev.push_back(Form("/smurf/sixie/MCGenerators/Madgraph/WGammaStar8TeV/EE_OLD/gridpack_wgammastarToEE8TeV_%i_unweighted_events.lhe", nfile));
    */
    
  } else if (Option == 1) { 
    outfname = "wz_gentuple_mm.root";
    fnamev.push_back("LHEFiles/gridpack_wgammastarToMuMu_1_unweighted_events.lhe");
    fnamev.push_back("LHEFiles/gridpack_wgammastarToMuMu_2_unweighted_events.lhe");
  }

  
  TFile* outfile = new TFile(outfname.c_str(),"RECREATE");
  TTree::SetMaxTreeSize(kMaxLong64);
  TTree* tree = new TTree("Events","Events");  
  tree->Branch("Events",&data.npho,
               "npho/i:scalePdf/F:weight:wmass:wpt:wy:wphi:zmass:zpt:zy:zphi:mass:pt:y:phi:mt:pt_1:eta_1:phi_1:pt_2:eta_2:phi_2:q1/I:q2:pt_zlepton1/F:eta_zlepton1:phi_zlepton1:pdgid_zlepton1/I:pt_zlepton2/F:eta_zlepton2:phi_zlepton2:pdgid_zlepton2/I:pt_wlepton/F:eta_wlepton:phi_wlepton:pdgid_wlepton/I:pt_wneutrino/F:eta_wneutrino:phi_wneutrino:fourLeptonMass:wlnumass:zllmass:x_1:x_2:id_1/I:id_2:acqd/F:aqed"); 
    
  const Int_t bosonID =-24;
  const Int_t lepID1  = 13;
  const Int_t lepID2  =-14;
  
  //--------------------------------------------------------------------------------------------------
  
  for(UInt_t ifile=0; ifile<fnamev.size(); ifile++) {
  
    const char* lhefile = fnamev[ifile].c_str();
    ifstream ifs;
    ifs.open(lhefile);
    assert(ifs.is_open()); 
  
    string line; 
        
    cout << "Processing file: " << fnamev[ifile] << endl;
    
    do {
      getline(ifs,line);
    } while(line.compare("<init>")!=0);
    
    getline(ifs,line);
    stringstream ss1(line);
    int idbmup1, idbmup2; 
    double ebmup1, ebmup2; 
    int pdfgup1, pdfgup2, pdfsup1, pdfsup2, idwtup, nprup;
    ss1 >> idbmup1 >> idbmup2 >> ebmup1 >> ebmup2 >> pdfgup1 >> pdfgup2 >> pdfsup1 >> pdfsup2 >> idwtup >> nprup;
    
    getline(ifs,line);
    stringstream ss2(line);
    double xsecup, xerrup, xmaxup;
    int lprup;
    ss2 >> xsecup >> xerrup >> xmaxup >> lprup;
    getline(ifs,line);
    ss2 >> xsecup >> xerrup >> xmaxup >> lprup;
    
    getline(ifs,line);
    assert(line.compare("</init>")==0);
    
    int count = 0;

    while(getline(ifs,line)) {
           
      if(line.compare("<event>")==0) {
	if ( count%10000 == 0 ) 
	  std::cout << "Processing " << count << " entries...\n"; 
	
	if ( verbose ) {
	  std::cout << "--------------------------------------\n"; 
	  std::cout << "Processing " << count << " entries...\n"; 
	}
        getline(ifs,line);
	stringstream ss3(line);
	int nup, idprup;
	double xwgtup, scalup, aqedup, aqcdup;
	ss3 >> nup >> idprup >> xwgtup >> scalup >> aqedup >> aqcdup;
	data.scalePdf = scalup;
	data.weight   = xwgtup;
	data.aqcd     = aqcdup;
	data.aqed     = aqedup;
	
        if (!(nup >= 6 && nup <= 8)) cout << "Error: number of particles is not right \n" 
                                       << nup << " " << idprup << " " << xwgtup 
                                       << " " << scalup << " " << aqedup << " " << aqcdup << endl;

	if ( verbose) 
	  cout << nup << " " << idprup << " " << xwgtup 
	       << " " << scalup << " " << aqedup << " " << aqcdup << endl;
	
	
	int idup, istup, mothup1, mothup2, icolup1, icolup2; 
	double  pup1, pup2, pup3, pup4, pup5, vtimup, spinup;
	
	// quark 1 info
        getline(ifs,line);
        stringstream ssq1(line);
	ssq1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
	data.x_1  = fabs(pup3/ebmup1);
	data.id_1 = idup;
	
	// quark 2 info
        getline(ifs,line);
        stringstream ssq2(line);
	ssq2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
	data.x_2  = fabs(pup3/ebmup2);
	data.id_2 = idup;
	
 
	//get w info
        if (nup >= 7) {
          getline(ifs,line);
          stringstream ssw(line);
          ssw >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
          TLorentzVector wboson;
          wboson.SetPxPyPzE(pup1,pup2,pup3,pup4);
          data.wmass = wboson.M();
          data.wpt   = wboson.Pt();
          data.wy    = wboson.Rapidity();
          data.wphi  = wboson.Phi();
        }

        if (nup == 8) {
          getline(ifs,line);
          stringstream ssw(line);
          ssw >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
          //two W boson entryies
        }

	if (verbose ) 
	  std::cout << "w: " <<  idup << " " <<  istup << " " <<  mothup1 << " " <<  mothup2 << " " <<  icolup1 << " " <<  icolup2 
		    << " " <<  pup1 << " " <<  pup2 << " " <<  pup3 << " " <<  pup4 << " " <<  pup5 << " \n";
	
	//get z lepton1 info
        getline(ifs,line);
        stringstream sszlepton1(line);
	sszlepton1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector zlepton1;
        zlepton1.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.pt_zlepton1   = zlepton1.Pt();
        data.eta_zlepton1  = zlepton1.Eta();
        data.phi_zlepton1  = zlepton1.Phi();
        data.pdgid_zlepton1= idup;

	if (verbose ) 
	  std::cout << "zlepton1: " <<  idup << " " <<  istup << " " <<  mothup1 << " " <<  mothup2 << " " <<  icolup1 << " " <<  icolup2 
		    << " " <<  pup1 << " " <<  pup2 << " " <<  pup3 << " " <<  pup4 << " " <<  pup5 << " \n";

	//get z lepton2 info
        getline(ifs,line);
        stringstream sszlepton2(line);
	sszlepton2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector zlepton2;
        zlepton2.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.pt_zlepton2   = zlepton2.Pt();
        data.eta_zlepton2  = zlepton2.Eta();
        data.phi_zlepton2  = zlepton2.Phi();
        data.pdgid_zlepton2= idup;

	if (verbose ) 
	  std::cout << "zlepton2: " <<  idup << " " <<  istup << " " <<  mothup1 << " " <<  mothup2 << " " <<  icolup1 << " " <<  icolup2 
		    << " " <<  pup1 << " " <<  pup2 << " " <<  pup3 << " " <<  pup4 << " " <<  pup5 << " \n";

        TLorentzVector zboson = zlepton1 + zlepton2;
        data.zmass = zboson.M();
        data.zpt   = zboson.Pt();
        data.zy    = zboson.Rapidity();
        data.zphi  = zboson.Phi();


	//get w lepton info
        getline(ifs,line);
        stringstream sswlepton(line);
	sswlepton >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector wlepton;
        wlepton.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.pt_wlepton   = wlepton.Pt();
        data.eta_wlepton  = wlepton.Eta();
        data.phi_wlepton  = wlepton.Phi();
        data.pdgid_wlepton = idup;

	if (verbose ) 
	  std::cout << "wlepton: " <<  idup << " " <<  istup << " " <<  mothup1 << " " <<  mothup2 << " " <<  icolup1 << " " <<  icolup2 
		    << " " <<  pup1 << " " <<  pup2 << " " <<  pup3 << " " <<  pup4 << " " <<  pup5 << " \n";


	//get w neutrino info
        getline(ifs,line);
        stringstream sswneutrino(line);
	sswneutrino >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector wneutrino;
        wneutrino.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.pt_wneutrino   = wneutrino.Pt();
        data.eta_wneutrino  = wneutrino.Eta();
        data.phi_wneutrino  = wneutrino.Phi();
	
	if (verbose ) 
	  std::cout << "wneutrino: " <<  idup << " " <<  istup << " " <<  mothup1 << " " <<  mothup2 << " " <<  icolup1 << " " <<  icolup2 
		    << " " <<  pup1 << " " <<  pup2 << " " <<  pup3 << " " <<  pup4 << " " <<  pup5 << " \n";
	
	// dilepton info
        TLorentzVector lep1 = wlepton;
        TLorentzVector lep2 = zlepton1;
        if (zlepton1.Pt() > lep1.Pt()) {
          lep2 = lep1;
          lep1 = zlepton1;
        }
        if (zlepton2.Pt() > lep1.Pt()) {
          lep2 = lep1;
          lep1 = zlepton2;
        } else if (zlepton2.Pt() > lep2.Pt()) {
          lep2 = zlepton2;
        }
        data.pt_1   = lep1.Pt();
        data.eta_1  = lep1.Eta();
        data.phi_1  = lep1.Phi();
        data.pt_2   = lep2.Pt();
        data.eta_2  = lep2.Eta();
        data.phi_2  = lep2.Phi();

	TLorentzVector dilep = lep1 + lep2;
	data.mass = dilep.M();
	data.pt   = dilep.Pt();
	data.y    = dilep.Rapidity();
	data.phi  = dilep.Phi();
	
	double et1 = sqrt(lep1.Perp2() + lep1.M2());
        double et2 = sqrt(lep2.Perp2() + lep2.M2());
        data.mt = sqrt( (et1+et2)*(et1+et2) - dilep.Pt()*dilep.Pt() );
	

        TLorentzVector fourLeptonSystem = zlepton1 + zlepton2 + wlepton + wneutrino;
        data.fourLeptonMass = fourLeptonSystem.M();
        TLorentzVector wlnuSystem = wlepton + wneutrino;
        TLorentzVector zllSystem = zlepton1 + zlepton2;
        data.wlnumass = wlnuSystem.M();
        data.zllmass = zllSystem.M();

	tree->Fill();
	count++;
      }  
    }
    ifs.close();
  }
    
  tree->Print();
  outfile->Write();  
  outfile->Close();
    
  gBenchmark->Show("lheGENtupler");
}
