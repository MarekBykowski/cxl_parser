#ifndef CXL_IO_H
#define CXL_IO_H

#include "common.h"
#include "cxl.h"

class cxl_io
{
public:   
   cxl_io();
   void proc_cxl_io(const char*, unsigned long long, unsigned, unsigned *, unsigned);
   void print_address(unsigned, unsigned *, unsigned &);
   void print_data(unsigned *, unsigned &index);
   
private:
   unsigned tlp_length;
   unsigned tlp_tag;
   unsigned data_length;
   unsigned dw[FLIT_DW];
   
   stp_t          stp;
   header_dw0_t   h0;
   header_dw1_t   h1;
   cfg_adrs_t     cfg_adrs;
   tlp_prefix_t   p;
   tlp_completion c;
};

#endif
