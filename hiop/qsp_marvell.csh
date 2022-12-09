#/bin/tcsh
set run_root=/nfs/site/disks/xpg.sprmccfe.0019/${USER}/cosim/qsp/marvell/runs/pc5/pi5_ser

# find the latest run
set run=`ls -t $run_root | grep marvell_simics_ser.list | head -1`

echo Processing $run

set results_dir=logs_${USER}_qsp_marvell

set wc_file=cxl_relay_server_log.txt
mkdir -p ${results_dir}
if (-e ${CXL_RELAY_ROOT}/${wc_file}) then
    echo ${CXL_RELAY_ROOT}/${wc_file} does exist ... copying ...
    cp ${CXL_RELAY_ROOT}/${wc_file} ${results_dir}
else
    echo ${CXL_RELAY_ROOT}/${wc_file} does not exist
endif

set run_directory=$run_root/$run/pi5_base_test_restore_cxlio_fsdb_fsdb

echo run_dir will be set to $run_directory
echo logs_dir will be set to $results_dir

make run_dir=$run_directory logs_dir=$results_dir wc_file=cxl_relay_server_log.txt analysis

