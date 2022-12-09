#ifndef CXL_H
#define CXL_H

// Notes:
// Transmission in structs happens from most significant byte to least significant byte
// The structs are laid out lsb to msb
//-------------------------------------------------------------------------------------

// externs for strings
extern const char* fmt_text[];
extern const char* msg_type[];
extern const char* type_text[];
extern const char* prefix_type_local[];
extern const char* prefix_type_end2end[];
extern const char* cpl_status[];
extern const char* fc_type[];
extern const char* almp_virtual_lsm_state[][16];
extern const char* ide_sub_type[];

// CM
extern const char* retry_sub_type[];

// constants
const unsigned FLIT_BITS  = 528;
const unsigned FLIT_BYTES = FLIT_BITS/sizeof(unsigned char)/8;
const unsigned FLIT_DW    = (FLIT_BYTES+2)/sizeof(unsigned); // 2 for round-off
const unsigned PAYLOAD_DW = (FLIT_BYTES)/sizeof(unsigned);

// Stream IDs
const unsigned IO_SID   = 0x33;
const unsigned CM_SID   = 0x55;
const unsigned ALMP_SID = 0xff;

// masks
const unsigned MSG_MASK = 7;

// TLPs
typedef union start_tlp
{
   unsigned dw;

   struct 
   {
      unsigned tlp_seq_no  : 12;
      unsigned fcrc        : 4;
      unsigned tlp_len_h   : 7;
      unsigned parity      : 1;
      unsigned token       : 4;
      unsigned tlp_len_l   : 4;
   } f;

   friend ostream& operator<<(ostream& os, const start_tlp &v);
} stp_t;
inline ostream& operator<< (ostream& os, const start_tlp &v)
{
   os << " tlp_len = " << setw(4) << (v.f.tlp_len_h << 7 | v.f.tlp_len_l);
   os << " seq     = "     << setw(4) << v.f.tlp_seq_no;
   return os;
}

typedef union header_dw0
{
   unsigned dw;
   
   struct
   {
      unsigned data_len    : 10;
      unsigned at          : 2;
      unsigned attr_l      : 2;
      unsigned ep          : 1;
      unsigned td          : 1;
      unsigned th          : 1;
      unsigned ln          : 1;
      unsigned attr_h      : 1;
      unsigned tag8        : 1;
      unsigned tc          : 3;
      unsigned tag9        : 1;
      unsigned type        : 5;
      unsigned fmt         : 3;
   } f;
   friend ostream& operator<<(ostream& os, const header_dw0 &v);
   
} header_dw0_t;

inline ostream& operator<< (ostream& os, const header_dw0 &v)
{
   os << " data_len = " << setw(4) << v.f.data_len;
   os <<  setw(11) << fmt_text[v.f.fmt];
   os <<  setw(13) << type_text[v.f.type];
   return os;
}

typedef union cfg_adrs
{
   unsigned dw;
   
   struct
   {
      unsigned rsvd0      : 2;
      unsigned reg_adrs   : 10;
      unsigned rsvd1      : 4;
      unsigned func_num   : 3;
      unsigned dev_num    : 5;
      unsigned bus_num    : 8;
   } f;
   friend ostream& operator<<(ostream& os, const header_dw0 &v);
   
} cfg_adrs_t;

inline ostream& operator<< (ostream& os, const cfg_adrs &v)
{
   os << " DST-Bus:Dev:Fun = " << v.f.bus_num << " : " << v.f.dev_num << " : " << v.f.func_num;
   os << " register = " << setw(4) << v.f.reg_adrs;
   return os;
}


typedef union header_dw1
{
   unsigned dw;
   struct
   {
      unsigned first_be : 4;
      unsigned last_be  : 4;
      unsigned tag      : 8;
      unsigned req_id   : 16;
   } f;

   struct
   {
      unsigned msg_code : 8;
      unsigned tag      : 8;
      unsigned req_id   : 16;
   } m;
   
   friend ostream& operator<<(ostream& os, const header_dw1 &v);   
} header_dw1_t;

inline ostream& operator<< (ostream& os, const header_dw1 &v)
{
   os << " req_id = " << setw(4) << v.f.req_id;
   return os;
}

typedef union tlp_prefix
{
   unsigned dw;

   struct
   {
      unsigned contents : 24;
      unsigned type     : 4;
      unsigned l0e1     : 1;             // local prefix:0 , end2end prefix: 1
      unsigned fmt      : 3;
   } f;

   friend ostream& operator<<(ostream& os, const tlp_prefix &v);   

} tlp_prefix_t;
inline ostream& operator<< (ostream& os, const tlp_prefix &v)
{
   if (v.f.l0e1 == 0)
   {
      os << " prefix = " << setw(5) << "local";
      os << " type   = " << setw(8) << prefix_type_local[v.f.type];
   }
   else
   {
      os << " prefix = " << setw(5) << "e2e";
      os << " type   = " << setw(8) << prefix_type_end2end[v.f.type];
   }
   os << " prefix_content = " << setw(8) << hex << v.f.contents;
   
   return os;
}

typedef union tlp_completion
{
   unsigned dw[2];

   struct
   {
      unsigned byte_cnt : 12;
      unsigned bcm      : 1;
      unsigned status   : 3;
      unsigned cpl_id   : 16;
      
      unsigned low_adrs : 7;
      unsigned rsvd     : 1;
      unsigned tag      : 8;
      unsigned req_id   : 16;
   } f;
      
   friend ostream& operator<<(ostream& os, const tlp_completion &v);
      
} tlp_completion_t;
inline ostream& operator<< (ostream& os, const tlp_completion &v)
{
   os << hex;
   os << " status   = " << setw(5) << cpl_status[v.f.status];
   os << " byte_cnt = " << setw(8) << v.f.byte_cnt;
   os << " cpl_id   = " << setw(4) << v.f.cpl_id;
   os << " req_id   = " << setw(4) << v.f.req_id;
   os << " low_adrs = " << setw(4) << v.f.low_adrs;
   
   return os;
}

// tokens
const unsigned STP_TOKEN = 0xF;
const unsigned SDP_TOKEN = 0xF0AC;

// dllp types
const unsigned FC  = 0xc0;               // any bit is 1
const unsigned ACK = 0x00;
const unsigned NAK = 0x01;
const unsigned DLF = 0x02;               // data link feature
const unsigned PM  = 0x20;               // power management
const unsigned NOP = 0x31;
const unsigned VDD = 0x30;               // vendor defined dllp

typedef union
{
   unsigned dw;
   struct {
      unsigned       : 16;
      unsigned token : 16;               // 0xf0ac
   } f;
} sdp_t;

typedef struct flow_control 
{
   unsigned   hdr_credits_l: 6;
   unsigned   hdr_scale    : 2;
   unsigned   vc           : 3;
   unsigned   zero         : 1;          // this bit is zero for FC packets
   unsigned   type         : 4;
   unsigned   sdp_token    : 16;

   unsigned   crc          : 16;
   unsigned   data_credits : 12;
   unsigned   data_scale   : 2;
   unsigned   hdr_credits_h: 2;          // break it to align with 32-bit boundary

   friend ostream& operator<<(ostream& os, const flow_control &v);      

} flow_control_t;

inline ostream& operator<< (ostream& os, const flow_control &v)
{
   os << dec;
   os << fc_type[v.type];
   os << " VC           = " << setw(2) << v.vc;
   os << " hdr_scale    = " << setw(4) << v.hdr_scale;
   os << " hdr_credits  = " << setw(4) << (v.hdr_credits_h << 6 | (v.hdr_credits_l) );
   os << " data_scale   = " << setw(4) << v.data_scale;
   os << " data_credits = " << setw(4) << v.data_credits;
   os << hex << " crc   = " << setw(4) << v.crc;
   return os;
}

typedef struct ack_nak
{
   unsigned   reserved_l   : 8;
   unsigned   type         : 8;
   unsigned   sdp_token    : 16;

   unsigned   crc          : 16;
   unsigned   seq_number   : 12;
   unsigned   reserved_h   : 4;

   friend ostream& operator<<(ostream& os, const ack_nak &v);            
} ack_nak_t;
inline ostream& operator<< (ostream& os, const ack_nak &v)
{
   os << dec;
   if (v.type              == ACK) 
      os << " ACK ";
   else if (v.type         == NAK)
      os << " NAK ";      
   os << " sequence_number  = " << v.seq_number;
   os << hex << " crc       = " << setw(4) << v.crc;   
   return os;
}

typedef struct rest
{
   unsigned   contents_l   : 8;
   unsigned   type         : 8;
   unsigned   sdp_token    : 16;

   unsigned   crc          : 16;
   unsigned   contents_h   : 16;

   friend ostream& operator<<(ostream& os, const rest &v);            
} rest_t;
inline ostream& operator<< (ostream& os, const rest &v)
{
   if (v.type == DLF) 
      os << " DLLP DLF ";
   else if ( (v.type & PM) == PM)
      os << " DLLP PM  ";
   else if (v.type         == NOP)
      os << " DLLP NOP ";
   else if (v.type         == VDD)
      os << " DLLP VDM ";
   os << hex;
   os << " Contents= " << ((v.contents_h << 16) | (v.contents_l));
   
   os << hex << " crc = " << setw(4) << v.crc;   
   return os;
}

typedef union dllp
{
   unsigned       dw[2];
   flow_control_t fc;
   ack_nak_t      an;
   rest_t         r;
} dllp_t;

const unsigned ALMP_TOKEN  = 0x08;
const unsigned ALMP_REPEAT = 4;
typedef union almp
{
   unsigned dw[ALMP_REPEAT];

   struct almp_encoding
   {
      unsigned  rsvd2      : 8;          // byte-3
      unsigned  token      : 8;          // is 0x08
      
      unsigned  vlsm_state : 4;          // byte-1
      unsigned  rsvd1      : 3;
      unsigned  s0r1       : 1;          

      unsigned  almp_type  : 4;          // byte-0
      unsigned  rsvd0      : 4;          
   } f;

   friend ostream& operator<<(ostream& os, const almp &v);            
} almp_t;

inline ostream& operator<< (ostream& os, const almp &v)
{
   if (v.f.almp_type == 1)
      os << " ALMP-IO ";
   else if (v.f.almp_type == 2)
      os << " ALMP-CM ";
   if (v.f.s0r1)
      os << " Request ";
   else
      os << " Status  ";

   os << " VLSM_state= " << almp_virtual_lsm_state[v.f.s0r1][v.f.vlsm_state];

   return os;
}

// CXL_CM Control Flits
// UP  484368505 55 00000001 00000031 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00004198

const unsigned LLCRD = 0x0;           // LLCRD Flit containing only link layer credit return and/or Ack information, but no protocol information
const unsigned RETRY = 0x1;           // link layer retry flit
const unsigned IDE   = 0x2;           // IDE control messages (CXL2.0)
const unsigned INIT  = 0xc;           // Link layer initialization flit

const unsigned RETRY_IDLE = 0;
const unsigned RETRY_REQ  = 1;
const unsigned RETRY_ACK  = 2;
const unsigned RETRY_FRM  = 3;

typedef struct retry_req
{
   unsigned flit_type      : 1;
   unsigned rsvd0          : 7;
   unsigned rsvd1          : 24;

   unsigned llctrl         : 4;
   unsigned sub_type       : 4;
   unsigned zero           : 24;

   unsigned seq_num        : 8;
   unsigned                : 8;
   unsigned num_retry      : 5;
   unsigned num_phy_reinit : 5;
   unsigned                : 6;

   friend ostream& operator<<(ostream& os, const retry_req &v);

} retry_req_t;
inline ostream& operator<< (ostream& os, const retry_req &v)
{
   os << " LLCTRL  RETRY-REQ  ";
   os << " seq_num= " << setw(3) << v.seq_num;
   os << " num_retry= " << setw(3) << v.num_retry;
   os << " num_phy_reinit= " << setw(3) << v.num_phy_reinit;

   return os;
}
typedef struct retry_ack
{
   unsigned flit_type      : 1;
   unsigned rsvd0          : 7;
   unsigned rsvd1          : 24;

   unsigned llctrl         : 4;
   unsigned sub_type       : 4;
   unsigned zero           : 24;

   unsigned empty          : 1;
   unsigned viral          : 1;
   unsigned                : 1;
   unsigned echo_num_retry : 5;
   unsigned wr_ptr         : 8;
   unsigned echo_seq_num   : 8;
   unsigned num_free_buf   : 8;


   friend ostream& operator<<(ostream& os, const retry_ack &v);

} retry_ack_t;
inline ostream& operator<< (ostream& os, const retry_ack &v)
{
   os << "LLCTRL  RETRY-ACK ";
   os << " empty= " << v.empty;
   os << " viral= " << v.viral;
   os << " echo_num_retry= " << setw(3) << v.echo_num_retry;
   os << " wr_ptr= " << setw(3) << v.wr_ptr;
   os << " echo_seq_num= " << v.echo_seq_num;
   os << " num_free_buf= " << v.num_free_buf;
   return os;
}
typedef struct init
{
   unsigned flit_type      : 1;
   unsigned rsvd0          : 7;
   unsigned rsvd1          : 24;

   unsigned llctrl         : 4;
   unsigned sub_type       : 4;
   unsigned zero           : 24;

   unsigned cxl_version    : 4;
   unsigned                : 4;
   unsigned                : 5;
   unsigned                : 11;
   unsigned llr_wrap_value : 8;

   friend ostream& operator<<(ostream& os, const init &v);

} init_t;
inline ostream& operator<< (ostream& os, const init &v)
{
   os << "LLCTRL  INIT";
   os << " cxl_version= " << setw(4) << v.cxl_version;
   os << " llr_wrap_value= " << setw(4) << v.llr_wrap_value;
   return os;
}


typedef struct llcrd
{
   unsigned flit_type : 1;
   unsigned           : 1;
   unsigned ak_3      : 1;
   unsigned           : 5;
   unsigned           : 8;
   unsigned           : 4;
   unsigned rspcrd    : 4;
   unsigned reqcrd    : 4;
   unsigned datcrd    : 4;
   
   unsigned llctrl    : 4;
   unsigned sub_type  : 4;               // 0-N/A, 1-Acknowledge
   unsigned zero      : 24;

   unsigned ak_210    : 3;
   unsigned           : 3;
   unsigned ak_7654   : 4;
   unsigned           : 22;
   
   friend ostream& operator<<(ostream& os, const llcrd &v);

} llcrd_t;
inline ostream& operator<< (ostream& os, const llcrd &v)
{
   unsigned ack;
   
   os << "LLCTRL-CREDIT  ";
   if (v.sub_type)
   {
      ack = ( (v.ak_7654<<4) | (v.ak_3<<3) | (v.ak_210) );
      os << " Acknowledge:  Full_ack= " << setw(4) << ack;
      os << " rspcrd= " << setw(3) << v.rspcrd;
      os << " reqcrd= " << setw(3) << v.reqcrd;
      os << " datcrd= " << setw(3) << v.datcrd;
   }
   else
      os << " N/A ";

   return os;
}

typedef struct llide
{
   unsigned flit_type : 1;               // 0-protocol, 1-control
   unsigned           : 4;
   unsigned ctl_fmt   : 3;               // 001 - 96bit payload
   unsigned           : 8;               // rsvd
   unsigned           : 8;               // rsvd
   unsigned tmac0     : 8;               // first byte of 96 bits of tmac
   unsigned llctrl    : 4;               // 0010 - IDE control flit
   unsigned sub_type  : 4;               // 0: ide-idle, 1: ide-start, 2: ide-tmac, rest: rsvd

   unsigned tmac1     : 8;              // remaining 11 bytes of tmac (total: 96)
   unsigned tmac2     : 8;
   unsigned tmac3     : 8;
   unsigned tmac4     : 8;
   unsigned tmac5     : 8;
   unsigned tmac6     : 8;   
   unsigned tmac7     : 8;   
   unsigned tmac8     : 8;   
   unsigned tmac9     : 8;   
   unsigned tmac10    : 8;   
   unsigned tmac11    : 8;   

   friend ostream& operator<<(ostream& os, const llcrd &v);

} llide_t;

const unsigned IDE_TMAC=0x2;
// IDE constants
inline ostream& operator<< (ostream& os, const llide &v)
{
   char orig_fill_char;
   os << "LLCTRL-IDE     ";
   os << ide_sub_type[v.sub_type];
   if ( (v.ctl_fmt == 1) && (v.sub_type == IDE_TMAC) )
   {
      orig_fill_char = cout.fill('0');
      os << "MAC= " << hex
         << setw(2) << v.tmac0 << setw(2) << v.tmac1 << setw(2) << v.tmac2 << setw(2) << v.tmac3
         << setw(2) << v.tmac4 << setw(2) << v.tmac5 << setw(2) << v.tmac6 << setw(2) << v.tmac7
         << setw(2) << v.tmac8 << setw(2) << v.tmac9 << setw(2) << v.tmac10 << setw(2) << v.tmac11
         << dec;
      cout.fill(orig_fill_char);
   }

   return os;
}

typedef union
{
   unsigned dw[FLIT_DW];

   retry_req_t rr;
   retry_ack_t ra;

   llcrd_t c;
   llide_t s;                            // security feature
   init    i;

   struct
   {
      unsigned flit_type_p0c1 : 1;
      unsigned                :31;

      unsigned llctrl         : 4;
      unsigned sub_type       : 4;
   } f;
   
} cm_t;

#endif

