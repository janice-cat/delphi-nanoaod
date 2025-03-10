#!/usr/bin/env python3

import argparse

import ROOT

def main():

    parser = argparse.ArgumentParser(description='Simple plots')
    parser.add_argument('--input', help='Input file', required=True)
    parser.add_argument('--output', help='Output file')
    args = parser.parse_args()
    
    if args.output is None:
        args.output = args.input.replace(".root", "_hists.root")        
    
    file = ROOT.TFile.Open(args.input)
    df = ROOT.RDF.Experimental.FromRNTuple(file.Events)
    df.Describe().Print()
    
    hists = (
        df.Histo1D(("Event_nrun", "Event_nrun", 100, -10000, 10000), "Event_nrun"),
        df.Histo1D(("Event_ievt", "Event_ievt", 100, 0, 100000), "Event_ievt"),
        df.Histo1D(("Event_fill", "Event_fill", 100, 0, 1000), "Event_fill"),
        df.Histo1D(("Event_magField", "Event_magField", 100, -10, 10), "Event_magField"),
        df.Histo1D(("Event_dstVersion", "Event_dstVersion", 100, 0, 100), "Event_dstVersion"),
        df.Histo1D(("Event_hadronTag", "Event_hadronTag", 100, 0, 100), "Event_hadronTag"),
        df.Histo1D(("Event_nChaMultT4", "Event_nChaMultT4", 100, 0, 100), "Event_nChaMultT4"),
        df.Histo1D(("Event_nChaMult", "Event_nChaMult", 100, 0, 100), "Event_nChaMult"),
        df.Histo1D(("Event_nNeuMult", "Event_nNeuMult", 100, 0, 100), "Event_nNeuMult"),
        df.Histo1D(("Event_cmEnergy", "Event_cmEnergy", 100, 0, 100), "Event_cmEnergy"),
        df.Histo1D(("Event_totalChaEnergy", "Event_totalChaEnergy", 100, 0, 100), "Event_totalChaEnergy"),
        df.Histo1D(("Event_totalEMEnergy", "Event_totalEMEnergy", 100, 0, 100), "Event_totalEMEnergy"),
        df.Histo1D(("Event_totalHadEnergy", "Event_totalHadEnergy", 100, 0, 100), "Event_totalHadEnergy"),
    )
    
    with ROOT.TFile.Open(args.output, "RECREATE") as f:
        for hist in hists:
            hist.Write() 
    
    
    
if __name__ == '__main__':
    main()