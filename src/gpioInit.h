#ifndef __gpio_Init_H__
#define __gpio_Init_H__

#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "sysctrl.h"

void init_gpio(void);
void read_anlogs(uint16_t *, uint16_t *, uint8_t *);
bool Read_Up_Button(uint8_t *);

#endif

