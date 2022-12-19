#ifndef VM_GETOPT_H
#define VM_GETOPT_H

int vm_getopt(int nargc, char *const nargv[], const char *ostr);

extern const char *vm_optarg;
extern int vm_opterr;
extern int vm_optind;
extern int vm_optopt;
extern int vm_optreset;

#endif /* VM_GETOPT_H */
