#!/bin/bash -x

# parse arguments
NICKNAME="$1"
if [ "$2" = "default" ]; then
    CONFIG="delphi-nanoaod"
    OUTPUT="${NICKNAME}.root"
else
    CONFIG="delphi-nanoaod-${CONFIG}"
    OUTPUT="${NICKNAME}-${CONFIG}.root"
fi
shift 2

# Set up the environment
set +x
source /cvmfs/delphi.cms.cern.ch/setup.sh
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.34.04/x86_64-almalinux9.5-gcc115-opt/bin/thisroot.sh
set -x

PROJECT_DIR="$(dirname "$(dirname "$SUBMIT_FILE")")"
CONFIG_FILE="${PROJECT_DIR}/config/${CONFIG}.yaml"

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" -c "$CONFIG_FILE" -o "$OUTPUT" "$@"
