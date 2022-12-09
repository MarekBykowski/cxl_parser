#/bin/tcsh

set run_root=/nfs/site/disks/xpg.sprmccfe.0019/${USER}/cosim/bhs-2022ww39.3/cadence/runs/pc5/pi5

# find the latest run
set run=`ls -t $run_root | grep cadence_simics_par | head -1`

echo Processing $run

set results_dir=logs_${USER}_bhs_cadence

# remove previous results
rm -rf $results_dir

set wc_file=cxl_relay_client_log.txt
mkdir -p ${results_dir}
if (-e ${CXL_RELAY_ROOT}/${wc_file}) then
    echo ${CXL_RELAY_ROOT}/${wc_file} does exists ... copying ...
    cp ${CXL_RELAY_ROOT}/${wc_file} ${results_dir}
else
    echo ${CXL_RELAY_ROOT}/${wc_file} does not exist
endif


make run_dir=$run_root/$run/pi5_base_test_restore_cxlio logs_dir=${results_dir}
make run_dir=$run_root/$run/pi5_base_test_restore_cxlio logs_dir=${results_dir}  analysis


