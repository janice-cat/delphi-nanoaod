#!/usr/bin/env python3

import argparse

import ROOT

def main():

    parser = argparse.ArgumentParser(description='Simple plots')
    parser.add_argument('--input', help='Input file', required=True)
    parser.add_argument('--output', help='Output file')
    args = parser.parse_args()        
    
    file = ROOT.TFile.Open(args.input)
    
    with ROOT.TFile.Open(args.input) as f:
        reader = ROOT.Experimental.RNTupleReader.Open(f["Events"])
        print(f"Entries = {reader.GetNEntries()}:

    
    
    # df = ROOT.RDF.Experimental.FromRNTuple("Events", args.input)
    # df.Describe().Print()
    # print(f"Rows = {df.Count().GetValue()}")
    
    
if __name__ == '__main__':
    main()