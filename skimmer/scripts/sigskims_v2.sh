#
# Run skimmer locally over unmerged signal skims (i.e. does not assume the skims have already been merged into one ROOT file per sample).
#

c2v="1p5"
c3="1p0"
sig_tag="VBSVVH_VBSCuts_13TeV_4f_LO_MG_2_9_18_c2v_${c2v}_c3_${c3}_c2Vc3scan_slc7_amd64_gcc10_CMSSW_12_4_8" #"v2"     #                 #"-run2-c2v-jguiang"
sig_job_dir=${sig_tag} #${sig_tag}"_merged"  #"run2_c2v_jguiang"
sig_production_tag="" #"_v2"
nanoaod_affix="NANOGEN" #"-106X_privateMC_NANOGEN"
nanoaod_suffix="VBSCuts_13TeV" #"VBSCuts"
new_suffix="C2V_${c2v}_C3_${c3}_13TeV_4f_LO" # starting of subdir name to match what is expected by xsec.json in RDF 
signal_samples_dir="/ceph/cms/store/user/mmazza/SignalGeneration/" #"/ceph/cms/store/user/jguiang/VBSVVHSignalGeneration/v2/" #

SAMPLES_DIRS="
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSZZH_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_SS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL16APV_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL17_${nanoaod_affix}${sig_production_tag}
${signal_samples_dir}/${sig_job_dir}/VBSWWH_OS_${nanoaod_suffix}_TuneCP5_RunIISummer20UL18_${nanoaod_affix}${sig_production_tag}
"
ANALYSIS_TAG=None

SKIMTAG=nanoaodv9_sig_cuts-${ANALYSIS_TAG}_${sig_tag}_21Nov2025_unmerged
SKIMDIR=/ceph/cms/store/user/mmazza/skim/${SKIMTAG}/ 
mkdir -p $SKIMDIR

SKIMMER=skim
#echo "Begin " > $outLogFile
outLogFile=${SKIMDIR}/skimmer_out.log
if [[ "$SAMPLES_DIRS" != "" && "$SKIMDIR" != "" && "$SKIMTAG" != "" ]]; then
    for sampledir in $SAMPLES_DIRS; do
        shopt -s nullglob
        files=("$sampledir"/output_*.root)
        shopt -u nullglob
        for sample in "${files[@]}"; do
            echo "Skimming $sample" >> $outLogFile
            filename=$(basename "$sample")
            filename_tmp=${new_suffix}_${filename%.root}
            ./$SKIMMER -t Events -d . -a $ANALYSIS_TAG -n $filename_tmp -T Events $sample --dump_truth --is_signal>> $outLogFile
            original_subdir=${sampledir##*/}
            new_subdir=${original_subdir/${nanoaod_suffix}/${new_suffix}}
            targetdir=$SKIMDIR/$new_subdir
            echo "original_subdir = $original_subdir"
            echo "new_subdir = $new_subdir"
            echo "targetdir = $targetdir"
            mkdir -p $targetdir
            echo "filename = $filename"
            mv ${filename_tmp}.root $targetdir/$filename
            mv cutflow.txt $targetdir/.
            # Move and rename the two specific cutflow files (if they exist)
            if [[ -f "${filename_tmp}_Cutflow.cflow" ]]; then
                mv "${filename_tmp}_Cutflow.cflow" "${targetdir}/${filename_tmp}_Cutflow.cflow"
                echo "Moved ${filename_tmp}_Cutflow.cflow" >> "$outLogFile"
            fi

            if [[ -f "${filename_tmp}_Cutflow_TheEnd.csv" ]]; then
                mv "${filename_tmp}_Cutflow_TheEnd.csv" "${targetdir}/${filename_tmp}_Cutflow_TheEnd.csv"
                echo "Moved ${filename_tmp}_Cutflow_TheEnd.csv" >> "$outLogFile"
            fi

            echo "Wrote skim to $targetdir/$filename" >> $outLogFile
            echo "Wrote skim to $targetdir/$filename"
            echo "ls $targetdir" >> $outLogFile
            ls $targetdir >> $outLogFile
        done
    done
fi
