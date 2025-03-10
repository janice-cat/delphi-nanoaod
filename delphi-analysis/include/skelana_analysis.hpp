#ifndef SKELANA_ANALYSIS_HPP
#define SKELANA_ANALYSIS_HPP

#include <map>
#include <string>

#include "phdst_analysis.hpp"
#include "skelana.hpp"

namespace skelana
{
    class Analysis : public phdst::Analysis
    {
    private:
        typedef phdst::Analysis super;
    public:
        Analysis(const Analysis &) = delete;
        Analysis &operator=(const Analysis &) = delete;
        virtual ~Analysis();
        static Analysis *getInstance();
        void selectHadrons();
        void setOption(const std::string &option, int value);
        void printSkelanaSettings(std::ostream &os);
    protected:
        Analysis();
        virtual void user00();
        virtual int user01();
        virtual void user02();
        virtual void user99();
    private:
        bool checkHadron();
        bool checkRecordType();
        bool checkBadHPC();

        bool selectHadrons_;
        inline static std::map<std::string, int*> optionMap_ =  {  
            {"IFLTRA", &pscflg_.ifltra},    
            {"IFLFIX", &pscflg_.iflfix},
            {"IFLRNQ", &pscflg_.iflrnq},
            {"IFLSTR", &pscflg_.iflstr},
            {"IFLJET", &pscflg_.ifljet},
            {"IFLSIM", &pscflg_.iflsim},
            {"IFLBSP", &pscflg_.iflbsp},
            {"IFLBTG", &pscflg_.iflbtg},
            {"IFLEMC", &pscflg_.iflemc},
            {"IFLHAC", &pscflg_.iflhac},
            {"IFLSTC", &pscflg_.iflstc},
            {"IFLELE", &pscflg_.iflele},
            {"IFLPHO", &pscflg_.iflpho},
            {"IFLMUO", &pscflg_.iflmuo},
            {"IFLHAD", &pscflg_.iflhad},
            {"IFLVDH", &pscflg_.iflvdh},
            {"IFLRV0", &pscflg_.iflrv0},
            {"IFLUTE", &pscflg_.iflute},
            {"IFLPHC", &pscflg_.iflphc},
            {"IFLVEC", &pscflg_.iflvec},
            {"IFLBHP", &pscflg_.iflbhp},
            {"IFLECL", &pscflg_.iflecl},
            {"IFLRVR", &pscflg_.iflrvr},
            {"IFLODR", &pscflg_.iflodr},
            {"IFLPVT", &pscflg_.iflpvt},
            {"IFLCUT", &pscflg_.iflcut},
            {"IFLVDR", &pscflg_.iflvdr},
            {"IFLFCT", &pscflg_.iflfct},
            {"IFLENR", &pscflg_.iflenr}
        };
    };
};

#endif // SKELANA_ANALYSIS_HPP