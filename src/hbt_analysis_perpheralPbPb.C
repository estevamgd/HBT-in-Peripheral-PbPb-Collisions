#include "TFile.h"
#include "TTree.h"
#include <TROOT.h>
#include <TF1.h>
#include <TMath.h>
#include <TMath.h>
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <TStyle.h>
#include "TLegend.h"
#include <TText.h>
#include <TBenchmark.h>
#include <TProfile.h>
#include "../include/my_func.h"
#include "../include/normalizer.h"
#include "../include/analyze_tools.h"

void hbt_analysis_perpheralPbPb() {
    // Keeping track of time
    time_t ttime1 = time(NULL);
    struct tm date1 = *localtime(&ttime1);
    std::cout << "Start: " << date1.tm_mday << "/" << date1.tm_mon + 1 << "/" << date1.tm_year + 1900 <<  " " 
        << date1.tm_hour << ":" << date1.tm_min << ":" << date1.tm_sec << std::endl;

    TFile *fr = nullptr;
    TTree *t = nullptr;

    getFileTree("data/HiForestAOD_UPC.root", "demo/HBT", fr, t);

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

    // Getting how many entries
    Long64_t nentries = t->GetEntries();

    // Setting canvases
    TCanvas *c1 = new TCanvas("c1", "Histograms", 7680, 4320);
    c1->Divide(3, 2);

    TCanvas *c2 = new TCanvas("c2", "Histograms", 7680, 4320);
    c2->Divide(3, 2);

    TCanvas *c3 = new TCanvas("c3", "Histograms", 7680, 4320);

    TCanvas *canvases[] = {c1, c2, c3};
    int numCanvases = 3;

    // Setting histograms
    TH1D *h1 = cHist("h1", "HFsumET[GeV]", "#Events/bin", nentries, -20, 400);
    TH1D *h2 = cHist("h2", "coulombWOS", "#Pairs/bin", 10000, 0.4, 1.18);
    TH1D *h3 = cHist("h3", "coulombWSS", "#Pairs/bin", 10000, 0.8, 3.2);
    TH1D *h4 = cHist("h4", "Ntrk", "#Events/bin", 10000, -20, 500);
    TH1D *h5 = cHist("h5", "qinvSigOS[GeV]", "#Pairs/bin", nentries, -0.1, 1.23);
    TH1D *h6 = cHist("h6", "qinvSigSS[GeV]", "#Pairs/bin", nentries, -0.1, 1.3);
    TH1D *h7 = cHist("h7", "pT[GeV]", "#Tracks/bin", 10000, -1, 45);
    TH1D *h8 = cHist("h8", "trkEta", "#Tracks/bin", nentries, -2.6, 2.6);
    TH1D *h9 = cHist("h9", "trkPhi", "#Tracks/bin", nentries, -3.4, 3.4);
    TH1D *h10 = cHist("h10", "trkPtRes", "#Tracks/bin", nentries, 0, 0.11);
    TH1D *h11 = cHist("h11", "trkDxySig", "#Tracks/bin", nentries, -3.5, 3.5);
    TH1D *h12 = cHist("h12", "trkDzSig", "#Tracks/bin", nentries, -3.5, 3.5);
    TH1D *h13 = cHist("h13", "trkNpixLayers", "#Tracks/bin", 100, 0, 5);

    TH1D *histograms[] = {h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12, h13};
    int numHistograms = 13;

    // Filling histograms
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        h1->Fill(HFsumET);
        h4->Fill(Ntrk);

        for (int j = 0; j < Ntrk; j++) {
            h7->Fill(trkPt[j]);
            h8->Fill(trkEta[j]);
            h9->Fill(trkPhi[j]);
            h10->Fill(trkPtRes[j]);
            h11->Fill(trkDxySig[j]);
            h12->Fill(trkDzSig[j]);
            h13->Fill(trkNpixLayers[j]);
        }
        for (int k = 0; k < NOSpair; k++) {
            h2->Fill(coulombWOS[k]);
            h5->Fill(qinvSigOS[k]);
        }
        for (int l = 0; l < NSSpair; l++) {
            h3->Fill(coulombWSS[l]);
            h6->Fill(qinvSigSS[l]);
        }

        // Display progress
        float progress = (float)(i + 1) / nentries * 100;
        std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
    }

    // Drawing histograms
    c1->cd(1); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h1->Draw(); 
    c1->cd(2); gPad->SetGrid(); gPad->SetLogy(); gPad->SetLeftMargin(0.15); h2->Draw(); 
    c1->cd(3); gPad->SetGrid(); gPad->SetLogy(); gPad->SetLeftMargin(0.15); h3->Draw(); 
    c1->cd(4); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h4->Draw(); 
    c1->cd(5); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h5->Draw(); 
    c1->cd(6); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h6->Draw(); 
    c2->cd(1); gPad->SetGrid(); gPad->SetLogy(); gPad->SetLeftMargin(0.15); h7->Draw(); 
    c2->cd(2); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h8->Draw(); 
    c2->cd(3); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h9->Draw(); 
    c2->cd(4); gPad->SetGrid(); gPad->SetLogy(); gPad->SetLeftMargin(0.15); h10->Draw(); 
    c2->cd(5); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h11->Draw(); 
    c2->cd(6); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h12->Draw(); 
    c3->cd(); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h13->Draw(); 

    // Save canvas images
    const char *path = "./imgs/teste/";
    const char *prefix = "all-histograms";
    const char *file_type = "png";

    save_canvas_images(canvases, numCanvases, path, prefix, file_type);

    // close file
    close_program(canvases, numCanvases, histograms, numHistograms, fr);
}