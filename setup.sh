source /cvmfs/cms.cern.ch/cmsset_default.sh

export ANALYSISPATH=$PWD

echo "NOTE: If running in a screen session, it should be the first time you run the setup since you logged in via ssh."

REL_VERSION=$(cat /etc/os-release | grep -E '^VERSION_ID=' | cut -d'"' -f2 | grep -oE '[0-9]+' | head -n1)
echo "REL_VERSION = $REL_VERSION"

# For now it only works with 7 even if the architecture is wrong 
# RHELVER=7

if [[ $REL_VERSION == 7 ]]; then
    cd /cvmfs/cms.cern.ch/slc7_amd64_gcc12/cms/cmssw/CMSSW_14_0_5; cmsenv; cd -
elif [[ $REL_VERSION == 8 ]]; then
    cd /cvmfs/cms.cern.ch/el8_amd64_gcc12/cms/cmssw/CMSSW_14_0_5; cmsenv; cd - 
fi

# Export some useful CMSSW paths
CMSSW_EXT=$CMSSW_BASE/../../../external
export CORRECTIONLIBDIR=$(ls -d ${CMSSW_EXT}/py3-correctionlib/* | sort -V | tail -n 1)/lib/python3.9/site-packages/correctionlib
export BOOSTDIR=$(ls -d ${CMSSW_EXT}/boost/* | sort -V | tail -n 1)
echo $BOOSTDIR
echo $CORRECTIONLIBDIR

# Modify PATH
LIBPATHS="
$CMSSW_BASE/lib/$SCRAM_ARCH
"
for LIBPATH in $LIBPATHS; do
    # Add library to LD_LIBRARY_PATH
    if [[ "$LD_LIBRARY_PATH" != *"$LIBPATH"* ]]; then
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBPATH
    fi
    # Add library to ROOT_INCLUDE_PATH
    if [[ "$ROOT_INCLUDE_PATH" != *"$LIBPATH"* ]]; then
        export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$LIBPATH
    fi
done

# Add current working directory to Python PATH
export PYTHONPATH=$PYTHONPATH:$PWD
