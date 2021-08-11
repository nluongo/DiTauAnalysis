#See: https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/SoftwareTutorialxAODAnalysisInCMake for more details about anything here

from glob import glob

print('In jobOptions')

myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.450524.MadGraphPythia8EvtGen_A14NNPDF23LO_X2000tohh_bbtautau_lephad.recon.AOD.e7244_s3126_r10201/*')
outName = 'ditau_450524_X2000tohh_bbtautau_lephad.recon.AOD.ntuple.root'
#myFiles = glob('/eos/user/n/nicholas/SWAN_projects/DiTauReco/samples/mc16_13TeV.425105.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_4tau_c10_M3000.recon.AOD.e6072_s3126_r10724/*.root.1')
#outName = '../samples/ditau_425105_G_hh_4tau_M3000.recon.AOD.ntuple.root'

#override next line on command line with: --filesInput=XXX
jps.AthenaCommonFlags.FilesInput = myFiles

#Specify AccessMode (read mode) ... ClassAccess is good default for xAOD
jps.AthenaCommonFlags.AccessMode = "ClassAccess" 

jps.AthenaCommonFlags.HistOutputs = ["ANALYSIS:"+outName]

# Create the algorithm's configuration.
#from AnaAlgorithm.DualUseConfig import createAlgorithm
#alg = createAlgorithm ( 'DiTauAnalysis', 'AnalysisAlg' )
from AthenaCommon import CfgMgr
algClass = getattr(CfgMgr, 'DiTauAnalysis')
alg = algClass('AnalysisAlg')

# later on we'll add some configuration options for our algorithm that go here
#alg.outName = outName

# Add our algorithm to the main alg sequence
athAlgSeq += alg

# limit the number of events (for testing purposes)
theApp.EvtMax = -1

#MessageSvc.OutputLevel = 1

# optional include for reducing printout from athena
include("AthAnalysisBaseComps/SuppressLogging.py")

