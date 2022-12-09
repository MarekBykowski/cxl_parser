// Description: Extern definitions of the misc utility functions

#ifndef UTL_MISC_H
#define UTL_MISC_H

extern int    better_sleep(double sleep_time);
extern void   setup_signal_handlers(char *pgmname);
extern bool   open_file(ifstream &file, const char *fname);
extern bool   open_file(ofstream &file, const char *fname);
extern double diff_time(struct timeval *start, struct timeval *end);
extern void   extract_base_dir_root(const char *fname, char *basename, char *unixdir, 
                                    char *root, const char *winmap, char *windir, int skip_dirs);

// must match application
const bool SUCCESS = 0;
const bool FAILURE = 1;                     // (0 | 1) = 1 so failure can be latched

#endif
