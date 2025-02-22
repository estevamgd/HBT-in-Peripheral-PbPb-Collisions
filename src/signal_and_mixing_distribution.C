#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiM4D.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TBenchmark.h"
#include "TStopwatch.h"
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
    Float_t HFsumET, trkCharge[maxSize], trkPt[maxSize], trkEta[maxSize], trkPhi[maxSize], pionMass = 0.13957039; // Pion mass [GeV] from PDG

    t->SetBranchAddress("HFsumET", &HFsumET);
    t->SetBranchAddress("Ntrk", &Ntrk);
    //t->SetBranchAddress("trkCharge", trkCharge);
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
    TStopwatch stopwatch, stopwatch1, stopwatch2;

    // Create 4-vector
    stopwatch.Start(kFALSE);
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);
 
        if (HFsumET < 100 && HFsumET > 375) continue;

        std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double>>> TrackFourVector;
        for (int j = 0; j < Ntrk; j++) {
            ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double>> vec(trkPt[j], trkEta[j], trkPhi[j], pionMass);
            TrackFourVector.push_back(vec);
        }

        // --- SIGNAL ---
        if (TrackFourVector.size() > 1) {
            // Double-loop method
            stopwatch1.Start(kFALSE);
            for (size_t p1 = 0; p1 < TrackFourVector.size(); p1++) {
                for (size_t p2 = p1 + 1; p2 < TrackFourVector.size(); p2++) {
                    if (trkCharge[p1]*trkCharge[p2] > 0) {
                        Double_t qinv_SS = GetQ(TrackFourVector[p1], TrackFourVector[p2]);
                    } else {
                        Double_t qinv_OS = GetQ(TrackFourVector[p1], TrackFourVector[p2]);
                    }
                }
            }
            stopwatch1.Stop();

            // Single-loop method
            stopwatch2.Start(kFALSE);
            for (int k = 0; k < TrackFourVector.size()*(TrackFourVector.size() - 1); k++) {
                int p1 = k / (TrackFourVector.size() - 1);
                int p2 = (k % (TrackFourVector.size() - 1)) + 1;

                if (p1 > p2) continue;
                if (trkCharge[p1]*trkCharge[p2] > 0) {
                    Double_t qinv_SS = GetQ(TrackFourVector[p1], TrackFourVector[p2]);
                } else {
                    Double_t qinv_OS = GetQ(TrackFourVector[p1], TrackFourVector[p2]);
                }
            }
            stopwatch2.Stop();
        }
        
    }
    stopwatch.Stop();
    
    // Print benchmark results
    std::cout << "Benchmark Results:" << std::endl;
    stopwatch.Print();
    stopwatch1.Print();
    stopwatch2.Print();
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