#include "cxl_cm.h"

// Constructor
cxl_cm::cxl_cm() {}

void cxl_cm::proc_cxl_cm(const char* direction, unsigned long long timestamp, unsigned sid, unsigned *flit)
{
   cm_t      cm;
   
   // stp
   cm.dw[0] = flit[0];

   if (cm.f.flit_type_p0c1)
   {
      // make a local copy of the data
      memcpy(cm.dw, flit, sizeof(unsigned)*FLIT_DW);

      switch (cm.f.llctrl)
      {
         case RETRY:
            switch(cm.f.sub_type)
            {
               case RETRY_IDLE:
                  cout << direction << "  " << dec << timestamp << hex << " LLCTRL  RETRY-IDLE  ";
                  break;
               case RETRY_REQ:
                  cout << direction << "  " << dec << timestamp << hex << "  " << cm.rr;
                  break;
               case RETRY_ACK:
                  cout << direction << "  " << dec << timestamp << hex << "  " << cm.ra;
                  break;
               case RETRY_FRM:
                  cout << direction << "  " << dec << timestamp << hex << " LLCTRL  RETRY-FRAME (5x reqd before retry.req or retry.ack)";
                  break;
               default:
                  cout << direction << "  " << dec << timestamp << hex << " LLCTRL  RETRY-Reserved";
                  break;
            }
            break;
         case LLCRD:
            cout << direction << "  " << dec << timestamp << hex << " " << cm.c;
            break;
         case INIT:
            cout << direction << "  " << dec << timestamp << hex << " " << cm.i;
            break;
         case IDE:
            cout << direction << "  " << dec << timestamp << hex << " " << cm.s;                // security info
            break;
         default:
            cout << direction << "  " << dec << timestamp << hex << "  " << "LLCTRL - Reserved";
            break;
      }

      cout << endl;
   }
}
