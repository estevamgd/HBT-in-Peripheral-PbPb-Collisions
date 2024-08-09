#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <stdbool.h>

void hbt_analysis_perpheralPbPb() {
    // Open the input file
    TFile *fr = new TFile("HiForestAOD_UPC.root", "READ");
    if (!fr || fr->IsZombie()) {
        std::cerr << "Error: File could not be opened!" << std::endl;
        return;
    }

    // Get the tree from demo/HBT
    TTree *t;
    fr->GetObject("demo/HBT;570", t);
    if (!t) {
        std::cerr << "Error: Tree 'demo/HBT;570' not found!" << std::endl;
        return;
    }

    Float_t HFsumET, coulombWOS;

    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("coulombWOS", &coulombWOS);

    Long64_t nentries = t->GetEntries();

    TCanvas *c1 = new TCanvas("c1", "Histograms part 1", 1280, 720);
    c1->Divide(1,2);

    TH1D *h1 = new TH1D("h1", "HFsumET Histogram", 10000, 0, 400);
    TH1D *h2 = new TH1D("h2", "coulombWOS Histogram", 1000, 0, 2);

    for (int i=0;i<nentries;i++) {
        t->GetEntry(i);
        h1->Fill(HFsumET);
        h2->Fill(coulombWOS);
    }

    c1->cd(1); h1->Draw();
    c1->cd(2); h2->Draw();
    
}