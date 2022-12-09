#ifndef CXL_DLLP_H
#define CXL_DLLP_H

#include "common.h"
#include "cxl.h"

class cxl_dllp
{
public:   
   cxl_dllp();
   void proc_cxl_dllp(const char*, unsigned long long, unsigned, unsigned *);
   
private:
   unsigned dw[FLIT_DW];

   dllp_t    dllp;
};

#endif
