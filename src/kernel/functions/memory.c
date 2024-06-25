void *my_memset(void *ptr, int value, unsigned int num) {
    unsigned char *temp = (unsigned char*)ptr;
    while (num--) {
        *temp++ = (unsigned char)value;
    }
    return ptr;
}

void *my_memcpy(void *dest, const void *src, unsigned int num) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    while (num--) {
        *d++ = *s++;
    }
    return dest;
}