check_defined = \
    $(strip $(foreach 1,$1, \
        $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
      $(error Undefined $1$(if $2, ($2))))

LOG?=../hiop/logs_sapai_bhs_cadence/hiop.log
INPUT_LOG=$(shell readlink -f $(LOG))
OUTPUT_LOG=$(shell basename $(LOG))

all: build install

build:
	@make -C c-files

install:
	cp c-files/ec_walker bin

python:
	$(call check_defined, LOG)
	cd python-files; ./parse_log.py $(INPUT_LOG) ../dumps/$(OUTPUT_LOG).bin

clean:
	@make -C c-files clean
	rm -rf bin/*

.PHONY: all build clean python
