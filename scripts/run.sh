#!/bin/bash -x

PROJECT_DIR="$(dirname "$(dirname "$(realpath "${BASH_SOURCE[0]}")")")"

"$PROJECT_DIR/build/delphi-nanoaod/delphi-nanoaod" "$@"


