#ifndef SKELANA_PXCONS_HPP
#define SKELANA_PXCONS_HPP

namespace skelana
{
/*    +KEEP,PXCONS.         General constans (from PXDST)
 */
    extern "C" struct
    {
        float bmag;
        float ampi;
        float pi;
        float ebeam;
        float ebmdef;
    } pxcons_;

    inline float &BMAG = pxcons_.bmag;
    inline float &AMPI = pxcons_.ampi;
    inline float &PI = pxcons_.pi;
    inline float &EBEAM = pxcons_.ebeam;
    inline float &EBMDEF = pxcons_.ebmdef;
}

#endif // SKELANA_PXCONS_HPP