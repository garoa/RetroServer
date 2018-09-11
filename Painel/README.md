# RetroServer
Controle do Painel

O painel do RetroServor tem como principal função apresentar "feitos lúdicos", 
ou seja, brincadeiras para chamar a atenção. Além disso ele informa o IP e
"carga" (CPU / memória / disco) do servidor e convida as pessoas a se
conectarem. O botão no painel permite comandar um reboot ou shutdown.

O controle do painel é feito por um script Python, que é disparado no boot e
executado como daemon através do systemd.

Para executar o script é necessário:

* Interface I2C habilitada
* Biblioteca psutil
* Biblioteca Adafruit_SSD1306 
* Biblioteca python-imaging
* Fonte Minecraftia: https://www.dafont.com/minecraftia.font

WORK IN PROGRESS!
