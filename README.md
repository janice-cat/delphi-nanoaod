# DELPHI NanoAOD


# Installation

Setup on lxplus EL9 or similar machine

DELPHI software
```bash
source /cvmfs/delphi.cern.ch/setup.sh
```

ROOT
```bash
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.34.04/x86_64-almalinux9.5-gcc115-opt/bin/thisroot.sh
```

Checkout the repository and compile

```bash
git clone https://gitlab.cern.ch/delphi/delphi-nanoaod.git
cd delphi-nanoaod

# Configure make files
cmake -B build

# Build the software
make -C build
```