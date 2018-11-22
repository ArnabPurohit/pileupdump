# pileupdump
Analyzer for dumping the MC PU distribution in a root file
Here is the "How to use this repo":

cmsrel CMSSW_9_4_9
cd CMSSW_9_4_9/src
cmsenv
git cms-init
cd $CMSSW_BASE/src
mkdir forPU
cd forPU
git clone https://github.com/ArnabPurohit/pileupdump pileupdump
cd $CMSSW_BASE/src
scram b -j 4

If everything works fine, now you can submit crab jobs to get an ROOT file containing few histograms of PU distributions
and a tree.
File to Run: tasty_crab.py (Please change this file to specify the dataset, storage ...)

Commands to submit job in crab:
source /cvmfs/cms.cern.ch/crab3/crab.sh
cd forPU/pileupdump/test
python tasty_crab.py
