PR228B Analysis Scripts
========================

Calibrate
---------
Calibrate silicon (for 228Th currently). Generates an offline
compatible file in pr228b/analysis, and a CSV.

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
TODO: Use Ex rather than X1pos so runs with different calibrations
can work together (with TDC change AND a PID change)

Pedestals
---------
Calculate ADC pedestals from a run with no silicon events

RetuneLithium
-------------
Recalibrates focal plane based on a Li2CO3 run/chain. Requires a reasonable
initial calibration to work!!

TDCAlign
--------
Aligns silicon TDC channels so the maximum peak value is at the same value on each channel
TODO: Align to a fixed reference point (rather than to TDC0)

