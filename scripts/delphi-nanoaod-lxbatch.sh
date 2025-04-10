#!/bin/bash -x

# parse arguments
NICKNAME="$1"
if [ "$2" = "default" ]; then
    CONFIG="delphi-nanoaod"
    OUTPUT="$NICKNAME.root"
else
    CONFIG="delphi-nanoaod-$2"
    OUTPUT="$NICKNAME-$2.root"
fi
shift 2

# Set up the environment
set +x
if [ -z "$DELPHI" ]; then
    source /cvmfs/delphi.cern.ch/setup.sh
fi
if [ -z "$ROOTSYS" ]; then
    source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.34.04/x86_64-almalinux9.5-gcc115-opt/bin/thisroot.sh
fi
set -x

CONFIG_FILE="$(realpath "$SUBMIT_DIR/../config/$CONFIG.yaml")"
BINARY_FILE="$(realpath "$SUBMIT_DIR/../build/delphi-nanoaod/delphi-nanoaod")"

if [ "$1" = "." ]; then
    "$BINARY_FILE" -N "$NICKNAME" -C "$CONFIG_FILE" -O "$OUTPUT"
else
    "$BINARY_FILE" -N "$NICKNAME" -C "$CONFIG_FILE" -O "$OUTPUT" "$@"
fi

xrdcp --force --retry 3 "$OUTPUT" "root://eosproject.cern.ch//eos/project/d/delphi/public/nanoaod/v0.1.0/$OUTPUT"
