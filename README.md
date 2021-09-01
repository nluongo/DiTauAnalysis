# DiTauAnalysis


author: Nicholas Luongo  
contact: nicholas.andrew.luongo@cern.ch

A package for reconstructing and identifying DiTau objects

              
Setup
=====


```
git atlas init-workdir https://:@gitlab.cern.ch:8443/atlas/athena.git
cd athena
git clone https://gitlab.cern.ch/nicholas/ditauanalysis.git
mkdir build run
cd build
asetup AthAnalysis,21.2,latest
cmake ../athena/Projects/WorkDir
make
source x86_64-centos7-gcc8-opt/setup.sh
cd ../run
athena DiTau/diTau_jobOptions.py
```
