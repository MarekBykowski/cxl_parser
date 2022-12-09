#ifndef CXL_ALMP_H
#define CXL_ALMP_H

#include "common.h"
#include "cxl.h"

class cxl_almp
{
public:   
   cxl_almp();
   void proc_cxl_almp(const char*, unsigned long long, unsigned, unsigned *);
   
private:
   unsigned dw[FLIT_DW];

   almp_t    almp;
};

#endif
