import os
import shutil
import glob
import sys
import subprocess
import multiprocessing

fatmen = [
    "xs_wphact24cc_e182.7_m80.4_c97_1l_g1",
    "xs_wphact211ncgg_e182.7_m80.4_c97_1l_g1",
    "xs_wphact21nc4f_e182.7_m80.4_c97_1l_g1",
    "xs_qedbk23eegg_e183.5_l97_1l_g1",
    "xs_gpym6143wc0eeqq_e182.7_c97_1l_g1",
    "xs_qedbk23eegg_e183.5_l97_1l_g1",
    "xs_kk2f4143qq_e182.7_l97_1l_g1",
    "xs_kk2f4144tthl_e182.7_r97_1l_g1",
    "xs_kk2f4143mumu_e182.7_l97_1l_g1",
]


nevt = 0

def infer_year(yeartype):
    """Infers the year from the yeartype string."""
    if '9' in yeartype:
        return '19' + yeartype[yeartype.find('9'):yeartype.find('9') + 2]
    return None

def run(nn):
    # out_top = "simulation" if suffix == "MC" else "collision"
    year = infer_year(nn)
    out_top = "simulation"
    top = f'/data/DELPHI/{out_top}_data/{year}/{nn}'
    os.makedirs(top, exist_ok=True)
    output = f"{top}_evt{nevt}.root" if nevt > 0 else f"{top}.root"
    execution = f"{exe} --nickname {nn} --mc --config config/delphi-nanoaod.yaml --output {output}"
    if nevt > 0:
        execution += f" -m {nevt}"
    print(execution)
    os.system(execution + f" |& tee {top}.log")
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
    with multiprocessing.Pool(processes=20) as pool:  # adjust the number of processes as needed
        pool.map(run, fatmen)
