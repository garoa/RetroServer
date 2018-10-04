# Pirate's Adventure
Time flies.

Assim começa o artigo de Scott Adams na edição de dezembro de 1980 da revista Byte. Neste artigo
Scott (não confundir com o homônimo que escreve e desenha o Dilbert) apresenta os fontes do jogo 
"Pirate's Adventure", a segunda aventura comercializada por sua empresa Adventure International.

Para rodar o jogo nos micros da época, Scott criou um interpretador que mastigava longas tabelas
geradas por um editor que ele também criou. A versão original do interpretador, que ele usou como
base para o artigo, foi escrita em Level II BASIC para o TRS-80 com 16K de Ram. A versão publicada
estava quebrada em dois programas, um para gravar em K7 as tabelas que descrevem o jogo e outro para 
ler e interpretar. Os bem aventurados com um TRS-80 com 32K podeiam juntar os dois programas e
dispensar a gravação e leitura do K7.

No começo de 1981 eu adaptei o programa para o único computador a que eu tinha acesso (no estágio), 
um Cobra-400. A linguagem disponível era um dialeto não muito sofisticado de COBOL.

Em 1982, já formado e trabalhando em uma empresa que fabricava (entre outras coisas) micros CP/M,
adaptei a listagem original para o MBASIC (o que foi trivial, pois tando ele como o BASIC do TRS-80
foram desenvolvidos pela Microsoft).

A conversão para o YABASIC foi bastante complicada. O programa original é bastante confuso, provavelmente
como resultado da tentativa de reduzir a memória ocupada. Não somente múltiplos comandos são colocados
na mesma linha, como a divisão às vezes não é lógica (por exemplo, ter o FOR no meio de uma linha e o
NEXT no meio de outra). A grande encrenca foi o imenso abuso dos FOR/NEXT. Em alguns casos bastou usar
o BREAK mas em outros acabei trocando o FOR/NEXT por IF GOTO. Existem também várias pequenas diferenças
de sintaxe, como a obrigatoriedade de ENDIF quando THEN é usado no IF e a não disponibilidade do ';'
como separador de parâmetros no PRINT. É provável que alguns erros de lógica tenham sido introduzidos.

