#ifndef __DEF__
#define __DEF__
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

/** http://soundfile.sapp.org/doc/WaveFormat/ */
typedef struct _wave_header{
    struct {
        char     r_id[4];   // should be 'RIFF'
        uint32_t r_size;    // size of everything after riff_section
        char    r_fmt[4];   // should be 'WAVE'
    }riff_section;


    // TODO : f_size is variable
    // so the size of this sub-struct is also variable
    struct {
        char     f_id[4];           // should be 'fmt ', note the trailing space
        uint32_t f_size;            // size of of the rest of the fmt_section
        uint16_t f_encoding;        // audio format
        uint16_t f_numchan;         // number of channels, 1 = mono, 2 = stereo, etc
        uint32_t f_samplerate;
        uint32_t f_byterate;        // SampleRate * NumChannels * BitsPerSample/8
        uint16_t f_blockalign;      // NumChannels * BitsPerSample/8
        uint16_t f_bitspersample;   // NumChannels * BitsPerSample/8
    }fmt_section;

    struct {
        char     d_id[4]; // should be 'data'
        uint32_t d_size;  // size of the res of the file
    }data_section;
} wave_header_t;

#define IGNORE(x) (void)x


#endif // end __DEF__