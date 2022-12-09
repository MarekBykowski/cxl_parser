#/bin/tcsh

set run_root=/nfs/site/disks/xpg.sprmccfe.0019/${USER}/cosim/qsp/cadence/runs/pc5/pi5

# find the latest run
set run=`ls -t $run_root | grep restore_cxl_relay | head -1`

set results=logs_${USER}_qsp_cadence

# remove previous results
rm -rf $results

make run_dir=$run_root/$run/pi5_base_test_restore_cxlio logs_dir=${results}
