# %
sig_tag="VBSVVH_VBSCuts_13TeV_4f_LO_MG_2_9_18_c2v_1p5_c3_1p0_c2Vc3scan_slc7_amd64_gcc10_CMSSW_12_4_8" #"v2"     #                 #"-run2-c2v-jguiang"
sig_job_dir=${sig_tag} #${sig_tag}"_merged"  #"run2_c2v_jguiang"
sig_production_tag="" #"_v2"
nanoaod_affix="NANOGEN" #"-106X_privateMC_NANOGEN"
nanoaod_suffix="VBSCuts_13TeV" #"VBSCuts"
new_suffix="C2V_1p0_C3_1p0_13TeV_4f_LO" # starting of subdir name to match what is expected by xsec.json in RDF 
signal_samples_dir="/ceph/cms/store/user/mmazza/SignalGeneration/" #"/ceph/cms/store/user/jguiang/VBSVVHSignalGeneration/v2/" #

SAMPLES="
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
"
ANALYSIS_TAG=None

SKIMTAG=nanoaodv9_sig_cuts-${ANALYSIS_TAG}_${sig_tag}_11Nov2025_v2
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
        original_subdir=${sampledir##*/}
        new_subdir=${original_subdir/${nanoaod_suffix}/${new_suffix}}
        targetdir=$SKIMDIR/$new_subdir
        mkdir -p $targetdir
        mv output.root $targetdir/merged.root
        mv cutflow.txt $targetdir/.
        echo "Wrote skim to $targetdir/merged.root" >> $outLogFile
        echo "ls $targetdir" >> $outLogFile
        ls $targetdir >> $outLogFile
    done
fi


# # %
# sig_tag="VBSVVH_VBSCuts_13TeV_4f_LO_MG_2_9_18_c2v_1p5_c3_1p0_c2Vc3scan_slc7_amd64_gcc10_CMSSW_12_4_8" #"v2"     #                 #"-run2-c2v-jguiang"
# sig_job_dir=${sig_tag} #${sig_tag}"_merged"  #"run2_c2v_jguiang"
# sig_production_tag="" #"_v2"
# nanoaod_affix="NANOGEN" #"-106X_privateMC_NANOGEN"
# nanoaod_suffix="VBSCuts_13TeV_TuneCP5" #"VBSCuts_TuneCP5"
# signal_samples_dir="/ceph/cms/store/user/mmazza/SignalGeneration/" #"/ceph/cms/store/user/jguiang/VBSVVHSignalGeneration/v2/" #
# SAMPLES="
# ${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_RunIISummer20UL16APV${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}/merged.root
# ${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}/merged.root
# "
# ANALYSIS_TAG=None
# SKIMTAG=nanoaodv9_sig_cuts-${ANALYSIS_TAG}_${sig_tag}_11Nov2025
# SKIMDIR=/ceph/cms/store/user/mmazza/skim/${SKIMTAG}/ 
# mkdir -p $SKIMDIR

# SKIMMER=skim
# #echo "Begin " > $outLogFile
# outLogFile=${SKIMDIR}/skimmer_out.log
# if [[ "$SAMPLES" != "" && "$SKIMDIR" != "" && "$SKIMTAG" != "" ]]; then
#     for sample in $SAMPLES; do
#         echo "Skimming $sample" >> $outLogFile
#         ./$SKIMMER -t Events -d . -a $ANALYSIS_TAG -n output -T Events $sample --dump_truth --is_signal>> $outLogFile
#         sampledir=$(dirname $sample)
#         targetdir=$SKIMDIR/${sampledir##*/}
#         mkdir -p $targetdir
#         mv output.root $targetdir/merged.root
#         mv cutflow.txt $targetdir/.
#         echo "Wrote skim to $targetdir/merged.root" >> $outLogFile
#         echo "ls $$targetdir" >> $outLogFile
#         ls $targetdir >> $outLogFile
#     done
# fi


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
