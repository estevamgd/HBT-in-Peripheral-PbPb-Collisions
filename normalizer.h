#ifndef NORMALIZER_H
#define NORMALIZER_H

#include "TH1D.h"
#include "normalizer.h"

void normalizer(TH1D* histograms[], int numHistograms, Double_t scale = 1.0) {
    for (int i = 0; i < numHistograms; i++) {
        TH1D* hist = histograms[i];
        if (hist->Integral() != 0) {  // Avoid division by zero
            hist->Scale(scale / hist->Integral(), "width");
        }
    }
}

#endif