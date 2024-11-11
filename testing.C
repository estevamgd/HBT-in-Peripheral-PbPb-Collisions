#include "TFile.h"
#include "TTree.h"
#include <TROOT.h>
#include <TF1.h>
#include <TMath.h>
#include "TCanvas.h"
#include "TH1D.h"
#include <iostream>
#include <iomanip>
#include <TStyle.h>
#include "my_func.h"
#include "TLegend.h"
#include "normalizer.h"
#include "analyze_tools.h"
#include <TText.h>
#include <TBenchmark.h>

void test_NaN(Double_t x) {
    if (TMath::IsNaN(x) != 0) {
        std::cout << "Is NaN" << std::endl;
    } else {
        std::cout << "Isn't NaN" << std::endl;
    }
}

void test_NaN(Float_t x) {
    if (TMath::IsNaN(x) != 0) {
        std::cout << "Is NaN" << std::endl;
    } else {
        std::cout << "Isn't NaN" << std::endl;
    }
}

Double_t test_func(Double_t* x, Double_t* par) {
    Double_t v = (par[1] - par[2]) / (x[0]-0.6);
    std::cout << "Function value: " << x[0] << std::endl;

    if (TMath::IsNaN(v)) {
        std::cout << "REJECTED" << std::endl;
        TF1::RejectPoint();  // Rejects the current point in the fit
        return 0; // Return zero for the rejected point
    }
    
    return v;
}

void testing() {
    // Creating a histogram with 5 bins in the range 0-1
    TH1D *hist_test = new TH1D("h", "Test Histogram", 5, 0, 1);
    double_t j = 1;
    double_t k;
    double_t l = 1;

    for (double_t i = 1; i <= 5; i++) {
        k = j / l;
        hist_test->Fill(k);
        l++;
    }

    // Define a TF1 with test_func in range 0-1, with 3 parameters
    TF1 *fit_test = new TF1("fit_test", test_func, 0, 1, 3);
    fit_test->SetParameters(1.0, 3.0, 3.0);  // Initialize parameters to avoid division by zero

    // Fit histogram with the function, using the "R" option for range restriction
    hist_test->Fit(fit_test, "Q");

    // Set colors and draw
    fit_test->SetLineColor(kRed);
    hist_test->SetLineColor(kBlue);

    TCanvas *c1 = new TCanvas("c1", "Test Canvas", 800, 600);
    hist_test->Draw();
    fit_test->Draw("Same");

    c1->Update();
}
