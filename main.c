#include "libgraph3.h"

 /*
  "Omega_Chess" Ver. 2.0 es un programa que simula un tablero
  para jugar ajedrez, entre 2 personas. propiedad de Omar Rodriguez Arce.
  sian@correoweb.com
  Ver archivo anexo: leeme.txt
*/


/*bibliotecas necesarias*/

#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <piezas.h>
#include <p-table.h>

/*declaracion de variables globales*/

int password;/*variable que indica si el password es correcto*/
/*variables para validar movimientos de las piezas*/
int valido=0;
/*variable para ciclos for*/
int fo;
/*variables para definir casillas de origen y destino de piezas*/
char m1_1, m1_2, m2_2, m2_1;
int  m11, m12, m21, m22, mov_o, mov_d;
/*declaracion de variables para mover la piezas origen y destino*/
int a=0,b=8,c=16,d=24,e=32,f=40,g=48,h=56;

		 /*declaracion de variables para validar piezas*/

/*declaracion de variables, para la piezas negras*/
int tn1=1,tn2=8,nn1=2,nn2=7,an1=3,an2=6,dn=4,rn=5,pn1=9,
	 pn2=10,pn3=11,pn4=12,pn5=13,pn6=14,pn7=15,pn8=16;
/*declaracion de variables, para la piezas blancas*/
int tb1=57,tb2=64,nb1=58,nb2=63,ab1=59,ab2=62,db=60,rb=61,pb1=49,
	 pb2=50,pb3=51,pb4=52,pb5=53,pb6=54,pb7=55,pb8=56;

/*variables de control*/
int status, status1, conta,conta2,cap=0, hay=0/**/,
	 con=1,figura=0/*variable para saber que pieza se escogio para jugarse*/;


/*variables para las funciones de las piezas y sus posiciones*/
int obs=8, cla=7;/*definen los colores de el tablero de juego*//*obs=8, cla=7;*/


void juego(void); /*Funcion que controla el juego en si*/

void cuadroc(void);/*Funcion que dibuja un cuadro claro
							empezando en las coordenadas que indiquen
							las variables "x" y "y"*/

void cuadroo(void);/*Funcion que dibuja un cuadro obscuro
							empezando en las coordenadas que indiquen
							las variables "x" y "y"*/

void rellena(void);/*Funcion que rellena los cuadros de colores*/

void colocar(void);/*funcion para poner las piezas al inico de la partida*/

void control(void);/*funcion para controlar la piezas del tablero*/

void una(void);/*funcion para resibir casillas de origen*/

void otra(void);/*funcion para resibir casillas de destino*/

void cual(void);/*funcion para saber que pieza se escogio*/

void poner(void);/*funcion para poner la pieza en su nueva posicion*/

void ubicarxy(void);/*funcion para mover las variable "x" y "y"*/

void ubicarxy1(void);/*funcion para mover las variable "x" y "y"*/

void cla_obs(void);/*dibuja un cuadro claro u obscuro segun sea el caso*/

void cla_obs1(void);/*dibuja un cuadro claro u obscuro segun sea el caso*/

void validar(void);/*funcion que verifica si el movimiento es valido*/

void tomar(void);/*funcion que verifica si se captura una pieza*/

void hayp(void);/*funcion que verifica si hay una pieza*/

void sacar(void);/*funcion que sacar una pieza del tablero*/

void definir(void);/*cambiar el valor de la pieza jugada a su nueva posicion*/

void mensajef(void);/*mensaje para finalizar el juego*/

void pass(void);/*funcion para utilizacion de password*/


//*******************************************************************
//*******************************************************************

void main(void) /*Funcion principal*/
{
int p=0;
do
{
pass();
p++;
}/*PASSWORD*/
while(password==0 && p!=5);

if(password==1)
{
gra();/*abrir graficos*/
pre();/*presentacion*/
gra();/*abrir graficos nuevamente para limpiar pantalla*/
tablero();
rellena();
gotoxy(11,23);
printf("\"O M E G A _ C H E S S   VER. 2.00\" Programado por El_Omega.");
colocar();
while(rn!=0 && rb!=0)
{
juego();
}
mensajef();
getch();
closegraph();
}

else
{printf("\n\tSE ACABARON lAS OPORTUNIDADES, EL PROGRAMA TERMINARA.");
getch();}
}

//*******************************************************************
//*******************************************************************


void rellena(void)
{
int aux;

/*Diagonal 1*/
x=157;   /*157*/
y=308;   /*21*/
	for(aux=0;aux<1;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 2*/
x=157;   /*157*/
y=267;   /*267*/
	for(aux=0;aux<2;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}
/*Diagonal 3*/
x=157;   /*157*/
y=226;   /*226*/
	for(aux=0;aux<3;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 4*/
x=157;   /*157*/
y=185;   /*185*/
	for(aux=0;aux<4;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}
	/*Diagonal 5*/
x=157;   /*157*/
y=144;   /*144*/
	for(aux=0;aux<5;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 6*/
x=157;   /*157*/
y=103;   /*103*/
	for(aux=0;aux<6;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}
/*Diagonal 7*/
x=157;   /*157*/
y=62;   /*62*/
	for(aux=0;aux<7;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 8*/
x=157;   /*157*/
y=21;   /*21*/
	for(aux=0;aux<8;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}

/*Diagonal 9*/
x=198;   /*198*/
y=21;   /*21*/
	for(aux=0;aux<7;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 10*/
x=239;   /*239*/
y=21;   /*21*/
	for(aux=0;aux<6;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}
/*Diagonal 11*/
x=280;   /*280*/
y=21;   /*21*/
	for(aux=0;aux<5;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 12*/
x=321;   /*321*/
y=21;   /*21*/
	for(aux=0;aux<4;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}
/*Diagonal 13*/
x=362;   /*362*/
y=21;   /*21*/
	for(aux=0;aux<3;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
/*Diagonal 14*/
x=403;   /*403*/
y=21;   /*21*/
	for(aux=0;aux<2;aux++)
	{
	cuadroc();
	x=x+41;
	y=y+41;

	}
/*Diagonal 15*/
x=444;   /*444*/
y=21;   /*21*/
	for(aux=0;aux<1;aux++)
	{
	cuadroo();
	x=x+41;
	y=y+41;

	}
ret=2;
}
//*******************************************************************
void cuadroc(void)
{
estilo=6;
delay(ret/2);
setfillstyle(estilo,cla);
bar(x,y,x+39,y+39);
}
//*******************************************************************
void cuadroo(void)
{
estilo=9;
delay(ret/2);
setfillstyle(estilo,obs);
bar(x,y,x+39,y+39);
}
//*******************************************************************
void colocar(void)
{
ret=15;
/*piezas negras*/

x=152;
y=16;
torren();

x=193;
y=16;
noeln();

x=234;
y=16;
alfiln();

x=275;
y=16;
claun();

x=316;
y=16;
reyn();

x=357;
y=16;
alfiln();

x=398;
y=16;
noeln();

x=439;
y=16;
torren();

/*peones negros*/

x=152;
y=57;
peonn();

x=193;
y=57;
peonn();

x=234;
y=57;
peonn();

x=275;
y=57;
peonn();

x=316;
y=57;
peonn();

x=357;
y=57;
peonn();

x=398;
y=57;
peonn();

x=439;
y=57;
peonn();

/*piezas blancas*/

x=152;
y=303;
torreb();

x=193;
y=303;
noelb();

x=234;
y=303;
alfilb();

x=275;
y=303;
claub();

x=316;
y=303;
reyb();

x=357;
y=303;
alfilb();

x=398;
y=303;
noelb();

x=439;
y=303;
torreb();

/*peones blancos*/
x=152;
y=262;
peonb();

x=193;
y=262;
peonb();

x=234;
y=262;
peonb();

x=275;
y=262;
peonb();

x=316;
y=262;
peonb();

x=357;
y=262;
peonb();

x=398;
y=262;
peonb();

x=439;
y=262;
peonb();
ret=100;
}

//*******************************************************************

void juego(void)
{

do
{
una();
}
while(status!=1 && status1!=1);

ubicarxy();

do
{
otra();
tomar();
hayp();
validar();
}
while(mov_o==mov_d || valido==0);
if(cap==1)
sacar();
definir();
cla_obs();
ubicarxy1();
cla_obs1();
x=x-5;
y=y-5;
poner();

}

//*******************************************************************

void una(void)
{
/*borrando lineas anteriores*/
gotoxy(12,24);
printf("                                                                     ");
gotoxy(12,25);
printf("                                                                     ");
						 /*ciclo para obtener la casilla origen*/
gotoxy(12,24);
/*mensaje*/
printf("CASILLA DE LA PIEZA A MOVER:");
/*ciclo para solo aceptar letras de la A a la H ya se mayusculas o minusculas*/
do
	  {
	  m1_1=getch();
	  }
while(m1_1!='a' && m1_1!='A' && m1_1!='b'&&m1_1!='B' && m1_1!='c'&&m1_1!='C' &&  m1_1!='d'&&m1_1!='D' &&  m1_1!='e'&&m1_1!='E' &&  m1_1!='f'&&m1_1!='F'&&  m1_1!='g'&&m1_1!='G' &&  m1_1!='h' &&m1_1!='H');
gotoxy(41,24);
/*desplegando el valor valido*/
printf("%c,",m1_1);
/*ciclo para solo aceptar numeros del 1 al 8*/
do
	  {
	  m1_2=getch();
	  }
while(m1_2!=49 && m1_2!=50 && m1_2!=51 &&m1_2!=52 && m1_2!=53 && m1_2!=54 &&  m1_2!=55 &&m1_2!=56);
/*desplegando el valor valido*/
gotoxy(43,24);
printf("%c",m1_2);
/*convirtiendo el caracter a su valor numerico*/
if(m1_1=='a' || m1_1=='A')
m11=0;
if(m1_1=='b' || m1_1=='B')
m11=8;
if(m1_1=='c' || m1_1=='C')
m11=16;
if(m1_1=='d' || m1_1=='D')
m11=24;
if(m1_1=='e' || m1_1=='E')
m11=32;
if(m1_1=='f' || m1_1=='F')
m11=40;
if(m1_1=='g' || m1_1=='G')
m11=48;
if(m1_1=='h' || m1_1=='H')
m11=56;
/*convirtiendo el caracter a su valor numerico*/
m12=m1_2-48;
/*definiendo la casilla origen*/
mov_o=m11+m12;
control();/*validando que una pieza se encuentre en la casilla especificada*/
gotoxy(45,24);
if (status==1 || status1==1)
{
printf("->");
cual();
printf(" OK ");
}

if (status!=1 && status1!=1)
{printf(" -> MAL INTENTA DE NUEVO");
getch();}
}

//*******************************************************************


void control(void)
{
if(mov_o==tn1 || mov_o==tn2 || mov_o==nn1 || mov_o==nn2
	||
	mov_o==an1 || mov_o==an2 || mov_o==dn  || mov_o==rn
	||
	mov_o==pn1 || mov_o==pn2 || mov_o==pn3 || mov_o==pn4
	||
	mov_o==pn5 || mov_o==pn6 || mov_o==pn7 || mov_o==pn8)
	{
	status=1;
	}
	else
	{
	status=0;
	}

if(mov_o==tb1 || mov_o==tb2 || mov_o==nb1 || mov_o==nb2
	||
	mov_o==ab1 || mov_o==ab2 || mov_o==db  || mov_o==rb
	||
	mov_o==pb1 || mov_o==pb2 || mov_o==pb3 || mov_o==pb4
	||
	mov_o==pb5 || mov_o==pb6 || mov_o==pb7 || mov_o==pb8)
	{
	status1=1;
	}
	else
	{
	status1=0;
	}
}

//*******************************************************************

void cual(void)
{
/*ver si es pieza negra*/
if(mov_o==tn1 || mov_o==tn2)
  {figura=11;}

if(mov_o==nn1 || mov_o==nn2)
  {figura=21;}

if(mov_o==an1 || mov_o==an2)
  {figura=31;}

if(mov_o==dn)
  {figura=41;}

if(mov_o==rn)
  {figura=51;}

if(mov_o==pn1 || mov_o==pn2 || mov_o==pn3 || mov_o==pn4 || mov_o==pn5 || mov_o==pn6 || mov_o==pn7 || mov_o==pn8)
  {figura=61;}

/*ver si es pieza blanca*/
if(mov_o==tb1 || mov_o==tb2)
  {figura=12;}

if(mov_o==nb1 || mov_o==nb2)
  {figura=22;}

if(mov_o==ab1 || mov_o==ab2)
  {figura=32;}

if(mov_o==db)
  {figura=42;}

if(mov_o==rb)
  {figura=52;}

if(mov_o==pb1 || mov_o==pb2 || mov_o==pb3 || mov_o==pb4 || mov_o==pb5 || mov_o==pb6 || mov_o==pb7 || mov_o==pb8)
  {figura=62;}


/*si la pieza es negra*/
if(figura==11)
	printf(" TORRE NEGRA");
if(figura==21)
	printf(" CABALLO NEGRO");
if(figura==31)
	printf(" ALFIL NEGRO");
if(figura==41)
	printf(" DAMA NEGRA");
if(figura==51)
	printf(" REY NEGRO");
if(figura==61)
	printf(" PEON NEGRO");

  /*si la pieza es blanca*/
if(figura==12)
	printf(" TORRE BLANCA");
if(figura==22)
	printf(" CABALLO BLANCO");
if(figura==32)
	printf(" ALFIL BLANCO");
if(figura==42)
	printf(" DAMA BLANCA");
if(figura==52)
	printf(" REY BLANCO");
if(figura==62)
	printf(" PEON BLANCO");
}

//*******************************************************************

void poner(void)
{

if(figura==11)
torren();
if(figura==21)
noeln();
if(figura==31)
alfiln();
if(figura==41)
claun();
if(figura==51)
reyn();
if(figura==61)
peonn();

if(figura==12)
torreb();
if(figura==22)
noelb();
if(figura==32)
alfilb();
if(figura==42)
claub();
if(figura==52)
reyb();
if(figura==62)
peonb();
}

//*******************************************************************


void ubicarxy(void)/*mover las variables "x" y "y" segun se requiera*/
{
/*defeniendo "x"*/
if(mov_o==1 || mov_o==9 || mov_o==17 || mov_o==25 || mov_o==33 || mov_o==41 || mov_o==49 || mov_o==57)
{x=152;}
if(mov_o==2 || mov_o==10 || mov_o==18 || mov_o==26 || mov_o==34 || mov_o==42 || mov_o==50 || mov_o==58)
{x=193;}
if(mov_o==3 || mov_o==11 || mov_o==19 || mov_o==27 || mov_o==35 || mov_o==43 || mov_o==51 || mov_o==59)
{x=234;}
if(mov_o==4 || mov_o==12 || mov_o==20 || mov_o==28 || mov_o==36 || mov_o==44 || mov_o==52 || mov_o==60)
{x=275;}
if(mov_o==5 || mov_o==13 || mov_o==21 || mov_o==29 || mov_o==37 || mov_o==45 || mov_o==53 || mov_o==61)
{x=316;}
if(mov_o==6 || mov_o==14 || mov_o==22 || mov_o==30 || mov_o==38 || mov_o==46 || mov_o==54 || mov_o==62)
{x=357;}
if(mov_o==7 || mov_o==15 || mov_o==23 || mov_o==31 || mov_o==39 || mov_o==47 || mov_o==55 || mov_o==63)
{x=398;}
if(mov_o==8 || mov_o==16 || mov_o==24 || mov_o==32 || mov_o==40 || mov_o==48 || mov_o==56 || mov_o==64)
{x=439;}
 x=x+5;
/*defeniendo "y"*/
if(mov_o==1 || mov_o==2 || mov_o==3 || mov_o==4 || mov_o==5 || mov_o==6 || mov_o==7 || mov_o==8)
{y=16;}
if(mov_o==9 || mov_o==10 || mov_o==11 || mov_o==12 || mov_o==13 || mov_o==14 || mov_o==15 || mov_o==16)
{y=57;}
if(mov_o==17 || mov_o==18 || mov_o==19 || mov_o==20 || mov_o==21 || mov_o==22 || mov_o==23 || mov_o==24)
{y=98;}
if(mov_o==25 || mov_o==26 || mov_o==27 || mov_o==28 || mov_o==29 || mov_o==30 || mov_o==31 || mov_o==32)
{y=139;}
if(mov_o==33 || mov_o==34 || mov_o==35 || mov_o==36 || mov_o==37 || mov_o==38 || mov_o==39 || mov_o==40)
{y=180;}
if(mov_o==41 || mov_o==42 || mov_o==43 || mov_o==44 || mov_o==45 || mov_o==46 || mov_o==47 || mov_o==48)
{y=221;}
if(mov_o==49 || mov_o==50 || mov_o==51 || mov_o==52 || mov_o==53 || mov_o==54 || mov_o==55 || mov_o==56)
{y=262;}
if(mov_o==57 || mov_o==58 || mov_o==59 || mov_o==60 || mov_o==61 || mov_o==62 || mov_o==63 || mov_o==64)
{y=303;}
 y=y+5;
}

//*******************************************************************

void ubicarxy1(void)/*mover las variables "x" y "y" segun se requiera*/
{
/*defeniendo "x"*/
if(mov_d==1 || mov_d==9 || mov_d==17 || mov_d==25 || mov_d==33 || mov_d==41 || mov_d==49 || mov_d==57)
{x=152;}
if(mov_d==2 || mov_d==10 || mov_d==18 || mov_d==26 || mov_d==34 || mov_d==42 || mov_d==50 || mov_d==58)
{x=193;}
if(mov_d==3 || mov_d==11 || mov_d==19 || mov_d==27 || mov_d==35 || mov_d==43 || mov_d==51 || mov_d==59)
{x=234;}
if(mov_d==4 || mov_d==12 || mov_d==20 || mov_d==28 || mov_d==36 || mov_d==44 || mov_d==52 || mov_d==60)
{x=275;}
if(mov_d==5 || mov_d==13 || mov_d==21 || mov_d==29 || mov_d==37 || mov_d==45 || mov_d==53 || mov_d==61)
{x=316;}
if(mov_d==6 || mov_d==14 || mov_d==22 || mov_d==30 || mov_d==38 || mov_d==46 || mov_d==54 || mov_d==62)
{x=357;}
if(mov_d==7 || mov_d==15 || mov_d==23 || mov_d==31 || mov_d==39 || mov_d==47 || mov_d==55 || mov_d==63)
{x=398;}
if(mov_d==8 || mov_d==16 || mov_d==24 || mov_d==32 || mov_d==40 || mov_d==48 || mov_d==56 || mov_d==64)
{x=439;}
 x=x+5;
/*defeniendo "y"*/
if(mov_d==1 || mov_d==2 || mov_d==3 || mov_d==4 || mov_d==5 || mov_d==6 || mov_d==7 || mov_d==8)
{y=16;}
if(mov_d==9 || mov_d==10 || mov_d==11 || mov_d==12 || mov_d==13 || mov_d==14 || mov_d==15 || mov_d==16)
{y=57;}
if(mov_d==17 || mov_d==18 || mov_d==19 || mov_d==20 || mov_d==21 || mov_d==22 || mov_d==23 || mov_d==24)
{y=98;}
if(mov_d==25 || mov_d==26 || mov_d==27 || mov_d==28 || mov_d==29 || mov_d==30 || mov_d==31 || mov_d==32)
{y=139;}
if(mov_d==33 || mov_d==34 || mov_d==35 || mov_d==36 || mov_d==37 || mov_d==38 || mov_d==39 || mov_d==40)
{y=180;}
if(mov_d==41 || mov_d==42 || mov_d==43 || mov_d==44 || mov_d==45 || mov_d==46 || mov_d==47 || mov_d==48)
{y=221;}
if(mov_d==49 || mov_d==50 || mov_d==51 || mov_d==52 || mov_d==53 || mov_d==54 || mov_d==55 || mov_d==56)
{y=262;}
if(mov_d==57 || mov_d==58 || mov_d==59 || mov_d==60 || mov_d==61 || mov_d==62 || mov_d==63 || mov_d==64)
{y=303;}
 y=y+5;
}

//*******************************************************************

void otra(void)
{

/*borrando lineas anteriores*/
gotoxy(12,25);
printf("                                                                    ");
						 /*ciclo para obtener la casilla origen*/
gotoxy(12,25);
/*mensaje*/
printf("CASILLA PARA PONER LA PIEZA:");
/*ciclo para solo aceptar letras de la A a la H ya se mayusculas o minusculas*/
do
	  {
	  m2_1=getch();
	  }
while(m2_1!='a' && m2_1!='A' && m2_1!='b'&&m2_1!='B' && m2_1!='c'&&m2_1!='C' &&  m2_1!='d'&&m2_1!='D' &&  m2_1!='e'&&m2_1!='E' &&  m2_1!='f'&&m2_1!='F'&&  m2_1!='g'&&m2_1!='G' &&  m2_1!='h' &&m2_1!='H');
gotoxy(41,25);
/*desplegando el valor valido*/
printf("%c,",m2_1);
/*ciclo para solo aceptar numeros del 1 al 8*/
do
	  {
	  m2_2=getch();
	  }
while(m2_2!=49 && m2_2!=50 && m2_2!=51 &&m2_2!=52 && m2_2!=53 && m2_2!=54 &&  m2_2!=55 &&m2_2!=56);
/*desplegando el valor valido*/
gotoxy(43,25);
printf("%c",m2_2);
/*convirtiendo el caracter a su valor numerico*/
if(m2_1=='a' || m2_1=='A')
m21=0;
if(m2_1=='b' || m2_1=='B')
m21=8;
if(m2_1=='c' || m2_1=='C')
m21=16;
if(m2_1=='d' || m2_1=='D')
m21=24;
if(m2_1=='e' || m2_1=='E')
m21=32;
if(m2_1=='f' || m2_1=='F')
m21=40;
if(m2_1=='g' || m2_1=='G')
m21=48;
if(m2_1=='h' || m2_1=='H')
m21=56;
/*convirtiendo el caracter a su valor numerico*/
m22=m2_2-48;
/*definiendo la casilla origen*/
mov_d=m21+m22;

if(mov_o==mov_d)
printf(" -> MAL INTENTA DE NUEVO");
gotoxy(45,25);
}

//*******************************************************************

void cla_obs(void)
{

if (
	mov_o==1 ||  mov_o==3 || mov_o==5 || mov_o==7
	||
	mov_o==10 ||  mov_o==12 || mov_o==14 || mov_o==16
	||
	mov_o==17 ||  mov_o==19 || mov_o==21 || mov_o==23
	||
	mov_o==26 ||  mov_o==28 || mov_o==30 || mov_o==32
	||
	mov_o==33 ||  mov_o==35 || mov_o==37 || mov_o==39
	||
	mov_o==42 ||  mov_o==44 || mov_o==46 || mov_o==48
	||
	mov_o==49 ||  mov_o==51 || mov_o==53 || mov_o==55
	||
	mov_o==58 ||  mov_o==60 || mov_o==62 || mov_o==64
	)
cuadroc();
else
cuadroo();
}

//*******************************************************************

void cla_obs1(void)
{

if (
	mov_d==1 ||  mov_d==3 || mov_d==5 || mov_d==7
	||
	mov_d==10 ||  mov_d==12 || mov_d==14 || mov_d==16
	||
	mov_d==17 ||  mov_d==19 || mov_d==21 || mov_d==23
	||
	mov_d==26 ||  mov_d==28 || mov_d==30 || mov_d==32
	||
	mov_d==33 ||  mov_d==35 || mov_d==37 || mov_d==39
	||
	mov_d==42 ||  mov_d==44 || mov_d==46 || mov_d==48
	||
	mov_d==49 ||  mov_d==51 || mov_d==53 || mov_d==55
	||
	mov_d==58 ||  mov_d==60 || mov_d==62 || mov_d==64
	)
cuadroc();
else
cuadroo();
}

//*******************************************************************

void validar(void)
{
valido=0;   /*inicializar variable*/

					 /*validando peones*/

				//*******************************************************************
if (figura==61)                /*61 = peon negro*/
  {
							  /*lateral izquierda*/
	 if(mov_o==1 || mov_o==9 || mov_o==17 || mov_o==25 || mov_o==33 || mov_o==41 || mov_o==49 || mov_o==57)
		{
		if (mov_d==mov_o+8 && hay==0)/*avanza recto si no hay pieza*/
			{
			valido=1;
			}

		if (mov_d==mov_o+9 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		}


							  /*lateral derecha*/
	 if(mov_o==8 || mov_o==16 || mov_o==24 || mov_o==32 || mov_o==40 || mov_o==48 || mov_o==56 || mov_o==64)
		{
		if (mov_d==mov_o+8 && hay==0)/*avanza recto si no hay pieza*/
			{
			valido=1;
			}

		if (mov_d==mov_o+7 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		}


							  /*centro*/
	 if(mov_o!=1 &&  mov_o!=8 &&  mov_o!=9 &&  mov_o!=16 &&  mov_o!=17 &&  mov_o!=24 &&  mov_o!=25 &&  mov_o!=32 &&  mov_o!=33 &&  mov_o!=40 &&  mov_o!=41 &&  mov_o!=48 &&  mov_o!=49 &&  mov_o!=56 &&  mov_o!=57 &&  mov_o!=64)
		{
		if (mov_d==mov_o+7 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		if (mov_d==mov_o+8 && hay==0)/*avanza recto si no hay pieza*/
			{
			valido=1;
			}

		if (mov_d==mov_o+9 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		}
		  /*movimiento especial si el peon no se ha movido*/
  if (mov_o==9 || mov_o==10 || mov_o==11 || mov_o==12 || mov_o==13 || mov_o==14 || mov_o==15 || mov_o==16)
	{
	if(mov_d==mov_o+16)
	valido=1;
	}

  }
				//*******************************************************************
if (figura==62)                /*62 = peon blanco*/
  {
							  /*lateral izquierda*/
	 if(mov_o==1 || mov_o==9 || mov_o==17 || mov_o==25 || mov_o==33 || mov_o==41 || mov_o==49 || mov_o==57)
		{
		if (mov_d==mov_o-8 && hay==0)/*avanza recto si no hay pieza*/
			{
			valido=1;
			}

		if (mov_d==mov_o-7 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		}


							  /*lateral derecha*/
	 if(mov_o==8 || mov_o==16 || mov_o==24 || mov_o==32 || mov_o==40 || mov_o==48 || mov_o==56 || mov_o==64)
		{
		if (mov_d==mov_o-8 && hay==0)/*avanza recto si no hay pieza*/
			{
			valido=1;
			}

		if (mov_d==mov_o-9 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		}


							  /*centro*/
	 if(mov_o!=1 &&  mov_o!=8 &&  mov_o!=9 &&  mov_o!=16 &&  mov_o!=17 &&  mov_o!=24 &&  mov_o!=25 &&  mov_o!=32 &&  mov_o!=33 &&  mov_o!=40 &&  mov_o!=41 &&  mov_o!=48 &&  mov_o!=49 &&  mov_o!=56 &&  mov_o!=57 &&  mov_o!=64)
		{
		if (mov_d==mov_o-7 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		if (mov_d==mov_o-8 && hay==0)/*avanza recto si no hay pieza*/
			{
			valido=1;
			}

		if (mov_d==mov_o-9 && hay==1)/*captura en diagonal*/
			{
			valido=1;
			}
		}
		  /*movimiento especial si el peon no se ha movido*/
  if (mov_o==49 || mov_o==50 || mov_o==51 || mov_o==52 || mov_o==53 || mov_o==54 || mov_o==55 || mov_o==56)
	{
	if(mov_d==mov_o-16)
	valido=1;
	}


  }
  //*******************************************************************
								/*validando torres y dama*/
			/*11 t-negra,   12 t-blanca,  41 d-blanca,  42 d-negra*/
if(figura==11 || figura==12 || figura==41 || figura==42)
{
	 if(mov_o>0 && mov_o<9) /*si esta en la primera linea*/
	 {
		  if(mov_d>0 && mov_d<9) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>8 && mov_o<17)/*si esta en la segunda linea*/
	 {
			if(mov_d>8 && mov_d<17) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>16 && mov_o<25)/*si esta en la tercera linea*/
	 {
		  if(mov_d>16 && mov_d<25) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>24 && mov_o<33)/*si esta en la cuarta linea*/
	 {
		  if(mov_d>24 && mov_d<33) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>32 && mov_o<41)/*si esta en la quinta linea*/
	 {
		  if(mov_d>32 && mov_d<41) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>40 && mov_o<49)/*si esta en la sexta linea*/
	 {
		  if(mov_d>40 && mov_d<49) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>48 && mov_o<57)/*si esta en la septima linea*/
	 {
		  if(mov_d>48 && mov_d<57) /*moverse en la misma linea*/
		  {valido=1;}
	 }

	 if(mov_o>56 && mov_o<65)/*si esta en la octava linea*/
	 {
		  if(mov_d>56 && mov_d<65) /*moverse en la misma linea*/
		  {valido=1;}
	 }
					  /*moviendose por las columnas*/
	 if(mov_d==mov_o-56)
	 {valido=1;}
	 if(mov_d==mov_o-48)
	 {valido=1;}
	 if(mov_d==mov_o-40)
	 {valido=1;}
	 if(mov_d==mov_o-32)
	 {valido=1;}
	 if(mov_d==mov_o-24)
	 {valido=1;}
	 if(mov_d==mov_o-16)
	 {valido=1;}
	 if(mov_d==mov_o-8)
	 {valido=1;}
	 if(mov_d==mov_o+8)
	 {valido=1;}
	 if(mov_d==mov_o+16)
	 {valido=1;}
	 if(mov_d==mov_o+24)
	 {valido=1;}
	 if(mov_d==mov_o+32)
	 {valido=1;}
	 if(mov_d==mov_o+40)
	 {valido=1;}
	 if(mov_d==mov_o+48)
	 {valido=1;}
	 if(mov_d==mov_o+56)
	 {valido=1;}
}

//*******************************************************************//
								/*validando alfiles y dama*/
			/*31 a-negro,   32 a-blanco,  41 d-blanca,  42 d-negra*/
if(figura==31 || figura==32 || figura==41 || figura==42)
{
		  /*diagonales "de izquierda a derecha"*/

	/*diagonal 1*/ /*se validara despues*/
	/*diagonal 2*/
	 if(mov_o==49 || mov_o==58)
	 if(mov_d==49 || mov_d==58)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 3*/
	 if(mov_o==41 || mov_o==50 || mov_o==59)
	 if(mov_d==41 || mov_d==50 || mov_d==59)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 4*/
	 if(mov_o==33 || mov_o==42 || mov_o==51 || mov_o==60)
	 if(mov_d==33 || mov_d==42 || mov_d==51 || mov_d==60)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 5*/
	 if(mov_o==25 || mov_o==34 || mov_o==43 || mov_o==52 || mov_o==61)
	 if(mov_d==25 || mov_d==34 || mov_d==43 || mov_d==52 || mov_d==61)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 6*/
	 if(mov_o==17 || mov_o==26 || mov_o==35 || mov_o==44 || mov_o==53 || mov_o==62)
	 if(mov_d==17 || mov_d==26 || mov_d==35 || mov_d==44 || mov_d==53 || mov_d==62)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 7*/
	 if(mov_o==9 || mov_o==18 || mov_o==27 || mov_o==36 || mov_o==45 || mov_o==54 || mov_o==63)
	 if(mov_d==9 || mov_d==18 || mov_d==27 || mov_d==36 || mov_d==45 || mov_d==54 || mov_d==63)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 8*/
	 if(mov_o==1 || mov_o==10 || mov_o==19 || mov_o==28 || mov_o==37 || mov_o==46 || mov_o==55 || mov_o==64)
	 if(mov_d==1 || mov_d==10 || mov_d==19 || mov_d==28 || mov_d==37 || mov_d==46 || mov_d==55 || mov_d==64)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 9*/
	 if(mov_o==2 || mov_o==11 || mov_o==20 || mov_o==29 || mov_o==38 || mov_o==47 || mov_o==56)
	 if(mov_d==2 || mov_d==11 || mov_d==20 || mov_d==29 || mov_d==38 || mov_d==47 || mov_d==56)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 10*/
	 if(mov_o==3 || mov_o==12 || mov_o==21 || mov_o==30 || mov_o==39 || mov_o==48)
	 if(mov_d==3 || mov_d==12 || mov_d==21 || mov_d==30 || mov_d==39 || mov_d==48)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 11*/
	 if(mov_o==4 || mov_o==13 || mov_o==22 || mov_o==31 || mov_o==40)
	 if(mov_d==4 || mov_d==13 || mov_d==22 || mov_d==31 || mov_d==40)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 12*/
	 if(mov_o==5 || mov_o==14 || mov_o==23 || mov_o==32)
	 if(mov_d==5 || mov_d==14 || mov_d==23 || mov_d==32)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 13*/
	 if(mov_o==6 || mov_o==15 || mov_o==24)
	 if(mov_d==6 || mov_d==15 || mov_d==24)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 14*/
	 if(mov_o==7 || mov_o==16)
	 if(mov_d==7 || mov_d==16)   /*solo se mueve en esa diagonal*/
	 valido=1;
	 /*diagonal 15*/ /*se validara despues*/


		  /*las diagonales inversas a las anteriores*/
	/*diagonal 1*/  /*se valido ya*/
	/*diagonal 2*/
	if(mov_o==56 || mov_o==63)
	if(mov_d==56 || mov_d==63)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 3*/
	if(mov_o==48 || mov_o==55 || mov_o==62)
	if(mov_d==48 || mov_d==55 || mov_d==62)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 4*/
	if(mov_o==40 || mov_o==47 || mov_o==54 || mov_o==61)
	if(mov_d==40 || mov_d==47 || mov_d==54 || mov_d==61)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 5*/
	if(mov_o==32 || mov_o==39 || mov_o==46 || mov_o==53 || mov_o==60)
	if(mov_d==40 || mov_d==47 || mov_d==54 || mov_d==61)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 6*/
	if(mov_o==24 || mov_o==31 || mov_o==38 || mov_o==45 || mov_o==52 || mov_o==59)
	if(mov_d==24 || mov_d==31 || mov_d==38 || mov_d==45 || mov_d==52 || mov_d==59)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 7*/
	if(mov_o==16 || mov_o==23 || mov_o==30 || mov_o==37 || mov_o==44 || mov_o==51 || mov_o==58)
	if(mov_d==16|| mov_d==23 || mov_d==30 || mov_d==37 || mov_d==44 || mov_d==51 || mov_d==58)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 8*/
	if(mov_o==8 || mov_o==15 || mov_o==22 || mov_o==29 || mov_o==36 || mov_o==43 || mov_o==50 || mov_o==57)
	if(mov_d==8 || mov_d==15 || mov_d==22 || mov_d==29 || mov_d==36 || mov_d==43 || mov_d==50 || mov_d==57)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 9*/
	if(mov_o==7 || mov_o==14 || mov_o==21 || mov_o==28 || mov_o==35 || mov_o==42 || mov_o==49)
	if(mov_d==7 || mov_d==14 || mov_d==21 || mov_d==28 || mov_d==35 || mov_d==42 || mov_d==49)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 10*/
	if(mov_o==6 || mov_o==13 || mov_o==20 || mov_o==27 || mov_o==34 || mov_o==41)
	if(mov_d==6 || mov_d==13 || mov_d==20 || mov_d==27 || mov_d==34 || mov_d==41)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 11*/
	if(mov_o==5 || mov_o==12 || mov_o==19 || mov_o==26 || mov_o==33)
	if(mov_d==5 || mov_d==12 || mov_d==19 || mov_d==26 || mov_d==33)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 12*/
	if(mov_o==4 || mov_o==11 || mov_o==18 || mov_o==25)
	if(mov_d==4 || mov_d==11 || mov_d==18 || mov_d==25)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 13*/
	if(mov_o==3 || mov_o==10 || mov_o==17)
	if(mov_d==3 || mov_d==10 || mov_d==17)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 14*/
	if(mov_o==2 || mov_o==9)
	if(mov_d==2 || mov_d==9)   /*solo se mueve en esa diagonal*/
	valido=1;
	/*diagonal 15*/  /*se valido ya*/
}

//*******************************************************************//
			/*validando noeles (caballos)*/ /*"noel" es por un amigo.
														  va por ti.*/
if(figura==21 || figura==22)/*21 negro, 22 blanco*/
{
				  /*si esta la casilla 1*/

  if(mov_o==1)
	{
		if(mov_d==mov_o+10 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta la casilla 2*/

  if(mov_o==2)
	{
		if(mov_d==mov_o+10 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}
				  /*si esta la casilla 3 al 6*/

  if(mov_o>2 && mov_o<7)
	{
		if(mov_d==mov_o+6 || mov_d==mov_o+10 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta la casilla 7*/

  if(mov_o==7)
	{
		if(mov_d==mov_o+6 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta la casilla 8*/

  if(mov_o==8)
	{
		if(mov_d==mov_o+6 || mov_d==mov_o+15)
			{valido=1;}
	}

				  /*si esta la casilla 9*/

  if(mov_o==9)
	{
		if(mov_d==mov_o-6 || mov_d==mov_o+10 || mov_d==mov_o+17)
			{valido=1;}
	}
				  /*si esta la casilla 10*/

  if(mov_o==10)
	{
		if(mov_d==mov_o-6 || mov_d==mov_o+10 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta la casilla 11 al 14*/

  if(mov_o>10 && mov_o<15)
	{
		if(mov_d==mov_o-10 || mov_d==mov_o-6 || mov_d==mov_o+6 || mov_d==mov_o+10 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta la casilla 15*/

  if(mov_o==15)
	{
		if(mov_d==mov_o-10 || mov_d==mov_o+6 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta la casilla 16*/

  if(mov_o==16)
	{
		if(mov_d==mov_o-10 || mov_d==mov_o+6 || mov_d==mov_o+15)
			{valido=1;}
	}

				  /*si esta las casillas 17,25,33,41*/

  if(mov_o==17 || mov_o==25 || mov_o==33 || mov_o==41)
	{
		if(mov_d==mov_o-15 || mov_d==mov_o-6 || mov_d==mov_o+10 || mov_d==mov_o+17)
			{valido=1;}
	}
				  /*si esta las casillas 18,26,34,42*/

  if(mov_o==18 || mov_o==26 || mov_o==34 || mov_o==42)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-6 || mov_d==mov_o+10 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta las casillas 23,31,39,47*/

  if(mov_o==23 || mov_o==31 || mov_o==39 || mov_o==47)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-10 || mov_d==mov_o+6 || mov_d==mov_o+15 || mov_d==mov_o+17)
			{valido=1;}
	}

				  /*si esta las casillas 24,32,40,48*/

  if(mov_o==24 || mov_o==32 || mov_o==40 || mov_o==48)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-10 || mov_d==mov_o+6 || mov_d==mov_o+15)
			{valido=1;}
	}



				  /*si esta las casillas centrales*/

  if(mov_o>18 && mov_o<23
	  ||
	  mov_o>26 && mov_o<31
	  ||
	  mov_o>34 && mov_o<39
	  ||
	  mov_o>42 && mov_o<47)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-10 || mov_d==mov_o-6
			||
			mov_d==mov_o+17 || mov_d==mov_o+15 || mov_d==mov_o+10 || mov_d==mov_o+6)
			{valido=1;}
	}

					 /*si esta en la casilla 49*/
  if(mov_o==49)
	{
		if(mov_d==mov_o-6 || mov_d==mov_o-15 || mov_d==mov_o+10)
			{valido=1;}
	}

					 /*si esta en la casilla 50*/
  if(mov_o==1)
	{
		if(mov_d==mov_o-6 || mov_d==mov_o-15 || mov_d==mov_o-17 || mov_d==mov_o+10)
			{valido=1;}
	}

					 /*si esta en la casilla 51 a 54*/
  if(mov_o>50 && mov_o<55)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-6 || mov_d==mov_o-10 || mov_d==mov_o+6 || mov_d==mov_o+10)
			{valido=1;}
	}

					 /*si esta en la casilla 55*/
  if(mov_o==55)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-10 || mov_d==mov_o+6)
			{valido=1;}
	}

					 /*si esta en la casilla 56*/
  if(mov_o==56)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-10 || mov_d==mov_o+6)
			{valido=1;}
	}

					 /*si esta en la casilla 57*/
  if(mov_o==57)
	{
		if(mov_d==mov_o-6 || mov_d==mov_o-15)
			{valido=1;}
	}

					 /*si esta en la casilla 58*/
  if(mov_o==58)
	{
		if(mov_d==mov_o-15 || mov_d==mov_o-17 || mov_d==mov_o-6)
			{valido=1;}
	}

					 /*si esta en la casilla 59 a 62*/
  if(mov_o>58 && mov_o<63)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-10 || mov_d==mov_o-6)
			{valido=1;}
	}

					 /*si esta en la casilla 63*/
  if(mov_o==63)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-15 || mov_d==mov_o-10)
			{valido=1;}
	}

					 /*si esta en la casilla 64*/
  if(mov_o==64)
	{
		if(mov_d==mov_o-17 || mov_d==mov_o-10)
			{valido=1;}
	}

}

//*******************************************************************
									/*validando al Omega osea el REY*/

if (figura==51 || figura==52)
{
					/*si esta la casilla 1*/
	if(mov_o==1)
	{
		if(mov_d==mov_o+1 || mov_d==mov_o+8 || mov_d==mov_o+9)
		valido=1;
	}
					/*si esta la casilla 2 a 7*/
	if(mov_o>1 && mov_o<8)
	{
		if(mov_d==mov_o-1 || mov_d==mov_o+1 || mov_d==mov_o+7 || mov_d==mov_o+8 || mov_d==mov_o+9)
		valido=1;
	}
					/*si esta la casilla 8*/
	if(mov_o==8)
	{
		if(mov_d==mov_o-1 || mov_d==mov_o+7 || mov_d==mov_o+8)
		valido=1;
	}
					/*si esta la casilla 9,17,25,33,41,49*/
	if(mov_o==9 || mov_o==17 || mov_o==25 || mov_o==33 || mov_o==41 || mov_o==48)
	{
		if(mov_d==mov_o-8 || mov_d==mov_o-7 || mov_d==mov_o+1 || mov_d==mov_o+8 || mov_d==mov_o+9)
		valido=1;
	}
					/*si esta la casilla 16,24,32,40,48,56,*/
	if(mov_o==16 || mov_o==24 || mov_o==32 || mov_o==40 || mov_o==48 || mov_o==56)
	{
		if(mov_d==mov_o-8 || mov_d==mov_o-9 || mov_d==mov_o-1 || mov_d==mov_o+7 || mov_d==mov_o+8)
		valido=1;
	}
					/*si esta la casilla 57*/
	if(mov_o==57)
	{
		if(mov_d==mov_o-8 || mov_d==mov_o-7 || mov_d==mov_o+1)
		valido=1;
	}
					/*si esta la casilla 58 a 63*/
	if(mov_o>57 && mov_o<64)
	{
		if(mov_d==mov_o-9 || mov_d==mov_o-8 || mov_d==mov_o-7 || mov_d==mov_o-1 || mov_d==mov_o+1)
		valido=1;
	}
					/*si esta la casilla 64*/
	if(mov_o==64)
	{
		if(mov_d==mov_o-9 || mov_d==mov_o-9 || mov_d==mov_o-1)
		valido=1;
	}
					/*si esta las casillas centrales*/
	if(mov_o>9 && mov_o<16 || mov_o>17 && mov_o<24 || mov_o>25 && mov_o<32 || mov_o>33 && mov_o<40 || mov_o>41 && mov_o<48 || mov_o>49 && mov_o<56)
	{
		if(mov_d==mov_o-9 || mov_d==mov_o-8 || mov_d==mov_o-7 || mov_d==mov_o-1 || mov_d==mov_o+1 || mov_d==mov_o+7 || mov_d==mov_o+8 || mov_d==mov_o+9)
		valido=1;
	}

}



}
//*******************************************************************

void tomar(void)
{
cap=0;
if(mov_d==tn1 || mov_d==tn2 || mov_d==nn1 || mov_d==nn2
	||
	mov_d==an1 || mov_d==an2 || mov_d==dn  || mov_d==rn
	||
	mov_d==pn1 || mov_d==pn2 || mov_d==pn3 || mov_d==pn4
	||
	mov_d==pn5 || mov_d==pn6 || mov_d==pn7 || mov_d==pn8)
	{
	cap=1;
	}

if(mov_d==tb1 || mov_d==tb2 || mov_d==nb1 || mov_d==nb2
	||
	mov_d==ab1 || mov_d==ab2 || mov_d==db  || mov_d==rb
	||
	mov_d==pb1 || mov_d==pb2 || mov_d==pb3 || mov_d==pb4
	||
	mov_d==pb5 || mov_d==pb6 || mov_d==pb7 || mov_d==pb8)
	{
	cap=1;
	}
}

void hayp(void)
{
hay=0;
if(mov_d==tn1 || mov_d==tn2 || mov_d==nn1 || mov_d==nn2
	||
	mov_d==an1 || mov_d==an2 || mov_d==dn  || mov_d==rn
	||
	mov_d==pn1 || mov_d==pn2 || mov_d==pn3 || mov_d==pn4
	||
	mov_d==pn5 || mov_d==pn6 || mov_d==pn7 || mov_d==pn8)
	{
	hay=1;
	}

if(mov_d==tb1 || mov_d==tb2 || mov_d==nb1 || mov_d==nb2
	||
	mov_d==ab1 || mov_d==ab2 || mov_d==db  || mov_d==rb
	||
	mov_d==pb1 || mov_d==pb2 || mov_d==pb3 || mov_d==pb4
	||
	mov_d==pb5 || mov_d==pb6 || mov_d==pb7 || mov_d==pb8)
	{
	hay=1;
	}
}

//*******************************************************************

void captura(void)
{
cap=0;
if(mov_d==tn1 || mov_d==tn2 || mov_d==nn1 || mov_d==nn2
	||
	mov_d==an1 || mov_d==an2 || mov_d==dn  || mov_d==rn
	||
	mov_d==pn1 || mov_d==pn2 || mov_d==pn3 || mov_d==pn4
	||
	mov_d==pn5 || mov_d==pn6 || mov_d==pn7 || mov_d==pn8)
	{
	cap=1;
	}

if(mov_d==tb1 || mov_d==tb2 || mov_d==nb1 || mov_d==nb2
	||
	mov_d==ab1 || mov_d==ab2 || mov_d==db  || mov_d==rb
	||
	mov_d==pb1 || mov_d==pb2 || mov_d==pb3 || mov_d==pb4
	||
	mov_d==pb5 || mov_d==pb6 || mov_d==pb7 || mov_d==pb8)
	{
	cap=1;
	}
}

//*******************************************************************

void sacar(void)
{
/*sacar negras*/
if(tn1==mov_d)
tn1=0;
if(tn2==mov_d)
tn2=0;
if(nn1==mov_d)
nn1=0;
if(nn2==mov_d)
nn2=0;
if(an1==mov_d)
an1=0;
if(an2==mov_d)
an2=0;
if(dn==mov_d)
dn=0;
if(rn==mov_d)
rn=0;
if(pn1==mov_d)
pn1=0;
if(pn2==mov_d)
pn2=0;
if(pn3==mov_d)
pn3=0;
if(pn4==mov_d)
pn4=0;
if(pn5==mov_d)
pn5=0;
if(pn6==mov_d)
pn6=0;
if(pn7==mov_d)
pn7=0;
if(pn8==mov_d)
pn8=0;
/*sacar blancas*/
if(tb1==mov_d)
tb1=0;
if(tb2==mov_d)
tb2=0;
if(nb1==mov_d)
nb1=0;
if(nb2==mov_d)
nb2=0;
if(ab1==mov_d)
ab1=0;
if(ab2==mov_d)
ab2=0;
if(db==mov_d)
db=0;
if(rb==mov_d)
rb=0;
if(pb1==mov_d)
pb1=0;
if(pb2==mov_d)
pb2=0;
if(pb3==mov_d)
pb3=0;
if(pb4==mov_d)
pb4=0;
if(pb5==mov_d)
pb5=0;
if(pb6==mov_d)
pb6=0;
if(pb7==mov_d)
pb7=0;
if(pb8==mov_d)
pb8=0;
}

//*******************************************************************


void definir(void)
{
/*redefinir negras*/
if(tn1==mov_o)
tn1=mov_d;
if(tn2==mov_o)
tn2=mov_d;
if(nn1==mov_o)
nn1=mov_d;
if(nn2==mov_o)
nn2=mov_d;
if(an1==mov_o)
an1=mov_d;
if(an2==mov_o)
an2=mov_d;
if(dn==mov_o)
dn=mov_d;
if(rn==mov_o)
rn=mov_d;
if(pn1==mov_o)
pn1=mov_d;
if(pn2==mov_o)
pn2=mov_d;
if(pn3==mov_o)
pn3=mov_d;
if(pn4==mov_o)
pn4=mov_d;
if(pn5==mov_o)
pn5=mov_d;
if(pn6==mov_o)
pn6=mov_d;
if(pn7==mov_o)
pn7=mov_d;
if(pn8==mov_o)
pn8=mov_d;

/*redefinir blancas*/
if(tb1==mov_o)
tb1=mov_d;
if(tb2==mov_o)
tb2=mov_d;
if(nb1==mov_o)
nb1=mov_d;
if(nb2==mov_o)
nb2=mov_d;
if(ab1==mov_o)
ab1=mov_d;
if(ab2==mov_o)
ab2=mov_d;
if(db==mov_o)
db=mov_d;
if(rb==mov_o)
rb=mov_d;
if(pb1==mov_o)
pb1=mov_d;
if(pb2==mov_o)
pb2=mov_d;
if(pb3==mov_o)
pb3=mov_d;
if(pb4==mov_o)
pb4=mov_d;
if(pb5==mov_o)
pb5=mov_d;
if(pb6==mov_o)
pb6=mov_d;
if(pb7==mov_o)
pb7=mov_d;
if(pb8==mov_o)
pb8=mov_d;
}

//*******************************************************************

void mensajef(void)
{
printf("\n\n\t\t EL JUEGO TERMINO, SE HA CAPTURADO EL REY ");
if(rb==0)
printf("BLANCO\n\t\t NEGRAS GANARON");
else
printf("NEGRO \n\t\t BLANCAS GANARON");
printf("\n\t\t GRACIAS POR USAR OMEGA_CHESS(C). PROPIEDAD DE Omega_Soft(c)\n\t\t\t\t\t\t  sian@correoweb.com");
}


void pass(void)
{
int a,b,c;
clrscr();
printf("\n\n\t\tDEBIDO A CUESTIONES PERSONALES Y LEGALES\n\t\tNECESITO QUE ANTES DE USAR MI PROGRAMA");
printf("  \n\t\tLEAS EL ARCHIVO LEEME.TXT EL CUAL CONTIENE\n\t\tEL PASSWORD, ASI QUE PRIMERO LEELO,");
printf("  \n\t\tY SI HACEPTAS MIS CONDICIONES DE USO.\n\t\tDISFRUTA MI PROGRAMA.");

gotoxy(15,15);
printf("PASSWORD:");


  gotoxy(24,15);
  printf("\"       \"		");
  gotoxy(25,15);
  a=getch();
  printf("P");
  b=getch();
  printf("A");
  c=getch();
  printf("S");
  d=getch();
  printf("S");
  e=getch();
  printf("K");
  f=getch();
  printf("E");
  g=getch();
  printf("Y");

  if(a==115 && b==110 && c==111 && d==114 && e==108 && f==97 && g==120)
		 {
		 printf("\"  PASSWORD CORRECTO.\n\n\n\tUSTED HACEPTA MIS CONDICIONES.");
		 password=1;
		 }

  if(a!=115 || b!=110 || c!=111 || d!=114 || e!=108 || f!=97 || g!=120)
		 {
		 printf("\"  PASSWORD INCORRECTO.");
		 password=0;
		 }
getch();
}