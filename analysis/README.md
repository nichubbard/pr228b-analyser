PR228B Analysis Scripts
========================

Calibrate
---------
Calibrate silicon (for 228Th currently). Generates an offline
compatible file in pr228b/analysis, and a CSV.

chain
-----
Loads a TChain into ROOT, and sets DATA to point to the chain instead.
Also starts PROOF on the chain, to speed up processing big chains
(run chain.SetProof(0) to disable this)
You need to change the file to choose the runs to be part of the chain,
there's an array at the top, just add runs to that.

CheckCalibration
----------------
Check the calibration by plotting a 2D histogram of energy against
channel.

TODO: This script needs to be updated to parse the offline calibration
file rather than having a header file (that is annoying to change
when combining runs)

Correlation
-----------
Confirm coincidence correlation is still working by plotting X1vsSi
for each quarter of the run

Li2CO3
------
Analyse a Li2CO3 run/chain and try to find pair efficiency
and other parameters.

Pedestals
---------
Calculate ADC pedestals from a run with no silicon events

RetuneLithium
-------------
Recalibrates focal plane based on a Li2CO3 run/chain. Requires a reasonable
initial calibration to work!! Probably needs modifying to ignore fake
peaks found at the edges, etc. Not very flexible on its own (yet)

Spectra
-------
Provides a bunch of simple ROOT macros for plotting common spectra.

Ex, X1pos, SiliconTime all plot their respective TTree variables with
sensible axes and gates (if applicable)

ExSi() plots SiliconEnergy against Ex, again with sensibile axes.

PID() plots pad1vstof for PID gating.

CheckLut() plots res1:res0 for X1,U1,X2,U2 for verifying LUT offsets.
See the K600 manual for information.

ExSi, SiliconTime, X1pos, Ex all implicitly have a cut on CUTpid so make
sure a CUTpid is loaded (see the CUT\*.C macros)

ExSi, SiliconTime, X1pos take an optional argument of additional cuts
to include

TDCAlign
--------
Aligns silicon TDC channels so the maximum peak value is at the same value on each channel

TODO: Align to a fixed reference point (rather than to TDC0)

