// Generator level info data object
struct TGenData
{
  UInt_t  npho;                   // number of photons
  Float_t scalePdf;               // pdf scale sqrt(Q^2)  
  Float_t weight;                 // event weight
  Float_t vmass, vpt, vy, vphi;   // boson kinematics
  Float_t mass, pt, y, phi, mt;   // dilepton kinematics
  Float_t pt_1, eta_1, phi_1;     // lepton kinematics
  Float_t pt_2, eta_2, phi_2;     // lepton kinematics
  Int_t   q_1, q_2;               // lepton charge 
  Float_t phopt, phoeta, phophi;  // leading photon kinematics
  Float_t x_1, x_2;               // parton momentum fraction
  Int_t   id_1, id_2;             // parton PDG ID
  
  Float_t aqcd, aqed;             // QCD, QED coupling constants
  Float_t jetpt, jeteta, jetphi;  // "jet" kinematics
  Int_t   jetid;                  // "jet" PDG ID
};

struct TGenWGammastarData
{
  UInt_t  npho;                   // number of photons
  Float_t scalePdf;               // pdf scale sqrt(Q^2)  
  Float_t weight;                 // event weight
  Float_t wmass, wpt, wy, wphi;   // boson kinematics
  Float_t zmass, zpt, zy, zphi;   // boson kinematics
  Float_t mass, pt, y, phi, mt;   // dilepton kinematics
  Float_t pt_1, eta_1, phi_1;     // lepton kinematics
  Float_t pt_2, eta_2, phi_2;     // lepton kinematics
  Int_t   q_1, q_2;               // lepton charge 

  Float_t pt_zlepton1, eta_zlepton1, phi_zlepton1 ;    // lepton kinematics
  Int_t   pdgid_zlepton1;
  Float_t pt_zlepton2, eta_zlepton2, phi_zlepton2;     // lepton kinematics
  Int_t   pdgid_zlepton2;
  Float_t pt_wlepton, eta_wlepton, phi_wlepton;     // lepton kinematics
  Int_t   pdgid_wlepton;
  Float_t pt_wneutrino, eta_wneutrino, phi_wneutrino;     // lepton kinematics
  Float_t fourLeptonMass;
  Float_t wlnumass;
  Float_t zllmass;

  Float_t x_1, x_2;               // parton momentum fraction
  Int_t   id_1, id_2;             // parton PDG ID
  
  Float_t aqcd, aqed;             // QCD, QED coupling constants
};


