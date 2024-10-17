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

void coulomb_correction() {
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

    // Setting canvases
    TCanvas *c1 = new TCanvas("c1", "", 7680, 4320);
    TCanvas *c2 = new TCanvas("c2", "", 7680, 4320);
    TCanvas *c3 = new TCanvas("c3", "", 7680, 4320);

    TCanvas *canvases[] = {c1, c2, c3};
    int numCanvases = 3;

    // Setting histograms
    TH1D *h1 = cHist("h1", "qinv[GeV]", "#Pairs/bin", nentries, -0.1, 1.1, 0, 0, 0, 920);
    TH1D *h2 = cHist("h2", "qinv[GeV]", "#Pairs/bin", nentries, -0.1, 1.1, 0, 0, 0, 920);
    TH1D *h3 = cHist("h3", "", "", nentries, -0.1, 1.1, 0, 0, 0, 632);
    TH1D *h4 = cHist("h4", "", "", nentries, -0.1, 1.1, 0, 0, 0, 632);

    TH1D *tonormhist[] = {h1, h2, h3, h4};
    int numTonorm = 4;

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

    // Normalize histograms
    Double_t scale = 1;
    normalizer(tonormhist, numTonorm, scale);
    
    // Dividing SS/OS
    TH1D *sr = (TH1D *)h2->Clone("sr");
    TH1D *sr_cor = (TH1D *)h4->Clone("sr_cor");
    sr->Divide(h1);
    sr_cor->Divide(h3);

    // Adding Title
    sr->SetTitle("CMS Open Data 2011 - PbPb 2.76 TeV");
    h1->SetTitle("CMS Open Data 2011 - PbPb 2.76 TeV");
    h2->SetTitle("CMS Open Data 2011 - PbPb 2.76 TeV");

    // Adding labels to single ratio histogram
    sr->GetXaxis()->SetTitle("qinv[GeV]");
    sr->GetYaxis()->SetTitle("Single Ratio SS/OS");
    
    // Single Ratio Legend
    TLegend *h1_legend = new TLegend(0.2, 0.7, 0.4, 0.9);
    h1_legend->AddEntry((TObject*)0, "Invariant 4-momentum", "");
    h1_legend->AddEntry((TObject*)0, "Opposite charge pairs", "");
    h1_legend->AddEntry((TObject*)0, "Centrality: 50-70%", "");
    h1_legend->AddEntry(h1, "Without Coulomb Correction", "l");
    h1_legend->AddEntry(h3, "With Coulomb Correction", "l");

    // Single Ratio Legend
    TLegend *h2_legend = new TLegend(0.2, 0.7, 0.4, 0.9);
    h2_legend->AddEntry((TObject*)0, "Invariant 4-momentum", "");
    h2_legend->AddEntry((TObject*)0, "Same charge pairs", "");
    h2_legend->AddEntry((TObject*)0, "Centrality: 50-70%", "");
    h2_legend->AddEntry(h1, "Without Coulomb Correction", "l");
    h2_legend->AddEntry(h3, "With Coulomb Correction", "l");

    // Single Ratio Legend
    TLegend *sr_legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    sr_legend->AddEntry((TObject*)0, "Single Ratio SS/OS", "");
    sr_legend->AddEntry((TObject*)0, "Centrality: 50-70%", "");
    sr_legend->AddEntry(sr, "Without Coulomb Correction", "l");
    sr_legend->AddEntry(sr_cor, "With Coulomb Correction", "l");
    
    // Setting y range to 0.95<y<1.6
    sr->GetYaxis()->SetRangeUser(0.95, 1.6);
    sr_cor->GetYaxis()->SetRangeUser(0.95, 1.6);

    // Removing statistics box
    TH1D *histograms[] = {h1, h2, h3, h4, sr, sr_cor};
    int numHistograms = 6;

    no_statbox(histograms, numHistograms);

    // Drawing
    c1->cd(); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h1->Draw(); h3->Draw("same"); h1_legend->Draw();
    c2->cd(); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h2->Draw(); h4->Draw("same"); h2_legend->Draw();
    c3->cd(); gPad->SetGrid(); gPad->SetLeftMargin(0.15); sr->Draw(); sr_cor->Draw("same"); sr_legend->Draw();
    
    // Saving image
    const char *path = "./imgs/final/";
    const char *prefix = "final-coulomb-correction";
    const char *file_type = "png";
    save_canvas_images(canvases, numCanvases, path, prefix, file_type);

    // Closing program
    close_program(canvases, numCanvases, histograms, numHistograms, fr);
}
