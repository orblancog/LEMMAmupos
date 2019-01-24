#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"

#include "input2root.h"

int runParticles(string fname="track_pos.dat"){
  cout << "HI THERE ! This script runs particles through out"<< endl;
  cout << "  an accelerator with a fixed target"<<endl;
  cout << "  authors: oblancog, collamati, ciarma, boscolo" << endl;
  cout << "  2019" << endl;
  cout << endl;

  cout << "NOW..." <<endl;
  cout << "  Checking for madx..."<<endl;
  gSystem->Exec("hash madx 2>/dev/null ||  {echo >&2 'Oops... I require madx. Aborting'; exit 1;}");
  cout << "  ... madx found" << endl;
  cout << "  Checking for awk..."<<endl;
  gSystem->Exec("hash awk 2>/dev/null ||  {echo >&2 'Oops... I require madx. Aborting'; exit 1;}");
  cout << "  ... awk found" << endl;
   
  cout << "ALL SOFTWARE requirements are OK..."<<endl;
  cout << endl;
  cout << endl;

  // Number of targets
  int NTARGETS = 10;
  
  //Creating the multiple target tree
  TDirectory* DirTarg[NTARGETS];
  TDirectory* DirEmit;
  TFile* outfile;
  TString filename = "MT";
  TFile *f = new TFile(Form("%s.root",filename.Data()));
  TTree* MTtree = (TTree*) gDirectory->Get("LEMMA");
  //  Init(tree);
  
  outfile = new TFile(Form("%s_tree.root","MT"),"RECREATE");
  
  for (int ii=0;ii<NTARGETS;ii++){
    DirTarg[ii]=outfile->mkdir(Form("Targ%03d",ii));
  }
  DirEmit = outfile->mkdir("Emittances");
		       //  Init(tree);
  //reading positron input
  input2root(fname);
  
  return 0;
}
