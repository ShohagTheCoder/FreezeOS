#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

typedef void (*keyboardCallback)(char);

void keyboard_init();
void keyboard_bind(keyboardCallback callback);

#endif