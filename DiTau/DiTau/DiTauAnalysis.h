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

#include "EgammaAnalysisInterfaces/IAsgElectronLikelihoodTool.h"

#include "JetRecTools/CaloClusterConstituentsOrigin.h"
#include "JetRecTools/JetConstituentModSequence.h" 
#include "JetInterface/IJetConstituentModifier.h"
#include "JetInterface/IJetExecuteTool.h"

#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "FlavorTagDiscriminants/HbbTag.h"

#include "DiTauMassTools/IMissingMassTool.h"

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
  bool m_isDAOD;
  std::string m_jetCollectionName;
  std::string m_largeRJetCollectionName;

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

  ToolHandle<IAsgElectronLikelihoodTool> m_checkEleVeryLooseLH;
  ToolHandle<IAsgElectronLikelihoodTool> m_checkEleLooseLH;
  ToolHandle<IAsgElectronLikelihoodTool> m_checkEleMediumLH;
  ToolHandle<IAsgElectronLikelihoodTool> m_checkEleTightLH;
  
  ToolHandle<IJetConstituentModifier> m_pCaloClusterConstituentsOrigin;
  ToolHandle<IJetExecuteTool> m_jetConstituentModSequence;

  ToolHandle<IBTaggingSelectionTool> m_bTaggingSelectionTool;
    
  ToolHandle<IMissingMassTool> m_missingMassTool;

  std::unique_ptr<FlavorTagDiscriminants::HbbTag> m_hbbTagTool;

  unsigned int m_eventsPassed = 0;
  unsigned int m_pdgTruthMatchType = 0;
  unsigned int m_truthMatchedHadHad = 0;
  unsigned int m_truthMatchedHadEl = 0;
  unsigned int m_truthMatchedHadMu = 0;

  unsigned int m_runNumber = 0;
  unsigned long long m_eventNumber = 0;
  unsigned long long m_uniqueEventID = 0;

  float m_truthBPt = 0;
  float m_truthBEta = 0;
  float m_truthBPhi = 0;
  float m_truthBE = 0;

  float m_truthAntiBPt = 0;
  float m_truthAntiBEta = 0;
  float m_truthAntiBPhi = 0;
  float m_truthAntiBE = 0;

  float m_truthBdR = 0;

  float m_truthTauHiggsPt = 0;
  float m_truthTauHiggsEta = 0;
  float m_truthTauHiggsPhi = 0;
  float m_truthTauHiggsE = 0;

  float m_truthBHiggsPt = 0;
  float m_truthBHiggsEta = 0;
  float m_truthBHiggsPhi = 0;
  float m_truthBHiggsE = 0;

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

  float m_truthHadTauVisFinalLeptondR = 0;

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
  std::vector<float> *m_hadElDiTauM = nullptr;
  std::vector<float> *m_hadElDiTauBDTScore = nullptr;
  float m_bestHadElDiTauBDTScore = 0;
  std::vector<float> *m_hadElDiTauFlatBDTScore = nullptr;
  std::vector<unsigned int> *m_hadElDiTauTruthMatchType = nullptr;
  unsigned int m_nHadElTaus = 0;
  std::vector<float> *m_hadElTauPt = nullptr;
  std::vector<float> *m_hadElTauEta = nullptr;
  std::vector<float> *m_hadElTauPhi = nullptr;
  std::vector<float> *m_hadElTauE = nullptr;
  std::vector<float> *m_hadElTauTruthdR = nullptr;
  unsigned int m_nHadElElectrons = 0;
  std::vector<float> *m_hadElElectronPt = nullptr;
  std::vector<float> *m_hadElElectronEta = nullptr;
  std::vector<float> *m_hadElElectronPhi = nullptr;
  std::vector<float> *m_hadElElectronE = nullptr;
  std::vector<float> *m_hadElDiTaudR = nullptr;

  unsigned int m_nHadMuDiTaus = 0;
  std::vector<float> *m_hadMuDiTauPt = nullptr;
  std::vector<float> *m_hadMuDiTauEta = nullptr;
  std::vector<float> *m_hadMuDiTauPhi = nullptr;
  std::vector<float> *m_hadMuDiTauM = nullptr;
  std::vector<float> *m_hadMuDiTauBDTScore = nullptr;
  float m_bestHadMuDiTauBDTScore = 0;
  std::vector<float> *m_hadMuDiTauFlatBDTScore = nullptr;
  std::vector<unsigned int> *m_hadMuDiTauTruthMatchType = nullptr;
  unsigned int m_nHadMuTaus = 0;
  std::vector<float> *m_hadMuTauPt = nullptr;
  std::vector<float> *m_hadMuTauEta = nullptr;
  std::vector<float> *m_hadMuTauPhi = nullptr;
  std::vector<float> *m_hadMuTauE = nullptr;
  std::vector<float> *m_hadMuTauTruthdR = nullptr;
  unsigned int m_nHadMuMuons = 0;
  std::vector<float> *m_hadMuMuonPt = nullptr;
  std::vector<float> *m_hadMuMuonEta = nullptr;
  std::vector<float> *m_hadMuMuonPhi = nullptr;
  std::vector<float> *m_hadMuMuonE = nullptr;
  std::vector<float> *m_hadMuDiTaudR = nullptr;

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
  std::vector<float> *m_tauTruthdR = nullptr;
  std::vector<unsigned int> *m_tauID = nullptr;
  float m_tauRecoTruthMindR = 0;

  // Very Loose taus
  unsigned int m_nTausVeryLoose = 0;
  std::vector<float> *m_tauVeryLoosePt = nullptr;
  std::vector<float> *m_tauVeryLooseEta = nullptr;
  std::vector<float> *m_tauVeryLoosePhi = nullptr;
  std::vector<float> *m_tauVeryLooseE = nullptr;
  float m_leadingTauVeryLoosePt = 0;
  float m_leadingTauVeryLooseEta = 0;
  float m_leadingTauVeryLoosePhi = 0;
  float m_leadingTauVeryLooseE = 0;

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
  std::vector<float> *m_muTruthdR = nullptr;
  std::vector<unsigned int> *m_muID = nullptr;
  float m_muRecoTruthMindR = 0;

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
  std::vector<float> *m_eleTruthdR = nullptr;
  std::vector<unsigned int> *m_eleID = nullptr;

  float m_eleRecoTruthMindR = 0;

  // Very Loose electrons
  unsigned int m_nElectronsVeryLoose = 0;
  std::vector<float> *m_eleVeryLoosePt = nullptr;
  std::vector<float> *m_eleVeryLooseEta = nullptr;
  std::vector<float> *m_eleVeryLoosePhi = nullptr;
  std::vector<float> *m_eleVeryLooseE = nullptr;
  float m_leadingEleVeryLoosePt = 0;
  float m_leadingEleVeryLooseEta = 0;
  float m_leadingEleVeryLoosePhi = 0;
  float m_leadingEleVeryLooseE = 0;

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

  // Tau-Ele ditau system
  std::vector<float> *m_tauEleLoosePt = nullptr;
  std::vector<float> *m_tauEleLooseEta = nullptr;
  std::vector<float> *m_tauEleLoosePhi = nullptr;
  std::vector<float> *m_tauEleLooseE = nullptr;
  std::vector<float> *m_tauEleLooseM = nullptr;
  std::vector<float> *m_tauEleLoosedR = nullptr;

  // Chosen Tau-Ele system
  float m_hadElChosenTauPt = 0;
  float m_hadElChosenTauEta = 0;
  float m_hadElChosenTauPhi = 0;
  float m_hadElChosenTauE = 0;
  float m_hadElChosenTauM = 0;
  float m_hadElChosenTauTruthdR = 0;
  float m_hadElChosenElePt = 0;
  float m_hadElChosenEleEta = 0;
  float m_hadElChosenElePhi = 0;
  float m_hadElChosenEleE = 0;
  float m_hadElChosenEleM = 0;
  float m_hadElChosenEleID = 0;
  float m_hadElChosenEleTruthdR = 0;
  float m_hadElChosenTauElePt = 0;
  float m_hadElChosenTauEleEta = 0;
  float m_hadElChosenTauElePhi = 0;
  float m_hadElChosenTauEleE = 0;
  float m_hadElChosenTauEleM = 0;
  float m_hadElChosenTauEledR = 0;

  // Tau-Mu ditau system
  std::vector<float> *m_tauMuTightPt = nullptr;
  std::vector<float> *m_tauMuTightEta = nullptr;
  std::vector<float> *m_tauMuTightPhi = nullptr;
  std::vector<float> *m_tauMuTightE = nullptr;
  std::vector<float> *m_tauMuTightM = nullptr;
  std::vector<float> *m_tauMuTightdR = nullptr;

  // Chosen Tau-Mu system
  float m_hadMuChosenTauPt = 0;
  float m_hadMuChosenTauEta = 0;
  float m_hadMuChosenTauPhi = 0;
  float m_hadMuChosenTauE = 0;
  float m_hadMuChosenTauM = 0;
  float m_hadMuChosenTauTruthdR = 0;
  float m_hadMuChosenMuPt = 0;
  float m_hadMuChosenMuEta = 0;
  float m_hadMuChosenMuPhi = 0;
  float m_hadMuChosenMuE = 0;
  float m_hadMuChosenMuM = 0;
  float m_hadMuChosenMuID = 0;
  float m_hadMuChosenMuTruthdR = 0;
  float m_hadMuChosenTauMuPt = 0;
  float m_hadMuChosenTauMuEta = 0;
  float m_hadMuChosenTauMuPhi = 0;
  float m_hadMuChosenTauMuE = 0;
  float m_hadMuChosenTauMuM = 0;
  float m_hadMuChosenTauMudR = 0;

  // Resolved analysis values
  float m_resolvedMissingMass = 0;

  // Jets
  unsigned int m_nJets = 0;
  std::vector<float> *m_jetPt = nullptr;
  std::vector<float> *m_jetEta = nullptr;
  std::vector<float> *m_jetPhi = nullptr;
  std::vector<float> *m_jetE = nullptr;
  std::vector<float> *m_jetBTagAccept = nullptr;
  float m_leadingJetPt = 0;
  float m_leadingJetEta = 0;
  float m_leadingJetPhi = 0;
  float m_leadingJetE = 0;
  float m_subleadingJetPt = 0;
  float m_subleadingJetEta = 0;
  float m_subleadingJetPhi = 0;
  float m_subleadingJetE = 0;

  // B-tagged Jets
  unsigned int m_nBTagJets = 0;
  std::vector<float> *m_bTagJetPt = nullptr;
  std::vector<float> *m_bTagJetEta = nullptr;
  std::vector<float> *m_bTagJetPhi = nullptr;
  std::vector<float> *m_bTagJetE = nullptr;
  float m_leadingBTagJetPt = 0;
  float m_leadingBTagJetEta = 0;
  float m_leadingBTagJetPhi = 0;
  float m_leadingBTagJetE = 0;
  float m_subleadingBTagJetPt = 0;
  float m_subleadingBTagJetEta = 0;
  float m_subleadingBTagJetPhi = 0;
  float m_subleadingBTagJetE = 0;

  // Large-R Jets
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

  float m_largeRJetdR = 0;
  float m_largeRJetdEta = 0;
  float m_largeRJetdPhi = 0;
  float m_largeRJetdPt = 0;

  // Tau large-R jet
  float m_tauLRJetPt = 0;
  float m_tauLRJetEta = 0;
  float m_tauLRJetPhi = 0;
  float m_tauLRJetE = 0;
  float m_tauLRJetM = 0;
  float m_tauLRJetEleLoosedR = 0;
  float m_tauLRJetMuTightdR = 0;

  // b large-R jet
  float m_bLRJetPt = 0;
  float m_bLRJetEta = 0;
  float m_bLRJetPhi = 0;
  float m_bLRJetE = 0;
  float m_bLRJetM = 0;
  float m_bLRJetEleLoosedR = 0;
  float m_bLRJetMuTightdR = 0;

  // Large-R X (= di-Higgs)
  float m_largeRXPt = 0;
  float m_largeRXEta = 0;
  float m_largeRXPhi = 0;
  float m_largeRXE = 0;
  float m_largeRXM = 0;
  
  // Hbb large-R jets
  unsigned int m_nLargeRHbbJets = 0;
  std::vector<float> *m_lRHbbJetPt = nullptr;
  std::vector<float> *m_lRHbbJetEta = nullptr;
  std::vector<float> *m_lRHbbJetPhi = nullptr;
  std::vector<float> *m_lRHbbJetE = nullptr;
  std::vector<float> *m_lRHbbJetM = nullptr;
  std::vector<float> *m_lRHbbJetpHiggs = nullptr;
  std::vector<float> *m_lRHbbJetpQCD = nullptr;
  std::vector<float> *m_lRHbbJetpTop = nullptr;
  std::vector<float> *m_lRHbbJetDiscriminant = nullptr;
  std::vector<float> *m_lRHbbJetTruthdR = nullptr;
  
  float m_leadingLRHbbJetPt = 0;
  float m_leadingLRHbbJetEta = 0;
  float m_leadingLRHbbJetPhi = 0;
  float m_leadingLRHbbJetE = 0;
  float m_leadingLRHbbJetM = 0;
  float m_leadingLRHbbJetDisc = 0;

  float m_subleadingLRHbbJetPt = 0;
  float m_subleadingLRHbbJetEta = 0;
  float m_subleadingLRHbbJetPhi = 0;
  float m_subleadingLRHbbJetE = 0;
  float m_subleadingLRHbbJetM = 0;
  float m_subleadingLRHbbJetDisc = 0;
  
  float m_maxDiscLRHbbJetDisc = 0;

  float m_hadElLRHbbJetPt = 0;
  float m_hadElLRHbbJetEta = 0;
  float m_hadElLRHbbJetPhi = 0;
  float m_hadElLRHbbJetE = 0;
  float m_hadElLRHbbJetM = 0;
  float m_hadElLRHbbJetDisc = 0;
  float m_hadElLRHbbJetTruthdR = 0;
  
  float m_hadElDiHiggsPt = 0;
  float m_hadElDiHiggsEta = 0;
  float m_hadElDiHiggsPhi = 0;
  float m_hadElDiHiggsE = 0;
  float m_hadElDiHiggsM = 0;
  float m_hadElDiHiggsTruthdR = 0;
  
  float m_hadMuLRHbbJetPt = 0;
  float m_hadMuLRHbbJetEta = 0;
  float m_hadMuLRHbbJetPhi = 0;
  float m_hadMuLRHbbJetE = 0;
  float m_hadMuLRHbbJetM = 0;
  float m_hadMuLRHbbJetDisc = 0;
  float m_hadMuLRHbbJetTruthdR = 0;

  float m_hadMuDiHiggsPt = 0;
  float m_hadMuDiHiggsEta = 0;
  float m_hadMuDiHiggsPhi = 0;
  float m_hadMuDiHiggsE = 0;
  float m_hadMuDiHiggsM = 0;
  float m_hadMuDiHiggsDisc = 0;
  float m_hadMuDiHiggsTruthdR = 0;

  float calculate_hbb_discriminant(const xAOD::Jet * jet) {
    float p_higgs = jet->auxdecor<float>("Xbb202006_Higgs");
    float p_qcd = jet->auxdecor<float>("Xbb202006_QCD");
    float p_top = jet->auxdecor<float>("Xbb202006_Top");
    
    float anti_topness = 0.25;
    float discriminant = log(p_higgs / ((1 - anti_topness) * p_qcd + anti_topness * p_top));

    return discriminant;
  }
};

#endif

