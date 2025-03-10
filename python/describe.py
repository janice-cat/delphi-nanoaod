#!/usr/bin/env python3

import argparse

import ROOT

def main():

    parser = argparse.ArgumentParser(description='Simple plots')
    parser.add_argument('--input', help='Input file', required=True)
    args = parser.parse_args()
    
    file = ROOT.TFile.Open(args.input)
    df = ROOT.RDF.Experimental.FromRNTuple(file['Events'])
    df.Describe().Print()
    
if __name__ == '__main__':
    main()
