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
#include <string>

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
  declareProperty( "jetCollectionName" , m_jetCollectionName="AntiKt4EMTopoJets_BTagging201810" );
  declareProperty( "largeRJetCollectionName" , m_largeRJetCollectionName="AntiKt10LCTopoJets" );
  declareProperty( "isDAOD" , m_isDAOD=true );

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
  delete m_hadElTauTruthdR;
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
  delete m_hadMuTauTruthdR;
  delete m_hadMuMuonPt;
  delete m_hadMuMuonEta;
  delete m_hadMuMuonPhi;
  delete m_hadMuMuonE;
  delete m_hadMuDiTaudR;

  delete m_tauPt;
  delete m_tauEta;
  delete m_tauPhi;
  delete m_tauE;
  delete m_tauTruthdR;
  delete m_tauID;
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
  delete m_tauLooseNTracks;
  delete m_tauLooseCharge;
  delete m_tauVeryLoosePt;
  delete m_tauVeryLooseEta;
  delete m_tauVeryLoosePhi;
  delete m_tauVeryLooseE;

  delete m_muPt;
  delete m_muEta;
  delete m_muPhi;
  delete m_muE;
  delete m_muTruthdR;
  delete m_muID;
  delete m_muTightPt;
  delete m_muTightEta;
  delete m_muTightPhi;
  delete m_muTightE;
  delete m_muMediumPt;
  delete m_muMediumEta;
  delete m_muMediumPhi;
  delete m_muMediumE;
  delete m_muMediumCharge;
  delete m_muLoosePt;
  delete m_muLooseEta;
  delete m_muLoosePhi;
  delete m_muLooseE;

  delete m_elePt;
  delete m_eleEta;
  delete m_elePhi;
  delete m_eleE;
  delete m_eleTruthdR;
  delete m_eleID;
  delete m_eleTightPt;
  delete m_eleTightEta;
  delete m_eleTightPhi;
  delete m_eleTightE;
  delete m_eleTightCharge;
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
  m_mytree->Branch("UniqueEventID", &m_uniqueEventID);

  if(m_isSignal)
  {
    m_mytree->Branch("PDGTruthMatchType", &m_pdgTruthMatchType);

    m_mytree->Branch("TruthBPt", &m_truthBPt);
    m_mytree->Branch("TruthBEta", &m_truthBEta);
    m_mytree->Branch("TruthBPhi", &m_truthBPhi);
    m_mytree->Branch("TruthBE", &m_truthBE);

    m_mytree->Branch("TruthAntiBPt", &m_truthAntiBPt);
    m_mytree->Branch("TruthAntiBEta", &m_truthAntiBEta);
    m_mytree->Branch("TruthAntiBPhi", &m_truthAntiBPhi);
    m_mytree->Branch("TruthAntiBE", &m_truthAntiBE);

    m_mytree->Branch("TruthBdR", &m_truthBdR);

    m_mytree->Branch("TruthTauHiggsPt", &m_truthTauHiggsPt);
    m_mytree->Branch("TruthTauHiggsEta", &m_truthTauHiggsEta);
    m_mytree->Branch("TruthTauHiggsPhi", &m_truthTauHiggsPhi);
    m_mytree->Branch("TruthTauHiggsE", &m_truthTauHiggsE);

    m_mytree->Branch("TruthBHiggsPt", &m_truthBHiggsPt);
    m_mytree->Branch("TruthBHiggsEta", &m_truthBHiggsEta);
    m_mytree->Branch("TruthBHiggsPhi", &m_truthBHiggsPhi);
    m_mytree->Branch("TruthBHiggsE", &m_truthBHiggsE);

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
  m_hadElTauTruthdR = new std::vector<float>();
  m_mytree->Branch("HadElTauTruthdR", &m_hadElTauTruthdR);
  
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
  m_hadMuTauTruthdR = new std::vector<float>();
  m_mytree->Branch("HadMuTauTruthdR", &m_hadMuTauTruthdR);

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
  m_mytree->Branch("LeadingTauPt", &m_leadingTauPt);
  m_mytree->Branch("LeadingTauEta", &m_leadingTauEta);
  m_mytree->Branch("LeadingTauPhi", &m_leadingTauPhi);
  m_mytree->Branch("LeadingTauE", &m_leadingTauE);
  m_mytree->Branch("TauTruthdR", &m_tauTruthdR);
  m_mytree->Branch("TauID", &m_tauID);
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
  m_mytree->Branch("NResolvedTaus", &m_nResolvedTaus);
  m_tauLoosePt = new std::vector<float>();
  m_mytree->Branch("TauLoosePt", &m_tauLoosePt);
  m_tauLooseEta = new std::vector<float>();
  m_mytree->Branch("TauLooseEta", &m_tauLooseEta);
  m_tauLoosePhi = new std::vector<float>();
  m_mytree->Branch("TauLoosePhi", &m_tauLoosePhi);
  m_tauLooseE = new std::vector<float>();
  m_mytree->Branch("TauLooseE", &m_tauLooseE);
  m_tauLooseNTracks = new std::vector<float>();
  m_mytree->Branch("TauLooseNTracks", &m_tauLooseNTracks);
  m_tauLooseCharge = new std::vector<float>();
  m_mytree->Branch("TauLooseCharge", &m_tauLooseCharge);
  m_mytree->Branch("LeadingTauLoosePt", &m_leadingTauLoosePt);
  m_mytree->Branch("LeadingTauLooseEta", &m_leadingTauLooseEta);
  m_mytree->Branch("LeadingTauLoosePhi", &m_leadingTauLoosePhi);
  m_mytree->Branch("LeadingTauLooseE", &m_leadingTauLooseE);
  m_mytree->Branch("LeadingTauLooseNTracks", &m_leadingTauLooseNTracks);
  m_mytree->Branch("LeadingTauLooseCharge", &m_leadingTauLooseCharge);

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
  m_mytree->Branch("MuTruthdR", &m_muTruthdR);
  m_mytree->Branch("MuID", &m_muID);
  m_mytree->Branch("MuRecoTruthMindR", &m_muRecoTruthMindR);

  // Loose muons
  m_mytree->Branch("NMuLoose", &m_nMuonsLoose);
  m_mytree->Branch("NResolvedMuons", &m_nResolvedMuons);
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
  m_muMediumCharge = new std::vector<float>();
  m_mytree->Branch("MuMediumCharge", &m_muMediumCharge);
  m_mytree->Branch("LeadingMuMediumPt", &m_leadingMuMediumPt);
  m_mytree->Branch("LeadingMuMediumEta", &m_leadingMuMediumEta);
  m_mytree->Branch("LeadingMuMediumPhi", &m_leadingMuMediumPhi);
  m_mytree->Branch("LeadingMuMediumE", &m_leadingMuMediumE);
  m_mytree->Branch("LeadingMuMediumCharge", &m_leadingMuMediumCharge);

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
  m_mytree->Branch("EleTruthdR", &m_eleTruthdR);
  m_mytree->Branch("EleID", &m_eleID);
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
  m_mytree->Branch("NResolvedElectrons", &m_nResolvedElectrons);
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
  m_eleTightCharge = new std::vector<float>();
  m_mytree->Branch("EleTightCharge", &m_eleTightCharge);
  m_mytree->Branch("LeadingEleTightPt", &m_leadingEleTightPt);
  m_mytree->Branch("LeadingEleTightEta", &m_leadingEleTightEta);
  m_mytree->Branch("LeadingEleTightPhi", &m_leadingEleTightPhi);
  m_mytree->Branch("LeadingEleTightE", &m_leadingEleTightE);
  m_mytree->Branch("LeadingEleTightCharge", &m_leadingEleTightCharge);

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
  m_mytree->Branch("HadElChosenTauPt", &m_hadElChosenTauPt);
  m_mytree->Branch("HadElChosenTauEta", &m_hadElChosenTauEta);
  m_mytree->Branch("HadElChosenTauPhi", &m_hadElChosenTauPhi);
  m_mytree->Branch("HadElChosenTauE", &m_hadElChosenTauE);
  m_mytree->Branch("HadElChosenTauM", &m_hadElChosenTauM);
  m_mytree->Branch("HadElChosenTauTruthdR", &m_hadElChosenTauTruthdR);
  m_mytree->Branch("HadElChosenElePt", &m_hadElChosenElePt);
  m_mytree->Branch("HadElChosenEleEta", &m_hadElChosenEleEta);
  m_mytree->Branch("HadElChosenElePhi", &m_hadElChosenElePhi);
  m_mytree->Branch("HadElChosenEleE", &m_hadElChosenEleE);
  m_mytree->Branch("HadElChosenEleM", &m_hadElChosenEleM);
  m_mytree->Branch("HadElChosenEleID", &m_hadElChosenEleID);
  m_mytree->Branch("HadElChosenEleTruthdR", &m_hadElChosenEleTruthdR);
  m_mytree->Branch("HadElChosenTauElePt", &m_hadElChosenTauElePt);
  m_mytree->Branch("HadElChosenTauEleEta", &m_hadElChosenTauEleEta);
  m_mytree->Branch("HadElChosenTauElePhi", &m_hadElChosenTauElePhi);
  m_mytree->Branch("HadElChosenTauEleE", &m_hadElChosenTauEleE);
  m_mytree->Branch("HadElChosenTauEleM", &m_hadElChosenTauEleM);
  m_mytree->Branch("HadElChosenTauEledR", &m_hadElChosenTauEledR);

  // Tau-Mu ditau system
  m_tauMuTightPt = new std::vector<float>();
  m_mytree->Branch("TauMuTightPt", &m_tauMuTightPt);
  m_tauMuTightEta = new std::vector<float>();
  m_mytree->Branch("TauMuTightEta", &m_tauMuTightEta);
  m_tauMuTightPhi = new std::vector<float>();
  m_mytree->Branch("TauMuTightPhi", &m_tauMuTightPhi);
  m_tauMuTightE = new std::vector<float>();
  m_mytree->Branch("TauMuTightE", &m_tauMuTightE);
  m_tauMuTightM = new std::vector<float>();
  m_mytree->Branch("TauMuTightM", &m_tauMuTightM);
  m_tauMuTightdR = new std::vector<float>();
  m_mytree->Branch("TauMuTightdR", &m_tauMuTightdR);

  // Chosen Tau-Mu system
  m_mytree->Branch("HadMuChosenTauPt", &m_hadMuChosenTauPt);
  m_mytree->Branch("HadMuChosenTauEta", &m_hadMuChosenTauEta);
  m_mytree->Branch("HadMuChosenTauPhi", &m_hadMuChosenTauPhi);
  m_mytree->Branch("HadMuChosenTauE", &m_hadMuChosenTauE);
  m_mytree->Branch("HadMuChosenTauM", &m_hadMuChosenTauM);
  m_mytree->Branch("HadMuChosenTauTruthdR", &m_hadMuChosenTauTruthdR);
  m_mytree->Branch("HadMuChosenMuPt", &m_hadMuChosenMuPt);
  m_mytree->Branch("HadMuChosenMuEta", &m_hadMuChosenMuEta);
  m_mytree->Branch("HadMuChosenMuPhi", &m_hadMuChosenMuPhi);
  m_mytree->Branch("HadMuChosenMuE", &m_hadMuChosenMuE);
  m_mytree->Branch("HadMuChosenMuM", &m_hadMuChosenMuM);
  m_mytree->Branch("HadMuChosenMuID", &m_hadMuChosenMuID);
  m_mytree->Branch("HadMuChosenMuTruthdR", &m_hadMuChosenMuTruthdR);
  m_mytree->Branch("HadMuChosenTauMuPt", &m_hadMuChosenTauMuPt);
  m_mytree->Branch("HadMuChosenTauMuEta", &m_hadMuChosenTauMuEta);
  m_mytree->Branch("HadMuChosenTauMuPhi", &m_hadMuChosenTauMuPhi);
  m_mytree->Branch("HadMuChosenTauMuE", &m_hadMuChosenTauMuE);
  m_mytree->Branch("HadMuChosenTauMuM", &m_hadMuChosenTauMuM);
  m_mytree->Branch("HadMuChosenTauMudR", &m_hadMuChosenTauMudR);

  // Resolved analysis values
  m_mytree->Branch("ResolvedMissingMass", &m_resolvedMissingMass);
  m_mytree->Branch("ResolvedDiBJetMass", &m_resolvedDiBJetMass);

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
  m_mytree->Branch("TauLargeRJetMuTightdR", &m_tauLRJetMuTightdR);

  // b large-R jet, jet containing bs
  m_mytree->Branch("BLargeRJetPt", &m_bLRJetPt);
  m_mytree->Branch("BLargeRJetEta", &m_bLRJetEta);
  m_mytree->Branch("BLargeRJetPhi", &m_bLRJetPhi);
  m_mytree->Branch("BLargeRJetE", &m_bLRJetE);
  m_mytree->Branch("BLargeRJetM", &m_bLRJetM);
  m_mytree->Branch("BLargeRJetMuTightdR", &m_bLRJetMuTightdR);

  // Large-R X (= di-Higgs)
  m_mytree->Branch("LargeRXPt", &m_largeRXPt);
  m_mytree->Branch("LargeRXEta", &m_largeRXEta);
  m_mytree->Branch("LargeRXPhi", &m_largeRXPhi);
  m_mytree->Branch("LargeRXE", &m_largeRXE);
  m_mytree->Branch("LargeRXM", &m_largeRXM);

  // Hbb Large-R Jets (used in the Hbb tagging algorithm)
  m_mytree->Branch("NLargeRHbbJet", &m_nLargeRHbbJets);
  m_lRHbbJetPt = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetPt", &m_lRHbbJetPt);
  m_lRHbbJetEta = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetEta", &m_lRHbbJetEta);
  m_lRHbbJetPhi = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetPhi", &m_lRHbbJetPhi);
  m_lRHbbJetE = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetE", &m_lRHbbJetE);
  m_lRHbbJetM = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetM", &m_lRHbbJetM);
  m_lRHbbJetpHiggs = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetpHiggs", &m_lRHbbJetpHiggs);
  m_lRHbbJetpQCD = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetpQCD", &m_lRHbbJetpQCD);
  m_lRHbbJetpTop = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetpTop", &m_lRHbbJetpTop);
  m_lRHbbJetDiscriminant = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetDiscriminant", &m_lRHbbJetDiscriminant);
  m_lRHbbJetTruthdR = new std::vector<float>();
  m_mytree->Branch("LargeRHbbJetTruthdR", &m_lRHbbJetTruthdR);

  m_mytree->Branch("LeadingLargeRHbbJetPt", &m_leadingLRHbbJetPt);
  m_mytree->Branch("LeadingLargeRHbbJetEta", &m_leadingLRHbbJetEta);
  m_mytree->Branch("LeadingLargeRHbbJetPhi", &m_leadingLRHbbJetPhi);
  m_mytree->Branch("LeadingLargeRHbbJetE", &m_leadingLRHbbJetE);
  m_mytree->Branch("LeadingLargeRHbbJetM", &m_leadingLRHbbJetM);

  m_mytree->Branch("SubleadingLargeRHbbJetPt", &m_subleadingLRHbbJetPt);
  m_mytree->Branch("SubleadingLargeRHbbJetEta", &m_subleadingLRHbbJetEta);
  m_mytree->Branch("SubleadingLargeRHbbJetPhi", &m_subleadingLRHbbJetPhi);
  m_mytree->Branch("SubleadingLargeRHbbJetE", &m_subleadingLRHbbJetE);
  m_mytree->Branch("SubleadingLargeRHbbJetM", &m_subleadingLRHbbJetM);

  m_mytree->Branch("MaxDiscLargeRHbbJetDiscriminant", &m_maxDiscLRHbbJetDisc);

  m_mytree->Branch("HadElLargeRHbbJetPt", &m_hadElLRHbbJetPt);
  m_mytree->Branch("HadElLargeRHbbJetEta", &m_hadElLRHbbJetEta);
  m_mytree->Branch("HadElLargeRHbbJetPhi", &m_hadElLRHbbJetPhi);
  m_mytree->Branch("HadElLargeRHbbJetE", &m_hadElLRHbbJetE);
  m_mytree->Branch("HadElLargeRHbbJetM", &m_hadElLRHbbJetM);
  m_mytree->Branch("HadElLargeRHbbJetDiscriminant", &m_hadElLRHbbJetDisc);
  m_mytree->Branch("HadElLargeRHbbJetTruthdR", &m_hadElLRHbbJetTruthdR);

  m_mytree->Branch("HadElDiHiggsPt", &m_hadElDiHiggsPt);
  m_mytree->Branch("HadElDiHiggsEta", &m_hadElDiHiggsEta);
  m_mytree->Branch("HadElDiHiggsPhi", &m_hadElDiHiggsPhi);
  m_mytree->Branch("HadElDiHiggsE", &m_hadElDiHiggsE);
  m_mytree->Branch("HadElDiHiggsM", &m_hadElDiHiggsM);

  m_mytree->Branch("HadMuLargeRHbbJetPt", &m_hadMuLRHbbJetPt);
  m_mytree->Branch("HadMuLargeRHbbJetEta", &m_hadMuLRHbbJetEta);
  m_mytree->Branch("HadMuLargeRHbbJetPhi", &m_hadMuLRHbbJetPhi);
  m_mytree->Branch("HadMuLargeRHbbJetE", &m_hadMuLRHbbJetE);
  m_mytree->Branch("HadMuLargeRHbbJetM", &m_hadMuLRHbbJetM);
  m_mytree->Branch("HadMuLargeRHbbJetDiscriminant", &m_hadMuLRHbbJetDisc);
  m_mytree->Branch("HadMuLargeRHbbJetTruthdR", &m_hadMuLRHbbJetTruthdR);

  m_mytree->Branch("HadMuDiHiggsPt", &m_hadMuDiHiggsPt);
  m_mytree->Branch("HadMuDiHiggsEta", &m_hadMuDiHiggsEta);
  m_mytree->Branch("HadMuDiHiggsPhi", &m_hadMuDiHiggsPhi);
  m_mytree->Branch("HadMuDiHiggsE", &m_hadMuDiHiggsE);
  m_mytree->Branch("HadMuDiHiggsM", &m_hadMuDiHiggsM);

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

  // Hbb tagger
  m_hbbTagTool = std::make_unique<FlavorTagDiscriminants::HbbTag>(FlavorTagDiscriminants::HbbTagConfig("BTagging/202006/Xbb/GhostVR30Rmax4Rmin02TrackJet_BTagging201903/network.json"));

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
  ANA_CHECK( evtStore()->retrieve( jets, m_jetCollectionName ) );

  const xAOD::JetContainer* largerjets = nullptr;
  ANA_CHECK( evtStore()->retrieve( largerjets, m_largeRJetCollectionName ) );

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

  const xAOD::JetContainer* largerjets_hbb = nullptr;
  if (m_isDAOD) {
    ANA_CHECK( evtStore()->retrieve( largerjets_hbb, "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets" ) );
  } 

  m_runNumber = 0;
  m_eventNumber = 0;
  m_uniqueEventID = 0;

  m_pdgTruthMatchType = 0;

  m_truthBPt = 0;
  m_truthBEta = 0;
  m_truthBPhi = 0;
  m_truthBE = 0;

  m_truthAntiBPt = 0;
  m_truthAntiBEta = 0;
  m_truthAntiBPhi = 0;
  m_truthAntiBE = 0;

  m_truthBdR = 0;

  m_truthTauHiggsPt = 0;
  m_truthTauHiggsEta = 0;
  m_truthTauHiggsPhi = 0;
  m_truthTauHiggsE = 0;

  m_truthBHiggsPt = 0;
  m_truthBHiggsEta = 0;
  m_truthBHiggsPhi = 0;
  m_truthBHiggsE = 0;

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
  m_hadElTauTruthdR->clear();
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
  m_hadMuTauTruthdR->clear();
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
  m_leadingTauPt = 0;
  m_leadingTauEta = 0;
  m_leadingTauPhi = 0;
  m_leadingTauE = 0;
  m_tauTruthdR->clear();
  m_tauID->clear();

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
  m_nResolvedTaus = 0;
  m_tauLoosePt->clear();
  m_tauLooseEta->clear();
  m_tauLoosePhi->clear();
  m_tauLooseE->clear();
  m_tauLooseNTracks->clear();
  m_tauLooseCharge->clear();
  m_leadingTauLoosePt = 0;
  m_leadingTauLooseEta = 0;
  m_leadingTauLoosePhi = 0;
  m_leadingTauLooseE = 0;
  m_leadingTauLooseNTracks = 0;
  m_leadingTauLooseCharge = 0;

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
  m_muTruthdR->clear();
  m_muID->clear();

  // Loose muons
  m_nMuonsLoose = 0;
  m_nResolvedMuons = 0;
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
  m_muMediumCharge->clear();
  m_leadingMuMediumPt = 0;
  m_leadingMuMediumEta = 0;
  m_leadingMuMediumPhi = 0;
  m_leadingMuMediumE = 0;
  m_leadingMuMediumCharge = 0;

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
  m_eleTruthdR->clear();
  m_eleID->clear();

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
  m_nResolvedElectrons = 0;
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
  m_eleTightCharge->clear();
  m_leadingEleTightPt = 0;
  m_leadingEleTightEta = 0;
  m_leadingEleTightPhi = 0;
  m_leadingEleTightE = 0;
  m_leadingEleTightCharge = 0;

  // Tau-Ele ditau system
  m_tauEleLoosePt->clear();
  m_tauEleLooseEta->clear();
  m_tauEleLoosePhi->clear();
  m_tauEleLooseE->clear();
  m_tauEleLooseM->clear();
  m_tauEleLoosedR->clear();

  // Chosen tau-ele ditau system
  m_hadElChosenTauPt = 0;
  m_hadElChosenTauEta = 0;
  m_hadElChosenTauPhi = 0;
  m_hadElChosenTauE = 0;
  m_hadElChosenTauM = 0;
  m_hadElChosenTauTruthdR = 0;
  m_hadElChosenElePt = 0;
  m_hadElChosenEleEta = 0;
  m_hadElChosenElePhi = 0;
  m_hadElChosenEleE = 0;
  m_hadElChosenEleM = 0;
  m_hadElChosenEleID = 0;
  m_hadElChosenEleTruthdR = 0;
  m_hadElChosenTauElePt = 0;
  m_hadElChosenTauEleEta = 0;
  m_hadElChosenTauElePhi = 0;
  m_hadElChosenTauEleE = 0;
  m_hadElChosenTauEleM = 0;
  m_hadElChosenTauEledR = 0;

  // Tau-Mu ditau system
  m_tauMuTightPt->clear();
  m_tauMuTightEta->clear();
  m_tauMuTightPhi->clear();
  m_tauMuTightE->clear();
  m_tauMuTightM->clear();
  m_tauMuTightdR->clear();

  // Chosen tau-mu ditau system
  m_hadMuChosenTauPt = 0;
  m_hadMuChosenTauEta = 0;
  m_hadMuChosenTauPhi = 0;
  m_hadMuChosenTauE = 0;
  m_hadMuChosenTauM = 0;
  m_hadMuChosenTauTruthdR = 0;
  m_hadMuChosenMuPt = 0;
  m_hadMuChosenMuEta = 0;
  m_hadMuChosenMuPhi = 0;
  m_hadMuChosenMuE = 0;
  m_hadMuChosenMuM = 0;
  m_hadMuChosenMuID = 0;
  m_hadMuChosenMuTruthdR = 0;
  m_hadMuChosenTauMuPt = 0;
  m_hadMuChosenTauMuEta = 0;
  m_hadMuChosenTauMuPhi = 0;
  m_hadMuChosenTauMuE = 0;
  m_hadMuChosenTauMuM = 0;
  m_hadMuChosenTauMudR = 0;

  m_resolvedMissingMass = 0;
  m_resolvedDiBJetMass = 0;

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

  // Large-R X (= di-Higgs)
  m_largeRXPt = 0;
  m_largeRXEta = 0;
  m_largeRXPhi = 0;
  m_largeRXE = 0;
  m_largeRXM = 0;

  // Hbb large-R Jets
  m_nLargeRHbbJets = 0;
  m_lRHbbJetPt->clear();
  m_lRHbbJetEta->clear();
  m_lRHbbJetPhi->clear();
  m_lRHbbJetE->clear();
  m_lRHbbJetM->clear();
  m_lRHbbJetpHiggs->clear();
  m_lRHbbJetpQCD->clear();
  m_lRHbbJetpTop->clear();
  m_lRHbbJetDiscriminant->clear();
  m_lRHbbJetTruthdR->clear(); 

  m_leadingLRHbbJetPt = 0;
  m_leadingLRHbbJetEta = 0;
  m_leadingLRHbbJetPhi = 0;
  m_leadingLRHbbJetE = 0;
  m_leadingLRHbbJetM = 0;
  m_leadingLRHbbJetDisc = 0;
  
  m_subleadingLRHbbJetPt = 0;
  m_subleadingLRHbbJetEta = 0;
  m_subleadingLRHbbJetPhi = 0;
  m_subleadingLRHbbJetE = 0;
  m_subleadingLRHbbJetM = 0;
  m_subleadingLRHbbJetDisc = 0;

  m_maxDiscLRHbbJetDisc = 0;

  m_hadElLRHbbJetPt = 0;
  m_hadElLRHbbJetEta = 0;
  m_hadElLRHbbJetPhi = 0;
  m_hadElLRHbbJetE = 0;
  m_hadElLRHbbJetM = 0;
  m_hadElLRHbbJetDisc = 0;
  m_hadElLRHbbJetTruthdR = 0;

  m_hadElDiHiggsPt = 0;
  m_hadElDiHiggsEta = 0;
  m_hadElDiHiggsPhi = 0;
  m_hadElDiHiggsE = 0;
  m_hadElDiHiggsM = 0;

  m_hadMuLRHbbJetPt = 0;
  m_hadMuLRHbbJetEta = 0;
  m_hadMuLRHbbJetPhi = 0;
  m_hadMuLRHbbJetE = 0;
  m_hadMuLRHbbJetM = 0;
  m_hadMuLRHbbJetDisc = 0;
  m_hadMuLRHbbJetTruthdR = 0;

  m_hadMuDiHiggsPt = 0;
  m_hadMuDiHiggsEta = 0;
  m_hadMuDiHiggsPhi = 0;
  m_hadMuDiHiggsE = 0;
  m_hadMuDiHiggsM = 0;

  const xAOD::TruthParticle* truth_tau_higgs = nullptr;
  const xAOD::TruthParticle* truth_b_higgs = nullptr;
  const xAOD::TruthParticle* truth_tau = nullptr;
  const xAOD::TruthParticle* truth_anti_tau = nullptr;
  const xAOD::TruthParticle* truth_b = nullptr;
  const xAOD::TruthParticle* truth_anti_b = nullptr;
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
            truth_b = child;
            has_b = 1;
          }
          else if (child_id == -5) {
            truth_anti_b = child;
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
  TLorentzVector truth_b_higgs_p4;

  bool do_truth = 0;
  if (m_isSignal && has_tau_higgs && has_b_higgs && has_hadronic_tau && has_leptonic_tau && has_final_lepton) {
    do_truth = 1;
  }
  
  bool has_visible = 1;

  // Let's do this thing
  m_eventsPassed++;
  m_runNumber = eventInfo->runNumber();
  m_eventNumber = eventInfo->eventNumber();

  if(do_truth)
  {
    truth_final_lepton_p4 = truth_final_lepton->p4();
    truth_hadronic_tau_p4 = truth_hadronic_tau->p4();
    truth_b_higgs_p4 = truth_b_higgs->p4();
  
    int final_lepton_id = truth_final_lepton->pdgId();
    // Values match up with other TruthMatch variable
    // 0 = no match  1 = dihad (N/A here)  2 = hadel  3 = hadmu
    if (final_lepton_id == 11 || final_lepton_id == -11) {
      m_pdgTruthMatchType = 2; 
    } 
    else if ( final_lepton_id == 13 || final_lepton_id == -13) {
      m_pdgTruthMatchType = 3;
    }

    // Truth b
    m_truthBPt = truth_b->pt() / 1000.;
    m_truthBEta = truth_b->eta();
    m_truthBPhi = truth_b->phi();
    m_truthBE = truth_b->e() / 1000.;

    // Truth anti-b
    m_truthAntiBPt = truth_anti_b->pt() / 1000.;
    m_truthAntiBEta = truth_anti_b->eta();
    m_truthAntiBPhi = truth_anti_b->phi();
    m_truthAntiBE = truth_anti_b->e() / 1000.;

    // Truth b dR
    TLorentzVector truth_b_p4 = truth_b->p4();
    TLorentzVector truth_antib_p4 = truth_anti_b->p4();
    m_truthBdR = truth_b_p4.DeltaR(truth_antib_p4);

    // Truth Higgs decaying to taus
    m_truthTauHiggsPt = truth_tau_higgs->pt() / 1000.;
    m_truthTauHiggsEta = truth_tau_higgs->eta();
    m_truthTauHiggsPhi = truth_tau_higgs->phi();
    m_truthTauHiggsE = truth_tau_higgs->e() / 1000.;

    // Truth Higgs decaying to b
    m_truthBHiggsPt = truth_b_higgs->pt() / 1000.;
    m_truthBHiggsEta = truth_b_higgs->eta();
    m_truthBHiggsPhi = truth_b_higgs->phi();
    m_truthBHiggsE = truth_b_higgs->e() / 1000.;

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
    float hadel_tau_pt = acc_tau_pt(*hadelditau) / 1000.;
    float hadel_tau_eta = acc_tau_eta(*hadelditau);
    float hadel_tau_phi = acc_tau_phi(*hadelditau);
    float hadel_tau_e = acc_tau_E(*hadelditau) / 1000.;
    m_hadElTauPt->push_back(hadel_tau_pt);
    m_hadElTauEta->push_back(hadel_tau_eta);
    m_hadElTauPhi->push_back(hadel_tau_phi);
    m_hadElTauE->push_back(hadel_tau_e);
    TLorentzVector hadel_tau_p4;
    hadel_tau_p4.SetPtEtaPhiE(hadel_tau_pt, hadel_tau_eta, hadel_tau_phi, hadel_tau_e);
    if (do_truth && has_visible) {
      float hadel_tau_truth_dr = hadel_tau_p4.DeltaR(vis_had_tau_p4);
      m_hadElTauTruthdR->push_back(hadel_tau_truth_dr);
    }

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

    if(do_truth)
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
    float hadmu_tau_pt = acc_tau_pt(*hadmuditau) / 1000.;
    float hadmu_tau_eta = acc_tau_eta(*hadmuditau);
    float hadmu_tau_phi = acc_tau_phi(*hadmuditau);
    float hadmu_tau_e = acc_tau_E(*hadmuditau) / 1000.;
    m_hadMuTauPt->push_back(hadmu_tau_pt);
    m_hadMuTauEta->push_back(hadmu_tau_eta);
    m_hadMuTauPhi->push_back(hadmu_tau_phi);
    m_hadMuTauE->push_back(hadmu_tau_e);
    TLorentzVector hadmu_tau_p4;
    hadmu_tau_p4.SetPtEtaPhiE(hadmu_tau_pt, hadmu_tau_eta, hadmu_tau_phi, hadmu_tau_e);
    if (do_truth && has_visible) {
      float hadmu_tau_truth_dr = hadmu_tau_p4.DeltaR(vis_had_tau_p4);
      m_hadMuTauTruthdR->push_back(hadmu_tau_truth_dr);
    }

    static const SG::AuxElement::Accessor<float> acc_el_pt ("muon_pt");
    static const SG::AuxElement::Accessor<float> acc_el_eta ("muon_eta");
    static const SG::AuxElement::Accessor<float> acc_el_phi ("muon_phi");
    static const SG::AuxElement::Accessor<float> acc_el_E ("muon_E");

    m_nHadMuMuons++;
    m_hadMuMuonPt->push_back(acc_el_pt(*hadmuditau) / 1000.);
    m_hadMuMuonEta->push_back(acc_el_eta(*hadmuditau));
    m_hadMuMuonPhi->push_back(acc_el_phi(*hadmuditau));
    m_hadMuMuonE->push_back(acc_el_E(*hadmuditau) / 1000.);

    if (m_isDAOD) {
      static const SG::AuxElement::Accessor<float> acc_DeltaR ("DeltaR");
      m_hadMuDiTaudR->push_back(acc_DeltaR(*hadmuditau));

      CHECK(m_hadMuDiTauIDVarCalculator->execute(*hadmuditau));

      CHECK(m_hadMuDiTauDiscrTool->execute(*hadmuditau));
      CHECK(m_hadMuDiTauWPDecorator->execute(*hadmuditau));
      double bdt = hadmuditau->auxdata<double>("JetBDT");
      double flat_bdt = hadmuditau->auxdata<double>("JetBDTFlat");
      m_hadMuDiTauBDTScore->push_back(bdt);
      m_hadMuDiTauFlatBDTScore->push_back(flat_bdt);

      if (bdt > m_bestHadMuDiTauBDTScore || m_bestHadMuDiTauBDTScore == -1)
      {
        m_bestHadMuDiTauBDTScore = bdt;
      }
    }

    if(do_truth)
    {
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
  float min_tau_truth_dr = -1;
  for (auto tau: *taus) {
    m_nTaus++;

    float tau_pt = tau->pt() / 1000.;
    float tau_eta = tau->eta();
    float tau_phi = tau->phi();
    float tau_e = tau->e() / 1000.;
    unsigned int tau_ntracks = tau->nTracks();
    int tau_charge = tau->charge();
    TLorentzVector tau_p4 = tau->p4();

    // Calculate dR of closest reco tau to truth
    if (do_truth && has_visible) {
      float truth_dr = tau_p4.DeltaR(vis_had_tau_p4);
      m_tauTruthdR->push_back(truth_dr);
      if (min_tau_truth_dr == -1 || truth_dr < min_tau_truth_dr) {
        min_tau_truth_dr = truth_dr;
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
    // Variable holding tau ID (0=None, 1=VLoose, 2=Loose, 3=Med, 4=Tight)
    unsigned int tau_id = 0;
    // Very Loose taus
    if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigVeryLoose)) {
      m_nTausVeryLoose++;
      tau_id = 1;
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

      float abs_tau_eta = std::abs(tau_eta);
      if ((tau_pt > 20) && (abs_tau_eta < 2.5) && ((abs_tau_eta < 1.37) || (abs_tau_eta > 1.52)) && ((tau_ntracks == 1) || (tau_ntracks == 3)) && (std::abs(tau_charge) == 1)) {
        m_nResolvedTaus++;
      }

      tau_id = 2;
      m_tauLoosePt->push_back(tau_pt);
      m_tauLooseEta->push_back(tau_eta);
      m_tauLoosePhi->push_back(tau_phi);
      m_tauLooseE->push_back(tau_e);
      m_tauLooseNTracks->push_back(tau_ntracks);
      m_tauLooseCharge->push_back(tau_charge);
      if (tau_pt >= m_leadingTauLoosePt) {
        leading_loose_tau = tau;
        m_leadingTauLoosePt = tau_pt;
        m_leadingTauLooseEta = tau_eta;
        m_leadingTauLoosePhi = tau_phi;
        m_leadingTauLooseE = tau_e;
        m_leadingTauLooseNTracks = tau_ntracks;
        m_leadingTauLooseCharge = tau_charge;
      }
    }
    // Medium taus
    if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigMedium)) {
      m_nTausMedium++;
      tau_id = 3;
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
      tau_id = 4;
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
    m_tauID->push_back(tau_id);
  }

  m_tauRecoTruthMindR = min_tau_truth_dr;

  // Reconstructed muons
  const xAOD::Muon* leading_muon = nullptr;
  const xAOD::Muon* leading_loose_muon = nullptr;
  const xAOD::Muon* leading_medium_muon = nullptr;
  const xAOD::Muon* leading_tight_muon = nullptr;
  float min_mu_truth_dr = -1;
  for (auto muon: *muons) {
    m_nMuons++;

    // Calculate dR of closest reco muon to truth
    if (do_truth) {
      TLorentzVector muon_p4 = muon->p4();
      float truth_dr = muon_p4.DeltaR(truth_final_lepton_p4);
      m_muTruthdR->push_back(truth_dr);
      if (min_mu_truth_dr == -1 || truth_dr < min_mu_truth_dr) {
        min_mu_truth_dr = truth_dr;
      }
    }

    float muon_pt = muon->pt() / 1000.;
    float muon_eta = muon->eta();
    float muon_phi = muon->phi();
    float muon_e = muon->e() / 1000.;
    int muon_charge = muon->charge();
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
    bool resolved_muon_pteta = false;
    if ((muon_pt > 7) && (std::abs(muon_eta) < 2.7)) {
      resolved_muon_pteta = true;
    }
    // Muon quality 0=Tight 1=Medium 2=Loose 3=VeryLoose(all)
    unsigned int muQuality = muon->quality();
    m_muID->push_back(muQuality);
    // Tight muon, fill for all WPs
    if (muQuality == 0) {
      // Loose muon
      m_nMuonsLoose++;
      if (resolved_muon_pteta) {
        m_nResolvedMuons++;
      }
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
      m_muMediumCharge->push_back(muon_charge);
      if (muon_pt >= m_leadingMuMediumPt) {
        leading_medium_muon = muon;
        m_leadingMuMediumPt = muon_pt;
        m_leadingMuMediumEta = muon_eta;
        m_leadingMuMediumPhi = muon_phi;
        m_leadingMuMediumE = muon_e;
        m_leadingMuMediumCharge = muon_charge;
      }
      // Tight muon
      m_nMuonsTight++;
      m_muTightPt->push_back(muon_pt);
      m_muTightEta->push_back(muon_eta);
      m_muTightPhi->push_back(muon_phi);
      m_muTightE->push_back(muon_e);
      if (muon_pt >= m_leadingMuTightPt) {
        leading_tight_muon = muon;
        m_leadingMuTightPt = muon_pt;
        m_leadingMuTightEta = muon_eta;
        m_leadingMuTightPhi = muon_phi;
        m_leadingMuTightE = muon_e;
      }
    // Medium muon, fill Medium and Loose
    } else if (muQuality == 1) {
      // Loose muon
      m_nMuonsLoose++;
      if (resolved_muon_pteta) {
        m_nResolvedMuons++;
      }
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
      m_muMediumCharge->push_back(muon_charge);
      if (muon_pt >= m_leadingMuMediumPt) {
        leading_medium_muon = muon;
        m_leadingMuMediumPt = muon_pt;
        m_leadingMuMediumEta = muon_eta;
        m_leadingMuMediumPhi = muon_phi;
        m_leadingMuMediumE = muon_e;
        m_leadingMuMediumCharge = muon_charge;
      }
    // Loose muon, just Loose
    } else if (muQuality == 2) {
      // Loose muon
      m_nMuonsLoose++;
      if (resolved_muon_pteta) {
        m_nResolvedMuons++;
      }
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
  std::vector<const xAOD::Electron*> loose_electrons;
  float min_ele_truth_dr = -1;
  for (auto electron: *electrons) {
    m_nElectrons++;

    // Calculate dR of closest reco electron to truth
    if (do_truth) {
      TLorentzVector electron_p4 = electron->p4();
      float truth_dr = electron_p4.DeltaR(truth_final_lepton_p4);
      m_eleTruthdR->push_back(truth_dr);
      if (min_ele_truth_dr == -1 || truth_dr < min_ele_truth_dr) {
        min_ele_truth_dr = truth_dr;
      }
    }

    float electron_pt = electron->pt() / 1000.;
    float electron_eta = electron->eta();
    float electron_phi = electron->phi();
    float electron_e = electron->e() / 1000.;
    int electron_charge = electron->charge();
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
    // Variable holding electron ID (0=None, 1=VLoose, 2=Loose, 3=Med, 4=Tight)
    unsigned int ele_id = 0;
    if (static_cast<int>(m_checkEleVeryLooseLH->accept(electron))) {
      m_nElectronsVeryLoose++;
      ele_id = 1;
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
      ele_id = 2;
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
      if ((electron_pt > 7) && ((std::abs(electron_eta) < 2.47) && ((std::abs(electron_eta) < 1.37) || (std::abs(electron_eta) > 1.52)))) {
        m_nResolvedElectrons++;
      }
    }
    if (static_cast<int>(m_checkEleMediumLH->accept(electron))) {
      m_nElectronsMedium++;
      ele_id = 3;
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
      ele_id = 4;
      m_eleTightPt->push_back(electron_pt);
      m_eleTightEta->push_back(electron_eta);
      m_eleTightPhi->push_back(electron_phi);
      m_eleTightE->push_back(electron_e);
      m_eleTightCharge->push_back(electron_charge);
      if (electron_pt >= m_leadingEleTightPt) {
        leading_tight_electron = electron;
        m_leadingEleTightPt = electron_pt;
        m_leadingEleTightEta = electron_eta;
        m_leadingEleTightPhi = electron_phi;
        m_leadingEleTightE = electron_e;
        m_leadingEleTightCharge = electron_charge;
      }
    }
    m_eleID->push_back(ele_id);
  }

  m_eleRecoTruthMindR = min_ele_truth_dr;

  // Reconstructed jets
  const xAOD::Jet* leading_btag_jet;
  const xAOD::Jet* subleading_btag_jet;
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
        subleading_btag_jet = jet;
        m_subleadingBTagJetPt = m_leadingBTagJetPt;
        m_subleadingBTagJetEta = m_leadingBTagJetEta;
        m_subleadingBTagJetPhi = m_leadingBTagJetPhi;
        m_subleadingBTagJetE = m_leadingBTagJetE;
        leading_btag_jet = jet;
        m_leadingBTagJetPt = jet_pt;
        m_leadingBTagJetEta = jet_eta;
        m_leadingBTagJetPhi = jet_phi;
        m_leadingBTagJetE = jet_e;
      }
      else if (jet_pt >= m_subleadingBTagJetPt) {
        subleading_btag_jet = jet;
        m_subleadingBTagJetPt = jet_pt;
        m_subleadingBTagJetEta = jet_eta;
        m_subleadingBTagJetPhi = jet_phi;
        m_subleadingBTagJetE = jet_e;
      }
    }
  }

  // b-jet system mass
  if (m_nBTagJets >= 2) {
    TLorentzVector leading_bjet_p4 = leading_btag_jet->p4();
    TLorentzVector subleading_bjet_p4 = subleading_btag_jet->p4();
    TLorentzVector dib_p4 = leading_bjet_p4 + subleading_bjet_p4;
    m_resolvedDiBJetMass = dib_p4.M() / 1000.;
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

  const xAOD::Jet* leading_hbb_largerjet;
  const xAOD::Jet* subleading_hbb_largerjet;
  if (m_isDAOD) {
    float max_hbb_discriminant = -10;
    for (auto largerjet_hbb: *largerjets_hbb) {
      m_nLargeRHbbJets++;
      float largerjet_hbb_pt = largerjet_hbb->pt() / 1000.;
      float largerjet_hbb_eta = largerjet_hbb->eta();
      float largerjet_hbb_phi = largerjet_hbb->phi();
      float largerjet_hbb_e = largerjet_hbb->e() / 1000.;
      float largerjet_hbb_m = largerjet_hbb->m() / 1000.;

      // Hbb calculations
      m_hbbTagTool->decorate(*largerjet_hbb);
      float p_higgs = largerjet_hbb->auxdecor<float>("Xbb202006_Higgs");
      float p_qcd = largerjet_hbb->auxdecor<float>("Xbb202006_QCD");
      float p_top = largerjet_hbb->auxdecor<float>("Xbb202006_Top");
      m_lRHbbJetpHiggs->push_back(p_higgs);
      m_lRHbbJetpQCD->push_back(p_qcd); 
      m_lRHbbJetpTop->push_back(p_top); 
      float largerjet_hbb_discriminant = calculate_hbb_discriminant(largerjet_hbb);

      m_lRHbbJetPt->push_back(largerjet_hbb_pt);
      m_lRHbbJetEta->push_back(largerjet_hbb_eta);
      m_lRHbbJetPhi->push_back(largerjet_hbb_phi);
      m_lRHbbJetE->push_back(largerjet_hbb_e);
      m_lRHbbJetM->push_back(largerjet_hbb_m);
      m_lRHbbJetDiscriminant->push_back(largerjet_hbb_discriminant);

      // Fill maximum Hbb discriminant value, -10 chosen as default below any real value
      if (largerjet_hbb_discriminant > max_hbb_discriminant) {
        max_hbb_discriminant = largerjet_hbb_discriminant;
      }

      // Fill leading and subleading values
      if (largerjet_hbb_pt >= m_leadingLRHbbJetPt) {
        // Leading jet bumped down to subleading
        subleading_hbb_largerjet = leading_hbb_largerjet;
        m_subleadingLRHbbJetPt = m_leadingLRHbbJetPt;
        m_subleadingLRHbbJetEta = m_leadingLRHbbJetEta;
        m_subleadingLRHbbJetPhi = m_leadingLRHbbJetPhi;
        m_subleadingLRHbbJetE = m_leadingLRHbbJetE;
        m_subleadingLRHbbJetM = m_leadingLRHbbJetM;
        m_subleadingLRHbbJetDisc = m_leadingLRHbbJetDisc;
        // New jet moved to leading
        leading_hbb_largerjet = largerjet_hbb;
        m_leadingLRHbbJetPt = largerjet_hbb_pt;
        m_leadingLRHbbJetEta = largerjet_hbb_eta;
        m_leadingLRHbbJetPhi = largerjet_hbb_phi;
        m_leadingLRHbbJetE = largerjet_hbb_e;
        m_leadingLRHbbJetM = largerjet_hbb_m;
        m_leadingLRHbbJetDisc = largerjet_hbb_discriminant;
      }
      else if (largerjet_hbb_pt >= m_subleadingLRHbbJetPt) {
        subleading_hbb_largerjet = largerjet_hbb;
        m_subleadingLRHbbJetPt = largerjet_hbb_pt;
        m_subleadingLRHbbJetEta = largerjet_hbb_eta;
        m_subleadingLRHbbJetPhi = largerjet_hbb_phi;
        m_subleadingLRHbbJetE = largerjet_hbb_e;
        m_subleadingLRHbbJetM = largerjet_hbb_m;
        m_subleadingLRHbbJetDisc = largerjet_hbb_discriminant;
      }

      if (has_b_higgs) {
        TLorentzVector jet_p4 = largerjet_hbb->p4();
        TLorentzVector truth_higgs_p4 = truth_b_higgs->p4();
        float jet_higgs_dr = jet_p4.DeltaR(truth_higgs_p4);
        m_lRHbbJetTruthdR->push_back(jet_higgs_dr); 
      }
    }

    m_maxDiscLRHbbJetDisc = max_hbb_discriminant;
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

  if (leading_tight_muon && m_nTaus > 0) {
    TLorentzVector leading_tight_muon_p4 = leading_tight_muon->p4();

    for (auto tau: *taus) {
      TLorentzVector tau_p4 = tau->p4();
      TLorentzVector tau_tight_muon_p4 = tau_p4 + leading_tight_muon_p4;

      m_tauMuTightPt->push_back(tau_tight_muon_p4.Pt() / 1000.);
      m_tauMuTightEta->push_back(tau_tight_muon_p4.Eta());
      m_tauMuTightPhi->push_back(tau_tight_muon_p4.Phi());
      m_tauMuTightE->push_back(tau_tight_muon_p4.E() / 1000.);
      m_tauMuTightM->push_back(tau_tight_muon_p4.M() / 1000.);
      m_tauMuTightdR->push_back(tau_p4.DeltaR(leading_tight_muon_p4));
    }

    if (m_nLargeRJets > 1) {
      TLorentzVector leading_largerjet_p4 = leading_largerjet->p4();
      TLorentzVector subleading_largerjet_p4 = subleading_largerjet->p4();

      float mu_tight_leading_largerjet_dr = leading_tight_muon_p4.DeltaR(leading_largerjet_p4);
      float mu_tight_subleading_largerjet_dr = leading_tight_muon_p4.DeltaR(subleading_largerjet_p4);
      TLorentzVector tau_largerjet_p4;
      // If the loose electron is closest to the leading large-R jet, then it is the tau jet
      if (mu_tight_leading_largerjet_dr < mu_tight_subleading_largerjet_dr) {
        tau_largerjet_p4 = leading_largerjet_p4;
        m_tauLRJetPt = m_leadingLRJetPt;
        m_tauLRJetEta = m_leadingLRJetEta;
        m_tauLRJetPhi = m_leadingLRJetPhi;
        m_tauLRJetE = m_leadingLRJetE;
        m_tauLRJetM = m_leadingLRJetM;
        m_tauLRJetMuTightdR = mu_tight_leading_largerjet_dr;
        m_bLRJetPt = m_subleadingLRJetPt;
        m_bLRJetEta = m_subleadingLRJetEta;
        m_bLRJetPhi = m_subleadingLRJetPhi;
        m_bLRJetE = m_subleadingLRJetE;
        m_bLRJetM = m_subleadingLRJetM;
        m_bLRJetMuTightdR = mu_tight_subleading_largerjet_dr;
      // If closest to the subleading large-R jet, then it is the tau jet
      }
      else {
        m_bLRJetPt = m_leadingLRJetPt;
        m_bLRJetEta = m_leadingLRJetEta;
        m_bLRJetPhi = m_leadingLRJetPhi;
        m_bLRJetE = m_leadingLRJetE;
        m_bLRJetM = m_leadingLRJetM;
        m_bLRJetMuTightdR = mu_tight_leading_largerjet_dr;
        tau_largerjet_p4 = subleading_largerjet_p4;
        m_tauLRJetPt = m_subleadingLRJetPt;
        m_tauLRJetEta = m_subleadingLRJetEta;
        m_tauLRJetPhi = m_subleadingLRJetPhi;
        m_tauLRJetE = m_subleadingLRJetE;
        m_tauLRJetM = m_subleadingLRJetM;
        m_tauLRJetMuTightdR = mu_tight_subleading_largerjet_dr;
      }
    }      
  }

  // Define the chosen tau, electron, and muon to make up ditau system. First electron then muon.
  TLorentzVector chosen_hadel_tau_p4;
  TLorentzVector chosen_hadmu_tau_p4;
  unsigned int chosen_electron_id;
  TLorentzVector chosen_electron_p4;
  unsigned int chosen_muon_id;
  TLorentzVector chosen_muon_p4;
  bool set_electron = 0;
  bool set_muon = 0;
  bool set_hadel_tau = 0;
  bool set_hadmu_tau = 0;

  // First find leading loose electron if it exists
  if (m_nElectronsLoose > 0) {
    auto el = leading_loose_electron;
    chosen_electron_p4.SetPtEtaPhiE(el->pt(), el->eta(), el->phi(), el->e());
    // Chosen electron ID (0=None, 1=VLoose, 2=Loose, 3=Med, 4=Tight)
    if (static_cast<int>(m_checkEleVeryLooseLH->accept(el))) {
      chosen_electron_id = 1;
    }
    if (static_cast<int>(m_checkEleLooseLH->accept(el))) {
      chosen_electron_id = 2;
    }
    if (static_cast<int>(m_checkEleMediumLH->accept(el))) {
      chosen_electron_id = 3;
    }
    if (static_cast<int>(m_checkEleTightLH->accept(el))) {
      chosen_electron_id = 4;
    }
    set_electron = 1;
  }

  // Find closest tau to chosen electron
  if (set_electron && (m_nTaus > 0)) {
    float min_dr = 0;
    for (auto tau: *taus) {
      TLorentzVector tau_p4 = tau->p4();
      float dr = chosen_electron_p4.DeltaR(tau_p4);
      if ((dr < min_dr) || (min_dr == 0)) {
        chosen_hadel_tau_p4.SetPtEtaPhiE(tau->pt(), tau->eta(), tau->phi(), tau->e());
        set_hadel_tau = 1;
        min_dr = dr;
      }
    }
  }

  // If dR between electron and tau is < 0.1, then look for another in 0.1 < dR < 1 and pt > 20 GeV
  float chosen_dr = 0;
  unsigned int switched_taus = 0;
  if (set_electron && set_hadel_tau) {
    chosen_dr = chosen_electron_p4.DeltaR(chosen_hadel_tau_p4);
    if (chosen_dr < 0.1) {
      float min_dr = 0;
      for (auto tau: *taus) {
        TLorentzVector tau_p4 = tau->p4();
        float tau_pt = tau->pt() / 1000.;
        float dr = chosen_electron_p4.DeltaR(tau_p4);
        if (((dr < min_dr) || (min_dr == 0)) && tau_pt > 20. && (dr > 0.1) && (dr < 1.0)) {
          chosen_hadel_tau_p4.SetPtEtaPhiE(tau->pt(), tau->eta(), tau->phi(), tau->e());
          set_hadel_tau = 1;
          min_dr = dr;
          switched_taus = 1;
        }
      }
    }
  } 

  // If no tau found but another loose electron within 1.0 switch to that
  if (set_electron && set_hadel_tau && (chosen_dr < 0.1) && !switched_taus) {
    float min_dr = 0;
    for (auto loose_electron: loose_electrons) {
      TLorentzVector loose_electron_p4 = loose_electron->p4();
      float dr = chosen_electron_p4.DeltaR(loose_electron_p4);
      if (((dr < min_dr) || min_dr == 0) && (dr > 0.1) && (dr < 1.0)) {
        auto el = loose_electron;
        chosen_electron_p4.SetPtEtaPhiE(el->pt(), el->eta(), el->phi(), el->e());
        // Chosen electron ID (0=None, 1=VLoose, 2=Loose, 3=Med, 4=Tight)
        if (static_cast<int>(m_checkEleVeryLooseLH->accept(el))) {
          chosen_electron_id = 1;
        }
        if (static_cast<int>(m_checkEleLooseLH->accept(el))) {
          chosen_electron_id = 2;
        }
        if (static_cast<int>(m_checkEleMediumLH->accept(el))) {
          chosen_electron_id = 3;
        }
        if (static_cast<int>(m_checkEleTightLH->accept(el))) {
          chosen_electron_id = 4;
        }
        min_dr = dr;
      }
    } 
  }

  // If electron and tau still within 0.2 then look to ditau with overlapping electron
  if (set_electron && set_hadel_tau) {
    chosen_dr = chosen_electron_p4.DeltaR(chosen_hadel_tau_p4);
  }
  if (set_electron && set_hadel_tau && (chosen_dr < 0.2)) {
    float max_pt = 0;
    static const SG::AuxElement::Accessor<float> acc_el_pt ("electron_pt");
    static const SG::AuxElement::Accessor<float> acc_el_eta ("electron_eta");
    static const SG::AuxElement::Accessor<float> acc_el_phi ("electron_phi");
    static const SG::AuxElement::Accessor<float> acc_el_E ("electron_E");
    static const SG::AuxElement::Accessor<float> acc_tau_pt ("tau_pt");
    static const SG::AuxElement::Accessor<float> acc_tau_eta ("tau_eta");
    static const SG::AuxElement::Accessor<float> acc_tau_phi ("tau_phi");
    static const SG::AuxElement::Accessor<float> acc_tau_E ("tau_E");
    for (auto ditau: *hadelditaus) {
      float hadel_electron_pt = acc_el_pt(*ditau);
      float hadel_electron_eta = acc_el_eta(*ditau);
      float hadel_electron_phi = acc_el_phi(*ditau);
      float hadel_electron_e = acc_el_E(*ditau);
      TLorentzVector hadel_electron_p4;
      hadel_electron_p4.SetPtEtaPhiE(hadel_electron_pt, hadel_electron_eta, hadel_electron_phi, hadel_electron_e);
      float electrons_dr = chosen_electron_p4.DeltaR(hadel_electron_p4);
      double ditau_pt = ditau->pt();
      // Update chosen tau if its pt is greatest and electrons overlap
      if (((ditau_pt > max_pt) || max_pt == 0) && (electrons_dr < 0.1)) {
        float hadel_tau_pt = acc_tau_pt(*ditau);
        float hadel_tau_eta = acc_tau_eta(*ditau);
        float hadel_tau_phi = acc_tau_phi(*ditau);
        float hadel_tau_e = acc_tau_E(*ditau);
        TLorentzVector hadel_tau_p4;
        hadel_tau_p4.SetPtEtaPhiE(hadel_tau_pt, hadel_tau_eta, hadel_tau_phi, hadel_tau_e);
        chosen_hadel_tau_p4 = hadel_tau_p4;
        set_hadel_tau = 1;
        max_pt = ditau_pt;
      }
    }
  }

  // First find leading tight muon if it exists
  if (m_nMuonsTight > 0) {
    auto mu = leading_tight_muon;
    chosen_muon_p4.SetPtEtaPhiE(mu->pt(), mu->eta(), mu->phi(), mu->e());
    chosen_muon_id = mu->quality();
    set_muon = 1;
  }

  // Find leading tau to chosen muon within 0.2 < dR < 1
  if (set_muon && (m_nTaus > 0)) {
    float max_pt = 0;
    for (auto tau: *taus) {
      TLorentzVector tau_p4 = tau->p4();
      float tau_pt = tau->pt() / 1000.;
      float dr = chosen_muon_p4.DeltaR(tau_p4);
      // Only consider 0.2 < dR < 1 range
      if ((dr < 0.2) || (dr > 1)) {
        continue;
      }
      // Find leading tau within constraints
      if ((tau_pt > max_pt) || (max_pt == 0)) {
        chosen_hadmu_tau_p4.SetPtEtaPhiE(tau->pt(), tau->eta(), tau->phi(), tau->e());
        set_hadmu_tau = 1;
        max_pt = tau_pt;
      }
    }
  }    

  // If no tau found then use closest to chosen muon
  if (set_muon && !set_hadmu_tau && (m_nTaus > 0)) {
    float min_dr = 0;
    for (auto tau: *taus) {
      TLorentzVector tau_p4 = tau->p4();
      float dr = chosen_muon_p4.DeltaR(tau_p4);
      if ((dr < min_dr) || (min_dr == 0)) {
        chosen_hadmu_tau_p4.SetPtEtaPhiE(tau->pt(), tau->eta(), tau->phi(), tau->e());
        set_hadmu_tau = 1;
        min_dr = dr;
      }
    }
  }
  
  if (set_hadel_tau) {
    m_hadElChosenTauPt = chosen_hadel_tau_p4.Pt() / 1000.; 
    m_hadElChosenTauEta = chosen_hadel_tau_p4.Eta(); 
    m_hadElChosenTauPhi = chosen_hadel_tau_p4.Phi(); 
    m_hadElChosenTauE = chosen_hadel_tau_p4.E() / 1000.; 
    m_hadElChosenTauM = chosen_hadel_tau_p4.M() / 1000.; 
    if (do_truth && has_visible) {
      float dr = chosen_hadel_tau_p4.DeltaR(vis_had_tau_p4);
      m_hadElChosenTauTruthdR = dr;
    }
  }

  if (set_hadmu_tau) {
    m_hadMuChosenTauPt = chosen_hadmu_tau_p4.Pt() / 1000.; 
    m_hadMuChosenTauEta = chosen_hadmu_tau_p4.Eta(); 
    m_hadMuChosenTauPhi = chosen_hadmu_tau_p4.Phi(); 
    m_hadMuChosenTauE = chosen_hadmu_tau_p4.E() / 1000.; 
    m_hadMuChosenTauM = chosen_hadmu_tau_p4.M() / 1000.; 
    if (do_truth && has_visible) {
      float dr = chosen_hadmu_tau_p4.DeltaR(vis_had_tau_p4);
      m_hadMuChosenTauTruthdR = dr;
    }
  }

  if (set_electron) {
    m_hadElChosenElePt = chosen_electron_p4.Pt() / 1000.; 
    m_hadElChosenEleEta = chosen_electron_p4.Eta(); 
    m_hadElChosenElePhi = chosen_electron_p4.Phi(); 
    m_hadElChosenEleE = chosen_electron_p4.E() / 1000.; 
    m_hadElChosenEleM = chosen_electron_p4.M() / 1000.; 
    if (do_truth && has_final_lepton) {
      float dr = chosen_electron_p4.DeltaR(truth_final_lepton_p4);
      m_hadElChosenEleTruthdR = dr;
    }
    m_hadElChosenEleID = chosen_electron_id;
  }

  if (set_muon) {
    m_hadMuChosenMuPt = chosen_muon_p4.Pt() / 1000;
    m_hadMuChosenMuEta = chosen_muon_p4.Eta();
    m_hadMuChosenMuPhi = chosen_muon_p4.Phi();
    m_hadMuChosenMuE = chosen_muon_p4.E() / 1000;
    m_hadMuChosenMuM = chosen_muon_p4.M() / 1000;
    if (do_truth && has_final_lepton) {
      float dr = chosen_muon_p4.DeltaR(truth_final_lepton_p4);
      m_hadMuChosenMuTruthdR = dr;
    }
    // This is to change the athena ID (0=Tight ..) to 4=Tight so that 0 remains if no chosen muon found
    m_hadMuChosenMuID = 4 - chosen_muon_id;
  }

  TLorentzVector chosen_hadel_system_p4;
  if (set_hadel_tau && set_electron) {
    chosen_hadel_system_p4 = chosen_hadel_tau_p4 + chosen_electron_p4;
    m_hadElChosenTauElePt = chosen_hadel_system_p4.Pt() / 1000.; 
    m_hadElChosenTauEleEta = chosen_hadel_system_p4.Eta(); 
    m_hadElChosenTauElePhi = chosen_hadel_system_p4.Phi(); 
    m_hadElChosenTauEleE = chosen_hadel_system_p4.E() / 1000.; 
    m_hadElChosenTauEleM = chosen_hadel_system_p4.M() / 1000.; 
    m_hadElChosenTauEledR = chosen_hadel_tau_p4.DeltaR(chosen_electron_p4);
  }

  TLorentzVector chosen_hadmu_system_p4;
  if (set_hadmu_tau && set_muon) {
    chosen_hadmu_system_p4 = chosen_hadmu_tau_p4 + chosen_muon_p4;
    m_hadMuChosenTauMuPt = chosen_hadmu_system_p4.Pt() / 1000.; 
    m_hadMuChosenTauMuEta = chosen_hadmu_system_p4.Eta(); 
    m_hadMuChosenTauMuPhi = chosen_hadmu_system_p4.Phi(); 
    m_hadMuChosenTauMuE = chosen_hadmu_system_p4.E() / 1000.; 
    m_hadMuChosenTauMuM = chosen_hadmu_system_p4.M() / 1000.; 
    m_hadMuChosenTauMudR = chosen_hadmu_tau_p4.DeltaR(chosen_muon_p4);
  }

  if (m_isDAOD) {
    // Fill offlep Hbb jet ie of the leading and subleading, the furthest from chosen lepton
    
    TLorentzVector hadel_chosen_hbbjet_p4;
    TLorentzVector hadmu_chosen_hbbjet_p4;

    if (m_nLargeRHbbJets > 1) {
      TLorentzVector leading_hbbjet_p4 = leading_hbb_largerjet->p4();
      TLorentzVector subleading_hbbjet_p4 = subleading_hbb_largerjet->p4();
      float leading_hbbjet_truth_dr = leading_hbbjet_p4.DeltaR(truth_b_higgs_p4);
      float subleading_hbbjet_truth_dr = subleading_hbbjet_p4.DeltaR(truth_b_higgs_p4);
    
      // Electrons
      float leading_hbbjet_ele_dr = leading_hbbjet_p4.DeltaR(chosen_electron_p4);
      float subleading_hbbjet_ele_dr = subleading_hbbjet_p4.DeltaR(chosen_electron_p4);
      if (leading_hbbjet_ele_dr >= subleading_hbbjet_ele_dr) {
        hadel_chosen_hbbjet_p4 = leading_hbbjet_p4;
        m_hadElLRHbbJetPt = m_leadingLRHbbJetPt;
        m_hadElLRHbbJetEta = m_leadingLRHbbJetEta;
        m_hadElLRHbbJetPhi = m_leadingLRHbbJetPhi;
        m_hadElLRHbbJetE = m_leadingLRHbbJetE;
        m_hadElLRHbbJetM = m_leadingLRHbbJetM;
        m_hadElLRHbbJetDisc = m_leadingLRHbbJetDisc;
        m_hadElLRHbbJetTruthdR = leading_hbbjet_truth_dr;
      }
      else {
        hadel_chosen_hbbjet_p4 = subleading_hbbjet_p4;
        m_hadElLRHbbJetPt = m_subleadingLRHbbJetPt;
        m_hadElLRHbbJetEta = m_subleadingLRHbbJetEta;
        m_hadElLRHbbJetPhi = m_subleadingLRHbbJetPhi;
        m_hadElLRHbbJetE = m_subleadingLRHbbJetE;
        m_hadElLRHbbJetM = m_subleadingLRHbbJetM;
        m_hadElLRHbbJetDisc = m_subleadingLRHbbJetDisc;
        m_hadElLRHbbJetTruthdR = subleading_hbbjet_truth_dr;
      }
      

      // Muons
      float leading_hbbjet_mu_dr = leading_hbbjet_p4.DeltaR(chosen_muon_p4);
      float subleading_hbbjet_mu_dr = subleading_hbbjet_p4.DeltaR(chosen_muon_p4);
      if (leading_hbbjet_mu_dr >= subleading_hbbjet_mu_dr) {
        hadmu_chosen_hbbjet_p4 = leading_hbbjet_p4;
        m_hadMuLRHbbJetPt = m_leadingLRHbbJetPt;
        m_hadMuLRHbbJetEta = m_leadingLRHbbJetEta;
        m_hadMuLRHbbJetPhi = m_leadingLRHbbJetPhi;
        m_hadMuLRHbbJetE = m_leadingLRHbbJetE;
        m_hadMuLRHbbJetM = m_leadingLRHbbJetM;
        m_hadMuLRHbbJetDisc = m_leadingLRHbbJetDisc;
        m_hadMuLRHbbJetTruthdR = leading_hbbjet_truth_dr;
      }
      else {
        hadmu_chosen_hbbjet_p4 = subleading_hbbjet_p4;
        m_hadMuLRHbbJetPt = m_subleadingLRHbbJetPt;
        m_hadMuLRHbbJetEta = m_subleadingLRHbbJetEta;
        m_hadMuLRHbbJetPhi = m_subleadingLRHbbJetPhi;
        m_hadMuLRHbbJetE = m_subleadingLRHbbJetE;
        m_hadMuLRHbbJetM = m_subleadingLRHbbJetM;
        m_hadMuLRHbbJetDisc = m_subleadingLRHbbJetDisc;
        m_hadMuLRHbbJetTruthdR = subleading_hbbjet_truth_dr;
      }
    }
    // If only one jet then just use the leading
    else if (m_nLargeRHbbJets == 1) {
      TLorentzVector leading_hbbjet_p4 = leading_hbb_largerjet->p4();
      float leading_hbbjet_truth_dr = leading_hbbjet_p4.DeltaR(truth_b_higgs_p4);

      hadel_chosen_hbbjet_p4 = leading_hbbjet_p4;
      hadmu_chosen_hbbjet_p4 = leading_hbbjet_p4;
    
      m_hadElLRHbbJetPt = m_leadingLRHbbJetPt;
      m_hadElLRHbbJetEta = m_leadingLRHbbJetEta;
      m_hadElLRHbbJetPhi = m_leadingLRHbbJetPhi;
      m_hadElLRHbbJetE = m_leadingLRHbbJetE;
      m_hadElLRHbbJetM = m_leadingLRHbbJetM;
      m_hadElLRHbbJetTruthdR = leading_hbbjet_truth_dr;

      m_hadMuLRHbbJetPt = m_leadingLRHbbJetPt;
      m_hadMuLRHbbJetEta = m_leadingLRHbbJetEta;
      m_hadMuLRHbbJetPhi = m_leadingLRHbbJetPhi;
      m_hadMuLRHbbJetE = m_leadingLRHbbJetE;
      m_hadMuLRHbbJetM = m_leadingLRHbbJetM;
      m_hadMuLRHbbJetTruthdR = leading_hbbjet_truth_dr;
    }

    TLorentzVector chosen_hadel_dihiggs_p4 = hadel_chosen_hbbjet_p4 + chosen_hadel_system_p4;
    m_hadElDiHiggsPt = chosen_hadel_dihiggs_p4.Pt() / 1000.;
    m_hadElDiHiggsEta = chosen_hadel_dihiggs_p4.Eta();
    m_hadElDiHiggsPhi = chosen_hadel_dihiggs_p4.Phi();
    m_hadElDiHiggsE = chosen_hadel_dihiggs_p4.E() / 1000.;
    m_hadElDiHiggsM = chosen_hadel_dihiggs_p4.M() / 1000.;

    TLorentzVector chosen_hadmu_dihiggs_p4 = hadmu_chosen_hbbjet_p4 + chosen_hadmu_system_p4;
    m_hadMuDiHiggsPt = chosen_hadmu_dihiggs_p4.Pt() / 1000.;
    m_hadMuDiHiggsEta = chosen_hadmu_dihiggs_p4.Eta();
    m_hadMuDiHiggsPhi = chosen_hadmu_dihiggs_p4.Phi();
    m_hadMuDiHiggsE = chosen_hadmu_dihiggs_p4.E() / 1000.;
    m_hadMuDiHiggsM = chosen_hadmu_dihiggs_p4.M() / 1000.;
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

  // The unique event ID is a combination of the event number, leading large-R jet Pt, leading loose electron pt, and leading tight muon pt
  std::string event_num_string = std::to_string(m_eventNumber);
  std::string leading_jet_pt_string = std::to_string(int(m_leadingLRJetPt));
  std::string leading_eleloose_pt_string = std::to_string(int(m_leadingEleLoosePt));
  std::string leading_mutight_pt_string = std::to_string(int(m_leadingMuTightPt));

  leading_jet_pt_string.resize(3, '0');
  leading_eleloose_pt_string.resize(3, '0');
  leading_mutight_pt_string.resize(3, '0');

  std::string unique_event_id_str = event_num_string + leading_jet_pt_string + leading_eleloose_pt_string + leading_mutight_pt_string;
  unsigned long long unique_event_id = std::stol(unique_event_id_str);
  m_uniqueEventID = unique_event_id;  


  m_mytree->Fill();
  

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
