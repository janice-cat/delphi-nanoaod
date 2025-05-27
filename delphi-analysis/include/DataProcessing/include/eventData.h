#ifndef EVENTDATA_H
#define EVENTDATA_H

//cpp dependencies
#include <string>
#include <vector>

//ROOT dependencies
#include "TTree.h"

inline bool do_chThrust           = true;
inline bool do_neuThrust          = true;
inline bool do_thrustCorr         = false;
inline bool do_thrustCorrInverse  = false;
inline bool do_thrustMissP        = true;

class eventData{
 public:
  Bool_t passesNTupleAfterCut;
  Bool_t passesTotalChgEnergyMin;
  Bool_t passesNTrkMin;
  Bool_t passesNeuNch;
  Bool_t passesSTheta;
  Bool_t passesMissP;
  Bool_t passesLEP1TwoPC;

  Bool_t passesISR;
  Bool_t passesWW;

  Bool_t passesAll;
  
  Bool_t passesBELLE;

  Float_t missP;
  Float_t missPt;
  Float_t missTheta;
  Float_t missPhi;
  Float_t missChargedP;
  Float_t missChargedPt;
  Float_t missChargedTheta;
  Float_t missChargedPhi;
  Int_t nChargedHadrons;
  Int_t nChargedHadronsHP;
  Float_t nChargedHadronsHP_Corrected;
  Int_t nChargedHadrons_GT0p4;
  Int_t nChargedHadrons_GT0p4Thrust;
  Int_t nChargedParticle;
  Int_t nChargedParticleHP;
  Int_t nChargedParticleHPSmear;
  Int_t nChargedParticleHPUnfold;
  Int_t nParticleHP;

  //thrust axis variables
  Float_t Thrust;
  Float_t TTheta;
  Float_t TPhi;
  Float_t Thrust_charged;
  Float_t TTheta_charged;
  Float_t TPhi_charged;
  Float_t Thrust_neutral;
  Float_t TTheta_neutral;
  Float_t TPhi_neutral;
  Float_t ThrustCorr;
  Float_t TThetaCorr;
  Float_t TPhiCorr;
  Float_t ThrustCorrInverse;
  Float_t TThetaCorrInverse;
  Float_t TPhiCorrInverse;
  Float_t ThrustWithMissP;
  Float_t TThetaWithMissP;
  Float_t TPhiWithMissP;

  //Sphericity variables
  Float_t Sphericity;
  Float_t STheta;
  Float_t SPhi;
  Float_t Aplanarity;
  Float_t Sphericity_linearized;
  Float_t STheta_linearized;
  Float_t SPhi_linearized;
  Float_t Aplanarity_linearized;
  Float_t C_linearized;
  Float_t D_linearized;

  Float_t Mvis;
  Float_t sPrime;
  Float_t d2;
  Float_t cW;

  // user study:
  //    not involved in the output procedure,
  //    but just provide a placeholder here
  Float_t chgdE;
  Float_t spherTheta;
  Float_t ISRE;
  Float_t gE;
  Float_t qbarE;

  static const int nVar = 61;
  std::string varStr[nVar] = {"passesNTupleAfterCut",
			      "passesTotalChgEnergyMin",
			      "passesNTrkMin",
			      "passesSTheta",
			      "passesMissP",
			      "passesISR",
			      "passesWW",
			      "passesNeuNch",
			      "passesAll",
			      "missP",
			      "missPt",
			      "missTheta",
			      "missPhi",
			      "missChargedP",
			      "missChargedPt",
			      "missChargedTheta",
			      "missChargedPhi",
			      "nChargedHadrons",
			      "nChargedHadronsHP",
			      "nChargedHadronsHP_Corrected",
			      "nChargedHadrons_GT0p4",
			      "nChargedHadrons_GT0p4Thrust",
			      "nChargedParticle",
			      "nChargedParticleHP",
			      "nChargedParticleHPSmear",
			      "nChargedParticleHPUnfold",
			      "nParticleHP",
			      "Thrust",
			      "TTheta",
			      "TPhi",
			      "Thrust_charged",
			      "TTheta_charged",
			      "TPhi_charged",
			      "Thrust_neutral",
			      "TTheta_neutral",
			      "TPhi_neutral",
			      "ThrustCorr",
			      "TThetaCorr",
			      "TPhiCorr",
			      "ThrustCorrInverse",
			      "TThetaCorrInverse",
			      "TPhiCorrInverse",
			      "ThrustWithMissP",
			      "TThetaWithMissP",
			      "TPhiWithMissP",
                              "Sphericity",
                              "STheta",
                              "SPhi",
                              "Aplanarity",
                              "Sphericity_linearized",
                              "STheta_linearized",
                              "SPhi_linearized",
                              "Aplanarity_linearized",
                              "C_linearized",
                              "D_linearized",
			      "passesLEP1TwoPC",
			      "passesBELLE",
			      "Mvis",
			      "sPrime",
			      "d2",
			      "cW"
                             };

  bool varIsGood[nVar];

  eventData();
  void SetStatusAndAddressRead(TTree* inTree_p, std::vector<std::string> inList);
  void SetBranchWrite(TTree* inTree_p, bool doMinimal=0);

  // quick eventData placeholder template 
  // If you want to do so, please change the definition of varStr first, and the do _init() -> _setRead() -> _setWrite(),
  // you can get the correspond codes instantaneously.
  // Please use with care. Here assuming the branchname:
  //      1. with "passes" to be boolean type, 
  //      2. with "Particle" to be integer type, 
  //      3. else are float type.
  // One should check it if you have different naming rules.
  void _init();
  void _setRead();
  void _setWrite();
};

inline eventData::eventData()
{
  passesNTupleAfterCut = false;
  passesTotalChgEnergyMin = false;
  passesNTrkMin = false;
  passesSTheta = false;
  passesMissP = false;
  passesISR = false;
  passesWW = false;
  passesNeuNch = false;
  passesAll = false;
  missP = -999;
  missPt = -999;
  missTheta = -999;
  missPhi = -999;
  missChargedP = -999;
  missChargedPt = -999;
  missChargedTheta = -999;
  missChargedPhi = -999;
  nChargedHadrons = -999;
  nChargedHadronsHP = -999;
  nChargedHadronsHP_Corrected = -999;
  nChargedHadrons_GT0p4 = -999;
  nChargedHadrons_GT0p4Thrust = -999;
  nChargedParticle = -999;
  nChargedParticleHP = -999;
  nChargedParticleHPSmear = -999;
  nChargedParticleHPUnfold = -999;
  nParticleHP = -999;
  Thrust = -999;
  TTheta = -999;
  TPhi = -999;
  Thrust_charged = -999;
  TTheta_charged = -999;
  TPhi_charged = -999;
  Thrust_neutral = -999;
  TTheta_neutral = -999;
  TPhi_neutral = -999;
  ThrustCorr = -999;
  TThetaCorr = -999;
  TPhiCorr = -999;
  ThrustCorrInverse = -999;
  TThetaCorrInverse = -999;
  TPhiCorrInverse = -999;
  ThrustWithMissP = -999;
  TThetaWithMissP = -999;
  TPhiWithMissP = -999;
  Sphericity = -999;
  STheta = -999;
  SPhi = -999;
  Aplanarity = -999;
  Sphericity_linearized = -999;
  STheta_linearized = -999;
  SPhi_linearized = -999;
  Aplanarity_linearized = -999;
  C_linearized = -999;
  D_linearized = -999;
  passesLEP1TwoPC = false;
  passesBELLE = false;

  Mvis = -999;
  sPrime = -999;
  d2 = -999;
  cW = -999;

  chgdE      = -999;
  spherTheta = -999;
  ISRE       = -999;
  gE         = -999;
  qbarE      = -999;

  for(int i = 0; i < nVar; ++i){varIsGood[i] = true;}
  return;
}

inline void eventData::SetStatusAndAddressRead(TTree* inTree_p, std::vector<std::string> inList)
{
  if(inList.size() != 0){
    for(int i = 0; i < nVar; ++i){varIsGood[i] = false;}

    for(unsigned int i = 0; i < inList.size(); ++i){

      for(Int_t j = 0; j < nVar; ++j){
        if(inList.at(i).size() == varStr[j].size() && inList.at(i).find(varStr[j]) != std::string::npos){
          varIsGood[j] = true;
          break;
        }
      }
    }
  }

  for(Int_t i = 0; i < nVar; ++i){
    if(varIsGood[i]) inTree_p->SetBranchStatus(varStr[i].c_str(), 1);
  }

  if(varIsGood[0]) inTree_p->SetBranchAddress("passesNTupleAfterCut", &passesNTupleAfterCut);
  if(varIsGood[1]) inTree_p->SetBranchAddress("passesTotalChgEnergyMin", &passesTotalChgEnergyMin);
  if(varIsGood[2]) inTree_p->SetBranchAddress("passesNTrkMin", &passesNTrkMin);
  if(varIsGood[3]) inTree_p->SetBranchAddress("passesSTheta", &passesSTheta);
  if(varIsGood[4]) inTree_p->SetBranchAddress("passesMissP", &passesMissP);
  if(varIsGood[5]) inTree_p->SetBranchAddress("passesISR", &passesISR);
  if(varIsGood[6]) inTree_p->SetBranchAddress("passesWW", &passesWW);
  if(varIsGood[7]) inTree_p->SetBranchAddress("passesNeuNch", &passesNeuNch);
  if(varIsGood[8]) inTree_p->SetBranchAddress("passesAll", &passesAll);
  if(varIsGood[9]) inTree_p->SetBranchAddress("missP", &missP);
  if(varIsGood[10]) inTree_p->SetBranchAddress("missPt", &missPt);
  if(varIsGood[11]) inTree_p->SetBranchAddress("missTheta", &missTheta);
  if(varIsGood[12]) inTree_p->SetBranchAddress("missPhi", &missPhi);
  if(varIsGood[13]) inTree_p->SetBranchAddress("missChargedP", &missChargedP);
  if(varIsGood[14]) inTree_p->SetBranchAddress("missChargedPt", &missChargedPt);
  if(varIsGood[15]) inTree_p->SetBranchAddress("missChargedTheta", &missChargedTheta);
  if(varIsGood[16]) inTree_p->SetBranchAddress("missChargedPhi", &missChargedPhi);
  if(varIsGood[17]) inTree_p->SetBranchAddress("nChargedHadrons", &nChargedHadrons);
  if(varIsGood[18]) inTree_p->SetBranchAddress("nChargedHadronsHP", &nChargedHadronsHP);
  if(varIsGood[19]) inTree_p->SetBranchAddress("nChargedHadronsHP_Corrected", &nChargedHadronsHP_Corrected);
  if(varIsGood[20]) inTree_p->SetBranchAddress("nChargedHadrons_GT0p4", &nChargedHadrons_GT0p4);
  if(varIsGood[21]) inTree_p->SetBranchAddress("nChargedHadrons_GT0p4Thrust", &nChargedHadrons_GT0p4Thrust);
  if(varIsGood[22]) inTree_p->SetBranchAddress("nChargedParticle", &nChargedParticle);
  if(varIsGood[23]) inTree_p->SetBranchAddress("nChargedParticleHP", &nChargedParticleHP);
  if(varIsGood[24]) inTree_p->SetBranchAddress("nChargedParticleHPSmear", &nChargedParticleHPSmear);
  if(varIsGood[25]) inTree_p->SetBranchAddress("nChargedParticleHPUnfold", &nChargedParticleHPUnfold);
  if(varIsGood[26]) inTree_p->SetBranchAddress("nParticleHP", &nParticleHP);

  if(varIsGood[27]) inTree_p->SetBranchAddress("Thrust", &Thrust);
  if(varIsGood[28]) inTree_p->SetBranchAddress("TTheta", &TTheta);
  if(varIsGood[29]) inTree_p->SetBranchAddress("TPhi", &TPhi);
  if(varIsGood[30]) inTree_p->SetBranchAddress("Thrust_charged", &Thrust_charged);
  if(varIsGood[31]) inTree_p->SetBranchAddress("TTheta_charged", &TTheta_charged);
  if(varIsGood[32]) inTree_p->SetBranchAddress("TPhi_charged", &TPhi_charged);

  if(varIsGood[33]) inTree_p->SetBranchAddress("Thrust_neutral", &Thrust_neutral);
  if(varIsGood[34]) inTree_p->SetBranchAddress("TTheta_neutral", &TTheta_neutral);
  if(varIsGood[35]) inTree_p->SetBranchAddress("TPhi_neutral", &TPhi_neutral);
  if(varIsGood[36]) inTree_p->SetBranchAddress("ThrustCorr", &ThrustCorr);
  if(varIsGood[37]) inTree_p->SetBranchAddress("TThetaCorr", &TThetaCorr);
  if(varIsGood[38]) inTree_p->SetBranchAddress("TPhiCorr", &TPhiCorr);
  if(varIsGood[39]) inTree_p->SetBranchAddress("ThrustCorrInverse", &ThrustCorrInverse);
  if(varIsGood[40]) inTree_p->SetBranchAddress("TThetaCorrInverse", &TThetaCorrInverse);
  if(varIsGood[41]) inTree_p->SetBranchAddress("TPhiCorrInverse", &TPhiCorrInverse);
  if(varIsGood[42]) inTree_p->SetBranchAddress("ThrustWithMissP", &ThrustWithMissP);
  if(varIsGood[43]) inTree_p->SetBranchAddress("TThetaWithMissP", &TThetaWithMissP);
  if(varIsGood[44]) inTree_p->SetBranchAddress("TPhiWithMissP", &TPhiWithMissP);


  if(varIsGood[45]) inTree_p->SetBranchAddress("Sphericity", &Sphericity);
  if(varIsGood[46]) inTree_p->SetBranchAddress("STheta", &STheta);
  if(varIsGood[47]) inTree_p->SetBranchAddress("SPhi", &SPhi);
  if(varIsGood[48]) inTree_p->SetBranchAddress("Aplanarity", &Aplanarity);
  if(varIsGood[49]) inTree_p->SetBranchAddress("Sphericity_linearized", &Sphericity_linearized);
  if(varIsGood[50]) inTree_p->SetBranchAddress("STheta_linearized", &STheta_linearized);
  if(varIsGood[51]) inTree_p->SetBranchAddress("SPhi_linearized", &SPhi_linearized);
  if(varIsGood[52]) inTree_p->SetBranchAddress("Aplanarity_linearized", &Aplanarity_linearized);
  if(varIsGood[53]) inTree_p->SetBranchAddress("C_linearized", &C_linearized);
  if(varIsGood[54]) inTree_p->SetBranchAddress("D_linearized", &D_linearized);
  if(varIsGood[55]) inTree_p->SetBranchAddress("passesLEP1TwoPC", &passesLEP1TwoPC);
  if(varIsGood[56]) inTree_p->SetBranchAddress("passesBELLE", &passesBELLE);

  if(varIsGood[57]) inTree_p->SetBranchAddress("Mvis", &Mvis);
  if(varIsGood[58]) inTree_p->SetBranchAddress("sPrime", &sPrime);
  if(varIsGood[59]) inTree_p->SetBranchAddress("d2", &d2);
  if(varIsGood[60]) inTree_p->SetBranchAddress("cW", &cW);

  inTree_p->SetBranchAddress("chgdE", &chgdE);
  inTree_p->SetBranchAddress("spherTheta", &spherTheta);
  inTree_p->SetBranchAddress("ISRE", &ISRE);
  inTree_p->SetBranchAddress("gE", &gE);
  inTree_p->SetBranchAddress("qbarE", &qbarE);


  return;
}

inline void eventData::SetBranchWrite(TTree* inTree_p, bool doMinimal)
{
  if (!doMinimal){
    inTree_p->Branch("passesNTupleAfterCut", &passesNTupleAfterCut, "passesNTupleAfterCut/O");
    inTree_p->Branch("passesTotalChgEnergyMin", &passesTotalChgEnergyMin, "passesTotalChgEnergyMin/O");
    inTree_p->Branch("passesNTrkMin", &passesNTrkMin, "passesNTrkMin/O");
    inTree_p->Branch("passesSTheta", &passesSTheta, "passesSTheta/O");
    //inTree_p->Branch("passesMissP", &passesMissP, "passesMissP/O");
    inTree_p->Branch("passesNeuNch", &passesNeuNch, "passesNeuNch/O");
    //inTree_p->Branch("passesAll", &passesAll, "passesAll/O");
    inTree_p->Branch("missTheta", &missTheta, "missTheta/F");
    inTree_p->Branch("missPhi", &missPhi, "missPhi/F");
    inTree_p->Branch("missChargedP", &missChargedP, "missChargedP/F");
    inTree_p->Branch("missChargedPt", &missChargedPt, "missChargedPt/F");
    inTree_p->Branch("missChargedTheta", &missChargedTheta, "missChargedTheta/F");
    inTree_p->Branch("missChargedPhi", &missChargedPhi, "missChargedPhi/F");
    inTree_p->Branch("nChargedHadrons", &nChargedHadrons, "nChargedHadrons/I");
    inTree_p->Branch("nChargedHadronsHP", &nChargedHadronsHP, "nChargedHadronsHP/I");
    //inTree_p->Branch("nChargedHadronsHP_Corrected", &nChargedHadronsHP_Corrected, "nChargedHadronsHP_Corrected/F");
    //inTree_p->Branch("nChargedHadrons_GT0p4", &nChargedHadrons_GT0p4, "nChargedHadrons_GT0p4/I");
    //inTree_p->Branch("nChargedHadrons_GT0p4Thrust", &nChargedHadrons_GT0p4Thrust, "nChargedHadrons_GT0p4Thrust/I");
  }
  inTree_p->Branch("nChargedParticle", &nChargedParticle, "nChargedParticle/I");
  inTree_p->Branch("nChargedParticleHP", &nChargedParticleHP, "nChargedParticleHP/I");
  inTree_p->Branch("nChargedParticleHPSmear", &nChargedParticleHPSmear, "nChargedParticleHPSmear/I");
  inTree_p->Branch("nChargedParticleHPUnfold", &nChargedParticleHPUnfold, "nChargedParticleHPUnfold/I");
  inTree_p->Branch("nParticleHP", &nParticleHP, "nParticleHP/I");
  inTree_p->Branch("Thrust", &Thrust, "Thrust/F");
  inTree_p->Branch("TTheta", &TTheta, "TTheta/F");
  inTree_p->Branch("TPhi", &TPhi, "TPhi/F");
  if (do_chThrust)          inTree_p->Branch("Thrust_charged", &Thrust_charged, "Thrust_charged/F");
  if (do_chThrust)          inTree_p->Branch("TTheta_charged", &TTheta_charged, "TTheta_charged/F");
  if (do_chThrust)          inTree_p->Branch("TPhi_charged", &TPhi_charged, "TPhi_charged/F");
  if (do_neuThrust)         inTree_p->Branch("Thrust_neutral", &Thrust_neutral, "Thrust_neutral/F");
  if (do_neuThrust)         inTree_p->Branch("TTheta_neutral", &TTheta_neutral, "TTheta_neutral/F");
  if (do_neuThrust)         inTree_p->Branch("TPhi_neutral", &TPhi_neutral, "TPhi_neutral/F");
  if (do_thrustCorr)        inTree_p->Branch("ThrustCorr", &ThrustCorr, "ThrustCorr/F");
  if (do_thrustCorr)        inTree_p->Branch("TThetaCorr", &TThetaCorr, "TThetaCorr/F");
  if (do_thrustCorr)        inTree_p->Branch("TPhiCorr", &TPhiCorr, "TPhiCorr/F");
  if (do_thrustCorrInverse) inTree_p->Branch("ThrustCorrInverse", &ThrustCorrInverse, "ThrustCorrInverse/F");
  if (do_thrustCorrInverse) inTree_p->Branch("TThetaCorrInverse", &TThetaCorrInverse, "TThetaCorrInverse/F");
  if (do_thrustCorrInverse) inTree_p->Branch("TPhiCorrInverse", &TPhiCorrInverse, "TPhiCorrInverse/F");
  if (do_thrustMissP)       inTree_p->Branch("ThrustWithMissP", &ThrustWithMissP, "ThrustWithMissP/F");
  if (do_thrustMissP)       inTree_p->Branch("TThetaWithMissP", &TThetaWithMissP, "TThetaWithMissP/F");
  if (do_thrustMissP)       inTree_p->Branch("TPhiWithMissP", &TPhiWithMissP, "TPhiWithMissP/F");
  if (!doMinimal){
    inTree_p->Branch("STheta", &STheta,"STheta/F");
    inTree_p->Branch("SPhi", &SPhi,"SPhi/F");
    inTree_p->Branch("Sphericity_linearized", &Sphericity_linearized,"Sphericity_linearized/F");
    inTree_p->Branch("STheta_linearized", &STheta_linearized,"STheta_linearized/F");
    inTree_p->Branch("SPhi_linearized", &SPhi_linearized,"SPhi_linearized/F");
    inTree_p->Branch("Aplanarity_linearized", &Aplanarity_linearized,"Aplanarity_linearized/F");
    inTree_p->Branch("C_linearized", &C_linearized,"C_linearized/F");
    inTree_p->Branch("D_linearized", &D_linearized,"D_linearized/F");
    //inTree_p->Branch("passesLEP1TwoPC", &passesLEP1TwoPC, "passesLEP1TwoPC/O");
  }
  inTree_p->Branch("Sphericity", &Sphericity,"Sphericity/F");
  inTree_p->Branch("Aplanarity", &Aplanarity,"Aplanarity/F");
  inTree_p->Branch("passesBELLE", &passesBELLE, "passesBELLE/O");
  inTree_p->Branch("passesISR", &passesISR, "passesISR/O");
  inTree_p->Branch("passesWW", &passesWW, "passesWW/O");
  inTree_p->Branch("missPt", &missPt, "missPt/F");
  inTree_p->Branch("missP", &missP, "missP/F");

  inTree_p->Branch("Mvis", &Mvis, "Mvis/F");
  inTree_p->Branch("sPrime", &sPrime, "sPrime/F");
  inTree_p->Branch("d2", &d2, "d2/F");
  inTree_p->Branch("cW", &cW, "cW/F");
  
  return;
}

inline void eventData::_init()
{
  printf("// Variable Declaration\n");
  for (int i = 0; i < nVar; ++i)
  {
    if(varStr[i].find("passes") != std::string::npos)         printf("\tBool_t  %s;\n", varStr[i].c_str());
    else if(varStr[i].find("Particle") != std::string::npos)  printf("\tInt_t   %s;\n", varStr[i].c_str());
    else                                                      printf("\tFloat_t %s;\n", varStr[i].c_str());
  }
  printf("// Variable Initialization\n");
  for (int i = 0; i < nVar; ++i)
  {
    if(varStr[i].find("passes") != std::string::npos) printf("\t%s = false;\n", varStr[i].c_str());
    else                                              printf("\t%s = -999;\n" , varStr[i].c_str());
  }
}

inline void eventData::_setRead()
{
  for (int i = 0; i < nVar; ++i)
  {
    printf("\tif(varIsGood[%d]) inTree_p->SetBranchAddress(\"%s\", &%s);\n", i, varStr[i].c_str(), varStr[i].c_str());
  }
}

inline void eventData::_setWrite()
{
  for (int i = 0; i < nVar; ++i)
  {
    if(varStr[i].find("passes") != std::string::npos)         printf("\tinTree_p->Branch(\"%s\", &%s, \"%s/O\");\n", varStr[i].c_str(), varStr[i].c_str(), varStr[i].c_str());
    else if(varStr[i].find("Particle") != std::string::npos)  printf("\tinTree_p->Branch(\"%s\", &%s, \"%s/I\");\n", varStr[i].c_str(), varStr[i].c_str(), varStr[i].c_str());
    else                                                      printf("\tinTree_p->Branch(\"%s\", &%s, \"%s/F\");\n", varStr[i].c_str(), varStr[i].c_str(), varStr[i].c_str());
  }
}


#endif
