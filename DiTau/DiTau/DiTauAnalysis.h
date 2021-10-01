#ifndef DiTau_DiTauAnalysis_H
#define DiTau_DiTauAnalysis_H

#include <AnaAlgorithm/AnaAlgorithm.h>

#include "AsgTools/ToolHandle.h"

#include "xAODEventFormat/EventFormat.h"

#include "DiTauRec/HadElBuilder.h"
#include "DiTauRec/HadMuBuilder.h"

#include "tauRecTools/DiTauDiscriminantTool.h"
#include "tauRecTools/DiTauIDVarCalculator.h"
#include "tauRecTools/DiTauWPDecorator.h"
#include "tauRecTools/BuildTruthTaus.h"
#include "tauRecTools/IDiTauToolBase.h"

#include "TauAnalysisTools/DiTauTruthMatchingTool.h"
#include "TauAnalysisTools/IDiTauTruthMatchingTool.h"
#include "TauAnalysisTools/TauTruthMatchingTool.h"

#include "JetRecTools/CaloClusterConstituentsOrigin.h"
#include "JetRecTools/JetConstituentModSequence.h" 
#include "JetInterface/IJetConstituentModifier.h"
#include "JetInterface/IJetExecuteTool.h"

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
  
  bool m_isSignal;

  //Create TFile and TTree
  TFile* m_myfile = 0;
  TTree* m_mytree = 0;

  xAOD::EventFormat* m_ef;

  //TauAnalysisTools::TauTruthMatchingTool* m_tauTruthMatchingTool;

  ToolHandle<tauRecTools::IDiTauToolBase> m_hadElDiTauIDVarCalculator;
  ToolHandle<tauRecTools::IDiTauToolBase> m_hadMuDiTauIDVarCalculator;

  ToolHandle<tauRecTools::IDiTauToolBase> m_hadElDiTauDiscrTool;
  ToolHandle<tauRecTools::IDiTauToolBase> m_hadMuDiTauDiscrTool;

  ToolHandle<tauRecTools::IDiTauToolBase> m_hadElDiTauWPDecorator;
  ToolHandle<tauRecTools::IDiTauToolBase> m_hadMuDiTauWPDecorator;

  DiTauRec::ILepHadBuilder* m_hadElBuilder;
  DiTauRec::ILepHadBuilder* m_hadMuBuilder;

  ToolHandle<TauAnalysisTools::IDiTauTruthMatchingTool> m_diTauTruthMatchingTool;
  
  ToolHandle<IJetConstituentModifier> m_pCaloClusterConstituentsOrigin;
  ToolHandle<IJetExecuteTool> m_jetConstituentModSequence;

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

  float m_truthDiTauVisPt = 0;
  float m_truthDiTauVisEta = 0;
  float m_truthDiTauVisPhi = 0;
  float m_truthDiTauVisE = 0;

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

  // Taus
  unsigned int m_nTaus = 0;
  std::vector<float> *m_tauPt = nullptr;
  std::vector<float> *m_tauEta = nullptr;
  std::vector<float> *m_tauPhi = nullptr;
  std::vector<float> *m_tauE = nullptr;
  float m_leadingTauPt = 0;
  float m_leadingTauEta = 0;
  float m_leadingTauPhi = 0;
  float m_leadingTauE = 0;

  // Loose taus
  unsigned int m_nTausLoose = 0;
  std::vector<float> *m_tauLoosePt = nullptr;
  std::vector<float> *m_tauLooseEta = nullptr;
  std::vector<float> *m_tauLoosePhi = nullptr;
  std::vector<float> *m_tauLooseE = nullptr;
  float m_leadingTauLoosePt = 0;
  float m_leadingTauLooseEta = 0;
  float m_leadingTauLoosePhi = 0;
  float m_leadingTauLooseE = 0;

  // Medium taus
  unsigned int m_nTausMedium = 0;
  std::vector<float> *m_tauMediumPt = nullptr;
  std::vector<float> *m_tauMediumEta = nullptr;
  std::vector<float> *m_tauMediumPhi = nullptr;
  std::vector<float> *m_tauMediumE = nullptr;
  float m_leadingTauMediumPt = 0;
  float m_leadingTauMediumEta = 0;
  float m_leadingTauMediumPhi = 0;
  float m_leadingTauMediumE = 0;

  // Tight taus
  unsigned int m_nTausTight = 0;
  std::vector<float> *m_tauTightPt = nullptr;
  std::vector<float> *m_tauTightEta = nullptr;
  std::vector<float> *m_tauTightPhi = nullptr;
  std::vector<float> *m_tauTightE = nullptr;
  float m_leadingTauTightPt = 0;
  float m_leadingTauTightEta = 0;
  float m_leadingTauTightPhi = 0;
  float m_leadingTauTightE = 0;

  // Muons
  unsigned int m_nMuons = 0;
  std::vector<float> *m_muPt = nullptr;
  std::vector<float> *m_muEta = nullptr;
  std::vector<float> *m_muPhi = nullptr;
  std::vector<float> *m_muE = nullptr;
  float m_leadingMuPt = 0;
  float m_leadingMuEta = 0;
  float m_leadingMuPhi = 0;
  float m_leadingMuE = 0;

  // Loose muons
  unsigned int m_nMuonsLoose = 0;
  std::vector<float> *m_muLoosePt = nullptr;
  std::vector<float> *m_muLooseEta = nullptr;
  std::vector<float> *m_muLoosePhi = nullptr;
  std::vector<float> *m_muLooseE = nullptr;
  float m_leadingMuLoosePt = 0;
  float m_leadingMuLooseEta = 0;
  float m_leadingMuLoosePhi = 0;
  float m_leadingMuLooseE = 0;

  // Medium muons
  unsigned int m_nMuonsMedium = 0;
  std::vector<float> *m_muMediumPt = nullptr;
  std::vector<float> *m_muMediumEta = nullptr;
  std::vector<float> *m_muMediumPhi = nullptr;
  std::vector<float> *m_muMediumE = nullptr;
  float m_leadingMuMediumPt = 0;
  float m_leadingMuMediumEta = 0;
  float m_leadingMuMediumPhi = 0;
  float m_leadingMuMediumE = 0;

  // Tight muons
  unsigned int m_nMuonsTight = 0;
  std::vector<float> *m_muTightPt = nullptr;
  std::vector<float> *m_muTightEta = nullptr;
  std::vector<float> *m_muTightPhi = nullptr;
  std::vector<float> *m_muTightE = nullptr;
  float m_leadingMuTightPt = 0;
  float m_leadingMuTightEta = 0;
  float m_leadingMuTightPhi = 0;
  float m_leadingMuTightE = 0;

  // Electrons
  unsigned int m_nElectrons = 0;
  std::vector<float> *m_elePt = nullptr;
  std::vector<float> *m_eleEta = nullptr;
  std::vector<float> *m_elePhi = nullptr;
  std::vector<float> *m_eleE = nullptr;
  float m_leadingElePt = 0;
  float m_leadingEleEta = 0;
  float m_leadingElePhi = 0;
  float m_leadingEleE = 0;

  // Loose electrons
  unsigned int m_nElectronsLoose = 0;
  std::vector<float> *m_eleLoosePt = nullptr;
  std::vector<float> *m_eleLooseEta = nullptr;
  std::vector<float> *m_eleLoosePhi = nullptr;
  std::vector<float> *m_eleLooseE = nullptr;
  float m_leadingEleLoosePt = 0;
  float m_leadingEleLooseEta = 0;
  float m_leadingEleLoosePhi = 0;
  float m_leadingEleLooseE = 0;

  // Medium electrons
  unsigned int m_nElectronsMedium = 0;
  std::vector<float> *m_eleMediumPt = nullptr;
  std::vector<float> *m_eleMediumEta = nullptr;
  std::vector<float> *m_eleMediumPhi = nullptr;
  std::vector<float> *m_eleMediumE = nullptr;
  float m_leadingEleMediumPt = 0;
  float m_leadingEleMediumEta = 0;
  float m_leadingEleMediumPhi = 0;
  float m_leadingEleMediumE = 0;

  // Tight electrons
  unsigned int m_nElectronsTight = 0;
  std::vector<float> *m_eleTightPt = nullptr;
  std::vector<float> *m_eleTightEta = nullptr;
  std::vector<float> *m_eleTightPhi = nullptr;
  std::vector<float> *m_eleTightE = nullptr;
  float m_leadingEleTightPt = 0;
  float m_leadingEleTightEta = 0;
  float m_leadingEleTightPhi = 0;
  float m_leadingEleTightE = 0;

  // Jets
  unsigned int m_nJets = 0;
  std::vector<float> *m_jetPt = nullptr;
  std::vector<float> *m_jetEta = nullptr;
  std::vector<float> *m_jetPhi = nullptr;
  std::vector<float> *m_jetE = nullptr;
  float m_leadingJetPt = 0;
  float m_leadingJetEta = 0;
  float m_leadingJetPhi = 0;
  float m_leadingJetE = 0;
  float m_subleadingJetPt = 0;
  float m_subleadingJetEta = 0;
  float m_subleadingJetPhi = 0;
  float m_subleadingJetE = 0;

  unsigned int m_nLargeRJets = 0;
  std::vector<float> *m_lRJetPt = nullptr;
  std::vector<float> *m_lRJetEta = nullptr;
  std::vector<float> *m_lRJetPhi = nullptr;
  std::vector<float> *m_lRJetE = nullptr;
  std::vector<float> *m_lRJetM = nullptr;
  float m_leadingLRJetPt = 0;
  float m_leadingLRJetEta = 0;
  float m_leadingLRJetPhi = 0;
  float m_leadingLRJetE = 0;
  float m_leadingLRJetM = 0;
  float m_subleadingLRJetPt = 0;
  float m_subleadingLRJetEta = 0;
  float m_subleadingLRJetPhi = 0;
  float m_subleadingLRJetE = 0;
  float m_subleadingLRJetM = 0;

};

#endif

