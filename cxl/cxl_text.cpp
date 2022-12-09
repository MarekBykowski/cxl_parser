const char *fmt_text[8] = {
   " H3DW-READ",
   " H4DW-READ",
   " H3DW-WRITE",
   " H4DW-WRITE",
   " TLP Prefix",
   " Undefined",
   " Undefined",
   " Undefined"
};

// type[2:0]
const char *msg_type[] = {
   " implicitly routed to RC",         
   " routed by address",
   " routed by ID ",
   " implicitly broadcast from RC",
   " local, terminate at receiver ",
   " gather and route to RC",
   " local",
   " local",
};

const char *type_text[32] = {
   " MemReq",                            // 0
   " MemReadLckReq",                     // 1
   " I/O-Req",                           // 2
   " Rsvd",                              // 3
   " CfgTyp0",                           // 4
   " CfgTyp1",                           // 5
   " Rsvd",                              // 6
   " Rsvd",                              // 7

   " Rsvd",                              // 8
   " Rsvd",                              // 9
   " Cmpl",                              // 10  (0a) 
   " CmplLock",                          // 11  (0b)
   " AtomicReq",                         // 12  (0c)
   " AtomicReq",                         // 13  (0d)
   " AtomicReq",                         // 14  (0e)
   " Rsvd",                              // 15  (0f)

   " Msg",                               // 16  (10) implicitly routed to RC         
   " Msg",                               // 17  (11) routed by address
   " Msg",                               // 18  (12) routed by ID 
   " Msg",                               // 19  (13) implicitly broadcast from RC
   " Msg",                               // 20  (14) local, terminate at receiver 
   " Msg",                               // 21  (15) gather and route to RC
   " Rsvd",                              // 22  (16) local
   " Rsvd",                              // 23  (17) local
            
   " Rsvd",                              // 24  (18)
   " Rsvd",                              // 25  (19)
   " Rsvd",                              // 26  (1a)
   " DeprTLP-T4",                        // 27  (1b)
   " Rsvd",                              // 28  (1c)
   " Rsvd",                              // 29  (1d)
   " Rsvd",                              // 30  (1e)
   " Rsvd",                              // 31  (1f)
};

// PCIE Gen-5 spec: Table 2-36 Local TLP Prefix Types
const char *prefix_type_local[]={
   " MR-IOV", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " VendPrefixL0", " VendPrefixL1",
};

// PCIE Gen-5 spec: Table 2-37 End-End TLP Prefix Types
const char *prefix_type_end2end[]={
   " Extd-TPH", " PASID", " IDE-PREFIX", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " Rsvd", " VendPrefixE0", " VendPrefixE1",
};   

const char *cpl_status[]={
   "SC", "UR", "CRS", "CA", " Rsvd", " Rsvd", " Rsvd", " Rsvd",
};

const char *fc_type[]={
   "Reserved        ",                   // 0
   "Reserved        ",                   // 1
   "Reserved        ",                   // 2
   "Reserved        ",                   // 3
   "DLLP InitFC1-P  ",                   // 4
   "DLLP InitFC1-NP ",                   // 5
   "DLLP InitFC1-C  ",                   // 6
   "Reserved        ",                   // 7
   "DLLP UpdateFC-P ",                   // 8
   "DLLP UpdateFC-NP",                   // 9
   "DLLP UpdateFC-C ",                   // 10
   "Reserved        ",                   // 11
   "DLLP InitFC2-P  ",                   // 12
   "DLLP InitFC2-NP ",                   // 13
   "DLLP InitFC2-C  ",                   // 14
   "Reserved        ",                   // 15
};

// Request/Status Type
// 1: Virtual LSM Request ALMP
// 0: Virtual LSM Status ALMP
const char * almp_virtual_lsm_state[][16]={  // Table 62, CXL2.0 Spec: Virtual LSM State Encoding:
   {
   "Reset       ",                          // 0000: Reset (for Status ALMP)  
   "ACTIVE      ",                          // 0001: ACTIVE                                                                           
   "Reserved    ",                          // 0010: Reserved                                                                         
   "Reserved    ",                          // 0011: Reserved (for Status ALMP)   
   "IDLE_L1.0   ",                          // 0100: IDLE_L1.0 (maps to PCIe L1)                                                      
   "IDLE_L1.1   ",                          // 0101: IDLE_L1.1 (reserved for future use)                                              
   "IDLE_L1.2   ",                          // 0110: IDLE_L1.2 (reserved for future use)                                              
   "IDLE_L1.3   ",                          // 0111: IDLE_L1.3 (reserved for future use)                                              
   "L2          ",                          // 1000: L2                                                                               
   "Reserved    ",                          // 1001: Reserved                                                                         
   "Reserved    ",                          // 1010: Reserved                                                                         
   "Retrain     ",                          // 1011: Retrain (for Status ALMP only)
   "Reserved    ",                          // 1100: Reserved                                                                         
   "Reserved    ",                          // 1101: Reserved                                                                         
   "Reserved    ",                          // 1110: Reserved                                                                         
   "Reserved    ",                          // 1111: Reserved
   },
   {
   "Reserved    ",                          // 0000: Reserved (for Request ALMP)                       
   "ACTIVE      ",                          // 0001: ACTIVE                                                                           
   "Reserved    ",                          // 0010: Reserved                                                                         
   "Deepest PM  ",                          // 0011: DEEPEST ALLOWABLE PM STATE (for Request ALMP)
   "IDLE_L1.0   ",                          // 0100: IDLE_L1.0 (maps to PCIe L1)                                                      
   "IDLE_L1.1   ",                          // 0101: IDLE_L1.1 (reserved for future use)                                              
   "IDLE_L1.2   ",                          // 0110: IDLE_L1.2 (reserved for future use)                                              
   "IDLE_L1.3   ",                          // 0111: IDLE_L1.3 (reserved for future use)                                              
   "L2          ",                          // 1000: L2                                                                               
   "Reserved    ",                          // 1001: Reserved                                                                         
   "Reserved    ",                          // 1010: Reserved                                                                         
   "Reserved    ",                          // 1011: Reserved (for Request ALMP)                 
   "Reserved    ",                          // 1100: Reserved                                                                         
   "Reserved    ",                          // 1101: Reserved                                                                         
   "Reserved    ",                          // 1110: Reserved                                                                         
   "Reserved    ",                          // 1111: Reserved
   }
} ;


// CM
const char* retry_sub_type[]={
   "Retry.Idle ",
   "Retry.Req  ",
   "Retry.Ack  ",
   "Retry.Frm  ",
   "Rsvd       ","Rsvd       ","Rsvd       ","Rsvd       ",
   "Rsvd       ","Rsvd       ","Rsvd       ","Rsvd       ",
   "Rsvd       ","Rsvd       ","Rsvd       ","Rsvd       ",
} ;
   
// IDE
const char* ide_sub_type[]={
   "IDE-IDLE   ",
   "IDE-START  ",
   "IDE-TMAC   ",
   "Rsvd       ",   "Rsvd       ",   "Rsvd       ",
   "Rsvd       ",   "Rsvd       ",   "Rsvd       ",
   "Rsvd       ",   "Rsvd       ",   "Rsvd       ",
   "Rsvd       ",   "Rsvd       ",   "Rsvd       ",
};
