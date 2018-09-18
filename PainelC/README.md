# RetroServer
Controle do Painel - Versão C

O painel do RetroServor tem como principal função apresentar "feitos lúdicos", 
ou seja, brincadeiras para chamar a atenção. Além disso ele informa o IP e
"carga" (CPU / memória / disco) do servidor e convida as pessoas a se
conectarem. O botão no painel permite comandar um reboot ou shutdown.

Esta versão é em C, para obter um desempenho melhor nas animações. É disparado 
no boot e executado como daemon através do systemd.

O acesso ao gpio e I2C é feito utilizando a biblioteca pigpio
(http://abyz.me.uk/rpi/pigpio/cif.html)

O fonte usado para escrita é uma conversão de font8x8_basic de Daniel Hepper,
que por sua vez se baseou em uma fonte feita por Marcel Sondaar, ambas no
domínio público.
