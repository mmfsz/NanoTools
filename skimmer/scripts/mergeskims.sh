
# Requires NanoTools/skimmer/setpu.sh

SIGJOBDIR="v2"

BASEDIR_NANOTOOLS="/home/users/mmazza/projects/VVHjj/NanoTools/"
OUTPUT_BASEDIR="/ceph/cms/store/user/mmazza/SignalGeneration/${SIGJOBDIR}/"
#VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${SIGJOBDIR}/
# VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${SIGJOBDIR}/
# VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${SIGJOBDIR}/
# VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${SIGJOBDIR}/
# "
SAMPLEDIRS="
VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${SIGJOBDIR}/
VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${SIGJOBDIR}/
"

# go to output dir of MC production in ceph/
cd $OUTPUT_BASEDIR

if [[ "$SAMPLEDIRS" != "" ]]; then
    for sampledir in $SAMPLEDIRS; do       
        echo "Merging files in $sampledir"
        python3 ${BASEDIR_NANOTOOLS}/scripts/haddnano.py $sampledir/merged.root $sampledir"/output*.root"
        echo "Wrote merged file to $sampledir/merged.root"
    done
fi

cd -


