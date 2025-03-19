#ifndef PHDST_FUNCTIONS_HPP
#define PHDST_FUNCTIONS_HPP

#include <string>
#include <cstring>

namespace phdst
{
    extern "C" void phdst_(char *, int *, int *, size_t);
    extern "C" void phset_(char *, int *, size_t);
    extern "C" void phrty_(char *, size_t);
    extern "C" int iphpic_(char *, int *, size_t);
    extern "C" void timed_(float *);
    extern "C" void timex_(float *);

    inline void PHDST(const std::string &name, int &&n, int &m)
    {
        char c_name[name.size()];
        size_t len = name.size();
        std::strncpy(c_name, name.c_str(), len);
        phdst_(c_name, &n, &m, len);
    }

    inline void PHSET(const std::string &name, int &&n)
    {
        char c_name[name.size()];
        size_t len = name.size();
        std::strncpy(c_name, name.c_str(), len);
        phset_(c_name, &n, len);
    }

    inline std::string PHRTY()
    {
        char name[] = "1234";
        size_t len = 4;
        phrty_(name, len);

        // strip blanks
        while (len > 0 && name[len - 1] == ' ')
        {
            len--;
        }

        return std::string(name, len);
    }

    inline int IPHPIC(const std::string &name, int &&n)
    {
        char c_name[name.size()];
        size_t len = name.size();
        std::strncpy(c_name, name.c_str(), len);
        return iphpic_(c_name, &n, len);
    }

    inline void TIMED(float &time)
    {
        timed_(&time);
    }

    inline void TIMEX(float &time)
    {
        timex_(&time);
    }
}

#endif // PHDST_FUNCTIONS_HPP