#!/usr/bin/python3
#
# Controle do painel
#

from gpiozero import LED
from gpiozero import Button

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

from time import time
from time import sleep

from subprocess import call

import logging
import psutil
import socket
import Adafruit_SSD1306

logging.basicConfig(level="INFO")

# Conexoes ao GPIO
leds = [ LED(18), LED(14), LED(15), LED(17) ]
botao = Button(27, pull_up=False)

# Iniciacoes de display
disp = Adafruit_SSD1306.SSD1306_128_64(rst=None, i2c_address=0x3c, i2c_bus=1)
disp.begin()
disp.clear()
disp.display()
width = disp.width
height = disp.height
image = Image.new('1', (width, height)) #imagem binaria (somente 1's e 0's)
draw = ImageDraw.Draw(image)
font8 = ImageFont.truetype('Minecraftia-Regular.ttf', 8)
font10 = ImageFont.truetype('Minecraftia-Regular.ttf', 10)
logo = Image.open("retroserver.png")
tape = [ Image.open("tape1.png"), Image.open("tape2.png"), Image.open("tape3.png") ]

# Descobre o nosso IP
ip = "desconhecido"
redes = psutil.net_if_addrs()
for rede in redes["eth0"]:
    if rede.family == socket.AF_INET:
        ip = rede.address

# Inicia efeitos
iled1 = 0
iled2 = 3
dir = 1
itape = 0

# Apaga o LED anterior e acende o proximo
def pisca_led():
    global iled1, iled2, dir
    leds[iled2].off()
    leds[iled1].on()
    iled2 = iled1
    if iled1 == 3:
        dir = -1
    elif iled1 == 0:
        dir = 1
    iled1 = iled1+dir

# Tela com log, ip e carga
def tela_logo():
    global ip
    cpu = psutil.cpu_percent()
    mem = psutil.virtual_memory().percent
    disk = psutil.disk_usage('/').percent
    draw.rectangle((0,0,width,height), outline=0, fill=0)
    image.paste(logo,(0, 0))
    draw.text((0, 32), "IP: "+ip,font=font8,fill=255)
    draw.text((0, 45), "CPU:{0:4.1f}% Mem:{1:4.1f}%".format (cpu, mem, disk),font=font8,fill=255)
    draw.text((0, 54), "Disk:{2:4.1f}%".format (cpu, mem, disk),font=font8,fill=255)
    disp.image(image)
    disp.display()

# Tela com instrucoes de uso
def tela_uso():
    draw.rectangle((0,0,width,height), outline=0, fill=0)
    draw.text((0, 0),  "JOGOS CLASSICOS",font=font8,fill=255)
    draw.text((0, 20), "Conecte com SSH em",font=font8,fill=255)
    draw.text((0, 30), ip,font=font10,fill=255)
    draw.text((0, 45), "Login: hacker",font=font8,fill=255)
    draw.text((0, 54), "Password: garoa",font=font8,fill=255)
    disp.image(image)
    disp.display()
    
# Tela com animacao de fita
def tela_fita():
    global itape
    image.paste(tape[itape],(0, 0))
    disp.image(image)
    disp.display()
    itape = (itape+1) % 3

# Tratamento botao
def trata_botao():
    # espera soltar
    draw.rectangle((0,0,width,height), outline=0, fill=0)
    draw.text((0, 0),  "Solte o botao",font=font8,fill=255)
    disp.image(image)
    disp.display()
    sleep (0.2)
    while botao.is_pressed:
        sleep (0.2)
    
    # explica opcoes
    draw.rectangle((0,0,width,height), outline=0, fill=0)
    draw.text((0, 0),  "Pressione por:",font=font8,fill=255)
    draw.text((0, 10),  " 5s para shutdown",font=font8,fill=255)
    draw.text((0, 20),  "10s para reboot",font=font8,fill=255)
    disp.image(image)
    disp.display()
    
    # espera apertar por ate 5 seg
    inicio = time()
    while ((time()-inicio) < 5.0) and not botao.is_pressed:
        sleep (0.2)
    sleep (0.2)
    
    # se apertou espera soltar
    if botao.is_pressed:
        inicio = time()
        estagio = 0
        delata = 0
        while botao.is_pressed:
            sleep (0.2)
            delta = time()-inicio
            if (estagio == 0) and (delta >= 5.0):
                draw.text((0, 40),  "SHUTDOWN",font=font8,fill=255)
                disp.image(image)
                disp.display()
                estagio = 1
            if (estagio == 1) and (delta >= 10.0):
                draw.rectangle((0,40,width,height), outline=0, fill=0)
                draw.text((0, 40),  "REBOOT",font=font8,fill=255)
                disp.image(image)
                disp.display()
                estagio = 2
        # faz a acao associada ao tempo que ficou apertado
        if delta >= 10.0:
            call(["sudo", "reboot"])
        elif delta >= 5.0
            call(["sudo", "shutdown", "-h", "now"])

#try:
passo = 0
while True:
    sleep (0.1)
    if botao.is_pressed:
        trata_botao()
    if (passo % 5) == 0:
        pisca_led()
    if (passo == 0):
        tela_logo ()
    if (passo == 50):
        tela_uso()
    if (passo > 100):
        if (passo & 1) == 0:
            tela_fita ()
    passo = (passo + 1) % 120
#except Exception:
#    exit(1)
