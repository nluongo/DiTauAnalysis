#See: https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/SoftwareTutorialxAODAnalysisInCMake for more details about anything here
from glob import glob
'''
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('sample', type=str, default='X2000AOD')
parser.add_argument('--nevents', type=int, default=10000)
args = vars(parser.parse_args())
sample = args['sample']
nevents = args['--nevents']
print('Sample: {}'.format(sample))
print('NEvents: {}'.format(nevents))
'''
print('In jobOptions')

daod_template = '/eos/user/n/nicholas/SWAN_projects/Derivation/run/DAOD_HIGGBOOSTEDLH.{}.DAOD_HIGGBOOSTEDLH.pool.root'
ntuple_template = '{}.ntuple.root'

if sample == 'X1000DAOD':
    sample_name = '450522_X1000_bbtautau_lephad'
    daod_path = daod_template.format(sample_name)
    myFiles = glob(daod_path)
    outName = ntuple_template.format(sample_name)
    isSignal = 1
elif sample == 'X1000AOD':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.450522.MadGraphPythia8EvtGen_A14NNPDF23LO_X1000tohh_bbtautau_lephad.recon.AOD.e7244_s3126_r10201/*')
    outName = 'ditau_450522_X1000tohh_bbtautau_lephad.recon.AOD.ntuple.root'
    isSignal = 1
elif sample == 'X1600DAOD':
    sample_name = '450166_X1600_bbtautau_lephad'
    daod_path = daod_template.format(sample_name)
    myFiles = glob(daod_path)
    outName = ntuple_template.format(sample_name)
    isSignal = 1
elif sample == 'X1600AOD':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/user.nicholas.mc16_13TeV.450166.MadGraphHerwig7EvtGen_PDF23LO_X1600tohh_bbtautau_lephad.recon.AOD.e8317_s3126_r10724_der1632783813/*')
    outName = 'ditau_450166_X1600tohh_bbtautau_lephad.recon.AOD.ntuple.root'
    isSignal = 1
elif sample == 'X2000DAOD':
    sample_name = '450524_X2000_bbtautau_lephad'
    daod_path = daod_template.format(sample_name)
    myFiles = glob(daod_path)
    outName = ntuple_template.format(sample_name)
    isSignal = 1
elif sample == 'X2000AOD':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.450524.MadGraphPythia8EvtGen_A14NNPDF23LO_X2000tohh_bbtautau_lephad.recon.AOD.e7244_s3126_r10201/*')
    outName = 'ditau_450524_X2000tohh_bbtautau_lephad.recon.AOD.ntuple.root'
    isSignal = 1
elif sample == 'X2000ESD':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/bbtautau/run3Trigger/r3athanalysis/samples/mc16_13TeV.450524.MadGraphPythia8EvtGen_A14NNPDF23LO_X2000tohh_bbtautau_lephad.recon.ESD.e7244_e5984_s3126_r12406/*')
    outName = 'ditau_450524_X2000tohh_bbtautau_lephad.recon.ESD.ntuple.root'
    isSignal = 1
elif sample == 'Z+jetsDAOD':
    sample_name = '364139_Ztautau_BFilter'
    daod_path = daod_template.format(sample_name)
    myFiles = glob(daod_path)
    outName = ntuple_template.format(sample_name)
    isSignal = 0
elif sample == 'Z+jets':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.364139.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_BFilter.recon.AOD.e5313_s3126_r10201/*')
    outName = 'background_364139_Ztautau_MAXHTPTV280_500_BFilter.recon.AOD.ntuple.root'
    isSignal = 0
elif sample == 'ttbarDAOD':
    sample_name = '410470_ttbar_hdamp258p75_nonallhad'
    daod_path = daod_template.format(sample_name)
    myFiles = glob(daod_path)
    outName = ntuple_template.format(sample_name)
    isSignal = 0
elif sample == 'ttbar':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.recon.AOD.e6337_e5984_s3126_r10201/*')
    outName = 'background_410470_ttbar_nonallhad.recon.AOD.ntuple.root'
    isSignal = 0

#myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.425105.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_4tau_c10_M3000.recon.AOD.e6072_s3126_r10724/*.root.1')
#outName = '../samples/ditau_425105_G_hh_4tau_M3000.recon.AOD.ntuple.root'

#myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/group.perf-tau.425104.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_4tau_c10_M2500.R22_NewTunes_EXT0/*')
#outName = 'ditau_425104_4tau.AOD.ntuple.root'

#override next line on command line with: --filesInput=XXX
jps.AthenaCommonFlags.FilesInput = myFiles

from PyUtils import AthFile
import AthenaPoolCnvSvc.ReadAthenaPool
from AthenaCommon.AthenaCommonFlags import athenaCommonFlags

#Some database settings, needed for ESD  
#from RecExConfig import AutoConfiguration
#AutoConfiguration.ConfigureSimulationOrRealData()
#AutoConfiguration.ConfigureGeo()

from AthenaCommon.DetFlags import DetFlags
DetFlags.detdescr.all_setOff()
DetFlags.detdescr.BField_setOn()
DetFlags.detdescr.Calo_setOn()
#include("RecExCond/AllDet_detDescr.py")

#jps.AthenaCommonFlags.AccessMode = "ClassAccess" 
jps.AthenaCommonFlags.HistOutputs = ["ANALYSIS:"+outName]

# Create the algorithm's configuration.
#from AnaAlgorithm.DualUseConfig import createAlgorithm
#alg = createAlgorithm ( 'DiTauAnalysis', 'AnalysisAlg' )
from AthenaCommon import CfgMgr
from AnaAlgorithm.DualUseConfig import addPrivateTool
algClass = getattr(CfgMgr, 'DiTauAnalysis')
alg = algClass('AnalysisAlg')

# later on we'll add some configuration options for our algorithm that go here
#alg.outName = outName
alg.isSignal = isSignal

# DiTauTruthMatchingTool configuration
addPrivateTool( alg, 'diTauTruthMatchingTool', 'TauAnalysisTools::DiTauTruthMatchingTool')
alg.diTauTruthMatchingTool.WriteTruthTaus = 1
alg.diTauTruthMatchingTool.TruthMuonContainerName = 'MuonTruthParticles'
alg.diTauTruthMatchingTool.TruthElectronContainerName = 'egammaTruthParticles'

# DiTauIDVarCalculator configuration
addPrivateTool( alg, 'hadElDiTauIDVarCalculator', 'tauRecTools::DiTauIDVarCalculator')
alg.hadElDiTauIDVarCalculator.DiTauDecayChannel = 'HadEl'

addPrivateTool( alg, 'hadMuDiTauIDVarCalculator', 'tauRecTools::DiTauIDVarCalculator')
alg.hadMuDiTauIDVarCalculator.DiTauDecayChannel = 'HadMu'

# DiTauDiscriminantTool configuration
addPrivateTool( alg, 'hadElDiTauDiscrTool', 'tauRecTools::DiTauDiscriminantTool')
alg.hadElDiTauDiscrTool.DiTauDecayChannel = 'HadEl'
alg.hadElDiTauDiscrTool.WeightsFile = 'weight_files/bdt_hadel_v9.root'

addPrivateTool( alg, 'hadMuDiTauDiscrTool', 'tauRecTools::DiTauDiscriminantTool')
alg.hadMuDiTauDiscrTool.DiTauDecayChannel = 'HadMu'
alg.hadMuDiTauDiscrTool.WeightsFile = 'weight_files/bdt_hadmu_v18.root'

# DiTauWPDecorator configuration
addPrivateTool( alg, 'hadElDiTauWPDecorator', 'tauRecTools::DiTauWPDecorator')
alg.hadElDiTauWPDecorator.DiTauDecayChannel = 'HadEl'
alg.hadElDiTauWPDecorator.flatteningFile = 'weight_files/tuner_hadel_v9.root'

addPrivateTool( alg, 'hadMuDiTauWPDecorator', 'tauRecTools::DiTauWPDecorator')
alg.hadMuDiTauWPDecorator.DiTauDecayChannel = 'HadMu'
alg.hadMuDiTauWPDecorator.flatteningFile = 'weight_files/tuner_hadmu_v18.root'

# Electron likelihood tools
addPrivateTool( alg, 'asgElectronVeryLooseLikelihoodTool', 'AsgElectronLikelihoodTool')
alg.asgElectronVeryLooseLikelihoodTool.primaryVertexContainer = 'PrimaryVertices'
alg.asgElectronVeryLooseLikelihoodTool.ConfigFile = 'ElectronPhotonSelectorTools/offline/mc16_20170828/ElectronLikelihoodVeryLooseOfflineConfig2017_Smooth.conf'

addPrivateTool( alg, 'asgElectronLooseLikelihoodTool', 'AsgElectronLikelihoodTool')
alg.asgElectronLooseLikelihoodTool.primaryVertexContainer = 'PrimaryVertices'
alg.asgElectronLooseLikelihoodTool.ConfigFile = 'ElectronPhotonSelectorTools/offline/mc16_20170828/ElectronLikelihoodLooseOfflineConfig2017_Smooth.conf'

addPrivateTool( alg, 'asgElectronMediumLikelihoodTool', 'AsgElectronLikelihoodTool')
alg.asgElectronMediumLikelihoodTool.primaryVertexContainer = 'PrimaryVertices'
alg.asgElectronMediumLikelihoodTool.ConfigFile = 'ElectronPhotonSelectorTools/offline/mc16_20170828/ElectronLikelihoodMediumOfflineConfig2017_Smooth.conf'

addPrivateTool( alg, 'asgElectronTightLikelihoodTool', 'AsgElectronLikelihoodTool')
alg.asgElectronTightLikelihoodTool.primaryVertexContainer = 'PrimaryVertices'
alg.asgElectronTightLikelihoodTool.ConfigFile = 'ElectronPhotonSelectorTools/offline/mc16_20170828/ElectronLikelihoodTightOfflineConfig2017_Smooth.conf'

# BTagging Tool
addPrivateTool( alg, 'bTaggingSelectionTool', 'BTaggingSelectionTool')
alg.bTaggingSelectionTool.TaggerName = 'DL1'
alg.bTaggingSelectionTool.OperatingPoint = 'FixedCutBEff_77'
alg.bTaggingSelectionTool.JetAuthor = 'AntiKt4EMTopoJets_BTagging201810'
alg.bTaggingSelectionTool.FlvTagCutDefinitionsFileName = 'xAODBTaggingEfficiency/13TeV/2020-21-13TeV-MC16-CDI-2020-03-11_v3.root'
alg.bTaggingSelectionTool.MinPt = 20000

# MissingMassCalculator
addPrivateTool( alg, 'missingMassTool', 'MissingMassTool')
alg.missingMassTool.CalibSet = '2016MC15C'
alg.missingMassTool.Decorate = True

from AthenaCommon.AppMgr import ToolSvc
from JetRecTools.JetRecToolsConf import CaloClusterConstituentsOrigin

clusterOrigin = CaloClusterConstituentsOrigin("CaloClusterConstitOrigin",
                                              InputType=1)
ToolSvc += clusterOrigin

addPrivateTool( alg, 'jetConstituentModSequence', 'JetConstituentModSequence')
alg.jetConstituentModSequence.InputContainer = 'CaloCalTopoClusters'
alg.jetConstituentModSequence.OutputContainer = 'LCOriginTopoClusters'
alg.jetConstituentModSequence.InputType = 1
alg.jetConstituentModSequence.Modifiers = [clusterOrigin]

# Add our algorithm to the main alg sequence
athAlgSeq += alg

# limit the number of events (for testing purposes)
theApp.EvtMax = 20000

#MessageSvc.OutputLevel = 1

# optional include for reducing printout from athena
include("AthAnalysisBaseComps/SuppressLogging.py")

