
void write_to_file(char *name, char *buf, const uint32_t size ){
    return;
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
