// Define fixed-width integer types if stdint.h is not available
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#define VGA_MEMORY_ADDRESS 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int cursor_row = 0;
int cursor_col = 0;

// outb: Send a byte to an I/O port
static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1"
                 : // No output
                 : "a"(val), "Nd"(port));
}

/**
 * Move cursor function
 */

void move_cursor()
{
    unsigned short position = cursor_row * SCREEN_WIDTH + cursor_col;

    outb(0x3d4, 14);
    outb(0x3d5, (position >> 8) & 0xff);

    outb(0x3d4, 15);
    outb(0x3d5, position & 0xff);
}

/**
 * Put character on screen
 */
void putchar(char c)
{
    if (c == '\n')
    {
        cursor_col = 0;
        cursor_row++;
    }
    else
    {
        volatile unsigned char *video_memory =
            (volatile unsigned char *)0xb8000;
        video_memory[(cursor_row * SCREEN_WIDTH + cursor_col) * 2] = c;
        video_memory[(cursor_row * SCREEN_WIDTH + cursor_col) * 2 + 1] = 0x0f;

        cursor_col++;

        if (cursor_col >= SCREEN_WIDTH)
        {
            cursor_col = 0;
            cursor_row++;
        }
    }

    move_cursor();
}

/**
 * Print string function
 */
void prints(const char *str)
{
    while (*str)
    {
        putchar(*str++);
    }
}

// Function to clear the screen in VGA text mode
void clear_screen()
{
    volatile unsigned char *video_memory =
        (volatile unsigned char *)VGA_MEMORY_ADDRESS;
    int screen_size = VGA_WIDTH * VGA_HEIGHT;
    int i;

    for (i = 0; i < screen_size; i++)
    {
        video_memory[i * 2] = ' ';      // Character: space
        video_memory[i * 2 + 1] = 0x0F; // Attribute: white on black
    }

    cursor_col = 0;
    cursor_row = 0;
    move_cursor();
}
