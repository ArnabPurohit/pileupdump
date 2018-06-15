if __name__ == '__main__':

# Usage : python crab.py (to create jobs)
#         ./multicrab -c status -d <work area> (to check job status)

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    from CRABClient.UserUtilities import config
    config = config()
    
    from multiprocessing import Process

    # Common configuration

    config.General.workArea     = 'crab_project'
    config.General.transferLogs = False
    config.JobType.pluginName   = 'Analysis' # PrivateMC
    config.JobType.psetName     = 'pileupdumper.py'
    #config.JobType.sendExternalFolder = True
    config.Data.inputDBS        = 'global'    
    config.Data.splitting       = 'FileBased'#for MC #'LumiBased'For data EventBased, FileBased, LumiBased (1 lumi ~= 300 events)
    config.Data.totalUnits      = -1
    #config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt'
    config.Data.publication     = False
    #config.Site.storageSite     = 'T2_CH_CERN'
    #config.Site.storageSite     = 'T2_US_UCSD'
    config.Site.storageSite     = 'T3_US_FNALLPC'

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers

    # dataset dependent configuration
    config.General.requestName = 'job_March_MC_VHToGG_M70'
    config.Data.unitsPerJob    = 2
    #config.Data.inputDataset   = '/GluGluHToGG_M70_13TeV_amcatnloFXFX_pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM' #dataset name
    #config.Data.inputDataset   = '/VBFHToGG_M70_13TeV_amcatnlo_pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM' #dataset name
    config.Data.inputDataset   = '/VHToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v11-v1/MINIAODSIM' #dataset name
    #config.Data.outLFNDirBase  = '/store/user/arpurohi/'
    p = Process(target=submit, args=(config,))
    p.start()
    #p.join()
