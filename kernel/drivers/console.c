#include "../includes/console.h"
#include "../includes/io.h"
#include "../includes/string.h"
#include "../includes/types.h"

// Define fixed-width integer types if stdint.h is not available
#define VGA_MEMORY_ADDRESS 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F;  // White on black

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
#define CURSOR_HIGH_BYTE 14
#define CURSOR_LOW_BYTE 15

int cursor_row = 0;
int cursor_col = 0;

/**
 * Move cursor function
 */

void move_cursor()
{
    // Valid cursor positoin
    if (cursor_row > VGA_HEIGHT || cursor_col > VGA_WIDTH)
    {
        return;  // Handle the error appropriately
    }

    // Get the actual cursor positoin from cursor_row and cursor_col
    unsigned short position = cursor_row * VGA_WIDTH + cursor_col;

    // Set hight byte of cursor position
    outb(VGA_COMMAND_PORT, CURSOR_HIGH_BYTE);
    outb(VGA_DATA_PORT, (position >> 8) & 0xFF);

    // Set low byte of cursorposition
    outb(VGA_COMMAND_PORT, CURSOR_LOW_BYTE);
    outb(VGA_DATA_PORT, position & 0xFF);
}

void cursor_back(int count)
{
    cursor_col -= count;
    move_cursor();
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
        volatile unsigned char* video_memory = (volatile unsigned char*)0xb8000;
        video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2] = c;
        video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2 + 1] = WHITE_ON_BLACK;

        cursor_col++;

        if (cursor_col >= VGA_WIDTH)
        {
            cursor_col = 0;
            cursor_row++;
        }
    }

    // Move the cursot to next position
    move_cursor();
}

void put_nl()
{
    putchar('\n');
}

void hr()
{
    // Move to new line if needed
    if (cursor_col > 0)
    {
        cursor_col = 0;
        cursor_row += 1;
        move_cursor();
    }

    // Print the hr
    for (char i = 0; i < VGA_WIDTH; i++)
    {
        putchar('-');
    }
}

// Function to clear the screen in VGA text mode
void clear_screen()
{
    volatile unsigned char* video_memory = (volatile unsigned char*)VGA_MEMORY_ADDRESS;
    int screen_size = VGA_WIDTH * VGA_HEIGHT;
    int i;

    for (i = 0; i < screen_size; i++)
    {
        video_memory[i * 2] = ' ';                 // Character: space
        video_memory[i * 2 + 1] = WHITE_ON_BLACK;  // Attribute: white on black
    }

    cursor_col = 0;
    cursor_row = 0;
    move_cursor();
}

void s(char* str)
{
    print_str(str);
    put_nl();
}

void n(int n)
{
    print_int(n);
    putchar(' ');
}

void d(char* str, int n)
{
    print_str(str);
    print_str(" : ");
    print_int(n);
    put_nl();
}