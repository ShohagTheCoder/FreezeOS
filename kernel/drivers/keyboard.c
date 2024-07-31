#include "../includes/keyboard.h"
#include "../includes/console.h"
#include "../includes/idt.h"
#include "../includes/io.h"
#include "../includes/shell.h"
#include "../includes/string.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1

static keyboardCallback g_callback = 0;

// Lookup table for scancodes to ASCII characters
static char scancode_to_char[128] = {
    0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
    '9',  '0', '-', '=', '\b',                           /* Backspace */
    '\t',                                                /* Tab */
    'q',  'w', 'e', 'r',                                 /* 19 */
    't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n',      /* Enter key */
    0,                                                   /* 29   - Control */
    'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', /* 39 */
    '\'', '`', 0,                                        /* Left shift */
    '\\', 'z', 'x', 'c', 'v',  'b', 'n',                 /* 49 */
    'm',  ',', '.', '/', 0,                              /* Right shift */
    '*',  0,                                             /* Alt */
    ' ',                                                 /* Space bar */
    0,                                                   /* Caps lock */
    0,                                                   /* 59 - F1 key ... > */
    0,    0,   0,   0,   0,    0,   0,   0,   0,         /* < ... F10 */
    0,                                                   /* 69 - Num lock*/
    0,                                                   /* Scroll Lock */
    0,                                                   /* Home key */
    0,                                                   /* Up Arrow */
    0,                                                   /* Page Up */
    '-',  0,                                             /* Left Arrow */
    0,    0,                                             /* Right Arrow */
    '+',  0,                                             /* 79 - End key*/
    0,                                                   /* Down Arrow */
    0,                                                   /* Page Down */
    0,                                                   /* Insert Key */
    0,                                                   /* Delete Key */
    0,    0,   0,   0,                                   /* F11 Key */
    0,                                                   /* F12 Key */
    0,                                                   /* All other keys are undefined */
};

void keyboard_bind(keyboardCallback callback)
{
    if (callback)
    {
        g_callback = callback;
    }
}

void keyboard_handler()
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    if (scancode & 0x80)
    {
        // Key release event
    }
    else
    {
        // Key press event
        char c = scancode_to_char[scancode];
        if (c != 0)
        {
            if (g_callback)
            {
                g_callback(c);
            }
            else
            {
                puts("Keyboard has no binding to handle characters.\n");
            }
        }
    }
}

void keyboard_init()
{
    // Unmask IRQ1
    unmask_irq(KEYBOARD_IRQ);
}
