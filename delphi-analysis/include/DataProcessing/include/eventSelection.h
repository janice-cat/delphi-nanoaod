#ifndef EVENTSELECTION_H
#define EVENTSELECTION_H

#include <vector>
#include <map>
#include <boost/range/adaptor/reversed.hpp>

#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/JadePlugin.hh"

#include "DataProcessing/include/trackSelection.h"
#include "DataProcessing/include/neutralHadronSelection.h"

#include "DataProcessing/include/particleData.h"
#include "DataProcessing/include/eventData.h"
#include "DataProcessing/include/sphericityTools.h"

using namespace std;

class MyInfo: public fastjet::PseudoJet::UserInfoBase {
  public:
  MyInfo(int pwflag, bool highPurity) : _pwflag(pwflag), _highPurity(highPurity) {};
  int pwflag() const {return _pwflag;};
  bool highPurity() const {return _highPurity;};
  int _pwflag;
  bool _highPurity;
};

class eventSelection{
public:
  std::vector<fastjet::PseudoJet> particles;
  std::vector<fastjet::PseudoJet> particlesFourJetClustering;
  std::map<int, fastjet::PseudoJet> ISRgammaCand;
  vector<fastjet::PseudoJet> ISRgammaJet;
  vector<fastjet::PseudoJet> ISRgammaJetSelected;

  int nJISR = 2;
  fastjet::JetDefinition jDefISR;

  int nJWW = 4;
  fastjet::JetDefinition jDefWW;

  Double_t pxSumInit_;
  Double_t pySumInit_;
  Double_t pzSumInit_;
  Double_t eSumInit_;

  std::vector<TLorentzVector> initFourJet;

  Double_t masslessRescale_;

  Double_t factorA_;
  Double_t factorB_;
  Double_t factorC_;
  Double_t factorD_;

  Double_t pxSumFinal_;
  Double_t pySumFinal_;
  Double_t pzSumFinal_;
  Double_t eSumFinal_;

  std::vector<TLorentzVector> finalFourJet;

  Bool_t passesTotalChgEnergyMin = false;
  Bool_t passesNTrkMin = false;
  Bool_t passesSTheta = false;
  Bool_t passesMissP = false;
  Bool_t passesNeuNch = false;

  Bool_t passesISR = false;
  Bool_t passesISR2 = false;
  Bool_t passesWW = false;
  Bool_t passesD2 = false;
  Bool_t passesCW = false;

  eventSelection(){
    jDefISR = fastjet::JetDefinition(fastjet::ee_kt_algorithm);
    jDefWW = fastjet::JetDefinition(fastjet::ee_kt_algorithm);
  }
  eventSelection(particleData* inPart, eventData* inData){
    jDefISR = fastjet::JetDefinition(fastjet::ee_kt_algorithm);
    jDefWW = fastjet::JetDefinition(fastjet::ee_kt_algorithm);
    setEventSelection(inPart, inData);
  }
  Bool_t checkDoRescale(std::vector<TLorentzVector>* inJets, std::vector<double>* factors, Float_t cmEnergy);
  void setEventSelection(particleData* inPart, eventData* inData);
  void setEventSelection(particleData* inPart, eventData* inData,
    Double_t& out_sPrime, Double_t& out_mvis,
    Double_t& out_d2, Double_t& out_cW,
    Double_t& out_NeuNch, Double_t& out_cosSTheta,
    Double_t& out_TotalChgEnergyMin);
  void removeISRgammaCandidate();
  Bool_t getPassesTotalChgEnergyMin(){return passesTotalChgEnergyMin;}
  Bool_t getPassesNTrkMin(){return passesNTrkMin;}
  Bool_t getPassesNeuNch(){return passesNeuNch;}
  Bool_t getPassesSTheta(){return passesSTheta;}
  Bool_t getPassesMissP(){return passesMissP;}

  Bool_t getPassesISR(){return passesISR;}
  Bool_t getPassesWW(){return passesWW;}

  Float_t getMvis(){return _Mvis;}
  Float_t getsPrime(){return _sPrime;}
  Float_t getd2(){return _d2;}
  Float_t getcW(){return _cW;}

private:
  TrackSelection trkSel = TrackSelection();
  NeutralHadronSelection neutralHadronSel;

  Float_t _d2 = 9999999.;
  Float_t _cW = 9999999.;
  Float_t _Mvis = 9999999.;
  Float_t _sPrime = 9999999.;

};


inline Bool_t eventSelection::checkDoRescale(std::vector<TLorentzVector>* inJets, std::vector<double>* factors, Float_t cmEnergy)
{
  Float_t px0 = inJets->at(0).Px();
  Float_t py0 = inJets->at(0).Py();
  Float_t pz0 = inJets->at(0).Pz();
  Float_t p0 = TMath::Sqrt(px0*px0 + py0*py0 + pz0*pz0);

  Float_t px1 = inJets->at(1).Px();
  Float_t py1 = inJets->at(1).Py();
  Float_t pz1 = inJets->at(1).Pz();
  Float_t p1 = TMath::Sqrt(px1*px1 + py1*py1 + pz1*pz1);
  
  Float_t px2 = inJets->at(2).Px();
  Float_t py2 = inJets->at(2).Py();
  Float_t pz2 = inJets->at(2).Pz();
  Float_t p2 = TMath::Sqrt(px2*px2 + py2*py2 + pz2*pz2);

  Float_t px3 = inJets->at(3).Px();
  Float_t py3 = inJets->at(3).Py();
  Float_t pz3 = inJets->at(3).Pz();
  Float_t p3 = TMath::Sqrt(px3*px3 + py3*py3 + pz3*pz3);

  Float_t p10xy = py1 - py0*px1/px0;
  Float_t p20xy = py2 - py0*px2/px0;
  Float_t p30xy = py3 - py0*px3/px0;

  Float_t ct = pz2 - pz1*p20xy/p10xy - pz0*px2/px0 + pz0*px1*p20xy/(px0*p10xy);
  Float_t dt = pz3 - pz1*p30xy/p10xy - pz0*px3/px0 + pz0*px1*p30xy/(px0*p10xy);

  Float_t d = 1.;
  Float_t c = -d*dt/ct;
  Float_t b = -(c*p20xy + d*p30xy)/p10xy;
  Float_t a = -1.*(b*px1 + c*px2 + d*px3)/px0;

  d = 1.;
  c = 1.;
  b = 1.;
  a = 1.;

  p0 = a*TMath::Sqrt(px0*px0 + py0*py0 + pz0*pz0);
  p1 = b*TMath::Sqrt(px1*px1 + py1*py1 + pz1*pz1);
  p2 = c*TMath::Sqrt(px2*px2 + py2*py2 + pz2*pz2);
  p3 = d*TMath::Sqrt(px3*px3 + py3*py3 + pz3*pz3);

  Float_t cmFactor = cmEnergy/(p0 + p1 + p2 + p3);
  p0 *= cmFactor;
  p1 *= cmFactor;
  p2 *= cmFactor;
  p3 *= cmFactor;
  
  a *= cmFactor;
  b *= cmFactor;
  c *= cmFactor;
  d *= cmFactor;

  inJets->at(0).SetPxPyPzE(a*px0, a*py0, a*pz0, p0);
  inJets->at(1).SetPxPyPzE(b*px1, b*py1, b*pz1, p1);
  inJets->at(2).SetPxPyPzE(c*px2, c*py2, c*pz2, p2);
  inJets->at(3).SetPxPyPzE(d*px3, d*py3, d*pz3, p3);

  factors->push_back(a);
  factors->push_back(b);
  factors->push_back(c);
  factors->push_back(d);
  //  std::cout << " a,b,c,d: " << a << ", " << b << ", " << c << ", " << d << std::endl;

  return false;
}

inline void eventSelection::setEventSelection(particleData* inPart, eventData* inData)
{
  particles.clear();

  passesTotalChgEnergyMin = false;
  passesNTrkMin = false;
  passesSTheta = false;
  passesMissP = false;

  passesISR = false;
  passesISR2 = false;
  passesWW = false;
  passesD2 = false;
  passesCW = false;

  initFourJet.clear();
  finalFourJet.clear();

  initFourJet.push_back({0,0,0,0});
  initFourJet.push_back({0,0,0,0});
  initFourJet.push_back({0,0,0,0});
  initFourJet.push_back({0,0,0,0});

  finalFourJet.push_back({0,0,0,0});
  finalFourJet.push_back({0,0,0,0});
  finalFourJet.push_back({0,0,0,0});
  finalFourJet.push_back({0,0,0,0});

  if(inPart->nParticle < 4) return;

  Float_t TotalChgEnergy = 0;
  Int_t NTrk = 0;
  Int_t Neu = 0;
  Float_t STheta = inData->STheta;
  Float_t MissP = inData->missP;

  for(Int_t pI = 0; pI < inPart->nParticle; ++pI){
    Double_t e = TMath::Sqrt(inPart->pmag[pI]*inPart->pmag[pI] + inPart->mass[pI]*inPart->mass[pI]);
    particles.push_back(fastjet::PseudoJet(inPart->px[pI], inPart->py[pI], inPart->pz[pI], e));

    if (neutralHadronSel.highPurity(inPart,pI)) {
       Neu++;
    }

    if(!trkSel.highPurity(inPart, pI)) continue;
    TotalChgEnergy += TMath::Sqrt(inPart->pmag[pI]*inPart->pmag[pI] + inPart->mass[pI]*inPart->mass[pI]);
    NTrk += 1;
  }

  passesTotalChgEnergyMin = TotalChgEnergy >= 15;
  passesNTrkMin = NTrk >= 5;
  passesSTheta = TMath::Abs(TMath::Cos(STheta)) <= .82;
  passesMissP = MissP < 20;
  passesNeuNch = (Neu+NTrk)>=13;

  fastjet::ClusterSequence csISR(particles, jDefISR);
  std::vector<fastjet::PseudoJet> twoJetsFJ = csISR.exclusive_jets(nJISR);
  std::vector<TLorentzVector> twoJets;

  //two jet implementation follows https://arxiv.org/pdf/hep-ex/9810047.pdf, 1,2,3 eq.
  for(unsigned int i = 0; i < twoJetsFJ.size(); ++i){
    TLorentzVector temp(twoJetsFJ.at(i).px(), twoJetsFJ.at(i).py(), twoJetsFJ.at(i).pz(), twoJetsFJ.at(i).E());
    twoJets.push_back(temp);
  }
  Double_t absBeta = TMath::Abs(TMath::Sin(twoJets.at(0).Theta() + twoJets.at(1).Theta()));
  absBeta /= (TMath::Sin(twoJets.at(0).Theta()) + TMath::Sin(twoJets.at(1).Theta()));
  Double_t x = 2*absBeta/(1+absBeta);
  Double_t sPrime = inPart->Energy*inPart->Energy*(1-x);
  Double_t mass = (twoJets.at(0) + twoJets.at(1)).M();

  //cuts from http://cds.cern.ch/record/690637/files/ep-2003-084.pdf, page2, middle paragraph (before d^2 equation
  passesISR = mass/inPart->Energy > .7 || sPrime/(inPart->Energy*inPart->Energy) > .81;

  _Mvis = static_cast<float>(mass);
  _sPrime = static_cast<float>(sPrime);

  // if(!passesISR) return;

  fastjet::ClusterSequence csWW(particles, jDefWW);
  std::vector<fastjet::PseudoJet> fourJetsFJ = csWW.exclusive_jets(nJWW);
  std::vector<TLorentzVector> fourJets;

  for(unsigned int i = 0; i < fourJetsFJ.size(); ++i){
    TLorentzVector temp(fourJetsFJ.at(i).px(), fourJetsFJ.at(i).py(), fourJetsFJ.at(i).pz(), fourJetsFJ.at(i).E());
    fourJets.push_back(temp);
    initFourJet.at(i) = temp;
  }

  for(unsigned int i = 0; i < fourJets.size(); ++i){
    for(unsigned int j = i+1; j < fourJets.size(); ++j){
      if(fourJets.at(i).E() > fourJets.at(j).E()){
	TLorentzVector temp = fourJets.at(j);
	fourJets.at(j) = fourJets.at(i);
	fourJets.at(i) = temp;

	initFourJet.at(j) = initFourJet.at(i);
	initFourJet.at(i) = temp;
      }
    }
  }

  pxSumInit_ = 0;
  pySumInit_ = 0;
  pzSumInit_ = 0;
  eSumInit_ = 0;

  pxSumFinal_ = 0;
  pySumFinal_ = 0;
  pzSumFinal_ = 0;
  eSumFinal_ = 0;
  
  for(unsigned int i = 0; i < fourJets.size(); ++i){
    pxSumInit_ += fourJets.at(i).Px();
    pySumInit_ += fourJets.at(i).Py();
    pzSumInit_ += fourJets.at(i).Pz();
    eSumInit_ += fourJets.at(i).E();
  }
  
  bool doRescale = (TMath::Abs(pxSumInit_) > 1. || TMath::Abs(pySumInit_) > 1. || TMath::Abs(pzSumInit_) > 1. || TMath::Abs(eSumInit_ - inPart->Energy) > 1.);
  std::vector<double> factors;

  factorA_ = 1.;
  factorB_ = 1.;
  factorC_ = 1.;
  factorD_ = 1.;
  
  while(doRescale){
    doRescale = checkDoRescale(&fourJets, &factors, inPart->Energy);
    factorA_ = factors.at(0);
    factorB_ = factors.at(1);
    factorC_ = factors.at(2);
    factorD_ = factors.at(3);
  }


  /*
  Float_t pxSumMid_ = 0;
  Float_t pySumMid_ = 0;
  Float_t pzSumMid_ = 0;
  Float_t eSumMid_ = 0;
  for(unsigned int i = 0; i < fourJets.size(); ++i){
    pxSumMid_ += fourJets.at(i).Px();
    pySumMid_ += fourJets.at(i).Py();
    pzSumMid_ += fourJets.at(i).Pz();
    eSumMid_ += fourJets.at(i).E();
  }

  masslessRescale_ = inPart->Energy/eSumMid_;
  //  std::cout << pxSumMid_ << ", " << pySumMid_ << ", " << pzSumMid_ << std::endl;

  for(unsigned int i = 0; i < fourJets.size(); ++i){fourJets.at(i) *= masslessRescale_;}
  */

  for(unsigned int i = 0; i < fourJets.size(); ++i){
    pxSumFinal_ += fourJets.at(i).Px();
    pySumFinal_ += fourJets.at(i).Py();
    pzSumFinal_ += fourJets.at(i).Pz();
    eSumFinal_ += fourJets.at(i).E();
    finalFourJet.at(i) = fourJets.at(i);
  }

  _d2 = 999999999.;
  Double_t smallestAngle = 999999.;
  
  for(unsigned int i = 0; i < fourJets.size()-1; ++i){
    for(unsigned int j = i+1; j < fourJets.size(); ++j){
      Double_t tempAngle = fourJets.at(i).Angle(fourJets.at(j).Vect());

      if(tempAngle < smallestAngle) smallestAngle = tempAngle;
    }
  }
  
  _cW = TMath::Cos(smallestAngle);

  std::vector<double> twoJetMasses;
  for(unsigned int i = 0; i < fourJets.size()-1; ++i){
    for(unsigned int j = i+1; j < fourJets.size(); ++j){
      TLorentzVector temp = fourJets.at(i) + fourJets.at(j);
      twoJetMasses.push_back(temp.M());
    }
  }

  for(unsigned int i = 0; i < twoJetMasses.size()-1; ++i){
    for(unsigned int j = i+1; j < twoJetMasses.size(); ++j){
      Double_t d2Temp = (twoJetMasses.at(i) - 80.4)*(twoJetMasses.at(i) - 80.4);
      d2Temp += (twoJetMasses.at(j) - 80.4)*(twoJetMasses.at(j) - 80.4);
      d2Temp /= (80.4*80.4);
      
      if(d2Temp < _d2) _d2 = d2Temp;
    }
  }

  passesD2 = _d2 >= .1;
  passesCW = _cW >= .9;
  passesWW = passesD2 || passesCW;

  return;
}

// *** Used in setEventSelection ***
inline void eventSelection::removeISRgammaCandidate()
{
  // printf("particles' size: %d\n", (int) particles.size());
  // for (int i = 0; i < (int) particles.size(); ++i)
  // {
  //     printf("%d %d %d (%f %f %f)\n", i, 
  //           particles[i].user_index(), 
  //           particles[i].user_info<MyInfo>().pwflag(),
  //           particles[i].px(),
  //           particles[i].py(),
  //           particles[i].pz() );
  // }
  ISRgammaCand.clear();
  ISRgammaJetSelected.clear();

  fastjet::JadePlugin();
  fastjet::ClusterSequence csISRgammaCandidate(particles, jDefISR);
  std::vector<fastjet::PseudoJet> ISRgammaList = csISRgammaCandidate.exclusive_jets_ycut(0.008);

  for (int i = 0; i < (int) ISRgammaList.size(); ++i)
  {
    ISRgammaJet = ISRgammaList[i].constituents();
    
    // if (verbose) printf("%d multiplicity: %d\n", i, (int) ISRgammaJet.size());

    double eEM  = 0;
    double etot = ISRgammaList[i].e();
    for(int j = 0; j < (int) ISRgammaJet.size(); ++j)
    {
      // if (verbose)
      // {
      //   printf("%d %d %d (%f %f %f) (%f %f)\n", j, 
      //         ISRgammaJet[j].user_index(), 
      //         ISRgammaJet[j].user_info<MyInfo>().pwflag(),
      //         ISRgammaJet[j].px(),
      //         ISRgammaJet[j].py(),
      //         ISRgammaJet[j].pz(),
      //         ISRgammaJet[j].eta(),
      //         ISRgammaJet[j].phi() );
      // }
      int pwflag  = ISRgammaJet[j].user_info<MyInfo>().pwflag();
      if ((pwflag==1) || (pwflag==4)) eEM += ISRgammaJet[j].e();
    }

    // if (verbose) printf("%.3f/%.3f = %.3f\n", eEM, etot, eEM/etot);
    if (eEM/etot > .90 && etot > 10) {
      ISRgammaJetSelected.push_back(ISRgammaList[i]);
      for(int j = 0; j < (int) ISRgammaJet.size(); ++j)
      {
        ISRgammaCand[ ISRgammaJet[j].user_index() ] = ISRgammaJet[j];
      }
    }
  }

  for (int i = 0; i < (int) particles.size(); ++i)
  {
    TVector3 temp(particles[i].px(), particles[i].py(), particles[i].pz());
    if (TMath::Abs( TMath::Cos( temp.Angle(TVector3(0,0,1)) ) ) > 
        TMath::Cos(2/180.*TMath::Pi()) ) // the ptl is close to the beam pipe
    {
      ISRgammaCand[ particles[i].user_index() ] = particles[i];
      ISRgammaJetSelected.push_back(particles[i]);
    }
  }
  // if (verbose)
  // {
  //   for ( const auto &[key, value] : boost::adaptors::reverse(ISRgammaCand) )
  //   {
  //     printf("%d %d (%f %f %f) (%f %f)\n",
  //           key, 
  //           value.user_info<MyInfo>().pwflag(),
  //           value.px(),
  //           value.py(),
  //           value.pz(),
  //           value.eta(),
  //           value.phi() );
  //   }
  // }

  for ( const auto &[key, value] : boost::adaptors::reverse(ISRgammaCand) )
  {
    // printf("%d %d (%f %f %f) (%f %f)\n",
    //       key, 
    //       (particles.begin()+key)->user_info<MyInfo>().pwflag(),
    //       (particles.begin()+key)->px(),
    //       (particles.begin()+key)->py(),
    //       (particles.begin()+key)->pz(),
    //       (particles.begin()+key)->eta(),
    //       (particles.begin()+key)->phi() );
    particles.erase( particles.begin()+key );
  }
}

inline void eventSelection::setEventSelection(particleData* inPart, eventData* inData,
  Double_t& out_sPrime, Double_t& out_mvis,
  Double_t& out_d2, Double_t& out_cW,
  Double_t& out_NeuNch, Double_t& out_cosSTheta,
  Double_t& out_TotalChgEnergyMin)
{
  particles.clear();
  particlesFourJetClustering.clear();

  passesTotalChgEnergyMin = false;
  passesNTrkMin = false;
  passesSTheta = false;
  passesMissP = false;

  passesISR = false;
  passesWW = false;
  passesD2 = false;
  passesCW = false;

  initFourJet.clear();
  finalFourJet.clear();

  initFourJet.push_back({0,0,0,0});
  initFourJet.push_back({0,0,0,0});
  initFourJet.push_back({0,0,0,0});
  initFourJet.push_back({0,0,0,0});

  finalFourJet.push_back({0,0,0,0});
  finalFourJet.push_back({0,0,0,0});
  finalFourJet.push_back({0,0,0,0});
  finalFourJet.push_back({0,0,0,0});

  // if(inPart->nParticle < 4) return;

  Float_t TotalChgEnergy = 0;
  Int_t NTrk = 0;
  Int_t Neu = 0;
  Sphericity spher = Sphericity(inPart->nParticle, inPart->px, inPart->py, inPart->pz, inPart->pwflag, false);
  spher.setTree(inData);

  Float_t STheta = inData->STheta;
  Float_t MissP = inData->missP;

  for(Int_t pI = 0; pI < inPart->nParticle; ++pI){
    inPart->pmag[pI] = TMath::Sqrt(inPart->px[pI]*inPart->px[pI]+inPart->py[pI]*inPart->py[pI]+inPart->pz[pI]*inPart->pz[pI]);
    Double_t e = TMath::Sqrt(inPart->pmag[pI]*inPart->pmag[pI] + inPart->mass[pI]*inPart->mass[pI]);
    
    fastjet::PseudoJet ptl = fastjet::PseudoJet(inPart->px[pI], inPart->py[pI], inPart->pz[pI], e);
    ptl.set_user_index(pI);
    ptl.set_user_info(new MyInfo(inPart->pwflag[pI], inPart->highPurity[pI]));
    particles.push_back(ptl);

    if (neutralHadronSel.highPurity(inPart,pI)) {
       Neu++;
    }

    if(!(TMath::Abs(inPart->pwflag[pI])<=2 && inPart->highPurity[pI])) continue;
    TotalChgEnergy += TMath::Sqrt(inPart->pmag[pI]*inPart->pmag[pI] + inPart->mass[pI]*inPart->mass[pI]);
    NTrk += 1;
  }
  out_NeuNch = Neu+NTrk;
  out_cosSTheta = TMath::Abs(TMath::Cos(STheta));
  out_TotalChgEnergyMin = TotalChgEnergy;

  passesTotalChgEnergyMin = TotalChgEnergy >= 15;
  passesNTrkMin = NTrk >= 5;
  passesSTheta = TMath::Abs(TMath::Cos(STheta)) <= .82;
  passesMissP = MissP < 20;
  passesNeuNch = (Neu+NTrk)>=13;

  // removeISRgammaCandidate();
  if (particles.size()<=1)
  {
    passesISR = 0;
    out_sPrime= -999;
    out_mvis  = -999;
    return;
  }

  fastjet::ClusterSequence csISR(particles, jDefISR);
  std::vector<fastjet::PseudoJet> twoJetsFJ = csISR.exclusive_jets(nJISR);
  std::vector<TLorentzVector> twoJets;

  //two jet implementation follows https://arxiv.org/pdf/hep-ex/9810047.pdf, 1,2,3 eq.
  for(unsigned int i = 0; i < twoJetsFJ.size(); ++i){
    TLorentzVector temp(twoJetsFJ.at(i).px(), twoJetsFJ.at(i).py(), twoJetsFJ.at(i).pz(), twoJetsFJ.at(i).E());
    twoJets.push_back(temp);
  }
  Double_t absBeta = TMath::Abs(TMath::Sin(twoJets.at(0).Theta() + twoJets.at(1).Theta()));
  absBeta /= (TMath::Sin(twoJets.at(0).Theta()) + TMath::Sin(twoJets.at(1).Theta()));
  Double_t x = 2*absBeta/(1+absBeta);
  Double_t sPrime = inPart->Energy*inPart->Energy*(1-x);
  Double_t mass = (twoJets.at(0) + twoJets.at(1)).M();

  //cuts from http://cds.cern.ch/record/690637/files/ep-2003-084.pdf, page2, middle paragraph (before d^2 equation
  passesISR = mass/inPart->Energy > .7 || sPrime/(inPart->Energy*inPart->Energy) > .81;
  out_sPrime= sPrime;
  out_mvis  = mass;

  for(auto ele: particles)
  {
    // if (ele.user_info<MyInfo>().pwflag()>=0 and 
    //     ele.user_info<MyInfo>().pwflag()<=2 // and 
    //     // ele.user_info<MyInfo>().highPurity() and 
    //     // ele.e() > 1
    //     ) 
      particlesFourJetClustering.push_back(ele);
  }
  // if(!passesISR) return;
  if (particlesFourJetClustering.size()<=3)
  {
    passesD2 = 0;
    passesCW = 0;
    passesWW = 0;
    out_d2 = -999;
    out_cW = -999;
    return;
  }

  fastjet::ClusterSequence csWW(particlesFourJetClustering, jDefWW);
  std::vector<fastjet::PseudoJet> fourJetsFJ = csWW.exclusive_jets(nJWW);
  std::vector<TLorentzVector> fourJets;

  for(unsigned int i = 0; i < fourJetsFJ.size(); ++i){
    TLorentzVector temp(fourJetsFJ.at(i).px(), fourJetsFJ.at(i).py(), fourJetsFJ.at(i).pz(), fourJetsFJ.at(i).E());
    fourJets.push_back(temp);
    initFourJet.at(i) = temp;
  }

  for(unsigned int i = 0; i < fourJets.size(); ++i){
    for(unsigned int j = i+1; j < fourJets.size(); ++j){
      if(fourJets.at(i).E() > fourJets.at(j).E()){
  TLorentzVector temp = fourJets.at(j);
  fourJets.at(j) = fourJets.at(i);
  fourJets.at(i) = temp;

  initFourJet.at(j) = initFourJet.at(i);
  initFourJet.at(i) = temp;
      }
    }
  }

  pxSumInit_ = 0;
  pySumInit_ = 0;
  pzSumInit_ = 0;
  eSumInit_ = 0;

  pxSumFinal_ = 0;
  pySumFinal_ = 0;
  pzSumFinal_ = 0;
  eSumFinal_ = 0;
  
  for(unsigned int i = 0; i < fourJets.size(); ++i){
    pxSumInit_ += fourJets.at(i).Px();
    pySumInit_ += fourJets.at(i).Py();
    pzSumInit_ += fourJets.at(i).Pz();
    eSumInit_ += fourJets.at(i).E();
  }
  
  bool doRescale = (TMath::Abs(pxSumInit_) > 1. || TMath::Abs(pySumInit_) > 1. || TMath::Abs(pzSumInit_) > 1. || TMath::Abs(eSumInit_ - inPart->Energy) > 1.);
  std::vector<double> factors;

  factorA_ = 1.;
  factorB_ = 1.;
  factorC_ = 1.;
  factorD_ = 1.;
  
  while(doRescale){
    doRescale = checkDoRescale(&fourJets, &factors, inPart->Energy);
    factorA_ = factors.at(0);
    factorB_ = factors.at(1);
    factorC_ = factors.at(2);
    factorD_ = factors.at(3);
  }


  /*
  Float_t pxSumMid_ = 0;
  Float_t pySumMid_ = 0;
  Float_t pzSumMid_ = 0;
  Float_t eSumMid_ = 0;
  for(unsigned int i = 0; i < fourJets.size(); ++i){
    pxSumMid_ += fourJets.at(i).Px();
    pySumMid_ += fourJets.at(i).Py();
    pzSumMid_ += fourJets.at(i).Pz();
    eSumMid_ += fourJets.at(i).E();
  }

  masslessRescale_ = inPart->Energy/eSumMid_;
  //  std::cout << pxSumMid_ << ", " << pySumMid_ << ", " << pzSumMid_ << std::endl;

  for(unsigned int i = 0; i < fourJets.size(); ++i){fourJets.at(i) *= masslessRescale_;}
  */

  for(unsigned int i = 0; i < fourJets.size(); ++i){
    pxSumFinal_ += fourJets.at(i).Px();
    pySumFinal_ += fourJets.at(i).Py();
    pzSumFinal_ += fourJets.at(i).Pz();
    eSumFinal_ += fourJets.at(i).E();
    finalFourJet.at(i) = fourJets.at(i);
  }

  _d2 = 999999999.;
  Double_t smallestAngle = 999999.;
  
  for(unsigned int i = 0; i < fourJets.size()-1; ++i){
    for(unsigned int j = i+1; j < fourJets.size(); ++j){
      Double_t tempAngle = fourJets.at(i).Angle(fourJets.at(j).Vect());

      if(tempAngle < smallestAngle) smallestAngle = tempAngle;
    }
  }
  
  _cW = TMath::Cos(smallestAngle);

  std::vector<double> twoJetMasses;
  for(unsigned int i = 0; i < fourJets.size()-1; ++i){
    for(unsigned int j = i+1; j < fourJets.size(); ++j){
      TLorentzVector temp = fourJets.at(i) + fourJets.at(j);
      twoJetMasses.push_back(temp.M());
    }
  }

  for(unsigned int i = 0; i < twoJetMasses.size()-1; ++i){
    for(unsigned int j = i+1; j < twoJetMasses.size(); ++j){
      Double_t d2Temp = (twoJetMasses.at(i) - 80.4)*(twoJetMasses.at(i) - 80.4);
      d2Temp += (twoJetMasses.at(j) - 80.4)*(twoJetMasses.at(j) - 80.4);
      d2Temp /= (80.4*80.4);
      
      if(d2Temp < _d2) _d2 = d2Temp;
    }
  }

  passesD2 = _d2 >= .1;
  passesCW = _cW >= .9;
  passesWW = passesD2 || passesCW;
  out_d2 = _d2;
  out_cW = _cW;

  return;
}
#endif
