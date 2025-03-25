#!/bin/bash -x

PROJECT_DIR="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

OUTPUT_DIR="/afs/cern.ch/work/${USER:0:1}/${USER}/delphi-nanoaod"
mkdir -p "$OUTPUT_DIR"

WORK_DIR=$(mktemp -d)
cd $WORK_DIR

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" \
	--nickname short94_c2 \
	--max-events 1000 \
	--config "$PROJECT_DIR/config/delphi-nanoaod.yaml" \
	--output "$OUTPUT_DIR/short94_c2.root" 
