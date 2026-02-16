
#!/bin/bash

# Requires NanoTools/skimmer/setup.sh

SIGJOBDIR="VBSVVH_VBSCuts_13TeV_4f_LO_MG_2_9_18_c2v_1p5_c3_1p0_c2Vc3scan_slc7_amd64_gcc10_CMSSW_12_4_8"
OUTPUT_BASEDIR="/ceph/cms/store/user/mmazza/SignalGeneration/${SIGJOBDIR}"

# Check if directory exists
if [ ! -d "$OUTPUT_BASEDIR" ]; then
    echo "Error: Directory '$OUTPUT_BASEDIR' does not exist."
    exit 1
fi

# Go to output dir
cd "$OUTPUT_BASEDIR" || exit 1

BASEDIR_NANOTOOLS="/home/users/mmazza/projects/VVHjj/NanoTools/"
# Find and loop over subdirectories containing "NANOGEN" in the name
for sampledir in */; do
    # Check if it's a directory and name contains "NANOGEN"
    if [[ -d "$sampledir" && "$sampledir" == *"NANOGEN"* ]]; then
        # Remove trailing slash for cleaner output
        dirname="${sampledir%/}"
        echo "Merging files in $dirname"
        python3 "${BASEDIR_NANOTOOLS}/scripts/haddnano.py" "$dirname/merged.root" "$dirname/output*.root"
        echo "Wrote merged file to $dirname/merged.root"
    fi
done

cd - > /dev/null


