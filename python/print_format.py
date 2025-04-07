#!/usr/bin/env python3 
#
#   python/print_ntuple.py [--mc]
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
    parser.add_argument('--data',
    args = parser.parse_args()
    
    with ROOT.TFile.Open(args.input) as f:
        reader = ROOT.Experimental.RNTupleReader.Open(f["Events"])
        desc = reader.GetDescriptor()
        for i in range(desc.GetNFields()-1):
            desc1 = desc.GetFieldDescriptor(i)
            #TODO: improve the printout
            print(f"{desc1.GetFieldName():<40} |  {simplify(desc1.GetTypeName()):<40} | {desc1.GetFieldDescription():<60}")


if __name__ == '__main__':
    main()
