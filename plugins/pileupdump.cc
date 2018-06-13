// -*- C++ -*-
//
// Package:    DYJets2LL_study/pileupdump
// Class:      pileupdump
// 
/**\class pileupdump pileupdump.cc DYJets2LL_study/pileupdump/plugins/pileupdump.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arnab Purohit
//         Created:  Wed, 13 Jun 2018 11:49:24 GMT
//
//


// system include files
#include <memory>
#include "TH1.h"//New                                                                                           
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
//#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"

#include <iostream>
#include <vector>

/* Analyzer for dumping the MC PU distribution in a root file
--- By Arnab Purohit(arnab.purohit@cern.ch)
*/
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.
using namespace std;
using namespace edm;

float genWeight_;
Int_t            nPUInfo_;
vector<int>      nPU_;
vector<int>      puBX_;
vector<float>    puTrue_;



class pileupdump : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit pileupdump(const edm::ParameterSet&);
      ~pileupdump();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      edm::EDGetTokenT<GenEventInfoProduct> generatorLabel_;
      edm::EDGetTokenT<vector<PileupSummaryInfo> > puCollection_;
      edm::EDGetTokenT<vector<reco::GenParticle> > genParticlesCollection_;
      TTree   *tree_;
  //TH1F    *hEvents_;
      TH1F    *hPU_;
      TH1F    *hPUTrue_;
      TH1F    *hGenWeight_;
      TH1F *hSumGenWeight_;
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
pileupdump::pileupdump(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  generatorLabel_            = consumes<GenEventInfoProduct>           (iConfig.getParameter<InputTag>("generatorLabel"));
  puCollection_              = consumes<vector<PileupSummaryInfo> >    (iConfig.getParameter<InputTag>("pileupCollection"));
  genParticlesCollection_ = consumes<vector<reco::GenParticle> > (iConfig.getParameter<InputTag>("genParticleSrc"));
   usesResource("TFileService");
   //hPU = new TH1F("pileup","pileup",100,0,100);
   Service<TFileService> fs;
   tree_ = fs->make<TTree>("PUTree", "Event data");
   tree_->Branch("genWeight", &genWeight_);
   tree_->Branch("nPUInfo",       &nPUInfo_);
   tree_->Branch("nPU",           &nPU_);
   tree_->Branch("puBX",          &puBX_);
   tree_->Branch("puTrue",        &puTrue_);

   hPU_        = fs->make<TH1F>("hPU",        "number of pileup",      200,  0, 200);
   hPUTrue_    = fs->make<TH1F>("hPUTrue",    "number of true pilepu", 100, 0, 200);
   hGenWeight_ = fs->make<TH1F>("hGenWeight", "Gen weights",           2,    0, 2);
   hSumGenWeight_ = fs->make<TH1F>("hSumGenWeight", "Sum of Gen weights",1, 0, 1);
}


pileupdump::~pileupdump()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
pileupdump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   genWeight_ = -99;
   nPUInfo_ = 0;
   nPU_          .clear();
   puBX_         .clear();
   puTrue_       .clear();
   double w = 1.0;

   edm::Handle<GenEventInfoProduct> genEventInfoHandle;
   iEvent.getByToken(generatorLabel_, genEventInfoHandle);

   if (genEventInfoHandle.isValid()) {
     genWeight_ = genEventInfoHandle->weight();
     if (genWeight_ >= 0) hGenWeight_->Fill(0.5);    
     else hGenWeight_->Fill(1.5);
     if (abs(genWeight_)>1) hSumGenWeight_->Fill(0.5,genWeight_/abs(genWeight_));
     else hSumGenWeight_->Fill(0.5,genWeight_);
     if (genEventInfoHandle->weight() < 0.0) {
       w = -1.0;
     }
   } else
     edm::LogWarning("pileupdump") << "no GenEventInfoProduct in event";

   edm::Handle<vector<PileupSummaryInfo> > genPileupHandle;
   iEvent.getByToken(puCollection_, genPileupHandle);

   if (genPileupHandle.isValid()) {
     for (vector<PileupSummaryInfo>::const_iterator pu = genPileupHandle->begin(); pu != genPileupHandle->end(); ++pu) {
       if (pu->getBunchCrossing() == 0) {
	 hPU_->Fill(pu->getPU_NumInteractions(),w);
	 hPUTrue_->Fill(pu->getTrueNumInteractions(),w);
       }

       nPU_   .push_back(pu->getPU_NumInteractions());
       puTrue_.push_back(pu->getTrueNumInteractions());
       puBX_  .push_back(pu->getBunchCrossing());

       nPUInfo_++;
     }
   }
   else
     edm::LogWarning("pileupdump") << "no PileupSummaryInfo in event";

   tree_->Fill();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
pileupdump::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
pileupdump::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
pileupdump::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(pileupdump);
