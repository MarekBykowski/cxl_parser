# Where to build
obj_dir = ${bld}/obj
bin_dir = ${bld}/bin
dep_dir = ${bld}/dep

# Set the default compile and link variables (use -pg for profiling)
CC        = g++

# All source files
all_src   = $(main_src) $(rest_src)

# main_src has main(), rest are supporting sources 
obj_files = $(all_src:%.cpp=$(obj_dir)/%.o)
dep_files = $(all_src:%.cpp=$(dep_dir)/%.d)

# target name is same as main_src name without extn
TARGET   = $(main_src:%.cpp=$(bin_dir)/%)

# Standard rules modified for directing output to directories
$(TARGET): $(obj_files)
	mkdir -p $(bin_dir)
	$(LINK.cpp) $^ $(LDLIBS) -o $@

# Rule to make dependency files
$(dep_dir)/%.d: %.cpp
	@mkdir -p $(dep_dir)
	@rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(obj_dir)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# compile
$(obj_dir)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) ${CXXFLAGS} -c -o $@ $<

#$(obj_dir)/%.o: %.cpp
#	@mkdir -p $(obj_dir)
#	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# Build shared object library for common files if one doesn't exist
$(lib_dir)/libcommon.so: FORCE
	@cd $(libcpp) && $(MAKE)
FORCE:

# Targets which will execute even if there is a file by the same name
.PHONY: clean ultraclean help run

# Run
run:
	${TARGET} -i ${flit_file} > report.log

# emacs: esc-x => gdb -i=mi -x cmds.gdb
gdb:
	emacs -bg black -fg gray -l gdb_mi.el -f load-gdb

# Clean deletes object files
clean:
	rm -rf $(obj_files) $(dep_files) $(TARGETS)

# Ultraclean removes object directories
ultraclean:
	rm -rf $(lib_dir) $(bin_dir) $(obj_dir) $(dep_dir) $(cpp_doc)


# Don't generate or create dep files if the goal is to delete them
ifneq ($(MAKECMDGOALS), clean)
include $(dep_files)
endif

# Also useful for understanding the Makefile
help:
	@echo "Native Operating System: $(OS)"
	@echo "                    CXX: $(CXX)"
	@echo "         CXXFLAGS flags: $(CXXFLAGS)"
	@echo "         CPPFLAGS flags: $(CPPFLAGS)"
	@echo " Libraries to be linked: $(LDLIBS)"
	@echo "           Linker flags: $(LDFLAGS)"
	@echo "  Object file directory: $(obj_dir)"
	@echo "   Dependency directory: $(dep_dir)"	
	@echo "      Library directory: $(lib_dir)"
	@echo "      Current Directory: $(cwd)"
	@echo "---------------------------------------------------------"
	@echo "           Source files: $(all_src)"
	@echo "---------------------------------------------------------"
	@echo "           Object files: $(obj_files)"
	@echo "---------------------------------------------------------"
	@echo "       Dependency files: $(dep_files)"
	@echo "---------------------------------------------------------"
	@echo "       Main target name: $(TARGET)"
	@echo "---------------------------------------------------------"
	@echo "          Other targets: help, clean, ultraclean"
	@echo "---------------------------------------------------------"
	@echo " For make actions, type: make -n target_name"
	@echo "---------------------------------------------------------"
	@echo " The release directory: $(rel_dir)"
	@echo "---------------------------------------------------------"

# Finally make the release
release:
	mkdir -p $(rel_dir)
	cp $(TARGET) $(rel_dir)

# generate only html documentation. "doxygen -g" shows all defaults
doc:
	mkdir -p $(cpp_doc)
	@rm -rf $(cpp_doc)/Doxyfile
	@echo "PROJECT_NAME           = $(tb_name)"      >> $(cpp_doc)/Doxyfile
	@echo "OUTPUT_DIRECTORY       = $(cpp_doc)"      >> $(cpp_doc)/Doxyfile
	@echo "TAB_SIZE               = 4"               >> $(cpp_doc)/Doxyfile
	@echo "INPUT                  = $(pwd)"          >> $(cpp_doc)/Doxyfile
	@echo "FILE_PATTERNS          = *.h *.cpp"       >> $(cpp_doc)/Doxyfile
	@echo "SOURCE_BROWSER         = YES"             >> $(cpp_doc)/Doxyfile
	@echo "GENERATE_LATEX         = NO"              >> $(cpp_doc)/Doxyfile
	doxygen $(cpp_doc)/Doxyfile
