#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <string>

extern "C" void psini_();
extern "C" void psbeg_();
extern "C" void psrunq_(int *);


namespace skelana
{
    inline void PSINI() {psini_();}
    inline void PSBEG() {psbeg_();}
    inline void PSRUNQ(int *n) {psrunq_(n);
}

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

#define MTRACK 300
#define MJET 30

#define LVPART 1
#define LVJET 260
#define LVTHRU 250
#define LVSPHE 255

extern "C" struct 
{
    float vecp[3*MTRACK][10];
} pucppp_ ;

extern "C" struct 
{
    int nvecp;
    int ncvecp;
    int nnvecp;
    int nvecmc;
    int njet;
    int lvlock[3*MTRACK];
    int invecp[3*MTRACK];
} pscvec_;

#define MRQFIL 5000
#define NRQDET 38
#define NRQTRG 23
#define MSRUNS 3000

extern "C" struct {
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

#define NCUT 3
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
    float vdonly[NCUT];
    int idvdwz[NCUT];
    int ihadrj[NCUT];
    float istoel[NCUT];
} pscutt_;

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

#define NPMAX 4000
extern "C" struct 
{
    int np;
    int kp[5][NPMAX];
    float pp[5][NPMAX];
    float vp[5][NPMAX];
} pscluj_;

#define LENVTX 17
#define NVTXMX 150

extern "C" struct
{
    int nvtx;
    int nvtxmc;
    int kvtx[2*NVTXMX][LENVTX];
} pscvtx_;

namespace skelana 
{
    inline int& IFLTRA() {return pscflg_.ifltra;}
    inline int& IFLFIX() {return pscflg_.iflfix;}
    inline int& IFLRNQ() {return pscflg_.iflrnq;}
    inline int& IFLSTR() {return pscflg_.iflstr;}
    inline int& IFLJET() {return pscflg_.ifljet;}
    inline int& IFLSIM() {return pscflg_.iflsim;}
    inline int& IFLBSP() {return pscflg_.iflbsp;}
    inline int& IFLBTG() {return pscflg_.iflbtg;}
    inline int& IFLEMC() {return pscflg_.iflemc;}
    inline int& IFLHAC() {return pscflg_.iflhac;}
    inline int& IFLSTC() {return pscflg_.iflstc;}
    inline int& IFLELE() {return pscflg_.iflele;}
    inline int& IFLPHO() {return pscflg_.iflpho;}
    inline int& IFLMUO() {return pscflg_.iflmuo;}
    inline int& IFLHAD() {return pscflg_.iflhad;}
    inline int& IFLVDH() {return pscflg_.iflvdh;}
    inline int& IFLRV0() {return pscflg_.iflrv0;}
    inline int& IFLUTE() {return pscflg_.iflute;}
    inline int& IFLPHC() {return pscflg_.iflphc;}
    inline int& IFLVEC() {return pscflg_.iflvec;}
    inline int& IFLBHP() {return pscflg_.iflbhp;}
    inline int& IFLECL() {return pscflg_.iflecl;}
    inline int& IFLRVR() {return pscflg_.iflrvr;}
    inline int& IFLODR() {return pscflg_.iflodr;}
    inline int& IFLPVT() {return pscflg_.iflpvt;}
    inline int& IFLCUT() {return pscflg_.iflcut;}
    inline int& IFLVDR() {return pscflg_.iflvdr;}
    inline int& IFLFCT() {return pscflg_.iflfct;}
    inline int& IFLENR() {return pscflg_.iflenr;}

    inline float& TRKMOM(int i) {return pscutt_.trkmom[i-1];}
    inline float& TRKLEN(int i) {return pscutt_.trklen[i-1];}
    inline float& TRKRPH(int i) {return pscutt_.trkrph[i-1];}
    inline float& TRKZET(int i) {return pscutt_.trkzet[i-1];}
    inline float& TRKMAX(int i) {return pscutt_.trkmax[i-1];}
    inline float& TRKERR(int i) {return pscutt_.trkerr[i-1];}
    inline float& RECCAL(int i) {return pscutt_.reccal[i-1];}
    inline float& EHPC(int i) {return pscutt_.ehpc[i-1];}
    inline float& EFEMC(int i) {return pscutt_.efemc[i-1];}
    inline float& EHAC(int i) {return pscutt_.ehac[i-1];}
    inline float& ESTIC(int i) {return pscutt_.estic[i-1];}
    inline float& VDONLY(int i) {return pscutt_.vdonly[i-1];}
    inline int& IDVDWZ(int i) {return pscutt_.idvdwz[i-1];}
    inline int& IHADRJ(int i) {return pscutt_.ihadrj[i-1];}
    inline float& ISTOEL(int i) {return pscutt_.istoel[i-1];}

    inline float& BMAG() {return pxcons_.bmag;}
    inline float& AMPI() {return pxcons_.ampi;}
    inline float& PI() {return pxcons_.pi;}
    inline float& EBEAM() {return pxcons_.ebeam;}
    inline float& EBMDEF() {return pxcons_.ebmdef;}

    inline float& VECP(int i, int j) {return pucppp_.vecp[j-1][i-1];}
    inline int& IVECP(int i, int j) {return *reinterpret_cast<int*>(&pucppp_.vecp[j-1][i-1]);}

    inline int& NVEC() {return pscvec_.nvecp;}
    inline int& NCVEC() {return pscvec_.ncvecp;}
    inline int& NNVEC() {return pscvec_.nnvecp;}
    inline int& NVMC() {return pscvec_.nvecmc;}
    inline int& NJET() {return pscvec_.njet;}
    inline int& LVLOCK(int i) {return pscvec_.lvlock[i-1];}
    inline int& INVEC(int i) {return pscvec_.invecp[i-1];}  

    inline int& NRQFIL() {return pscrnq_.nrqfil;}
    inline int& IRQFIL(int i) {return pscrnq_.irqfil[i-1];}
    inline float& ERQFIL(int i) {return pscrnq_.erqfil[i-1];}
    inline int& IRQDMN(int i) {return pscrnq_.irqdmn[i-1];}
    inline int& IRQDMX(int i) {return pscrnq_.irqdmx[i-1];}
    inline int& IRQTMN(int i) {return pscrnq_.irqtmn[i-1];}
    inline int& IRQTMX(int i) {return pscrnq_.irqtmx[i-1];}
    inline int& IRQDET(int i) {return pscrnq_.irqdet[i-1];}
    inline int& IRQTRG(int i) {return pscrnq_.irqtrg[i-1];}
    inline int& NSRUNS() {return pscrnq_.nsruns;}
    inline int& IFRUNS(int i) {return pscrnq_.ifruns[i-1];}
    inline int& ILRUNS(int i) {return pscrnq_.ilruns[i-1];}
    inline int& IFFILE(int i) {return pscrnq_.iffile[i-1];}
    inline int& ILFILE(int i) {return pscrnq_.ilfile[i-1];}

    inline int& ISVER() {return pscevt_.isver;}
    inline int& IHAD4() {return pscevt_.ihad4;}
    inline int& NCTR4() {return pscevt_.nctr4;}
    inline int& NCTRK() {return pscevt_.nctrk;}
    inline int& NNTRK() {return pscevt_.nntrk;}
    inline float& ECMAS() {return pscevt_.ecmas;}
    inline float& ECHAR() {return pscevt_.echar;}
    inline float& EMNEU() {return pscevt_.emneu;}
    inline float& EHNEU() {return pscevt_.ehneu;}

    inline std::string CDTYPE() {return std::string(pscevc_.cdtype,4);}
    inline std::string LABO() {return std::string(pscevc_.labo, 4);}

    inline int& NP() {return pscluj_.np;}
    inline int& KP(int i, int j) {return pscluj_.kp[j-1][i-1];}
    inline float& PP(int i, int j) {return pscluj_.pp[j-1][i-1];}
    inline float& VP(int i, int j) {return pscluj_.vp[j-1][i-1];}

    inline int& NVTX() {return pscvtx_.nvtx;}
    inline int& NVTXMC() {return pscvtx_.nvtxmc;}
    inline int& KVTX(int i, int j) {return pscvtx_.kvtx[j-1][i-1];}
    inline float& QVTX(int i, int j) {return *reinterpret_cast<float*>(&pscvtx_.kvtx[j-1][i-1]);}
}
#endif // SKELETON_HPP
