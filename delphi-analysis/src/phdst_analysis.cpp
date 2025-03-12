#include "phdst_analysis.hpp"
#include "phdst.hpp"
#include <iostream>

namespace phdst
{
    Analysis *Analysis::instance_ = nullptr;

    Analysis::Analysis() : maxEventsToProcess_(0) {};

    Analysis::~Analysis() {};

    Analysis *Analysis::getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new Analysis();
        }
        return instance_;
    }

    void Analysis::setMaxEventsToProcess(int maxEventsToProcess)
    {
        this->maxEventsToProcess_ = maxEventsToProcess;
    }

    int Analysis::run(const std::string &option)
    {
        int flag = 0;
        PHDST(option, 0, flag);
        return flag;
    }

    void Analysis::user00()
    {
        // std::cout << "PHDST::Analysis::user00: Initialising" << std::endl;
    }

    int Analysis::user01()
    {
        // std::cout << "PHDST::Analysis::user01: Processing pilot " << NEVENT+1 << std::endl;
        if (maxEventsToProcess_ > 0 && NEVENT > maxEventsToProcess_)
        {
            std::cout << "PHDST::Analysis::user01: Reached maximum number of events" << std::endl;
            return -3;
        }
        return 1;
    }

    void Analysis::user02()
    {
        // std::cout << "PHDST::Analysis::user02: Processing event " << NEVENT+1 << std::endl;
    }

    void Analysis::user99()
    {
        // std::cout << "PHDST::Analysis::user99: Terminating" << std::endl;
    }
}

void user00_()
{
    phdst::Analysis::getInstance()->user00();
}

void user01_(int *need)
{
    *need = phdst::Analysis::getInstance()->user01();
}

void user02_()
{
    phdst::Analysis::getInstance()->user02();
}

void user99_()
{
    phdst::Analysis::getInstance()->user99();
}
