#!/usr/bin/env python3
#
#   python/describe.py --input /path/to/file.root
#
# This script prints the description of the dataframe using the RDF interface.
#
# Dietrich Liko, 2025 

import argparse

import ROOT

def main():

    parser = argparse.ArgumentParser(description='Print dataframe description')
    parser.add_argument('--input', help='Input file', required=True)
    args = parser.parse_args()
    
    file = ROOT.TFile.Open(args.input)
    df = ROOT.RDF.Experimental.FromRNTuple(file['Events'])
    df.Describe().Print()
    
if __name__ == '__main__':
    main()
