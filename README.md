
# Calculating the 8<sup>th</sup> Dedekind number

This project contains the code for the bachelor project of Arjen Teijo Zijlstra
 which had the goal to calculate the 8<sup>th</sup> Dedekind number following
 Wiedemann (1991)'s strategy to compute d<sub>n+2</sup> from D<sub>n</sup>.

The bachelor project itself can be found [here](http://irs.ub.rug.nl/dbi/51deb7b40990d "Finding the 8<sup>th</sup> Dedekind Number").

## Compile

A simple `Makefile` is included. The program can be used after running `make`.

## Usage

    mpirun -np N ./project -d X

Where *X* in *[2..n)* is the Dedekind number to calculate. And *N* is the number
 of processes you would like to use. **Note**: The program will also work when
 running without using mpirun. In that case the program will just run in one
 process.

## Millipede

The program is written to run on the millipede cluster of the University of
 Groningen. More information on the millipede can be found [here](http://www.rug.nl/science-and-society/centre-for-information-technology/research/hpcv/publications/docs/millipede_man/millipede-cluster-user-guide_-introduction "Millipede cluster user guide").

An example Job script is added to the repository, to make it easier for others
 to use the code. A manual to writing these job scripts can be found [here](http://www.rug.nl/science-and-society/centre-for-information-technology/research/hpcv/publications/docs/millipede_man/millipede-cluster-user-guide_-submitting-jobs "Millipede cluster submitting jobs").
