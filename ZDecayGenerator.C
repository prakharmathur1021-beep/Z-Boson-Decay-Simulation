#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <cmath>
#include <iostream>

void ZDecayGenerator(int Nevents = 100000) {
    // RNG (seed 0 => automatic seed)
    TRandom3 rng(0);

    // Z parameters (PDG)
    const double MZ0 = 91.1876; // GeV
    const double GZ  = 2.4952;  // GeV

    // electron mass (GeV)
    const double me = 0.000511;

    // Output file & tree
    TFile *fout = TFile::Open("ZDecay.root","RECREATE");
    TTree *t = new TTree("t","Z -> e+ e- decays");

    // branches
    double mz;
    double px_e, py_e, pz_e, E_e;
    double px_p, py_p, pz_p, E_p;
    double px_Z, py_Z, pz_Z, E_Z;

    t->Branch("mz", &mz, "mz/D");
    t->Branch("px_e", &px_e, "px_e/D");
    t->Branch("py_e", &py_e, "py_e/D");
    t->Branch("pz_e", &pz_e, "pz_e/D");
    t->Branch("E_e",  &E_e,  "E_e/D");
    t->Branch("px_p", &px_p, "px_p/D");
    t->Branch("py_p", &py_p, "py_p/D");
    t->Branch("pz_p", &pz_p, "pz_p/D");
    t->Branch("E_p",  &E_p,  "E_p/D");
    t->Branch("px_Z", &px_Z, "px_Z/D");
    t->Branch("py_Z", &py_Z, "py_Z/D");
    t->Branch("pz_Z", &pz_Z, "pz_Z/D");
    t->Branch("E_Z",  &E_Z,  "E_Z/D");

    for (int i=0; i<Nevents; ++i) {
        // sample Z mass from a Breit-Wigner (relativistic width is approximated)
        mz = rng.BreitWigner(MZ0, GZ);

        // sample Z pT from an exponential distribution (typical falling shape).
        // mean ~ 8-12 GeV is realistic-ish for illustrative purposes.
        double pT = rng.Exp(8.0);
        if (pT > 80.0) pT = 80.0; // safety cap

        // azimuthal angle
        double phiZ = rng.Uniform(0, 2*M_PI);

        // sample rapidity (z boost). Gaussian around 0 with sigma ~1.0 is fine for toy.
        double yZ = rng.Gaus(0.0, 1.0);

        // transverse mass and total energy
        double mT = std::sqrt(mz*mz + pT*pT);
        double pz_val = mT * std::sinh(yZ);
        double E_val  = mT * std::cosh(yZ);

        // Construct Z four-vector in lab
        TLorentzVector Z;
        Z.SetPxPyPzE(pT*std::cos(phiZ), pT*std::sin(phiZ), pz_val, E_val);

        // Two-body decay in Z rest frame (isotropic)
        double Ee_rest = mz/2.0; // in rest frame
        double p_rest_mag = std::sqrt(std::max(0.0, Ee_rest*Ee_rest - me*me));

        double cos_th = rng.Uniform(-1.0, 1.0);
        double sin_th = std::sqrt(std::max(0.0, 1.0 - cos_th*cos_th));
        double phi_e = rng.Uniform(0, 2*M_PI);

        // electron 4-vector in Z rest frame
        double px_rf = p_rest_mag * sin_th * std::cos(phi_e);
        double py_rf = p_rest_mag * sin_th * std::sin(phi_e);
        double pz_rf = p_rest_mag * cos_th;

        TLorentzVector e_minus_rf(px_rf, py_rf, pz_rf, Ee_rest);
        TLorentzVector e_plus_rf(-px_rf, -py_rf, -pz_rf, Ee_rest);

        // boost to lab frame using Z boost
        TVector3 beta = Z.BoostVector();
        e_minus_rf.Boost(beta);
        e_plus_rf.Boost(beta);

        // fill branches (electron = e_minus_rf, positron = e_plus_rf)
        px_e = e_minus_rf.Px(); py_e = e_minus_rf.Py(); pz_e = e_minus_rf.Pz(); E_e = e_minus_rf.E();
        px_p = e_plus_rf.Px();  py_p = e_plus_rf.Py();  pz_p = e_plus_rf.Pz();  E_p = e_plus_rf.E();
        px_Z = Z.Px(); py_Z = Z.Py(); pz_Z = Z.Pz(); E_Z = Z.E();

        t->Fill();
    }

    fout->Write();
    fout->Close();
    std::cout << "✅ Simulation finished: " << Nevents << " events written to ZDecay.root\n";
}

