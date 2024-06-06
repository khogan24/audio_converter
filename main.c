#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

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

#define OPTS_INPLACE 0x1000000
#define OPTS_CONVERT 0x0010000
#define OPTS_UPSCALE 0x0000100

#define MAX_UPSCALE  0x00000ff
#define UPSCALE_MASK MAX_UPSCALE

// clamp so this doesn't overwrite the flag
#define SET_FACTOR(o,f) (o |= (f & MAX_UPSCALE))

#define LINEAR_OPT 'l'
#define ALAW_OPT   'a'
#define MULAW_OPT  'm'

#define LINEAR          0x0001000
#define ALAW            0x0002000
#define MULAW           0x0003000
#define ENCODING_MASK   0x000f000

#define SET_ENCODING(o,e) (o |= e)

#define DOES_UPSCALE(o)  (o & OPTS_UPSCALE)
#define DOES_ENCODE(o)   (o & OPTS_CONVERT)
#define DOES_INPLACE(o)  (o & OPTS_INPLACE)

#define GET_FACTOR(o)    (o & UPSCALE_MASK)
#define GET_ENCODE(o)    (o & ENCODING_MASK)

/** Type/Structs */

typedef enum{
    linear = LINEAR,
    alaw   = ALAW,
    mulaw  = MULAW,
} opt_encode_t;

typedef enum{
    todo = 0,
} wave_encoding_t;

typedef struct _wave_header{

} wave_header_t;

/** Functions */

/**
 * file_size
 * 
 * Description: Returns the size of a file
 * 
 * Parameters: name - ASCIIZ string containing the file in question
 * 
 * Returns:  File size on success, UINT32_MAX if it fails
 * 
 * Don't ask for a file that is actually 4294967295 bytes long... just don't.
*/
const uint32_t file_size(const char *name){
    if(!name)
    {
        return UINT32_MAX;
    }

    struct stat sb = {0};
    if(stat(name,&sb) == -1){
        // calling function prints a debug for this
        return UINT32_MAX;
    }
    return sb.st_size;
}

const int set_upscale_opt_from_str(char *argv, uint32_t *opts){
    // -u<factor>
    // lack of space is important
    if(strlen(argv) > 4) return -1;
    
    errno = 0;
    char *loc = argv; // local copy, idk if strtoul moves things

    uint32_t factor = (uint32_t)strtoul(loc,NULL,10);
    IGNORE(factor);
    if(errno != 0){
        return errno;
    }
    if(factor > MAX_UPSCALE) factor = MAX_UPSCALE;
    SET_FACTOR(*opts,factor);
    return 0;
}


/**
 * set_convert_opt_from_str()
 * 
 * parses out the options and sets flags in opts
 * 
 * 
*/
const int set_convert_opt_from_str(char *argv, uint32_t *opts){
    // -c<form>
    // lack of space is important
    if(strlen(argv) >  4) return -1;
    
    errno = 0;
    const char form = argv[2];

    switch (form)
    {
    case ALAW_OPT:
    {
        SET_ENCODING(*opts,ALAW);
    }
    break;

    case MULAW_OPT:
    {
        SET_ENCODING(*opts,MULAW);
    }
    break;

    case LINEAR_OPT:
    {
        SET_ENCODING(*opts,LINEAR);
    }
    break;

    default:
        return -1;
    }
    return 0;
}


/**
 * set_convert_opt_from_str()
 * 
 * parses out the options and sets flags in opts
 * 
 * if it finds what it thinks is a file name, it will store that in ofname
 * MALLOCS ofname
 * 
*/
const int parse_opts(char *argv, uint32_t *opts, char *ofname ){
    if(!argv) return -1;
    
    const uint32_t len = strlen(argv);
    IGNORE(len);
    
    if(strncmp(argv,"-u",2) == 0){
        *opts |= OPTS_UPSCALE;
        return set_upscale_opt_from_str(argv,opts);
    }else if(strncmp(argv,"-c",2) == 0){
        *opts |= OPTS_CONVERT;
        return set_convert_opt_from_str(argv,opts);
    }else if(strncmp(argv,"-i",2) == 0){
        *opts |= OPTS_INPLACE;
        return 0;
    }else{
        // must be outfile
        if(ofname) return -1;
        ofname = (char*)malloc(len);
        memcpy(ofname,argv,len);
        return 0;
    }
    return -1;
}

const int load_file(char *name,char *fbuf, uint32_t size)
{
    int err = 0;
    if(!name  ){
        err = -1;
        goto out;
    }
    
    const int fd = open(name, O_RDONLY);
    
    if(!fd) return -1;

    fbuf = (char*)malloc(size);
    if(!fbuf){
        goto out;
    }
    int bytes_left = size;
    while( bytes_left > 0)
    {
        // I could trust the kernel to do this at once, but it doesn't have to
        bytes_left -= write(fd,fbuf,bytes_left);
    }

out:
    close(fd);
    return err;
}

const int encode(char *buf, uint32_t *size, opt_encode_t newenc){
    // TODO do
    IGNORE(buf);
    IGNORE(size);
    IGNORE(newenc);
    return 0;
}

const int _upscale(char *buf, uint32_t *size){
    return 0;
}

const int upscale(char *buf, uint32_t *size, const uint32_t factor){
    IGNORE(buf);
    IGNORE(size);
    IGNORE(factor);
    // do this by a factor of 2 every time.
    // ignore odd requests for now
    return _upscale(buf,size);
}

void write_to_file(char *name, char *buf, const uint32_t size ){
    return;
}

int main(int argc, char *argv[]){
    uint32_t opts = 0;
    uint32_t encoding = 0;
    uint16_t factor = 1;
    char *fname = NULL;
    char *ofname = NULL;
    char *fbuf = NULL;
    uint32_t size = 0;
    IGNORE(factor);

    if(argc == 1){
        print_usage();
        return 0;
    }
    if(argv[1][0] == '-'){
        if(strncmp(argv[1],"--help",6) == 0 
        || strncmp(argv[1],"-help",5) == 0
        || strncmp(argv[1],"--h",3) == 0
        || strncmp(argv[1],"-h",2) == 0
        ){
            print_usage();
            return 0;
        }
        // I could parse for file name and params in any order,
        // I wont because I don't want to.
        // This is my code, my word is law.
        eprintf("Invalid file name. Input file must be first argument\n");
        return -1;
    }

    const uint32_t fname_len = strlen(argv[1]);
    fname = (char*)malloc(fname_len);

    if(!fname){
        eprintf("Failed to malloc space for file name\n");
        return -ENOMEM;
    }

    memcpy(fname,argv[1],fname_len);

    size = file_size(fname);
    if(size == UINT32_MAX){
        eprintf("Failed to stat file %s\n",fname);
        goto fail_stat;
    }

    for(int i = 2 ; i < argc; ++i){
        if (parse_opts(argv[i],&opts,ofname ) != 0){
            goto fail_opts;
        }
    }

    if(load_file(fname, fbuf, size) != 0)
    {
        goto fail_open;
    }

    if(DOES_UPSCALE(opts)){
        factor = GET_FACTOR(opts);
        if(upscale(fbuf,&size,factor) != 0)
            goto fail_general;
    }

    if(DOES_ENCODE(opts)){
        encoding = GET_ENCODE(opts);
        if(encode(fbuf,&size,encoding) != 0)
            goto fail_general;
    }

    if(!DOES_INPLACE(opts)){
        write_to_file(ofname,fbuf,size);

    }else{
        write_to_file(fname,fbuf,size);
    }

    return 0;

fail_general:
    if(ofname) free(ofname);
    free(fbuf);
fail_open:
fail_opts:
    eprintf("Invalid option\n");
fail_stat:
    free(fname);

    return 1;
}