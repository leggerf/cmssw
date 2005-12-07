// -*- C++ -*-
//
// Class:      TestSuite
// 
/**\class TestSuite

 Description: test suite for Mixing Module

*/
//
// Original Author:  Ursula Berthon
//         Created:  Fri Sep 23 11:38:38 CEST 2005
// $Id: TestSuite.cc,v 1.1 2005/12/06 10:18:53 uberthon Exp $
//
//


// system include files
#include <memory>
#include <utility>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimGeneral/MixingModule/interface/TestSuite.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "TH1I.h"

using namespace edm;

TestSuite::TestSuite(const edm::ParameterSet& iConfig):  bunchcr_(iConfig.getParameter<int>("BunchNr")) ,filename_(iConfig.getParameter<std::string>("fileName"))
{
  std::cout << "Constructed testSuite , bunchcr "<<bunchcr_<<" filename: "<<filename_<<std::endl;

  histfile_ = new TFile(filename_.c_str(),"UPDATE");
}


TestSuite::~TestSuite()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  histfile_->ls();
  histfile_->Write();
  histfile_->Close();

}


//
// member functions
//

// ------------ method called to analyze the data  ------------
void
TestSuite::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

// Get input
    edm::Handle<CrossingFrame> cf;
    iEvent.getByType(cf);

// use MixCollection and its iterator
// Please note that bunch() and getTrigger() are methods of the iterator itself
// while operator-> points to the templated objects!!!!

//track histo
    char histotracks[10],histotracksindsig[22],histotracksind[15];
    sprintf(histotracks,"Tracks_%d",bunchcr_);
    sprintf(histotracksind,"VtxPointers_%d",bunchcr_);
    sprintf(histotracksindsig,"VtxPointers_signal_%d",bunchcr_);
    TH1I * trhist = new TH1I(histotracks,"Bunchcrossings",8,-5,3);
    TH1I * trindhist = new TH1I(histotracksind,"Track to Vertex indices",100,0,500);
    TH1I * trindhistsig = new TH1I(histotracksindsig,"Signal Track to Vertex indices",100,0,500);
    std::auto_ptr<MixCollection<EmbdSimTrack> > col1(new MixCollection<EmbdSimTrack>(cf.product()));
    MixCollection<EmbdSimTrack>::iterator cfi1;
    for (cfi1=col1->begin(); cfi1!=col1->end();cfi1++) {
      trhist->Fill(cfi1.bunch());
      trindhist->Fill(cfi1->vertIndex());
      if (cfi1.getTrigger())  trindhistsig->Fill(cfi1->vertIndex());
     }

//vertex histo
//     char histovertices[10],histovertexindices[17],histovertexindicessig[24];
//     sprintf(histovertices,"Vertices_%d",bunchcr_);
//     sprintf(histovertexindices,"TrackPointers_%d",bunchcr_);
//     sprintf(histovertexindicessig,"TrackPointers_signal_%d",bunchcr_);
//     TH1I * vtxhist = new TH1I(histovertices,"Bunchcrossings",8,-5,3);
//     TH1I * vtxindhist = new TH1I(histovertexindices,"Vertex to Track Indices",100,0,500);
//     TH1I * vtxindhistsig = new TH1I(histovertexindicessig,"SignalVertex to Track Indices",100,0,500);
//     std::auto_ptr<MixCollection<EmbdSimVertex> > col2(new MixCollection<EmbdSimVertex>(cf.product()));
//     MixCollection<EmbdSimVertex>::iterator cfi2;
//     for (cfi2=col2->begin(); cfi2!=col2->end();cfi2++) {
//       vtxhist->Fill(cfi2.bunch());
//       vtxindhist->Fill(cfi2->parentIndex());
//       if (cfi2.getTrigger())  vtxindhistsig->Fill(cfi2->parentIndex());
//     }
// 
	
 //PSimHit histo
    char psimhitvertices[10],psimhitvertexindices[17],psimhitvertexindicessig[24];
    sprintf(psimhitvertices,"Vertices_%d",bunchcr_);
    sprintf(psimhitvertexindices,"TrackPointers_%d",bunchcr_);
    sprintf(psimhitvertexindicessig,"TrackPointers_signal_%d",bunchcr_);
    TH1I * vtxhist = new TH1I(psimhitvertices,"Bunchcrossings",8,-5,3);
    TH1I * vtxindhist = new TH1I(psimhitvertexindices,"Vertex to Track Indices",100,0,500);
    TH1I * vtxindhistsig = new TH1I(psimhitvertexindicessig,"SignalVertex to Track Indices",100,0,500);
    std::auto_ptr<MixCollection<EmbdSimVertex> > col2(new MixCollection<EmbdSimVertex>(cf.product()));
    MixCollection<EmbdSimVertex>::iterator cfi2;
    for (cfi2=col2->begin(); cfi2!=col2->end();cfi2++) {
      vtxhist->Fill(cfi2.bunch());
      vtxindhist->Fill(cfi2->parentIndex());
      if (cfi2.getTrigger())  vtxindhistsig->Fill(cfi2->parentIndex());
    }
}

