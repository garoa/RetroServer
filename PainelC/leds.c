/*
    leds.c  Modulo de controle dos LEDs do painel do RetroServer
*/

#include <stdio.h>
#include <pigpio.h>
#include "painelc.h"

#define ON  1
#define OFF 0

// Pinos dos LEDs
static int led[] = { 18, 14, 15, 17 };
#define NLEDS (sizeof(led)/sizeof(led[0]))

// Controle da animacao
static int iled1 = 0;
static int iled2 = NLEDS-1;
static int dir = 1;

// Iniciacao dos LEDs
void ledInit() 
{
    int i;
    
    for (i = 0; i < NLEDS; i++)
    {
        gpioSetMode(led[i], PI_OUTPUT);
        gpioWrite(led[i], OFF);
    }
}

// Atualiza os LEDs
void ledPisca()
{
    gpioWrite(led[iled2], OFF);
    gpioWrite(led[iled1], ON);
    iled2 = iled1;
    if (iled1 == (NLEDS-1))
    {
        dir = -1;
    }
    else if (iled1 == 0)
    {
        dir = 1;
    }
    iled1 = iled1+dir;
}

