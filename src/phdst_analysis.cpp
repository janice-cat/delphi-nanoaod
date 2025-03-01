#include  "phdst_analysis.hpp"
#include "phdst.hpp"

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
    }

    int Analysis::user01()
    {
        if (this->maxEventsToProcess_ > 0 && NEVENT() >= this->maxEventsToProcess_)
        {
            return -3;
        }
        return 0;
    }

    void Analysis::user02()
    {
    }

    void Analysis::user99()
    {
    }
}