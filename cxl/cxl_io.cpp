#include "cxl_io.h"

// Constructor
cxl_io::cxl_io() {}

void cxl_io::print_data(unsigned *dw, unsigned &index)
{
   unsigned i;
   char     fill_char;
   
   cout << " payload[0:" << data_length-1 << "]= ";
   fill_char = cout.fill('0');
   for (i= 0; (i<data_length) && (index < PAYLOAD_DW); i++)
   {
      cout << setw(8) << htobe32(dw[index++]);
      if (i < data_length-1)
         cout << "_";
   }

   cout.fill(fill_char);
}

void cxl_io::print_address(unsigned long_address, unsigned *dw, unsigned &index)
{
   unsigned lo_adrs;
   unsigned hi_adrs;
   char     fill_char;

   fill_char = cout.fill('0');
   if (long_address)
   {
      hi_adrs = dw[index++];
      lo_adrs = dw[index++];
      cout << " ADRS= " <<  setw(8) << hi_adrs << "_" << lo_adrs;
   }
   else
   {
      lo_adrs = dw[index++];
      cout << " ADRS= " <<  setw(9) << " " << setw(8) << lo_adrs;      
   }
   cout.fill(fill_char);
}

void cxl_io::proc_cxl_io(const char* direction, unsigned long long timestamp, unsigned sid, unsigned *flit, unsigned length)
{
   unsigned           i;
   unsigned index;
   unsigned tlp_tag;
   
   // make a local copy of the data
   memcpy(dw, flit, sizeof(unsigned)*length);

   // stp
   // Raw Flit: TX 117069000000 33 000000000000000000000000000000000000aa9b533bbbbbbbbbbbbbbbbb00b7802000010000cf0200ff0220006003e0008f00000000000000000000000000000000     // Reverse : TX 117069000000 33 00000000 00000000 00000000 00000000 8f00e003 60002002 ff0002cf 00000100 2080b700 bbbbbbbb bbbbbbbb 3b539baa 00000000 00000000 00000000 00000000 00000000

   index = 0;
   do 
   {
      stp.dw = dw[index++];
      if (stp.dw == 0)                   // skip all 0 words
         continue;
      else
         break;
   } while (index < length);

   if (index == length)
      cout << "Strange:  All zero flit observed for CXL.io" << endl;
   else
   {
      if (stp.f.token == 0xf)
      {
         tlp_length = (stp.f.tlp_len_h << 7 | stp.f.tlp_len_l);

         // start of tlp token
         cout << stp;

         // process prefixes
         for (i=0; i<tlp_length; i++)
         {
            p.dw = dw[index];
            if (p.f.fmt == 0x4)
            {
               cout << p << endl;
               index++;
            }
            else
               break;
         }

         // proces transactions
         h0.dw          = dw[index++];
         cout << h0;
         data_length    = h0.f.data_len;
         if (data_length == 0)
            data_length = 1024;

         if ( (h0.f.type == 0xa)  || (h0.f.type == 0xb) ) // completion
         {
            c.dw[0] = dw[index++];
            c.dw[1] = dw[index++];
            tlp_tag = (h0.f.tag9 << 9 | h0.f.tag8 << 8 | c.f.tag);   
         
            cout << c;
            cout << " tag = " << setw(4) << tlp_tag;
            if (h0.f.fmt & 2)
               print_data(dw, index);
         }
         else 
         {
            h1.dw         = dw[index++];
            cout << h1;
            tlp_tag = (h0.f.tag9 << 9 | h0.f.tag8 << 8 | h1.f.tag);
            cout << " tag = " << setw(4) << tlp_tag;

            if ( (h0.f.type & 0x18) == 0x10) // msg
            {
               cout << " msg_type = " << msg_type[h0.f.type & MSG_MASK];
               cout << " msg_code = " << setw(2) << h1.m.msg_code << (h1.m.msg_code == 0x7e ? " VDM Type-0 ":"");
               cout << " dw2      = " << setw(8) << dw[index++];
               cout << " dw3      = " << setw(8) << dw[index] << (dw[index]== 0x68 ? " CXL-PM " : "");
               index++;
               if (h0.f.fmt & 2)
                  print_data(dw, index);                  
            }
            else
            {
               if (h0.f.type == 0x4)     // config R/W
               {
                  cfg_adrs.dw = dw[index++];
                  cout << cfg_adrs;
                  print_data(dw,index);
               }
               else                      // Assume memory R/W
               {
                  print_address( (h0.f.fmt & 1), dw, index); // fmt conveys 3DW or 4DW TLP

                  if (h0.f.fmt & 2)      // write
                     print_data(dw, index);
               }
            }
         }
         if (h0.f.td)
            cout << " ECRC= " << dw[index++]; // ECRC - end-to-end CRC - calculated over header+data
         cout << " LCRC = " << dw[index++];  // LCRC - link level CRC - calculated over seq#+header+data+ECRC
         cout << endl;
      }
   }
}
