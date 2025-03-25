#!/bin/bash -x

# parse arguments
NICKNAME="$1"
if [ "$2" = "default" ]; then
    CONFIG="delphi-nanoaod"
else
    CONFIG="delphi-nanoaod-$2"
fi
OUTPUT="${NICKNAME}-$2.root"
shift 2

# Set up the environment
set +x
source /cvmfs/delphi.cms.cern.ch/setup.sh
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.34.04/x86_64-almalinux9.5-gcc115-opt/bin/thisroot.sh
set -x

PROJECT_DIR="$(dirname "$(dirname "$SUBMIT_FILE")")"
CONFIG_FILE="${PROJECT_DIR}/config/${CONFIG}.yaml"

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" -N "$NICKNAME" -C "$CONFIG_FILE" -O "$OUTPUT" "$@"

ls -l
