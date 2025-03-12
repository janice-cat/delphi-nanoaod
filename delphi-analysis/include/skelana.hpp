#ifndef SKELANA_HPP
#define SKELANA_HPP

#include <string>

namespace skelana
{

    extern "C" void psini_();
    extern "C" void psbeg_();
    extern "C" void psrunq_(int *);
    extern "C" void psruns_(int *);
    extern "C" void psenrg_();
    extern "C" void psbhpc_();

    inline void PSINI() { psini_(); }
    inline void PSBEG() { psbeg_(); }
    inline void PSRUNQ(int &&n) { psrunq_(&n); }
    inline int PSRUNS()
    {
        int n;
        psruns_(&n);
        return n;
    }
    inline void PSENRG() { psenrg_(); }
    inline void PSBHPC() { psbhpc_(); }

    extern "C" struct
    {
        int ifltra;
        int iflfix;
        int iflrnq;
        int iflstr;
        int ifljet;
        int iflsim;
        int iflbsp;
        int iflbtg;
        int iflemc;
        int iflhac;
        int iflstc;
        int iflele;
        int iflpho;
        int iflmuo;
        int iflhad;
        int iflvdh;
        int iflrv0;
        int iflute;
        int iflphc;
        int iflvec;
        int iflbhp;
        int iflecl;
        int iflrvr;
        int iflodr;
        int iflpvt;
        int iflcut;
        int iflvdr;
        int iflfct;
        int iflenr;
    } pscflg_;

    extern "C" struct
    {
        float bmag;
        float ampi;
        float pi;
        float ebeam;
        float ebmdef;
    } pxcons_;

    inline const int MTRACK = 300;
    inline const int MJET = 30;

    inline const int LVPART = 1;
    inline const int LVJET = 260;
    inline const int LVTHRU = 250;
    inline const int LVSPHE = 255;

    extern "C" struct
    {
        float vecp[3 * MTRACK][10];
    } pucppp_;

    extern "C" struct
    {
        int nvecp;
        int ncvecp;
        int nnvecp;
        int nvecmc;
        int njet;
        int lvlock[3 * MTRACK];
        int invecp[3 * MTRACK];
    } pscvec_;

    inline const int MRQFIL = 5000;
    inline const int NRQDET = 38;
    inline const int NRQTRG = 23;
    inline const int MSRUNS = 3000;
    extern "C" struct
    {
        int nrqfil;
        int irqfil[MRQFIL];
        float erqfil[MRQFIL];
        int irqdmn[NRQDET];
        int irqdmx[NRQDET];
        int irqtmn[NRQTRG];
        int irqtmx[NRQTRG];
        int irqdet[NRQDET];
        int irqtrg[NRQDET];
        int nsruns;
        int ifruns[MSRUNS];
        int ilruns[MSRUNS];
        int iffile[MSRUNS];
        int ilfile[MSRUNS];
    } pscrnq_;

    inline const int NCUT = 3;
    extern "C" struct
    {
        float trkmom[NCUT];
        float trklen[NCUT];
        float trkrph[NCUT];
        float trkzet[NCUT];
        float trccos[NCUT];
        float trncos[NCUT];
        float trkmax[NCUT];
        float trkerr[NCUT];
        float reccal[NCUT];
        float ehpc[NCUT];
        float efemc[NCUT];
        float ehac[NCUT];
        float estic[NCUT];
        int vdonly[NCUT];
        int idvdwz[NCUT];
        int ihadrj[NCUT];
        int istoel[NCUT];
    } pscutt_;

    inline const int MEVBAD = 50000;
    extern "C" struct
    {
        int nevbad;
        int irnbad[MEVBAD];
        int ievbad[MEVBAD];
    } pscbhp_;

    extern "C" struct
    {
        int isver;
        int ihad4;
        int nctr4;
        int nctrk;
        int nntrk;
        float ecmas;
        float echar;
        float emneu;
        float ehneu;
    } pscevt_;

    extern "C" struct
    {
        char cdtype[4];
        char labo[4];
    } pscevc_;

    inline const int NPMAX = 4000;
    extern "C" struct
    {
        int np;
        int kp[5][NPMAX];
        float pp[5][NPMAX];
        float vp[5][NPMAX];
    } pscluj_;

    inline const int LENVTX = 17;
    inline const int NVTXMX = 150;
    extern "C" struct
    {
        int nvtx;
        int nvtxmc;
        int kvtx[2 * NVTXMX][LENVTX];
    } pscvtx_;

    inline const int NVMAX = 511;
    extern "C" struct
    {
        int npa;
        int nst;
        int nsh;
        int nlu;
        int ishst[NVMAX];
        int istsh[NVMAX];
        int istpa[NVMAX];
        int ipast[NVMAX];
        int ilust[NVMAX];
        int istlu[NVMAX];
        int ipapv[NVMAX][2];
        int istvx[NVMAX][2];
    } psctbl_;

    inline int &IFLTRA = pscflg_.ifltra;
    inline int &IFLFIX = pscflg_.iflfix;
    inline int &IFLRNQ = pscflg_.iflrnq;
    inline int &IFLSTR = pscflg_.iflstr;
    inline int &IFLJET = pscflg_.ifljet;
    inline int &IFLSIM = pscflg_.iflsim;
    inline int &IFLBSP = pscflg_.iflbsp;
    inline int &IFLBTG = pscflg_.iflbtg;
    inline int &IFLEMC = pscflg_.iflemc;
    inline int &IFLHAC = pscflg_.iflhac;
    inline int &IFLSTC = pscflg_.iflstc;
    inline int &IFLELE = pscflg_.iflele;
    inline int &IFLPHO = pscflg_.iflpho;
    inline int &IFLMUO = pscflg_.iflmuo;
    inline int &IFLHAD = pscflg_.iflhad;
    inline int &IFLVDH = pscflg_.iflvdh;
    inline int &IFLRV0 = pscflg_.iflrv0;
    inline int &IFLUTE = pscflg_.iflute;
    inline int &IFLPHC = pscflg_.iflphc;
    inline int &IFLVEC = pscflg_.iflvec;
    inline int &IFLBHP = pscflg_.iflbhp;
    inline int &IFLECL = pscflg_.iflecl;
    inline int &IFLRVR = pscflg_.iflrvr;
    inline int &IFLODR = pscflg_.iflodr;
    inline int &IFLPVT = pscflg_.iflpvt;
    inline int &IFLCUT = pscflg_.iflcut;
    inline int &IFLVDR = pscflg_.iflvdr;
    inline int &IFLFCT = pscflg_.iflfct;
    inline int &IFLENR = pscflg_.iflenr;

    inline float &TRKMOM(int i) { return pscutt_.trkmom[i - 1]; }
    inline float &TRKLEN(int i) { return pscutt_.trklen[i - 1]; }
    inline float &TRKRPH(int i) { return pscutt_.trkrph[i - 1]; }
    inline float &TRKZET(int i) { return pscutt_.trkzet[i - 1]; }
    inline float &TRCCOS(int i) { return pscutt_.trccos[i - 1]; }
    inline float &TRNCOS(int i) { return pscutt_.trncos[i - 1]; }
    inline float &TRKMAX(int i) { return pscutt_.trkmax[i - 1]; }
    inline float &TRKERR(int i) { return pscutt_.trkerr[i - 1]; }
    inline float &RECCAL(int i) { return pscutt_.reccal[i - 1]; }
    inline float &EHPC(int i) { return pscutt_.ehpc[i - 1]; }
    inline float &EFEMC(int i) { return pscutt_.efemc[i - 1]; }
    inline float &EHAC(int i) { return pscutt_.ehac[i - 1]; }
    inline float &ESTIC(int i) { return pscutt_.estic[i - 1]; }
    inline int &VDONLY(int i) { return pscutt_.vdonly[i - 1]; }
    inline int &IDVDWZ(int i) { return pscutt_.idvdwz[i - 1]; }
    inline int &IHADRJ(int i) { return pscutt_.ihadrj[i - 1]; }
    inline int &ISTOEL(int i) { return pscutt_.istoel[i - 1]; }

    inline int &NEVBAD = pscbhp_.nevbad;
    inline int &IRNBAD(int i) { return pscbhp_.irnbad[i - 1]; }
    inline int &IEVBAD(int i) { return pscbhp_.ievbad[i - 1]; }

    inline float &BMAG = pxcons_.bmag;
    inline float &AMPI = pxcons_.ampi;
    inline float &PI = pxcons_.pi;
    inline float &EBEAM = pxcons_.ebeam;
    inline float &EBMDEF = pxcons_.ebmdef;

    inline float &VECP(int i, int j) { return pucppp_.vecp[j - 1][i - 1]; }
    inline int &IVECP(int i, int j) { return *reinterpret_cast<int *>(&pucppp_.vecp[j - 1][i - 1]); }

    inline int &NVECP = pscvec_.nvecp;
    inline int &NCVECP = pscvec_.ncvecp;
    inline int &NNVECP = pscvec_.nnvecp;
    inline int &NVECMC = pscvec_.nvecmc;
    inline int &NJET = pscvec_.njet;
    inline int &LVLOCK(int i) { return pscvec_.lvlock[i - 1]; }
    inline int &INVEC(int i) { return pscvec_.invecp[i - 1]; }

    inline int &NRQFIL = pscrnq_.nrqfil;
    inline int &IRQFIL(int i) { return pscrnq_.irqfil[i - 1]; }
    inline float &ERQFIL(int i) { return pscrnq_.erqfil[i - 1]; }
    inline int &IRQDMN(int i) { return pscrnq_.irqdmn[i - 1]; }
    inline int &IRQDMX(int i) { return pscrnq_.irqdmx[i - 1]; }
    inline int &IRQTMN(int i) { return pscrnq_.irqtmn[i - 1]; }
    inline int &IRQTMX(int i) { return pscrnq_.irqtmx[i - 1]; }
    inline int &IRQDET(int i) { return pscrnq_.irqdet[i - 1]; }
    inline int &IRQTRG(int i) { return pscrnq_.irqtrg[i - 1]; }
    inline int &NSRUNS = pscrnq_.nsruns;
    inline int &IFRUNS(int i) { return pscrnq_.ifruns[i - 1]; }
    inline int &ILRUNS(int i) { return pscrnq_.ilruns[i - 1]; }
    inline int &IFFILE(int i) { return pscrnq_.iffile[i - 1]; }
    inline int &ILFILE(int i) { return pscrnq_.ilfile[i - 1]; }

    inline int &ISVER = pscevt_.isver;
    inline int &IHAD4 = pscevt_.ihad4;
    inline int &NCTR4 = pscevt_.nctr4;
    inline int &NCTRK = pscevt_.nctrk;
    inline int &NNTRK = pscevt_.nntrk;
    inline float &ECMAS = pscevt_.ecmas;
    inline float &ECHAR = pscevt_.echar;
    inline float &EMNEU = pscevt_.emneu;
    inline float &EHNEU = pscevt_.ehneu;

    inline std::string CDTYPE() { return std::string(pscevc_.cdtype, 4); }
    inline std::string LABO() { return std::string(pscevc_.labo, 4); }

    inline int &NP = pscluj_.np;
    inline int &KP(int i, int j) { return pscluj_.kp[j - 1][i - 1]; }
    inline float &PP(int i, int j) { return pscluj_.pp[j - 1][i - 1]; }
    inline float &VP(int i, int j) { return pscluj_.vp[j - 1][i - 1]; }

    inline int &NVTX = pscvtx_.nvtx;
    inline int &NVTXMC = pscvtx_.nvtxmc;
    inline int &KVTX(int i, int j) { return pscvtx_.kvtx[j - 1][i - 1]; }
    inline float &QVTX(int i, int j) { return *reinterpret_cast<float *>(&pscvtx_.kvtx[j - 1][i - 1]); }

    inline int &NPA = psctbl_.npa;
    inline int &NST = psctbl_.nst;
    inline int &NSH = psctbl_.nsh;
    inline int &NLU = psctbl_.nlu;
    inline int &ISHST(int i) { return psctbl_.ishst[i - 1]; }
    inline int &ISTSH(int i) { return psctbl_.istsh[i - 1]; }
    inline int &ISTPA(int i) { return psctbl_.istpa[i - 1]; }
    inline int &IPAST(int i) { return psctbl_.ipast[i - 1]; }
    inline int &ILUST(int i) { return psctbl_.ilust[i - 1]; }
    inline int &ISTLU(int i) { return psctbl_.istlu[i - 1]; }
    inline int &IPAPV(int i, int j) { return psctbl_.ipapv[j - 1][i - 1]; }
    inline int &ISTVX(int i, int j) { return psctbl_.istvx[j - 1][i - 1]; }
} // namespace skelana

#endif // SKELANA_HPP
