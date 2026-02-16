#!/bin/bash

[ ! -z "$CMSSW_BASE" ] || {
    [ -e /cvmfs/ ] && {
        source /cvmfs/cms.cern.ch/cmsset_default.sh
        cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_9/ && eval `scramv1 runtime -sh` && cd -
        # source /cvmfs/cms.cern.ch/crab3/crab.sh
    }
}

export METIS_BASE="$( cd "$(dirname "$BASH_SOURCE")" ; pwd -P )"

# CRAB screws up our PYTHONPATH. Go figure.
export PYTHONPATH=${METIS_BASE}:$PYTHONPATH

# Add some scripts to the path
export PATH=${METIS_BASE}/scripts:$PATH

export USEDASGOCLIENT=x # or USEDASGOCLIENT= to fall back to dis

# export GRIDUSER=$(voms-proxy-info -identity -dont-verify-ac | cut -d '/' -f6 | cut -d '=' -f2)
export GRIDUSER=$(voms-proxy-info -identity | cut -d '/' -f6 | cut -d '=' -f2)
