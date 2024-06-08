/** Defines*/
#define bool uint8_t

#define eprintf(format, ...) \
  fprintf (stderr, format __VA_OPT__(,) __VA_ARGS__)

#define oprintf(format, ...) \
  fprintf (stdout, format __VA_OPT__(,) __VA_ARGS__)

#define print_usage() do{\
    eprintf("Usage: %s [infile] (flags) [outfile]\n",argv[0]);\
    eprintf("Flags:\n");\
    eprintf("-uF \t\t\t upscale by passed factor(F), factor must be positive\n");\
    eprintf("-i \t\t\t in place, overwrite infile with new audio\n");\
    eprintf("\t\t\t\t if -i, outfile is not needed and is ignored if passed\n");\
    eprintf("-cE \t\t\t convert audio to specified audio encoding E\n");\
    eprintf(" \t\t\t\t valid option are 'l' for linear, 'm' for mu-law, and 'a' for a-law\n");\
    eprintf("-h,--h,-help,--help\t view this screen\n");\
} while(0)

#define IGNORE(x) (void)x
