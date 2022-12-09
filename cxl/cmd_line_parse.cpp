// INCLUDES
#include <iostream>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "limits.h"
#include "common.h"

// DEFINES
#define VERSION "$Revision: 1.0 $"
const char *inp_file = "flit.log";
unsigned verbose = 0x1F;                 // analyze all flits

// Print help message
void print_help(char *pgmname)
{
   fprintf(stderr,"\nThis is CXL flit processor. Usage:\n\n");
   fprintf(stderr,"\t%s [-i flit_input_file] [-h] [-V]\n", pgmname);
   fprintf(stderr,"  -i <flit_input_file>,  default: ./flit.log\n");
   fprintf(stderr,"  -v <log_level>, default=0x1F (0x%x: cxl_io_tlp, 0x%x: cxl_io_dllp, 0x%x: cxl_almp, 0x%x: cxl_cm_control, 0x%x: cxl_cm_protocol)\n",
           PROC_CXL_IO_TLP, PROC_CXL_IO_DLLP, PROC_CXL_ALMP, PROC_CXL_CM_CNTRL, PROC_CXL_CM_PROTOCOL);
   fprintf(stderr,"  -h print this help and exit\n");
   fprintf(stderr,"  -V print version and exit\n");
   fprintf(stderr,"\n");
   exit(1);
}

// Parse command line options
void cmd_line_parse(int argc, char *argv[]) 
{
   int opt;

   while((opt = getopt(argc, argv, "i:v:hV")) != -1) {
      switch(opt) {
         case 'i':
            inp_file = optarg;
            break;
         case 'h':
            print_help(argv[0]);
            break;
         case 'v':
            verbose = (unsigned) strtoul(optarg, (char **) NULL, 0); 
            fprintf(stderr,"%s: Verbose option is set with bitmask    = 0x%x\n", argv[0], verbose);
            fprintf(stderr,"0x%x: cxl_io_tlp, 0x%x: cxl_io_dllp, 0x%x: cxl_almp, 0x%x: cxl_cm_control, 0x%x: cxl_cm_protocol\n",
                    PROC_CXL_IO_TLP, PROC_CXL_IO_DLLP, PROC_CXL_ALMP, PROC_CXL_CM_CNTRL, PROC_CXL_CM_PROTOCOL);
            break;
         case 'V':
            fprintf(stderr,"%s %s\n\n", argv[0], VERSION); 
            exit(0);
            break;
         case ':':
            fprintf(stderr, "%s: Error - Option `%c' needs a value\n\n", argv[0], optopt);
            print_help(argv[0]);
            break;
         case '?':
            fprintf(stderr, "%s: Error - No such option: `%c'\n\n", argv[0], optopt);
            print_help(argv[0]);
      }
   }

   /*  print all remaining options */
   for(; optind < argc; optind++)
      fprintf(stderr,"argument: %s\n", argv[optind]);
}
