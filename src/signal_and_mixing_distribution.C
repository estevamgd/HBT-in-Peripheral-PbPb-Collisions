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

void signal_and_mixing_distribution() {
    ROOT::EnableImplicitMT();
    auto poolSize = ROOT::GetThreadPoolSize();
    std::cout << "Pool size = " << poolSize << std::endl;

    // Load the ROOT file and tree
    TFile *fr = nullptr;
    TTree *t = nullptr;
    getFileTree("data/HiForestAOD_UPC.root", "demo/HBT", fr, t);

    // Variables from the tree
    Int_t maxSize = 1700, Ntrk; 
    Float_t HFsumET, trkPt[maxSize], trkEta[maxSize], trkPhi[maxSize], pionMass = 0.13957039; // Pion mass [GeV] from PDG
    int totalElements = 0;

    std::vector<std::vector<TLorentzVector>> AllTrackFourVector;

    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
    t->SetBranchAddress("trkPt", trkPt);
    t->SetBranchAddress("trkEta", trkEta);
    t->SetBranchAddress("trkPhi", trkPhi);

    // Getting how many entries
    Long64_t nentries = t->GetEntries(), trackvec_max_size = 0;

    // Histograms
    double ninterval = 1., nlength = 0.02, nscale = 1./1.;

    double nnscale = numBins(ninterval, nlength, nscale);

    TH1D* h_signal = new TH1D("h_signal", "", nnscale, 0., 1.6);
    TH1D* h_mixing = new TH1D("h_mixing", "", nnscale, 0., 1.6);
    
    h_signal->GetXaxis()->SetTitle("q_{inv}[GeV]");
    h_signal->GetYaxis()->SetTitle("#Pairs");

    h_mixing->GetXaxis()->SetTitle("q_{inv}[GeV]");
    h_mixing->GetYaxis()->SetTitle("#Pairs");

    Int_t numHistograms = 2;
    TH1D *histograms[] = {h_signal, h_mixing};

    // Benchmarking
    TBenchmark benchmark;


    // Create 4-vector
    benchmark.Start("4-Vector");

    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);
 
        if (HFsumET > 100 && HFsumET < 375) continue;

        std::vector<TLorentzVector> TrackFourVector;
        
        for (int j = 0; j < Ntrk; j++) {
            TLorentzVector vec;
            
            vec.SetPtEtaPhiM(trkPt[j], trkEta[j], trkPhi[j], pionMass);
            TrackFourVector.push_back(vec);
        }

        if (TrackFourVector.size() > 1) {
            int theseElements = TrackFourVector.size()*(TrackFourVector.size()-1); 
            totalElements += theseElements;

            AllTrackFourVector.push_back(TrackFourVector);
        }
        
    }
    benchmark.Stop("4-Vector");

    // Triple-loop method
    benchmark.Start("TripleLoop");
    for (int i = 0; i < AllTrackFourVector.size(); i++) {
        for (size_t p1 = 0; p1 < AllTrackFourVector[i].size(); p1++) {
            for (size_t p2 = p1 + 1; p2 < AllTrackFourVector[i].size(); p2++) {
                Double_t qinv = GetQ(AllTrackFourVector[i][p1], AllTrackFourVector[i][p2]);
            }
        }
    }
    benchmark.Stop("TripleLoop");
    
    // Double-loop method
    benchmark.Start("DoubleLoop");
    for (int i = 0; i < AllTrackFourVector.size(); i++) {
        
        for (int k = 0; k < AllTrackFourVector[i].size()*(AllTrackFourVector[i].size() - 1); k++) {
            int p1 = k / (AllTrackFourVector[i].size() - 1);
            int p2 = (k % (AllTrackFourVector[i].size() - 1)) + 1;

            if (p2 > p1) {
                Double_t qinv = GetQ(AllTrackFourVector[i][p1], AllTrackFourVector[i][p2]);
            }
        }
    }
    benchmark.Stop("DoubleLoop");

    // Single-loop method
    benchmark.Start("SingleLoop");
    for (int j = 0, a = 0, b = 0, i = 0; j < totalElements; j++) {
        int trigger = AllTrackFourVector[i].size() * (AllTrackFourVector[i].size() - 1);

        int k = j - a;
        
        int p1 = k / (AllTrackFourVector[i].size() - 1);
        int p2 = (k % (AllTrackFourVector[i].size() - 1)) + 1;
        
        if (p2 > p1) {
            Double_t qinv = GetQ(AllTrackFourVector[i][p1], AllTrackFourVector[i][p2]);
        }

        b++;
        if (b == trigger) {
            a = j + 1;
            b = 0;
            i++;
        }
    }
    benchmark.Stop("SingleLoop");
    
    
    // Print benchmark results
    std::cout << "Benchmark Results:" << std::endl;
    benchmark.Show("4-Vector");
    benchmark.Show("TripleLoop");
    benchmark.Show("DoubleLoop");
    benchmark.Show("SingleLoop"); 
    /*
    // Save histograms
    TCanvas *c1 = new TCanvas("c1", "Signal and Mixing Distributions", 1200, 800);
    
    TCanvas *canvases[] = {c1};
    int numCanvases = 1;
    
    gStyle->SetOptStat(0);
    h_signal->SetLineColor(kRed);
    h_mixing->SetLineColor(kBlue);
    h_signal->Draw("HIST");
    h_mixing->Draw("HIST SAME");

    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(h_signal, "Signal", "l");
    legend->AddEntry(h_mixing, "Mixing", "l");
    legend->Draw();

        // comment/uncomment to save/show in TBrowser the image
    // Saving image
    const char *path = "./imgs/teste/";
    const char *prefix = "sigmix_dist";
    const char *file_type = "png";
    save_canvas_images(canvases, numCanvases, path, prefix, file_type);
    
    // Closing program
    close_program(canvases, numCanvases, histograms, numHistograms, fr);  
    */    // comment/uncomment to save/show in TBrowser the image
}