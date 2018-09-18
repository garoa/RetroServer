/*
    display.c   Modulo de controle do display do painel do RetroServer
*/

#include <stdio.h>
#include <pigpio.h>
#include "painelc.h"

#include "fonte8x8.h"

// Endereco I2C do display
#define DISP_ADDR  0x3C

// Comandos
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Tamanho da tela
#define SSD1306_LCDWIDTH    128
#define SSD1306_LCDHEIGHT   64

// Sequencia de iniciacao do display
static char cmdInit[] =
{
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,
    SSD1306_SETMULTIPLEX, 0x3F,
    SSD1306_SETDISPLAYOFFSET, 0x00,
    SSD1306_SETSTARTLINE | 0x0,
    SSD1306_CHARGEPUMP, 0x14,
    SSD1306_MEMORYMODE, 0x00,
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS, 0x12,
    SSD1306_SETCONTRAST, 0xCF,
    SSD1306_SETPRECHARGE, 0xF1,
    SSD1306_SETVCOMDETECT, 0x40,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON
};

// Rotinas locais
static void sendSeqCmd (char *cmd, unsigned nCmds);
static void sendCmd (char cmd);

// Iniciacao
void displayInit()
{
  sendSeqCmd (cmdInit, sizeof(cmdInit));
  displayClear();
}

// Um buzzer de zeros para limpar a memporia do video
static char zeros[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Limpa todo o display
void displayClear()
{
    int i;
    int hnd;
    
    // Define endereços iniciais e finais de colunas e páginas
    sendCmd (SSD1306_COLUMNADDR);
    sendCmd (0);
    sendCmd (SSD1306_LCDWIDTH-1);
    sendCmd (SSD1306_PAGEADDR);
    sendCmd (0);
    sendCmd (7);

    // Preenche a memória com zeros
    hnd = i2cOpen (1, DISP_ADDR, 0);
    if (hnd >= 0)
    {
        for (i = 0; i < (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i += 16) 
        {
            i2cWriteI2CBlockData (hnd, 0x40, zeros, 16);
        }
        i2cClose (hnd);
    }
}

// Apresenta bitmap no display
// ys e altura devem ser multiplos de 8
// bitmap no formato do display
void displayBitmap(int xe, int ys, int largura, int altura, char *bmp)
{
    int i;
    int y;
    int hnd;
    
    // Define endereços iniciais e finais de colunas e páginas
    sendCmd (SSD1306_COLUMNADDR);
    sendCmd (xe);
    sendCmd (xe+largura-1);
    sendCmd (SSD1306_PAGEADDR);
    sendCmd (ys >> 3);
    sendCmd ((ys+altura-1) >> 3);

    // Copia o bitmap para a memoria
    hnd = i2cOpen (1, DISP_ADDR, 0);
    if (hnd >= 0)
    {
        for (y = 0; y < altura; y += 8) 
        {
            // Escrita máxima é de 32 bytes
            for (i = largura; i > 0; i -= 32)
            {
                i2cWriteI2CBlockData (hnd, 0x40, bmp, i > 32 ? 32 : i);
                bmp += 32;
            }
        }
        i2cClose (hnd);
    }
}

// Escreve texto no display
// y deve ser múltiplo de 8
void displayText(int x, int y, char *texto)
{
    int c;
    char *pc;
    int hnd;
    
    for (; *texto; texto++)
    {
        // Determina o padrao a escrever
        c = *texto;
        if ((c < 0x20) || (c > 0x7F))
        {
            c = 0;
        }
        else
        {
            c = c-32;
        }
        pc = &gc[c << 3];
        
        // Endereca
        sendCmd (SSD1306_COLUMNADDR);
        sendCmd (x);
        sendCmd (x + 7);
        sendCmd (SSD1306_PAGEADDR);
        sendCmd (y >> 3);
        sendCmd (y >> 3);
        
        // Escreve
        hnd = i2cOpen (1, DISP_ADDR, 0);
        if (hnd >= 0)
        {
            i2cWriteI2CBlockData (hnd, 0x40, pc, 8);
            i2cClose (hnd);
        }
        
        // Avanca cursor
        x += 8;
    }
}

// Envia sequência de comandos ao display
static void sendSeqCmd(char *cmd, unsigned nCmds)
{
    int hnd;
    
    hnd = i2cOpen (1, DISP_ADDR, 0);
    if (hnd >= 0)
    {
        i2cWriteI2CBlockData (hnd, 0, cmd, nCmds);
        i2cClose (hnd);
    }
}

// Envia um byte de comando ao display
static void sendCmd(char cmd)
{
    int hnd;
    
    hnd = i2cOpen (1, DISP_ADDR, 0);
    if (hnd >= 0)
    {
        i2cWriteByteData (hnd, 0, cmd);
        i2cClose (hnd);
    }
}

