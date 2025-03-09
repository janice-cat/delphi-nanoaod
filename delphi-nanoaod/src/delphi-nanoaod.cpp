#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "argparse/argparse.hpp"
#include "yaml-cpp/yaml.h"
#include "delphi-nanoaod_version.hpp"
#include "nanoaod_writer.hpp"

void configure_parser(argparse::ArgumentParser &program)
{
  auto &group = program.add_mutually_exclusive_group(true);

  group.add_argument("-N", "--nickname")
      .metavar("NICKNAME")
      .help("Fatmen nickname of the sample");

  group.add_argument("-P", "--pdlinput")
      .metavar("FILE")
      .help("Path to the pdl input file");

  program.add_argument("-C", "--config")
      .help("Path to the yaml configuration file")
      .metavar("FILE")
      .default_value(std::string("delphi-nanoaod.yaml"));

  program.add_argument("-m", "--max-events")
      .help("Maximum number of events to process")
      .metavar("N")
      .scan<'i', int>();

  program.add_argument("--mc")
      .help("Write simulation information")
      .flag();

  program.add_argument("-O", "--output")
      .help("Output file name")
      .metavar("FILE")
      .default_value(std::string("delphi-nanoaod.root"));
}

int create_pdlinput(const argparse::ArgumentParser &program)
{
  std::filesystem::remove("PDLINPUT");

  if (auto nickname = program.present("--nickname"))
  {
    std::cout << "nickname: " << nickname.value() << std::endl;
    std::ofstream pdlinput("PDLINPUT");
    if (pdlinput.is_open())
    {
      pdlinput << "FAT = " << nickname.value() << std::endl;
      pdlinput.close();
    }
    else
    {
      std::cerr << "ERROR: Unable to open file PDLINPUT" << std::endl;
      return 1;
    }
  }
  else
  {
    auto pdlinput = std::filesystem::path(program.get("--pdlinput"));
    std::cout << "pdlinput: " << pdlinput << std::endl;
    if (!std::filesystem::exists(pdlinput))
    {
      std::cerr << "ERROR: File " << pdlinput << " does not exist" << std::endl;
      return 1;
    }
    std::filesystem::create_symlink(pdlinput, "PDLINPUT");
  }
  return 0;
}


int main(int argc, char *argv[])
{
  argparse::ArgumentParser program("delphi-nanoaod", DELPHI_NANOAOD_VERSION);
  configure_parser(program);

  try {
      program.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  NanoAODWriter *nanoAODWriter = NanoAODWriter::getInstance();

  if (int rc = create_pdlinput(program))
  {
    return rc;
  }

  if (auto maxEvents = program.present<int>("--max-events"))  
  {
    std::cout << "max-events: " << maxEvents.value() << std::endl;
    nanoAODWriter->setMaxEventsToProcess(maxEvents.value());
  }

  if (program.get<bool>("--mc"))
  {
    std::cout << "mc: true" << std::endl;
    nanoAODWriter->setMC();
  }

  auto output = program.get<std::string>("--output");  
  std::cout << "output: " << output << std::endl;
  nanoAODWriter->setOutput(output);

  auto config = program.get<std::string>("--config");  
  std::cout << "config: " << config << std::endl;
  try
  {
    YAML::Node configNode = YAML::LoadFile(config);
  }
  catch (const YAML::BadFile &err)
  {
    std::cerr << "ERROR: Unable to open file " << config << std::endl;
    return 1;
  }

  int rc = nanoAODWriter->run(" ");

  return rc;
}