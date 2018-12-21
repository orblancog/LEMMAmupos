#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"


//Questo programma legge il .dat dei muoni
//e lo converte in un .root da far leggere a ms_mu

void input2root_mu(string nome_file="ciao"){

  string nome_tree;
  nome_tree = nome_file.substr(0, nome_file.size()-4); //tolgo il .dat
  nome_tree = nome_tree + ".root";
  
  TFile f(nome_tree.c_str(),"recreate");
  TTree mu("mu","info sui positroni");
  int partID,turn;
  double x, px, y, py, ct, dE_E, s, E, w;
  mu.Branch("partID",&partID); //ID particella
  mu.Branch("turn",&turn); // -1 = non ha girato
  mu.Branch("x",&x); // m
  mu.Branch("px",&px); // px/p0
  mu.Branch("y",&y);
  mu.Branch("py",&py);
  mu.Branch("ct",&ct); // -ct? m
  mu.Branch("dE_E",&dE_E); //
  mu.Branch("s",&s);
  mu.Branch("E",&E); // GeV
  mu.Branch("w",&w);

  //variabili di appoggio per la lettura del .dat
  ifstream in;
  string line,word;
  std::vector<string> numeri;
  istringstream iss("ciao");
  
  cout << "lettura di " << nome_file << " in corso." << endl;
  in.open(nome_file);
  cout<<"fff"<<endl;
  if (in.is_open()){

    while(getline(in,line)){
      cout<<line<<endl;
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
      w =      std::stod(numeri[10]);
      mu.Fill();

    }//while getline

  }//if in.is_open()
  in.close();
  mu.Write();


}//FINE


