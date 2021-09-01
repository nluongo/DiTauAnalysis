#See: https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/SoftwareTutorialxAODAnalysisInCMake for more details about anything here

from glob import glob

print('In jobOptions')

sample = 'X2000AOD'

if sample == 'X2000AOD':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.450524.MadGraphPythia8EvtGen_A14NNPDF23LO_X2000tohh_bbtautau_lephad.recon.AOD.e7244_s3126_r10201/*')
    outName = 'ditau_450524_X2000tohh_bbtautau_lephad.recon.AOD.ntuple.root'
    isSignal = 1
elif sample == 'X2000ESD':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/bbtautau/run3Trigger/r3athanalysis/samples/mc16_13TeV.450524.MadGraphPythia8EvtGen_A14NNPDF23LO_X2000tohh_bbtautau_lephad.recon.ESD.e7244_e5984_s3126_r12406/*')
    outName = 'ditau_450524_X2000tohh_bbtautau_lephad.recon.ESD.ntuple.root'
    isSignal = 1
elif sample == 'Z+jets':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.364139.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_BFilter.recon.AOD.e5313_s3126_r10201/*')
    outName = 'background_364139_Ztautau_MAXHTPTV280_500_BFilter.recon.AOD.ntuple.root'
    isSignal = 0
elif sample == 'ttbar':
    myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.recon.AOD.e6337_e5984_s3126_r10201/*')
    outName = 'background_410470_ttbar_nonallhad.recon.AOD.ntuple.root'
    isSignal = 0
#myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.425105.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_4tau_c10_M3000.recon.AOD.e6072_s3126_r10724/*.root.1')
#outName = '../samples/ditau_425105_G_hh_4tau_M3000.recon.AOD.ntuple.root'

#myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/group.perf-tau.425104.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_4tau_c10_M2500.R22_NewTunes_EXT0/*')
#outName = 'ditau_425104_4tau.AOD.ntuple.root'

isESD = 1

#override next line on command line with: --filesInput=XXX
jps.AthenaCommonFlags.FilesInput = myFiles

from PyUtils import AthFile
import AthenaPoolCnvSvc.ReadAthenaPool
from AthenaCommon.AthenaCommonFlags import athenaCommonFlags

#Specify AccessMode (read mode) ... ClassAccess is good default for xAOD
if not isESD:
    jps.AthenaCommonFlags.AccessMode = "ClassAccess" 
else:
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
algClass = getattr(CfgMgr, 'DiTauAnalysis')
alg = algClass('AnalysisAlg')

# later on we'll add some configuration options for our algorithm that go here
#alg.outName = outName
alg.isSignal = isSignal

# Add our algorithm to the main alg sequence
athAlgSeq += alg

# limit the number of events (for testing purposes)
theApp.EvtMax = -1

#MessageSvc.OutputLevel = 1

# optional include for reducing printout from athena
include("AthAnalysisBaseComps/SuppressLogging.py")

