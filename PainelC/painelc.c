/*
    painelc.c   Modulo principal do controle do painel do RetroServer
    
    TODO: Adaptar para rodar como new-style deamon
          https://www.freedesktop.org/software/systemd/man/daemon.html#New-Style%20Daemons
*/

#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "painelc.h"

// Pino do botao
#define BOTAO 27

// Bitmaps
#include "retroserver.h"
#include "tape1.h"
#include "tape2.h"
#include "tape3.h"
char *tape[] = { tape1, tape2, tape3 };

// Prototipo das funcoes locais
static void sig_handler(int signo);
static void telaLogo(char * sIP);
static void telaUso(char *sIP);
static void telaFita(void);
static void trataBotao(void);

// Ponto de entrada
int main(int argc, char *argv[])
{
    int passo = 0;
    char sIP[20];   // (indefinido) ou xxx.xxx.xxx.xxx
    
    // Inicia acesso ao IO
    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    // Captura sinalizacoes
    if (signal(SIGINT, sig_handler) == SIG_ERR) 
    {
        fprintf(stderr, "can't catch SIGINT\n");
    }
    if (signal(SIGTERM, sig_handler) == SIG_ERR) 
    {
        fprintf(stderr, "can't catch SIGTERM\n");
    }

    // Inicia tratamento dos LEDs
    ledInit();
    
    // Inicia tratamento do display
    displayInit();
    
    // Inicia o botao
    gpioSetMode(BOTAO, PI_INPUT);
    gpioSetPullUpDown(BOTAO, PI_PUD_DOWN);
    
    // Obtem o nosso IP
    getIP(sIP);
    
    // Loop principal
    for (;;)
    {
        usleep(100000);
        if (gpioRead(BOTAO))
            trataBotao();
        if ((passo % 4) == 0)
        {
            ledPisca();
        }
        if (passo == 0)
        {
            telaLogo (sIP);
        }
        if (passo == 50)
        {
            telaUso(sIP);
        }
        if ((passo > 100) && ((passo & 1) == 0))
        {
            telaFita ();
        }
        passo = (passo + 1) % 120;
    }
}

// Tela do logotipo
static void telaLogo (char *sIP)
{
    char aux[100];
    int cpu, mem;
    
    displayClear ();
    displayBitmap (0, 0, 128, 24, retroserver);
    displayText (0, 32, sIP);
    getProcMem (&cpu, &mem);
    sprintf (aux, "CPU:%d%% Mem:%d%%", cpu, mem);
    displayText (0, 48, aux);
    sprintf (aux, "Disk:%d%%", getDisk());
    displayText (0, 56, aux);
}

static void telaUso (char *sIP)
{
    displayClear ();
    displayText (0, 0, "JOGOS CLASSICOS");
    displayText (0, 16, "Conecte com SSH");
    displayText (0, 32, sIP);
    displayText (0, 48, "Login: hacker");
    displayText (0, 56, "Password: garoa");
}

static void telaFita ()
{
    static int iTape = 0;
    
    displayBitmap (0, 0, 128, 64, tape[iTape]);
    iTape = (iTape+1) % 3;
}

static void trataBotao ()
{
    struct timespec inicio, agora;
    int estagio, delta;
    
    // Espera soltar
    displayClear ();
    displayText (8, 24, "Solte o botao");
    usleep(200000);
    while (gpioRead(BOTAO))
        usleep(200000);
    
    // Explica opcoes
    displayClear ();
    displayText (0, 0,  "Pressione por:");
    displayText (0, 16, "5s para shutdown");
    displayText (0, 24, "10s para reboot");

    // Espera apertar por ate 5 seg
    clock_gettime(CLOCK_REALTIME, &inicio);
    while (!gpioRead(BOTAO))
    {
        clock_gettime(CLOCK_REALTIME, &agora);
        if (clockDif(&inicio, &agora) > 5)
        {
            break;
        }
        usleep(200000);
    }
    usleep(200000);
    
    // Se apertou espera soltar
    if (gpioRead(BOTAO))
    {
        clock_gettime(CLOCK_REALTIME, &inicio);
        estagio = delta = 0;
        while (gpioRead(BOTAO))
        {
            usleep(200000);
            clock_gettime(CLOCK_REALTIME, &agora);
            delta = clockDif(&inicio, &agora);
            if ((estagio == 0) && (delta >= 5))
            {
                estagio = 1;
                displayText (0, 40, "SHUTDOWN");
            }
            if ((estagio == 1) && (delta >= 10))
            {
                estagio = 2;
                displayText (0, 40, "REBOOT  ");
            }
        }
        
        // Faz a acao associada ao tempo que ficou apertado
        if (delta >= 10)
        {
            system ("sudo reboot");
        }
        else if (delta >= 5)
        {
            system ("sudo shutdown -h now");
        }
    }
}

// Tratamento das sinalizacoes do Linux
static void sig_handler(int signo)
{
    switch (signo)
    {
        case SIGINT:
        case SIGTERM:
            gpioTerminate();
            exit (0);
            break;
    }
    
  if (signo == SIGINT)
    printf("received SIGINT\n");
}
