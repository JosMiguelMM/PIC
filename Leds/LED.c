#include <18F4550.h>
#fuses     NOPROTECT,NOPROTECT, NOWDT, PUT, NOCPD, NODEBUG, NOLVP, NOMCLR, HS
#use       delay(clock=20000000)
#include   <stdlib.h>  //LIBRERIAS MATEMATICAS IGUAL LA DE ABAJO
#include   <math.h>
#define    led PIN_B0  //NOMBRO EL PIN B0 ASI Igual en la sigiente linea
#define    pulsador PIN_A0

//Vamos a nombrar los registros de proposito GRAL
int dato, clave[4], i;
//char datom;
//float temperatura, promedio, medida;
//short flag;
//long contador;

/**************************
SECCION DE METODOS
************************/

void prender()
{
  output_high(PIN_B0); //prender led
  delay_ms(300);
  output_high(PIN_B1); //prender led
  delay_ms(300);                            //output_low(PIN_B1); //APAGAR LED
                                           //delay_ms(300);
  output_high(PIN_B2); //prender led
  delay_ms(300);
  output_high(PIN_B3); //prender led
  output_low(PIN_B0); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B4); //prender led
  output_low(PIN_B1); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B5); //prender led
  output_low(PIN_B2); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B6); //prender led
  output_low(PIN_B3); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B7); //prender led
  output_low(PIN_B4); //APAGAR LED
  delay_ms(300);
  
  //Apagado individual
  output_low(PIN_B5); //APAGAR LED
  delay_ms(300);
  output_low(PIN_B6); //APAGAR LED
  delay_ms(300);
  output_low(PIN_B7); //APAGAR LED
  delay_ms(300);
  
  
  //DE regreso hacia atras 
  output_high(PIN_B7); //prender led
  delay_ms(300);
  output_high(PIN_B6); //prender led
  delay_ms(300);
  output_high(PIN_B5); //prender led
  delay_ms(300);
  
  output_high(PIN_B4); //prender led
  output_low(PIN_B7); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B3); //prender led
  output_low(PIN_B6); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B2); //prender led
  output_low(PIN_B5); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B1); //prender led
  output_low(PIN_B4); //APAGAR LED
  delay_ms(300);
  output_high(PIN_B0); //prender led
  output_low(PIN_B3); //APAGAR LED
  delay_ms(300);
  
  //apagado individual
  output_low(PIN_B2); //APAGAR LED
  delay_ms(300);
  output_low(PIN_B1); //APAGAR LED
  delay_ms(300);
  output_low(PIN_B0); //APAGAR LED
  delay_ms(300);
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
    
    output_A(0x00);// limpiar el puerto A
    output_B(0x00);//Lim�piar el puerto B
    
    prender();
    delay_ms(0);
    reset_cpu();//resetear programa
}
