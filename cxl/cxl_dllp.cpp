#include "cxl_dllp.h"

cxl_dllp::cxl_dllp() {};

void cxl_dllp::proc_cxl_dllp(const char* direction, unsigned long long timestamp, unsigned sid, unsigned *flit)
{
   unsigned           index;
   
   // make a local copy of the data
   memcpy(dw, flit, sizeof(unsigned)*FLIT_DW);

   for (index=0; index<PAYLOAD_DW; )
   {
      dllp.dw[0] = dw[index++];
      dllp.dw[1] = dw[index++];
      
      if (dllp.fc.sdp_token == SDP_TOKEN)
      {
         if (dllp.r.type & FC)
            cout << dllp.fc;
         else if ((dllp.r.type == ACK) || (dllp.r.type == NAK))
            cout << dllp.an;
         else
            cout << dllp.r;

         cout << endl;
      }
   }
}
