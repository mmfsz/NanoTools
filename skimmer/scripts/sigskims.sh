
# %
sig_tag="-run2-c2v-jguiang"     #"v2"                 #"run2-c2v-jguiang"
sig_job_dir="run2_c2v_jguiang"  #${sig_tag}"_merged"  #"run2_c2v_jguiang"
sig_production_tag="v2"
signal_samples_dir="/ceph/cms/store/user/mmazza/SignalGeneration/" #"/ceph/cms/store/user/jguiang/VBSVVHSignalGeneration/v2/" #
SAMPLES="
${signal_samples_dir}/${sig_job_dir}/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL16-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL16APV-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL17-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_VBSCuts_TuneCP5_RunIISummer20UL18-106X_privateMC_NANOGEN_${sig_production_tag}/merged.root
"
ANALYSIS_TAG=LepVeto
SKIMTAG=nanoaodv9_sig${sig_tag}_${ANALYSIS_TAG}_5Feb2025
SKIMDIR=/ceph/cms/store/user/mmazza/skim/${SKIMTAG}/ 
mkdir -p $SKIMDIR

SKIMMER=skim
#echo "Begin " > $outLogFile
outLogFile=${SKIMDIR}/skimmer_out.log
if [[ "$SAMPLES" != "" && "$SKIMDIR" != "" && "$SKIMTAG" != "" ]]; then
    for sample in $SAMPLES; do
        echo "Skimming $sample" >> $outLogFile
        ./$SKIMMER -t Events -d . -a $ANALYSIS_TAG -n output -T Events $sample --dump_truth --is_signal>> $outLogFile
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
