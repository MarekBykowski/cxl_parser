SHELL=tcsh

flit_file ?= ${PWD}/flit.log

# Compile flags
CXXFLAGS  = -g -Wall -Wno-long-long -Wno-deprecated -fexceptions

# main_src has main(), the executable will have the same name wo extension
main_src  = cxl.cpp

# Supporting sources
rest_src  = cxl_text.cpp cxl_io.cpp cxl_dllp.cpp cxl_almp.cpp cxl_cm.cpp \
	cmd_line_parse.cpp utl_misc.cpp  

# where to build
bld := ${PWD}/scratch

# This file has all the build and run  rules
include cpp.mk

# emacs: esc-x => gdb -i=mi -x cmds.gdb
