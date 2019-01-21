// oblancog. 2018.11.22 fitting particle distributions
// this code reads the track output and finds the Mean and RMS values


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


int MeanRMS6Dcut_cernroot (const char * fnametrack, const int giro=-2){
  // if giro is -2, uses all turns
  TString * myflname = new TString(fnametrack);
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
  double_t blength = 1;
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

  // Read twiss info
  TString * betafl = new TString("betaSTART.txt");
  //betafl->Append(".txt");
  beta0in.open(betafl->Data());
  if (!beta0in) {
    // if we cannot open the file, 
    // print an error message and return immediatly
    cout << "Error: cannot open file betaSTART.txt !"<<endl;
    return 1;
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
    cout <<"    twissgammax "<<gammax<<endl;
    cout <<"    twissgammay "<<gammay<<endl;
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
      if (!strcmp(madx01,"sigt")){
	beam0in >> madx02;
	beam0in >> madx03;
	beam0in >> madx04;
	cout<<"    "<<madx01<<" "<<madx04<<endl;
	blength=atof(madx04);}
    }
    cout << "    ... all others ignored.";
    cout << "    beam0.txt read."<<endl;
    beam0in.close();
  // Calculate sigma0
    sigmax0  = TMath::Sqrt(ex*betax + etax*etax*Energyspread*Energyspread);
    sigmapx0 = TMath::Sqrt(ex/betax);
    sigmay0  = TMath::Sqrt(ey*betay + etay*etay*Energyspread*Energyspread);
    sigmapy0 = TMath::Sqrt(ey/betay);
    sigmas0  = blength;//TMath::Sqrt(et*0);
    sigmad0  = Energyspread;
    offsetx0 = etax*Energyspread;
    offsety0 = etay*Energyspread;
    //  sigmad0  = TMath::Sqrt(et*0);
    cout << "  Beam size to first order "<<endl;
    cout << "    sigmax0  = " << sigmax0 << " m"<< endl;
    cout << "    sigmapx0 = " << sigmapx0 << " rad"<< endl;
    cout << "    sigmay0  = " << sigmay0 << " m"<< endl;
    cout << "    sigmapy0 = " << sigmapy0 << " rad"<< endl;
    cout << "    sigmas0  = " << sigmas0 << " m"<< endl;
    cout << "    sigmad0  = " << sigmad0 << " dP/P"<< endl;
  }  

  track0in.open(myflname->Data());
  if (!track0in) {
    // if we cannot open the file, 
    // print an error message and return immediatly
    cout << "Error: cannot open file "<<myflname->Data()<<" !"<<endl;
    return 1;
  }else{
    float scalehv=1e3;//mm
    Double_t w = 1800;
    Double_t h =  600;

    // introducing cuts in the distrubution
    float cutsinsigma=10;
    

    TH2 * trackxpx = new TH2F("H. Phase Space","H Phase Space",400,-4,4,400,-8,8);//number,-x,x,number,-y,y
    TH2 * trackypy = new TH2F("V. Phase Space","V Phase Space",400,-4.00,4.00,400,-8,8);
    TH2 * tracksd = new TH2F("L. Phase Space","L. Phase Space",400,-400.0,400.0,400,-400.0,400.0);

    int    curturn = -2;
    ofstream pok;
    int npok = 0;
    pok.open ("pok.txt", fstream::app);
    while(!track0in.eof()){
      track0in >> track01 >> track02 >> track03 >> track04 >> track05 >> track06 >> track07 >> track08 >> track09 >> track10;
      if (giro == atof(track02) || giro == -2){
	curturn = atof(track02);
	if(abs(atof(track03)) < cutsinsigma*sigmax0 && 
	   abs(atof(track04)) < cutsinsigma*sigmapx0 &&
	   abs(atof(track05)) < cutsinsigma*sigmay0 &&
	   abs(atof(track06)) < cutsinsigma*sigmapy0 &&
	   abs(atof(track07)) < cutsinsigma*sigmas0 &&
	   abs(atof(track08)) < cutsinsigma*sigmad0 
	   ){
	  trackxpx->Fill(atof(track03)*scalehv,atof(track04)*scalehv);
	  trackypy->Fill(atof(track05)*scalehv,atof(track06)*scalehv);
	  tracksd-> Fill(atof(track07)*scalehv,atof(track08)*scalehv);
	}
      }
    }
    pok << curturn << " " << tracksd->GetEntries()<<endl;
    pok.close();
    TH1 * trackx = trackxpx->ProjectionX("x");
    TH1 * trackpx = trackxpx->ProjectionY("px");
    TH1 * tracky = trackypy->ProjectionX("y");
    TH1 * trackpy = trackypy->ProjectionY("py");
    TH1 * tracks = tracksd->ProjectionX("s");
    TH1 * trackd = tracksd->ProjectionY("d");
        trackd->Draw();
    TFitResultPtr rx = trackx->Fit("gaus","NSQ");
    TFitResultPtr rpx = trackpx->Fit("gaus","NSQ");
    TFitResultPtr ry = tracky->Fit("gaus","NSQ");
    TFitResultPtr rpy = trackpy->Fit("gaus","NSQ");
    TFitResultPtr rs = tracks->Fit("gaus","NSQ");
    TFitResultPtr rd = trackd->Fit("gaus","NSQ");
    /*    cout << "  sigmaxfit  = "<< rx->Parameter(2) <<endl;
    cout << "  sigmapxfit  = "<< rpx->Parameter(2) <<endl;
    cout << "  sigmayfit  = "<< ry->Parameter(2) <<endl;
    cout << "  sigmapyfit  = "<< rpy->Parameter(2) <<endl;
    cout << "  sigmasfit  = "<< rd->Parameter(2) <<endl;
    cout << "  sigmadfit  = "<< rs->Parameter(2) <<endl;
    */

    float remitx = rx->Parameter(2)*rpx->Parameter(2)*TMath::Sqrt(1- TMath::Power(trackxpx->GetCorrelationFactor(),2) ); 
    float remity = ry->Parameter(2)*rpy->Parameter(2)*TMath::Sqrt(1- TMath::Power(trackypy->GetCorrelationFactor(),2) ); 
    float remite = rs->Parameter(2)*rd->Parameter(2) *TMath::Sqrt(1- TMath::Power(tracksd->GetCorrelationFactor(),2) ); 
    
    ofstream fitresult;
    
    fitresult.open ("fitresult.txt", fstream::app);
    fitresult << curturn << " "
	      << rx->Parameter(1) /scalehv << " " 
	      << rx->Parameter(2) /scalehv << " "
	      << rpx->Parameter(1)/scalehv << " "
	      << rpx->Parameter(2)/scalehv  << " "
	      << ry->Parameter(1) /scalehv << " "
	      << ry->Parameter(2) /scalehv << " "
	      << rpy->Parameter(1)/scalehv << " "
	      << rpy->Parameter(2)/scalehv  << " "
	      << rs->Parameter(1) /scalehv << " "
	      << rs->Parameter(2) /scalehv << " "
	      << rd->Parameter(1) /scalehv  << " "
	      << rd->Parameter(2) /scalehv << " "
	      << " emit "
	      << remitx /scalehv /scalehv << " "<< trackxpx->GetCorrelationFactor()<<" "
	      << remity /scalehv /scalehv << " "<< trackypy->GetCorrelationFactor()<<" "
	      << remite /scalehv /scalehv << " "<< tracksd->GetCorrelationFactor()<<" "
	      << endl;
  }



  return 0;
}


int fitthetrack ( const char * k){
  ofstream fitresult;
  ofstream pok;

  if (1) fitresult.open ("fitresult.txt");
  fitresult << "# turn meanx rmsx meanpx rmspx meany rmsy meanpy rmspy meant rmst meanpt rmspt ex corx ey cory et cort"<<endl; 
  fitresult.close();
  if (1) pok.open ("fitpok.txt");
  pok << "# turn n_par" <<endl;
  pok.close();
  
  for (int i=-1; i<101; i++){
    MeanRMS6Dcut_cernroot(k,i);
  }
  return 0;
}
