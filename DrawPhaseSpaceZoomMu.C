// oblancog. 2016.09.xx Drawing beam profiles
// this code was used to generate the initial coordinates of the particles
// now it plots beam profiles

// For the particle generation
#include "TRandom2.h"//period=10**26, fast, 32 bits for the state
// To check if the courant-snyder invariant 
#include "TMath.h"
// To read/write into a file
#include <iostream>
#include "TFile.h"
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

int DrawPhaseSpaceZoomMu (const char * k, const int giro=-2) {//k is the flag name , giro is the turn number
  // if giro =-2 print all turns
  cout << "  Using flag : "<< k << endl;
  TString * myflname = new TString(k);
  //Beam geometrical emittances
  double_t ex = 1;
  double_t ey = 1;
  double_t et = 1;
  // Phase space at input
  double_t gammax = 0.5;
  double_t betax = 2;
  double_t alfax = -0.5;
  double_t gammay = 1;
  double_t betay = 1;
  double_t alfay = 0;
  double_t Energyspread = 1;
  double_t etax=1;//off-momentum function
  double_t etapx=1;//off-momentum function
  double_t etay=0;//off-momentum function
  double_t etapy=0;//off-momentum function
  // Linear beam sizes from emittances and twiss
  double_t sigmax0 = 0;
  double_t sigmapx0 = 0;
  double_t sigmay0 = 0;
  double_t sigmapy0 = 0;
  double_t sigmas0 = 0;
  double_t sigmad0 = 0;
  double_t offsetx0 = 0;
  double_t offsety0 = 0;

  ofstream mydebug;
  ofstream mymadxtrac;
  ifstream beta0in;
  string beta0string;
  ifstream beam0in;
  string beam0string;
  ifstream track0in;
  string track0string;

  int nlines;
  char beta0line[80];
  char beam0line[80];
  char track0line[80];

  char madx00[20];
  char madx01[20];
  char madx02[20];
  char madx03[20];
  char madx04[20];

  char track00[20];
  char track01[20];
  char track02[20];
  char track03[20];
  char track04[20];
  char track05[20];
  char track06[20];
  char track07[20];
  char track08[20];
  char track09[20];
  char track10[20];
  char track11[20];

  // Read twiss info
  TString * betafl = new TString("beta");
  betafl->Append(k);
  betafl->Append(".txt");
  beta0in.open(betafl->Data());
  if (!beta0in) {
    // if we cannot open the file, 
    // print an error message and return immediatly
    printf("Error: cannot open betaXXXX.txt!\n");
    //return 1;
  }else{
    cout << "  ... reading file "<<betafl->Data()<<" (twiss params at input)"<<endl;
    beta0in >>  madx00 >> madx01 >> madx02 >> madx03;
    while(!beta0in.eof()){
      beta0in >> madx00 >> madx01 >> madx02 >> madx03 >> madx04;
      //    cout << madx01<<endl;
      if (!strcmp(madx01,"betx")) {cout<<"    "<<madx01<<" "<<madx04<<endl;betax=atof(madx04);}
      if (!strcmp(madx01,"alfx")) {cout<<"    "<<madx01<<" "<<madx04<<endl;alfax=atof(madx04);}
      if (!strcmp(madx01,"bety")) {cout<<"    "<<madx01<<" "<<madx04<<endl;betay=atof(madx04);}
      if (!strcmp(madx01,"alfy")) {cout<<"    "<<madx01<<" "<<madx04<<endl;alfay=atof(madx04);}
      if (!strcmp(madx01,"dx")) {cout<<"    "<<madx01<<" "<<madx04<<endl;etax=atof(madx04);}
      //    if (!strcmp(madx01,"dpx")) {cout<<"    "<<madx01<<" "<<madx04<<endl;etapx=atof(madx04);}
      if (!strcmp(madx01,"dy")) {cout<<"    "<<madx01<<" "<<madx04<<endl;etay=atof(madx04);}
      //if (!strcmp(madx01,"dpy")) {cout<<"    "<<madx01<<" "<<madx04<<endl;etapy=atof(madx04);}
    }
    cout << "    ... all others ignored.";
    cout << "  "<<betafl->Data()<<" read."<<endl;
    beam0in.close();
    cout << "  Calculating gamma[xy]..."<<endl;
    gammax = (1 + alfax*alfax )/betax;
    gammay = (1 + alfay*alfay )/betay;
    cout <<"    gammax "<<gammax<<endl;
    cout <<"    gammay "<<gammay<<endl;
  }
  // Reading beam info
  beam0in.open("beam0.txt");
  if (!beam0in) {
    // if we cannot open the file, 
    // print an error message and return immediatly
    printf("Error: cannot open beam0.txt!\n");
    //return 1;
  }else{
    cout << "  ... reading file beam0.txt (beam params)"<<endl;
    beam0in >>  madx00 >> madx01 >> madx02 >> madx03;
    while(!beam0in.eof()){
      beam0in >> madx01;// >> madx02 >> madx03 >> madx04;
      //    cout << madx01<<endl;
      //    cout << madx01<<endl;
      if (!strcmp(madx01,"ex")){
	beam0in >> madx02;
	beam0in >> madx03;
	beam0in >> madx04;
	cout<<"    "<<madx01<<" "<<madx04<<endl;
	ex=atof(madx04);}
      if (!strcmp(madx01,"ey")){
	beam0in >> madx02;
	beam0in >> madx03;
	beam0in >> madx04;
	cout<<"    "<<madx01<<" "<<madx04<<endl;
	ey=atof(madx04);}
      if (!strcmp(madx01,"sige")){
	beam0in >> madx02;
	beam0in >> madx03;
	beam0in >> madx04;
	cout<<"    "<<madx01<<" "<<madx04<<endl;
	Energyspread=atof(madx04);}
    }
    cout << "    ... all others ignored.";
    cout << "    beam0.txt read."<<endl;
    beam0in.close();
  // Calculate sigma0
    sigmax0  = TMath::Sqrt(ex*betax + etax*etax*Energyspread*Energyspread);
    sigmapx0 = TMath::Sqrt(ex/betax);
    sigmay0  = TMath::Sqrt(ey*betay + etay*etay*Energyspread*Energyspread);
    sigmapy0 = TMath::Sqrt(ey/betay);
    sigmas0  = TMath::Sqrt(et*0);
    offsetx0 = etax*Energyspread;
    offsety0 = etay*Energyspread;
    //  sigmad0  = TMath::Sqrt(et*0);
  }  


  float scalehv=1e3;//mm
  Double_t w = 1800;
  Double_t h =  600;
  TCanvas * c1 = new TCanvas(betafl->Data(),betafl->Data(), w, h);
  c1->SetGrid();
  c1->Divide(3,1,0.01,0.01);
  c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));
  //  TCanvas *c1 = new TCanvas("c1");
  c1->cd(1);

  //  c1->Range(-40e-3*scalehv,-40e-3*scalehv,40e-3*scalehv,40e-3*scalehv);
  //  c1->SetFillColor(42);
  //  c1->SetGrid(5,5);
  //  c1->SetGridx(2);
  //  c1->SetGridy(2);
  //  c1->SetFillStyle(4000);
  //  c1->Draw();
  //  c1->Update();

  // From tracking
  // Read trackinfo
  //  TCanvas * c2 = new TCanvas("c2", "c2", w, h);


  //  gStyle->SetPalette(56);


  TString * trackfl = new TString("");
  trackfl->Append(k);
  TH2 * trackxpx = new TH2F("H. Phase Space","H Phase Space",400,-4   ,4   ,400,-8    ,8.0  );//number,-x,x,number,-y,y
  TH2 * trackypy = new TH2F("V. Phase Space","V Phase Space",400,-4   ,4   ,400,-8    ,8.00 );
  TH2 * tracksd  = new TH2F("L. Phase Space","L. Phase Space",400,-40.0,40.0,400,-800.0,800.0);

  //  betafl->Append(".txt");
  track0in.open(trackfl->Data());
  //  track0in.open("trackSTART");
  if (!track0in) {
    // if we cannot open the file, 
    // print an error message and return immediatly
    cout<<"Error: cannot open file "<<trackfl->Data()<<"\n";
    return 1;
  }
  cout << "  ... reading file "<<trackfl->Data()<<" (tracking results)"<<endl;
  //  track0in >>  madx00 >> madx01 >> madx02 >> madx03;
  while(!track0in.eof()){
    track0in >> track01 >> track02 >> track03 >> track04 >> track05 >> track06 >> track07 >> track08 >> track09 >> track10 >> track11;
    //    cout << track03<<track05<< endl;
    if (giro == atof(track02) || giro == -2){
      trackxpx->Fill(atof(track03)*scalehv,atof(track04)*scalehv);
      trackypy->Fill(atof(track05)*scalehv,atof(track06)*scalehv);
      tracksd-> Fill(atof(track07)*scalehv,atof(track08)*scalehv);
    }
  }
  //  cout << "    ... all others ignored.";
  cout << "  "<<trackfl->Data()<<" read."<<endl;
  track0in.close();
  cout << "  Calculating gamma[xy]..."<<endl;
  gammax = (1 + alfax*alfax )/betax;
  gammay = (1 + alfay*alfay )/betay;
  cout <<"    gammax "<<gammax<<endl;
  cout <<"    gammay "<<gammay<<endl;
  c1->cd(1);
  trackxpx->Draw("colz");
  trackxpx->SetTitle("H. Phase Space");
  trackxpx->GetXaxis()->SetTitle("x [mm]");
  trackxpx->GetYaxis()->SetTitle("px/p0 [mrad]");
  trackxpx->GetXaxis()->CenterTitle();
  trackxpx->GetYaxis()->CenterTitle();
  c1->cd(2);
  trackypy->Draw("colz");
  trackypy->SetTitle("V. Phase Space");
  trackypy->GetXaxis()->SetTitle("y [mm]");
  trackypy->GetYaxis()->SetTitle("py/p0 [mrad]");
  trackypy->GetXaxis()->CenterTitle();
  trackypy->GetYaxis()->CenterTitle();
  c1->cd(3);
  tracksd->Draw("colz");
  tracksd->SetTitle("L. Phase Space");
  tracksd->GetXaxis()->SetTitle("t [mm]");
  tracksd->GetYaxis()->SetTitle("e.spread/(p0*clight) [1e-3]");
  tracksd->GetXaxis()->CenterTitle();
  tracksd->GetYaxis()->CenterTitle();

  c1->RedrawAxis();
  c1->SaveAs(myflname->Append("phase.pdf"));


  return 0;
}

