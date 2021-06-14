# The name of the package:
atlas_subdir (DiTau)

# Add the share library:
atlas_add_library (DiTauLib
  DiTau/*.h Root/*.cxx
  PUBLIC_HEADERS DiTau
  LINK_LIBRARIES AnaAlgorithmLib tauRecToolsLib AsgTools DiTauRec TauAnalysisToolsLib JetRecToolsLib)

if (XAOD_STANDALONE)
  # Add the dictionary (for AnalysisBase only):
  atlas_add_dictionary (DiTauDict
   DiTau/DiTauDict.h
   DiTau/selection.xml
   LINK_LIBRARIES DiTauLib)
endif ()

if (NOT XAOD_STANDALONE)
  # Add a component library for AthAnalysis only:
  atlas_add_component (DiTau
    src/components/*.cxx
    LINK_LIBRARIES DiTauLib)
endif()

# Install files from the package:
atlas_install_joboptions( share/*_jobOptions.py )
atlas_install_joboptions( share/*_eljob.py )

