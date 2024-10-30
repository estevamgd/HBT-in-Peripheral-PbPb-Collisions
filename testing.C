#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <TStyle.h>
#include "my_func.h"
#include "TLegend.h"
#include "normalizer.h"
#include "analyze_tools.h"
#include <TText.h>
#include "TStopwatch.h"


void testing() {
    // Setting up, copy from HERE to...
    // Getting TTree
    TFile *fr = nullptr;
    TTree *t = nullptr;

    getFileTree("HiForestAOD_UPC.root", "demo/HBT", fr, t);
    
    // Variables
    Int_t maxSize = 17100;
    Int_t Ntrk = maxSize, NSSpair = maxSize, NOSpair = maxSize;
    Float_t HFsumET;
    Float_t coulombWOS[NOSpair], coulombWSS[NSSpair], qinvSigSS[NSSpair], qinvSigOS[NOSpair], 
            trkPt[Ntrk], trkEta[Ntrk], trkPhi[Ntrk], trkPtRes[Ntrk], 
            trkDxySig[Ntrk], trkNpixLayers[Ntrk], trkDzSig[Ntrk];

    // Arrays of variables
    void* variables[] = {
        &HFsumET, &Ntrk, &NSSpair, &NOSpair, coulombWOS, coulombWSS, qinvSigOS, qinvSigSS, 
        trkPt, trkEta, trkPhi, trkPtRes, trkDxySig, trkDzSig, trkNpixLayers
    };

    // Branch names
    const char* branchNames[] = {
        "HFsumET", "Ntrk", "NSSpair", "NOSpair", "coulombWOS", "coulombWSS", 
        "qinvSigOS", "qinvSigSS", "trkPt", "trkEta", "trkPhi", "trkPtRes", 
        "trkDxySig", "trkDzSig", "trkNpixLayers"
    };

    // Setting addresses
    int numBranches = sizeof(variables) / sizeof(variables[0]);
    for (int i = 0; i < numBranches; i++) {
        t->SetBranchAddress(branchNames[i], variables[i]);
    }

    // ...HERE
    // Getting how many entries
    Long64_t nentries = t->GetEntries();

    // Setting histograms
    TH1D *h1 = cHist("h1", "qinv[GeV]", "#Pairs/bin", nentries, -0.1, 1.1, 0, 0, 0, 920);
    TH1D *h2 = cHist("h2", "qinv[GeV]", "#Pairs/bin", nentries, -0.1, 1.1, 0, 0, 0, 920);
    TH1D *h3 = cHist("h3", "", "", nentries, -0.1, 1.1, 0, 0, 0, 632);
    TH1D *h4 = cHist("h4", "", "", nentries, -0.1, 1.1, 0, 0, 0, 632);
    // Create a stopwatch instance
    TStopwatch stopwatch;

    // Start the stopwatch
    stopwatch.Start();

    ROOT::EnableImplicitMT();
    // Filling histograms
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        if (HFsumET > 100 && HFsumET < 375) { // This selects a centrality
            for (int k = 0; k < NOSpair; k++) {
                h1->Fill(qinvSigOS[k]);
                h3->Fill(qinvSigOS[k], coulombWOS[k]);

            }
            for (int l = 0; l < NSSpair; l++) {
                h2->Fill(qinvSigSS[l]);
                h4->Fill(qinvSigSS[l], coulombWSS[l]);
            }    
        }

        // Display progress
        float progress = (float)(i + 1) / nentries * 100;
        std::cout << "\rFilling Progress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
    }
    ROOT::DisableImplicitMT();
    // Stop the stopwatch
    stopwatch.Stop();

    // Get the real (wall clock) time and CPU time
    double realTime = stopwatch.RealTime();
    double cpuTime = stopwatch.CpuTime();

    // Print the times
    std::cout << "Real time: " << realTime << " seconds" << std::endl;
    std::cout << "CPU time: " << cpuTime << " seconds" << std::endl;

    delete h1;
    delete h2;
    delete h3;
    delete h4;
}