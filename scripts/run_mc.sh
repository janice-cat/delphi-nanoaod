#!/bin/bash -x

PROJECT_DIR="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

OUTPUT_DIR="/afs/cern.ch/work/${USER:0:1}/${USER}/delphi-nanoaod"
mkdir -p "$OUTPUT_DIR"

WORK_DIR=$(mktemp -d)
cd $WORK_DIR

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" \
	--nickname sh_qqps_k94_2l_c2/C10 \
	--max-events 10 \
	--mc \
	--config "$PROJECT_DIR/delphi-nanoaod/delphi-nanoaod.yaml" \
	--output "$OUTPUT_DIR/sh_qqps_k94_2l_c2.root" 
