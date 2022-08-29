#include <18F4550.h>
#fuses     HS,NOPROTECT,NOPROTECT, NOWDT, PUT, NOCPD, NODEBUG, NOLVP, NOMCLR
#use       delay(clock=20000000)
#include   <stdlib.h>  //LIBRERIAS MATEMATICAS IGUAL LA DE ABAJO
#include   <math.h>
#define   LCD_E    PIN_E2
#define   LCD_CK   PIN_E1
#define   LCD_DAT  PIN_E0
#include <LCD4x20_3PIN.c>
#include <KBD18F.c>
//#include <DS1307.c>

#define SW PIN A5
//Vamos a nombrar los registros de proposito GRAL
//int dato, clave[4], i;
//char datom;
//float temperatura, promedio, medida;
//short flag;
//long contador;

/**************************
SECCION DE METODOS
************************/

void prender()
{
  
}
  
/**************************
PROGRAMA PRINCIPAL
************************/  

void main()
{#zero_ram
inicio:   //etiquetas siempre a la izquierda
    set_tris_A(0x28); /*0010 1000 EL 28 SALE ES DIVIDIENDO LOS 8BITS EN 4 DIGITOS Y TOMAR A QUE NUMERO CORRESPONCEN
    RA0 PIN 2  FREE
    RA1 PIN 3  FREE
    RA2 PIN 4  FREE
    RA3 PIN 5  FREE
    RA4 PIN 6  FREE
    RA5 PIN 7  FREE
    RA6 PIN 14 FREE
    RA7 NO EXISTE */
    
    set_tris_B(0x00); /*0000 0000 EL 28 SALE ES DIVIDIENDO LOS 8BITS EN 4 DIGITOS Y TOMAR A QUE NUMERO CORRESPONCEN
    RB0 PIN 33  LED1
    RB1 PIN 34  LED2
    RB2 PIN 35  LED4
    RB3 PIN 36  LED5
    RB4 PIN 37  LED6
    RB5 PIN 38  LED7
    RB6 PIN 39  LED8
    RB7 PIN 40  LED8 */
    
    set_tris_D(0x00); /*0000 0000 EL 28 SALE ES DIVIDIENDO LOS 8BITS EN 4 DIGITOS Y TOMAR A QUE NUMERO CORRESPONCEN
    RB0 PIN 33  LED1
    RB1 PIN 34  LED2
    RB2 PIN 35  LED4
    RB3 PIN 36  LED5
    RB4 PIN 37  LED6
    RB5 PIN 38  LED7
    RB6 PIN 39  LED8
    RB7 PIN 40  LED8 */
    
    lcd_init();
    kbd_init();
    output_A(0x00);//Limpiar el puerto A
    output_B(0x00);//Limipiar el puerto B
    output_D(0x00);//Limipiar el puerto D
   
menu:
   lcd_putc('\f'); //Limpiar pantalla comando
   lcd_gotoxy(1,1); //escribir en a primera fila primera posicion 
   lcd_putc("   BIENVENIDOS  ");//lo que se escribe
   lcd_gotoxy(1,2); //escribir en a primera fila primera posicion 
   lcd_putc("ARQ. COMP. TARDE");
   delay_ms(3000000);
   
   lcd_putc('\f'); //Limpiar pantalla comando
   lcd_gotoxy(1,1); //escribir en a primera fila primera posicion 
   lcd_putc("          MIGUEL MENDEZ");//lo que se escribe
   lcd_gotoxy(1,2); //escribir en a primera fila primera posicion 
   lcd_putc(" DAVID MARTINEZ ");
   //printf(lcd_putc,"\f   MIGUEL MENDEZ\n DAVID MARTINEZ ");// /n pase a sigueinet linea
   delay_ms(3000000);
   
   lcd_putc('\f'); //Limpiar pantalla comando
   lcd_gotoxy(1,1); //escribir en a primera fila primera posicion 
   lcd_putc("          GRUPO");//lo que se escribe
   lcd_gotoxy(1,2); //escribir en a primera fila primera posicion 
   lcd_putc(" NUMERO");
   //printf(lcd_putc,"\f        GRUPO   \n       NUMERO    ");// /n pase a sigueinet linea
   delay_ms(3000000);
   
   lcd_putc('\f'); //Limpiar pantalla comando
   lcd_gotoxy(1,1); //escribir en a primera fila primera posicion 
   lcd_putc("           POR FAVOR");//lo que se escribe
   lcd_gotoxy(1,2); //escribir en a primera fila primera posicion 
   lcd_putc(" INGRESE CLAVE");
   //printf(lcd_putc,"\f    POR FAVOR   \n INGRESE CLAVE");// /n pase a sigueinet linea
   delay_ms(3000000);
   reset_cpu();//resetear programa
}
