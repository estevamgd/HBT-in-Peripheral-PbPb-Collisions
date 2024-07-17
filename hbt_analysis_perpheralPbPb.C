#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"


void hbt_analysis_perpheralPbPb() {
    // Open the input file
    TFile* fr = new TFile("HiForestAOD_UPC.root", "READ");

    // Get the tree from demo/HBT
    TTree* t;
    fr->GetObject("demo/HBT", t);

    /* Variables to read from the tree: HFsumET, Ntrk, trkPt, trkEta, trkPhi, trkPtRes, 
    trkDzSig, trkDxySig, trkNpixLayers, qinvSigSS, coulombWSS, qinvSigOS, coulombWOS*/ 
    const char* varNames[] = {"HFsumET", "Ntrk", "trkPt", "trkEta", "trkPhi", "trkPtRes", "trkDzSig", 
    "trkDxySig", "trkNpixLayers", "qinvSigSS", "coulombWSS", "qinvSigOS", "coulombWOS"};

    Int_t nNames = sizeof(varNames);
    
    // Create histograms
    TCanvas* c1 = new TCanvas("c1", "Histograms part 1", 1280, 720); // Canvas 1
    c1->Divide(3,2);

    for (int i = 1; i < 7; i++) {
        c1->cd(i); t->Draw(varNames[i-1]);
    }

    TCanvas* c2 = new TCanvas("c2", "Histograms part 2", 1280, 720); // Canvas 2
    c2->Divide(3,2);

    for (int i = 1; i < 7; i++) {
        c2->cd(i); t->Draw(varNames[i+6]);
    }

    TCanvas* c3 = new TCanvas("c3", "Histograms part 3", 1280, 720); // Canvas 3
    c3->cd(1); t->Draw("coulombWOS");
    

    // Save the canvas
    c1->SaveAs("./imgs/img-11.eps");
    c2->SaveAs("./imgs/img-22.eps");
    c3->SaveAs("./imgs/img-33.eps");

    // Close the file
    c1->Close();
    c2->Close();
    c3->Close();
    
    fr->Close();

}