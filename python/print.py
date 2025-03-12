#!/usr/bin/env python3

import argparse

import ROOT

def main():

    parser = argparse.ArgumentParser(description='Simple plots')
    parser.add_argument('columns', nargs='*')
    parser.add_argument('--input', help='Input file', required=True)
    parser.add_argument("--events", help='Events to print', default=10)
    args = parser.parse_args()
    
    df = ROOT.RDF.Experimental.FromRNTuple("Events", args.input)
    df.Display(args.columns,args.events).Print()

if __name__ == '__main__':
    main()
