#ifndef PHDST_HPP
#define PHDST_HPP

#include <string>
#include <cstring>

extern "C" void phdst_(char *, int *, int *, size_t*);
extern "C" void phset_(char *, int *, size_t*);
extern "C" void phrty_(char *, size_t*);
extern "C" int iphpic_(char *, int *, size_t*);
extern "C" void phrty_(char *, size_t*);
extern "C" void timed_(float *);
extern "C" void timex_(float *);

namespace phdst
{
    inline void PHDST(const std::string &name, int &&n, int &m)
    {
        char c_name[name.size()];
        std::strncpy(c_name, name.c_str(), name.size()); 
        size_t len = name.size();
        phdst_(c_name, &n, &m, &len);
    }

    inline void PHSET(const std::string &name, int &n)
    {
        char c_name[name.size()];
        size_t len = name.size();
        std::strncpy(c_name, name.c_str(), name.size());    
        phset_(c_name, &n, &len);
    }

    inline std::string PHRTY()
    {
        char name[] = "1234";
        size_t len = 4;
        phrty_(name, &len);
        return std::string(name, len);
    }

    inline int IPHPIC(const std::string &name, int &n)
    {
        char c_name[name.size()];
        std::strncpy(c_name, name.c_str(), name.size());        
        size_t len = name.size();
        return iphpic_(c_name, &n, &len);
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

extern "C" struct {
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

extern "C" struct {
    int iiiexp;
    int iiirun;
    int iifile;
    int iiievt;
    int iiidat;
    int iiitim;
    int iifill;
} phciii_;

extern "C" struct {
    int uxfenv[10];
    int luxstr[30];
    int luxref[30];
} uxcom_;

extern "C" struct {
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

#define NWPXMA 1024
extern "C" struct {
    int nwpilt;
    int iupilt[NWPXMA];
} pxchdr_;

namespace phdst
{
    inline int& PHGFST() { return phgen_.phgfst; }
    inline int& NFZFIL() { return phgen_.nfzfil; }
    inline int& NFZPIL() { return phgen_.nfzpil; }
    inline int& NFZGET() { return phgen_.nfzget; }
    inline int& NFZPIX() { return phgen_.nfzpix; }
    inline int& NFZGEX() { return phgen_.nfzgex; }
    inline int& NEVENT() { return phgen_.nevent; }
    inline int& NGOODS() { return phgen_.ngoods; }
    inline int& NEVENX() { return phgen_.nevenx; }
    inline int& NGOODX() { return phgen_.ngoodx; }
    inline int& INTRCT() { return phgen_.intrct; }
    inline float& TIMTOT() { return phgen_.timtot; }
    inline float& TIMEND() { return phgen_.timend; }
    inline int& MAXPST() { return phgen_.maxpst; }
    inline int& LIMPSF() { return phgen_.limpsf; }
    inline int& LAPSTG() { return phgen_.lapstg; }
    inline int& NFILOU() { return phgen_.nfilou; }
    inline int& NDSSAV(int i) { return phgen_.ndssav[i-1]; }
    inline int& PACKED() { return phgen_.packed; }
    inline int& IVPACK() { return phgen_.ivpack; }
    inline float& FILIMT() { return phgen_.filimt; }
    inline int& FILACC() { return phgen_.filacc; }
    inline int& STGPUTW() { return phgen_.stgputw; }
    inline int& SGNEXIT() { return phgen_.sgnexit; }
    inline int& PHGLST() { return phgen_.phglst; }  

    inline int& IIIEXP() { return phciii_.iiiexp; }
    inline int& IIIRUN() { return phciii_.iiirun; }
    inline int& IIFILE() { return phciii_.iifile; }
    inline int& IIIEVT() { return phciii_.iiievt; }
    inline int& IIIDAT() { return phciii_.iiidat; }
    inline int& IIITIM() { return phciii_.iiitim; }
    inline int& IIFILL() { return phciii_.iifill; }

    inline int& LQ(int i) { return uxcom_.luxstr[i-1]; }
    inline int& IQ(int i) { return uxcom_.luxstr[i+7]; }
    inline float&  Q(int i) { return *reinterpret_cast<float*>(&uxcom_.luxstr[i+7]); }

    inline int& LTEMP() { return uxlink_.ltemp; }
    inline int& LRTOP() { return uxlink_.lrtop; }
    inline int& LSTOP() { return uxlink_.lstop; }
    inline int& LTTOP() { return uxlink_.lttop; }
    inline int& LITOP() { return uxlink_.litop; }
    inline int& LRTEMP() { return uxlink_.lrtemp; }
    inline int& LRWTMP() { return uxlink_.lrwtmp; }
    inline int& LRAWUX() { return uxlink_.lrawux; }
    inline int& LBKTOP() { return uxlink_.lbktop; }
    inline int& LORTOP() { return uxlink_.lortop; }
    inline int& LRTINT() { return uxlink_.lrtint; }
    inline int& LDTOP() { return uxlink_.ldtop; }       
}


#endif // PHDST_HPP
