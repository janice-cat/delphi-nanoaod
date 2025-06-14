import os
import shutil
import glob
import sys
import subprocess
import multiprocessing
from functools import partial


sima0 = "xs_(wphact21nc4f|wphact24cc|wphact20ccfixed|kk2f4143qq_|kk2f4144tthl|qedbk23eegg).*(e2).*a0.*(e1|u1)"
dataa0 = ["xsdst00_u1", "xsdst00_e1"]

sim99 = "xs_(wphact21nc4f|wphact24cc|kk2f4143qq_|kk2f4144tthl|qedbk23eegg).*(e1|e2).*99.*e1"
data99 = "xsdst99_e1"

sim98 = r'xs_[^h].*e18.*98.*e1'
data98 = 'xsdst98_e1'

data97 = ['xsdst97_e183_g1', 'xsdst97_e183_g2']

data96 = "xsdst96_.*g.*"
sim96 = "xs_.*e1.*c96.*g.*"

env = os.environ.copy()
result = subprocess.run(['fatfind', '-N', sim96],
                        stdout=subprocess.PIPE, env=env, text=True)

fatmen = result.stdout.splitlines()
# fatmen_data = dataa0 + [data99, data98] + data97

# result = subprocess.run(['fatfind', '-N', data96],
#                         stdout=subprocess.PIPE, env=env, text=True)
# fatmen_data = result.stdout.splitlines()
# print(fatmen_data)
fatmen_data = ['xsdst98_d2']

nevt = 0

def infer_year(yeartype):
    """Infers the year from the yeartype string."""
    for nine_str in ['r9', 'c9']:
        if nine_str in yeartype:
            return '19' + yeartype[yeartype.find(nine_str) + 1:yeartype.find(nine_str) + 3]
    elif 'st9' in yeartype:
        return '19' + yeartype[yeartype.find('st9') + 2:yeartype.find('st9') + 4]
    elif 'a0' in yeartype or '00' in yeartype:
        return '2000'
    return None

def run(nn, data=False, dryrun=False):
    out_top = "simulation" if nn[2] == '_' else 'collision'
    year = infer_year(nn)
    top = f'/data00/DELPHI/{out_top}_data/{year}/{nn}'
    os.makedirs(top, exist_ok=True)
    output = f"{top}_evt{nevt}.root" if nevt > 0 else f"{top}.root"
    execution = f"../{exe} --nickname {nn} --config ../config/delphi-nanoaod.yaml --output {output}"
    if nevt > 0:
        execution += f" -m {nevt}"
    if not data:
        execution += ' --mc'
    print(execution)
    # os.makedirs(nn, exist_ok=True)
    if not dryrun:
        os.chdir(nn)
        os.system(execution + f" |& tee {nn}.log")
        os.chdir("..")
        os.system(f"""root -q -b -l scripts/treefy.C+'("{output}")'""")
    else:
        print(execution)

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "debug":
        exe = "gdb --args debug/delphi-nanoaod/delphi-nanoaod"
        build_dir = "debug"
    else:
        exe = "build/delphi-nanoaod/delphi-nanoaod"
        build_dir = "build"
    os.system("source setup.sh")
    os.system(f"cmake -B {build_dir}")
    os.system(f"cmake --build {build_dir}")
    with multiprocessing.Pool(processes=20) as pool:  # adjust the number of processes as needed
        pool.map(partial(run, data=True, dryrun=False), fatmen_data)
        pool.map(partial(run, data=False, dryrun=False), fatmen)
