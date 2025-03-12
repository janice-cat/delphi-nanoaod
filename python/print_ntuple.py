#!/usr/bin/env python3 
#
#   python/print_ntuple.py --input /path/to/file.root
#
# This script prints the description of the dataframe using the reader interface.
#
# The python interface to RNTuple is still experimental and subject to change.
#
# Dietrich Liko, 2025

import argparse

import ROOT

def main()->None:

    parser = argparse.ArgumentParser(description='Print Ntuple Structure for a given file')
    parser.add_argument('--input', help='Input file', required=True)
    args = parser.parse_args()
    
    with ROOT.TFile.Open(args.input) as f:
        reader = ROOT.Experimental.RNTupleReader.Open(f["Events"])
        desc = reader.GetDescriptor()
        for i in range(desc.GetNFields()-1):
            desc1 = desc.GetFieldDescriptor(i)
            #TODO: improve the printout
            print(f"{desc1.GetFieldName():<40} |  {simplify(desc1.GetTypeName()):<40} | {desc1.GetFieldDescription():<60}")

def simplify(name: str) -> str:
    
    if name.startswith("std::vector<"):
        return "std::vector< ... >"
    elif name.startswith("ROOT::Math::LorentzVector<"):
        return "ROOT::Math::LorentzVector< ... >"
    elif name.startswith("ROOT::Math::PositionVector3D<"):
        return "ROOT::Math::PositionVector3D< ... >"
    elif name.startswith("ROOT::Math::DisplacementVector3D<"):
        return "ROOT::Math::DisplacementVector3D< ... >"
    elif name.startswith("ROOT::Math::SMatrix<"):
        return "ROOT::Math::SMatrix< ... >"
    else:
        return name
    

if __name__ == '__main__':
    main()