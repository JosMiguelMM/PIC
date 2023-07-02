#include <18F4550.h>
#fuses     HS,NOPROTECT,NOBROWNOUT,NOWDT,PUT,NOCPD,NODEBUG,NOLVP,NOMCLR 
#use       delay(clock=20000000)

#define  LCD_E pin_E2
#define  LCD_CK pin_E1 
#define  LCD_DAT pin_E0

//Incluimos las librerias

#include <LCD4X20_3PIN.c>   //Libreria del LCD
#include <KBD18F.c>  //Libreria del teclado

int clave[4];
long auxsegundos=0;
long segundos=0;
int i, b=6;
char datom;

//Metodo del teclado
void teclado()
{
auxsegundos=0;
segundos=0;
b=6;

lcd_putc('\f');
for(i=0; i<4;i++){
do{
   datom=kbd_getc();//Escanea el teclado
   delay_ms(60);//retardo constante
   if(datom !='\0')// si datom tiene un numero valido
   {
      switch(i+1)
      {
         case 1://funciona como un apuntador
         clave[0]=datom-0x30;//a datom restele el 30 me quita el ascii
         lcd_gotoxy(6,1);
         lcd_putc("*");
         swap(clave[0]);
         delay_ms(50);
         break;
         
         case 2:
         clave[1]=datom-0x30;//a datom restele el 30 me quita el ascii
         lcd_putc("*");
         clave[0]=clave[0]+clave[1];
         delay_ms(50);
         break;
         
         case 3:
         clave[2]=datom-0x30;
         swap(clave[2]);
         lcd_putc("*");
         delay_ms(50);
         break;
         
         case 4: 
         clave[3]=datom-0x30;
         clave[1]=clave[2]+clave[3];
         lcd_putc("*");
         delay_ms(50);
         break;
      }
      i++;
      delay_ms(500);
      auxsegundos=0;
      segundos=0;
   }
   auxsegundos++;
   if(auxsegundos>20)
   {
      auxsegundos=0;
      segundos++;
   }
   //segundos <6 o menor4
}while(segundos<6 && i<4);
      clave[i]="#";
      lcd_gotoxy(b,1);//me apunta a la posicion 6
      lcd_putc("*");
      b++;
      segundos=0;
      auxsegundos=0;
}     //Digite clave
}


//Programa principal
void main()
{
   #zero_ram      //Limpia la memoria ram
   //Configuracion de los puertos      1=in / entrada    0=output / salida
   set_tris_E(0x07); /* 0000 0111      Se completa con ceros los puertos que no existen
                        RE0 E PIN08
                        RE1 CK PIN09
                        RE2 DAT PIN10
                        
   */
   set_tris_B(0x00);/* 0000 0000
                        RB0 FREE PIN19
                        RB1 FREE PIN20
                        RB2 FREE PIN21
                        RB3 FREE PIN22
                        RB4 FREE PIN27
                        RB5 FREE PIN28
                        RB6 FREE PIN29
                        RB7 FREE PIN30       
   */
   
   output_E(0x00);
   output_B(0x00);
   
   lcd_init();
   kbd_init();
   
   
   //Limpiamos la pantalla
   lcd_Putc('\f');
   //Instruccion para que me imprima el mensaje
   lcd_gotoxy(1,1); //Me escribe el mensaje en la primera linea
   lcd_putc(" BIENVENIDOS  "); //Mensaje
   lcd_gotoxy(1,2); //Me escribe el mensaje en la segunda linea
   lcd_putc(" ARQUITECTURA T");
   delay_ms(3000);
   
   //
   lcd_putc('\f'); //Me limpia la pantalla
   lcd_gotoxy(4,1); //Me escribe en la primera linea posicion 5
   lcd_putc("JEIMY"); //Mensaje
   lcd_gotoxy(3,2); //Me escribe en la segunda linea posicion 4
   lcd_putc("HERRERA R");
   delay_ms(3000);
   
   lcd_putc('\f');
   lcd_gotoxy(4,1); //Me escribe en la primera linea posicion 5
   lcd_putc("GRUPO"); //Mensaje
   lcd_gotoxy(3,2); //Me escribe en la segunda linea posicion 4
   lcd_putc("NUMERO 9");
   delay_ms(3000);
   
   // Mensaje para pedir la clave
   lcd_putc('\f');
   lcd_gotoxy(2,1); //Me escribe en la primera linea posicion 5
   lcd_putc("   POR FAVOR   "); //Mensaje
   lcd_gotoxy(1,2); //Me escribe en la segunda linea posicion 4
   lcd_putc(" INGRESE CLAVE ");
   delay_ms(3000);
   
   // Llama al metodo teclado y pregunta si la clave ingresada es correcta
      teclado(); //
      if(clave[0]==0x43 & clave[1]==0x89)  // Clave 4389
      {
      // Mensaje de bienvenida
      
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("BIENVENIDOS");
      lcd_gotoxy(3,2);
      lcd_putc("AL MENU");
      delay_ms(3000);
      }else{
      //Error de clave
      lcd_putc('\f');
      lcd_gotoxy(4,1);
      lcd_putc("CLAVE");
      lcd_gotoxy(2,2);
      lcd_putc("INCORRECTA");
      delay_ms(3000);
      }
      delay_ms(3000);
      reset_cpu();
}
