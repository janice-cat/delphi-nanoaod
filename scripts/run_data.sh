#!/bin/bash -x

PROJECT_DIR="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" \
	--nickname short94_c2 \
	--max-events 10 \
	--config "$PROJECT_DIR/delphi-nanoaod/delphi-nanoaod.yaml" \
	--output short94_c2.root 
