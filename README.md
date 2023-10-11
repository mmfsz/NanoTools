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
cd skimmer/
make -j;
./sklimmer /data/userdata/phchang/nanoaod/mc/Run3Summer22EENanoAODv11/TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/NANOAODSIM/126X_mcRun3_2022_realistic_postEE_v1-v1/30000/3f60166c-edbe-4a47-b455-ebad7407ca44.root
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

