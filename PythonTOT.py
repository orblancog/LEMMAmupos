#file = open("uno")
import random
import sys

NRigheMad=int(sys.argv[5])#10
#print sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],sys.argv[5],sys.argv[6],sys.argv[7]
NColonneMad=int(sys.argv[4])#200

NRigheMC=int(sys.argv[7])#6
NColonneMC=int(sys.argv[6])#1e4

clight = 299792458 #[m]

partfname =sys.argv[1]
targfname =sys.argv[2]
ofname    =sys.argv[3]

outfile=open(ofname,"w+")
#outfile.write("OutputFile\n")
#outfile.write("X\t PX\t Y\t PY\t T\t PT\n ")


with open (partfname,"r") as madfile:
    with open (targfname, "r") as mcfile:
        dataMC=mcfile.read()
        wordsMC = dataMC.split(None)
        dataMAD=madfile.read()
        wordsMAD = dataMAD.split(None)
        #            print words1[3] + ",", words2[2]
        
        '''        
        for MadIndex in range(0,NColonneMad):
        RndIndex=random.randint(0, NColonneMC-1)
        print "Indice Mad %d, Indice Random %d" % (MadIndex, RndIndex)
        print "X= %lf" %(float(wordsMAD[2+MadIndex*NRigheMad]) + float(wordsMC[0+RndIndex*NRigheMC]))
        print "PX= %lf" %(float(wordsMAD[3+MadIndex*NRigheMad]) + float(wordsMC[1+RndIndex*NRigheMC]))
        print "Y= %.9f" %(float(wordsMAD[4+MadIndex*NRigheMad]) + float(wordsMC[2+RndIndex*NRigheMC]))
        print "PY= %.9f" %(float(wordsMAD[5+MadIndex*NRigheMad]) + float(wordsMC[3+RndIndex*NRigheMC]))
        '''

        
        
        for MadIndex in range(0,NColonneMad):
            RndIndex=random.randint(0, NColonneMC-1)

            ResX=(float(wordsMAD[2+MadIndex*NRigheMad]) + 0*float(wordsMC[0+RndIndex*NRigheMC]))
            ResPX=(float(wordsMAD[3+MadIndex*NRigheMad]) + 0*float(wordsMC[1+RndIndex*NRigheMC]))
            ResY=(float(wordsMAD[4+MadIndex*NRigheMad]) + 0*float(wordsMC[2+RndIndex*NRigheMC]))
            ResPY=(float(wordsMAD[5+MadIndex*NRigheMad]) + 0*float(wordsMC[3+RndIndex*NRigheMC]))
            ResT=(float(wordsMAD[6+MadIndex*NRigheMad]) - 0*clight*float(wordsMC[4+RndIndex*NRigheMC]))
            ResPT=(float(wordsMAD[7+MadIndex*NRigheMad]) + 0*float(wordsMC[5+RndIndex*NRigheMC]))
            
            print  >> outfile, "%.9f\t%.9f\t%.9f\t%.9f\t%.9f\t%.9f\t" %( ResX, ResPX, ResY , ResPY, ResT, ResPT )
#            print  >> outfile, "%.9f\t%.9f\t%.9f\t%.9f\t" %( (float(wordsMAD[2+MadIndex*NRigheMad]) + float(wordsMC[0+RndIndex*NRigheMC])), (float(wordsMAD[3+MadIndex*NRigheMad]) + float(wordsMC[1+RndIndex*NRigheMC])),  (float(wordsMAD[4+MadIndex*NRigheMad]) + float(wordsMC[2+RndIndex*NRigheMC])),  (float(wordsMAD[5+MadIndex*NRigheMad]) + float(wordsMC[3+RndIndex*NRigheMC])))

            
    #for i in range(5):
    #print i
   # line = myfile.readline()
    #words = line.split()
    #print words[3]
