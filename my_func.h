#ifndef MY_FUNC_H
#define MY_FUNC_H

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <TStyle.h>
#include "my_func.h"

// Input file and tree names get tree and file
bool getFileTree(const char* file_name, const char* tree_name, TFile *&fr, TTree *&t) {
    // Open the input file
    fr = new TFile(file_name, "READ");
    if (!fr || fr->IsZombie()) {
        std::cerr << "Error: File could not be opened!" << std::endl;
        return false;
    }

    // Get the tree from the specified path
    fr->GetObject(tree_name, t);
    if (!t) {
        std::cerr << "Error: Tree '" << tree_name << "' not found!" << std::endl;
        fr->Close();
        delete fr;
        return false;
    }

    return true;
}

// Bin size calculator
int numBins(int numValues) {
    return (int)sqrt(numValues);
}

/* Histogram layout

Colors: 
kWhite  = 0, kBlack     = 1, kGray      = 920, kRed     = 632, kGreen   = 416, 
kBlue   = 600, kYellow  = 400, kMagenta = 616, kCyan    = 432, kOrange  = 800, 
kSpring = 820, kTeal    = 840, kAzure   = 860, kViolet  = 880, kPink    = 900

Fill Style:
kFDotted1  = 3001, kFDotted2    = 3002, kFDotted3  = 3003,
kFHatched1 = 3004, kHatched2    = 3005, kFHatched3 = 3006,
kFHatched4 = 3007, kFWicker     = 3008, kFScales   = 3009,
kFBricks   = 3010, kFSnowflakes = 3011, kFCircles  = 3012,
kFTiles    = 3013, kFMondrian   = 3014, kFDiamonds = 3015,
kFWaves1   = 3016, kFDashed1    = 3017, kFDashed2  = 3018,
kFAlhambra = 3019, kFWaves2     = 3020, kFStars1   = 3021,
kFStars2   = 3022, kFPyramids   = 3023, kFFrieze   = 3024,
kFMetopes  = 3025, kFEmpty      = 0   , kFSolid    = 1

Line Style:
kSolid = 1, kDashed = 2, kDotted = 3, kDashDotted = 4
*/
TH1D* cHist(const char* name, const char* xAxisTitle, const char* yAxisTitle, 
                      int nVal, double xMin, double xMax, 
                      int fill_color = 920, double fill_alpha = 1, int fill_style = 1001, 
                      int line_color = 1, double line_alpha = 1, int line_style = 1001, 
                      int line_width = 1, double label_size = 0.04) {
    // Create the histogram
    int nBins = numBins(nVal);
    TH1D* h = new TH1D(name, "", nBins, xMin, xMax);

    // Set visual properties
    h->SetFillColorAlpha(fill_color, fill_alpha); 
    h->SetFillStyle(fill_style); 
    h->SetLineColorAlpha(line_color, line_alpha);
    h->SetLineStyle(line_style);
    h->SetLineWidth(line_width); 

    // Set titles and axis labels
    h->SetTitle("");
    h->GetXaxis()->SetTitle(xAxisTitle);
    h->GetYaxis()->SetTitle(yAxisTitle);
    
    // Set label sizes
    h->GetXaxis()->SetLabelSize(label_size);
    h->GetYaxis()->SetLabelSize(label_size);
    
    return h;
}

// Saving the canvases
void save_canvas_images(TCanvas *canvases[], int numCanvases, const char *path, const char *prefix, const char *file_type) {
    if (numCanvases > 0) {
        // Get the current time
        time_t ttime = time(NULL);
        struct tm date = *localtime(&ttime);
        
        // Loop through all canvases and save them
        for (int i = 0; i < numCanvases; i++) {
            char canvas_name[200];  // Adjust size to hold path, prefix, and time info
            // Generate a unique file name for each canvas
            sprintf(canvas_name, "%s%s-%d-%02d-%02d-%02d-%02d-%02d-%02d.%s", 
                    path, prefix, 
                    date.tm_year + 1900, 
                    date.tm_mon + 1, 
                    date.tm_mday, 
                    date.tm_hour, 
                    date.tm_min, 
                    date.tm_sec, 
                    i + 1,    // Canvas index
                    file_type);  // File type
        
            // Save the canvas as an image with the specified file type
            canvases[i]->Print(canvas_name);
        }
    }
    
}

// Delete canvases and histograms
void close_program(TCanvas *canvases[], int numCanvases, TH1D *histograms[], int numHistograms, TFile *fr,
                    TH2D *histograms2d[] = nullptr) {
    if (numCanvases > 0) {
        for (int i = 0; i < numCanvases; i++) {
            delete canvases[i];
        }
    }
    if (histograms != nullptr) {
        if (numHistograms > 0) {
            for (int i = 0; i < numHistograms; i++) {
                delete histograms[i];
            }
        }
    }else {
        if (numHistograms > 0) {
            for (int i = 0; i < numHistograms; i++) {
                delete histograms2d[i];
            }
        }
    }
    

    fr->Close();
    delete fr;
}

void no_statbox(TH1D *histograms[], int numHistograms) {
    for (int i = 0; i < numHistograms; i++) {
        histograms[i]->SetStats(0);
    }
}

#endif 
