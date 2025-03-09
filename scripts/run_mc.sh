#!/bin/bash -x

PROJECT_DIR="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" \
	--nickname sh_qqps_k94_2l_c2 \
	--mc \
	--max-events 10 \
	--config "$PROJECT_DIR/delphi-nanoaod/delphi-nanoaod.yaml" 
	--output sh_qqps_k94_2l_c2.root 
