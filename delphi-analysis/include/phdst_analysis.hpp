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
        virtual void user00();
        virtual int user01();
        virtual void user02();
        virtual void user99();
    protected:
        Analysis();

    private:
        int maxEventsToProcess_;
    protected:
        static Analysis * instance_;
    };
};

extern "C" void user00_();
extern "C" void user01_(int *);
extern "C" void user02_();
extern "C" void user99_();


#endif // PHDST_ANALYSIS_HPP