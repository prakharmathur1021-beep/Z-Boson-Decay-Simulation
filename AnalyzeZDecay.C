#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TString.h>
#include <iostream>
#include <cmath>

void AnalyzeZDecay(const char *outdir = "/mnt/c/Users/Prakhar Mathur/Downloads/Z-Decay-root-project/") {
    // open file
    TFile *f = TFile::Open("ZDecay.root");
    if (!f || f->IsZombie()) {
        std::cerr << "ERROR: cannot open ZDecay.root\n";
        return;
    }

    // get tree (the generator writes tree named "t")
    TTree *t = (TTree*)f->Get("t");
    if (!t) {
        std::cerr << "ERROR: Tree 't' not found in file\n";
        f->ls();
        return;
    }

    // set up local variables and set branch addresses
    double mz;
    double px_e, py_e, pz_e, E_e;
    double px_p, py_p, pz_p, E_p;

    t->SetBranchAddress("mz", &mz);
    t->SetBranchAddress("px_e", &px_e);
    t->SetBranchAddress("py_e", &py_e);
    t->SetBranchAddress("pz_e", &pz_e);
    t->SetBranchAddress("E_e",  &E_e);
    t->SetBranchAddress("px_p", &px_p);
    t->SetBranchAddress("py_p", &py_p);
    t->SetBranchAddress("pz_p", &pz_p);
    t->SetBranchAddress("E_p",  &E_p);

    // Histograms
    TH1D *h_mass = new TH1D("h_mass","Z Mass; m_{Z} (GeV); Events",100,80,100);
    TH1D *h_Ee   = new TH1D("h_Ee","Electron Energy; E_{e} (GeV); Events",100,0,100);
    TH1D *h_pT   = new TH1D("h_pT","Z Transverse Momentum; p_{T} (GeV); Events",100,0,50);

    // Loop over entries and fill
    Long64_t nentries = t->GetEntries();
    for (Long64_t i=0; i<nentries; ++i) {
        t->GetEntry(i);

        // fill mass & electron energy
        h_mass->Fill(mz);
        h_Ee  ->Fill(E_e);

        // compute Z pT from the two leptons
        double pxZ = px_e + px_p;
        double pyZ = py_e + py_p;
        double pTz = std::sqrt(pxZ*pxZ + pyZ*pyZ);
        h_pT->Fill(pTz);
    }

    // style
    gStyle->SetOptStat(1111);

    // create canvases and draw
    TCanvas *c1 = new TCanvas("c1","Z mass",1200,800);
    h_mass->SetLineColor(kBlue);
    h_mass->Draw("HIST");
    c1->Update();

    TCanvas *c2 = new TCanvas("c2","Electron energy",1200,800);
    h_Ee->SetLineColor(kBlue);
    h_Ee->Draw("HIST");
    c2->Update();

    TCanvas *c3 = new TCanvas("c3","Z pT",1200,800);
    h_pT->SetLineColor(kBlue);
    h_pT->Draw("HIST");
    // By default linear Y. If you want log-y uncomment next line:
    // c3->SetLogy(1);
    c3->Update();

    // Ensure outdir ends with '/'
    TString outdirS(outdir);
    if (!outdirS.EndsWith("/")) outdirS += "/";

    // create outdir if it doesn't exist
    if (gSystem->AccessPathName(outdirS, kWritePermission)) {
        // try to create directory (best-effort)
        gSystem->mkdir(outdirS, true);
    }

    // Save PNGs
    c1->SaveAs(outdirS + "Z_mass.png");
    c2->SaveAs(outdirS + "electron_energy.png");
    c3->SaveAs(outdirS + "Z_pT.png");

    std::cout << "✅ Analysis complete: PNGs written to " << outdirS.Data() << "\n";

    // clean
    delete c1; delete c2; delete c3;
    delete h_mass; delete h_Ee; delete h_pT;
    f->Close();
}
