#!/usr/bin/env python
#____________________________________________________________
#
#  SubmitJobs
#
# A very simple way to submit jobs
#
# Yanyan Gao
# Yanyan.Gao@cern.ch
#
# Fermilab, 2010
#
#____________________________________________________________

import sys,os,re,string
import commands

#####
release =os.environ["CMSSW_VERSION"]
workdir = "/afs/cern.ch/cms/tracking/workareas/yygao/LhcTrackAnalysis/"+release+"/"
#print workdir
cfidir = "../../python/"
cfifilepath = "UserCode.LhcTrackAnalyzer."

##### End of setting variables

def main():
    dataname = "Run132440_Express"
    lfiles = []
    nslscmd = "nsfind  /castor/cern.ch/cms/store/express/Commissioning10/ExpressPhysics/FEVT/v7/000/132/440/ > tmplist.txt "
    print nslscmd
    os.system(nslscmd)

    prefix = "rfio:"
    n=0
    fileHandle = open("tmplist.txt")
    lineList = fileHandle.readlines()
    nfiles = len(lineList)-1

    lineList = map(string.strip, lineList)
   
    for f in lineList:
        #print n
        if f.find("store") != -1:
            #print f
            if n < nfiles:
                if n % 256 == 0:
                    if n == 0:
                        lfiles.append("      '" + prefix + f + "',\n")
                    else:
                        lfiles.append("      '" + prefix + f + "' ] ) ;\n")
                        lfiles.append("readFiles.extend( [\n")
                else:
                    lfiles.append("      '" + prefix + f + "',\n")
            else:
                lfiles.append("      '" + prefix + f + "'\n")
        n=n+1
        fileHandle.close()

        inputtmp = open("Input_template_cfi.py")
        cfiname = cfidir+dataname+"_cfi.py"
        #print cfiname
        
        fout = open(cfiname,"w")
        
        for line in inputtmp:
            if line.find("INPUTFILES")!=-1:
                for f in lfiles:
                    fout.write(f)
            if not "INPUTFILES" in line:
                fout.write(line)
        fout.close()
        
        ## Copy cfi file to run directories
        #os.system("cp "+cfiname+" "+workdir)

#_________________________________    
if __name__ =='__main__':
    sys.exit(main())


