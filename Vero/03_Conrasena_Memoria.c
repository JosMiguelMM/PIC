#include <18F4550.h>
#fuses     HS,NOPROTECT,NOBROWNOUT,NOWDT,PUT,NOCPD,NODEBUG,NOLVP,NOMCLR 
#use       delay(clock=20000000)
#include   <stdlib.h>           //LIBRERIAS MATEMATICAS IGUAL LA DE ABAJO
#include   <math.h>
#define   LCD_E    PIN_E2
#define   LCD_CK   PIN_E1
#define   LCD_DAT  PIN_E0
#include <LCD4x20_3PIN.c>
#include <KBD18F.c>
//#include <DS1307.c>
#include <24C512.c>
#ROM     0xf00000={0x4523}     //colocar 0 para usuario 1, estan al reves primero esta los dos digitos ultimos y luego los otros
#ROM     0xf00002={0x5634}     //esto es asignacion de claves para los usuarios

int i, b, dato, clave[4], datoi[4], datoe[4], adres,  dato1, dato2;
char datom;
float valor;
short flag;
long segundos, auxsegundos;

void teclado()
{ 
     lcd_putc('\f');
    segundos=0;
    auxsegundos=0;
    b=6;
     for(i=0;i<4; i++)
      {
        do{
            datom=kbd_getc(); // escanea teclado
            delay_ms(60);
             if(datom!='\0')  //si hay dato valido
              {
                 switch (i+1)
                  {
                   case 1:
                   lcd_gotoxy(6,2);
                   lcd_putc(datom);
                   //printf(lcd_putc,datom);
                   clave[0]=datom-0X30;
                   swap(clave[0]);
                   lcd_gotoxy(6,1);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
          
                   case 2:
                    lcd_gotoxy(7,2);
                   lcd_putc(datom);
                  // printf(lcd_putc,datom);
                   clave[1]=datom-0X30;
                   clave[0]=clave[0]+clave[1];
                   lcd_gotoxy(7,1);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
             
                   case 3:
                   lcd_gotoxy(8,2);
                   lcd_putc(datom);
                   //printf(lcd_putc,datom);
                   clave[2]=datom-0X30;
                   swap(clave[2]);
                   lcd_gotoxy(8,1);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
             
                   case 4:
                   lcd_gotoxy(9,2);
                   lcd_putc(datom);
                  // printf(lcd_putc,datom);
                   clave[3]=datom-0X30;
                   clave[1]=clave[2]+clave[3];
                   lcd_gotoxy(9,1);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
                 } 
                  i++;
                  delay_ms(500);
                  auxsegundos=0;
                  segundos=0;
           }
           auxsegundos++;
           if(auxsegundos > 20)
            {
              segundos++;
              auxsegundos=0;
            }
        
     
   
    }while(i<4 && segundos<6);
      clave[i]='#';
      lcd_gotoxy(b,1);
      printf(lcd_putc,"*") ;
      b++;
      auxsegundos=0;
      segundos=0;
   }
}


void un_digito()
{ lcd_putc('\f');
  segundos=0;
  auxsegundos=0;
  flag=false;
  do{
      datom=kbd_getc();
      delay_ms(60);
      if(datom !='\0')
        {
          lcd_gotoxy(8,1);  //coloque el numero digitado  en posicion del LCD
          lcd_putc(datom); //visualice datom
          datom=datom-0x30; //quitele ASCII
          flag=true;
          
        }else {
               auxsegundos++;
               if(auxsegundos>20)
               {
                 segundos++;
                 auxsegundos=0;
                }
       }
        
  }while(segundos<6 && flag==false );
}

void escribir_memoria()
{
  for(i=0; i<2; i++)
  {  write_eeprom(adres, clave[i]);
     delay_ms(5);
     adres++;
  }
}

void escribir_memoria_ext()
{
  for(i=0; i<2; i++)
  {  write_ext_eeprom(adres, clave[i]);
     delay_ms(5);
     adres++;
  }
}


void leer()
{
  for(i=0; i<4; i++)
  {
    datoi[i]=read_eeprom(i);
    delay_ms(1);
  }
}


void leer_men_ext()
{
  for(i=0; i<4; i++)
  {
    datoe[i]=read_ext_eeprom(i);
    delay_ms(1);
   }
}

   
   
   
/********************
Programa Principal
********************/

void main()
{
inicio:
   #zero_ram
    set_tris_A(0x28);         /*0010 1000 EL 28 SALE ES DIVIDIENDO LOS 8BITS EN 4 DIGITOS Y TOMAR A QUE NUMERO CORRESPONCEN
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
    output_A(0x00);                            //Limpiar el puerto A
    output_B(0x00);                           //Limipiar el puerto B
    output_D(0x00);                           //Limipiar el puerto D

menu:
   lcd_putc('\f');   //Limpiar pantalla
   lcd_gotoxy(1,1);
   lcd_putc("  BIENVENIDOS");
   lcd_gotoxy(1,2);
   lcd_putc("ARQ. COMP. NOCHE");
   delay_ms(3000);
   
   printf(lcd_putc,"\f CAMILO PINLLA\n VERONICA FORERO");
   delay_ms(3000);
   
   printf(lcd_putc,"\f    POR FAVOR\n  DIGITE CLAVE");
   delay_ms(3000);
   
   
   leer();                                    //datoi[0]= clave que le demos datoi[1]=clave que le demos
   leer_men_ext();
   teclado(); 
   if(clave[0]==0x15 &  clave[1]==0x52)
   {admin:
      printf(lcd_putc,"\f BIENV. AL MENU\n DEL DISENADOR");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 1 PARA:\n MENU DE CAMILO");
      delay_ms(3000); 
      printf(lcd_putc,"\f MARQUE 2 PARA:\n MENU VERONICA");
      delay_ms(3000);  
      printf(lcd_putc,"\f MARQUE 3 PARA:\n SALIR");
      delay_ms(3000);  
      
      un_digito();  //datom
      
      if(datom==0x01)
      {
         //Leer_MemoriaInt();
         goto  camilo;
      }
      
     else if(datom==0x02)
      {
         goto  vero;
      }
      
     else if(datom==0x03)
      {
         reset_cpu();
      }else{
      
             printf(lcd_putc,"\f ERROR DE NUMERO:\n  ");
             delay_ms(3000);
             goto  admin;}
   }
   
  else  if(clave[0]==datoi[1] &  clave[1]==datoi[0])
   {
camilo: adres=0x00;
      printf(lcd_putc,"\f BIENV. AL MENU\n DE CAMILO");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 1 PARA \n CAMBIAR CLAVE");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 2 PARA \n FECHA Y HORA");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 3 PARA \n REPORTES");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 4 PARA \n MTR PASO A PASO");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 5 PARA \n SERVOMOTOR");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 6 PARA \n CAD");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 7 PARA \n DTFM");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 8 PARA \n SENSORES");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 9 PARA \n SALIR");
      delay_ms(3000);

      un_digito();
      
      if(datom==0x01)
     {
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("MEN CAMBIAR CLAV");     //lo que se escribe
       lcd_gotoxy(3,2);                  //escribir en a primera fila primera posicion 
       lcd_putc("DIGITE NU. CLAVE");     //clave[0]= y clave[1]=
       delay_ms(3000);
       teclado();
       
       dato1=clave[0];
       dato2=clave[1];
       
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("POR FAVOR DIGITE");     //lo que se escribe
       lcd_gotoxy(1,2);                  //escribir en a primera fila primera posicion 
       lcd_putc("DIGITE NUE.CLAVE");   
       delay_ms(3000);
       
       teclado();
       
       if(clave[0]==dato1 & clave[1]==dato2)
       {
         escribir_memoria();
         lcd_putc('\f');                   //Limpiar pantalla comando
         lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
         lcd_putc("CLAVE MODIFICADA");     //lo que se escribe
         lcd_gotoxy(3,2);                  //escribir en a primera fila primera posicion 
         lcd_putc("CON EXITO");   
         delay_ms(3000);
          goto camilo;
       }
       else{
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("ERROR DE CLAVE");     //lo que se escribe
       delay_ms(3000);
       goto camilo;
       }
       
     }
     else if(datom==0x02)
     {
       lcd_putc('\f');                  //Limpiar pantalla comando
       lcd_gotoxy(1,1);                 //escribir en a primera fila primera posicion 
       lcd_putc("MEN FECHA Y HORA");    //lo que se escribe
       lcd_gotoxy(3,2);                 //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;
     }
    else  if(datom==0x03)
     {
       lcd_putc('\f');                //Limpiar pantalla comando
       lcd_gotoxy(1,1);               //escribir en a primera fila primera posicion 
       lcd_putc("MENU REPORTES");     //lo que se escribe
       lcd_gotoxy(3,2);               //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;;
     }
    else if(datom==0x04)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MEN MOTOR PAS A."); //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;
     }
    else if(datom==0x05)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SERVOMOTOR");  //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;
     }
    else if(datom==0x06)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MENU CAD");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;
     }
    else if(datom==0x07)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MEN DTMF");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;
     }
    else if(datom==0x08)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SENSORES");    //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto camilo;
     }
    else if(datom==0x09)
     {
      goto admin;
     }
     else{
          lcd_putc('\f');                 //Limpiar pantalla comando
          lcd_gotoxy(1,2);                //escribir en a primera fila primera posicion 
          lcd_putc("ERROR DE NUMERO");   //lo que se escribe
          delay_ms(3000);
          goto menu;
      }
   }
   else if(clave[0]==datoi[3] &  clave[1]==datoi[2])
   {
vero:   adres=0x02;
      printf(lcd_putc,"\f BIENV. AL MENU\n DE VERONICA");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 1 PARA \n CAMBIAR CLAVE");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 2 PARA \n FECHA Y HORA");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 3 PARA \n REPORTES");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 4 PARA \n MTR PASO A PASO");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 5 PARA \n SERVOMOTOR");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 6 PARA \n CAD");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 7 PARA \n DTFM");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 8 PARA \n SENSORES");
      delay_ms(3000);
      printf(lcd_putc,"\f MARQUE 9 PARA \n SALIR");
      delay_ms(3000);
     
      
     un_digito();
      if(datom==0x01)
     {
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("MEN CAMBIAR CLAV");     //lo que se escribe
       lcd_gotoxy(3,2);                  //escribir en a primera fila primera posicion 
       lcd_putc("DIGITE NU. CLAVE");     //clave[0]= y clave[1]=
       delay_ms(3000);
       teclado();
       
       dato1=clave[0];
       dato2=clave[1];
       
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("POR FAVOR DIGITE");     //lo que se escribe
       lcd_gotoxy(1,2);                  //escribir en a primera fila primera posicion 
       lcd_putc("DIGITE NUE.CLAVE");   
       delay_ms(3000);
       
       teclado();
       
       if(clave[0]==dato1 & clave[1]==dato2)
       {
        escribir_memoria();
        lcd_putc('\f');                   //Limpiar pantalla comando
        lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
        lcd_putc("CLAVE MODIFICADA");     //lo que se escribe
        lcd_gotoxy(3,2);                  //escribir en a primera fila primera posicion 
        lcd_putc("CON EXITO");   
        delay_ms(3000);
          goto vero;
       }
       else{
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("ERROR DE CLAVE");     //lo que se escribe
       delay_ms(3000);
       goto vero;
       }
       
     }
     else if(datom==0x02)
     {
       lcd_putc('\f');                  //Limpiar pantalla comando
       lcd_gotoxy(1,1);                 //escribir en a primera fila primera posicion 
       lcd_putc("MEN FECHA Y HORA");    //lo que se escribe
       lcd_gotoxy(3,2);                 //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;
     }
    else  if(datom==0x03)
     {
       lcd_putc('\f');                //Limpiar pantalla comando
       lcd_gotoxy(1,1);               //escribir en a primera fila primera posicion 
       lcd_putc("MENU REPORTES");     //lo que se escribe
       lcd_gotoxy(3,2);               //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;;
     }
    else if(datom==0x04)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MEN MOTOR PAS A."); //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;
     }
    else if(datom==0x05)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SERVOMOTOR");  //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;
     }
    else if(datom==0x06)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MENU CAD");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;
     }
    else if(datom==0x07)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MEN DTMF");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;
     }
    else if(datom==0x08)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SENSORES");    //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto vero;
     }
    else if(datom==0x09)
     {
      goto admin;
     }
     else{
          lcd_putc('\f');                 //Limpiar pantalla comando
          lcd_gotoxy(1,2);                //escribir en a primera fila primera posicion 
          lcd_putc("ERROR DE NUMERO");   //lo que se escribe
          delay_ms(3000);
          goto menu;
      }
     }else{
          lcd_putc('\f');                 //Limpiar pantalla comando
          lcd_gotoxy(1,2);                //escribir en a primera fila primera posicion 
          lcd_putc("CONTRA. ERRONEA");   //lo que se escribe
          delay_ms(3000);
          goto menu;
     }
   }

