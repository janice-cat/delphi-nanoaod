#ifndef PHDST_ANALYSIS_HPP
#define PHDST_ANALYSIS_HPP

#include <string>

namespace phdst
{
    class Analysis
    {
    public:
        Analysis(const Analysis &) = delete;
        Analysis &operator=(const Analysis &) = delete;
        virtual ~Analysis();
        static Analysis *getInstance();
        int run(const std::string &option);
        void setMaxEventsToProcess(int maxEventsToProcess);
    protected:
        Analysis();
        virtual void user00();
        virtual int user01();
        virtual void user02();
        virtual void user99();
    private:
        int maxEventsToProcess_;
    protected:
        static Analysis *instance_;
    };
};