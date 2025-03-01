#include <iostream> 

#include "skelana_analysis.hpp"
#include "skelana.hpp"

namespace skelana
{
    Analysis::Analysis() : filterT4Hadrons_(false) {};

    Analysis::~Analysis() {};

    Analysis *Analysis::getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new Analysis();
        }
        return static_cast<Analysis*>(instance_);
    }

    void Analysis::enableRunSelection()
    {
        pscflg_.iflrnq = 1;
    }

    void Analysis::filterT4Hadrons()
    {
        filterT4Hadrons_ = true;
    }

    void Analysis::setOption(const std::string &option, int value)
    {
        auto it = optionMap_.find(option);
        if (it != optionMap_.end())
        {
            *it->second = value;
        }   
        else
        {
            std::cerr << "Option " << option << " not found." << std::endl;
        }
    }

    void Analysis::printSkelanaSettings(std::ostream &os)
    {
        os << "Skelana flags:" << std::endl;
        for (const auto &it : optionMap_)
        {
            os << it.first << " = " << *it.second << std::endl;
        }

        if (IFLSTR > 0)
        {
            os << "track selection cuts for this run:" << std::endl;
            os << "TRKMOM = " << TRKMOM(IFLCUT) << std::endl;
            os << "TRKMAX = " << TRKMAX(IFLCUT) << std::endl;
            os << "TRKERR = " << TRKERR(IFLCUT) << std::endl;
            os << "TRKRPH = " << TRKRPH(IFLCUT) << std::endl;
            os << "TRKZET = " << TRKZET(IFLCUT) << std::endl;
            os << "TRKLEN = " << TRKLEN(IFLCUT) << std::endl;
            os << "TRCCOS = " << TRCCOS(IFLCUT) << std::endl;
            os << "VDONLY = " << VDONLY(IFLCUT) << std::endl;
            os << "IDVDWZ = " << IDVDWZ(IFLCUT) << std::endl;
            os << "IHADRJ = " << IHADRJ(IFLCUT) << std::endl;
            os << "ISTOEL = " << ISTOEL(IFLCUT) << std::endl;
            os << "EHPC   = " << EHPC(IFLCUT) << std::endl;
            os << "EFEMC  = " << EFEMC(IFLCUT) << std::endl;
            os << "EHAC   = " << EHAC(IFLCUT) << std::endl;
            os << "ESTIC  = " << ESTIC(IFLCUT) << std::endl;
            os << "TRNCOS = " << TRNCOS(IFLCUT) << std::endl;
            os << "RECCAL = " << RECCAL(IFLCUT) << std::endl;          
        }
    }

    void Analysis::user00()
    {
        // Supress floating point errors
        PHSET('FPE', 0);
        // Skelana initialization
        PSINI();

        printSkelanaSettings(std::cout);
        // RunQuality slection
        if (IFLRNQ > 0)
        {
            IRQDMN[0] = 1;
            IRQDMN[1] = 1;
            IRQDMN[4] = 7;
            IRQDMN[5] = 7;
            PSRUNQ(0);
        }
        
    }

    int Analysis::user01()
    {
        int need = super::user01();
        if (need != 0)
        {
            return need;
        }   

        return 0;
    }

    void Analysis::user02()
    {
        int n = 0;
        PSRUNQ(&n);
        PSBEG();
    }

    void Analysis::user99()
    {
    }
}