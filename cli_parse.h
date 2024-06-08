
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
