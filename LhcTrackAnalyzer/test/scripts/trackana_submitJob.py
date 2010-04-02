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
release = os.environ["CMSSW_VERSION"]
workdir = "/afs/cern.ch/cms/tracking/workareas/yygao/LhcTrackAnalyzer/"+release+"/"
sequence = "only_analyze"
nevent = "50000"
dataset = "/MinBias/Spring10-START3X_V25B_356ReReco-v1/GEN-SIM-RECO"
#"/MinimumBias/Commissioning10-CollisionRecoSequence-v7/RECO"
#"/MinimumBias/Commissioning10-PromptReco-v7/RECO"
#"/ExpressPhysics/Commissioning10-Express-v7/FEVT"
dataname ="MinBias_Spring10_START3X_V25B_356ReReco_v1"
#"Run132440_CollisionSequence"

#"MinBias_Spring10_START3X_V25_354p1ReReco_v1"
#"MinBias_Spring10_START3X_V25B_v1"
#"MinBias900GeV_Mar24thReReco"
globaltag = "GR10_H_V4"
#"START3X_V25B"
#"GR10_P_V4"
#"START3X_V25B"
#"GR09_R_35X_V4"


######
ntupledir = workdir+"ntuple/"
cfgdir = workdir+"cfgfiles/"
logdir = workdir+"log/"
epsdir = workdir+"epsfiles/"
pngdir = workdir+"epsfiles/"

os.system("mkdir -p "+workdir)
os.system("mkdir -p "+cfgdir)
os.system("mkdir -p "+logdir) 
os.system("mkdir -p "+epsdir)
os.system("mkdir -p "+pngdir)
os.system("mkdir -p "+ntupledir)

cfidir = "../../python/"
cfifilepath = "UserCode.LhcTrackAnalyzer."

##### End of setting variables

def get_list_files(datamode,mode):

    lfiles = []
    inputtmp = open("Input_template_cfi.py","r")
    
    if datamode == "mc":
        dbsquery = "dbs search --noheader --query=\"find file where dataset="+dataset+"\" > tmplist.txt"
        cfiname = cfidir+dataname+"_cfi"
    elif datamode == "data":
        dbsquery = "dbs search --noheader --query=\"find file where dataset="+dataset+"\" > tmplist.txt"
        cfiname = cfidir+dataname+"_cfi"
    else:
        dbsquery = "dbs search --noheader --query=\"find file where dataset="+dataset+" and run = "+datamode + " \" > tmplist.txt"
        cfiname = cfidir+dataname+"_"+datamode+"_cfi"
    print dbsquery
    os.system(dbsquery)
    print cfiname

    n=0
    fileHandle = open("tmplist.txt")
    lineList = fileHandle.readlines()
    nfiles = len(lineList)-1

    lineList = map(string.strip, lineList)
    prefix = ""

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

    fout = open(cfiname+".py","w")
    for line in inputtmp:
        if line.find("INPUTFILES")!=-1:
            for f in lfiles:
                fout.write(f)
        if not "INPUTFILES" in line:
            fout.write(line)
    fout.close()
        
    ## Copy cfi file to run directories
    os.system("rm -f  tmplist.txt")    
    os.system("cp "+cfiname+".py "+cfgdir)

###############################
####### Run cmsRUN
###############################
        
def runjob(datamode,jobmode):

    if datamode == "data":
        cfiname = cfifilepath+dataname+"_cfi"
        cfgfileName = cfgdir+dataname+"_"+sequence+"_cfg.py"

    elif datamode == "mc":
        cfiname = cfifilepath+dataname+"_cfi"
        cfgfileName = cfgdir+dataname+"_"+sequence+"_cfg.py"

    else:
        cfiname = cfifilepath+dataname+"_"+datamode+"_cfi"
        cfgfileName = cfgdir+dataname+"_"+datamode+"_"+sequence+"_cfg.py"

    print cfiname
    print cfgfileName

    ## For MC
    if datamode == "mc":
        cfgfile = open(cfgfileName, "w")
        cfgtmp = open("Step1_template_MC_cfg.py")
        replacetag = [
            ('INPUT_FILE',cfiname),
            ('NEVENT',nevent),
            ('SEQUENCE',sequence), 
            ('OUTFILE',ntupledir+dataname+"_"+sequence+".root"),
            ('GLOBALTAG',globaltag) 
            ]
        for line in cfgtmp:
            for itag in replacetag:
                line = line.replace(itag[0],itag[1])
            cfgfile.write(line)
            
    elif datamode == "data":
        cfgfileName = cfgdir+dataname+"_"+sequence+"_cfg.py"
        cfgfile = open(cfgfileName, "w")
        cfgtmp = open("Step1_template_Data_cfg.py")
        lumilist = open("runlumi.txt")
        lineList = lumilist.readlines()
        nLines = len(lineList)-1 
        lumirange = ""

        for f in lineList:
            lumirange += f
        
        replacetag = [
            ('INPUT_FILE',cfiname),
            ('NEVENT',nevent),
            ('LUMIRANGE',lumirange),
            ('SEQUENCE',sequence),
            ('OUTFILE',ntupledir+dataname+"_"+sequence+".root"),
            ('GLOBALTAG',globaltag)
            ]

        for line in cfgtmp:
            for itag in replacetag:
                line = line.replace(itag[0],itag[1])
            cfgfile.write(line)

    else:
        print cfiname
        cfgfileName = cfgdir+dataname+"_"+datamode+"_"+sequence+"_cfg.py"
        cfgfile = open(cfgfileName, "w")
        cfgtmp = open("Step1_template_Data_cfg.py")
        lumilist = open("runlumi.txt")
        lineList = lumilist.readlines()
        nLines = len(lineList)-1 
        lumirange = ""

        for f in lineList:
            lumirange += f
        
        replacetag = [
            ('INPUT_FILE',cfiname),
            ('NEVENT',nevent),
            ('LUMIRANGE',lumirange),
            ('SEQUENCE',sequence),
            ('OUTFILE',ntupledir+dataname+"_"+datamode+"_"+sequence+".root"),
            ('GLOBALTAG',globaltag)
            ]

        for line in cfgtmp:
            for itag in replacetag:
                line = line.replace(itag[0],itag[1])
            cfgfile.write(line)       
                
    cfgfile.close()    ### Finish writing cfg files


###Run cmsRun
    if jobmode == "local":
        #scriptdir = os.environ['PWD']
        #print scriptdir
        os.system("cd $CMSSW_BASE/src/ ; scramv1 b ;")
        runjobcmd = "cmsRun "+cfgfileName+" >& "+logdir+dataname+"_"+sequence+".log &"
        print runjobcmd
        os.system(runjobcmd)
                                                                
###############################
####### Main Program
###############################

def main():
    
    if len(sys.argv) < 5:
        print "\n [Usage] python SubmitJobs.py <data/mc/runno> <getinput/runjob/comptrk> <caf/lxplus/cmslpc> <local/batch>"
        sys.exit()

    datamode = sys.argv[1]
    jobtype = sys.argv[2]
    mode = sys.argv[3]
    jobmode = sys.argv[4]
    
    if mode == "caf":
        fmode = 0
    elif mode == "lxplus":
        fmode = 1
    elif mode == "cmslpc":
        fmode = 2
        jobmode = "local" ## temporary
    else:
        print "Mode not supported"
        sys.exit()
        
    if jobtype == "getinput":
        #print jobtype
        get_list_files(datamode,mode)
    elif jobtype == "runjob":
        runjob(datamode,jobmode)
        
        
    else:
       sys.exit()
        

                        
#_________________________________    
if __name__ =='__main__':
    sys.exit(main())


