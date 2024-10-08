#ifndef MY_FUNC_H
#define MY_FUNC_H

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
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
    
    return h;
}

#endif 
