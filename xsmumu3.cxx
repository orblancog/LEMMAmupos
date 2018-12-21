double xsmumu3(double*x, double*par) {
  
  double Elab=x[0];
  //     double rts=x[0];
  double me =0.511e-3;
  double mmu=0.105658;
  double rts = pow(2*me*Elab+me*me,0.5);
  double eb = rts/2.;
  double hc2 =0.389 ;
  double alpha = 1./137.;
  double pi=acos(-1.);
  double coulomb=0;
  if(eb-mmu<0)return 0;
  double pmu = pow(eb*eb-mmu*mmu,0.5);
    double beta = pmu/eb;
    
    //  double ctmax=sqrt(rts**2/4.-mmu**2)/(rts**2/(4.*me));
    
    double X = alpha*pi*pow(1-beta*beta,0.5)/beta;
    
    double corr = X/(1-exp(-X)); //coulomb corrections
    
    double  xs = 2*pi*alpha*alpha*beta/((2*eb)*(2*eb))*(1-beta*beta/3.);

    // sqrt(x**2/4.-105.6e-3**2)/(x**2/(4.*0.511e-3))

    // (x**2/(4.*0.511e-3))-sqrt(x**2/4-105.6e-3**2)*x/(2*0.511e-3)  
    // (x**2/(4.*0.511e-3))+sqrt(x**2/4-105.6e-3**2)*x/(2*0.511e-3) 

    double anglemax = pow(rts*rts/4-mmu*mmu,0.5)/(rts*rts/(4*me));
    double dee = 2*pow(rts*rts/4-mmu*mmu,0.5)*rts/(2*me)/(Elab/2.)/pow(12,0.5);

//    corr=1;
//    coulomb=xs*corr*hc2*1000;// ! xs in microbarn
    //    coulomb = anglemax;

        coulomb = xs*hc2*1000*corr;
//    coulomb=dee;
  
    
 
   return coulomb;
   //   return 1;
}
