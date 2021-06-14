#ifndef DiTau_DiTauAnalysis_H
#define DiTau_DiTauAnalysis_H

#include <AnaAlgorithm/AnaAlgorithm.h>

#include "DiTauRec/HadElBuilder.h"
#include "DiTauRec/HadMuBuilder.h"

#include "tauRecTools/DiTauDiscriminantTool.h"
#include "tauRecTools/DiTauIDVarCalculator.h"
#include "tauRecTools/DiTauWPDecorator.h"
#include "tauRecTools/BuildTruthTaus.h"

#include "TauAnalysisTools/DiTauTruthMatchingTool.h"
#include "TauAnalysisTools/TauTruthMatchingTool.h"

#include "JetRecTools/CaloClusterConstituentsOrigin.h"
#include "JetInterface/IJetConstituentModifier.h"
#include "JetRecTools/JetConstituentModSequence.h" 

class DiTauAnalysis : public EL::AnaAlgorithm
{
public:
  // this is a standard algorithm constructor
  DiTauAnalysis (const std::string& name, ISvcLocator* pSvcLocator);

  ~DiTauAnalysis () override;


  // these are the functions inherited from Algorithm
  virtual StatusCode initialize () override;
  virtual StatusCode execute () override;
  virtual StatusCode finalize () override;

private:
  // Configuration, and any other types of variables go here.
  
  //Create TFile and TTree
  TFile* m_myfile = 0;
  TTree* m_mytree = 0;

  TauAnalysisTools::TauTruthMatchingTool* m_tauTruthMatchingTool;

  tauRecTools::DiTauIDVarCalculator* m_hadElDiTauIDVarCalculator;
  tauRecTools::DiTauDiscriminantTool* m_hadElDiTauDiscrTool;
  tauRecTools::DiTauWPDecorator* m_hadElDiTauWPDecorator;

  tauRecTools::DiTauIDVarCalculator* m_hadMuDiTauIDVarCalculator;
  tauRecTools::DiTauDiscriminantTool* m_hadMuDiTauDiscrTool;
  tauRecTools::DiTauWPDecorator* m_hadMuDiTauWPDecorator;

  tauRecTools::BuildTruthTaus* m_buildTruthTaus;

  DiTauRec::ILepHadBuilder* m_hadElBuilder;
  DiTauRec::ILepHadBuilder* m_hadMuBuilder;

  TauAnalysisTools::DiTauTruthMatchingTool* m_diTauTruthMatchingTool;
  
  ToolHandleArray<IJetConstituentModifier> modifiers;
  CaloClusterConstituentsOrigin* m_pCaloClusterConstituentsOrigin;

  unsigned int m_eventsPassed = 0;
  unsigned int m_pdgTruthMatchType = 0;
  unsigned int m_truthMatchedHadHad = 0;
  unsigned int m_truthMatchedHadEl = 0;
  unsigned int m_truthMatchedHadMu = 0;

  unsigned int m_runNumber = 0;
  unsigned long long m_eventNumber = 0;

  float m_truthHiggsPt = 0;
  float m_truthHiggsEta = 0;
  float m_truthHiggsPhi = 0;
  float m_truthHiggsE = 0;

  float m_truthHadTauPt = 0;
  float m_truthHadTauEta = 0;
  float m_truthHadTauPhi = 0;
  float m_truthHadTauE = 0;
  int m_truthHadTauPdgId = 0;
  float m_truthHadTauVisPt = 0;
  float m_truthHadTauVisEta = 0;
  float m_truthHadTauVisPhi = 0;
  float m_truthHadTauVisM = 0;

  float m_truthLepTauPt = 0;
  float m_truthLepTauEta = 0;
  float m_truthLepTauPhi = 0;
  float m_truthLepTauE = 0;
  int m_truthLepTauPdgId = 0;

  float m_truthFinalLeptonPt = 0;
  float m_truthFinalLeptonEta = 0;
  float m_truthFinalLeptonPhi = 0;
  float m_truthFinalLeptonE = 0;
  int m_truthFinalLeptonPdgId = 0;

  unsigned int m_nDiTaus = 0;
  std::vector<float> *m_diTauPt = nullptr;
  std::vector<float> *m_diTauEta = nullptr;
  std::vector<float> *m_diTauPhi = nullptr;
  std::vector<unsigned int> *m_nDiTauSubjets = nullptr;

  unsigned int m_nHadElDiTaus = 0;
  std::vector<float> *m_hadElDiTauPt = nullptr;
  std::vector<float> *m_hadElDiTauEta = nullptr;
  std::vector<float> *m_hadElDiTauPhi = nullptr;
  std::vector<float> *m_hadElDiTauBDTScore = nullptr;
  std::vector<float> *m_hadElDiTauFlatBDTScore = nullptr;
  std::vector<unsigned int> *m_hadElDiTauTruthMatchType = nullptr;

  unsigned int m_nHadMuDiTaus = 0;
  std::vector<float> *m_hadMuDiTauPt = nullptr;
  std::vector<float> *m_hadMuDiTauEta = nullptr;
  std::vector<float> *m_hadMuDiTauPhi = nullptr;
  std::vector<float> *m_hadMuDiTauBDTScore = nullptr;
  std::vector<float> *m_hadMuDiTauFlatBDTScore = nullptr;
  std::vector<unsigned int> *m_hadMuDiTauTruthMatchType = nullptr;

  unsigned int m_nTaus = 0;
  std::vector<float> *m_tauPt = nullptr;
  std::vector<float> *m_tauEta = nullptr;
  std::vector<float> *m_tauPhi = nullptr;
  std::vector<float> *m_tauE = nullptr;
  std::vector<unsigned int> *m_tauLoose = nullptr;
  std::vector<unsigned int> *m_tauMedium = nullptr;
  std::vector<unsigned int> *m_tauTight = nullptr;

  unsigned int m_nElectrons = 0;
  std::vector<float> *m_elePt = nullptr;
  std::vector<float> *m_eleEta = nullptr;
  std::vector<float> *m_elePhi = nullptr;
  std::vector<float> *m_eleE = nullptr;
  std::vector<unsigned int> *m_eleLoose = nullptr;
  std::vector<unsigned int> *m_eleMedium = nullptr;
  std::vector<unsigned int> *m_eleTight = nullptr;

  unsigned int m_nMuons = 0;
  std::vector<float> *m_muPt = nullptr;
  std::vector<float> *m_muEta = nullptr;
  std::vector<float> *m_muPhi = nullptr;
  std::vector<unsigned int> *m_muLoose = nullptr;
  std::vector<unsigned int> *m_muMedium = nullptr;
  std::vector<unsigned int> *m_muTight = nullptr;


};

#endif

