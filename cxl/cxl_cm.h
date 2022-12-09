#ifndef CXL_CM_H
#define CXL_CM_H

#include "common.h"
#include "cxl.h"

class cxl_cm
{
public:   
   cxl_cm();
   void proc_cxl_cm(const char*, unsigned long long, unsigned, unsigned *);
   
private:
   unsigned dw[FLIT_DW];
   
};

#endif
