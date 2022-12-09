#!/bin/tcsh
#-------------------------------------
# Copy HIOP tracker file locally
#-------------------------------------

if (! $?CXL_RELAY_ROOT) then
   echo "please setenv CXL_RELAY_ROOT to cxl_relay directory"
   exit
endif

if (! $?logs_dir) then
    setenv logs_dir logs_${USER}
endif    
echo logs_dir is set to $logs_dir

mkdir -p ${logs_dir}

if (! $?run_dir) then
    echo environment variable run_dir is not set
    exit 0
endif    

echo run_dir is set to $run_dir

set hiop_tracker_file=HIOP.trk_soc_tb.pi5.pi5_tb_bind_wrapper_inst.gnrpc5_psf0_ti.psf0_pg0_agt_p0_ti_iosf.trk

if (-f ${logs_dir}/${hiop_tracker_file} ) then
    echo ${logs_dir}/${hiop_tracker_file} already exists, nothing to do!
    exit 0
endif

if (-f ${run_dir}/${hiop_tracker_file}.gz ) then
    cp ${run_dir}/${hiop_tracker_file}.gz ${logs_dir}
    gunzip ${logs_dir}/${hiop_tracker_file}.gz
else if (-f ${run_dir}/${hiop_tracker_file} ) then    
    cp ${run_dir}/${hiop_tracker_file} ${logs_dir}
else
    echo ${hiop_tracker_file} or ${hiop_tracker_file}.gz does not exist in ${run_dir}
    exit 1
endif
