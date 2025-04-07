#!/usr/bin/env python
# -*- coding: utf-8 -*-


import pathlib

import click
import ROOT


@click.command
@click.argument(
    "input_paths",
    metavar="PATH",
    nargs=-1,
    type=click.Path(exists=True, dir_okay=False, path_type=pathlib.Path),
)
def main(input_paths: list[pathlib.Path]):
    ROOT.EnableImplicitMT()
    for path in input_paths:
        df = ROOT.RDF.Experimental.FromRNTuple("Events", str(path))
        max_run_number = df.Max("Event_runNumber")
        min_run_number = df.Min("Event_runNumber")
        max_event_number = df.Max("Event_evtNumber")
        min_event_number = df.Min("Event_evtNumber")
        max_fill_number = df.Max("Event_fillNumber")
        min_fill_number = df.Min("Event_fillNumber")
        max_cmEnergy = df.Max("Event_cmEnergy")
        min_cmEnergy = df.Min("Event_cmEnergy")
        event = df.Count()
        print(f"File: {path}")

        if min_run_number.GetValue() == max_run_number.GetValue():
            print(f"  Run number      : {int(min_run_number.GetValue())}")
        else:
            print(
                f"  Run number      : {int(min_run_number.GetValue())} - {int(max_run_number.GetValue())}"
            )
        if min_event_number.GetValue() == max_event_number.GetValue():
            print(f"  Event number   : {int(min_event_number.GetValue())}")
        else:
            print(
                f"  Event number    : {int(min_event_number.GetValue())} - {int(max_event_number.GetValue())}"
            )
        if min_fill_number.GetValue() == max_fill_number.GetValue():
            print(f"  Fill number     : {int(min_fill_number.GetValue())}")
        else:
            print(
                f"  Fill number     : {int(min_fill_number.GetValue())} - {int(max_fill_number.GetValue())}"
            )
        if min_cmEnergy.GetValue() == max_cmEnergy.GetValue():
            print(f"  CM energy       : {min_cmEnergy.GetValue()}")
        else:
            print(
                f"  CM energy       : {min_cmEnergy.GetValue()} - {max_cmEnergy.GetValue()}"
            )
        print(f"  Number of events: {int(event.GetValue())}")


if __name__ == "__main__":
    main()
