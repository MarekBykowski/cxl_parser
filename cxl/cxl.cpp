#include "common.h"
#include "cxl.h"
#include "cxl_io.h"
#include "cxl_dllp.h"
#include "cxl_almp.h"
#include "cxl_cm.h"

int get_flit (ifstream &flits, string &direction, unsigned long long &timestamp, unsigned &sid, unsigned char *bytes)
{
   string line;
   string flit;
   long int cc;
   unsigned char * cptr = bytes + FLIT_BYTES - 1; // FLIT_BYTES = 66 (528 bits), bytes array is 68 chars or 17 words
   unsigned *dw = (unsigned *) bytes;
   typedef union
   {
      unsigned dw;
      unsigned char bytes[sizeof(unsigned)];
   } dw_byte_t;

   dw_byte_t x, y;
   bool ret_val;

   // Zero half word (528 bits = 16.5 words, bytes array holds 17 words, so zero two bytes so the data is not garbage)
   *(cptr+1) = 0;  // 67th byte counting from 1
   *(cptr+2) = 0;  // 68th byte counting from 1
   
   if ( getline(flits, line))
   {
      istringstream iss(line);

      ret_val = false;
      ret_val = (iss >> direction);
      ret_val = (iss >> dec >> timestamp);
      ret_val = (iss >> hex >> sid);
      ret_val = (iss >> flit);
      
      if (ret_val == false)
         return 0;

      unsigned length = flit.length();
      for (unsigned i=0; i<length; i+=2)
      {
         string byte = flit.substr(i, 2); // process upper/lower nibble in a byte
         cc  = strtol(byte.c_str(), NULL, 16);
         *cptr-- = (unsigned char) cc;
      }

      if (sid == IO_SID)
      {
         for (unsigned i=0; i<FLIT_DW; i++)
         {
            x.dw = dw[i];
            y.bytes[0] = x.bytes[3];
            y.bytes[1] = x.bytes[2];
            y.bytes[2] = x.bytes[1];
            y.bytes[3] = x.bytes[0];
            dw[i] = y.dw;
         }
      }
         
      return 1;
   }
   else
      return 0;
}

void print_flit(const char *direction, unsigned long long timestamp, unsigned sid, unsigned *dw, unsigned length)
{
   char fill_char;
   unsigned i;
   
   cout << endl;
   cout << direction << " " << dec << setw(10) << timestamp << " " << hex << setw(2) << sid;
   fill_char = cout.fill('0'); 
   for (i=0; i<length; i++)
      cout << " " << hex << setw(8) << dw[i];
   cout.fill(fill_char);

   cout << endl;
}


int main(int argc, char **argv)
{
   string   direction;                   // UP/DN
   unsigned long long timestamp;
   unsigned sid;                         // stream id
   ifstream flits;
   ofstream report;
   unsigned flit[FLIT_DW];
   
   // classes
   cxl_io   io;
   cxl_dllp dllp;
   cxl_almp almp;
   cxl_cm   cm;
   
   // vars
   sdp_t d;
   stp_t s;
   
   // process command line options
   cmd_line_parse(argc, argv);

   // open the files
   open_file(flits, inp_file);

   // process data
   while ( get_flit(flits, direction, timestamp, sid, (unsigned char*) flit) )
   {
      if (sid == IO_SID)
      {
         d.dw = flit[0];
         if (d.f.token == SDP_TOKEN)
         {
            if (verbose & PROC_CXL_IO_DLLP)
            {
               print_flit(direction.c_str(), timestamp, sid, flit, FLIT_DW);
               dllp.proc_cxl_dllp(direction.c_str(), timestamp, sid, flit);
            }
            int index = sizeof(dllp_t)/sizeof(unsigned); // dllp size = 2 DW or 64 bits
            s.dw = flit[index]; 
            if (s.f.token == STP_TOKEN)
            {
               if (verbose & PROC_CXL_IO_TLP)
               {
                  print_flit(direction.c_str(), timestamp, sid, &flit[index], FLIT_DW-index);
                  io.proc_cxl_io(direction.c_str(), timestamp, sid, &flit[index], FLIT_DW-index);
               }
            }
         }
         else
         {
            if ( verbose & PROC_CXL_IO_TLP)
            {
               print_flit(direction.c_str(), timestamp, sid, flit, FLIT_DW);            
               io.proc_cxl_io(direction.c_str(), timestamp, sid, flit, FLIT_DW);
            }
         }
      }
      else if (sid == ALMP_SID) 
      {
         if (verbose & PROC_CXL_ALMP)
         {
            print_flit(direction.c_str(), timestamp, sid, flit, FLIT_DW);         
            almp.proc_cxl_almp(direction.c_str(), timestamp, sid, flit);
         }
      }
      else if (sid == CM_SID)
      {
         if  (verbose & (PROC_CXL_CM_CNTRL | PROC_CXL_CM_PROTOCOL) )
         {
            print_flit(direction.c_str(), timestamp, sid, flit, FLIT_DW);         
            cm.proc_cxl_cm(direction.c_str(), timestamp, sid, flit);
         }
      }
   }
    
   flits.close();

   return 0;
}
