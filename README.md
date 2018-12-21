# LEMMAmupos

This code tracks particles through out an accelerator line with a fixed target

Usage
=====

$ ./runParticles

all particles coordinates are stored in trackSUMMend

To draw the phase space, do

$ root -l
.x DrawPhaseSpaceZoom.C ("SUMMend",-1) #

it will produce the plot of the three phase spaces

To fit the phase space
$ root -l
.L MeanRMS6Dcut_cernroot.cxx
fitthetrack("trackSUMMend")

It will produce a file called pok.txt and fitresults.txt


Authors
=======
orblancog, collamati, ciarma

o
