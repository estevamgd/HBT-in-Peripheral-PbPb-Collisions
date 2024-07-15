#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"


void hbt_analysis_perpheralPbPb() {

    // Open the input file
    TFile* fr = new TFile("HiForestAOD_UPC.root", "READ");

    // Get the tree from demo/HBT
    TTree* t;
    fr->GetObject("demo/HBT", t);

    /*Declare variables to read from the tree: HFsumET, 
    Ntrk, trkPt, trkEta, trkPhi, trkPtRes, trkDzSig, trkDxySig, 
    trkNpixLayers, qinvSigSS, coulombWSS, qinvSigOS, coulombWOS*/ 
    
    Float_t HFsumET, trkPt, trkEta, trkPhi, trkPtRes, trkDzSig, trkDxySig, 
    trkNpixLayers, qinvSigSS, coulombWSS, qinvSigOS, coulombWOS;

    Int_t Ntrk;

    // Set branches
    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
    t->SetBranchAddress("trkPt", &trkPt);
    t->SetBranchAddress("trkEta", &trkEta);
    t->SetBranchAddress("trkPhi", &trkPhi);
    t->SetBranchAddress("trkPtRes", &trkPtRes);
    t->SetBranchAddress("trkDzSig", &trkDzSig);
    t->SetBranchAddress("trkDxySig", &trkDxySig);
    t->SetBranchAddress("trkNpixLayers", &trkNpixLayers);
    t->SetBranchAddress("qinvSigSS", &qinvSigSS);
    t->SetBranchAddress("coulombWSS", &coulombWSS);
    t->SetBranchAddress("qinvSigOS", &qinvSigOS);
    t->SetBranchAddress("coulombWOS", &coulombWOS);

    // Create histograms
    TCanvas* c1 = new TCanvas("c1", "Histograms part 1", 1280, 720);
    c1->Divide(2,3);

    c1->cd(1); t->Draw("HFsumET"); 
    c1->cd(2); t->Draw("Ntrk"); 
    c1->cd(3); t->Draw("trkPt"); 
    
    TCanvas* c2 = new TCanvas("c2", "Histograms part 2", 1280, 720);
    c2->Divide(2,4);

    // Save the canvas

    // Close the file
    fr->Close();

    // Clean up memory
    
}