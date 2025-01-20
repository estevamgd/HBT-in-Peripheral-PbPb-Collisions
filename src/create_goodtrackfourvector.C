#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TBenchmark.h"
#include "TLegend.h"

#include "../include/data_func.h"
#include "../include/my_func.h"

void create_goodtrackfourvector() {
    ROOT::EnableImplicitMT();
    auto poolSize = ROOT::GetThreadPoolSize();
    std::cout << "Pool size = " << poolSize << std::endl;

    // Load the ROOT file and tree
    TFile *fr = nullptr;
    TTree *t = nullptr;
    getFileTree("data/HiForestAOD_UPC.root", "demo/HBT", fr, t);

    // Variables from the tree
    Int_t maxSize = 1700, Ntrk;
    Float_t HFsumET, trkPt[maxSize], trkEta[maxSize], trkPhi[maxSize], pionMass = 0.13957;

    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
    t->SetBranchAddress("trkPt", trkPt);
    t->SetBranchAddress("trkEta", trkEta);
    t->SetBranchAddress("trkPhi", trkPhi);

    // Getting how many entries
    Long64_t nentries = t->GetEntries();

    // Creating a new ROOT file to store the output
    TFile *outputFile = new TFile("data/output_particles.root", "RECREATE");

    // Creating a new TTree to store particles
    TTree *tree = new TTree("particles", "Tree with particle four-vectors");

    // Defining the structure to store the TLorentzVector objects
    std::vector<TLorentzVector> particles;
    tree->Branch("particles", &particles);

    // Setting Lorentz Vector
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);
        particles.clear();
        std::vector<TLorentzVector> particles;

        if (HFsumET > 100 && HFsumET < 375) continue;
        for (int j = 0; j < Ntrk; j++) {
            TLorentzVector vec;
            vec.SetPtEtaPhiM(trkPt[j], trkEta[j], trkPhi[j], pionMass);
            particles.push_back(vec);
            std::cout << "nentries: " << nentries << " i: " << i << " Ntrk: " << Ntrk << " j: " << j << std::endl;
        }

        tree->Fill();

    }

    // Writing the particles in the root file
    outputFile->Write();
    outputFile->Close();
    
    std::cout << "particles saved on the root file!" << std::endl;
}