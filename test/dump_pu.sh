#!/bin/sh
echo "I shall dump entries in the bins of true pile-up (pile-up) distribution from MC (data)" 
echo -n "would you like to dump Data pu?[Y/N] "
read doData
if [ "${doData,,}" = "y" -o "${doData,,}" = "yes" ]
then
    echo -n "Do you have Data pu file?[Y/N] "
    read haveData
    if [ "${haveData,,}" = "n" -o "${haveData,,}" = "no" ]
    then
	echo "Which Data Lumi JSon file? press enter to use default json: "
	read datajson
	echo "running command pileupCalc.py -i $datajson  --inputLumiJSON  /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69200  --maxPileupBin 100 --numPileupBins 100  data_pileup.root"
	if [ "$datajson" = "" ]
	then
	    echo "Using default Json /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt"
	    pileupCalc.py -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt --inputLumiJSON  /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69200  --maxPileupBin 100 --numPileupBins 100  data_pileup.root
	else
	    pileupCalc.py -i $datajson  --inputLumiJSON  $datajson --calcMode true --minBiasXsec 69200  --maxPileupBin 100 --numPileupBins 100  data_pileup.root
	fi
    fi
    sed -i 's,true,false,g' dumpbincontent.C
    root -l -b dumpbincontent.C > data_pileup.txt &
fi
sed -i 's,false,true,g' dumpbincontent.C
root -l -b dumpbincontent.C > MC_pileup.txt &