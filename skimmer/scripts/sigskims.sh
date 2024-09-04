
# %
# !! Current output SKIMDIR required running from UAF-2 !!
sig_job_dir="v2"
SAMPLES="
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
/ceph/cms/store/user/mmazza/SignalGeneration/${sig_job_dir}_merged/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_job_dir}/merged.root
"
ANALYSIS_TAG=AllHadRun2
SKIMTAG=nanoaodv9_sig${sig_job_dir}_${ANALYSIS_TAG}_3Sep2024
SKIMDIR=/ceph/cms/store/user/mmazza/skim/${SKIMTAG}/ 
mkdir -p $SKIMDIR

SKIMMER=skim
#echo "Begin " > $outLogFile
outLogFile=${SKIMDIR}/skimmer_out.log
if [[ "$SAMPLES" != "" && "$SKIMDIR" != "" && "$SKIMTAG" != "" ]]; then
    for sample in $SAMPLES; do
        echo "Skimming $sample" >> $outLogFile
        ./$SKIMMER -t Events -d . -a $ANALYSIS_TAG -n output -T Events $sample >> $outLogFile
        sampledir=$(dirname $sample)
        targetdir=$SKIMDIR/${sampledir##*/}
        mkdir -p $targetdir
        mv output.root $targetdir/merged.root
        mv cutflow.txt $targetdir/.
        echo "Wrote skim to $targetdir/merged.root" >> $outLogFile
        echo "ls $$targetdir" >> $outLogFile
        ls $targetdir >> $outLogFile
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
