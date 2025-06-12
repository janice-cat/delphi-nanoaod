import os
import shutil
import glob
import sys
import subprocess
import multiprocessing



sim99 = "xs_(wphact21nc4f|wphact24cc|kk2f4143qq_|kk2f4144tthl|qedbk23eegg).*(e1|e2).*99.*e1"
data99 = "xsdst99_e1"

sim98 = r'xs_[^h].*e18.*98.*e1'
data98 = 'xsdst98_e1'

env = os.environ.copy()
result = subprocess.run(['fatfind', '-N', sim98],
                        stdout=subprocess.PIPE, env=env, text=True)
fatmen = result.stdout.splitlines()
fatmen += [data98]

nevt = 0

def infer_year(yeartype):
    """Infers the year from the yeartype string."""
    if '9' in yeartype:
        return '19' + yeartype[yeartype.find('9'):yeartype.find('9') + 2]
    return None

def run(nn):
    out_top = "simulation" if nn[2] == '_' else 'collision'
    year = infer_year(nn)
    top = f'/data00/DELPHI/{out_top}_data/{year}/{nn}'
    os.makedirs(top, exist_ok=True)
    output = f"{top}_evt{nevt}.root" if nevt > 0 else f"{top}.root"
    execution = f"../{exe} --nickname {nn} --mc --config ../config/delphi-nanoaod.yaml --output {output}"
    if nevt > 0:
        execution += f" -m {nevt}"
    print(execution)
    os.makedirs(nn, exist_ok=True)
    os.chdir(nn)
    os.system(execution + f" |& tee {nn}.log")
    os.chdir("..")
    os.system(f"""root -q -b -l scripts/treefy.C+'("{output}")'""")

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
    with multiprocessing.Pool(processes=15) as pool:  # adjust the number of processes as needed
        pool.map(run, fatmen)
