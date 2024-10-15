#ifndef ANALYZE_TOOLS_H
#define ANALYZE_TOOLS_H

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "my_func.h"
#include "analyze_tools.h"

void scanBins(TH1D *h, int maxBins = 10000) {
    for (int nBin = 0; nBin < maxBins; nBin++) {
        Double_t bin2_content = h->GetBinContent(nBin);
        Double_t bin2_error = h->GetBinError(nBin);
        std::cout << "\n#bin: " << nBin << std::endl;
        std::cout << "content: " << bin2_content << std::endl;
        std::cout << "error: " << bin2_error << std::endl;
    }
}

void scanDifZeroBins(TH1D *h, int maxBins = 10000) {
    for (int nBin = 0; nBin < maxBins; nBin++) {
        Double_t bin_content = h->GetBinContent(nBin);
        Double_t bin_error = h->GetBinError(nBin);
        if (bin_content != 0) {
            std::cout << "\n#bin: " << nBin << std::endl;
            std::cout << "content: " << bin_content << std::endl;
            std::cout << "Error: " << bin_error << std::endl;
        }
    }
}

int difZeroBin(TH1D *h, int maxBins = 10000) {
    for (int nBin = 0; nBin < maxBins; nBin++) {
        Double_t bin_content = h->GetBinContent(nBin);
        if (bin_content != 0) {
            std::cout << "\n#bin: " << nBin << std::endl;
            std::cout << "content: " << bin_content << std::endl;
            return nBin;
        }
    }
}

void print_types(){
    std::cout <<"Opening the input file." << std::endl;
    TFile *file = new TFile("HiForestAOD_UPC.root","r");

    std::cout<< "Event.root File opended" << std::endl;
    TTree *ttr = (TTree*)file->Get("demo/HBT");
    ttr->MakeClass("postAnalyzer");
}
    
#endif