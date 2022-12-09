#-------------------------------------------
# Run as: gdb -x tb.gdb
#-------------------------------------------
# Load executable  (can't use env variables) so use the following workaround
shell rm -rf tmp.scr
shell echo file ${PWD}/scratch/bin/cxl               >> tmp.scr
shell echo b main                                    >> tmp.scr
#shell echo run -i ${PWD}/oink                       >> tmp.scr
shell echo run -i ${PWD}/pc5_cxl_io_rx_tx_flit.log   >> tmp.scr
#shell echo run -i ${PWD}/flit.log                   >> tmp.scr
source tmp.scr
shell rm -rf tmp.scr
