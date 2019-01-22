#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#import "xsmumu3.cxx"


//Questo programma legge una ntupla di positroni e
//da in uscita una ntupla dei muoni prodotti

void beam_int_6D_ntupla(){

  //ntupla positroni

  TFile *Tinput = TFile::Open("positroni.root");

  //output.dat

  ofstream out;
  string nome_out = "muoniprova";//al quale verranno aggiunte le info dopo
 
 
  //definizione variabili
  
  bool debug = false; //se true stampa grafici delle singole distribuzioni
  string tgt = "be"; //be, h2, li
  
  double m_e = 0.000510998; //GeV
  double m_mu = 0.1056583745; //GeV
  double alpha = 1./137.;
  double pi = acos(-1.);
  double n_av = 6.022e23;

  Double_t l_tgt; //target len in m
  Double_t X0; //pdg[g cm^-2]
  Double_t rho; //pdg[g cm^-3]???
  double A; //pdg
  double Z;
  Double_t X0_tgt; //n lungh rad del bers.

  double berillio[] = {0.003,65.19,1.848,9.01218,4};
  double idrogeno[] = {0,63.04,0.071,1.008,1};//l_tgt in base al berillio
  double litio[] = {0,82.78,0.534,6.94,3};//l_tgt in base al berillio
  
  //string proprieta [] = {"l_tgt","X0","rho","A","Z"}; 
  if(tgt=="be"){
    //    cout<<endl;
    cout<<"  Bersaglio di Be"<<endl;
    l_tgt = berillio[0]; //target len in m
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
    l_tgt=X0_tgt*berillio[0]/(berillio[1]/berillio[2]);
    //normalizzato al n lungh rad berillio
  }
   if(tgt=="li"){
    cout<<endl;
    cout<<"Bersaglio di Li"<<endl;
    cout<<endl;
    X0 = litio[1]; //pdg[g cm^-2]
    rho = litio[2]; //pdg[g cm^-3]???
    A = litio[3]; //pdg
    Z = litio[4];
    X0_tgt = X0/rho; //n lungh rad del bers.
    l_tgt=X0_tgt*berillio[0]/(berillio[1]/berillio[2]);
    //normalizzato al n lungh rad berillio
  }

   cout << "    X0   = "<< X0<< " [g cm^-2]"<<endl;
   cout << "    rho  = "<<rho<< " [g cm^-3]"<<endl;
   cout << "    A    = "<<A<< " [g mole-1]"<<endl;
   cout << "    Z    = "<<Z<<endl;
   cout << "    Ltar = "<<l_tgt*1e3<<" [mm]"<<endl;
   
   
   nome_out = nome_out + "_"+tgt+"_"+l_tgt*1000+"mm_t001_g000.dat";
   out.open(nome_out.c_str());

   double z; //zvertex
   double S; // = 2*m_e*E
   double xs;
   double theta_max;
   double x_0;
   double th_deg;
   double th;
   double phi;
   double x_mu1;
   double deltaEcorr;
   double segno;
   
   double n_mu;
   double n_bunch = 3e11;
   double n_muXS = n_bunch*1e-30*rho*n_av/A*Z*l_tgt*100;
   //   cout << n_muXS<<endl;
   //   cout << 3*1.848/9.01*6.022*4*0.05*1e4<<endl;
   double prob; //prob di avere evento
   double hit; //var per hit or miss
   double w;  //number of muons per particle in positron track
   double wt; //total number of muons
   
   //definizione funzioni

   TF1 *lrad = new TF1("lrad","e^(-x/[0])",0,l_tgt);
   lrad->SetParameter(0,X0_tgt);
   TF1 *xsmumu = new TF1("xsmumu",xsmumu3,40,80);  //import

   //definizione istogrammi

    TH1D *hxx = new TH1D("hxx","x e+",4000,-0.002,0.002);
    TH1D *hpx = new TH1D("hpx","px e+",4000,-0.001,0.001);
    TH1D *hyy = new TH1D("hyy","y e+",4000,-0.002,0.002);
    TH1D *hpy = new TH1D("hpy","py e+",4000,-0.001,0.001);
    TH1D *hct = new TH1D("hct","ct e+",4000,-0.015,0.015);
    TH1D *hdE = new TH1D("hdE","dE/E e+",4000,-0.5,0.05);
    TH1D *hEE = new TH1D("hEE","Energia e+",4000,43,47);
    TH1D *hAL = new TH1D("hAL","Alive e+",3000,0,2999);

    TH1D *hxxm = new TH1D("hxxm","x mu",4000,-0.002,0.002);
    TH1D *hpxm = new TH1D("hpxm","px mu",4000,-0.001,0.001);
    TH1D *hyym = new TH1D("hyym","y mu",4000,-0.002,0.002);
    TH1D *hpym = new TH1D("hpym","py mu",4000,-0.001,0.001);
    TH1D *hctm = new TH1D("hctm","ct mu",4000,-0.015,0.015);
    TH1D *hdEm = new TH1D("hdEm","dE/E mu",4000,-0.025,0.25);
    TH1D *hEEm = new TH1D("hEEm","Energia mu",4000,21,25);

    TH2D *hpxx = new TH2D("hpxx","px:x e+",4000,-0.0015,0.0015,4000,-0.001,0.001);
    TH2D *hpxxm = new TH2D("hpxxm","px:x mu",4000,-0.0015,0.0015,4000,-0.001,0.001);
    TH2D *hpyy = new TH2D("hpyy","py:y e+",4000,-0.0015,0.0015,4000,-0.001,0.001);
    TH2D *hpyym = new TH2D("hpyym","py:y mu",4000,-0.0015,0.0015,4000,-0.001,0.001);
    TH2D *hpxdEm = new TH2D("hpxdEm","px:dE_E mu",40,-0.0015,0.0015,40,-0.25,0.25);
    TH2D *hpxdE = new TH2D("hpxdE","px:dE_E e+",4000,-0.0015,0.0015,4000,-0.25,0.25);


    //definizione tree muoni

      TFile f("muoni.root","recreate");
      TTree mu("mu","info sui muoni");
      int partID_mu, turn_mu;
      double x_mu, px_mu, y_mu, py_mu, ct_mu, dE_E_mu, s_mu, E_mu;
      E_mu = 22.5;
      mu.Branch("partID_mu",&partID_mu); //ID particella
      mu.Branch("turn_mu",&turn_mu); // -1 = non ha girato
      mu.Branch("x_mu",&x_mu); // m
      mu.Branch("px_mu",&px_mu); // px/p0
      mu.Branch("y_mu",&y_mu);
      mu.Branch("py_mu",&py_mu);
      mu.Branch("ct_mu",&ct_mu); // -ct? m
      mu.Branch("dE_E_mu",&dE_E_mu); // differenza da nominale GeV
      mu.Branch("s_mu",&s_mu);
      mu.Branch("E_mu",&E_mu); // nominale GeV
   


    ////////// LETTURA TTREE //////////////////////////////////////////
    
      //   cout<<"Lettura TTree"<<endl;

   TTree *positroni = (TTree*)Tinput->Get("pos");
   int partID, turn;
   double x, px, y, py, ct, dE_E,s, E;
   positroni->SetBranchAddress("partID",&partID);
   positroni->SetBranchAddress("turn",&turn);
   positroni->SetBranchAddress("x",&x);
   positroni->SetBranchAddress("px",&px);
   positroni->SetBranchAddress("y",&y);
   positroni->SetBranchAddress("py",&py);
   positroni->SetBranchAddress("ct",&ct);
   positroni->SetBranchAddress("dE_E",&dE_E);//diff da nominale GeV
   positroni->SetBranchAddress("s",&s);
   positroni->SetBranchAddress("E",&E);//nominale GeV

   ////////// LOOP SULLE ENTRIES /////////////////////////////////////
   
   int nentries = positroni->GetEntries();
   //   cout<<"nentries = "<<nentries<<endl;

   int alive[101];
   int ntargh[101];
   for(int i=-1;i<101;i++){
     alive[i]=0;
     ntargh[i]=i;
   }

   for(int i=0;i<nentries;i++){
     positroni->GetEntry(i);
     alive[turn]++;
   }

   wt = 0;
   for(int i=0;i<nentries;i++){
     positroni->GetEntry(i);
     
     //produzione dei muoni
     xs = xsmumu->Eval(E*(1+dE_E));
     z = lrad->GetRandom();
     S = 2*m_e*(E*(1+dE_E));

     if(sqrt(S)/2>m_mu){
     theta_max = 4*m_e/S*sqrt(S/4-pow(m_mu,2));
     th = gRandom->Uniform(-theta_max,theta_max);
     phi = gRandom->Uniform(0,pi);
     
     n_mu = n_muXS*xs; // xs, cross section in ubarn
     w = n_mu/alive[0]; // normalize to the population of the initial bunch
     if (turn>=0)     wt = wt + w; // do not add the initial bunch turn=-1
     //     cout << n_mu << " " << xs << " " <<alive[0] << " " << w<<endl;
     deltaEcorr = sqrt(S)/(2*m_e)*sqrt(S/4-pow(m_mu,2));
     dE_E_mu = (E*(1+dE_E)/2-E_mu)/E_mu + gRandom->Uniform(-deltaEcorr,deltaEcorr)/E_mu;
     //dE_E_mu = deltaEcorr/E_mu;


     x_mu = x+(l_tgt-z)*sin(th*cos(phi))/cos(th*cos(phi));
     px_mu = px+th*cos(phi);
     //px_mu = th*cos(phi);
     
     y_mu = y+(l_tgt-z)*sin(th*sin(phi))/cos(th*sin(phi));
     py_mu = py+th*sin(phi);

     ct_mu=ct;
     s_mu=0;

     partID_mu = partID;
     turn_mu = turn+1;

     out<<partID<<" "<<turn_mu<<" "<<x_mu<<" "<<px_mu<<" "<<y_mu<<" "<<py_mu<<" "<<ct_mu<<" "<<dE_E_mu<<" "<<s_mu<<" "<<E_mu<<" "<<w<<endl;
     mu.Fill();
     }
     
     if(debug==true){//istogrammi debug
       hxx->Fill(x);
       hpx->Fill(px);
       hyy->Fill(y);
       hpy->Fill(py);
       hct->Fill(ct);
       hdE->Fill(dE_E);
       hEE->Fill(E*(1+dE_E));
       hAL->Fill(partID);
       
       hxxm->Fill(x_mu);
       hpxm->Fill(px_mu);
       hyym->Fill(y_mu);
       hpym->Fill(py_mu);
       hctm->Fill(ct_mu);
       hdEm->Fill(dE_E_mu);
       hEEm->Fill(E*(1+dE_E)/2);

       hpxx->Fill(px,x);
       hpxxm->Fill(px_mu,x_mu);
       hpyy->Fill(py,y);
       hpyym->Fill(py_mu,y_mu);

       if(turn==0)hpxdE->Fill(px,dE_E);
       if(turn_mu==0)hpxdEm->Fill(px_mu,dE_E_mu);
     }
     
   }//fine loop sulle entries
   cout << "  :::"<<wt<<" muons were produced !!!" << endl;
   mu.Write();
   out.close();

   if(debug==true){
     TCanvas *xx = new TCanvas;
     hxx->Draw();
     TCanvas *px = new TCanvas;
     hpx->Draw();
     TCanvas *yy = new TCanvas;
     hyy->Draw();
     TCanvas *py = new TCanvas;
     hpy->Draw();
     TCanvas *ct = new TCanvas;
     hct->Draw();
     TCanvas *dE = new TCanvas;
     hdE->Draw();
     TCanvas *EE = new TCanvas;
     hEE->Draw();
     TCanvas *AL = new TCanvas;
     hAL->Draw();
     TCanvas *vive = new TCanvas;
     TGraph *cvive = new TGraph(101,ntargh,alive);
     cvive->SetFillColor(38);
     cvive->Draw("AB");

     TCanvas *xxm = new TCanvas;
     hxxm->Draw();
     TCanvas *pxm = new TCanvas;
     hpxm->Draw();
     TCanvas *yym = new TCanvas;
     hyym->Draw();
     TCanvas *pym = new TCanvas;
     hpym->Draw();
     TCanvas *ctm = new TCanvas;
     hctm->Draw();
     TCanvas *dEm = new TCanvas;
     hdEm->Draw();
     TCanvas *EEm = new TCanvas;
     hEEm->Draw();

     TCanvas *pxx = new TCanvas;
     hpxx->Draw();
     TCanvas *pxxm = new TCanvas;
     hpxxm->Draw();
     TCanvas *pxxtot = new TCanvas;
     hpxx->Draw();
     hpxxm->Draw("same");
     TCanvas *pyy = new TCanvas;
     hpyy->Draw();
     TCanvas *pyym = new TCanvas;
     hpyym->Draw();
     TCanvas *pyytot = new TCanvas;
     hpyy->Draw();
     hpyym->Draw("same");

     TCanvas *pxdEm = new TCanvas;
     hpxdEm->Draw("col");
     hpxdE->Draw("same");
     //mu.Draw("px_mu:dE_E_mu","turn_mu==0","col");

     TCanvas *confr = new TCanvas;
     confr->Divide(3,2);
     
     confr->cd(1);
     mu.SetLineColor(kBlue);
     mu.Draw("x_mu","turn_mu==0","");
     mu.SetLineColor(kRed);
     mu.Draw("x_mu","turn_mu==100","same");

     confr->cd(2);
     mu.SetLineColor(kBlue);
     mu.Draw("y_mu","turn_mu==0","");
     mu.SetLineColor(kRed);
     mu.Draw("y_mu","turn_mu==100","same");
     
     confr->cd(3);
     mu.SetLineColor(kBlue);
     mu.Draw("ct_mu","turn_mu==0","");
     mu.SetLineColor(kRed);
     mu.Draw("ct_mu","turn_mu==100","same");
     
     confr->cd(4);
     mu.SetLineColor(kBlue);
     mu.Draw("px_mu","turn_mu==0","");
     mu.SetLineColor(kRed);
     mu.Draw("px_mu","turn_mu==100","same");
     
     confr->cd(5);
     mu.SetLineColor(kBlue);
     mu.Draw("py_mu","turn_mu==0","");
     mu.SetLineColor(kRed);
     mu.Draw("py_mu","turn_mu==100","same");
     
     confr->cd(6);
     mu.SetLineColor(kBlue);
     mu.Draw("E_mu*(1+dE_E_mu)","turn_mu==0","");
     mu.SetLineColor(kRed);
     mu.Draw("E_mu*(1+dE_E_mu)","turn_mu==100","same");
 
     
   }



}//FINE


