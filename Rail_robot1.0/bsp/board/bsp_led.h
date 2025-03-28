#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"

typedef enum {
    RED_LIGHT,   // ºìµÆ
    GREEN_LIGHT, // ÂÌµÆ
    BLUE_LIGHT   // À¶µÆ
} LightColor;

void LED_RED(void);
void LED_GREEN(void);
void LED_BLUE(void);
void LED_OUT(uint8_t led);
void RGB_OPEN(void);
void RGB_OFF(void);
void RGB_YELLOW(void);
void RGB_CYAN(void) ;
void RGB_PURPLE(void) ;
void Turn_LED(uint8_t led);

#endif


