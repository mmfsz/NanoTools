
# %
# !! Current output SKIMDIR required running from UAF-2 !!
SAMPLES="
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_fullTest3/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/fullTest3/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_fullTest3/merged.root
"
SKIMDIR=/data/userdata/mmazza/vvhjj/skims/allhad/
SKIMTAG=v3
SKIMMER=skim

if [[ "$SAMPLES" != "" && "$SKIMDIR" != "" && "$SKIMTAG" != "" ]]; then
    for sample in $SAMPLES; do
        echo "Skimming $sample"
        ./$SKIMMER -t Events -d . -n output -T Events $sample
        sampledir=$(dirname $sample)
        targetdir=$SKIMDIR/$SKIMTAG/${sampledir##*/}
        mkdir -p $targetdir
        mv output.root $targetdir/merged.root
        echo "Wrote skim to $targetdir/merged.root"
    done
fi


# SKIMDIR=/data/userdata/jguiang/nanoaod/forMaria/new

# if [[ "$SAMPLES" != "" && "$SKIMDIR" != "" && "$SKIMTAG" != "" ]]; then
#     make study=add_tth_mva clean; make study=add_tth_mva
#     make study==$SKIMMER clean; make study=$SKIMMER
#     for sample in $SAMPLES; do
#         sampledir=$(dirname $sample)
#         name=${sampledir##*/}
#         targetdir=$SKIMDIR/$SKIMTAG/${name}_$SKIMTAG
#         echo "$sample"
#         echo "Adding ttH UL MVA branch"
#         ./bin/add_tth_mva -t Events -d . -n ${name}_tthfix -T Events $sample
#         echo "Skimming"
#         ./bin/$SKIMMER -t Events -d . -n output -T Events ${name}_tthfix.root &> logs.txt
#         mkdir -p $targetdir
#         echo "Wrote skim to $targetdir/merged.root"
#         mv output.root $targetdir/merged.root
#         mv logs.txt $targetdir/logs.txt
#         rm ${name}_tthfix.root
#     done
# fi
