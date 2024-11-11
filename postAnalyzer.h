//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 11 23:04:13 2024 by ROOT version 6.28/06
// from TTree HBT/HBT
// found on file: HiForestAOD_UPC.root
//////////////////////////////////////////////////////////

#ifndef postAnalyzer_h
#define postAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class postAnalyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           evRunNumber;
   Int_t           evEventNumber;
   Int_t           Npv;
   Int_t           pvNDOF;
   Float_t         pvZ;
   Float_t         pvRho;
   Float_t         HFsumETPlus;
   Float_t         HFsumETMinus;
   Float_t         HFsumET;
   Float_t         zdcSum;
   Int_t           Ntrk;
   Float_t         trkPt[600];   //[Ntrk]
   Float_t         trkEta[600];   //[Ntrk]
   Float_t         trkPhi[600];   //[Ntrk]
   Float_t         trkPtRes[600];   //[Ntrk]
   Float_t         trkDzSig[600];   //[Ntrk]
   Float_t         trkDxySig[600];   //[Ntrk]
   Float_t         trkNpixLayers[600];   //[Ntrk]
   Int_t           NSSpair;
   Int_t           NOSpair;
   Float_t         qinvSigSS[16806];   //[NSSpair]
   Float_t         coulombWSS[16806];   //[NSSpair]
   Float_t         qinvSigOS[16898];   //[NOSpair]
   Float_t         coulombWOS[16898];   //[NOSpair]

   // List of branches
   TBranch        *b_evRunNumber;   //!
   TBranch        *b_evEventNumber;   //!
   TBranch        *b_Npv;   //!
   TBranch        *b_pvNDOF;   //!
   TBranch        *b_pvZ;   //!
   TBranch        *b_pvRho;   //!
   TBranch        *b_HFsumETPlus;   //!
   TBranch        *b_HFsumETMinus;   //!
   TBranch        *b_HFsumET;   //!
   TBranch        *b_zdcSum;   //!
   TBranch        *b_Ntrk;   //!
   TBranch        *b_trkPt;   //!
   TBranch        *b_trkEta;   //!
   TBranch        *b_trkPhi;   //!
   TBranch        *b_trkPtRes;   //!
   TBranch        *b_trkDzSig;   //!
   TBranch        *b_trkDxySig;   //!
   TBranch        *b_trkNpixLayers;   //!
   TBranch        *b_NSSpair;   //!
   TBranch        *b_NOSpair;   //!
   TBranch        *b_qinvSigSS;   //!
   TBranch        *b_coulombWSS;   //!
   TBranch        *b_qinvSigOS;   //!
   TBranch        *b_coulombWOS;   //!

   postAnalyzer(TTree *tree=0);
   virtual ~postAnalyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef postAnalyzer_cxx
postAnalyzer::postAnalyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("HiForestAOD_UPC.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("HiForestAOD_UPC.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("HiForestAOD_UPC.root:/demo");
      dir->GetObject("HBT",tree);

   }
   Init(tree);
}

postAnalyzer::~postAnalyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t postAnalyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t postAnalyzer::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void postAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evRunNumber", &evRunNumber, &b_evRunNumber);
   fChain->SetBranchAddress("evEventNumber", &evEventNumber, &b_evEventNumber);
   fChain->SetBranchAddress("Npv", &Npv, &b_Npv);
   fChain->SetBranchAddress("pvNDOF", &pvNDOF, &b_pvNDOF);
   fChain->SetBranchAddress("pvZ", &pvZ, &b_pvZ);
   fChain->SetBranchAddress("pvRho", &pvRho, &b_pvRho);
   fChain->SetBranchAddress("HFsumETPlus", &HFsumETPlus, &b_HFsumETPlus);
   fChain->SetBranchAddress("HFsumETMinus", &HFsumETMinus, &b_HFsumETMinus);
   fChain->SetBranchAddress("HFsumET", &HFsumET, &b_HFsumET);
   fChain->SetBranchAddress("zdcSum", &zdcSum, &b_zdcSum);
   fChain->SetBranchAddress("Ntrk", &Ntrk, &b_Ntrk);
   fChain->SetBranchAddress("trkPt", trkPt, &b_trkPt);
   fChain->SetBranchAddress("trkEta", trkEta, &b_trkEta);
   fChain->SetBranchAddress("trkPhi", trkPhi, &b_trkPhi);
   fChain->SetBranchAddress("trkPtRes", trkPtRes, &b_trkPtRes);
   fChain->SetBranchAddress("trkDzSig", trkDzSig, &b_trkDzSig);
   fChain->SetBranchAddress("trkDxySig", trkDxySig, &b_trkDxySig);
   fChain->SetBranchAddress("trkNpixLayers", trkNpixLayers, &b_trkNpixLayers);
   fChain->SetBranchAddress("NSSpair", &NSSpair, &b_NSSpair);
   fChain->SetBranchAddress("NOSpair", &NOSpair, &b_NOSpair);
   fChain->SetBranchAddress("qinvSigSS", qinvSigSS, &b_qinvSigSS);
   fChain->SetBranchAddress("coulombWSS", coulombWSS, &b_coulombWSS);
   fChain->SetBranchAddress("qinvSigOS", qinvSigOS, &b_qinvSigOS);
   fChain->SetBranchAddress("coulombWOS", coulombWOS, &b_coulombWOS);
   Notify();
}

Bool_t postAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void postAnalyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t postAnalyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef postAnalyzer_cxx
