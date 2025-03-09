#ifndef PHDST_HPP
#define PHDST_HPP

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

    extern "C" struct
    {
        int phgfst;
        int nfzfil;
        int nfzpil;
        int nfzget;
        int nfzpix;
        int nfzgex;
        int nevent;
        int ngoods;
        int nevenx;
        int ngoodx;
        int intrct;
        float timtot;
        float timend;
        int maxpst;
        int limpsf;
        int lapstg;
        int nfilou;
        int ndssav[10];
        int packed;
        int ivpack;
        float filimt;
        int filacc;
        int stgputw;
        int sgnexit;
        int phglst;
    } phgen_;

    extern "C" struct
    {
        int iiiexp;
        int iiirun;
        int iifile;
        int iiievt;
        int iiidat;
        int iiitim;
        int iifill;
    } phciii_;

    extern "C" struct
    {
        int uxfenv[10];
        int luxstr[30];
        int luxref[30];
    } uxcom_;

    extern "C" struct
    {
        int ltemp;
        int lrtop;
        int lstop;
        int lttop;
        int litop;
        int lrtemp;
        int lrwtmp;
        int lrawux;
        int lbktop;
        int lortop;
        int lrtint;
        int ldtop;
    } uxlink_;

    const int NWPXMA = 1024;
    extern "C" struct
    {
        int nwpilt;
        int iupilt[NWPXMA];
    } pxchdr_;

    inline int &PHGFST = phgen_.phgfst;
    inline int &NFZFIL = phgen_.nfzfil;
    inline int &NFZPIL = phgen_.nfzpil;
    inline int &NFZGET = phgen_.nfzget;
    inline int &NFZPIX = phgen_.nfzpix;
    inline int &NFZGEX = phgen_.nfzgex;
    inline int &NEVENT = phgen_.nevent;
    inline int &NGOODS = phgen_.ngoods;
    inline int &NEVENX = phgen_.nevenx;
    inline int &NGOODX = phgen_.ngoodx;
    inline int &INTRCT = phgen_.intrct;
    inline float &TIMTOT = phgen_.timtot;
    inline float &TIMEND = phgen_.timend;
    inline int &MAXPST = phgen_.maxpst;
    inline int &LIMPSF = phgen_.limpsf;
    inline int &LAPSTG = phgen_.lapstg;
    inline int &NFILOU = phgen_.nfilou;
    inline int &NDSSAV(int i) { return phgen_.ndssav[i - 1]; }
    inline int &PACKED = phgen_.packed;
    inline int &IVPACK = phgen_.ivpack;
    inline float &FILIMT = phgen_.filimt;
    inline int &FILACC = phgen_.filacc;
    inline int &STGPUTW = phgen_.stgputw;
    inline int &SGNEXIT = phgen_.sgnexit;
    inline int &PHGLST = phgen_.phglst;

    inline int &IIIEXP = phciii_.iiiexp;
    inline int &IIIRUN = phciii_.iiirun;
    inline int &IIFILE = phciii_.iifile;
    inline int &IIIEVT = phciii_.iiievt;
    inline int &IIIDAT = phciii_.iiidat;
    inline int &IIITIM = phciii_.iiitim;
    inline int &IIFILL = phciii_.iifill;

    inline int &LQ(int i) { return uxcom_.luxstr[i - 1]; }
    inline int &IQ(int i) { return uxcom_.luxstr[i + 7]; }
    inline float &Q(int i) { return *reinterpret_cast<float *>(&uxcom_.luxstr[i + 7]); }

    inline int &LTEMP  = uxlink_.ltemp;
    inline int &LRTOP  = uxlink_.lrtop;
    inline int &LSTOP  = uxlink_.lstop;
    inline int &LTTOP  = uxlink_.lttop;
    inline int &LITOP  = uxlink_.litop;
    inline int &LRTEMP  = uxlink_.lrtemp;
    inline int &LRWTMP  = uxlink_.lrwtmp;
    inline int &LRAWUX  = uxlink_.lrawux;
    inline int &LBKTOP  = uxlink_.lbktop;
    inline int &LORTOP  = uxlink_.lortop;
    inline int &LRTINT  = uxlink_.lrtint;
    inline int &LDTOP  = uxlink_.ldtop;

    inline int &NWPILT  = pxchdr_.nwpilt;
    inline int &IUPILT(int i) { return pxchdr_.iupilt[i - 1]; }
    inline int &IPILOT(int i) { return pxchdr_.iupilt[i - 1]; }

}

#endif // PHDST_HPP
