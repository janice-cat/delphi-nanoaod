#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# """
#   python/plotter.py --data /path/to/file.root --mc /path/to/file.root --output /path/to/output.root
# """

import logging
import pathlib

import click
import click_log
from click_help_colors import HelpColorsCommand
from ruamel.yaml import YAML

log = logging.getLogger(__name__)
click_log.basic_config(log)


# lazy loading of ROOT
# import ROOT

SCRIPT_DIR = pathlib.Path(__file__).parent.resolve()


@click.command(
    cls=HelpColorsCommand,
    help_headers_color="yellow",
    help_options_color="green",
)
@click.option(
    "--data",
    "-d",
    "data_path",
    metavar="PATH",
    type=click.Path(exists=True, dir_okay=False, path_type=pathlib.Path),
    required=True,
    help="Path to the data file.",
)
@click.option(
    "--mc",
    "-m",
    "mc_path",
    metavar="PATH",
    type=click.Path(exists=True, dir_okay=False, path_type=pathlib.Path),
    required=True,
    multiple=True,
    help="Path to the MC file.",
)
@click.option(
    "-c",
    "--config",
    "config_path",
    metavar="PATH",
    type=click.Path(dir_okay=False, exists=True, path_type=pathlib.Path),
    help="Path to the histgram configuration file.",
    default=SCRIPT_DIR / "plotter.yaml",
    show_default=True,
)
@click.option(
    "--output",
    "-o",
    "output_path",
    type=click.Path(dir_okay=False, writable=True, path_type=pathlib.Path),
    metavar="PATH",
    default="output.root",
    show_default=True,
    help="Path to the output file.",
)
@click.option(
    "-n",
    "--normalise",
    type=click.Choice(["NONE", "EVENTS", "T4HADRONS"], case_sensitive=False),
    default="none",
    help="Event weights for MC files.",
)
@click_log.simple_verbosity_option(log, "-l", "--logging", default="INFO")
def main(
    data_path: pathlib.Path,
    mc_path: list[pathlib.Path],
    output_path: pathlib.Path,
    config_path: pathlib.Path,
    normalise: str,
):
    """
    Plot DELPHI NanoAOD data and MC files using ROOT.
    """
    log.info("Plotting DELPHI NanoAOD data and MC files")
    log.info("Data file: %s", data_path)
    for path in mc_path:
        log.info("MC file: %s", path)

    import ROOT

    ROOT.EnableImplicitMT()
    # Suppress ROOT informational messages
    ROOT.gErrorIgnoreLevel = ROOT.kWarning

    # Suppress the statistics box
    ROOT.gStyle.SetOptStat(0)

    dataframes = {
        "data": ROOT.RDF.Experimental.FromRNTuple("Events", str(data_path)),
        "mc": ROOT.RDF.Experimental.FromRNTuple("Events", [str(p) for p in mc_path]),
    }

    data_events = dataframes["data"].Count().GetValue()
    mc_events = dataframes["mc"].Count().GetValue()
    log.info("Data events: %d, MC events %d", data_events, mc_events)
    if normalise.casefold() == "events":
        weight = data_events / mc_events
    elif normalise.casefold() == "t4hadrons":
        data_hadrons = (
            dataframes["data"].Filter("Event_hadronTag > 0").Count().GetValue()
        )
        mc_hadrons = dataframes["mc"].Filter("Event_hadronTag >0 ").Count().GetValue()
        log.info("Data hadrons: %d, MC hadrons %d", data_hadrons, mc_hadrons)
        weight = data_hadrons / mc_hadrons
    else:
        weight = 1.0

    log.info("Normalisation weight: %s", weight)

    # Include the C++ header file into PyROOT
    ROOT.gInterpreter.AddIncludePath(str(SCRIPT_DIR))
    ROOT.gInterpreter.Declare("""
    #include "plotter.hpp"
    """)

    yaml = YAML(typ="safe")
    with open(config_path, "r") as config_file:
        config = yaml.load(config_file)

    histos = {}
    for df_name, df in dataframes.items():
        df = df.Define("weight", "1.0" if df_name == "data" else f"{weight}")
        for df_config in config:
            for name, expression in df_config.get("Define", {}).items():
                log.debug("Defining %s: %s", name, expression)
                try:
                    # Check if the expression is a valid C++ expression
                    df = df.Define(name, expression)
                except (
                    TypeError,
                    RuntimeError,
                ):  # Handle the case where the expression is not valid
                    log.error("Error defining %s: %s", name, expression)
                    continue
            for name, histo1D in df_config["Histo1D"].items():
                title = histo1D.get("title", name)
                nbins, min_bin, max_bin = histo1D["bins"]
                varname = histo1D.get("varname", name)
                histo_name = f"{df_name}_{name}"
                log.debug("Creating histogram %s: %s", histo_name, varname)
                try:
                    histos[histo_name] = df.Histo1D(
                        (
                            histo_name,
                            title,
                            nbins,
                            min_bin,
                            max_bin,
                        ),
                        varname,
                        "weight",
                    )
                except (
                    TypeError
                ):  # Handle the case where the histogram cannot be created
                    log.error("Error creating histogram %s: %s", histo_name, varname)
                    continue
    log.info("Filling histograms")
    ROOT.RDF.RunGraphs(list(histos.values()))

    with ROOT.TFile(str(output_path), "RECREATE") as _output_file:
        for histo in histos.values():
            histo.Write()
    log.info("Histograms written to %s", output_path)

    pdf_output_path = output_path.with_suffix(".pdf")
    # Plot histograms with ratio plot
    for i, (name, histo1D) in enumerate(df_config["Histo1D"].items()):
        title = histo1D.get("title", name)

        # Create canvas with three pads: one for the histogram, one for the ratio, and one for the logarithmic plot
        canvas = ROOT.TCanvas(name, title, 800, 1000)  # Adjust height for three pads
        canvas.Divide(1, 2)

        # Upper pad for histograms
        upper_pad = canvas.cd(1)
        upper_pad.SetPad(0, 0.3, 1, 1)  # Top 70% of the canvas
        upper_pad.SetBottomMargin(0.15)  # Reduce bottom margin
        upper_pad.SetGrid()

        hist_data = histos[f"data_{name}"].GetValue()
        hist_mc = histos[f"mc_{name}"].GetValue()

        # Calculate the maximum value of both histograms
        max_data = hist_data.GetMaximum()
        max_mc = hist_mc.GetMaximum()
        max_y = max(max_data, max_mc) * 1.2  # Add 20% margin for better visualization

        # Set the Y-axis range for the upper pad
        hist_data.GetYaxis().SetRangeUser(0, max_y)

        # Set the Y-axis label (customizable with a default value of "Entries")
        y_axis_label = histo1D.get("ylabel", "Entries")
        hist_data.GetYaxis().SetTitle(y_axis_label)
        hist_data.GetYaxis().SetTitleOffset(1.4)  # Increased offset for more space

        # Draw data as polymarkers
        hist_data.SetMarkerStyle(20)
        hist_data.SetMarkerColor(ROOT.kBlack)
        hist_data.Draw("E1")  # E1 for error bars

        # Draw MC as a line
        hist_mc.SetLineColor(ROOT.kRed)
        hist_mc.SetLineWidth(2)
        hist_mc.Draw("HIST SAME")  # HIST for a histogram line

        # Add legend
        legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
        legend.AddEntry(hist_data, "Data", "p")
        legend.AddEntry(hist_mc, "MC", "l")
        legend.Draw()

        # Lower pad for ratio plot
        lower_pad = canvas.cd(2)
        lower_pad.SetPad(
            0.0, 0.0, 1, 0.3
        )  # Adjusted to increase the distance from the lower pad
        lower_pad.SetTopMargin(0.02)  # Reduce top margin
        lower_pad.SetBottomMargin(0.15)  # Increase bottom margin for axis labels
        lower_pad.SetGrid()

        # Create relative ratio histogram
        relative_ratio = hist_data.Clone(f"relative_ratio_{name}")
        relative_ratio.Add(hist_mc, -1)  # Subtract MC from Data
        relative_ratio.Divide(hist_mc)  # Divide by MC
        relative_ratio.SetTitle("")  # Remove title
        relative_ratio.GetYaxis().SetTitle("(Data - MC) / MC")
        relative_ratio.GetYaxis().SetTitleOffset(1.4)  # Increased offset for more space
        relative_ratio.GetYaxis().SetNdivisions(505)
        relative_ratio.GetYaxis().SetTitleSize(0.1)
        relative_ratio.GetYaxis().SetTitleOffset(0.5)
        relative_ratio.GetYaxis().SetLabelSize(0.08)
        relative_ratio.GetXaxis().SetTitleSize(0.1)
        relative_ratio.GetXaxis().SetLabelSize(0.08)
        relative_ratio.GetXaxis().SetTitle(
            histo1D.get("xlabel", "Variable")
        )  # Custom X-axis label
        # Set the Y-axis range for the relative ratio plot
        relative_ratio.GetYaxis().SetRangeUser(-1.0, 1.0)

        relative_ratio.Draw("E1")  # E1 for error bars

        # Save the canvas
        if i == 0:
            # Save the first canvas as a single page PDF
            canvas.Print(f"{pdf_output_path}(", "pdf")
        elif i == len(df_config["Histo1D"]) - 1:
            canvas.Print(f"{pdf_output_path})", "pdf")
        else:
            canvas.Print(str(pdf_output_path), "pdf")
    log.info("Plots written to %s", pdf_output_path)


if __name__ == "__main__":
    main()
