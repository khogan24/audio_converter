// std includes
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

// custome includes
#include "defs.h"
#include "cli_parse.h"
#include "fileio.h"
#include "audio.h"
/** Type/Structs */

typedef enum{
    linear = LINEAR,
    alaw   = ALAW,
    mulaw  = MULAW,
} opt_encode_t;

#define PCM 1 // linear quantization

#define MONO 1
#define STEREO 2

/** Functions */

const int encode(char *buf, uint32_t *size, opt_encode_t newenc){
    // TODO do
    IGNORE(buf);
    IGNORE(size);
    IGNORE(newenc);
    return 0;
}

const int _upscale(char *buf, uint32_t *size){
    if(!buf || !size) return -1;
    
    //upscaling, 2x the buffer
    uint8_t *new_buf = (uint8_t*)malloc((*size) * 2);
    if(!new_buf) return -1;

    new_buf[0] = buf[0];
    for(int i = 1; i < *size; ++i){
        new_buf[i] = (buf[i-1] + buf[i])/2;
        new_buf[i+1] = buf[i];
    }

    return 0;
}

const int upscale(char *buf, uint32_t *size, const uint32_t factor){
    wave_header_t *wave;
    IGNORE(buf);
    IGNORE(size);
    IGNORE(factor);
    // do this by a factor of 2 every time.
    // ignore odd requests for now

    if(!buf || !size) return -1;
    wave = (wave_header_t*)buf;
    
    switch (wave->fmt_section.f_encoding)
    {
    case PCM:
        /* code */
        if(factor == 2){
            return _upscale(buf,size);
        }
        break;
    
    default:
        eprintf("Un-handled encoding %d\n",wave->fmt_section.f_encoding);
        break;
    }
    return -1;
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
        goto success;
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
    success:
    eprintf("Done!\n");
    free(fbuf);
    return 0;
}