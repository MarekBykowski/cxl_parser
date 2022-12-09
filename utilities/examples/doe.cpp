#include <iostream>
#include <iomanip>
using namespace std;



//----------------------------  Can go in  pcie_cfg.h file ---------------------------------------------------
// List of extended capabilities
const unsigned DOE_ID = 0x2E;            // 16*2+14 = 32+14 = 46

unsigned verbose = 3;

// Their names
const char *ec_id[] = {

   " ZERO",                              // 0th ID doesn't exist
   
// TBP: To be populated   
   " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP",
   " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP",
   " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " EC_DOE", " TBP", " TBP", // <-- 46th element
   " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP", " TBP",
};

typedef union ec_header
{
   struct
   {
      unsigned cap_id      : 16;
      unsigned cap_ver     : 4;
      unsigned next_offset : 12;
   } f;

   unsigned dw;

   friend ostream& operator<<(ostream& os, const ec_header &v);

} ec_header_t;

inline ostream& operator<< (ostream& os, const ec_header &v)
{
   if (verbose & 1)
      os << " cap_id     = " << setw(3) << hex << v.f.cap_id << ec_id[v.f.cap_id];

   if (verbose & 2)
   {
      os << " cap_ver    = " << setw(3) << v.f.cap_ver;
      os << " next_offset= " << setw(3) << v.f.next_offset;
   }

   return os;
}


// doe extended capability
typedef union ec_doe
{
   struct doe_fields
   {
      ec_header_t h0;                    // common ec header

      struct doe_cap_register
      {
         unsigned intr_n0y1 : 1;         // itr support, 0: no, 1: yes support
         unsigned message   : 10;        // msi/msx interrupt vector 
         unsigned           : 20;
      } h1;

      struct doe_control_register
      {
         unsigned abort     : 1;
         unsigned int_en    : 1;
         unsigned           : 29;
         unsigned go        : 1;            
      } h2;

      struct doe_status_register
      {
         unsigned busy      : 1;
         unsigned int_sts   : 1;
         unsigned error     : 1;
         unsigned           : 28;
         unsigned ready     : 1;
      } h3;

      unsigned wr_data_reg;

      unsigned rd_data_reg;

   } f;
   

   unsigned dw[sizeof(doe_fields)];

   friend ostream& operator<<(ostream& os, const ec_doe &v);
   
} ec_doe_t;

inline ostream& operator<< (ostream& os, const ec_doe &v)
{
   // EC 
   os << v.f.h0 << endl;

   if (verbose & 2)
   {
      // Doe Registers
      os << setw(20) << "intr_support= " << v.f.h1.intr_n0y1 << endl;
      os << setw(20) << "intr_vector = " << v.f.h1.message   << endl;

      os << setw(20) << "doe_abort= " << v.f.h2.abort << endl;
      os << setw(20) << "doe_int_en= " << v.f.h2.int_en  << endl;
      os << setw(20) << "doe_go= " << v.f.h2.go;
   }

   return os;
}


//-------------------------- can go in pcie_cfg_parser.cpp -----------------------------------------
// address = de0 RD-data= e001002e
// address = de4 RD-data= 1
// address = de8 RD-data= 0
// address = de4 RD-data= 1

int main(void)
{
   // data stream from the log
   unsigned data[] = {0xe001002e, 1, 0};
   
   ec_header_t ec_hdr;
   ec_doe_t    ec_doe;

   // Start interpreting the log
   ec_hdr.dw = data[0];                     

   // Test which capability it is first
   if (ec_hdr.f.cap_id == DOE_ID)
   {

      if (verbose)
      {
         cout << ec_hdr << endl;

         // interpret rest of the data based on the offset
         ec_doe.dw[0] = data[0];
         ec_doe.dw[1] = data[1];
         ec_doe.dw[2] = data[2];

         cout << ec_doe << endl;
      }

   }

   return 0;
}
