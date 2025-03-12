#!/bin/bash -x

# parse arguments
NICKNAME = "$1"
if [ "$2" = "default" ]; then
    CONFIG = "delphi-nanoaod"
    OUTPUT = "${NICKNAME}.root"
else
    CONFIG = "delphi-nanoaod-${CONFIG}"
    OUTPUT = "${NICKNAME}-${CONFIG}.root"
fi
shift 2
EXTRA_ARGS = "$@"

# Set up the environment
source /cvmfs/delphi.cms.cern.ch/setuo.sh
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.34.04/x86_64-almalinux9.5-gcc115-opt/bin/thisroot.sh

# Run the job
PROJECT_DIR=$(dirname $(dirname $(readlink -f $BASH_SOURCE[0])))

CONFIG_FILE=${PROJECT_DIR}/config/${CONFIG}.yaml

$PROJECT_DIR/build/delphi-nanoaod -c $CONFIG_FILE -o $OUTPUT $EXTRA_ARGS
