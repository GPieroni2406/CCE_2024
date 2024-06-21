/*********************************************************************/
/*                                                                   */
/*         Author: Gadiel Seroussi                                   */
/*         Last modified: 2017/10/26                                 */
/*                                                                   */
/*********************************************************************/
#ifndef __GF__
#define __GF__

#ifndef _GLOB_M
//#define _GLOB_M 5
//#define _GLOB_M 6
//#define _GLOB_M 7
#define _GLOB_M 8
#endif

#define _GLOB_QM1 ((1U<<_GLOB_M)-1U)

#define gfadd(x,y)    ((x)^(y))
#define gfmulnz(x,y)  (_gfalog[(_gflog[x]+_gflog[y])%(_GLOB_QM1)])
#define gfmul(x,y)    (((x)*(y))?(gfmulnz(x,y)):(0))
#define gfinvnz(x)    (_gfalog[_GLOB_QM1-_gflog[x]])
#define gfinv(x)      ((x)?(gfinvnz(x)):(0))
#define gfpownz(x,k)  (_gfalog[(((_GLOB_QM1)+(k)%(_GLOB_QM1))*_gflog[x])%(_GLOB_QM1)])
#define gfpow(x, k)   ((x)?(gfpownz(x,k)):((k)?0:_gfalog[0]))
#define gfalog(i) ( _gfalog[((i%_GLOB_QM1)+_GLOB_QM1)%_GLOB_QM1] )
#define gflog(x) (_gflog[x])

extern unsigned _gfalog[];
extern unsigned _gflog[];
#endif
