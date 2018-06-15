#include "TH1.h"
#include "TFile.h"


void dumpbincontent(){
  bool isMC = true;
 
  //if(!isMC) std::cout<< "Hi Prasant" <<std::endl;

  if(!isMC){
    TFile *file = TFile::Open("data_pileup.root");
    TH1D* h = (TH1D*) file->Get("pileup");
    //std::cout<< h->Integral()<<std::endl;
    std::cout << "Data pileup : [";
    for (int i=1; i<=h->GetNbinsX();i++){
      if(isMC)  std::cout << h->GetBinContent(i)/h->Integral(-1,-1);
      else std::cout << h->GetBinContent(i);
      if(i!=h->GetNbinsX()) std::cout << ",";   
    }
    std::cout << "]" <<std::endl;
  }

  if(isMC){
    TFile *file = TFile::Open("pileup_ggH70.root");
    TH1D* h = (TH1D*) file->Get("pileupdump/hPUTrue");
    std::cout << "MC pileup : [";
    for (int i=1; i<=h->GetNbinsX();i++){
      if(isMC)  std::cout << h->GetBinContent(i)/h->Integral(-1,-1);
      else std::cout << h->GetBinContent(i);
      if(i!=h->GetNbinsX()) std::cout << ",";   
    }
    std::cout << "]" <<std::endl;
  }
  gSystem->Exit(0);
}
