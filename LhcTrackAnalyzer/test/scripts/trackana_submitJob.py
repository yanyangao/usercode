#!/usr/bin/env python
#____________________________________________________________
#
#  SubmitJobs locally
#
# Example:
# python trackana_submitJob.py  -dataset   /MinimumBias/Run2010A-PromptReco-v4/RECO -datamode 138747 -lumi 138747:1-138747:max -gtag GR10_P_V7::All -nEvts -1 -dir work/
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
workdir = "."
dataset = ''
lumi = ''
sequence = "only_analyze"
nevent = "50000"
global_tag_flag = ''#"GR10_P_V7::All"
global_tag = '' 
##### End of setting variables

def runjob(datamode):
    lfiles = []
    dataname = dataset.split('/')[1]+'_'+dataset.split('/')[2]
    print dataname
    
    if datamode == "mc":
        dbsquery = "dbs search --noheader --query=\"find file where dataset="+dataset+"\" > tmplist.txt"
    elif datamode == "data":
        dbsquery = "dbs search --noheader --query=\"find file where dataset="+dataset+"\" > tmplist.txt"
    else:
        dbsquery = "dbs search --noheader --query=\"find file where dataset="+dataset+" and run = "+datamode + " \" > tmplist.txt"

    print dbsquery
    os.system(dbsquery)

    n=0
    fileHandle = open("tmplist.txt")
    lineList = fileHandle.readlines()
    nfiles = len(lineList)-1
    lineList = map(string.strip, lineList)
    prefix = ""

    for f in lineList:
        if f.find("store") != -1:
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
                        
    dataname = dataset.split('/')[1]+'_'+dataset.split('/')[2]
    
    ntupledir = workdir+"/"+release+"/ntuple/"
    cfgdir = workdir+"/"+release+"/cfgfiles/"
    logdir = workdir+"/"+release+"/log/"
    epsdir = workdir+"/"+release+"/epsfiles/"
    pngdir = workdir+"/"+release+"/pngfiles/"
    
    os.system("mkdir -p "+cfgdir)
    os.system("mkdir -p "+logdir) 
    os.system("mkdir -p "+epsdir)
    os.system("mkdir -p "+pngdir)
    os.system("mkdir -p "+ntupledir)

    if datamode == "mc":
        cmsswSkelFile = "trackana_template_MC_cfg.py"
    else:
        cmsswSkelFile = "trackana_template_Data_cfg.py"
    if os.path.exists(cmsswSkelFile) == False:
        print 'CMSSW skeleton file does not exist. Exiting'
        sys.exit()
        
    inFile = open(cmsswSkelFile, 'r').read().split('\n')
    outFileName = cfgdir+dataname+"_"+datamode+"_"+sequence+"_cfg.py"

    print 'Writing CMS2 CMSSW python config file : ' + outFileName
    outFile = open(outFileName, 'w')

    for i in inFile:
        if i.find("INPUTFILES") != -1:
            for f in lfiles:
                outFile.write(f);
            continue
        if i.find('GLOBALTAG') != -1:
            outFile.write('process.GlobalTag.globaltag = "' + global_tag + '"\n'); continue
        if i.find('NEVENT') != -1:
           outFile.write('process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(' + str(nevent) + '))\n'); continue
        if i.find('SEQUENCE') != -1:
            outFile.write('process.p = cms.Path(process.GOODCOLL*process.' + sequence +  ')\n'); continue
        if i.find('LUMIRANGE') != -1:
            if lumi != '': 
                outFile.write('process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange( \'' + lumi +  '\')\n'); continue
            else:
                outFile.write(''); continue
        if i.find('OUTFILE') != -1:
            outFile.write('process.trackana.OutputFileName = cms.string("' + ntupledir + dataname + '_' + sequence + '.root")\n'); continue                
                
        outFile.write(i+'\n')        

    outFile.close()

    os.system("cd $CMSSW_BASE/src/ ; scramv1 b ;")
    runjobcmd = "cmsRun "+outFileName+" >& "+logdir+dataname+"_"+datamode+"_"+sequence+".log &"
    print runjobcmd
    os.system(runjobcmd)
                                                                
###############################
####### Main Program
###############################

if len(sys.argv) < 5 :
    print 'Usage: trackana_submitJob.py [OPTIONS]'
    print '\nWhere the required options are: '
    print '\t-dataset\t\tname of dataset'
    print '\t-datamode\t\tdata mode <data/mc/runno(interger)>'
    print '\nOptional arguments:'
    print '\t-dir\tworkdir. Default is the current directory'
    print '\t-nEvts\tNumber of Events.Default is 50000'
    print '\t-lumi\tLumi range. Default is the entire run'
    print '\t-gtag\tglobaltag. If not specfied, will take it directly from the DBS'
    sys.exit()
    
for i in range(0, len(sys.argv)):
    if sys.argv[i] == '-dataset':
        dataset = sys.argv[i+1]
    if sys.argv[i] == '-datamode':
        datamode = sys.argv[i+1]
    if sys.argv[i] == '-dir':
        workdir = sys.argv[i+1]
    if sys.argv[i] == '-nEvts':
        nevent = sys.argv[i+1]
    if sys.argv[i] == '-lumi':
        lumi = sys.argv[i+1]
    if sys.argv[i] == '-gtag':
        global_tag_flag = sys.argv[i+1]

if( global_tag_flag != '' ):
    print '\nUsing \'' + global_tag_flag + '\' specified by -gtag flag.\n'
    global_tag = global_tag_flag
else:       
    print '\nGetting global tag from DBS...'
    dbs_result = '';
    command = 'dbsql find config.name,config.content where dataset=' + dataset + '>config.content; while read line; do globaltag=`echo $line | sed -n \'s/^.*process.GlobalTag.globaltag = \([^p]*\).*$/\\1/p\'`; if [ "$globaltag" != "" ]; then echo $globaltag; break; fi; done <config.content; rm config.content';
    lines = os.popen(command);
    for i in lines.readlines():
        dbs_result = re.sub('\n', '', i)
        global_tag = re.sub('#.+$', '', dbs_result)
    print global_tag       
    if global_tag == "":
        print '\n GlobalTag is empty from the DBS, please specifiy globalTag manually by -gtag options in the format of(TAG::All) '
        sys.exit()

runjob(datamode)
