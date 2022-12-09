SHELL=tcsh

run_dir   ?= /nfs/site/disks/xpg.sprmccfe.0019/sapai/cosim/bhs-dhv-2022ww29.4/cadence/runs/pc5/pi5/restore_cxl_relay.list/pi5_base_test_restore_cxlio
logs_dir  ?= ${PWD}/logs_${USER}
hiop_file ?= HIOP.trk_soc_tb.pi5.pi5_tb_bind_wrapper_inst.gnrpc5_psf0_ti.psf0_pg0_agt_p0_ti_iosf.trk
wc_file   ?= cxl_relay_client_log.txt

ret := $(shell mkdir -p $(logs_dir))

analysis: copy hiop rt wc stat

.PHONY: copy hiop
copy:   ${logs_dir}/${hiop_file} 

${logs_dir}/${hiop_file}:
	setenv run_dir ${run_dir} && setenv logs_dir ${logs_dir} && hiop.csh
hiop:
	cd ${logs_dir} && ../hiop.pl ${hiop_file}         > hiop.log
rt:
	cd ${logs_dir} && ../rt_time.pl ${hiop_file}      > rt.log
wc:
	cd ${logs_dir} && ../wc_time.pl ${wc_file}        > wc.log
stat:
	cd ${logs_dir} && ../rt_wc_stat.pl rt.log wc.log  > stat.log


test:
ifneq ( "$(wildcard ${logs_dir}/${wc_file})", "")
	echo ${logs_dir}/${wc_file} exists
else
	echo ${logs_dir}/${wc_file} does not exist
endif

ultraclean:
	rm -rf ${logs_dir}
clean:
	rm -rf ${logs_dir}/hiop.log


