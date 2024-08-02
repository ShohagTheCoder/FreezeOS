typedef enum
{
    READ,
    WRITE,
    APPEND,
} FileMode;

typedef struct
{
    int fd;         // File descriptor
    FileMode mode;  // File mode
    char* buffer;   // Buffer to read/write oparations
    int size;       // Buffer size
    int pos;        // Current position in the buffer
    int flags;      // end of file flag
} __attribute__((packed)) FILE;