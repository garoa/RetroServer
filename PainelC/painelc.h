/*
    Definicoes globais do controle do painel do RetroServer
*/

// LEDs
void ledInit (void);
void ledPisca (void);

// Display
void displayInit (void);
void displayClear (void);
void displayBitmap (int xe, int ys, int largura, int altura, char *bmp);
void displayText (int x, int y, char *texto);

// Util
void getIP (char *sIP);
void getProcMem (int *percProc, int *percMem);
int getDisk (void);
int clockDif(struct timespec *inicio, struct timespec *agora);
