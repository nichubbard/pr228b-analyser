K600 Analyser for PR228b
==========================

A heavily modified version of the k600 analyser, sometimes hackish, sometimes not.

Building
--------
Now uses CMake, it is recommended to build in a subdir (e.g. build/) to avoid
polluting the folders and to make clean up easier.

Run `ccmake` /path/to/analyzer to configure in the terminal, such as to
enable ancillary detectors. You'll need to enable the appropriate \*Data
class to see specific detectors.

Run `make` to build, you can then move analyzer to the main directory
(needed to access config files) and run it

Usage
-----
Basic usage: `./analyzer -i /path/to/midas.mid.gz`

Options are mostly in the `configPR228b.cfg` file now, however controlling
of LUT offsets, PID gates for Histograms is still in the ODB. To change
run `odbedit`. You can load pr228b.odb for some sensible defaults.

Analysis
--------
Helpful scripts are in the `analysis` folder. See the readme there for
descriptions.

Issues
------
Note that this code is not (always) as flexibile as it pretends to be.
In particular although you can toggle various detectors, only MMM, W1
and W1 with X1 are likely to work properly. Other combinations will
either lead to crashes or not sorting all data. This will (hopefully)
be fixed at some later date.

