Protocol objectes joc:
______________________

Sending data:
HEADER  LENGTH  DATA
c     16     "Hooooooooooooola\0"
1B        1B     LENGTH Bytes

End sending data:
HEADER LENGTH
f       0
1B     1B



Protocol partida:
_________________

Què ens interessa saber?
Moviment eix robot		(RM)
Moviment eix persona	(PM)
Figura a la que vaig	(TO)
Figura eliminada    	(EF)
Figures que queden 	  (RF)
SobreFigura?          (OF)


Simplificacions per a enviar informació:

Moviments
Dreta	 	-->	R
Esquerra	-->	L
Amunt		-->	U
Avall		-->	D
Parat		-->	S
Finish --> F

Figures
No figure	-->	0
Figure1	-->	1
Figure2	-->	2
Figure3	-->	3

SobreFigura?
Sí --> Y
No --> N

Figura             --> ID [char]
Figura eliminada   --> ID [char]
Figures que queden --> Número [char]
SobreFigura?       --> [char]




Protocol:

TIMESTAMP	RM	PM	TO  EF	RF  OF
8B        1B  1B  1B  1B  1B  1B
