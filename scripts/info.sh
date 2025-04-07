#!/bin/bash

here=$(dirname "$(realpath "${BASH_SOURCE[0]}")")
project_dir=$(dirname "$here")

"$project_dir/python/info.py"  /eos/project/d/delphi/public/nanoaod/*.root
