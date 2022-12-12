all: build install

build:
	@make -C c-files

install:
	@cp c-files/ec_walker bin

python_parse:
	@cd python-files; ./parse_log.py

clean:
	@make -C c-files clean

.PHONY: all build clean
