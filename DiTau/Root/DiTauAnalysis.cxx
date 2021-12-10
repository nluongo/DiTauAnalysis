#include <DiTau/DiTauAnalysis.h>

#include <xAODEventInfo/EventInfo.h>

#include "xAODCaloEvent/CaloClusterContainer.h"

#include "AsgTools/ToolHandle.h"
#include "AsgTools/ToolHandleArray.h"
#include "AsgTools/AsgToolConfig.h"

#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJet.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/DiTauJet.h"
#include "xAODTau/DiTauJetContainer.h"
#include "xAODMissingET/MissingETContainer.h"

#include "TFile.h"

#include <typeinfo>
#include <cmath>

DiTauAnalysis :: DiTauAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator),
    m_diTauTruthMatchingTool ("DiTauTruthMatchingTool", this),
    m_hadElDiTauIDVarCalculator ("DiTauIDVarCalculator/HadElDiTauIDVarCalculator", this),
    m_hadMuDiTauIDVarCalculator ("DiTauIDVarCalculator/HadMuDiTauIDVarCalculator", this),
    m_hadElDiTauDiscrTool ("DiTauDiscriminantTool/HadElDiTauDiscrTool", this),
    m_hadMuDiTauDiscrTool ("DiTauDiscriminantTool/HadMuDiTauDiscrTool", this),
    m_hadElDiTauWPDecorator ("DiTauWPDecorator/HadElDiTauWPDecorator", this),
    m_hadMuDiTauWPDecorator ("DiTauWPDecorator/HadMuDiTauWPDecorator", this),
    m_jetConstituentModSequence("JetConstituentModSequence", this),
    m_checkEleVeryLooseLH("AsgElectronLikelihoodTool/AsgElectronVeryLooseLikelihoodTool", this),
    m_checkEleLooseLH("AsgElectronLikelihoodTool/AsgElectronLooseLikelihoodTool", this),
    m_checkEleMediumLH("AsgElectronLikelihoodTool/AsgElectronMediumLikelihoodTool", this),
    m_checkEleTightLH("AsgElectronLikelihoodTool/AsgElectronTightLikelihoodTool", this),
    m_bTaggingSelectionTool("BTaggingSelectionTool", this),
    m_missingMassTool("MissingMassTool", this)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.
  
  declareProperty( "isSignal" , m_isSignal=true );
  declareProperty( "diTauTruthMatchingTool", m_diTauTruthMatchingTool, "the DiTauTruthMatchingTool");
  declareProperty( "hadElDiTauIDVarCalculator", m_hadElDiTauIDVarCalculator, "the hadElDiTauIDVarCalculator");
  declareProperty( "hadMuDiTauIDVarCalculator", m_hadMuDiTauIDVarCalculator, "the hadMuDiTauIDVarCalculator");
  declareProperty( "hadElDiTauDiscrTool", m_hadElDiTauDiscrTool, "the hadElDiTauDiscriminantTool");
  declareProperty( "hadMuDiTauDiscrTool", m_hadMuDiTauDiscrTool, "the hadMuDiTauDiscriminantTool");
  declareProperty( "hadElDiTauWPDecorator", m_hadElDiTauWPDecorator, "the hadElDiTauWPDecorator");
  declareProperty( "hadMuDiTauWPDecorator", m_hadMuDiTauWPDecorator, "the hadMuDiTauWPDecorator");
  declareProperty( "jetConstituentModSequence", m_jetConstituentModSequence, "the jetConstituentModSequence");
  declareProperty( "asgElectronVeryLooseLikelihoodTool", m_checkEleVeryLooseLH, "the very loose AsgElectronLikelihoodTool");
  declareProperty( "asgElectronLooseLikelihoodTool", m_checkEleLooseLH, "the loose AsgElectronLikelihoodTool");
  declareProperty( "asgElectronMediumLikelihoodTool", m_checkEleMediumLH, "the loose AsgElectronLikelihoodTool");
  declareProperty( "asgElectronTightLikelihoodTool", m_checkEleTightLH, "the loose AsgElectronLikelihoodTool");
  declareProperty( "bTaggingSelectionTool", m_bTaggingSelectionTool, "the BTaggingSelectionTool");
  declareProperty( "missingMassTool", m_missingMassTool, "the MissingMassCalculatorTool");

  gErrorIgnoreLevel = kError;
}

DiTauAnalysis :: ~DiTauAnalysis () {
  delete m_diTauPt;
  delete m_diTauEta;
  delete m_diTauPhi;
  delete m_nDiTauSubjets;

  delete m_hadElDiTauPt;
  delete m_hadElDiTauEta;
  delete m_hadElDiTauPhi;
  delete m_hadElDiTauM;
  delete m_hadElDiTauBDTScore;
  delete m_hadElDiTauFlatBDTScore;
  delete m_hadElDiTauTruthMatchType;
  delete m_hadElTauPt;
  delete m_hadElTauEta;
  delete m_hadElTauPhi;
  delete m_hadElTauE;
  delete m_hadElElectronPt;
  delete m_hadElElectronEta;
  delete m_hadElElectronPhi;
  delete m_hadElElectronE;
  delete m_hadElDiTaudR;

  delete m_hadMuDiTauPt;
  delete m_hadMuDiTauEta;
  delete m_hadMuDiTauPhi;
  delete m_hadMuDiTauM;
  delete m_hadMuDiTauBDTScore;
  delete m_hadMuDiTauFlatBDTScore;
  delete m_hadMuDiTauTruthMatchType;
  delete m_hadMuTauPt;
  delete m_hadMuTauEta;
  delete m_hadMuTauPhi;
  delete m_hadMuTauE;
  delete m_hadMuMuonPt;
  delete m_hadMuMuonEta;
  delete m_hadMuMuonPhi;
  delete m_hadMuMuonE;
  delete m_hadMuDiTaudR;

  delete m_tauPt;
  delete m_tauEta;
  delete m_tauPhi;
  delete m_tauE;
  //delete m_tauP4;
  delete m_tauTightPt;
  delete m_tauTightEta;
  delete m_tauTightPhi;
  delete m_tauTightE;
  delete m_tauMediumPt;
  delete m_tauMediumEta;
  delete m_tauMediumPhi;
  delete m_tauMediumE;
  delete m_tauLoosePt;
  delete m_tauLooseEta;
  delete m_tauLoosePhi;
  delete m_tauLooseE;
  delete m_tauVeryLoosePt;
  delete m_tauVeryLooseEta;
  delete m_tauVeryLoosePhi;
  delete m_tauVeryLooseE;

  delete m_muPt;
  delete m_muEta;
  delete m_muPhi;
  delete m_muE;
  delete m_muTightPt;
  delete m_muTightEta;
  delete m_muTightPhi;
  delete m_muTightE;
  delete m_muMediumPt;
  delete m_muMediumEta;
  delete m_muMediumPhi;
  delete m_muMediumE;
  delete m_muLoosePt;
  delete m_muLooseEta;
  delete m_muLoosePhi;
  delete m_muLooseE;

  delete m_elePt;
  delete m_eleEta;
  delete m_elePhi;
  delete m_eleE;
  delete m_eleTightPt;
  delete m_eleTightEta;
  delete m_eleTightPhi;
  delete m_eleTightE;
  delete m_eleMediumPt;
  delete m_eleMediumEta;
  delete m_eleMediumPhi;
  delete m_eleMediumE;
  delete m_eleLoosePt;
  delete m_eleLooseEta;
  delete m_eleLoosePhi;
  delete m_eleLooseE;
  delete m_eleVeryLoosePt;
  delete m_eleVeryLooseEta;
  delete m_eleVeryLoosePhi;
  delete m_eleVeryLooseE;
}

StatusCode DiTauAnalysis :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_MSG_INFO("in initialize");

  m_mytree = new TTree("mytree", "mytree");
  CHECK( histSvc()->regTree("/ANALYSIS/mytree", m_mytree) );

  m_mytree->Branch("RunNumber", &m_runNumber);
  m_mytree->Branch("EventNumber", &m_eventNumber);

  if(m_isSignal)
  {
    m_mytree->Branch("PDGTruthMatchType", &m_pdgTruthMatchType);

    m_mytree->Branch("TruthHiggsPt", &m_truthHiggsPt);
    m_mytree->Branch("TruthHiggsEta", &m_truthHiggsEta);
    m_mytree->Branch("TruthHiggsPhi", &m_truthHiggsPhi);
    m_mytree->Branch("TruthHiggsE", &m_truthHiggsE);

    m_mytree->Branch("TruthHadTauPt", &m_truthHadTauPt);
    m_mytree->Branch("TruthHadTauEta", &m_truthHadTauEta);
    m_mytree->Branch("TruthHadTauPhi", &m_truthHadTauPhi);
    m_mytree->Branch("TruthHadTauE", &m_truthHadTauE);
    m_mytree->Branch("TruthHadTauPdgId", &m_truthHadTauPdgId);

    m_mytree->Branch("TruthHadTauVisPt", &m_truthHadTauVisPt);
    m_mytree->Branch("TruthHadTauVisEta", &m_truthHadTauVisEta);
    m_mytree->Branch("TruthHadTauVisPhi", &m_truthHadTauVisPhi);
    m_mytree->Branch("TruthHadTauVisM", &m_truthHadTauVisM);

    m_mytree->Branch("TruthLepTauPt", &m_truthLepTauPt);
    m_mytree->Branch("TruthLepTauEta", &m_truthLepTauEta);
    m_mytree->Branch("TruthLepTauPhi", &m_truthLepTauPhi);
    m_mytree->Branch("TruthLepTauE", &m_truthLepTauE);
    m_mytree->Branch("TruthLepTauPdgId", &m_truthLepTauPdgId);

    m_mytree->Branch("TruthFinalLeptonPt", &m_truthFinalLeptonPt);
    m_mytree->Branch("TruthFinalLeptonEta", &m_truthFinalLeptonEta);
    m_mytree->Branch("TruthFinalLeptonPhi", &m_truthFinalLeptonPhi);
    m_mytree->Branch("TruthFinalLeptonE", &m_truthFinalLeptonE);
    m_mytree->Branch("TruthFinalLeptonPdgId", &m_truthFinalLeptonPdgId);

    m_mytree->Branch("TruthHadTauVisFinalLeptondR", &m_truthHadTauVisFinalLeptondR);

    m_mytree->Branch("TruthDiTauVisPt", &m_truthDiTauVisPt);
    m_mytree->Branch("TruthDiTauVisEta", &m_truthDiTauVisEta);
    m_mytree->Branch("TruthDiTauVisPhi", &m_truthDiTauVisPhi);
    m_mytree->Branch("TruthDiTauVisE", &m_truthDiTauVisE);
  }

  m_mytree->Branch("NDiTau", &m_nDiTaus);
  m_diTauPt = new std::vector<float>();
  m_mytree->Branch("DiTauPt", &m_diTauPt);
  m_diTauEta = new std::vector<float>();
  m_mytree->Branch("DiTauEta", &m_diTauEta);
  m_diTauPhi = new std::vector<float>();
  m_mytree->Branch("DiTauPhi", &m_diTauPhi);
  m_nDiTauSubjets = new std::vector<unsigned int>();
  m_mytree->Branch("NDiTauSubjets", &m_nDiTauSubjets);

  // HadEl Ditaus
  m_mytree->Branch("NHadElDiTau", &m_nHadElDiTaus);
  m_hadElDiTauPt = new std::vector<float>();
  m_mytree->Branch("HadElDiTauPt", &m_hadElDiTauPt);
  m_hadElDiTauEta = new std::vector<float>();
  m_mytree->Branch("HadElDiTauEta", &m_hadElDiTauEta);
  m_hadElDiTauPhi = new std::vector<float>();
  m_mytree->Branch("HadElDiTauPhi", &m_hadElDiTauPhi);
  m_hadElDiTauM = new std::vector<float>();
  m_mytree->Branch("HadElDiTauM", &m_hadElDiTauM);
  m_hadElDiTauBDTScore = new std::vector<float>();
  m_mytree->Branch("HadElDiTauBDTScore", &m_hadElDiTauBDTScore);
  m_hadElDiTauFlatBDTScore = new std::vector<float>();
  m_mytree->Branch("BestHadElDiTauBDTScore", &m_bestHadElDiTauBDTScore);
  m_mytree->Branch("HadElDiTauFlatBDTScore", &m_hadElDiTauFlatBDTScore);
  m_hadElDiTauTruthMatchType = new std::vector<unsigned int>();
  m_mytree->Branch("HadElDiTauTruthMatchType", &m_hadElDiTauTruthMatchType);
  m_mytree->Branch("NHadElTau", &m_nHadElTaus);
  m_hadElTauPt = new std::vector<float>();
  m_mytree->Branch("HadElTauPt", &m_hadElTauPt);
  m_hadElTauEta = new std::vector<float>();
  m_mytree->Branch("HadElTauEta", &m_hadElTauEta);
  m_hadElTauPhi = new std::vector<float>();
  m_mytree->Branch("HadElTauPhi", &m_hadElTauPhi);
  m_hadElTauE = new std::vector<float>();
  m_mytree->Branch("HadElTauE", &m_hadElTauE);
  m_mytree->Branch("NHadElElectron", &m_nHadElElectrons);
  m_hadElElectronPt = new std::vector<float>();
  m_mytree->Branch("HadElElectronPt", &m_hadElElectronPt);
  m_hadElElectronEta = new std::vector<float>();
  m_mytree->Branch("HadElElectronEta", &m_hadElElectronEta);
  m_hadElElectronPhi = new std::vector<float>();
  m_mytree->Branch("HadElElectronPhi", &m_hadElElectronPhi);
  m_hadElElectronE = new std::vector<float>();
  m_mytree->Branch("HadElElectronE", &m_hadElElectronE);
  m_hadElDiTaudR = new std::vector<float>();
  m_mytree->Branch("HadElDiTaudR", &m_hadElDiTaudR);

  // HadMu DiTaus
  m_mytree->Branch("NHadMuDiTau", &m_nHadMuDiTaus);
  m_hadMuDiTauPt = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauPt", &m_hadMuDiTauPt);
  m_hadMuDiTauEta = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauEta", &m_hadMuDiTauEta);
  m_hadMuDiTauPhi = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauPhi", &m_hadMuDiTauPhi);
  m_hadMuDiTauM = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauM", &m_hadMuDiTauM);
  m_hadMuDiTauBDTScore = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauBDTScore", &m_hadMuDiTauBDTScore);
  m_hadMuDiTauFlatBDTScore = new std::vector<float>();
  m_mytree->Branch("BestHadMuDiTauBDTScore", &m_bestHadMuDiTauBDTScore);
  m_mytree->Branch("HadMuDiTauFlatBDTScore", &m_hadMuDiTauFlatBDTScore);
  m_hadMuDiTauTruthMatchType = new std::vector<unsigned int>();
  m_mytree->Branch("HadMuDiTauTruthMatchType", &m_hadMuDiTauTruthMatchType);
  m_mytree->Branch("NHadMuTau", &m_nHadMuTaus);
  m_hadMuTauPt = new std::vector<float>();
  m_mytree->Branch("HadMuTauPt", &m_hadMuTauPt);
  m_hadMuTauEta = new std::vector<float>();
  m_mytree->Branch("HadMuTauEta", &m_hadMuTauEta);
  m_hadMuTauPhi = new std::vector<float>();
  m_mytree->Branch("HadMuTauPhi", &m_hadMuTauPhi);
  m_hadMuTauE = new std::vector<float>();
  m_mytree->Branch("HadMuTauE", &m_hadMuTauE);
  m_mytree->Branch("NHadMuMuon", &m_nHadMuMuons);
  m_hadMuMuonPt = new std::vector<float>();
  m_mytree->Branch("HadMuMuonPt", &m_hadMuMuonPt);
  m_hadMuMuonEta = new std::vector<float>();
  m_mytree->Branch("HadMuMuonEta", &m_hadMuMuonEta);
  m_hadMuMuonPhi = new std::vector<float>();
  m_mytree->Branch("HadMuMuonPhi", &m_hadMuMuonPhi);
  m_hadMuMuonE = new std::vector<float>();
  m_mytree->Branch("HadMuMuonE", &m_hadMuMuonE);
  m_hadMuDiTaudR = new std::vector<float>();
  m_mytree->Branch("HadMuDiTaudR", &m_hadMuDiTaudR);

  // Taus
  m_mytree->Branch("NTau", &m_nTaus);
  m_tauPt = new std::vector<float>();
  m_mytree->Branch("TauPt", &m_tauPt);
  m_tauEta = new std::vector<float>();
  m_mytree->Branch("TauEta", &m_tauEta);
  m_tauPhi = new std::vector<float>();
  m_mytree->Branch("TauPhi", &m_tauPhi);
  m_tauE = new std::vector<float>();
  m_mytree->Branch("TauE", &m_tauE);
  //m_tauP4 = new std::vector<TLorentzVector>();
  //m_mytree->Branch("TauP4", &m_tauP4);
  m_mytree->Branch("LeadingTauPt", &m_leadingTauPt);
  m_mytree->Branch("LeadingTauEta", &m_leadingTauEta);
  m_mytree->Branch("LeadingTauPhi", &m_leadingTauPhi);
  m_mytree->Branch("LeadingTauE", &m_leadingTauE);

  m_mytree->Branch("BestTauPt", &m_bestTauPt);
  m_mytree->Branch("BestTauEta", &m_bestTauEta);
  m_mytree->Branch("BestTauPhi", &m_bestTauPhi);
  m_mytree->Branch("BestTauE", &m_bestTauE);

  m_mytree->Branch("TauRecoTruthMindR", &m_tauRecoTruthMindR);

  // Very Loose taus
  m_mytree->Branch("NTauVeryLoose", &m_nTausVeryLoose);
  m_tauVeryLoosePt = new std::vector<float>();
  m_mytree->Branch("TauVeryLoosePt", &m_tauVeryLoosePt);
  m_tauVeryLooseEta = new std::vector<float>();
  m_mytree->Branch("TauVeryLooseEta", &m_tauVeryLooseEta);
  m_tauVeryLoosePhi = new std::vector<float>();
  m_mytree->Branch("TauVeryLoosePhi", &m_tauVeryLoosePhi);
  m_tauVeryLooseE = new std::vector<float>();
  m_mytree->Branch("TauVeryLooseE", &m_tauVeryLooseE);
  m_mytree->Branch("LeadingTauVeryLoosePt", &m_leadingTauVeryLoosePt);
  m_mytree->Branch("LeadingTauVeryLooseEta", &m_leadingTauVeryLooseEta);
  m_mytree->Branch("LeadingTauVeryLoosePhi", &m_leadingTauVeryLoosePhi);
  m_mytree->Branch("LeadingTauVeryLooseE", &m_leadingTauVeryLooseE);

  // Loose taus
  m_mytree->Branch("NTauLoose", &m_nTausLoose);
  m_tauLoosePt = new std::vector<float>();
  m_mytree->Branch("TauLoosePt", &m_tauLoosePt);
  m_tauLooseEta = new std::vector<float>();
  m_mytree->Branch("TauLooseEta", &m_tauLooseEta);
  m_tauLoosePhi = new std::vector<float>();
  m_mytree->Branch("TauLoosePhi", &m_tauLoosePhi);
  m_tauLooseE = new std::vector<float>();
  m_mytree->Branch("TauLooseE", &m_tauLooseE);
  m_mytree->Branch("LeadingTauLoosePt", &m_leadingTauLoosePt);
  m_mytree->Branch("LeadingTauLooseEta", &m_leadingTauLooseEta);
  m_mytree->Branch("LeadingTauLoosePhi", &m_leadingTauLoosePhi);
  m_mytree->Branch("LeadingTauLooseE", &m_leadingTauLooseE);

  // Medium taus
  m_mytree->Branch("NTauMedium", &m_nTausMedium);
  m_tauMediumPt = new std::vector<float>();
  m_mytree->Branch("TauMediumPt", &m_tauMediumPt);
  m_tauMediumEta = new std::vector<float>();
  m_mytree->Branch("TauMediumEta", &m_tauMediumEta);
  m_tauMediumPhi = new std::vector<float>();
  m_mytree->Branch("TauMediumPhi", &m_tauMediumPhi);
  m_tauMediumE = new std::vector<float>();
  m_mytree->Branch("TauMediumE", &m_tauMediumE);
  m_mytree->Branch("LeadingTauMediumPt", &m_leadingTauMediumPt);
  m_mytree->Branch("LeadingTauMediumEta", &m_leadingTauMediumEta);
  m_mytree->Branch("LeadingTauMediumPhi", &m_leadingTauMediumPhi);
  m_mytree->Branch("LeadingTauMediumE", &m_leadingTauMediumE);

  // Tight taus
  m_mytree->Branch("NTauTight", &m_nTausTight);
  m_tauTightPt = new std::vector<float>();
  m_mytree->Branch("TauTightPt", &m_tauTightPt);
  m_tauTightEta = new std::vector<float>();
  m_mytree->Branch("TauTightEta", &m_tauTightEta);
  m_tauTightPhi = new std::vector<float>();
  m_mytree->Branch("TauTightPhi", &m_tauTightPhi);
  m_tauTightE = new std::vector<float>();
  m_mytree->Branch("TauTightE", &m_tauTightE);
  m_mytree->Branch("LeadingTauTightPt", &m_leadingTauTightPt);
  m_mytree->Branch("LeadingTauTightEta", &m_leadingTauTightEta);
  m_mytree->Branch("LeadingTauTightPhi", &m_leadingTauTightPhi);
  m_mytree->Branch("LeadingTauTightE", &m_leadingTauTightE);

  // Muons
  m_mytree->Branch("NMu", &m_nMuons);
  m_muPt = new std::vector<float>();
  m_mytree->Branch("MuPt", &m_muPt);
  m_muEta = new std::vector<float>();
  m_mytree->Branch("MuEta", &m_muEta);
  m_muPhi = new std::vector<float>();
  m_mytree->Branch("MuPhi", &m_muPhi);
  m_muE = new std::vector<float>();
  m_mytree->Branch("MuE", &m_muE);
  m_mytree->Branch("LeadingMuPt", &m_leadingMuPt);
  m_mytree->Branch("LeadingMuEta", &m_leadingMuEta);
  m_mytree->Branch("LeadingMuPhi", &m_leadingMuPhi);
  m_mytree->Branch("LeadingMuE", &m_leadingMuE);

  m_mytree->Branch("MuRecoTruthMindR", &m_muRecoTruthMindR);

  // Loose muons
  m_mytree->Branch("NMuLoose", &m_nMuonsLoose);
  m_muLoosePt = new std::vector<float>();
  m_mytree->Branch("MuLoosePt", &m_muLoosePt);
  m_muLooseEta = new std::vector<float>();
  m_mytree->Branch("MuLooseEta", &m_muLooseEta);
  m_muLoosePhi = new std::vector<float>();
  m_mytree->Branch("MuLoosePhi", &m_muLoosePhi);
  m_muLooseE = new std::vector<float>();
  m_mytree->Branch("MuLooseE", &m_muLooseE);
  m_mytree->Branch("LeadingMuLoosePt", &m_leadingMuLoosePt);
  m_mytree->Branch("LeadingMuLooseEta", &m_leadingMuLooseEta);
  m_mytree->Branch("LeadingMuLoosePhi", &m_leadingMuLoosePhi);
  m_mytree->Branch("LeadingMuLooseE", &m_leadingMuLooseE);

  // Medium muons
  m_mytree->Branch("NMuMedium", &m_nMuonsMedium);
  m_muMediumPt = new std::vector<float>();
  m_mytree->Branch("MuMediumPt", &m_muMediumPt);
  m_muMediumEta = new std::vector<float>();
  m_mytree->Branch("MuMediumEta", &m_muMediumEta);
  m_muMediumPhi = new std::vector<float>();
  m_mytree->Branch("MuMediumPhi", &m_muMediumPhi);
  m_muMediumE = new std::vector<float>();
  m_mytree->Branch("MuMediumE", &m_muMediumE);
  m_mytree->Branch("LeadingMuMediumPt", &m_leadingMuMediumPt);
  m_mytree->Branch("LeadingMuMediumEta", &m_leadingMuMediumEta);
  m_mytree->Branch("LeadingMuMediumPhi", &m_leadingMuMediumPhi);
  m_mytree->Branch("LeadingMuMediumE", &m_leadingMuMediumE);

  // Tight muons
  m_mytree->Branch("NMuTight", &m_nMuonsTight);
  m_muTightPt = new std::vector<float>();
  m_mytree->Branch("MuTightPt", &m_muTightPt);
  m_muTightEta = new std::vector<float>();
  m_mytree->Branch("MuTightEta", &m_muTightEta);
  m_muTightPhi = new std::vector<float>();
  m_mytree->Branch("MuTightPhi", &m_muTightPhi);
  m_muTightE = new std::vector<float>();
  m_mytree->Branch("MuTightE", &m_muTightE);
  m_mytree->Branch("LeadingMuTightPt", &m_leadingMuTightPt);
  m_mytree->Branch("LeadingMuTightEta", &m_leadingMuTightEta);
  m_mytree->Branch("LeadingMuTightPhi", &m_leadingMuTightPhi);
  m_mytree->Branch("LeadingMuTightE", &m_leadingMuTightE);

  // Electrons
  m_mytree->Branch("NEle", &m_nElectrons);
  m_elePt = new std::vector<float>();
  m_mytree->Branch("ElePt", &m_elePt);
  m_eleEta = new std::vector<float>();
  m_mytree->Branch("EleEta", &m_eleEta);
  m_elePhi = new std::vector<float>();
  m_mytree->Branch("ElePhi", &m_elePhi);
  m_eleE = new std::vector<float>();
  m_mytree->Branch("EleE", &m_eleE);
  m_mytree->Branch("LeadingElePt", &m_leadingElePt);
  m_mytree->Branch("LeadingEleEta", &m_leadingEleEta);
  m_mytree->Branch("LeadingElePhi", &m_leadingElePhi);
  m_mytree->Branch("LeadingEleE", &m_leadingEleE);

  m_mytree->Branch("BestElePt", &m_bestElePt);
  m_mytree->Branch("BestEleEta", &m_bestEleEta);
  m_mytree->Branch("BestElePhi", &m_bestElePhi);
  m_mytree->Branch("BestEleE", &m_bestEleE);

  m_mytree->Branch("EleRecoTruthMindR", &m_eleRecoTruthMindR);

  // VeryLoose electrons
  m_mytree->Branch("NEleVeryLoose", &m_nElectronsVeryLoose);
  m_eleVeryLoosePt = new std::vector<float>();
  m_mytree->Branch("EleVeryLoosePt", &m_eleVeryLoosePt);
  m_eleVeryLooseEta = new std::vector<float>();
  m_mytree->Branch("EleVeryLooseEta", &m_eleVeryLooseEta);
  m_eleVeryLoosePhi = new std::vector<float>();
  m_mytree->Branch("EleVeryLoosePhi", &m_eleVeryLoosePhi);
  m_eleVeryLooseE = new std::vector<float>();
  m_mytree->Branch("EleVeryLooseE", &m_eleVeryLooseE);
  m_mytree->Branch("LeadingEleVeryLoosePt", &m_leadingEleVeryLoosePt);
  m_mytree->Branch("LeadingEleVeryLooseEta", &m_leadingEleVeryLooseEta);
  m_mytree->Branch("LeadingEleVeryLoosePhi", &m_leadingEleVeryLoosePhi);
  m_mytree->Branch("LeadingEleVeryLooseE", &m_leadingEleVeryLooseE);

  // Loose electrons
  m_mytree->Branch("NEleLoose", &m_nElectronsLoose);
  m_eleLoosePt = new std::vector<float>();
  m_mytree->Branch("EleLoosePt", &m_eleLoosePt);
  m_eleLooseEta = new std::vector<float>();
  m_mytree->Branch("EleLooseEta", &m_eleLooseEta);
  m_eleLoosePhi = new std::vector<float>();
  m_mytree->Branch("EleLoosePhi", &m_eleLoosePhi);
  m_eleLooseE = new std::vector<float>();
  m_mytree->Branch("EleLooseE", &m_eleLooseE);
  m_mytree->Branch("LeadingEleLoosePt", &m_leadingEleLoosePt);
  m_mytree->Branch("LeadingEleLooseEta", &m_leadingEleLooseEta);
  m_mytree->Branch("LeadingEleLoosePhi", &m_leadingEleLoosePhi);
  m_mytree->Branch("LeadingEleLooseE", &m_leadingEleLooseE);

  // Medium electrons
  m_mytree->Branch("NEleMedium", &m_nElectronsMedium);
  m_eleMediumPt = new std::vector<float>();
  m_mytree->Branch("EleMediumPt", &m_eleMediumPt);
  m_eleMediumEta = new std::vector<float>();
  m_mytree->Branch("EleMediumEta", &m_eleMediumEta);
  m_eleMediumPhi = new std::vector<float>();
  m_mytree->Branch("EleMediumPhi", &m_eleMediumPhi);
  m_eleMediumE = new std::vector<float>();
  m_mytree->Branch("EleMediumE", &m_eleMediumE);
  m_mytree->Branch("LeadingEleMediumPt", &m_leadingEleMediumPt);
  m_mytree->Branch("LeadingEleMediumEta", &m_leadingEleMediumEta);
  m_mytree->Branch("LeadingEleMediumPhi", &m_leadingEleMediumPhi);
  m_mytree->Branch("LeadingEleMediumE", &m_leadingEleMediumE);

  // Tight electrons
  m_mytree->Branch("NEleTight", &m_nElectronsTight);
  m_eleTightPt = new std::vector<float>();
  m_mytree->Branch("EleTightPt", &m_eleTightPt);
  m_eleTightEta = new std::vector<float>();
  m_mytree->Branch("EleTightEta", &m_eleTightEta);
  m_eleTightPhi = new std::vector<float>();
  m_mytree->Branch("EleTightPhi", &m_eleTightPhi);
  m_eleTightE = new std::vector<float>();
  m_mytree->Branch("EleTightE", &m_eleTightE);
  m_mytree->Branch("LeadingEleTightPt", &m_leadingEleTightPt);
  m_mytree->Branch("LeadingEleTightEta", &m_leadingEleTightEta);
  m_mytree->Branch("LeadingEleTightPhi", &m_leadingEleTightPhi);
  m_mytree->Branch("LeadingEleTightE", &m_leadingEleTightE);

  // Tau-Ele ditau system
  m_tauEleLoosePt = new std::vector<float>();
  m_mytree->Branch("TauEleLoosePt", &m_tauEleLoosePt);
  m_tauEleLooseEta = new std::vector<float>();
  m_mytree->Branch("TauEleLooseEta", &m_tauEleLooseEta);
  m_tauEleLoosePhi = new std::vector<float>();
  m_mytree->Branch("TauEleLoosePhi", &m_tauEleLoosePhi);
  m_tauEleLooseE = new std::vector<float>();
  m_mytree->Branch("TauEleLooseE", &m_tauEleLooseE);
  m_tauEleLooseM = new std::vector<float>();
  m_mytree->Branch("TauEleLooseM", &m_tauEleLooseM);
  m_tauEleLoosedR = new std::vector<float>();
  m_mytree->Branch("TauEleLoosedR", &m_tauEleLoosedR);

  // Chosen Tau-Ele system
  m_mytree->Branch("ChosenTauPt", &m_chosenTauPt);
  m_mytree->Branch("ChosenTauEta", &m_chosenTauEta);
  m_mytree->Branch("ChosenTauPhi", &m_chosenTauPhi);
  m_mytree->Branch("ChosenTauE", &m_chosenTauE);
  m_mytree->Branch("ChosenElePt", &m_chosenElePt);
  m_mytree->Branch("ChosenEleEta", &m_chosenEleEta);
  m_mytree->Branch("ChosenElePhi", &m_chosenElePhi);
  m_mytree->Branch("ChosenEleE", &m_chosenEleE);
  m_mytree->Branch("ChosenTauElePt", &m_chosenTauElePt);
  m_mytree->Branch("ChosenTauEleEta", &m_chosenTauEleEta);
  m_mytree->Branch("ChosenTauElePhi", &m_chosenTauElePhi);
  m_mytree->Branch("ChosenTauEleE", &m_chosenTauEleE);
  m_mytree->Branch("ChosenTauEleM", &m_chosenTauEleM);
  m_mytree->Branch("ChosenTauEledR", &m_chosenTauEledR);

  // Resolved analysis values
  m_mytree->Branch("ResolvedMissingMass", &m_resolvedMissingMass);

  // Jets
  m_mytree->Branch("NJet", &m_nJets);
  m_jetPt = new std::vector<float>();
  m_mytree->Branch("JetPt", &m_jetPt);
  m_jetEta = new std::vector<float>();
  m_mytree->Branch("JetEta", &m_jetEta);
  m_jetPhi = new std::vector<float>();
  m_mytree->Branch("JetPhi", &m_jetPhi);
  m_jetE = new std::vector<float>();
  m_mytree->Branch("JetE", &m_jetE);
  m_jetBTagAccept = new std::vector<float>();
  m_mytree->Branch("LeadingJetPt", &m_leadingJetPt);
  m_mytree->Branch("LeadingJetEta", &m_leadingJetEta);
  m_mytree->Branch("LeadingJetPhi", &m_leadingJetPhi);
  m_mytree->Branch("LeadingJetE", &m_leadingJetE);
  m_mytree->Branch("SubleadingJetPt", &m_subleadingJetPt);
  m_mytree->Branch("SubleadingJetEta", &m_subleadingJetEta);
  m_mytree->Branch("SubleadingJetPhi", &m_subleadingJetPhi);
  m_mytree->Branch("SubleadingJetE", &m_subleadingJetE);
  m_mytree->Branch("JetBTagAccept", &m_jetBTagAccept);

  // B-tagged Jets
  m_mytree->Branch("NBTagJet", &m_nBTagJets);
  m_bTagJetPt = new std::vector<float>();
  m_mytree->Branch("BTagJetPt", &m_bTagJetPt);
  m_bTagJetEta = new std::vector<float>();
  m_mytree->Branch("BTagJetEta", &m_bTagJetEta);
  m_bTagJetPhi = new std::vector<float>();
  m_mytree->Branch("BTagJetPhi", &m_bTagJetPhi);
  m_bTagJetE = new std::vector<float>();
  m_mytree->Branch("BTagJetE", &m_bTagJetE);
  m_mytree->Branch("LeadingBTagJetPt", &m_leadingBTagJetPt);
  m_mytree->Branch("LeadingBTagJetEta", &m_leadingBTagJetEta);
  m_mytree->Branch("LeadingBTagJetPhi", &m_leadingBTagJetPhi);
  m_mytree->Branch("LeadingBTagJetE", &m_leadingBTagJetE);
  m_mytree->Branch("SubleadingBTagJetPt", &m_subleadingBTagJetPt);
  m_mytree->Branch("SubleadingBTagJetEta", &m_subleadingBTagJetEta);
  m_mytree->Branch("SubleadingBTagJetPhi", &m_subleadingBTagJetPhi);
  m_mytree->Branch("SubleadingBTagJetE", &m_subleadingBTagJetE);

  // Large-R jets
  m_mytree->Branch("NLargeRJet", &m_nLargeRJets);
  m_lRJetPt = new std::vector<float>();
  m_mytree->Branch("LargeRJetPt", &m_lRJetPt);
  m_lRJetEta = new std::vector<float>();
  m_mytree->Branch("LargeRJetEta", &m_lRJetEta);
  m_lRJetPhi = new std::vector<float>();
  m_mytree->Branch("LargeRJetPhi", &m_lRJetPhi);
  m_lRJetE = new std::vector<float>();
  m_mytree->Branch("LargeRJetE", &m_lRJetE);
  m_lRJetM = new std::vector<float>();
  m_mytree->Branch("LargeRJetM", &m_lRJetM);
  m_mytree->Branch("LeadingLargeRJetPt", &m_leadingLRJetPt);
  m_mytree->Branch("LeadingLargeRJetEta", &m_leadingLRJetEta);
  m_mytree->Branch("LeadingLargeRJetPhi", &m_leadingLRJetPhi);
  m_mytree->Branch("LeadingLargeRJetE", &m_leadingLRJetE);
  m_mytree->Branch("LeadingLargeRJetM", &m_leadingLRJetM);
  m_mytree->Branch("SubleadingLargeRJetPt", &m_subleadingLRJetPt);
  m_mytree->Branch("SubleadingLargeRJetEta", &m_subleadingLRJetEta);
  m_mytree->Branch("SubleadingLargeRJetPhi", &m_subleadingLRJetPhi);
  m_mytree->Branch("SubleadingLargeRJetE", &m_subleadingLRJetE);
  m_mytree->Branch("SubleadingLargeRJetM", &m_subleadingLRJetM);

  m_mytree->Branch("LargeRJetdR", &m_largeRJetdR);
  m_mytree->Branch("LargeRJetdEta", &m_largeRJetdEta);
  m_mytree->Branch("LargeRJetdPhi", &m_largeRJetdPhi);
  m_mytree->Branch("LargeRJetdPt", &m_largeRJetdPt);

  // Tau large-R jet, jet containing taus
  m_mytree->Branch("TauLargeRJetPt", &m_tauLRJetPt);
  m_mytree->Branch("TauLargeRJetEta", &m_tauLRJetEta);
  m_mytree->Branch("TauLargeRJetPhi", &m_tauLRJetPhi);
  m_mytree->Branch("TauLargeRJetE", &m_tauLRJetE);
  m_mytree->Branch("TauLargeRJetM", &m_tauLRJetM);
  m_mytree->Branch("TauLargeRJetEleLoosedR", &m_tauLRJetEleLoosedR);

  // b large-R jet, jet containing bs
  m_mytree->Branch("BLargeRJetPt", &m_bLRJetPt);
  m_mytree->Branch("BLargeRJetEta", &m_bLRJetEta);
  m_mytree->Branch("BLargeRJetPhi", &m_bLRJetPhi);
  m_mytree->Branch("BLargeRJetE", &m_bLRJetE);
  m_mytree->Branch("BLargeRJetM", &m_bLRJetM);
  m_mytree->Branch("BLargeRJetEleLoosedR", &m_bLRJetEleLoosedR);

  // Large-R Higgs
  m_mytree->Branch("LargeRXPt", &m_largeRXPt);
  m_mytree->Branch("LargeRXEta", &m_largeRXEta);
  m_mytree->Branch("LargeRXPhi", &m_largeRXPhi);
  m_mytree->Branch("LargeRXE", &m_largeRXE);
  m_mytree->Branch("LargeRXM", &m_largeRXM);

  // Builders
  m_hadElBuilder = new DiTauRec::HadElBuilder("HadElBuilder");
  CHECK(m_hadElBuilder->initialize());

  m_hadMuBuilder = new DiTauRec::HadMuBuilder("HadMuBuilder");
  CHECK(m_hadMuBuilder->initialize());

  // JetConstituentModSequence
  ANA_CHECK(m_jetConstituentModSequence.retrieve());

  //const xAOD::CaloClusterContainer* lc_container = nullptr;
  //ANA_CHECK( evtStore()->retrieve( lc_container, "LCOriginTopoClusters" ) );

  //Override the event store hash for the LCOriginTopoClusters
  //m_ef = const_cast< xAOD::EventFormat* >( xEvent.inputEventFormat() );
  //m_ef->add( xAOD::EventFormatElement( "LCOriginTopoClusters", "xAOD::CaloClusterContainer", "", 0x1bccf189 ) );

  // DiTauTruthMatchingTool
  ANA_CHECK(m_diTauTruthMatchingTool.retrieve());

  // DiTauIDVarCalculators
  ANA_CHECK(m_hadElDiTauIDVarCalculator.retrieve());
  ANA_CHECK(m_hadMuDiTauIDVarCalculator.retrieve());

  // DiTauDiscriminantTools
  ANA_CHECK(m_hadElDiTauDiscrTool.retrieve());
  ANA_CHECK(m_hadMuDiTauDiscrTool.retrieve());

  // DiTauWPDecorators
  ANA_CHECK(m_hadElDiTauWPDecorator.retrieve());
  ANA_CHECK(m_hadMuDiTauWPDecorator.retrieve());

  // Electron likelihood tools
  ANA_CHECK(m_checkEleLooseLH.retrieve());
  ANA_CHECK(m_checkEleMediumLH.retrieve());
  ANA_CHECK(m_checkEleTightLH.retrieve());

  // B-tagging
  ANA_CHECK(m_bTaggingSelectionTool.retrieve());

  // MMC
  ANA_CHECK(m_missingMassTool.retrieve());

  return StatusCode::SUCCESS;
}



StatusCode DiTauAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  ANA_MSG_INFO("in execute");

  m_diTauTruthMatchingTool->setNewEvent();
  m_diTauTruthMatchingTool->retrieveTruthTaus();

  const xAOD::EventInfo* eventInfo = nullptr;
  ANA_CHECK( evtStore()->retrieve( eventInfo, "EventInfo" ) );

  const xAOD::TruthParticleContainer* truthParticles = nullptr;
  ANA_CHECK( evtStore()->retrieve( truthParticles, "TruthParticles" ) );

  const xAOD::TruthParticleContainer* truthTaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( truthTaus, "TruthTaus" ) );

  const xAOD::ElectronContainer* electrons = nullptr;
  ANA_CHECK( evtStore()->retrieve( electrons, "Electrons" ) );

  const xAOD::MuonContainer* muons = nullptr;
  ANA_CHECK( evtStore()->retrieve( muons, "Muons" ) );

  const xAOD::TauJetContainer* taus = nullptr;
  ANA_CHECK( evtStore()->retrieve( taus, "TauJets" ) );

  const xAOD::JetContainer* jets = nullptr;
  ANA_CHECK( evtStore()->retrieve( jets, "AntiKt4EMTopoJets" ) );

  const xAOD::JetContainer* largerjets = nullptr;
  ANA_CHECK( evtStore()->retrieve( largerjets, "HLT_xAOD__JetContainer_a10r_tcemsubjesISFS" ) );

  const xAOD::DiTauJetContainer* ditaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( ditaus, "DiTauJets" ) );

  ANA_CHECK( m_hadElBuilder->execute() );
  const xAOD::DiTauJetContainer* hadelditaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( hadelditaus, "DiTauJetsHadEl" ) );

  ANA_CHECK( m_hadMuBuilder->execute() );
  const xAOD::DiTauJetContainer* hadmuditaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( hadmuditaus, "DiTauJetsHadMu" ) );

  const xAOD::MissingETContainer* mets = nullptr;
  ANA_CHECK( evtStore()->retrieve( mets, "MET_Reference_AntiKt4EMTopo" ) );

  m_runNumber = 0;
  m_eventNumber = 0;

  m_pdgTruthMatchType = 0;

  m_truthHiggsPt = 0;
  m_truthHiggsEta = 0;
  m_truthHiggsPhi = 0;
  m_truthHiggsE = 0;

  m_truthHadTauPt = 0;
  m_truthHadTauEta = 0;
  m_truthHadTauPhi = 0;
  m_truthHadTauE = 0;
  m_truthHadTauPdgId = 0;

  m_truthHadTauVisPt = 0;
  m_truthHadTauVisEta = 0;
  m_truthHadTauVisPhi = 0;
  m_truthHadTauVisM = 0;

  m_truthLepTauPt = 0;
  m_truthLepTauEta = 0;
  m_truthLepTauPhi = 0;
  m_truthLepTauE = 0;
  m_truthLepTauPdgId = 0;

  m_truthFinalLeptonPt = 0;
  m_truthFinalLeptonEta = 0;
  m_truthFinalLeptonPhi = 0;
  m_truthFinalLeptonE = 0;
  m_truthFinalLeptonPdgId = 0;

  m_truthHadTauVisFinalLeptondR = 0;

  m_truthDiTauVisPt = 0;
  m_truthDiTauVisEta = 0;
  m_truthDiTauVisPhi = 0;
  m_truthDiTauVisE = 0;

  m_nDiTaus = 0;
  m_diTauPt->clear();
  m_diTauEta->clear();
  m_diTauPhi->clear();
  m_nDiTauSubjets->clear();

  // HadEl DiTaus
  m_nHadElDiTaus = 0;
  m_hadElDiTauPt->clear();
  m_hadElDiTauEta->clear();
  m_hadElDiTauPhi->clear();
  m_hadElDiTauM->clear();
  m_hadElDiTauBDTScore->clear();
  m_bestHadElDiTauBDTScore = 0;
  m_hadElDiTauFlatBDTScore->clear();
  m_hadElDiTauTruthMatchType->clear();
  m_nHadElTaus = 0;
  m_hadElTauPt->clear();
  m_hadElTauEta->clear();
  m_hadElTauPhi->clear();
  m_hadElTauE->clear();
  m_nHadElElectrons = 0;
  m_hadElElectronPt->clear();
  m_hadElElectronEta->clear();
  m_hadElElectronPhi->clear();
  m_hadElElectronE->clear();
  m_hadElDiTaudR->clear();

  // HadMu DiTaus
  m_nHadMuDiTaus = 0;
  m_hadMuDiTauPt->clear();
  m_hadMuDiTauEta->clear();
  m_hadMuDiTauPhi->clear();
  m_hadMuDiTauM->clear();
  m_hadMuDiTauBDTScore->clear();
  m_bestHadMuDiTauBDTScore = 0;
  m_hadMuDiTauFlatBDTScore->clear();
  m_hadMuDiTauTruthMatchType->clear();
  m_nHadMuTaus = 0;
  m_hadMuTauPt->clear();
  m_hadMuTauEta->clear();
  m_hadMuTauPhi->clear();
  m_hadMuTauE->clear();
  m_nHadMuMuons = 0;
  m_hadMuMuonPt->clear();
  m_hadMuMuonEta->clear();
  m_hadMuMuonPhi->clear();
  m_hadMuMuonE->clear();
  m_hadMuDiTaudR->clear();

  // Taus
  m_nTaus = 0;
  m_tauPt->clear();
  m_tauEta->clear();
  m_tauPhi->clear();
  m_tauE->clear();
  //m_tauP4->clear();
  m_leadingTauPt = 0;
  m_leadingTauEta = 0;
  m_leadingTauPhi = 0;
  m_leadingTauE = 0;

  m_bestTauPt = 0;
  m_bestTauEta = 0;
  m_bestTauPhi = 0;
  m_bestTauE = 0;

  // Very Loose taus
  m_nTausVeryLoose = 0;
  m_tauVeryLoosePt->clear();
  m_tauVeryLooseEta->clear();
  m_tauVeryLoosePhi->clear();
  m_tauVeryLooseE->clear();
  m_leadingTauVeryLoosePt = 0;
  m_leadingTauVeryLooseEta = 0;
  m_leadingTauVeryLoosePhi = 0;
  m_leadingTauVeryLooseE = 0;

  // Loose taus
  m_nTausLoose = 0;
  m_tauLoosePt->clear();
  m_tauLooseEta->clear();
  m_tauLoosePhi->clear();
  m_tauLooseE->clear();
  m_leadingTauLoosePt = 0;
  m_leadingTauLooseEta = 0;
  m_leadingTauLoosePhi = 0;
  m_leadingTauLooseE = 0;

  // Medium taus
  m_nTausMedium = 0;
  m_tauMediumPt->clear();
  m_tauMediumEta->clear();
  m_tauMediumPhi->clear();
  m_tauMediumE->clear();
  m_leadingTauMediumPt = 0;
  m_leadingTauMediumEta = 0;
  m_leadingTauMediumPhi = 0;
  m_leadingTauMediumE = 0;

  // Tight taus
  m_nTausTight = 0;
  m_tauTightPt->clear();
  m_tauTightEta->clear();
  m_tauTightPhi->clear();
  m_tauTightE->clear();
  m_leadingTauTightPt = 0;
  m_leadingTauTightEta = 0;
  m_leadingTauTightPhi = 0;
  m_leadingTauTightE = 0;

  // Muons
  m_nMuons = 0;
  m_muPt->clear();
  m_muEta->clear();
  m_muPhi->clear();
  m_muE->clear();
  m_leadingMuPt = 0;
  m_leadingMuEta = 0;
  m_leadingMuPhi = 0;
  m_leadingMuE = 0;

  // Loose muons
  m_nMuonsLoose = 0;
  m_muLoosePt->clear();
  m_muLooseEta->clear();
  m_muLoosePhi->clear();
  m_muLooseE->clear();
  m_leadingMuLoosePt = 0;
  m_leadingMuLooseEta = 0;
  m_leadingMuLoosePhi = 0;
  m_leadingMuLooseE = 0;

  // Medium muons
  m_nMuonsMedium = 0;
  m_muMediumPt->clear();
  m_muMediumEta->clear();
  m_muMediumPhi->clear();
  m_muMediumE->clear();
  m_leadingMuMediumPt = 0;
  m_leadingMuMediumEta = 0;
  m_leadingMuMediumPhi = 0;
  m_leadingMuMediumE = 0;

  // Tight muons
  m_nMuonsTight = 0;
  m_muTightPt->clear();
  m_muTightEta->clear();
  m_muTightPhi->clear();
  m_muTightE->clear();
  m_leadingMuTightPt = 0;
  m_leadingMuTightEta = 0;
  m_leadingMuTightPhi = 0;
  m_leadingMuTightE = 0;

  // Electrons
  m_nElectrons = 0;
  m_elePt->clear();
  m_eleEta->clear();
  m_elePhi->clear();
  m_eleE->clear();
  m_leadingElePt = 0;
  m_leadingEleEta = 0;
  m_leadingElePhi = 0;
  m_leadingEleE = 0;

  m_bestElePt = 0;
  m_bestEleEta = 0;
  m_bestElePhi = 0;
  m_bestEleE = 0;

  // Very Loose electrons
  m_nElectronsVeryLoose = 0;
  m_eleVeryLoosePt->clear();
  m_eleVeryLooseEta->clear();
  m_eleVeryLoosePhi->clear();
  m_eleVeryLooseE->clear();
  m_leadingEleVeryLoosePt = 0;
  m_leadingEleVeryLooseEta = 0;
  m_leadingEleVeryLoosePhi = 0;
  m_leadingEleVeryLooseE = 0;

  // Loose electrons
  m_nElectronsLoose = 0;
  m_eleLoosePt->clear();
  m_eleLooseEta->clear();
  m_eleLoosePhi->clear();
  m_eleLooseE->clear();
  m_leadingEleLoosePt = 0;
  m_leadingEleLooseEta = 0;
  m_leadingEleLoosePhi = 0;
  m_leadingEleLooseE = 0;

  // Medium electrons
  m_nElectronsMedium = 0;
  m_eleMediumPt->clear();
  m_eleMediumEta->clear();
  m_eleMediumPhi->clear();
  m_eleMediumE->clear();
  m_leadingEleMediumPt = 0;
  m_leadingEleMediumEta = 0;
  m_leadingEleMediumPhi = 0;
  m_leadingEleMediumE = 0;

  // Tight electrons
  m_nElectronsTight = 0;
  m_eleTightPt->clear();
  m_eleTightEta->clear();
  m_eleTightPhi->clear();
  m_eleTightE->clear();
  m_leadingEleTightPt = 0;
  m_leadingEleTightEta = 0;
  m_leadingEleTightPhi = 0;
  m_leadingEleTightE = 0;

  // Tau-Ele ditau system
  m_tauEleLoosePt->clear();
  m_tauEleLooseEta->clear();
  m_tauEleLoosePhi->clear();
  m_tauEleLooseE->clear();
  m_tauEleLooseM->clear();
  m_tauEleLoosedR->clear();

  // Tau-Ele ditau system
  m_chosenTauPt = 0;
  m_chosenTauEta = 0;
  m_chosenTauPhi = 0;
  m_chosenTauE = 0;
  m_chosenElePt = 0;
  m_chosenEleEta = 0;
  m_chosenElePhi = 0;
  m_chosenEleE = 0;
  m_chosenTauElePt = 0;
  m_chosenTauEleEta = 0;
  m_chosenTauElePhi = 0;
  m_chosenTauEleE = 0;
  m_chosenTauEleM = 0;
  m_chosenTauEledR = 0;

  m_resolvedMissingMass = 0;

  // Jets
  m_nJets = 0;
  m_jetPt->clear();
  m_jetEta->clear();
  m_jetPhi->clear();
  m_jetE->clear();
  m_jetBTagAccept->clear();
  m_nBTagJets = 0;
  m_leadingJetPt = 0;
  m_leadingJetEta = 0;
  m_leadingJetPhi = 0;
  m_leadingJetE = 0;
  m_subleadingJetPt = 0;
  m_subleadingJetEta = 0;
  m_subleadingJetPhi = 0;
  m_subleadingJetE = 0;

  // B-tagged Jets
  m_nBTagJets = 0;
  m_bTagJetPt->clear();
  m_bTagJetEta->clear();
  m_bTagJetPhi->clear();
  m_bTagJetE->clear();
  m_leadingBTagJetPt = 0;
  m_leadingBTagJetEta = 0;
  m_leadingBTagJetPhi = 0;
  m_leadingBTagJetE = 0;
  m_subleadingBTagJetPt = 0;
  m_subleadingBTagJetEta = 0;
  m_subleadingBTagJetPhi = 0;
  m_subleadingBTagJetE = 0;

  // Large-R Jets
  m_nLargeRJets = 0;
  m_lRJetPt->clear();
  m_lRJetEta->clear();
  m_lRJetPhi->clear();
  m_lRJetE->clear();
  m_lRJetM->clear();
  m_leadingLRJetPt = 0;
  m_leadingLRJetEta = 0;
  m_leadingLRJetPhi = 0;
  m_leadingLRJetE = 0;
  m_leadingLRJetM = 0;
  m_subleadingLRJetPt = 0;
  m_subleadingLRJetEta = 0;
  m_subleadingLRJetPhi = 0;
  m_subleadingLRJetE = 0;
  m_subleadingLRJetM = 0;

  m_largeRJetdR = 0;
  m_largeRJetdEta = 0;
  m_largeRJetdPhi = 0;
  m_largeRJetdPt = 0;

  // Tau large-R jet
  m_tauLRJetPt = 0;
  m_tauLRJetEta = 0;
  m_tauLRJetPhi = 0;
  m_tauLRJetE = 0;
  m_tauLRJetM = 0;
  m_tauLRJetEleLoosedR = 0;

  // b large-R jet
  m_bLRJetPt = 0;
  m_bLRJetEta = 0;
  m_bLRJetPhi = 0;
  m_bLRJetE = 0;
  m_bLRJetM = 0;
  m_bLRJetEleLoosedR = 0;

  // Large-R Higgs
  m_largeRXPt = 0;
  m_largeRXEta = 0;
  m_largeRXPhi = 0;
  m_largeRXE = 0;
  m_largeRXM = 0;

  const xAOD::TruthParticle* truth_tau_higgs = nullptr;
  const xAOD::TruthParticle* truth_b_higgs = nullptr;
  const xAOD::TruthParticle* truth_tau = nullptr;
  const xAOD::TruthParticle* truth_anti_tau = nullptr;
  const xAOD::TruthParticle* truth_leptonic_tau = nullptr;
  const xAOD::TruthParticle* truth_hadronic_tau = nullptr;
  const xAOD::TruthParticle* truth_final_lepton = nullptr;
  TLorentzVector vis_had_tau_p4;
  
  bool has_tau = 0;
  bool has_anti_tau = 0;
  bool has_lepton = 0;
  bool has_leptonic_tau = 0;
  bool has_hadronic_tau = 0;
  bool has_final_lepton = 0;
  bool has_b = 0;
  bool has_anti_b = 0;
  bool has_tau_higgs = 0;
  bool has_b_higgs = 0;

  if(m_isSignal)
  {
    for (auto truth_particle: *truthParticles) {
      int pdg_id = truth_particle->pdgId();
      if (pdg_id == 25) {
        has_tau = 0;
        has_anti_tau = 0;
        has_b = 0;
        has_anti_b = 0;
        unsigned int nchildren = truth_particle->nChildren();
        for (unsigned int i=0; i<nchildren; i++) {
          const xAOD::TruthParticle* child = truth_particle->child(i);
          int child_id = child->pdgId();
          if (child_id == 15) {
            has_tau = 1;
          }
          else if (child_id == -15) {
            has_anti_tau = 1;
          }
          else if (child_id == 5) {
            has_b = 1;
          }
          else if (child_id == -5) {
            has_anti_b = 1;
          }
        }
        if (has_tau && has_anti_tau) {
          truth_tau_higgs = truth_particle;  
          has_tau_higgs = 1;
        }
        else if (has_b && has_anti_b) {
          truth_b_higgs = truth_particle;
          has_b_higgs = 1;
        }
      }
    }

    for (auto truth_tau : *truthTaus) {
      const xAOD::TruthParticle* tau_parent = truth_tau->parent(0);
      if (!tau_parent) {
        continue;
      }
      int parent_pdg = tau_parent->pdgId();
      if (parent_pdg != 25 && parent_pdg != 15 && parent_pdg != -15) {
        continue;
      }
      has_lepton = 0;
      unsigned int nchildren = truth_tau->nChildren();
      for (unsigned int i=0; i<nchildren; i++) {
        const xAOD::TruthParticle* child = truth_tau->child(i);
        int child_id = child->pdgId();
        if (child_id == 11 || child_id == -11 || child_id == 13 || child_id == -13) {
          has_lepton = 1;
        }
      }
      if (has_lepton) {
        has_leptonic_tau = 1;
        truth_leptonic_tau = truth_tau;
      }
      else {
        has_hadronic_tau = 1;
        truth_hadronic_tau = truth_tau;
      }
    }

    if (has_leptonic_tau) {
      unsigned int nchildren = truth_leptonic_tau->nChildren();
      for (unsigned int i=0; i<nchildren; i++) {
        const xAOD::TruthParticle* child = truth_leptonic_tau->child(i);
        int child_id = child->pdgId();
        if (child_id == 11 || child_id == -11 || child_id == 13 || child_id == -13) {
          has_final_lepton = 1;
          truth_final_lepton = child;
        }
      }
    }
  }

  TLorentzVector truth_final_lepton_p4;
  TLorentzVector truth_hadronic_tau_p4;

  if ((has_tau_higgs && has_b_higgs && has_hadronic_tau && has_leptonic_tau && has_final_lepton) || !m_isSignal) {
    bool has_visible = 1;

    // Let's do this thing
    m_eventsPassed++;
    m_runNumber = eventInfo->runNumber();
    m_eventNumber = eventInfo->eventNumber();

    if(m_isSignal)
    {
      truth_final_lepton_p4 = truth_final_lepton->p4();
      truth_hadronic_tau_p4 = truth_hadronic_tau->p4();
  
      int final_lepton_id = truth_final_lepton->pdgId();
      // Values match up with other TruthMatch variable
      // 0 = no match  1 = dihad (N/A here)  2 = hadel  3 = hadmu
      if (final_lepton_id == 11 || final_lepton_id == -11) {
        m_pdgTruthMatchType = 2; 
      } 
      else if ( final_lepton_id == 13 || final_lepton_id == -13) {
        m_pdgTruthMatchType = 3;
      }

      // Truth Higgs decaying to taus
      m_truthHiggsPt = truth_tau_higgs->pt() / 1000.;
      m_truthHiggsEta = truth_tau_higgs->eta();
      m_truthHiggsPhi = truth_tau_higgs->phi();
      m_truthHiggsE = truth_tau_higgs->e() / 1000.;

      // Define accessor for hadronic tau visible variables
      static SG::AuxElement::Accessor<double> accPtVis("pt_vis");
      static SG::AuxElement::Accessor<double> accEtaVis("eta_vis");
      static SG::AuxElement::Accessor<double> accPhiVis("phi_vis");
      static SG::AuxElement::Accessor<double> accMVis("m_vis");

      // Truth hadronic tau
      m_truthHadTauPt = truth_hadronic_tau->pt() / 1000.;
      m_truthHadTauEta = truth_hadronic_tau->eta();
      m_truthHadTauPhi = truth_hadronic_tau->phi();
      m_truthHadTauE = truth_hadronic_tau->e() / 1000.;
      m_truthHadTauPdgId = truth_hadronic_tau->pdgId();

      // If we can't find truthtau or the visible variables for hadronic truth tau, don't try to set
      if (truth_hadronic_tau != nullptr) {
        if (!accPtVis.isAvailable(*truth_hadronic_tau) || !accEtaVis.isAvailable(*truth_hadronic_tau) || !accPhiVis.isAvailable(*truth_hadronic_tau) || !accMVis.isAvailable(*truth_hadronic_tau)) {
          has_visible = 0;
          m_truthHadTauVisPt = 0;
          m_truthHadTauVisEta = 0;
          m_truthHadTauVisPhi = 0;
          m_truthHadTauVisM = 0;
        } 
        else {
          m_truthHadTauVisPt = accPtVis(*truth_hadronic_tau) / 1000.;
          m_truthHadTauVisEta = accEtaVis(*truth_hadronic_tau);
          m_truthHadTauVisPhi = accPhiVis(*truth_hadronic_tau);
          m_truthHadTauVisM = accMVis(*truth_hadronic_tau) / 1000.;
          vis_had_tau_p4.SetPtEtaPhiM(m_truthHadTauVisPt, m_truthHadTauVisEta, m_truthHadTauVisPhi, m_truthHadTauVisM);
        }
      }

      // Truth leptonic tau
      m_truthLepTauPt = truth_leptonic_tau->pt() / 1000.;
      m_truthLepTauEta = truth_leptonic_tau->eta();
      m_truthLepTauPhi = truth_leptonic_tau->phi();
      m_truthLepTauE = truth_leptonic_tau->e() / 1000.;
      m_truthLepTauPdgId = truth_leptonic_tau->pdgId();

      // Truth final lepton
      m_truthFinalLeptonPt = truth_final_lepton->pt() / 1000.;
      m_truthFinalLeptonEta = truth_final_lepton->eta();
      m_truthFinalLeptonPhi = truth_final_lepton->phi();
      m_truthFinalLeptonE = truth_final_lepton->e() / 1000.;
      TLorentzVector truth_final_lepton_p4;
      truth_final_lepton_p4.SetPtEtaPhiE(m_truthFinalLeptonPt, m_truthFinalLeptonEta, m_truthFinalLeptonPhi, m_truthFinalLeptonE);
      m_truthFinalLeptonPdgId = truth_final_lepton->pdgId();

      // Truth Visible Ditau
      if (!has_visible) {
        m_truthDiTauVisPt = 0;
        m_truthDiTauVisEta = 0;
        m_truthDiTauVisPhi = 0;
        m_truthDiTauVisE = 0;
      }
      else { 
        // Truth tau-lepton dR
        m_truthHadTauVisFinalLeptondR = vis_had_tau_p4.DeltaR(truth_final_lepton_p4);
        TLorentzVector truth_ditau_vis_p4 = vis_had_tau_p4 + truth_final_lepton_p4;
        m_truthDiTauVisPt = truth_ditau_vis_p4.Pt();
        m_truthDiTauVisEta = truth_ditau_vis_p4.Eta();
        m_truthDiTauVisPhi = truth_ditau_vis_p4.Phi();
        m_truthDiTauVisE = truth_ditau_vis_p4.E();
      }
    }

    m_jetConstituentModSequence->execute();

    // Reconstructed hadhad ditaus
    for (auto ditau: *ditaus) {
      m_nDiTaus++;
      m_diTauPt->push_back(ditau->pt() / 1000.);
      m_diTauEta->push_back(ditau->eta());
      m_diTauPhi->push_back(ditau->phi());
      m_nDiTauSubjets->push_back(ditau->nSubjets());
    }

    // Reconstructed hadel ditaus
    m_bestHadElDiTauBDTScore = -1; 
    for (auto hadelditau: *hadelditaus) {
      m_nHadElDiTaus++;
      m_hadElDiTauPt->push_back(hadelditau->pt() / 1000.);
      m_hadElDiTauEta->push_back(hadelditau->eta());
      m_hadElDiTauPhi->push_back(hadelditau->phi());
      m_hadElDiTauM->push_back(hadelditau->m() / 1000.);

      static const SG::AuxElement::Accessor<float> acc_tau_pt ("tau_pt");
      static const SG::AuxElement::Accessor<float> acc_tau_eta ("tau_eta");
      static const SG::AuxElement::Accessor<float> acc_tau_phi ("tau_phi");
      static const SG::AuxElement::Accessor<float> acc_tau_E ("tau_E");

      m_nHadElTaus++;
      m_hadElTauPt->push_back(acc_tau_pt(*hadelditau) / 1000.);
      m_hadElTauEta->push_back(acc_tau_eta(*hadelditau));
      m_hadElTauPhi->push_back(acc_tau_phi(*hadelditau));
      m_hadElTauE->push_back(acc_tau_E(*hadelditau) / 1000.);

      static const SG::AuxElement::Accessor<float> acc_el_pt ("electron_pt");
      static const SG::AuxElement::Accessor<float> acc_el_eta ("electron_eta");
      static const SG::AuxElement::Accessor<float> acc_el_phi ("electron_phi");
      static const SG::AuxElement::Accessor<float> acc_el_E ("electron_E");

      m_nHadElElectrons++;
      m_hadElElectronPt->push_back(acc_el_pt(*hadelditau) / 1000.);
      m_hadElElectronEta->push_back(acc_el_eta(*hadelditau));
      m_hadElElectronPhi->push_back(acc_el_phi(*hadelditau));
      m_hadElElectronE->push_back(acc_el_E(*hadelditau) / 1000.);

      static const SG::AuxElement::Accessor<float> acc_DeltaR ("DeltaR");
      m_hadElDiTaudR->push_back(acc_DeltaR(*hadelditau));

      CHECK(m_hadElDiTauIDVarCalculator->execute(*hadelditau));
      
      // this is to reproduce the same value as the DiTauIDVarCalculator did before a bug was fixed.
      // when the BDT is retrained with the results from the fixed DiTauIDVarCalculator this must be deleted
      static const SG::AuxElement::Accessor< int >   acc_el_IDSelection           ( "el_IDSelection" );
      static const SG::AuxElement::Accessor< int >   acc_tau_leadingElIDSelection ( "tau_leadingElIDSelection" );
      static const SG::AuxElement::Decorator< int >  dec_el_IDSelection           ( "el_IDSelection" );
      static const SG::AuxElement::Decorator< int >  dec_tau_leadingElIDSelection ( "tau_leadingElIDSelection" );
      
      if(acc_el_IDSelection(*hadelditau) == 4)
        dec_el_IDSelection(*hadelditau) = 5;
      if(acc_el_IDSelection(*hadelditau) == 1)
        dec_el_IDSelection(*hadelditau) = 3;
      if(acc_el_IDSelection(*hadelditau) == 2)
        dec_el_IDSelection(*hadelditau) = 3;
      
      if(acc_tau_leadingElIDSelection(*hadelditau) == 4)
        dec_tau_leadingElIDSelection(*hadelditau) = 5;
      if(acc_tau_leadingElIDSelection(*hadelditau) == 1)
        dec_tau_leadingElIDSelection(*hadelditau) = 3;
      if(acc_tau_leadingElIDSelection(*hadelditau) == 2)
        dec_tau_leadingElIDSelection(*hadelditau) = 3;
    
      CHECK(m_hadElDiTauDiscrTool->execute(*hadelditau));
      CHECK(m_hadElDiTauWPDecorator->execute(*hadelditau));
      double bdt = hadelditau->auxdata<double>("JetBDT");
      double flat_bdt = hadelditau->auxdata<double>("JetBDTFlat");
      m_hadElDiTauBDTScore->push_back(bdt);
      m_hadElDiTauFlatBDTScore->push_back(flat_bdt);
        
      if (bdt > m_bestHadElDiTauBDTScore || m_bestHadElDiTauBDTScore == -1)
      {
        m_bestHadElDiTauBDTScore = bdt;
      }

      if(m_isSignal)
      {
        m_diTauTruthMatchingTool->getTruth(*hadelditau);
        // 0 = no match  1 = dihad  2 = hadel  3 = hadmu
        unsigned int truth_match_type = 0;
        if (hadelditau->auxdata<char>((const char*)("IsTruthHadronic"))){
          truth_match_type = 1;
          m_truthMatchedHadHad++;
        }
        if (hadelditau->auxdata<char>((const char*)("IsTruthHadEl"))){
          truth_match_type = 2;
          m_truthMatchedHadEl++;
        }
        if (hadelditau->auxdata<char>((const char*)("IsTruthHadMu"))){
          truth_match_type = 3;
          m_truthMatchedHadMu++;
        }
        m_hadElDiTauTruthMatchType->push_back(truth_match_type);
      }
    }

    // Reconstructed hadmu ditaus
    m_bestHadMuDiTauBDTScore = -1; 
    for (auto hadmuditau: *hadmuditaus) {
      m_nHadMuDiTaus++;
      m_hadMuDiTauPt->push_back(hadmuditau->pt() / 1000.);
      m_hadMuDiTauEta->push_back(hadmuditau->eta());
      m_hadMuDiTauPhi->push_back(hadmuditau->phi());
      m_hadMuDiTauM->push_back(hadmuditau->m());

      static const SG::AuxElement::Accessor<float> acc_tau_pt ("tau_pt");
      static const SG::AuxElement::Accessor<float> acc_tau_eta ("tau_eta");
      static const SG::AuxElement::Accessor<float> acc_tau_phi ("tau_phi");
      static const SG::AuxElement::Accessor<float> acc_tau_E ("tau_E");

      m_nHadMuTaus++;
      m_hadMuTauPt->push_back(acc_tau_pt(*hadmuditau) / 1000.);
      m_hadMuTauEta->push_back(acc_tau_eta(*hadmuditau));
      m_hadMuTauPhi->push_back(acc_tau_phi(*hadmuditau));
      m_hadMuTauE->push_back(acc_tau_E(*hadmuditau) / 1000.);

      static const SG::AuxElement::Accessor<float> acc_el_pt ("muon_pt");
      static const SG::AuxElement::Accessor<float> acc_el_eta ("muon_eta");
      static const SG::AuxElement::Accessor<float> acc_el_phi ("muon_phi");
      static const SG::AuxElement::Accessor<float> acc_el_E ("muon_E");

      m_nHadMuMuons++;
      m_hadMuMuonPt->push_back(acc_el_pt(*hadmuditau) / 1000.);
      m_hadMuMuonEta->push_back(acc_el_eta(*hadmuditau));
      m_hadMuMuonPhi->push_back(acc_el_phi(*hadmuditau));
      m_hadMuMuonE->push_back(acc_el_E(*hadmuditau) / 1000.);

      static const SG::AuxElement::Accessor<float> acc_DeltaR ("DeltaR");
      m_hadMuDiTaudR->push_back(acc_DeltaR(*hadmuditau));

      if(m_isSignal)
      {
        /*
        ANA_MSG_INFO("IDVarCalculator");
        ANA_MSG_INFO(m_hadMuDiTauIDVarCalculator);
        CHECK(m_hadMuDiTauIDVarCalculator->execute(*hadmuditau));

        ANA_MSG_INFO("DiscrTool");
        CHECK(m_hadMuDiTauDiscrTool->execute(*hadmuditau));
        ANA_MSG_INFO("WPDecorator");
        CHECK(m_hadMuDiTauWPDecorator->execute(*hadmuditau));
        double bdt = hadmuditau->auxdata<double>("JetBDT");
        double flat_bdt = hadmuditau->auxdata<double>("JetBDTFlat");
        m_hadMuDiTauBDTScore->push_back(bdt);
        m_hadMuDiTauFlatBDTScore->push_back(flat_bdt);

        if (bdt > m_bestHadMuDiTauBDTScore || m_bestHadMuDiTauBDTScore == -1)
        {
          m_bestHadMuDiTauBDTScore = bdt;
        }
        */

        m_diTauTruthMatchingTool->getTruth(*hadmuditau);
        unsigned int truth_match_type = 0;
        if (hadmuditau->auxdata<char>((const char*)("IsTruthHadronic"))){
          truth_match_type = 1;
          m_truthMatchedHadHad++;
        }
        if (hadmuditau->auxdata<char>((const char*)("IsTruthHadEl"))){
          truth_match_type = 2;
          m_truthMatchedHadEl++;
        }
        if (hadmuditau->auxdata<char>((const char*)("IsTruthHadMu"))){
          truth_match_type = 3;
          m_truthMatchedHadMu++;
        }
        m_hadMuDiTauTruthMatchType->push_back(truth_match_type);
      }
    }

    // Reconstructed taus
    const xAOD::TauJet* leading_tau = nullptr;
    const xAOD::TauJet* leading_veryloose_tau = nullptr;
    const xAOD::TauJet* leading_loose_tau = nullptr;
    const xAOD::TauJet* leading_medium_tau = nullptr;
    const xAOD::TauJet* leading_tight_tau = nullptr;
    const xAOD::TauJet* best_tau = nullptr;
    float min_tau_truth_dr = -1;
    for (auto tau: *taus) {
      m_nTaus++;

      float tau_pt = tau->pt() / 1000.;
      float tau_eta = tau->eta();
      float tau_phi = tau->phi();
      float tau_e = tau->e() / 1000.;
      TLorentzVector tau_p4 = tau->p4();

      // Calculate dR of closest reco tau to truth
      if (m_isSignal) {
        if (has_visible) {
          float truth_dr = tau_p4.DeltaR(vis_had_tau_p4);
          if (min_tau_truth_dr == -1 || truth_dr < min_tau_truth_dr) {
            min_tau_truth_dr = truth_dr;
          }
        }
      }

      // All taus
      m_tauPt->push_back(tau_pt);
      m_tauEta->push_back(tau_eta);
      m_tauPhi->push_back(tau_phi);
      m_tauE->push_back(tau_e);
      //m_tauP4->push_back(tau_p4);
      if (tau_pt >= m_leadingTauPt) {
        leading_tau = tau;
        m_leadingTauPt = tau_pt;
        m_leadingTauEta = tau_eta;
        m_leadingTauPhi = tau_phi;
        m_leadingTauE = tau_e;
      }
      // Very Loose taus
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigVeryLoose)) {
        m_nTausVeryLoose++;
        m_tauVeryLoosePt->push_back(tau_pt);
        m_tauVeryLooseEta->push_back(tau_eta);
        m_tauVeryLoosePhi->push_back(tau_phi);
        m_tauVeryLooseE->push_back(tau_e);
        if (tau_pt >= m_leadingTauVeryLoosePt) {
          leading_veryloose_tau = tau;
          m_leadingTauVeryLoosePt = tau_pt;
          m_leadingTauVeryLooseEta = tau_eta;
          m_leadingTauVeryLoosePhi = tau_phi;
          m_leadingTauVeryLooseE = tau_e;
        }
      }
      // Loose taus
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigLoose)) {
        m_nTausLoose++;
        m_tauLoosePt->push_back(tau_pt);
        m_tauLooseEta->push_back(tau_eta);
        m_tauLoosePhi->push_back(tau_phi);
        m_tauLooseE->push_back(tau_e);
        if (tau_pt >= m_leadingTauLoosePt) {
          leading_loose_tau = tau;
          m_leadingTauLoosePt = tau_pt;
          m_leadingTauLooseEta = tau_eta;
          m_leadingTauLoosePhi = tau_phi;
          m_leadingTauLooseE = tau_e;
        }
      }
      // Medium taus
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigMedium)) {
        m_nTausMedium++;
        m_tauMediumPt->push_back(tau_pt);
        m_tauMediumEta->push_back(tau_eta);
        m_tauMediumPhi->push_back(tau_phi);
        m_tauMediumE->push_back(tau_e);
        if (tau_pt >= m_leadingTauMediumPt) {
          leading_medium_tau = tau;
          m_leadingTauMediumPt = tau_pt;
          m_leadingTauMediumEta = tau_eta;
          m_leadingTauMediumPhi = tau_phi;
          m_leadingTauMediumE = tau_e;
        }
      }
      // Tight taus
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigTight)) {
        m_nTausTight++;
        m_tauTightPt->push_back(tau_pt);
        m_tauTightEta->push_back(tau_eta);
        m_tauTightPhi->push_back(tau_phi);
        m_tauTightE->push_back(tau_e);
        if (tau_pt >= m_leadingTauTightPt) {
          leading_tight_tau = tau;
          m_leadingTauTightPt = tau_pt;
          m_leadingTauTightEta = tau_eta;
          m_leadingTauTightPhi = tau_phi;
          m_leadingTauTightE = tau_e;
        }
      }
    }

    m_tauRecoTruthMindR = min_tau_truth_dr;

    // Assign best tau as the leading tau with tightest ID
    if (m_nTausTight > 0) {
      best_tau = leading_tight_tau;
      m_bestTauPt = m_leadingTauTightPt;
      m_bestTauEta = m_leadingTauTightEta;
      m_bestTauPhi = m_leadingTauTightPhi;
      m_bestTauE = m_leadingTauTightE;
    }
    else if (m_nTausMedium > 0) {
      best_tau = leading_medium_tau;
      m_bestTauPt = m_leadingTauMediumPt;
      m_bestTauEta = m_leadingTauMediumEta;
      m_bestTauPhi = m_leadingTauMediumPhi;
      m_bestTauE = m_leadingTauMediumE;
    }
    else if (m_nTausLoose > 0) {
      best_tau = leading_loose_tau;
      m_bestTauPt = m_leadingTauLoosePt;
      m_bestTauEta = m_leadingTauLooseEta;
      m_bestTauPhi = m_leadingTauLoosePhi;
      m_bestTauE = m_leadingTauLooseE;
    }
    else if (m_nTausVeryLoose > 0) {
      best_tau = leading_veryloose_tau;
      m_bestTauPt = m_leadingTauVeryLoosePt;
      m_bestTauEta = m_leadingTauVeryLooseEta;
      m_bestTauPhi = m_leadingTauVeryLoosePhi;
      m_bestTauE = m_leadingTauVeryLooseE;
    }
    else if (m_nTaus > 0) {
      best_tau = leading_tau;
      m_bestTauPt = m_leadingTauPt;
      m_bestTauEta = m_leadingTauEta;
      m_bestTauPhi = m_leadingTauPhi;
      m_bestTauE = m_leadingTauE;
    }

    // Reconstructed muons
    const xAOD::Muon* leading_muon = nullptr;
    const xAOD::Muon* leading_loose_muon = nullptr;
    const xAOD::Muon* leading_medium_muon = nullptr;
    const xAOD::Muon* leading_tight_muon = nullptr;
    float min_mu_truth_dr = -1;
    for (auto muon: *muons) {
      m_nMuons++;

      // Calculate dR of closest reco muon to truth
      if (m_isSignal) {
        TLorentzVector muon_p4 = muon->p4();
        float truth_dr = muon_p4.DeltaR(truth_final_lepton_p4);
        if (min_mu_truth_dr == -1 || truth_dr < min_mu_truth_dr) {
          min_mu_truth_dr = truth_dr;
        }
      }

      float muon_pt = muon->pt() / 1000.;
      float muon_eta = muon->eta();
      float muon_phi = muon->phi();
      float muon_e = muon->e() / 1000.;
      m_muPt->push_back(muon_pt);
      m_muEta->push_back(muon_eta);
      m_muPhi->push_back(muon_phi);
      m_muE->push_back(muon_e);
      if (muon_pt >= m_leadingMuPt) {
        leading_muon = muon;
        m_leadingMuPt = muon_pt;
        m_leadingMuEta = muon_eta;
        m_leadingMuPhi = muon_phi;
        m_leadingMuE = muon_e;
      }
      // Muon quality 0=Tight 1=Medium 2=Loose 3=VeryLoose(all)
      unsigned int muQuality = muon->quality();
      // Tight muon, fill for all WPs
      if (muQuality == 0) {
        // Loose muon
        m_nMuonsLoose++;
        m_muLoosePt->push_back(muon_pt);
        m_muLooseEta->push_back(muon_eta);
        m_muLoosePhi->push_back(muon_phi);
        m_muLooseE->push_back(muon_e);
        if (muon_pt >= m_leadingMuLoosePt) {
          leading_loose_muon = muon;
          m_leadingMuLoosePt = muon_pt;
          m_leadingMuLooseEta = muon_eta;
          m_leadingMuLoosePhi = muon_phi;
          m_leadingMuLooseE = muon_e;
        }
        // Medium muon
        m_nMuonsMedium++;
        m_muMediumPt->push_back(muon_pt);
        m_muMediumEta->push_back(muon_eta);
        m_muMediumPhi->push_back(muon_phi);
        m_muMediumE->push_back(muon_e);
        if (muon_pt >= m_leadingMuMediumPt) {
          leading_medium_muon = muon;
          m_leadingMuMediumPt = muon_pt;
          m_leadingMuMediumEta = muon_eta;
          m_leadingMuMediumPhi = muon_phi;
          m_leadingMuMediumE = muon_e;
        }
        // Tight muon
        m_nMuonsTight++;
        m_muTightPt->push_back(muon_pt);
        m_muTightEta->push_back(muon_eta);
        m_muTightPhi->push_back(muon_phi);
        m_muTightE->push_back(muon_e);
        if (muon_pt >= m_leadingMuTightPt) {
          leading_tight_muon;
          m_leadingMuTightPt = muon_pt;
          m_leadingMuTightEta = muon_eta;
          m_leadingMuTightPhi = muon_phi;
          m_leadingMuTightE = muon_e;
        }
      // Medium muon, fill Medium and Loose
      } else if (muQuality == 1) {
        // Loose muon
        m_nMuonsLoose++;
        m_muLoosePt->push_back(muon_pt);
        m_muLooseEta->push_back(muon_eta);
        m_muLoosePhi->push_back(muon_phi);
        m_muLooseE->push_back(muon_e);
        if (muon_pt >= m_leadingMuLoosePt) {
          leading_loose_muon = muon;
          m_leadingMuLoosePt = muon_pt;
          m_leadingMuLooseEta = muon_eta;
          m_leadingMuLoosePhi = muon_phi;
          m_leadingMuLooseE = muon_e;
        }
        // Medium muon
        m_nMuonsMedium++;
        m_muMediumPt->push_back(muon_pt);
        m_muMediumEta->push_back(muon_eta);
        m_muMediumPhi->push_back(muon_phi);
        m_muMediumE->push_back(muon_e);
        if (muon_pt >= m_leadingMuMediumPt) {
          leading_medium_muon = muon;
          m_leadingMuMediumPt = muon_pt;
          m_leadingMuMediumEta = muon_eta;
          m_leadingMuMediumPhi = muon_phi;
          m_leadingMuMediumE = muon_e;
        }
      // Loose muon, just Loose
      } else if (muQuality == 2) {
        // Loose muon
        m_nMuonsLoose++;
        m_muLoosePt->push_back(muon_pt);
        m_muLooseEta->push_back(muon_eta);
        m_muLoosePhi->push_back(muon_phi);
        m_muLooseE->push_back(muon_e);
        if (muon_pt >= m_leadingMuLoosePt) {
          leading_loose_muon = muon;
          m_leadingMuLoosePt = muon_pt;
          m_leadingMuLooseEta = muon_eta;
          m_leadingMuLoosePhi = muon_phi;
          m_leadingMuLooseE = muon_e;
        }
      } 
    }

    m_muRecoTruthMindR = min_mu_truth_dr;

    // Reconstructed electrons
    const xAOD::Electron* leading_electron = nullptr;
    const xAOD::Electron* leading_veryloose_electron = nullptr;
    const xAOD::Electron* leading_loose_electron = nullptr;
    const xAOD::Electron* leading_medium_electron = nullptr;
    const xAOD::Electron* leading_tight_electron = nullptr;
    const xAOD::Electron* best_electron = nullptr;
    std::vector<const xAOD::Electron*> loose_electrons;
    float min_ele_truth_dr = -1;
    for (auto electron: *electrons) {
      m_nElectrons++;

      // Calculate dR of closest reco electron to truth
      if (m_isSignal) {
        TLorentzVector electron_p4 = electron->p4();
        float truth_dr = electron_p4.DeltaR(truth_final_lepton_p4);
        if (min_ele_truth_dr == -1 || truth_dr < min_ele_truth_dr) {
          min_ele_truth_dr = truth_dr;
        }
      }

      float electron_pt = electron->pt() / 1000.;
      float electron_eta = electron->eta();
      float electron_phi = electron->phi();
      float electron_e = electron->e() / 1000.;
      m_elePt->push_back(electron_pt);
      m_eleEta->push_back(electron_eta);
      m_elePhi->push_back(electron_phi);
      m_eleE->push_back(electron_e);
      if (electron_pt >= m_leadingElePt) {
        leading_electron = electron;
        m_leadingElePt = electron_pt;
        m_leadingEleEta = electron_eta;
        m_leadingElePhi = electron_phi;
        m_leadingEleE = electron_e;
      }
      if (static_cast<int>(m_checkEleVeryLooseLH->accept(electron))) {
        m_nElectronsVeryLoose++;
        m_eleVeryLoosePt->push_back(electron_pt);
        m_eleVeryLooseEta->push_back(electron_eta);
        m_eleVeryLoosePhi->push_back(electron_phi);
        m_eleVeryLooseE->push_back(electron_e);
        if (electron_pt >= m_leadingEleVeryLoosePt) {
          leading_veryloose_electron = electron;
          m_leadingEleVeryLoosePt = electron_pt;
          m_leadingEleVeryLooseEta = electron_eta;
          m_leadingEleVeryLoosePhi = electron_phi;
          m_leadingEleVeryLooseE = electron_e;
        }
      }
      if (static_cast<int>(m_checkEleLooseLH->accept(electron))) {
        m_nElectronsLoose++;
        loose_electrons.push_back(electron);
        m_eleLoosePt->push_back(electron_pt);
        m_eleLooseEta->push_back(electron_eta);
        m_eleLoosePhi->push_back(electron_phi);
        m_eleLooseE->push_back(electron_e);
        if (electron_pt >= m_leadingEleLoosePt) {
          leading_loose_electron = electron;
          m_leadingEleLoosePt = electron_pt;
          m_leadingEleLooseEta = electron_eta;
          m_leadingEleLoosePhi = electron_phi;
          m_leadingEleLooseE = electron_e;
        }
      }
      if (static_cast<int>(m_checkEleMediumLH->accept(electron))) {
        m_nElectronsMedium++;
        m_eleMediumPt->push_back(electron_pt);
        m_eleMediumEta->push_back(electron_eta);
        m_eleMediumPhi->push_back(electron_phi);
        m_eleMediumE->push_back(electron_e);
        if (electron_pt >= m_leadingEleMediumPt) {
          leading_medium_electron = electron;
          m_leadingEleMediumPt = electron_pt;
          m_leadingEleMediumEta = electron_eta;
          m_leadingEleMediumPhi = electron_phi;
          m_leadingEleMediumE = electron_e;
        }
      }
      if (static_cast<int>(m_checkEleTightLH->accept(electron))) {
        m_nElectronsTight++;
        m_eleTightPt->push_back(electron_pt);
        m_eleTightEta->push_back(electron_eta);
        m_eleTightPhi->push_back(electron_phi);
        m_eleTightE->push_back(electron_e);
        if (electron_pt >= m_leadingEleTightPt) {
          leading_tight_electron = electron;
          m_leadingEleTightPt = electron_pt;
          m_leadingEleTightEta = electron_eta;
          m_leadingEleTightPhi = electron_phi;
          m_leadingEleTightE = electron_e;
        }
      }
    }

    m_eleRecoTruthMindR = min_ele_truth_dr;

    // Assign best electron as the leading electron with tightest ID
    if (m_nElectronsTight > 0) {
      best_electron = leading_tight_electron;
      m_bestElePt = m_leadingEleTightPt;
      m_bestEleEta = m_leadingEleTightEta;
      m_bestElePhi = m_leadingEleTightPhi;
      m_bestEleE = m_leadingEleTightE;
    }
    else if (m_nElectronsMedium > 0) {
      best_electron = leading_medium_electron;
      m_bestElePt = m_leadingEleMediumPt;
      m_bestEleEta = m_leadingEleMediumEta;
      m_bestElePhi = m_leadingEleMediumPhi;
      m_bestEleE = m_leadingEleMediumE;
    }
    else if (m_nElectronsLoose > 0) {
      best_electron = leading_loose_electron;
      m_bestElePt = m_leadingEleLoosePt;
      m_bestEleEta = m_leadingEleLooseEta;
      m_bestElePhi = m_leadingEleLoosePhi;
      m_bestEleE = m_leadingEleLooseE;
    }
    else if (m_nElectronsVeryLoose > 0) {
      best_electron = leading_veryloose_electron;
      m_bestElePt = m_leadingEleVeryLoosePt;
      m_bestEleEta = m_leadingEleVeryLooseEta;
      m_bestElePhi = m_leadingEleVeryLoosePhi;
      m_bestEleE = m_leadingEleVeryLooseE;
    }
    else if (m_nElectrons > 0) {
      best_electron = leading_electron;
      m_bestElePt = m_leadingElePt;
      m_bestEleEta = m_leadingEleEta;
      m_bestElePhi = m_leadingElePhi;
      m_bestEleE = m_leadingEleE;
    }

    // Reconstructed jets
    // Number of jets > 25 GeV for MMC
    unsigned int njet25 = 0;
    for (auto jet: *jets) {
      m_nJets++;
      float jet_pt = jet->pt() / 1000.;
      float jet_eta = jet->eta();
      float jet_phi = jet->phi();
      float jet_e = jet->e() / 1000.;

      if (jet_pt > 25.) {
        njet25++;
      }

      m_jetPt->push_back(jet_pt);
      m_jetEta->push_back(jet_eta);
      m_jetPhi->push_back(jet_phi);
      m_jetE->push_back(jet_e);
      if (jet_pt >= m_leadingJetPt) {
        m_subleadingJetPt = m_leadingJetPt;
        m_subleadingJetEta = m_leadingJetEta;
        m_subleadingJetPhi = m_leadingJetPhi;
        m_subleadingJetE = m_leadingJetE;
        m_leadingJetPt = jet_pt;
        m_leadingJetEta = jet_eta;
        m_leadingJetPhi = jet_phi;
        m_leadingJetE = jet_e;
      }
      else if (jet_pt >= m_subleadingJetPt) {
        m_subleadingJetPt = jet_pt;
        m_subleadingJetEta = jet_eta;
        m_subleadingJetPhi = jet_phi;
        m_subleadingJetE = jet_e;
      }
      bool btag_accept = m_bTaggingSelectionTool->accept(*jet);     
      m_jetBTagAccept->push_back(btag_accept);
      if (btag_accept) {
        m_nBTagJets++;
        m_bTagJetPt->push_back(jet_pt);
        m_bTagJetEta->push_back(jet_eta);
        m_bTagJetPhi->push_back(jet_phi);
        m_bTagJetE->push_back(jet_e);
        if (jet_pt >= m_leadingBTagJetPt) {
          m_subleadingBTagJetPt = m_leadingBTagJetPt;
          m_subleadingBTagJetEta = m_leadingBTagJetEta;
          m_subleadingBTagJetPhi = m_leadingBTagJetPhi;
          m_subleadingBTagJetE = m_leadingBTagJetE;
          m_leadingBTagJetPt = jet_pt;
          m_leadingBTagJetEta = jet_eta;
          m_leadingBTagJetPhi = jet_phi;
          m_leadingBTagJetE = jet_e;
        }
        else if (jet_pt >= m_subleadingBTagJetPt) {
          m_subleadingBTagJetPt = jet_pt;
          m_subleadingBTagJetEta = jet_eta;
          m_subleadingBTagJetPhi = jet_phi;
          m_subleadingBTagJetE = jet_e;
        }
      }
    }

    // Reconstructed large-R jets
    const xAOD::Jet* leading_largerjet;
    const xAOD::Jet* subleading_largerjet;
    for (auto largerjet: *largerjets) {
      m_nLargeRJets++;
      float largerjet_pt = largerjet->pt() / 1000.;
      float largerjet_eta = largerjet->eta();
      float largerjet_phi = largerjet->phi();
      float largerjet_e = largerjet->e() / 1000.;
      float largerjet_m = largerjet->m() / 1000.;
      m_lRJetPt->push_back(largerjet_pt);
      m_lRJetEta->push_back(largerjet_eta);
      m_lRJetPhi->push_back(largerjet_phi);
      m_lRJetE->push_back(largerjet_e);
      m_lRJetM->push_back(largerjet_m);
      if (largerjet_pt >= m_leadingLRJetPt) {
        // Leading jet bumped down to subleading
        subleading_largerjet = leading_largerjet;
        m_subleadingLRJetPt = m_leadingLRJetPt;
        m_subleadingLRJetEta = m_leadingLRJetEta;
        m_subleadingLRJetPhi = m_leadingLRJetPhi;
        m_subleadingLRJetE = m_leadingLRJetE;
        m_subleadingLRJetM = m_leadingLRJetM;
        // New jet moved to leading
        leading_largerjet = largerjet;
        m_leadingLRJetPt = largerjet_pt;
        m_leadingLRJetEta = largerjet_eta;
        m_leadingLRJetPhi = largerjet_phi;
        m_leadingLRJetE = largerjet_e;
        m_leadingLRJetM = largerjet_m;
      }
      else if (largerjet_pt >= m_subleadingLRJetPt) {
        subleading_largerjet = largerjet;
        m_subleadingLRJetPt = largerjet_pt;
        m_subleadingLRJetEta = largerjet_eta;
        m_subleadingLRJetPhi = largerjet_phi;
        m_subleadingLRJetE = largerjet_e;
        m_subleadingLRJetM = largerjet_m;
      }
    }

    // Fill topological values of two leading large-R jets
    if (m_nLargeRJets > 1) {
      TLorentzVector leading_largerjet_p4 = leading_largerjet->p4();
      TLorentzVector subleading_largerjet_p4 = subleading_largerjet->p4();

      m_largeRJetdR = leading_largerjet_p4.DeltaR(subleading_largerjet_p4);
      m_largeRJetdEta = std::abs( leading_largerjet_p4.Eta() - subleading_largerjet_p4.Eta() );
      m_largeRJetdPhi = std::abs( leading_largerjet_p4.DeltaPhi(subleading_largerjet_p4) );
      m_largeRJetdPt = std::abs( leading_largerjet_p4.Pt() - subleading_largerjet_p4.Pt() ) / 1000.;

      TLorentzVector larger_x_p4 = leading_largerjet_p4 + subleading_largerjet_p4;
      
      m_largeRXPt = larger_x_p4.Pt() / 1000.;
      m_largeRXEta = larger_x_p4.Eta();
      m_largeRXPhi = larger_x_p4.Phi();
      m_largeRXE = larger_x_p4.E() / 1000.;
      m_largeRXM = larger_x_p4.M() / 1000.;
    }

    // If a loose electron was found, fill values dependent on it
    if (leading_loose_electron && m_nTaus > 0) {
      TLorentzVector leading_loose_electron_p4 = leading_loose_electron->p4();

      for (auto tau: *taus) {
        TLorentzVector tau_p4 = tau->p4();
        TLorentzVector tau_loose_electron_p4 = tau_p4 + leading_loose_electron_p4;

        m_tauEleLoosePt->push_back(tau_loose_electron_p4.Pt() / 1000.);
        m_tauEleLooseEta->push_back(tau_loose_electron_p4.Eta());
        m_tauEleLoosePhi->push_back(tau_loose_electron_p4.Phi());
        m_tauEleLooseE->push_back(tau_loose_electron_p4.E() / 1000.);
        m_tauEleLooseM->push_back(tau_loose_electron_p4.M() / 1000.);
        m_tauEleLoosedR->push_back(tau_p4.DeltaR(leading_loose_electron_p4));
      }

      if (m_nLargeRJets > 1) {
        TLorentzVector leading_largerjet_p4 = leading_largerjet->p4();
        TLorentzVector subleading_largerjet_p4 = subleading_largerjet->p4();

        float ele_loose_leading_largerjet_dr = leading_loose_electron_p4.DeltaR(leading_largerjet_p4);
        float ele_loose_subleading_largerjet_dr = leading_loose_electron_p4.DeltaR(subleading_largerjet_p4);
        TLorentzVector tau_largerjet_p4;
        // If the loose electron is closest to the leading large-R jet, then it is the tau jet
        if (ele_loose_leading_largerjet_dr < ele_loose_subleading_largerjet_dr) {
          tau_largerjet_p4 = leading_largerjet_p4;
          m_tauLRJetPt = m_leadingLRJetPt;
          m_tauLRJetEta = m_leadingLRJetEta;
          m_tauLRJetPhi = m_leadingLRJetPhi;
          m_tauLRJetE = m_leadingLRJetE;
          m_tauLRJetM = m_leadingLRJetM;
          m_tauLRJetEleLoosedR = ele_loose_leading_largerjet_dr;
          m_bLRJetPt = m_subleadingLRJetPt;
          m_bLRJetEta = m_subleadingLRJetEta;
          m_bLRJetPhi = m_subleadingLRJetPhi;
          m_bLRJetE = m_subleadingLRJetE;
          m_bLRJetM = m_subleadingLRJetM;
          m_bLRJetEleLoosedR = ele_loose_subleading_largerjet_dr;
        // If closest to the subleading large-R jet, then it is the tau jet
        }
        else {
          m_bLRJetPt = m_leadingLRJetPt;
          m_bLRJetEta = m_leadingLRJetEta;
          m_bLRJetPhi = m_leadingLRJetPhi;
          m_bLRJetE = m_leadingLRJetE;
          m_bLRJetM = m_leadingLRJetM;
          m_bLRJetEleLoosedR = ele_loose_leading_largerjet_dr;
          tau_largerjet_p4 = subleading_largerjet_p4;
          m_tauLRJetPt = m_subleadingLRJetPt;
          m_tauLRJetEta = m_subleadingLRJetEta;
          m_tauLRJetPhi = m_subleadingLRJetPhi;
          m_tauLRJetE = m_subleadingLRJetE;
          m_tauLRJetM = m_subleadingLRJetM;
          m_tauLRJetEleLoosedR = ele_loose_subleading_largerjet_dr;
        }
      }
    }
      
    // Define the chosen tau and electron to make up ditau system
    TLorentzVector chosen_tau_p4;
    TLorentzVector chosen_electron_p4;
    bool set_electron = 0;
    bool set_tau = 0;

    // First find leading loose electron if it exists
    if (m_nElectronsLoose > 0) {
      auto el = leading_loose_electron;
      chosen_electron_p4.SetPtEtaPhiE(el->pt(), el->eta(), el->phi(), el->e());
      set_electron = 1;
    }

    // Find closest tau to chosen electron
    if (set_electron && (m_nTaus > 0)) {
      float min_dr = 0;
      for (auto tau: *taus) {
        TLorentzVector tau_p4 = tau->p4();
        float dr = chosen_electron_p4.DeltaR(tau_p4);
        if ((dr < min_dr) || (min_dr == 0)) {
          chosen_tau_p4.SetPtEtaPhiE(tau->pt(), tau->eta(), tau->phi(), tau->e());
          set_tau = 1;
          min_dr = dr;
        }
      }
    }

    // If dR between electron and tau is < 0.1, then look for another in 0.1 < dR < 1 and pt > 20 GeV
    float chosen_dr = 0;
    unsigned int switched_taus = 0;
    if (set_electron && set_tau) {
      chosen_dr = chosen_electron_p4.DeltaR(chosen_tau_p4);
      if (chosen_dr < 0.1) {
        float min_dr = 0;
        for (auto tau: *taus) {
          TLorentzVector tau_p4 = tau->p4();
          float tau_pt = tau->pt() / 1000.;
          float dr = chosen_electron_p4.DeltaR(tau_p4);
          if (((dr < min_dr) || (min_dr == 0)) && tau_pt > 20. && (dr > 0.1) && (dr < 1.0)) {
            chosen_tau_p4.SetPtEtaPhiE(tau->pt(), tau->eta(), tau->phi(), tau->e());
            set_tau = 1;
            min_dr = dr;
            switched_taus = 1;
          }
        }
      }
    } 

    // If no tau found but another loose electron within 1.0 switch to that
    if (set_electron && set_tau && (chosen_dr < 0.1) && !switched_taus) {
      float min_dr = 0;
      for (auto loose_electron: loose_electrons) {
        TLorentzVector loose_electron_p4 = loose_electron->p4();
        float dr = chosen_electron_p4.DeltaR(loose_electron_p4);
        if (((dr < min_dr) || min_dr == 0) && (dr > 0.1) && (dr < 1.0)) {
          auto el = loose_electron;
          chosen_electron_p4.SetPtEtaPhiE(el->pt(), el->eta(), el->phi(), el->e());
          set_electron = 1;
          min_dr = dr;
        }
      } 
    }

    // If electron and tau still within 0.2 then look to ditau with overlapping electron
    if (set_electron && set_tau) {
      chosen_dr = chosen_electron_p4.DeltaR(chosen_tau_p4);
    }
    if (set_electron && set_tau && (chosen_dr < 0.2)) {
      float max_bdt = -5;
      for (auto ditau: *hadelditaus) {
        static const SG::AuxElement::Accessor<float> acc_el_pt ("electron_pt");
        float hadel_electron_pt = acc_el_pt(*ditau);
        double bdt = ditau->auxdata<double>("JetBDT");
        if (((bdt > max_bdt) || bdt == -5) && (chosen_electron_p4.Pt() == hadel_electron_pt)) {
          static const SG::AuxElement::Accessor<float> acc_tau_pt ("tau_pt");
          static const SG::AuxElement::Accessor<float> acc_tau_eta ("tau_eta");
          static const SG::AuxElement::Accessor<float> acc_tau_phi ("tau_phi");
          static const SG::AuxElement::Accessor<float> acc_tau_E ("tau_E");
          float hadel_tau_pt = acc_tau_pt(*ditau);
          float hadel_tau_eta = acc_tau_eta(*ditau);
          float hadel_tau_phi = acc_tau_phi(*ditau);
          float hadel_tau_e = acc_tau_E(*ditau);
          chosen_tau_p4.SetPtEtaPhiE(hadel_tau_pt, hadel_tau_eta, hadel_tau_phi, hadel_tau_e);
          set_tau = 1;
          max_bdt = bdt;
        }
      }
    }
    
    if (set_tau) {
      m_chosenTauPt = chosen_tau_p4.Pt() / 1000.; 
      m_chosenTauEta = chosen_tau_p4.Eta(); 
      m_chosenTauPhi = chosen_tau_p4.Phi(); 
      m_chosenTauE = chosen_tau_p4.E() / 1000.; 
    }

    if (set_electron) {
      m_chosenElePt = chosen_electron_p4.Pt() / 1000.; 
      m_chosenEleEta = chosen_electron_p4.Eta(); 
      m_chosenElePhi = chosen_electron_p4.Phi(); 
      m_chosenEleE = chosen_electron_p4.E() / 1000.; 
    }

    if (set_tau && set_electron) {
      TLorentzVector chosen_tauele_system_p4 = chosen_tau_p4 + chosen_electron_p4;
      m_chosenTauElePt = chosen_tauele_system_p4.Pt() / 1000.; 
      m_chosenTauEleEta = chosen_tauele_system_p4.Eta(); 
      m_chosenTauElePhi = chosen_tauele_system_p4.Phi(); 
      m_chosenTauEleE = chosen_tauele_system_p4.E() / 1000.; 
      m_chosenTauEleM = chosen_tauele_system_p4.M() / 1000.; 
      m_chosenTauEledR = chosen_tau_p4.DeltaR(chosen_electron_p4);
    }

    // Missing Mass Calculator
    const xAOD::MissingET* met = mets->at(mets->size() - 1);

    //if (leading_tau && leading_electron) {
    //  CP::CorrectionCode c = m_missingMassTool->apply(*eventInfo, leading_tau, leading_electron, met, njet25); 
    //}

    // MMC for resolved analysis, will calculate for superset of valid resolved signal events
    if (leading_tight_electron && leading_tau) {
      CP::CorrectionCode c = m_missingMassTool->apply(*eventInfo, leading_tau, leading_tight_electron, met, njet25); 
      m_resolvedMissingMass = eventInfo->auxdata<double>("mmc_mlnu3p_mass");
    }
    // Run for muon channel, may override previous, but those events have strong electron and muon and so won't make it into signal anyway
    if (leading_medium_muon && leading_tau) {
      CP::CorrectionCode c = m_missingMassTool->apply(*eventInfo, leading_tau, leading_medium_muon, met, njet25); 
      m_resolvedMissingMass = eventInfo->auxdata<double>("mmc_mlnu3p_mass");
    }


    m_mytree->Fill();
  }

  return StatusCode::SUCCESS;
}



StatusCode DiTauAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.
  ANA_MSG_INFO("in finalize");
  ANA_MSG_INFO("Events passed: " << m_eventsPassed);

  ANA_MSG_INFO("HadHad truth matches: " << m_truthMatchedHadHad);
  ANA_MSG_INFO("HadEl truth matches: " << m_truthMatchedHadEl);
  ANA_MSG_INFO("HadMu truth matches: " << m_truthMatchedHadMu);

  return StatusCode::SUCCESS;
}
