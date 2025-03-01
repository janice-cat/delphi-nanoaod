#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "argparse/argparse.hpp"
#include "yaml-cpp/yaml.h"

int main(int argc, char *argv[])
{

  argparse::ArgumentParser program("delphi-nanoaod");

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
      .default_value(0)
      .scan<'i', int>();

  program.add_argument("-O", "--output")
      .help("Output file name")
      .default_value(std::string("delphi-nanoaod.root"));

  try
  {
    program.parse_args(argc, argv);
  }
  catch (const std::exception &err)
  {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  std::filesystem::remove("PDLINPUT");

  if (program.present("--nickname"))
  {
    std::cout << "nickname: " << program.get<std::string>("--nickname") << std::endl;
    std::ofstream pdlinput("PDLINPUT");
    if (pdlinput.is_open())
    {
      pdlinput << "FAT = " << program.get<std::string>("--nickname");
      pdlinput.close();
    }
    else
    {
      std::cerr << "ERROR: Unable to open file PDLINPUT" << std::endl;
      return 1;
    }
    pdlinput << "FAT = " << program.get<std::string>("--nickname") << std::endl;
    pdlinput.close();
  }
  else
  {
    std::cout << "pdlinput: " << program.get<std::string>("--pdlinput") << std::endl;
    std::filesystem::path pdlinput(program.get<std::string>("--pdlinput"));
    if (!std::filesystem::exists(pdlinput))
    {
      std::cerr << "ERROR: File " << pdlinput << " does not exist" << std::endl;
      return 1;
    }
    std::filesystem::create_symlink(pdlinput, "PDLINPUT");
  }
  std::cout << "config: " << program.get<std::string>("--config") << std::endl;
  std::filesystem::path config(program.get<std::string>("--config"));
  if (!std::filesystem::exists(config))
  {
    std::cerr << "ERROR: File " << config << " does not exist" << std::endl;
    return 1;
  }
  if (config.extension() != ".yaml")
  {
    std::cerr << "ERROR: File " << config << " is not a yaml file" << std::endl;
    return 1;
  }
  if (auto maxEvents = program.present<int>("--max-events"))  
  {
    std::cout << "max-events: " << program.get<int>("--max-events") << std::endl;
  }

  std::cout << "output: " << program.get<std::string>("--output") << std::endl;
  std::filesystem::path output(program.get<std::string>("--output"));
  if (std::filesystem::exists(output))
  {
    std::cerr << "ERROR: File " << output << " already exists" << std::endl;
    return 1;
  }
  if (output.extension() != ".root")
  {
    std::cerr << "ERROR: File " << output << " is not a root file" << std::endl;
    return 1;
  }

  YAML::Node configNode = YAML::LoadFile(config);

  // if ( configNode["select_t4hadrons"])
  // {
  //   std::cout << "select_t4hadrons: " << configNode["select_t4hadrons"].as<bool>() << std::endl;
  // }

  return 0;
}