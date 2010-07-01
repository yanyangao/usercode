#!/usr/bin/env python
#____________________________________________________________
#
#  SubmitJobs locally
#
# Example:
# python makeDataCrabFiles.py -dataset /JetMETTau/Run2010A-PromptReco-v2/RECO -l JSON_Run136100.txt -m condor
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
cmsswSkelFile = "trackana_template_Data_cfg.py"
mode='glite'
workdir = "."
dataset = ''
lumimask = ''
nLumis = -1
nLumisPerJob = 25
sequence = "only_analyze"
nevent = "50000"
global_tag_flag = ''#"GR10_P_V7::All"
global_tag = ''
crabdir = workdir + "/" + release + '/'
ntupleName = 'ntuple_'+ sequence + '.root' 
os.system("mkdir -p "+ crabdir)

##### End of setting variables

#
def makeCMSSWConfig(cmsswSkelFile):
    inFile = open(cmsswSkelFile, 'r').read().split('\n')
    dataname = dataset.split('/')[1]+'_'+dataset.split('/')[2]
    #print dataname
    if os.path.exists(cmsswSkelFile) == False:
        print 'CMSSW skeleton file does not exist. Exiting'
        sys.exit()
        
    inFile = open(cmsswSkelFile, 'r').read().split('\n')

    outFileName = dataname + '_' + sequence + '_cfg.py'
    print 'Writing CMS2 CMSSW python config file : ' + outFileName
    outFile = open(outFileName, 'w')

    for i in inFile:
        if i.find("INPUTFILES") != -1:
            outFile.write('\'\'\n'); continue
        if i.find('GLOBALTAG') != -1:
            if sequence != 'only_analyze':
                outFile.write('process.GlobalTag.globaltag = "' + global_tag + '"\n'); continue
            else:
                outFile.write(''); continue
        if i.find('NEVENT') != -1:
           outFile.write('process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(' + str(nevent) + '))\n'); continue
        if i.find('SEQUENCE') != -1:
            outFile.write('process.p = cms.Path(process.GOODCOLL*process.' + sequence +  ')\n'); continue
        if i.find('LUMIRANGE') != -1:
            outFile.write(''); continue
        if i.find('OUTFILE') != -1:
            outFile.write('process.trackana.OutputFileName = cms.string("' + ntupleName + '")\n'); continue
                
        outFile.write(i+'\n')        

    outFile.close()

#
def makeCrabConfig():
    outFileName = dataset.split('/')[1]+'_'+dataset.split('/')[2] + '_' + sequence
    outFile = open(outFileName + '.cfg', 'w')
    print 'Writing CRAB config file: ' + outFileName + '.cfg'
    outFile.write('[CRAB]\n')
    outFile.write('jobtype   = cmssw\n')
    outFile.write('scheduler = ' + mode + '\n')
    outFile.write('\n[CMSSW]\n')
    outFile.write('datasetpath             = ' + dataset + '\n')
    outFile.write('pset                    = ' + outFileName + '_cfg.py \n')
    outFile.write('lumi_mask               = ' + lumimask + '\n')
    outFile.write('total_number_of_lumis   = ' + str(nLumis) + '\n')
    outFile.write('lumis_per_job           = ' + str(nLumisPerJob) + '\n')
    outFile.write('output_file             = ' + ntupleName + '\n\n\n')
    outFile.write('[USER]\n')
    outFile.write('return_data             = 1\n')
    outFile.write('copy_data               = 0\n')
    outFile.write('ui_working_dir          = ' + crabdir + outFileName + '\n')
    outFile.write('publish_data            = 0\n')
    outFile.write('[GRID]\n')
    outFile.write('maxtarballsize = 20\n')
    outFile.write('##here are some default sites that we \n')
    outFile.write('##run at. Comment/Uncomment at will\n')
    outFile.write('##UCSD \n')
    outFile.write('#SE_white_list = T2_US_UCSD\n')
    outFile.write('##WISC\n')
    outFile.write('#SE_white_list = T2_US_Wisconsin\n')
    outFile.write('##DESY\n')
    outFile.write('#SE_white_list = T2_DE_DESY\n')
    outFile.write('##Purdue\n')
    outFile.write('#SE_white_list = T2_US_Purdue\n')
    outFile.write('##MIT\n')
    outFile.write('#SE_white_list = T2_US_MIT\n')
    outFile.write('##Nebraska\n')
    outFile.write('#SE_white_list = T2_US_Nebraska\n')
    outFile.write('##IFCA\n')
    outFile.write('#SE_white_list = T2_ES_IFCA\n')
    outFile.write('##Lyon\n')
    outFile.write('#SE_white_list = T2_FR_CCIN2P3\n')
    outFile.write('##CIEMAT\n')
    outFile.write('#SE_white_list = T2_ES_CIEMAT\n')
    outFile.write('##IIHE\n')
    outFile.write('#SE_white_list = T2_BE_IIHE\n')
    outFile.write('##Aachen\n')
    outFile.write('#SE_white_list = T2_DE_RWTH\n')
#                                                               
###############################
####### Main Program
###############################

if len(sys.argv) < 4 :
    print 'Usage: makeDataCrabFiles.py [OPTIONS]'
    print '\nWhere the required options are: '
    print '\t-dataset\t\tname of dataset'
    print '\t-l\t\tJason File to specify the run/lumi'
    print '\nOptional arguments:'
    print '\t-nLumis\t\tTotal number of lumis you want to run on. Default is -1'
    print '\t-lumisPerJob\tNumber of lumis per job. Default is 25'
    print '\t-dir\tworkdir. Default is the current directory'
    print '\t-gtag\tglobaltag. If not specfied, will take it directly from the DBS'
    print '\t-m\t\tsubmission mode (possible: condor_g, condor, glite). Default is glite'
    print '\t-s\tsequence. Default is only_analyze'
        
    sys.exit()
    
for i in range(0, len(sys.argv)):
    if sys.argv[i] == '-dataset':
        dataset = sys.argv[i+1]
    if sys.argv[i] == '-l':
        lumimask = str(sys.argv[i+1])        
    if sys.argv[i] == '-cfg':
        cmsswSkelFile = sys.argv[i+1]        
    if sys.argv[i] == '-nLumis':
        nLumis = sys.argv[i+1]
    if sys.argv[i] == '-lumisPerJob':
        nLumisPerJob = sys.argv[i+1]        
    if sys.argv[i] == '-dir':
        workdir = sys.argv[i+1]
    if sys.argv[i] == '-gtag':
        global_tag_flag = sys.argv[i+1]
    if sys.argv[i] == '-m':
        mode = sys.argv[i+1]
    if sys.argv[i] == '-s':
        sequence = sys.argv[i+1]        


if( global_tag_flag != '' ):
    print '\nUsing \'' + global_tag_flag + '\' specified by -gtag flag.\n'
    global_tag = global_tag_flag
else:       
    if sequence != 'only_analyze':
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
            
makeCMSSWConfig(cmsswSkelFile)
makeCrabConfig() 
