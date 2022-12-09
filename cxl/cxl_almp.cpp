#include "cxl_almp.h"

cxl_almp::cxl_almp() {};

void cxl_almp::proc_cxl_almp(const char* direction, unsigned long long timestamp, unsigned sid, unsigned *flit)
{
   
   // make a local copy of the data
   memcpy(almp.dw, flit, sizeof(unsigned)*ALMP_REPEAT);

   if ( (almp.dw[0] == almp.dw[1]) && (almp.dw[0] == almp.dw[2]) && (almp.dw[0] == almp.dw[3]) )
   {
      if (almp.f.token == ALMP_TOKEN)
         cout << almp;
      else
         cout << "Error: ALMP Incorrect token, correct= " << ALMP_TOKEN << " received= " << almp.f.token;
   }
   else
      cout << "Error: ALMP not repeated " << ALMP_REPEAT << " times "
           << almp.dw[0] << "_" << almp.dw[1] << "_" << almp.dw[2] << "_" << almp.dw[3];

   cout << endl;

}
