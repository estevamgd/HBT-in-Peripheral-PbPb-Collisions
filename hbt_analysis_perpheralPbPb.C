#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <TStyle.h>

// Histogram layout class (future implementation)
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
    fr->GetObject("demo/HBT;570", t);
    if (!t) {
        std::cerr << "Error: Tree 'demo/HBT;570' not found!" << std::endl;
        return;
    }

    // Number of pairs
    int maxpairs_small = 600;
    int maxpairs_big = 16898;

    // Variables
    Int_t Ntrk;
    Float_t HFsumET;
    Float_t coulombWOS[maxpairs_big], coulombWSS[maxpairs_big], qinvSigSS[maxpairs_big], qinvSigOS[maxpairs_big], 
            trkPt[maxpairs_small], trkEta[maxpairs_small], trkPhi[maxpairs_small], trkPtRes[maxpairs_small], 
            trkDxySig[maxpairs_small], trkNpixLayers[maxpairs_small], trkDzSig[maxpairs_small];

    // Setting branches
    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
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
    TH1D *h1 = new TH1D("h1", "", 1900, 0, 380);
    h1->SetFillColor(kGray); h1->SetLineColor(kBlack); h1->SetLineWidth(1); 
    h1->SetTitle("");
    h1->GetXaxis()->SetTitle("HFsumET[GeV]");
    h1->GetYaxis()->SetTitle("#Events/bin");
    h1->GetXaxis()->SetLabelSize(0.04);
    h1->GetYaxis()->SetLabelSize(0.04);

    TH1D *h2 = new TH1D("h2", "", 1000, 0.4, 1);
    h2->SetFillColor(kGray); h2->SetLineColor(kBlack); h2->SetLineWidth(1); 
    h2->SetTitle("");
    h2->GetXaxis()->SetTitle("coulombWOS");
    h2->GetYaxis()->SetTitle("#Pairs/bin");
    h2->GetXaxis()->SetLabelSize(0.04);
    h2->GetYaxis()->SetLabelSize(0.04);

    TH1D *h3 = new TH1D("h3", "", 1000, 1, 3.5);
    h3->SetFillColor(kGray); h3->SetLineColor(kBlack); h3->SetLineWidth(1); 
    h3->SetTitle("");
    h3->GetXaxis()->SetTitle("coulombWSS");
    h3->GetYaxis()->SetTitle("#Pairs/bin");
    h3->GetXaxis()->SetLabelSize(0.04);
    h3->GetYaxis()->SetLabelSize(0.04);

    TH1D *h4 = new TH1D("h4", "", 1000, 0, 500);
    h4->SetFillColor(kGray); h4->SetLineColor(kBlack); h4->SetLineWidth(1); 
    h4->SetTitle("");
    h4->GetXaxis()->SetTitle("Ntrk");
    h4->GetYaxis()->SetTitle("#Events/bin");
    h4->GetXaxis()->SetLabelSize(0.04);
    h4->GetYaxis()->SetLabelSize(0.04);
    
    TH1D *h5 = new TH1D("h5", "", 1000, 0, 1);
    h5->SetFillColor(kGray); h5->SetLineColor(kBlack); h5->SetLineWidth(1); 
    h5->SetTitle("");
    h5->GetXaxis()->SetTitle("qinvSigOS[GeV]");
    h5->GetYaxis()->SetTitle("#Pairs/bin");
    h5->GetXaxis()->SetLabelSize(0.04);
    h5->GetYaxis()->SetLabelSize(0.04);

    TH1D *h6 = new TH1D("h6", "", 1000, 0, 1);
    h6->SetFillColor(kGray); h6->SetLineColor(kBlack); h6->SetLineWidth(1); 
    h6->SetTitle("");
    h6->GetXaxis()->SetTitle("qinvSigSS[GeV]");
    h6->GetYaxis()->SetTitle("#Pairs/bin");
    h6->GetXaxis()->SetLabelSize(0.04);
    h6->GetYaxis()->SetLabelSize(0.04);
    
    TH1D *h7 = new TH1D("h7", "", 1000, 0, 20);
    h7->SetFillColor(kGray); h7->SetLineColor(kBlack); h7->SetLineWidth(1); 
    h7->SetTitle("");
    h7->GetXaxis()->SetTitle("pT[GeV]");
    h7->GetYaxis()->SetTitle("#Tracks/bin");
    h7->GetXaxis()->SetLabelSize(0.04);
    h7->GetYaxis()->SetLabelSize(0.04);

    TH1D *h8 = new TH1D("h8", "", 1000, -2.6, 2.6);
    h8->SetFillColor(kGray); h8->SetLineColor(kBlack); h8->SetLineWidth(1); 
    h8->SetTitle("");
    h8->GetXaxis()->SetTitle("trkEta");
    h8->GetYaxis()->SetTitle("#Tracks/bin");
    h8->GetXaxis()->SetLabelSize(0.04);
    h8->GetYaxis()->SetLabelSize(0.04);

    TH1D *h9 = new TH1D("h9", "", 1000, -3.4, 3.4);
    h9->SetFillColor(kGray); h9->SetLineColor(kBlack); h9->SetLineWidth(1); 
    h9->SetTitle("");
    h9->GetXaxis()->SetTitle("trkPhi");
    h9->GetYaxis()->SetTitle("#Tracks/bin");
    h9->GetXaxis()->SetLabelSize(0.04);
    h9->GetYaxis()->SetLabelSize(0.04);

    TH1D *h10 = new TH1D("h10", "", 1000, 0, 0.11);
    h10->SetFillColor(kGray); h10->SetLineColor(kBlack); h10->SetLineWidth(1); 
    h10->SetTitle("");
    h10->GetXaxis()->SetTitle("trkPtRes");
    h10->GetYaxis()->SetTitle("#Tracks/bin");
    h10->GetXaxis()->SetLabelSize(0.04);
    h10->GetYaxis()->SetLabelSize(0.04);
    
    TH1D *h11 = new TH1D("h11", "", 1000, -3.5, 3.5);
    h11->SetFillColor(kGray); h11->SetLineColor(kBlack); h11->SetLineWidth(1); 
    h11->SetTitle("");
    h11->GetXaxis()->SetTitle("trkDxySig");
    h11->GetYaxis()->SetTitle("#Tracks/bin");
    h11->GetXaxis()->SetLabelSize(0.04);
    h11->GetYaxis()->SetLabelSize(0.04);

    TH1D *h12 = new TH1D("h12", "", 1000, -3, 3);
    h12->SetFillColor(kGray); h12->SetLineColor(kBlack); h12->SetLineWidth(1); 
    h12->SetTitle("");
    h12->GetXaxis()->SetTitle("trkDzSig");
    h12->GetYaxis()->SetTitle("#Tracks/bin");
    h12->GetXaxis()->SetLabelSize(0.04);
    h12->GetYaxis()->SetLabelSize(0.04);
    
    TH1D *h13 = new TH1D("h13", "", 1000, 0, 5);
    h13->SetFillColor(kGray); h13->SetLineColor(kBlack); h13->SetLineWidth(1); 
    h13->SetTitle("");
    h13->GetXaxis()->SetTitle("trkNpixLayers");
    h13->GetYaxis()->SetTitle("#Tracks/bin");
    h13->GetXaxis()->SetLabelSize(0.04);
    h13->GetYaxis()->SetLabelSize(0.04);

    // Filling histograms
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        h1->Fill(HFsumET);
        h4->Fill(Ntrk);

        for (int k = 0; k < maxpairs_small; k++) {
            h7->Fill(trkPt[k]);
            h8->Fill(trkEta[k]);
            h9->Fill(trkPhi[k]);
            h10->Fill(trkPtRes[k]);
            h11->Fill(trkDxySig[k]);
            h12->Fill(trkDzSig[k]);
            h13->Fill(trkNpixLayers[k]);
        }

        for (int j = 0; j < maxpairs_big; j++) {
            h2->Fill(coulombWOS[j]);
            h3->Fill(coulombWSS[j]);
            h5->Fill(qinvSigOS[j]);
            h6->Fill(qinvSigSS[j]);
        }

        // Calculate percentage
        float progress = (float)(i + 1) / nentries * 100;

        // Display percentage, overwrite previous percentage using '\r'
        std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
    }

    // Move to the next line after finishing
    std::cout << std::endl;

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
    c3->cd(1); gPad->SetGrid(); gPad->SetLeftMargin(0.15); h13->Draw(); 

    c1->Update();
    c2->Update();
    c3->Update();

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
    sprintf(c1_name, "%s-01.png", time_name);
    sprintf(c2_name, "%s-02.png", time_name);
    sprintf(c3_name, "%s-03.png", time_name);

    // Save canvas
    c1->Print(c1_name);
    c2->Print(c2_name);
    c3->Print(c3_name);

    // Keeping track of time
    std::cout << "Start: " << date.tm_mday << "/" << date.tm_mon + 1 << "/" << date.tm_year + 1900 <<  " " 
        << date.tm_hour << ":" << date.tm_min << ":" << date.tm_sec << std::endl;
    
    // Duration calculator (future implementation)

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
