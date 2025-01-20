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

    TFile *fr2 = nullptr;
    TTree *t2 = nullptr;
    getFileTree("data/output_particles.root", "particles", fr2, t2);

    // Variables from the tree
    Int_t maxSize = 1700, Ntrk;
    Float_t HFsumET, trkPt[maxSize], trkEta[maxSize], trkPhi[maxSize], trkMass = 0.13957; // Pion mass assumed

    std::vector<TLorentzVector> particles;

    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
    t->SetBranchAddress("trkPt", trkPt);
    t->SetBranchAddress("trkEta", trkEta);
    t->SetBranchAddress("trkPhi", trkPhi);

    t2->SetBranchAddress("particles", &particles);

    // Getting how many entries
    Long64_t nentries = t->GetEntries();

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
    
    // Double-loop method
    benchmark.Start("DoubleLoop");
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);
        std::vector<TLorentzVector> particles;

        // Create 4-momentum vectors for tracks
        for (int j = 0; j < Ntrk; j++) {
            TLorentzVector vec;
            vec.SetPtEtaPhiM(trkPt[j], trkEta[j], trkPhi[j], trkMass);
            particles.push_back(vec);
        }

        // Double-loop mixing
        for (size_t p1 = 0; p1 < particles.size(); p1++) {
            for (size_t p2 = p1 + 1; p2 < particles.size(); p2++) {
                Double_t qinv = GetQ(particles[p1], particles[p2]);
                //h_signal->Fill(qinv);
            }
        }
    }
    benchmark.Stop("DoubleLoop");
    
    // Single-loop method
    benchmark.Start("SingleLoop");
    for (Long64_t i = 0; i < Ntrk * nentries; i++) {
        int j = i / nentries;
        int k = i % nentries;

        std::cout << "max: " << Ntrk * nentries 
        << " i: " << i << " Ntrk: " << Ntrk << " nenties: " << nentries << std::endl;

        t->GetEntry(j);
        std::vector<TLorentzVector> particles;

        TLorentzVector vec;
        vec.SetPtEtaPhiM(trkPt[k], trkEta[k], trkPhi[k], trkMass);
        particles.push_back(vec);

        int p1 = k / particles.size();
        int p2 = k % particles.size();

        std::cout << "max: " << particles.size() * particles.size() 
        << "k: " << k << " p1: " << p1 << " p2: " << p2 << std::endl;

        TLorentzVector current = particles[p1];

        Double_t qinv = GetQ(current, particles[p2]);
        //h_mixing->Fill(qinv);
        
         
        // Create 4-momentum vectors for tracks
        for (int j = 0; j < Ntrk; j++) {
            TLorentzVector vec;
            vec.SetPtEtaPhiM(trkPt[j], trkEta[j], trkPhi[j], trkMass);
            particles.push_back(vec);
        }
        
        // Single-loop mixing
        for (int k = 0; k < particles.size() * particles.size(); k++) {
            int p1 = k / particles.size();
            int p2 = k % particles.size();

            std::cout << "max: " << particles.size() * particles.size() 
            << "k: " << k << " p1: " << p1 << " p2: " << p2 << std::endl;

            TLorentzVector current = particles[p1];

            Double_t qinv = GetQ(current, particles[p2]);
            //h_mixing->Fill(qinv);
        
        }
    }
    benchmark.Stop("SingleLoop");
    
    // Print benchmark results
    std::cout << "Benchmark Results:" << std::endl;
    benchmark.Show("DoubleLoop");
    benchmark.Show("SingleLoop"); 
    
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
        // comment/uncomment to save/show in TBrowser the image
}