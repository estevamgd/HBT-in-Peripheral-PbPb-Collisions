#ifndef FILL_HIST_H
#define FILL_HIST_H

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "my_func.h"
#include "fill_hist.h"

// Prototype fill error, might not even be worth to make this one
void fill_hist(Long64_t nEntries, TTree *&t, Int_t &Ntrk, Int_t &NOSpair, Int_t &NSSpair,
                TH1D *entry_hists[]=nullptr, void* entry_vars[], 
                TH1D *ntrk_hists[]=nullptr, void* ntrk_vars[],
                TH1D *qos_hists[]=nullptr, void* qos_vars[],
                TH1D *qss_hists[]=nullptr, void* qss_vars[]) {
    
    int nEntry, nNtrk, nQinvsigos, nQinvsigss;                

    if (entry_hists) { int nEntry = sizeof(entry_hists) / sizeof(entry_hists[0]); }
    if (ntrk_hists) { int nNtrk = sizeof(ntrk_hists) / sizeof(ntrk_hists[0]); }
    if (qos_hists) { int nQinvsigos = sizeof(qos_hists) / sizeof(qos_hists[0]); }
    if (qss_hists) { int nQinvsigss = sizeof(qss_hists) / sizeof(qss_hists[0]); }
 
    // Filling histograms
    for (Long64_t i = 0; i < nEntries; i++) {
        t->GetEntry(i);

        if (nEntry) {
            for (int ii = 0; ii < nEntry; ii++) {
                entry_hists[ii]->Fill(entry_vars[ii]);
            }
        }

        for (int j = 0; j < Ntrk; j++) {
            if (nEntry) {
                for (int ii = 0; ii < nEntry; ii++) {
                    entry_hists[ii]->Fill(entry_vars[ii]);
                }
        }
        }
        for (int k = 0; k < NOSpair; k++) {
            if (nEntry) {
                for (int ii = 0; ii < nEntry; ii++) {
                    entry_hists[ii]->Fill(entry_vars[ii]);
                }
        }
        }
        for (int l = 0; l < NSSpair; l++) {
            if (nEntry) {
                for (int ii = 0; ii < nEntry; ii++) {
                    entry_hists[ii]->Fill(entry_vars[ii]);
                }
        }
        }

        // Display progress
        float progress = (float)(i + 1) / nEntries * 100;
        std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
    }
}
#endif