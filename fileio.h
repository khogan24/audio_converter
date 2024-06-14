#ifndef __FILEIO__
#define __FILEIO__

#include "defs.h"
void write_to_file(char *name, char *buf, const uint32_t size ){
    return;
}

#define MIN_WAVE_SIZE ((uint32_t)sizeof(wave_header_t))

/**
 * validate_header
 * 
 * checks the header of the file to make sure it is a wave file
 */
const int validate_header(char *buf){
    wave_header_t *wv = (wave_header_t*)buf; // i love casting pointer to arbitrary types
    return strncmp(wv->riff_section.r_id, "RIFF",4) == 0 &&
           strncmp(wv->fmt_section.f_id, "fmt ",4) == 0 && 
           strncmp(wv->data_section.d_id, "data ",4) == 0 ;
}

/**
 * load_file
 * 
 * opens and reads a file into a buffer
 * This function will malloc fbuf, 
 * 
 * args:
 *      name - name of file, ASCIIZ
 *      fbuf - buffer malloced and filled
 *      size - size of file, be sure to stat this file first
 *          then call this function
 * 
 * returns:
 *      0 on success
 *      -1 on error
 */
const int load_file(char *name,char *fbuf, uint32_t size)
{
    int err = 0;
    if(!name  ){
        err = -1;
        goto out;
    }
    if(size < MIN_WAVE_SIZE){
        eprintf("File cannot possibly be a wave file, it is far to small\n");
        return -1;
    }

    const int fd = open(name, O_RDONLY);
    
    if(!fd) return -1;

    fbuf = (char*)malloc(size);
    if(!fbuf){
        goto out;
    }

    eprintf("Opening file...(%u)bytes\n",size);
    int bytes_left = size;
    while( bytes_left > 0)
    {
        // I could trust the kernel to do this at once, but it doesn't have to
        bytes_left -= read(fd,fbuf,bytes_left);
    }

     
    if((err = validate_header(fbuf)) != 0) free(fbuf);
    

out:
    close(fd);
    return err;
}

#endif // end __FILEIO__
