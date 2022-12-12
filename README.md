To run it do:

# cd utilities/

# find ../ -name hiop.log
../hiop/logs_sapai_qsp_marvell/hiop.log  
../hiop/logs_sapai_bhs_cadence/hiop.log  

# make python LOG=../hiop/logs_sapai_bhs_cadence/hiop.log
cd python-files; ./parse_log.py /home/xxbykowm/repos/cxl_sw_gold/hiop/logs_sapai_bhs_cadence/hiop.log ../dumps/hiop.log.bin    
addr: 0x0 val: 0x10017cd b'\xcd\x17\x00\x01'                                                                                   
addr: 0xc8 val: 0x0 b'\x00\x00\x00\x00'                                                                                        
addr: 0x190 val: 0x0 b'\x00\x00\x00\x00'        
addr: 0x258 val: 0x0 b'\x00\x00\x00\x00'                                                                              
addr: 0x320 val: 0x0 b'\x00\x00\x00\x00'       

# make

# ./bin/ec_walker ./dumps/hiop.log.bin
# List of all PCIe Extended Capabilites                                                                                                  
        EC[1] -> Advanced Error Reporting                                                                                                
        EC[2] -> Virtual Channel Capability                                                                                              
        EC[3] -> Device Serial Number                                                                                                    
        EC[4] -> Power Budgeting                                                                                                         
        EC[5] -> Root Complex Link Declaration                                                                                           
        EC[6] -> Root Complex Internal Link Control                                                                                      
        EC[7] -> Root Complex Event Collector                                                                                            
        EC[8] -> Multi-Function VC Capability                                                                                            
        EC[9] -> same as _VC                                                                                                             
        EC[a] -> Root Complex RB?                                                                                                        
        EC[b] -> Vendor-Specific                                                                                                         
        EC[c] -> Config Access - obsolete                                                                                                
        EC[d] -> Access Control Services                                                                                                 
        EC[e] -> Alternate Routing ID                                                                                                    
        EC[f] -> Address Translation Services                                                                                            
        EC[10] -> Single Root I/O Virtualization                                                                                         
        EC[11] -> Multi Root I/O Virtualization                                                                                          
        EC[12] -> Multicast                                                                                                              
        EC[13] -> Page Request Interface                                                                                                 
        EC[14] -> Reserved for AMD                                                                                                       
        EC[15] -> Resizable BAR                                                                                                          
        EC[16] -> Dynamic Power Allocation                                                                                               
        EC[17] -> TPH Requester                                                                                                          
        EC[18] -> Latency Tolerance Reporting                                                                                            
        EC[19] -> Secondary PCIe Capability                                                                                              
        EC[1a] -> Protocol Multiplexing                                                                                                  
        EC[1b] -> Process Address Space ID                                                                                               
        EC[1d] -> Downstream Port Containment                                                                                            
        EC[1e] -> L1 PM Substates                                                                                                        
        EC[1f] -> Precision Time Measurement                                                                                             
        EC[23] -> Designated Vendor-Specific                                                                                             
        EC[25] -> Data Link Feature                                                                                                      
        EC[26] -> Physical Layer 16.0 GT/s                                                                                               
        EC[2e] -> Data Object Exchange                                                                                                   
                                                                                                                                                                       
# Search for all the ECs for vendor:device "17cd:0100" from mem-dumped config space "./dumps/hiop.log.bin"                               
        EC[1] -> Advanced Error Reporting @ 100                                                                                          
                 15020001 @ 100                                                                                                          
                 0 @ 104                                                                                                                 
        EC[2] -> Virtual Channel Capability @ 4c0                                                                                        
                 5c010002 @ 4c0                                                                                                          
                 0 @ 4c4                                                                                                                 
        EC[3] -> Device Serial Number @ 150                                                                                              
                 16010003 @ 150                                                                                                          
                 0 @ 154                                                                                                                 
        EC[4] -> Power Budgeting @ 160                                                                                                   
                 1b810004 @ 160                                                                                                          
                 0 @ 164                                                                                                                 
        EC[b] -> Vendor-Specific @ 400                                                                                                   
                 4401000b @ 400                                                                                                          
                 1010001 @ 404                                                                                                           
        EC[f] -> Address Translation Services @ 5c0                                                                                      
                 6401000f @ 5c0                                                                                                          
                 61 @ 5c4                                                                                                                
        EC[13] -> Page Request Interface @ 640                                                                                           
                 90010013 @ 640                                                                                                          
                 81000000 @ 644                                                                                                          
        EC[16] -> Dynamic Power Allocation @ 1c0                                                                                         
                 30010016 @ 1c0                                                                                                          
                 8100007 @ 1c4                                                                                                           
        EC[18] -> Latency Tolerance Reporting @ 1b8
                 1c010018 @ 1b8              
                 0 @ 1bc                          
        EC[19] -> Secondary PCIe Capability @ 300
                 40010019 @ 300           
                 0 @ 304                     
        EC[1b] -> Process Address Space ID @ 440
                 4c01001b @ 440             
                 1406 @ 444                 
        EC[1e] -> L1 PM Substates @ 900 
                 9101001e @ 900           
                 28001f @ 904           
        EC[1f] -> Precision Time Measurement @ a20                                                                    
                 a701001f @ a20                                                                                       
                 11 @ a24                                                                                             
        EC[23] -> Designated Vendor-Specific @ e00                                                                    
                header e4010023                                                                                       
                vendorID 1e98                                                                                         
                revisionID 1                                                                                                             
                length 38                                                                                             
                ID 0                                                                                                  
                cap c05f                                                                                              
                ctrl 0                                                                                                                   
                range[0]: start-end 0xdeadbeef aka later :-)                       
                range[1]: start-end 0xdeadbeef aka later :-)                                                          
              EC[23] -> Designated Vendor-Specific @ e40
              header e6010023
              vendorID 1e98
              revisionID 1
              length 14
              ID 7
              cap 27
              ctrl 27
              range[0]: start-end 0xdeadbeef aka later :-)
              range[1]: start-end 0xdeadbeef aka later :-)
      EC[23] -> Designated Vendor-Specific @ e60
              header 10023
              vendorID 1e98
              revisionID 0
              length 24
              ID 8
              cap 0
              ctrl 100
              range[0]: start-end 0xdeadbeef aka later :-)
              range[1]: start-end 0xdeadbeef aka later :-)
      EC[25] -> Data Link Feature @ 910
               92010025 @ 910
               80000001 @ 914
      EC[26] -> Physical Layer 16.0 GT/s @ 9c0
               a4010026 @ 9c0
               0 @ 9c4
      EC[2e] -> Data Object Exchange @ de0
              header e001002e
              cap 1
              ctrl 0
              status 1
              not touching read/write as it will affect dev
