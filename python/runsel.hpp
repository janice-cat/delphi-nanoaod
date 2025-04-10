#ifndef RUNSEL_HPP
#define RUNSEL_HPP

#include <string>
#include <vector>

class RunSel {
public:
    RunSel() = default;
    ~RunSel() = default;
    RunSel(const RunSel&) = delete; // Disable copy constructor
    void init();
private:
    static const std::vector<std::string> runquali_names_;
};


#endif // RUNSEL_HPP