#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <TStyle.h>

// Bin size calculator
int numBins(int numValues) {
    return (int)sqrt(numValues);
}

// Histogram layout
TH1D* cHist(const char* name, const char* xAxisTitle, const char* yAxisTitle, 
                      int nVal, double xMin, double xMax) {
    // Create the histogram
    int nBins = numBins(nVal);
    TH1D* h = new TH1D(name, "", nBins, xMin, xMax);

    // Set visual properties
    h->SetFillColor(kGray); 
    h->SetLineColor(kBlack); 
    h->SetLineWidth(1); 

    // Set titles and axis labels
    h->SetTitle("");
    h->GetXaxis()->SetTitle(xAxisTitle);
    h->GetYaxis()->SetTitle(yAxisTitle);
    
    // Set label sizes
    h->GetXaxis()->SetLabelSize(0.04);
    h->GetYaxis()->SetLabelSize(0.04);
    std::cerr << "works!" << std::endl;
    
    return h;
}


// Function to draw histograms on a canvas
void DrawHistograms(TCanvas* canvas, TH1D* histograms[], int nHists, bool logY[]) {

    for (int i = 0; i < nHists; i++) {
        canvas->cd(i + 1);
        gPad->SetGrid();
        gPad->SetLeftMargin(0.15);

        if (logY[i]) {
            gPad->SetLogy();
        }

        histograms[i]->Draw();
    }
}

// Filling class (future implementation)

void hbt_analysis_perpheralPbPb() {
    // Keeping track of time
    time_t ttime1 = time(NULL);
    struct tm date1 = *localtime(&ttime1);
    std::cout << "Start: " << date1.tm_mday << "/" << date1.tm_mon + 1 << "/" << date1.tm_year + 1900 <<  " " 
        << date1.tm_hour << ":" << date1.tm_min << ":" << date1.tm_sec << std::endl;


    // Open the input file
    TFile *fr = new TFile("HiForestAOD_UPC.root", "READ");
    if (!fr || fr->IsZombie()) {
        std::cerr << "Error: File could not be opened!" << std::endl;
        return;
    }

    // Get the tree from demo/HBT
    TTree *t;
    fr->GetObject("demo/HBT", t);
    if (!t) {
        std::cerr << "Error: Tree 'demo/HBT not found!" << std::endl;
        return;
    }

    // Variables
    Int_t Ntrk, NSSpair, NOSpair;
    Float_t HFsumET;
    Float_t coulombWOS[NOSpair], coulombWSS[NSSpair], qinvSigSS[NSSpair], qinvSigOS[NOSpair], 
            trkPt[Ntrk], trkEta[Ntrk], trkPhi[Ntrk], trkPtRes[Ntrk], 
            trkDxySig[Ntrk], trkNpixLayers[Ntrk], trkDzSig[Ntrk];
    
    // Setting branches
    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
    t->SetBranchAddress("NSSpair", &NSSpair);
    t->SetBranchAddress("NOSpair", &NOSpair);
    t->SetBranchAddress("coulombWOS", coulombWOS);
    t->SetBranchAddress("coulombWSS", coulombWSS);
    t->SetBranchAddress("qinvSigOS", qinvSigOS);
    t->SetBranchAddress("qinvSigSS", qinvSigSS);
    t->SetBranchAddress("trkPt", trkPt);
    t->SetBranchAddress("trkEta", trkEta);
    t->SetBranchAddress("trkPhi", trkPhi);
    t->SetBranchAddress("trkPtRes", trkPtRes);
    t->SetBranchAddress("trkDxySig", trkDxySig);
    t->SetBranchAddress("trkDzSig", trkDzSig);
    t->SetBranchAddress("trkNpixLayers", trkNpixLayers);

    // Getting how many entries
    Long64_t nentries = t->GetEntries();

    // Setting canvases 
    TCanvas *c1 = new TCanvas("c1", "Histograms", 7680, 4320);
    c1->Divide(3, 2);

    TCanvas *c2 = new TCanvas("c2", "Histograms", 7680, 4320);
    c2->Divide(3, 2);

    TCanvas *c3 = new TCanvas("c3", "Histograms", 7680, 4320);

    // Setting histogram
    TH1D *h1 = cHist("h1", "HFsumET[GeV]", "#Events/bin", nentries, -0.1, 400);
    TH1D *h2 = cHist("h2", "coulombWOS", "#Pairs/bin", nentries, 0.3, 1.1);
    TH1D *h3 = cHist("h3", "coulombWSS", "#Pairs/bin", nentries, 0.8, 3.5);
    TH1D *h4 = cHist("h4", "Ntrk", "#Events/bin", nentries, -0.1, 500);
    TH1D *h5 = cHist("h5", "qinvSigOS[GeV]", "#Pairs/bin", nentries, -0.1, 1.1);
    TH1D *h6 = cHist("h6", "qinvSigSS[GeV]", "#Pairs/bin", nentries, -0.1, 1.1);
    TH1D *h7 = cHist("h7", "pT[GeV]", "#Tracks/bin", nentries, -0.1, 21);
    TH1D *h8 = cHist("h8", "trkEta", "#Tracks/bin", nentries, -2.6, 2.6);
    TH1D *h9 = cHist("h9", "trkPhi", "#Tracks/bin", nentries, -3.4, 3.4);
    TH1D *h10 = cHist("h10", "trkPtRes", "#Tracks/bin", nentries, 0, 0.11);
    TH1D *h11 = cHist("h11", "trkDxySig", "#Tracks/bin", nentries, -3.5, 3.5);
    TH1D *h12 = cHist("h12", "trkDzSig", "#Tracks/bin", nentries, -3.5, 3.5);
    TH1D *h13 = cHist("h13", "trkNpixLayers", "#Tracks/bin", 100, 0, 5);
    
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

        // Calculate percentage
        float progress = (float)(i + 1) / nentries * 100;

        // Display percentage, overwrite previous percentage using '\r'
        std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
    }
    
    // Move to the next line after finishing
    std::cout << std::endl;
    
    // Array of histograms and logY flags for each canvas
    TH1D* histograms1[] = {h1, h2, h3, h4, h5, h6};
    bool logY1[] = {false, true, true, false, false, false};
    int nHists1 = sizeof(histograms1) / sizeof(histograms1[0]);

    TH1D* histograms2[] = {h7, h8, h9, h10, h11, h12};
    bool logY2[] = {true, false, false, true, false, false};
    int nHists2 = sizeof(histograms2) / sizeof(histograms2[0]);

    TH1D* histograms3[] = {h13};
    bool logY3[] = {false};
    int nHists3 = sizeof(histograms3) / sizeof(histograms3[0]);

    // Draw histograms
    DrawHistograms(c1, histograms1, nHists1, logY1);
    DrawHistograms(c2, histograms2, nHists2, logY2);
    DrawHistograms(c3, histograms3, nHists3, logY3);

    // Image name
    time_t ttime = time(NULL);
    struct tm date = *localtime(&ttime);
    char time_name[38];
    sprintf(time_name, "./imgs/teste/teste-%d-%02d-%02d-%02d-%02d-%02d", // Check if its in the teste folder
            date.tm_year + 1900, 
            date.tm_mon + 1, 
            date.tm_mday, 
            date.tm_hour, 
            date.tm_min, 
            date.tm_sec);

    char c1_name[45], c2_name[45], c3_name[45];
    sprintf(c1_name, "%s-01.pdf", time_name);
    sprintf(c2_name, "%s-02.pdf", time_name);
    sprintf(c3_name, "%s-03.pdf", time_name);

    // Save canvas
    c1->Print(c1_name);
    c2->Print(c2_name);
    c3->Print(c3_name);

    // Keeping track of time
    std::cout << "Start: " << date.tm_mday << "/" << date.tm_mon + 1 << "/" << date.tm_year + 1900 <<  " " 
        << date.tm_hour << ":" << date.tm_min << ":" << date.tm_sec << std::endl;
    
    // Duration calculator (future implementation)
    // Clean up
    TH1D** histograms[] = {histograms1, histograms2, histograms3};
    int nHists[] = {nHists1, nHists2, nHists3};

    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < nHists[j]; i++) {
            delete histograms[j][i]; // Delete each histogram
        }
    }
    // Close to better memory usage
    delete c1;
    delete c2;
    delete c3;
    delete h1;
    delete h2;
    delete h3;
    delete h4;
    delete h5;
    delete h6;
    delete h7;
    delete h8;
    delete h9;
    delete h10;
    delete h11;
    delete h12;
    delete h13;
    fr->Close();
    delete fr;
}
