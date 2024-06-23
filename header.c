/**
 * header.c
 * 
 * dumps header of wave file
 * assumes file is wave, does no header validation
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/** http://soundfile.sapp.org/doc/WaveFormat/ */
typedef struct _wave_header{
    struct {
        char     r_id[4];   // should be 'RIFF'
        uint32_t r_size;    // size of everything after riff_section
        char r_fmt[4];      // should be 'WAVE'
    }riff_section;

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

int main(int argc, char **argv){
    if(argc < 2){
        printf("usage: %s <file>\n",argv[0]);
        return -1;
    }

    char *fname = argv[1];
    struct stat sb = {0};
    stat(fname,&sb);
    char *buf = (char*)malloc(sb.st_size);
    int fd = open(fname,O_RDONLY);
    read(fd,buf,sb.st_size);
    wave_header_t *wh = (wave_header_t*)buf;

    printf("---- header ----\n");
    printf("---- riff_section----\n");
    printf("r   id: %s\n",wh->riff_section.r_id);
    printf("r size: %d\n",wh->riff_section.r_size);
    printf("r fmt: %c%c%c%c\n",wh->riff_section.r_fmt[0],wh->riff_section.r_fmt[1],wh->riff_section.r_fmt[2],wh->riff_section.r_fmt[3]);
    printf("---- fmt_section----\n");
    printf("f id: %s\n",wh->fmt_section.f_id);
    printf(" f_size :%d\n",wh->fmt_section.f_size);
    printf(" f_encoding :%d\n",wh->fmt_section.f_encoding);
    printf(" f_numchan :%d\n",wh->fmt_section.f_numchan);
    printf(" f_samplerate :%d\n",wh->fmt_section.f_samplerate);
    printf(" f_byterate :%d\n",wh->fmt_section.f_byterate);
    printf(" f_blockalign :%d\n",wh->fmt_section.f_blockalign);
    printf(" f_bitspersample :%d\n",wh->fmt_section.f_bitspersample);
    printf("---- data_section----\n");
    printf("data: d id: %s\n",wh->data_section.d_id);
    printf("d size: %d\n",wh->data_section.d_size);
    printf("---- done ----\n");

    free(buf);
    close(fd);
    return 0;
}