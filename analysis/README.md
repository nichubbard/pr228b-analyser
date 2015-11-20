PR228B Analysis Scripts
========================

New Config System
-----------------
A new config system is being set up, see analysis.cfg

It provides a simple text-file for changing settings and files for the
analysis scripts, skipping having to modify (possibly numerous) various
C and H files. Currently only the following actually use this new file:

runs
----
Loads a TChain of runs into ROOT. The runs come from the config file
(see property `Runs`). If `UsePROOF` is yes, this also starts PROOF on the
chain, to speed up processing big chains. The following variables are
created

* `chain` - The `TChain` of all runs
* `DATA` - A `TTree*` for compatability with single run files - by
    always using `DATA` the same macros/lines work for the chain or
    for single runs.
* `proof` - A `TProof*` pointing to the PROOF instance created. Will be
    `nullptr` if `UsePROOF` was no.

Carbon
------
Analyse a carbon run/chain and try to find a pair branching ratio.

Usage: `root runs.C Carbon`

Li2CO3
------
Analyse a Li2CO3 run/chain and try to find pair efficiency
and other parameters.

Usage: `root runs.C Li2CO3`

Both Li2CO3 and Carbon (no extension) call `DATA->Process("XXX.C++")`
for convenience.

RetuneLithium
-------------
Recalibrates focal plane based on a Li2CO3 run/chain. Requires a reasonable
initial calibration to work!! Now ignores peaks close to the edge, but
otherwise not very flexible (yet). Depends on Spectra to run.

Usage: `root runs.C Spectra.C RetuneLithium.C`

Spectra
-------
Provides a bunch of simple ROOT macros for plotting common spectra.

`Ex`, `X1pos`, `SiliconTime` all plot their respective TTree variables with
sensible axes and gates (if applicable)

`ExSi` plots SiliconEnergy against Ex, again with sensibile axes.

`PID` plots pad1vstof for PID gating.

`CheckLut` plots res1:res0 for X1,U1,X2,U2 for verifying LUT offsets.
See the K600 manual for information.

`ExSi`, `SiliconTime`, `X1pos`, `Ex` all implicitly have a cut on PID, which
is automatically loaded by `runs.C`. Currently there is no way to have no
PID cut, but this a TODO.

`ExSi`, `SiliconTime`, `X1pos` take an optional argument of additional cuts
to include


Old Scripts
-----------
Below are scripts that do not use the new config system because they
have not been updated or because it's not applicable. Eventually once
all applicable scripts are updated this section will be changed to focus
on scripts that don't use the config system, for whatever reason.

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

Pedestals
---------
Calculate ADC pedestals from a run with no silicon events

TDCAlign
--------
Aligns silicon TDC channels so the maximum peak value is at the same value on each channel

