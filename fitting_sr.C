#include "TFile.h"
#include "TTree.h"
#include <TF1.h>
#include <TMath.h>
#include "TCanvas.h"
#include "TH1D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <TStyle.h>
#include "my_func.h"
#include "TLegend.h"
#include "normalizer.h"
#include "analyze_tools.h"
#include <TText.h>

//to reject a range in the fit -- in principle did not reject any range
Double_t reject_range_min = 0.0;
Double_t reject_range_max = 0.00001;

//Exponential function + long range
Double_t func1_exp(Double_t* x, Double_t* par){
    Double_t v = 0;
    if(reject_range_min<x[0] && x[0]<reject_range_max){TF1::RejectPoint();}
    else{v= par[0]*(1 + par[1]*exp(-par[2]*x[0]/0.1973))*(1+par[3]*x[0]);}
    return v;
}

//Gaussian function + long range
Double_t func2_gauss(Double_t* x, Double_t* par){
    Double_t v = 0;
    if(reject_range_min<x[0] && x[0]<reject_range_max){TF1::RejectPoint();}
    else{v= par[0]*(1 + par[1]*exp(-pow(par[2]*x[0]/0.1973,2.)))*(1+par[3]*x[0]);}
    return v;
}

//Levy function 
Double_t func3_levy(Double_t* x, Double_t* par){
    Double_t v = 0;
    if(reject_range_min<x[0] && x[0]<reject_range_max){TF1::RejectPoint();}
    else{v= par[0]*(1 + par[1]*exp(-pow(par[2]*x[0]/0.1973,par[4])))*(1+par[3]*x[0]);}
    return v;
}

void fitting_sr() {
    // Setting up, copy from HERE to...
    // Getting TTree
    TFile *fr = nullptr;
    TTree *t = nullptr;

    getFileTree("HiForestAOD_UPC.root", "demo/HBT", fr, t);
    
    // Variables
    Int_t maxSize = 17100;
    Int_t Ntrk = maxSize, NSSpair = maxSize, NOSpair = maxSize;
    Float_t HFsumET;
    Float_t coulombWOS[NOSpair], coulombWSS[NSSpair], qinvSigSS[NSSpair], qinvSigOS[NOSpair], 
            trkPt[Ntrk], trkEta[Ntrk], trkPhi[Ntrk], trkPtRes[Ntrk], 
            trkDxySig[Ntrk], trkNpixLayers[Ntrk], trkDzSig[Ntrk];

    // Arrays of variables
    void* variables[] = {
        &HFsumET, &Ntrk, &NSSpair, &NOSpair, coulombWOS, coulombWSS, qinvSigOS, qinvSigSS, 
        trkPt, trkEta, trkPhi, trkPtRes, trkDxySig, trkDzSig, trkNpixLayers
    };

    // Branch names
    const char* branchNames[] = {
        "HFsumET", "Ntrk", "NSSpair", "NOSpair", "coulombWOS", "coulombWSS", 
        "qinvSigOS", "qinvSigSS", "trkPt", "trkEta", "trkPhi", "trkPtRes", 
        "trkDxySig", "trkDzSig", "trkNpixLayers"
    };

    // Setting addresses
    int numBranches = sizeof(variables) / sizeof(variables[0]);
    for (int i = 0; i < numBranches; i++) {
        t->SetBranchAddress(branchNames[i], variables[i]);
    }

    // ...HERE
    // Getting how many entries
    Long64_t nentries = t->GetEntries();

    // Setting canvases
    TCanvas *c1 = new TCanvas("c1", "", 1920, 1080);

    TCanvas *canvases[] = {c1};
    int numCanvases = 1;

    // Setting histograms
    TH1D *h1 = cHist("h1", "qinv[GeV]", "#Pairs/bin", nentries, -0.01, 1.01, 0, 0, 0, 920, 1, 1);
    TH1D *h2 = cHist("h2", "", "", nentries, -0.01, 1.01, 0, 0, 0, 920, 1, 1);

    TH1D *tonormhist[] = {h1, h2};
    int numTonorm = 2;

    // Filling histograms
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        if (HFsumET > 100 && HFsumET < 375) { // This selects a centrality
            for (int k = 0; k < NOSpair; k++) {
                h1->Fill(qinvSigOS[k]);

            }
            for (int l = 0; l < NSSpair; l++) {
                h2->Fill(qinvSigSS[l]);
            }    
        }

        // Display progress
        float progress = (float)(i + 1) / nentries * 100;
        std::cout << "\rFilling Progress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
    }

    std::cout << std::endl;

    // Normalize histograms
    Double_t scale = 1;
    normalizer(tonormhist, numTonorm, scale);
   
    // Dividing SS/OS
    TH1D *sr = (TH1D *)h2->Clone("sr");
    sr->Divide(h1);
    sr->SetLineColor(kGray+2);

    TF1 *fit_exp = new TF1("fit_exp", func1_exp, -0.01, 1.01, 4);
    fit_exp->SetParameters(1.0, 1.0, 4.0, 0.0); 
    fit_exp->SetParName(0,"Const");
    fit_exp->SetParName(1,"#lambda");
    fit_exp->SetParName(2,"R (fm)");
    fit_exp->SetParName(3,"#epsilon");
    fit_exp->SetLineColor(kBlue); 
    fit_exp->SetLineWidth(1); 
    
    TF1 *fit_gauss = new TF1("f_gauss", func2_gauss, 0.0, 1.0, 4);
	fit_gauss->SetParameters(1.0, 1.0, 4.0, 0.0);
	fit_gauss->SetParName(0,"Const");
	fit_gauss->SetParName(1,"#lambda"); 
	fit_gauss->SetParName(2,"R (fm)");
	fit_gauss->SetParName(3,"#epsilon");
    fit_gauss->SetLineColor(kRed); 
	fit_gauss->SetLineStyle(2);
	fit_gauss->SetLineWidth(1);

    TF1 *fit_levy = new TF1("f_levy", func3_levy, -0.01, 1.01, 5);
	fit_levy->SetParameters(1.0, 1.0, 4, 0, 1.0);
	fit_levy->SetParName(0,"Const");
	fit_levy->SetParName(1,"#lambda");
    fit_levy->SetParLimits(1,0.0,2.0); 
	fit_levy->SetParName(2,"R (fm)");
	fit_levy->SetParName(3,"#epsilon");
    fit_levy->SetParLimits(4,1.0,2.0);
	fit_levy->SetParName(4,"#aplha");
    fit_levy->SetLineColor(kGreen); 
	fit_levy->SetLineWidth(1);
	fit_levy->SetLineStyle(3);

    sr->Fit(fit_exp);
    sr->Fit(fit_gauss);
    sr->Fit(fit_levy);

    // Adding labels
    sr->SetTitle("CMS Open Data 2011 - PbPb 2.76 TeV");
    sr->GetXaxis()->SetTitle("q_{inv} [GeV]");
    sr->GetYaxis()->SetTitle("Single Ratio SS/OS");

    // Adding legend
    TLegend *h1_legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    h1_legend->AddEntry((TObject*)0, "50-70%");
    h1_legend->AddEntry(sr, "Coulomb Correction");
    h1_legend->AddEntry(fit_exp, "Exponential Fit");
    h1_legend->AddEntry(fit_gauss, "Gaussian Fit");
    h1_legend->AddEntry(fit_levy, "Levy fit");

    // Setting y range to 0.95<y<1.6
    sr->GetYaxis()->SetRangeUser(0.95, 1.6);
    
    // Removing statistics box
    sr->SetStats(0);

    // Drawing the single ratio
    c1->cd(); gPad->SetGrid(); gPad->SetLeftMargin(0.15); sr->Draw(); 
    fit_exp->Draw("SameL"); 
    fit_gauss->Draw("SameL"); 
    fit_levy->Draw("SameL"); 
    h1_legend->Draw();
   
    // Saving image
    const char *path = "./imgs/teste/";
    const char *prefix = "teste-fitting-sr";
    const char *file_type = "pdf";
    save_canvas_images(canvases, numCanvases, path, prefix, file_type);

    // Closing program
    TH1D *histograms[] = {h1, h2, sr};
    int numHistograms = 3;

    delete fit_exp;
    delete fit_gauss;
    delete fit_levy;
    close_program(canvases, numCanvases, histograms, numHistograms, fr);
}