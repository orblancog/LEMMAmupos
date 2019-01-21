#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"


//Questo programma legge il .dat dei muoni
//e lo converte in un .root da far leggere a ms_mu

void madx_output(string nome_file="ciao"){

  
 TFile *Tinput = TFile::Open(nome_file.c_str());

  string nome_dat;
  nome_dat = nome_file.substr(0, nome_file.size()-5);

  //analisi nome_file
  //deve essere tipo xxxxx_MM_SSSmm_tTTT_gGGG.dat
  //Materiale, Spessore, Targhetta, Giro

    std::vector<std::string>   result;

    std::stringstream  data(nome_file.substr(0, nome_file.size()-5));

    std::string line;
    while(std::getline(data,line,'_'))
    {
        result.push_back(line);
    }
    cout<<result[1]<<endl;
    cout<<result[2]<<endl;
    cout<<result[3]<<endl;
    cout<<result[4]<<endl;

    string tgt = result[1];//be, h2
    double l_tgt = std::stod(result[2])*0.001; //in m
    int turn_in = std::stoi(result[3].substr(1,result[3].size()));//tolgo t
    int giro = std::stoi(result[4].substr(1,result[4].size()));//tolgo g

    nome_dat = nome_file.substr(0, nome_file.size()-13);

    ostringstream outbbb;
    outbbb << std::internal << std::setfill('0') << std::setw(3) << (turn_in+1);
    
    nome_dat = nome_dat + outbbb.str() + "_g000.dat";

    ofstream out;
    out.open(nome_dat);


     ////////// LETTURA TTREE //////////////////////////////////////////
    
   cout<<"Lettura TTree"<<endl;

   TTree *input = (TTree*)Tinput->Get("mu");
   int partID, turn;
   double x, px, y, py, ct, dE_E,s, E,w;
   input->SetBranchAddress("partID",&partID);
   input->SetBranchAddress("turn",&turn); //il numero di TARGHETTA
   input->SetBranchAddress("x",&x);
   input->SetBranchAddress("px",&px);
   input->SetBranchAddress("y",&y);
   input->SetBranchAddress("py",&py);
   input->SetBranchAddress("ct",&ct);
   input->SetBranchAddress("dE_E",&dE_E);//diff da nominale GeV
   input->SetBranchAddress("s",&s);
   input->SetBranchAddress("E",&E);//nominale GeV
   input->SetBranchAddress("w",&w);
  
   ifstream in;
  in.open("trackEND");
  string word;
  std::vector<string> numeri;
  istringstream iss("ciao");  
  if (in.is_open()){

    int nentries = input->GetEntries();
    for(int i=0;i<nentries;i++){
      input->GetEntry(i);

      if(turn<=turn_in){

	getline(in,line);
	//cout<<line<<endl;
      numeri.clear();
      iss.clear();
      iss.str(line);
      while(iss>>word) numeri.push_back(word);
      partID = std::stoi(numeri[0]);
      turn =   std::stoi(numeri[1])-1;
      x =      std::stod(numeri[2]);
      px =     std::stod(numeri[3]);
      y =      std::stod(numeri[4]);
      py =     std::stod(numeri[5]);
      ct =     std::stod(numeri[6]);
      dE_E =   std::stod(numeri[7]);
      s =      std::stod(numeri[8]);
      E =      std::stod(numeri[9]);


      }//fine if

      out<<partID<<" "<<turn<<" "<<x<<" "<<px<<" "<<y<<" "<<py<<" "<<ct<<" "<<dE_E<<" "<<s<<" "<<E<<" "<<w<<endl;

	}//fine loop entries

  }//if in.is_open()
  in.close();


  out.close();



}//FINE


