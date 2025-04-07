#!/bin/bash

here=$(dirname "$(readlink -f "$0")")

$here/../python/plotter.py \
    --data /eos/project/d/delphi/public/nanoaod/short95_d2.root \
    --mc /eos/project/d/delphi/public/nanoaod/sh_qqps_r95_1l_d2.root \
    --mc /eos/project/d/delphi/public/nanoaod/sh_qqps_b95_1l_d2.root \
    --mc /eos/project/d/delphi/public/nanoaod/sh_qqps_sa95_1l_d2.root \
    --normalise t4hadrons \
    --output output/plots95.root \
    "$@"
