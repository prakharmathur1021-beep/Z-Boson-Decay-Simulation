# Z → e⁺e⁻ Toy Decay Simulation (ROOT)

This is a small toy project I wrote to get comfortable with **ROOT**, Monte Carlo event generation, and basic collider kinematics.

---

## What the project does
- Generates a simple sample of **Z bosons decaying to e⁺e⁻ pairs** using a ROOT macro (`ZDecayGenerator.C`).
- Stores the events in a `TTree` inside `ZDecay.root`.
- An analysis macro (`AnalyzeZDecay.C`) reads the events and makes histograms of:
  - invariant mass of the electron–positron pair,
  - Z boson transverse momentum (toy distribution),
  - electron energy distribution.

---

## How I built it (step by step)
1. **Resonance mass sampling**
   - Sampled the Z mass from a **Breit–Wigner distribution** centered at 91.2 GeV with width ~2.5 GeV.
   - Initially got weird negative masses because I mixed up `TRandom3::BreitWigner` parameters — fixed after checking ROOT docs.

2. **Toy kinematics for the parent Z**
   - I didn’t use a real event generator (like PYTHIA), so I made a simple model:
     - exponential pₜ spectrum (mean ≈ 8 GeV),
     - Gaussian rapidity distribution (σ = 1),
     - flat φ distribution.
   - This is obviously not realistic, but it was good practice in generating four-vectors.

3. **Two-body decay in the rest frame**
   - Generated isotropic e⁺ and e⁻ directions in the Z rest frame.
   - Boosted them to the lab frame using ROOT’s `TLorentzVector::Boost`.

4. **Tree storage and analysis**
   - Wrote Z and lepton four-vectors into a `TTree`.
   - Later, analyzed the events to check invariant mass reconstruction.

---

## What I checked
- Verified that the reconstructed invariant mass from the electron + positron matched the generated `mz` event-by-event.
- Checked four-momentum conservation: `Z = e⁺ + e⁻` in all events.
- Histograms look reasonable: invariant mass peaks near 91 GeV, pₜ distribution falls steeply.

---

## What went wrong 
- At first, the histogram of invariant mass was shifted because I had forgotten to include electron mass in the kinematics. Adding it fixed the bug (though the effect is tiny).
- Had to cap pₜ values at ~80 GeV because the exponential sampling sometimes gave unrealistically large values.

---

## Limitations
- This is a **toy generator**, not a physics-accurate simulation.
- No detector effects (no smearing, no acceptance cuts).
- Z decay is treated isotropically, but real Z→ℓ⁺ℓ⁻ angular distributions depend on polarization and couplings.
- The pₜ and rapidity spectra are not physical.

---

## What I’d like to do next
- Add Gaussian smearing to simulate detector resolution and see how it broadens the invariant mass peak.
- Try generating events with **PYTHIA** or **MadGraph** instead of toy distributions, so spectra are closer to reality.
- Compare my toy histograms with published CMS Z boson distributions.

---

## What I learned
- How to use ROOT’s random sampling, `TLorentzVector`, and `TTree` storage.
- Basics of four-vector boosts and invariant mass reconstruction.
- Importance of validating every step (I caught bugs by checking 4-momentum conservation).
- That even simple toy simulations give intuition for more advanced analyses in papers (like Z–hadron correlations).



## ▶️ Usage

### 1. Run the generator
```bash
cd "/mnt/c/Users/Prakhar Mathur/Downloads/Z-Decay-root-project"
root -l -q "ZDecayGenerator.C(100000)"

