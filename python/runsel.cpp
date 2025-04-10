#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>

#include "runsel.hpp"

void RunSel::init() {
    const char * delphi_dat = std::getenv("DELPHI_DAT");
    if (delphi_dat == nullptr) {
        std::cerr << "Error: DELPHI_DAT environment variable is not set." << std::endl;
        return;
    }
    const std::filesystem::path data_path(delphi_dat);
    for(auto name : RunSel::runquali_names_) {
        std::filesystem::path file_path = data_path / name;
        std::ifstream file(file_path);
        if (!file) {
            std::cerr << "Error: Unable to open file " << file_path << std::endl;
            continue;
        }
        std::string line;
        int nr_runs = 0;
        int fill_nr = 0;
        while (std::getline(file, line)) {
            //std::cout << line << std::endl; 
            // Process the line as needed
            if (line.size() < 10) {
                continue;
            }
            std::cout << line.substr(1,4) << std::endl; // Example: print the substring
            if (line.substr(1,4) == "FILL") {
                std::cout << line << std::endl; // Example: print the line
                fill_nr = std::stoi(line.substr(5,4));
                float energy = std::stof(line.substr(11,7));
                nr_runs = std::stoi(line.substr(25,3));
                std::cout << "Fill Number: " << fill_nr << ", Energy: " << energy << ", Runs: " << nr_runs << std::endl;
                continue;
            }
            if (nr_runs > 0) {
                int run_nr;
                int nr_file;
                if (fill_nr > 5450) {
                    run_nr = std::stoi(line.substr(0, 5));
                    nr_file = std::stoi(line.substr(6, 2));
                } else {
                    run_nr = std::stoi(line.substr(0, 4));
                    nr_file = std::stoi(line.substr(5, 2));
                }
                std::cout << "Run Number: " << line.substr(0, 5) << std::endl;
                std::cout << "Run Quality: " << line.substr(6, 2) << std::endl;
                std::cout << "Run Status: " << line.substr(9, 2) << std::endl;
                std::cout << "Run Date: " << line.substr(12, 8) << std::endl;
                std::cout << "Run Time: " << line.substr(21, 8) << std::endl;
                nr_runs--;
            }
        }


        file.close();
        if (file.fail()) {
            std::cerr << "Error: Failed to read file " << file_path << std::endl;
        }
    }
};

const std::vector<std::string> RunSel::runquali_names_ = 
{
    "RUNQUALI.SUMARY90",
    "RUNQUALI.SUMARY91",
    "RUNQUALI.SUMARY92",
    "RUNQUALI.SUMARY93",
    "RUNQUALI.SUMARY94",
    "RUNQUALI.SUMARY95",
    "RUNQUALI.SUMARY95P3",
    "RUNQUALI.SUMARY96",
    "RUNQUALI.SUMARY97",
    "RUNQUALI.SUMARY98",
    "RUNQUALI.SUMARY99",
    "RUNQUALI.SUMARY00"
};

