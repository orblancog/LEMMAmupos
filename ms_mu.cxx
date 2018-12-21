#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"


//Questo programma legge una ntupla di positroni e
//da in uscita una ntupla dei muoni prodotti

void ms_mu(string nome_file="ciao"){

  TFile *Tinput = TFile::Open(nome_file.c_str());

  string nome_dat;
  nome_dat = nome_file.substr(0, nome_file.size()-5);
  nome_dat = nome_dat + "_ms.dat";

  ofstream out;
  out.open(nome_dat);

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
 
  //definizione variabili
  
  bool debug = true; //se true stampa grafici delle singole distribuzioni
 
  double m_e = 0.000510998; //GeV
  double m_mu = 0.1056583745; //GeV
  double alpha = 1./137.;
  double pi = acos(-1.);
  double n_av = 6.022e23;

  Double_t X0; //pdg[g cm^-2]
  Double_t rho; //pdg[g cm^-3]???
  double A; //pdg
  double Z;
  Double_t X0_tgt; //n lungh rad del bers.

  double berillio[] = {0,65.19,1.848,9.01218,4};//primo elem inutile
  double idrogeno[] = {0,63.04,0.071,1.008,1};
  
  //string proprieta [] = {"l_tgt","X0","rho","A","Z"}; 
  if(tgt=="be"){
    cout<<endl;
    cout<<"Bersaglio di Be"<<endl;
    cout<<endl;
    X0 = berillio[1]; //pdg[g cm^-2]
    rho = berillio[2]; //pdg[g cm^-3]???
    A = berillio[3]; //pdg
    Z = berillio[4];
    X0_tgt = X0/rho; //n lungh rad del bers.
  }
  if(tgt=="h2"){
    cout<<endl;
    cout<<"Bersaglio di H2"<<endl;
    cout<<endl;
    X0 = idrogeno[1]; //pdg[g cm^-2]
    rho = idrogeno[2]; //pdg[g cm^-3]???
    A = idrogeno[3]; //pdg
    Z = idrogeno[4];
    X0_tgt = X0/rho; //n lungh rad del bers.
  }

  double th_ms_max;
  double th_ms;
  double phi_ms;
  double deltax_ms;

   
   //definizione funzioni

   TF1 *lrad = new TF1("lrad","e^(-x/[0])",0,l_tgt);
   lrad->SetParameter(0,X0_tgt);

   //definizione istogrammi

   TH1D *hxx = new TH1D("hxx","x prima",4000,-0.002,0.002);
   TH1D *hpx = new TH1D("hpx","px prima",4000,-0.001,0.001);
   TH1D *hyy = new TH1D("hyy","y prima",4000,-0.002,0.002);
   TH1D *hpy = new TH1D("hpy","py prima",4000,-0.001,0.001);
   TH1D *hct = new TH1D("hct","ct prima",4000,-0.015,0.015);
   TH1D *hEE = new TH1D("hEE","Energia prima",4000,21,25);
   
   TH1D *hxxm = new TH1D("hxxm","x ms",4000,-0.002,0.002);
   TH1D *hpxm = new TH1D("hpxm","px ms",4000,-0.001,0.001);
   TH1D *hyym = new TH1D("hyym","y ms",4000,-0.002,0.002);
   TH1D *hpym = new TH1D("hpym","py ms",4000,-0.001,0.001);
   TH1D *hctm = new TH1D("hctm","ct ms",4000,-0.015,0.015);
   TH1D *hEEm = new TH1D("hEEm","Energia ms",4000,21,25);


   //definizione tree output

   string nome_tree = nome_file.substr(0,nome_file.size()-5);
   nome_tree = nome_tree + "_ms.root";
   
   TFile f(nome_tree.c_str(),"recreate");
   TTree mu_ms("mu_ms","info sui mu dopo ms");
   int partID_ms,turn_ms;
   double x_ms, px_ms, y_ms, py_ms, ct_ms, dE_E_ms, s_ms, E_ms, w_ms;
   mu_ms.Branch("partID",&partID_ms); //ID particella
   mu_ms.Branch("turn",&turn_ms); // -1 = non ha girato
   mu_ms.Branch("x",&x_ms); // m
   mu_ms.Branch("px",&px_ms); // px/p0
   mu_ms.Branch("y",&y_ms);
   mu_ms.Branch("py",&py_ms);
   mu_ms.Branch("ct",&ct_ms); // -ct? m
   mu_ms.Branch("dE_E",&dE_E_ms); //
   mu_ms.Branch("s",&s_ms);
   mu_ms.Branch("E",&E_ms); // GeV
   mu_ms.Branch("w",&w_ms);

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
   ////////// LOOP SULLE ENTRIES /////////////////////////////////////

   int nentries = input->GetEntries();
   //cout<<"s"<<l_tgt<<" t"<<turn<<" g"<<giro<<endl;
   for(int i=0;i<nentries;i++){
     input->GetEntry(i);
     //prima le cose che restano uguali
     partID_ms = partID;
     turn_ms = turn;
     ct_ms = ct;
     dE_E_ms = dE_E;
     s_ms = s;
     E_ms = E;
     w_ms = w;
     
     if(turn<turn_in){//e qui il ms
      
       th_ms_max = 0.0136*sqrt(l_tgt/X0_tgt)/E;
       th_ms = gRandom->Uniform(-th_ms_max,th_ms_max);
       phi_ms = gRandom->Uniform(0,pi);
       px_ms = px + th_ms*cos(phi_ms);
       py_ms = py + th_ms*sin(phi_ms);

       deltax_ms = l_tgt/sqrt(12)*th_ms_max;
       x_ms = x + gRandom->Gaus(0,deltax_ms);
       y_ms = y + gRandom->Gaus(0,deltax_ms);

       hxx->Fill(x); //riempio solo quelli che esistono
       hpx->Fill(px);
       hyy->Fill(y);
       hpy->Fill(py);
       hct->Fill(ct);
       hEE->Fill(E);
       hxxm->Fill(x_ms);
       hpxm->Fill(px_ms);
       hyym->Fill(y_ms);
       hpym->Fill(py_ms);
       hctm->Fill(ct_ms);
       hEEm->Fill(E_ms);

     }//fine if sulle part giuste
     else{//le part sbagliate restano uguali
       x_ms = x;
       px_ms = px;
       y_ms = y;
       py_ms = py;
     }//fine else

     mu_ms.Fill();
     out<<partID_ms<<" "<<turn_ms<<" "<<x_ms<<" "<<px_ms<<" "<<y_ms<<" "<<py_ms<<" "<<ct_ms<<" "<<dE_E_ms<<" "<<s_ms<<" "<<E_ms<<" "<<w_ms<<endl;
   }//fine loop sulle entries
   cout<<endl;
   mu_ms.Write();
   out.close();


   //grafici

   TCanvas *confr = new TCanvas;
   confr->Divide(3,2);

   confr->cd(1);
   hxx->SetLineColor(kBlue);
   hxx->Draw();
   hxxm->SetLineColor(kRed);
   hxxm->Draw("same");
   confr->cd(2);
   hyy->SetLineColor(kBlue);
   hyy->Draw();
   hyym->SetLineColor(kRed);
   hyym->Draw("same");
   confr->cd(3);
   hct->SetLineColor(kBlue);
   hct->Draw();
   hctm->SetLineColor(kRed);
   hctm->Draw("same");
   confr->cd(4);
   hpx->SetLineColor(kBlue);
   hpx->Draw();
   hpxm->SetLineColor(kRed);
   hpxm->Draw("same");
   confr->cd(5);
   hpy->SetLineColor(kBlue);
   hpy->Draw();
   hpym->SetLineColor(kRed);
   hpym->Draw("same");
   confr->cd(6);
   hEE->SetLineColor(kBlue);
   hEE->Draw();
   hEEm->SetLineColor(kRed);
   hEEm->Draw("same");


   
}//FINE


