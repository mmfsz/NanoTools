### Environment & setup
```bash
git clone https://github.com/cmstas/NanoTools
cd NanoTools/
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_13/ ; cmsenv ; cd -
cd NanoCORE
make test -j12
cd ..
```

### Unit tests

Unit tests of `NanoCORE/` are inside `NanoCORE/Tools/unit_tests/` and can be run with
`make test` inside of `NanoCORE/`.

### Skimmer

One must long on to ```uaf-10``` for this.
The newer linux version won't work.
For future we will have to one day fix this.

```bash
ssh uaf-10.t2.ucsd.edu
mkdir -p workarea/skimtest
cd workarea/skimtest
git clone https://github.com/cmstas/NanoTools
cd NanoTools/
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_13/ ; cmsenv ; cd -
cd NanoCORE
make test -j12
cd ..
cd skimmer/
make -j;
./sklimmer -h # To print help information
./sklimmer /ceph/cms/store/user/phchang/nanoclass/mc/RunIISummer20UL18NanoAODv9/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v1/120000/87DEE912-70CF-A549-B10B-1A229B256E88.root
```

### Style

We use `clang-format` based on LLVM style to format our code. To format the `ElectronSelections.cc` file in-place, do
```bash
clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4, ColumnLimit: 120, AllowShortIfStatementsOnASingleLine: true, AllowShortBlocksOnASingleLine: true}" -i ElectronSelections.cc
```

Add this to the ```~/.vimrc``` and use ```vim``` to code. (Assumes you are working on UAF)
```
autocmd BufNewFile,BufRead *.cc,*.h,*.C,*.cxx set formatprg=clang-format\ -style=\"{BasedOnStyle:\ llvm,\ IndentWidth:\ 4,\ ColumnLimit:\ 100,\ AllowShortIfStatementsOnASingleLine:\ true,\ AllowShortBlocksOnASingleLine:\ false,\ BreakBeforeBraces:\ Allman}\"
```

To format your code, press ```ggvGgq```.

