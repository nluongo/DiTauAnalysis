#include <DiTau/DiTauAnalysis.h>

#include <xAODEventInfo/EventInfo.h>

#include "AsgTools/ToolHandle.h"
#include "AsgTools/ToolHandleArray.h"

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

//#include "JetInterface/IJetConstituentModifier.h"
//#include "JetRecTools/JetConstituentModSequence.h"

#include "TFile.h"

#include <typeinfo>
#include <cmath>

DiTauAnalysis :: DiTauAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.
}

DiTauAnalysis :: ~DiTauAnalysis () {
  delete m_diTauPt;
  delete m_diTauEta;
  delete m_diTauPhi;
  delete m_nDiTauSubjets;

  delete m_hadElDiTauPt;
  delete m_hadElDiTauEta;
  delete m_hadElDiTauPhi;
  delete m_hadElDiTauBDTScore;
  delete m_hadElDiTauFlatBDTScore;
  delete m_hadElDiTauTruthMatchType;

  delete m_hadMuDiTauPt;
  delete m_hadMuDiTauEta;
  delete m_hadMuDiTauPhi;
  delete m_hadMuDiTauBDTScore;
  delete m_hadMuDiTauFlatBDTScore;
  delete m_hadMuDiTauTruthMatchType;

  delete m_tauPt;
  delete m_tauEta;
  delete m_tauPhi;
  delete m_tauE;
  delete m_tauLoose;
  delete m_tauMedium;
  delete m_tauTight;

  delete m_elePt;
  delete m_eleEta;
  delete m_elePhi;
  delete m_eleE;
  delete m_eleLoose;
  delete m_eleMedium;
  delete m_eleTight;

  delete m_muPt;
  delete m_muEta;
  delete m_muPhi;
  delete m_muE;
  delete m_muLoose;
  delete m_muMedium;
  delete m_muTight;

}

StatusCode DiTauAnalysis :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_MSG_INFO("in initialize test");

  //ANA_CHECK( book(TTree ("mytree", "mytree")));
  //m_myfile = new TFile("../samples/ditau_450524_X2000tohh_bbtautau_lephad.recon.AOD.ntuple.root", "recreate");
  m_mytree = new TTree("mytree", "mytree");
  CHECK( histSvc()->regTree("/ANALYSIS/mytree", m_mytree) );

  m_mytree->Branch("RunNumber", &m_runNumber);
  m_mytree->Branch("EventNumber", &m_eventNumber);

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

  m_mytree->Branch("NDiTau", &m_nDiTaus);
  m_diTauPt = new std::vector<float>();
  m_mytree->Branch("DiTauPt", &m_diTauPt);
  m_diTauEta = new std::vector<float>();
  m_mytree->Branch("DiTauEta", &m_diTauEta);
  m_diTauPhi = new std::vector<float>();
  m_mytree->Branch("DiTauPhi", &m_diTauPhi);
  m_nDiTauSubjets = new std::vector<unsigned int>();
  m_mytree->Branch("NDiTauSubjets", &m_nDiTauSubjets);

  m_mytree->Branch("NHadElDiTau", &m_nHadElDiTaus);
  m_hadElDiTauPt = new std::vector<float>();
  m_mytree->Branch("HadElDiTauPt", &m_hadElDiTauPt);
  m_hadElDiTauEta = new std::vector<float>();
  m_mytree->Branch("HadElDiTauEta", &m_hadElDiTauEta);
  m_hadElDiTauPhi = new std::vector<float>();
  m_mytree->Branch("HadElDiTauPhi", &m_hadElDiTauPhi);
  m_hadElDiTauBDTScore = new std::vector<float>();
  m_mytree->Branch("HadElDiTauBDTScore", &m_hadElDiTauBDTScore);
  m_hadElDiTauFlatBDTScore = new std::vector<float>();
  m_mytree->Branch("HadElDiTauFlatBDTScore", &m_hadElDiTauFlatBDTScore);
  m_hadElDiTauTruthMatchType = new std::vector<unsigned int>();
  m_mytree->Branch("HadElDiTauTruthMatchType", &m_hadElDiTauTruthMatchType);

  m_mytree->Branch("NHadMuDiTau", &m_nHadMuDiTaus);
  m_hadMuDiTauPt = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauPt", &m_hadMuDiTauPt);
  m_hadMuDiTauEta = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauEta", &m_hadMuDiTauEta);
  m_hadMuDiTauPhi = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauPhi", &m_hadMuDiTauPhi);
  m_hadMuDiTauBDTScore = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauBDTScore", &m_hadMuDiTauBDTScore);
  m_hadMuDiTauFlatBDTScore = new std::vector<float>();
  m_mytree->Branch("HadMuDiTauFlatBDTScore", &m_hadMuDiTauFlatBDTScore);
  m_hadMuDiTauTruthMatchType = new std::vector<unsigned int>();
  m_mytree->Branch("HadMuDiTauTruthMatchType", &m_hadMuDiTauTruthMatchType);

  m_mytree->Branch("NTau", &m_nTaus);
  m_tauPt = new std::vector<float>();
  m_mytree->Branch("TauPt", &m_tauPt);
  m_tauEta = new std::vector<float>();
  m_mytree->Branch("TauEta", &m_tauEta);
  m_tauPhi = new std::vector<float>();
  m_mytree->Branch("TauPhi", &m_tauPhi);
  m_tauE = new std::vector<float>();
  m_mytree->Branch("TauE", &m_tauE);
  m_tauLoose = new std::vector<unsigned int>();
  m_mytree->Branch("TauLoose", &m_tauLoose);
  m_tauMedium = new std::vector<unsigned int>();
  m_mytree->Branch("TauMedium", &m_tauMedium);
  m_tauTight = new std::vector<unsigned int>();
  m_mytree->Branch("TauTight", &m_tauTight);

  m_mytree->Branch("NEle", &m_nElectrons);
  m_elePt = new std::vector<float>();
  m_mytree->Branch("ElePt", &m_elePt);
  m_eleEta = new std::vector<float>();
  m_mytree->Branch("EleEta", &m_eleEta);
  m_elePhi = new std::vector<float>();
  m_mytree->Branch("ElePhi", &m_elePhi);
  m_eleE = new std::vector<float>();
  m_mytree->Branch("EleE", &m_eleE);
  m_eleLoose = new std::vector<unsigned int>();
  m_mytree->Branch("EleLoose", &m_eleLoose);
  m_eleMedium = new std::vector<unsigned int>();
  m_mytree->Branch("EleMedium", &m_eleMedium);
  m_eleTight = new std::vector<unsigned int>();
  m_mytree->Branch("EleTight", &m_eleTight);

  m_mytree->Branch("NMu", &m_nMuons);
  m_muPt = new std::vector<float>();
  m_mytree->Branch("MuPt", &m_muPt);
  m_muEta = new std::vector<float>();
  m_mytree->Branch("MuEta", &m_muEta);
  m_muPhi = new std::vector<float>();
  m_mytree->Branch("MuPhi", &m_muPhi);
  m_muE = new std::vector<float>();
  m_mytree->Branch("MuE", &m_muE);
  m_muLoose = new std::vector<unsigned int>();
  m_mytree->Branch("MuLoose", &m_muLoose);
  m_muMedium = new std::vector<unsigned int>();
  m_mytree->Branch("MuMedium", &m_muMedium);
  m_muTight = new std::vector<unsigned int>();
  m_mytree->Branch("MuTight", &m_muTight);

  // TauTruthMatching
  //CHECK(m_tauTruthMatchingTool.retrieve());
  m_tauTruthMatchingTool = new TauAnalysisTools::TauTruthMatchingTool("TauTruthMatchingTool");
  CHECK(m_tauTruthMatchingTool->initialize());
  CHECK(m_tauTruthMatchingTool->setProperty("WriteTruthTaus", true));

  // Builders
  m_hadElBuilder = new DiTauRec::HadElBuilder("HadElBuilder");
  CHECK(m_hadElBuilder->initialize());

  m_hadMuBuilder = new DiTauRec::HadMuBuilder("HadMuBuilder");
  CHECK(m_hadMuBuilder->initialize());
/*
  // Initialize origin correction
  //ToolHandleArray<IJetConstituentModifier> modifiers;
  ANA_MSG_INFO("modifiers: " << modifiers);
  std::cout << "modifiers: " << modifiers << std::endl;
  ANA_MSG_INFO("modifiers id: " << typeid(modifiers).name());
  //ToolHandleArray<IJetConstituentModifier> new_modifiers = modifiers;

  // CaloClusterOrigin
  CaloClusterConstituentsOrigin* m_pCaloClusterConstituentsOrigin = new CaloClusterConstituentsOrigin("CaloClusterConstituentsOrigin");
  CHECK( m_pCaloClusterConstituentsOrigin->setProperty("InputType",xAOD::Type::CaloCluster) );
  CHECK( m_pCaloClusterConstituentsOrigin->initialize() );
  ANA_MSG_INFO("caloclus id: " << typeid(m_pCaloClusterConstituentsOrigin).name());
  ANA_MSG_INFO("caloclus toolhandle id: " << typeid(ToolHandle<IJetConstituentModifier>(m_pCaloClusterConstituentsOrigin)).name());

  modifiers.push_back(ToolHandle<IJetConstituentModifier>(m_pCaloClusterConstituentsOrigin));
  ANA_MSG_INFO("modifiers: " << modifiers);
  std::cout << "modifiers: " << modifiers << std::endl;
  ANA_MSG_INFO("modifiers id: " << typeid(modifiers).name());
  ANA_MSG_INFO("Here");

  // Initialize cluster sequence
  JetConstituentModSequence jJetConstituentModSequence("JetConstituentModSequence");
  ANA_MSG_INFO("Here");
  CHECK( jJetConstituentModSequence.setProperty("InputContainer","CaloCalTopoClusters") );
  ANA_MSG_INFO("Here");
  CHECK( jJetConstituentModSequence.setProperty("OutputContainer","LCOriginTopoClusters") );
  ANA_MSG_INFO("Here");
  ANA_MSG_INFO("OutputContainer property");
  std::cout << "OutputContainer property" << std::endl;
  std::cout << "OutputContainer property: " << jJetConstituentModSequence.getProperty("OutputContainer") << std::endl;
  CHECK( jJetConstituentModSequence.setProperty("InputType",xAOD::Type::CaloCluster) );
  CHECK( jJetConstituentModSequence.setProperty("Modifiers", modifiers) );
  CHECK( jJetConstituentModSequence.initialize() );

  //Override the event store hash for the LCOriginTopoClusters
  //xAOD::EventFormat* ef = const_cast< xAOD::EventFormat* >( xEvent.inputEventFormat() );
  //ef->add( xAOD::EventFormatElement( "LCOriginTopoClusters", "xAOD::CaloClusterContainer", "", 0x1bccf189 ) );
*/
  // DiTauTruthMatchingTool
  m_diTauTruthMatchingTool = new TauAnalysisTools::DiTauTruthMatchingTool("DiTauTruthMatchingTool");
  CHECK(m_diTauTruthMatchingTool->setProperty("WriteTruthTaus", true));
  CHECK(m_diTauTruthMatchingTool->setProperty("TruthMuonContainerName", "MuonTruthParticles"));
  CHECK(m_diTauTruthMatchingTool->setProperty("TruthElectronContainerName", "egammaTruthParticles"));
  CHECK(m_diTauTruthMatchingTool->initialize());
  ANA_MSG_INFO(m_diTauTruthMatchingTool);

  // DiTauIDVarCalculators
  m_hadElDiTauIDVarCalculator = new tauRecTools::DiTauIDVarCalculator("HadElDiTauIdVarCalculator");
  m_hadElDiTauIDVarCalculator->setProperty("DiTauDecayChannel", "HadEl");
  CHECK(m_hadElDiTauIDVarCalculator->initialize());

  m_hadMuDiTauIDVarCalculator = new tauRecTools::DiTauIDVarCalculator("HadMuDiTauIdVarCalculator");
  m_hadMuDiTauIDVarCalculator->setProperty("DiTauDecayChannel", "HadMu");
  CHECK(m_hadMuDiTauIDVarCalculator->initialize());

  // DiTauDiscriminantTools
  m_hadElDiTauDiscrTool = new tauRecTools::DiTauDiscriminantTool("HadElDiTauDiscrTool");
  m_hadElDiTauDiscrTool->setProperty("DiTauDecayChannel", "HadEl");
  m_hadElDiTauDiscrTool->setProperty("WeightsFile", "/eos/user/n/nicholas/SWAN_projects/DiTauReco/DiTauLepHadExample/weight_files/bdt_hadel_v9.root");
  CHECK(m_hadElDiTauDiscrTool->initialize());

  m_hadMuDiTauDiscrTool = new tauRecTools::DiTauDiscriminantTool("HadMuDiTauDiscrTool");
  m_hadMuDiTauDiscrTool->setProperty("DiTauDecayChannel", "HadMu");
  m_hadMuDiTauDiscrTool->setProperty("WeightsFile", "/eos/user/n/nicholas/SWAN_projects/DiTauReco/DiTauLepHadExample/weight_files/bdt_hadmu_v18.root");
  CHECK(m_hadMuDiTauDiscrTool->initialize());

  // DiTauWPDecorators
  m_hadElDiTauWPDecorator = new tauRecTools::DiTauWPDecorator("HadElDiTauWPDecorator");
  m_hadElDiTauWPDecorator->setProperty("DiTauDecayChannel", "HadEl");
  m_hadElDiTauWPDecorator->setProperty("flatteningFile", "/eos/user/n/nicholas/SWAN_projects/DiTauReco/DiTauLepHadExample/weight_files/tuner_hadel_v9.root");
  m_hadElDiTauWPDecorator->initialize();

  m_hadMuDiTauWPDecorator = new tauRecTools::DiTauWPDecorator("HadMuDiTauWPDecorator");
  m_hadMuDiTauWPDecorator->setProperty("DiTauDecayChannel", "HadMu");
  m_hadMuDiTauWPDecorator->setProperty("flatteningFile", "/eos/user/n/nicholas/SWAN_projects/DiTauReco/DiTauLepHadExample/weight_files/tuner_hadmu_v18.root");
  m_hadMuDiTauWPDecorator->initialize();

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
/*
  const xAOD::TruthParticleContainer* myTruthTaus = nullptr;

  // Filter truth taus to only those coming from the tau Higgs
  for (auto truth_tau: *truthTaus) {
    xAOD::TruthParticle* parent_tau = truth_tau->parent(0);
    int parent_pdg = parent_tau->pdgId();
    if (parent_pdg == 25 || parent_pdg == 15 || parent_pdg == -15) {
      myTruthTaus->push_back(parent_tau);
    }
  }
*/
  const xAOD::ElectronContainer* electrons = nullptr;
  ANA_CHECK( evtStore()->retrieve( electrons, "Electrons" ) );

  const xAOD::MuonContainer* muons = nullptr;
  ANA_CHECK( evtStore()->retrieve( muons, "Muons" ) );

  const xAOD::TauJetContainer* taus = nullptr;
  ANA_CHECK( evtStore()->retrieve( taus, "TauJets" ) );

  const xAOD::DiTauJetContainer* ditaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( ditaus, "DiTauJets" ) );

  ANA_CHECK( m_hadElBuilder->execute() );
  const xAOD::DiTauJetContainer* hadelditaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( hadelditaus, "DiTauJetsHadEl" ) );

  ANA_CHECK( m_hadMuBuilder->execute() );
  const xAOD::DiTauJetContainer* hadmuditaus = nullptr;
  ANA_CHECK( evtStore()->retrieve( hadmuditaus, "DiTauJetsHadMu" ) );
  /*
  for (auto ditau: *ditaus) {
    m_diTauTruthMatchingTool->getTruth(*ditau);
    if (ditau->auxdata<char>((const char*)("IsTruthHadronic"))){
      m_truthMatchedHadHad++;
    }
    if (ditau->auxdata<char>((const char*)("IsTruthHadEl"))){
      m_truthMatchedHadEl++;
    }
    if (ditau->auxdata<char>((const char*)("IsTruthHadMu"))){
      m_truthMatchedHadMu++;
    }
  }
  for (auto ditau: *hadelditaus) {
    m_diTauTruthMatchingTool->getTruth(*ditau);
    if (ditau->auxdata<char>((const char*)("IsTruthHadronic"))){
      m_truthMatchedHadHad++;
    }
    if (ditau->auxdata<char>((const char*)("IsTruthHadEl"))){
      m_truthMatchedHadEl++;
    }
    if (ditau->auxdata<char>((const char*)("IsTruthHadMu"))){
      m_truthMatchedHadMu++;
    }
  }
  for (auto ditau: *hadmuditaus) {
    m_diTauTruthMatchingTool->getTruth(*ditau);
    if (ditau->auxdata<char>((const char*)("IsTruthHadronic"))){
      m_truthMatchedHadHad++;
    }
    if (ditau->auxdata<char>((const char*)("IsTruthHadEl"))){
      m_truthMatchedHadEl++;
    }
    if (ditau->auxdata<char>((const char*)("IsTruthHadMu"))){
      m_truthMatchedHadMu++;
    }
  }
  */
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

  m_nDiTaus = 0;
  m_diTauPt->clear();
  m_diTauEta->clear();
  m_diTauPhi->clear();
  m_nDiTauSubjets->clear();

  m_nHadElDiTaus = 0;
  m_hadElDiTauPt->clear();
  m_hadElDiTauEta->clear();
  m_hadElDiTauPhi->clear();
  m_hadElDiTauBDTScore->clear();
  m_hadElDiTauFlatBDTScore->clear();
  m_hadElDiTauTruthMatchType->clear();

  m_nHadMuDiTaus = 0;
  m_hadMuDiTauPt->clear();
  m_hadMuDiTauEta->clear();
  m_hadMuDiTauPhi->clear();
  m_hadMuDiTauBDTScore->clear();
  m_hadMuDiTauFlatBDTScore->clear();
  m_hadMuDiTauTruthMatchType->clear();

  m_nTaus = 0;
  m_tauPt->clear();
  m_tauEta->clear();
  m_tauPhi->clear();
  m_tauE->clear();
  m_tauLoose->clear();
  m_tauMedium->clear();
  m_tauTight->clear();

  m_nMuons = 0;
  m_muPt->clear();
  m_muEta->clear();
  m_muPhi->clear();
  m_muE->clear();
  m_muLoose->clear();
  m_muMedium->clear();
  m_muTight->clear();

  m_nElectrons = 0;
  m_elePt->clear();
  m_eleEta->clear();
  m_elePhi->clear();
  m_eleE->clear();
  m_eleLoose->clear();
  m_eleMedium->clear();
  m_eleTight->clear();

  const xAOD::TruthParticle* truth_tau_higgs = nullptr;
  const xAOD::TruthParticle* truth_b_higgs = nullptr;
  const xAOD::TruthParticle* truth_tau = nullptr;
  const xAOD::TruthParticle* truth_anti_tau = nullptr;
  const xAOD::TruthParticle* truth_leptonic_tau = nullptr;
  const xAOD::TruthParticle* truth_hadronic_tau = nullptr;
  const xAOD::TruthParticle* truth_final_lepton = nullptr;
  

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
          ANA_MSG_INFO("TruthParticle pt: " << child->pt());
          has_tau = 1;
        }
        else if (child_id == -15) {
          ANA_MSG_INFO("TruthParticle pt: " << child->pt());
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
  
  if (has_tau_higgs && has_b_higgs && has_hadronic_tau && has_leptonic_tau && has_final_lepton) {
    bool fill_yn = 1;

    // Let's do this thing
    m_eventsPassed++;
    m_runNumber = eventInfo->runNumber();
    m_eventNumber = eventInfo->eventNumber();

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
    m_truthHiggsPt = truth_tau_higgs->pt();
    m_truthHiggsEta = truth_tau_higgs->eta();
    m_truthHiggsPhi = truth_tau_higgs->phi();
    m_truthHiggsE = truth_tau_higgs->e();

    // Define accessor for hadronic tau visible variables
    static SG::AuxElement::Accessor<double> accPtVis("pt_vis");
    static SG::AuxElement::Accessor<double> accEtaVis("eta_vis");
    static SG::AuxElement::Accessor<double> accPhiVis("phi_vis");
    static SG::AuxElement::Accessor<double> accMVis("m_vis");

    // Truth hadronic tau
    m_truthHadTauPt = truth_hadronic_tau->pt();
    m_truthHadTauEta = truth_hadronic_tau->eta();
    m_truthHadTauPhi = truth_hadronic_tau->phi();
    m_truthHadTauE = truth_hadronic_tau->e();
    m_truthHadTauPdgId = truth_hadronic_tau->pdgId();
/*
    float min_pt_diff = 100000;
    const xAOD::TruthParticle* truthtaus_match = nullptr;
    for (auto truth_tau: *truthTaus) {
      float pt_diff = abs( truth_tau->pt() - truth_hadronic_tau->pt() );
      if (pt_diff < min_pt_diff) {
        min_pt_diff = pt_diff;
        truthtaus_match = truth_tau;
      }
    }
*/

    // If we can't find truthtau or the visible variables for hadronic truth tau, don't use this event
    if (truth_hadronic_tau != nullptr) {
      if (!accPtVis.isAvailable(*truth_hadronic_tau) || !accEtaVis.isAvailable(*truth_hadronic_tau) || !accPhiVis.isAvailable(*truth_hadronic_tau) || !accMVis.isAvailable(*truth_hadronic_tau)) {
        fill_yn = 0;
      } 
      else {
        m_truthHadTauVisPt = accPtVis(*truth_hadronic_tau);
        m_truthHadTauVisEta = accEtaVis(*truth_hadronic_tau);
        m_truthHadTauVisPhi = accPhiVis(*truth_hadronic_tau);
        m_truthHadTauVisM = accMVis(*truth_hadronic_tau);
      }
    }

    // Truth leptonic tau
    m_truthLepTauPt = truth_leptonic_tau->pt();
    m_truthLepTauEta = truth_leptonic_tau->eta();
    m_truthLepTauPhi = truth_leptonic_tau->phi();
    m_truthLepTauE = truth_leptonic_tau->e();
    m_truthLepTauPdgId = truth_leptonic_tau->pdgId();

    // Truth final lepton
    m_truthFinalLeptonPt = truth_final_lepton->pt();
    m_truthFinalLeptonEta = truth_final_lepton->eta();
    m_truthFinalLeptonPhi = truth_final_lepton->phi();
    m_truthFinalLeptonE = truth_final_lepton->e();
    m_truthFinalLeptonPdgId = truth_final_lepton->pdgId();

    // Reconstructed hadhad ditaus
    for (auto ditau: *ditaus) {
      m_nDiTaus++;
      m_diTauPt->push_back(ditau->pt());
      m_diTauEta->push_back(ditau->eta());
      m_diTauPhi->push_back(ditau->phi());
      m_nDiTauSubjets->push_back(ditau->nSubjets());
    }

    // Reconstructed hadel ditaus
    for (auto hadelditau: *hadelditaus) {
      m_nHadElDiTaus++;
      m_hadElDiTauPt->push_back(hadelditau->pt());
      m_hadElDiTauEta->push_back(hadelditau->eta());
      m_hadElDiTauPhi->push_back(hadelditau->phi());

      CHECK(m_hadElDiTauIDVarCalculator->execute(*hadelditau));
      
      //if(hadel){
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
      //}
    
      CHECK(m_hadElDiTauDiscrTool->execute(*hadelditau));
      CHECK(m_hadElDiTauWPDecorator->execute(*hadelditau));
      double bdt = hadelditau->auxdata<double>("JetBDT");
      double flat_bdt = hadelditau->auxdata<double>("JetBDTFlat");
      m_hadElDiTauBDTScore->push_back(bdt);
      m_hadElDiTauFlatBDTScore->push_back(flat_bdt);
      
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

    ANA_MSG_INFO("Starting hadmus");
    // Reconstructed hadmu ditaus
    for (auto hadmuditau: *hadmuditaus) {
      m_nHadMuDiTaus++;
      m_hadMuDiTauPt->push_back(hadmuditau->pt());
      m_hadMuDiTauEta->push_back(hadmuditau->eta());
      m_hadMuDiTauPhi->push_back(hadmuditau->phi());

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
      */
    }
    ANA_MSG_INFO("Done with hadmus");

    // Reconstructed taus
    for (auto tau: *taus) {
      m_nTaus++;
      m_tauPt->push_back(tau->pt());
      m_tauEta->push_back(tau->eta());
      m_tauPhi->push_back(tau->phi());
      m_tauE->push_back(tau->e());
      unsigned int tau_pass_loose = 0;
      unsigned int tau_pass_medium = 0;
      unsigned int tau_pass_tight = 0;
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigLoose)) {
        tau_pass_loose = 1;
      }
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigMedium)) {
        tau_pass_medium = 1;
      }
      if (tau->isTau(xAOD::TauJetParameters::IsTauFlag::JetBDTSigTight)) {
        tau_pass_tight = 1;
      }
      m_tauLoose->push_back(tau_pass_loose);
      m_tauMedium->push_back(tau_pass_medium);
      m_tauTight->push_back(tau_pass_tight);
    }

    // Reconstructed muons
    for (auto muon: *muons) {
      m_nMuons++;
      m_muPt->push_back(muon->pt());
      m_muEta->push_back(muon->eta());
      m_muPhi->push_back(muon->phi());
      m_muE->push_back(muon->e());
      // Muon quality 0=Tight 1=Medium 2=Loose 3=VeryLoose(all)
      unsigned int muQuality = muon->quality();
      unsigned int mu_pass_loose = 0;
      unsigned int mu_pass_medium = 0;
      unsigned int mu_pass_tight = 0;
      if (muQuality == 0) {
        mu_pass_loose = 1;
        mu_pass_medium = 1;
        mu_pass_tight = 1;
      } else if (muQuality == 1) {
        mu_pass_loose = 1;
        mu_pass_medium = 1;
      } else if (muQuality == 2) {
        mu_pass_loose = 1;
      } 
      m_muLoose->push_back(mu_pass_loose);
      m_muMedium->push_back(mu_pass_medium);
      m_muTight->push_back(mu_pass_tight);
    }

    // Reconstructed electrons
    static const SG::AuxElement::ConstAccessor<int> accTruthType("truthType");
    for (auto electron: *electrons) {
      m_nElectrons++;
      m_elePt->push_back(electron->pt());
      m_eleEta->push_back(electron->eta());
      m_elePhi->push_back(electron->phi());
      m_eleE->push_back(electron->e());
      unsigned int ele_pass_loose = 0;
      unsigned int ele_pass_medium = 0;
      unsigned int ele_pass_tight = 0;
      if (electron->passSelection("Loose")) {
        ele_pass_loose = 1;
      }
      if (electron->passSelection("Medium")) {
        ele_pass_medium = 1;
      }
      if (electron->passSelection("Tight")) {
        ele_pass_tight = 1;
      }
      m_eleLoose->push_back(ele_pass_loose);
      m_eleMedium->push_back(ele_pass_medium);
      m_eleTight->push_back(ele_pass_tight);
    }

    if (fill_yn) 
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
