#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"


//Questo programma legge il .dat dei positoni
//e lo converte in un .root da far leggere a beam_int_2D_ntupla

void input2root(){

  string nome_file = "trackSUMMend_pos.dat";

  TFile f("positroni.root","recreate");
  TTree pos("pos","info sui positroni");
  int partID,turn;
  double x, px, y, py, ct, dE_E, s, E;
  pos.Branch("partID",&partID); //ID particella
  pos.Branch("turn",&turn); // -1 = non ha girato
  pos.Branch("x",&x); // m
  pos.Branch("px",&px); // px/p0
  pos.Branch("y",&y);
  pos.Branch("py",&py);
  pos.Branch("ct",&ct); // -ct? m
  pos.Branch("dE_E",&dE_E); //
  pos.Branch("s",&s);
  pos.Branch("E",&E); // GeV

  //variabili di appoggio per la lettura del .dat
  ifstream in;
  string line,word;
  std::vector<string> numeri;
  istringstream iss("ciao");
  
  cout << "lettura di " << nome_file << "in corso." << endl;
  in.open(nome_file);    
  if (in.is_open()){

    while(getline(in,line)){
      //cout<<line<<endl;
      numeri.clear();
      iss.clear();
      iss.str(line);
      while(iss>>word) numeri.push_back(word);
      partID = std::stoi(numeri[0]);
      turn =   std::stoi(numeri[1]);
      x =      std::stod(numeri[2]);
      px =     std::stod(numeri[3]);
      y =      std::stod(numeri[4]);
      py =     std::stod(numeri[5]);
      ct =     std::stod(numeri[6]);
      dE_E =   std::stod(numeri[7]);
      s =      std::stod(numeri[8]);
      E =      std::stod(numeri[9]);
      pos.Fill();

    }//while getline

  }//if in.is_open()
  in.close();
  pos.Write();


}//FINE


