#!/usr/bin/env python3
#
#   python/describe.py --input /path/to/file.root
#
# This script prints the description of the dataframe using the RDF interface.
#
# Dietrich Liko, 2025

import pathlib

import click
import ROOT


@click.command()
@click.argument(
    "input_path",
    metavar="PATH",
    type=click.Path(exists=True, dir_okay=False, path_type=pathlib.Path),
)
def main(input_path: pathlib.Path):
    """
    Print the description of the dataframe using the RDF interface.
    """

    file = ROOT.TFile.Open(str(input_path))
    df = ROOT.RDF.Experimental.FromRNTuple(file["Events"])
    df.Describe().Print()


if __name__ == "__main__":
    main()
