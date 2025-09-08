# Z Boson Decay Simulation with ROOT

This project simulates **Z → e⁺e⁻** decays using the [ROOT](https://root.cern/) framework and analyzes the resulting kinematic distributions.  
It demonstrates how High Energy Physics (HEP) data analysis is performed using Monte Carlo techniques.

---

## 🔬 Features

- Generate Z bosons with:
  - Breit–Wigner mass distribution (PDG values: M<sub>Z</sub> = 91.2 GeV, Γ<sub>Z</sub> = 2.5 GeV)  
  - Exponentially falling transverse momentum (⟨p<sub>T</sub>⟩ ≈ 8 GeV)  
  - Gaussian rapidity distribution
- Isotropic decay into **e⁺ e⁻** in the Z rest frame
- Boost into the lab frame
- Save final-state four-momenta to a ROOT `TTree`
- Analyze and produce histograms:
  - **Z mass** (Breit–Wigner peak around 91 GeV)  
  - **Electron energy spectrum**  
  - **Z transverse momentum (p<sub>T</sub>) distribution**  

---

## 📂 Files

- `ZDecayGenerator.C` → Macro to simulate events and write `ZDecay.root`
- `AnalyzeZDecay.C` → Macro to analyze events and produce histograms
- `ZDecay.root` → Output ROOT file containing simulated events
- `Z_mass.png`, `electron_energy.png`, `Z_pT.png` → Analysis plots
- `README.md` → Project description and usage instructions

---

## ▶️ Usage

### 1. Run the generator
```bash
cd "/mnt/c/Users/Prakhar Mathur/Downloads/Z-Decay-root-project"
root -l -q "ZDecayGenerator.C(100000)"

