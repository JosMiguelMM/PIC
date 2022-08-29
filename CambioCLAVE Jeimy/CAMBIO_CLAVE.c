#include   <18F4550.h>
#fuses     HS,NOPROTECT,NOBROWNOUT,NOWDT,PUT,NOCPD,NODEBUG,NOLVP,NOMCLR 
#use       delay(clock=20000000)

#define  LCD_E pin_E2
#define  LCD_CK pin_E1 
#define  LCD_DAT pin_E0

//Incluimos las librerias

#include   <LCD4x20_3PIN.c>   //Libreria del LCD
#include   <KBD18F.c>  //Libreria del teclado
#include   <24C512.c>
#include   <ds1307.c>
#ROM    0xf00000={0x3455}
int clave[4], datoi[4], datoe[4];
long auxsegundos=0;
long segundos=0;
int i, b=6, dato1, dato2, direccion;
char datom;
short flag;

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

//Metodo para escribir un digito
void un_digito()
{    lcd_putc('\f');
     flag=false;  //=0
     auxsegundos=0;
     segundos=0;
     do
     {
      datom=kbd_getc();
      delay_ms(60);
      if(datom!='\0')
       {
       lcd_gotoxy(8,1);
       lcd_putc(datom);
       datom=datom-0x30;
       flag=true;
       }else{
       auxsegundos ++;
       if(auxsegundos>20)
       {
        segundos++;
        auxsegundos=0;
       }
      }
      }while(segundos<6 & flag==false);
}

//Metodo para escribir la memoria interna
void escribir_int()
{
   for(i=0; i<2; i++)
   {
     write_eeprom(direccion, clave[i]);
     delay_ms(5);
     direccion++;
   }
}

//Metodo para escribir la memoria interna
void memoria_int()
{
   for(i=0; i<2; i++)
   {
     datoi[i]=read_eeprom(i);
     delay_ms(5);
   }
} 

//Metodo para escribir la memoria externa
void escribir_ext()
{
   for(i=0; i<2; i++)
   {
     write_ext_eeprom(direccion, clave[i]);
     direccion++;
     delay_ms(5);
   }
}



//Metodo para escribir la memoria externa
void memoria_ext()
{
   for(i=2; i<4; i++)
   {
     datoe[i]=read_ext_eeprom(i);
     delay_ms(5);
   }
}


//Programa principal
void main()
{
   #zero_ram      //Limpia la memoria ram
   //Configuracion de los puertos      1=in / entrada    0=output / salida
   set_tris_A(0x00);  /* 0000 0011      Se completa con ceros los puertos que no existen
                        RA0 SCL   PIN02
                        RA1 SDA   PIN03
                        RA2 FREE  PIN04
                        RA3 FREE  PIN05
                        RA4 FREE  PIN06
                        RA5 FREE  PIN07
                        RA6 FREE  PIN14
                        RA7 FREE  NO EXISTE            
   */
   
   set_tris_E(0x07); /* 0000 0111      Se completa con ceros los puertos que no existen
                        RE0 E   PIN08
                        RE1 CK  PIN09
                        RE2 DAT PIN10
                        
   */
   set_tris_D(0x00);/* 0000 0000
                        RD0 FREE PIN19
                        RD1 FREE PIN20
                        RD2 FREE PIN21
                        RD3 FREE PIN22
                        RD4 FREE PIN27
                        RD5 FREE PIN28
                        RD6 FREE PIN29
                        RD7 FREE PIN30       
   */
   output_A(0x00);
   output_E(0x00);
   output_D(0x00);
   
   lcd_init();
   kbd_init();
   
   
   //Limpiamos la pantalla
   lcd_Putc('\f');
   //Instruccion para que me imprima el mensaje
   lcd_gotoxy(2,1); //Me escribe el mensaje en la primera linea
   lcd_putc("BIENVENIDOS"); //Mensaje
   lcd_gotoxy(1,2); //Me escribe el mensaje en la segunda linea
   lcd_putc("ARQUITECTURA T");
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
   lcd_putc("POR FAVOR"); //Mensaje
   lcd_gotoxy(1,2); //Me escribe en la segunda linea posicion 4
   lcd_putc("INGRESE CLAVE");
   delay_ms(3000);
   
   // Llama al metodo teclado y pregunta si la clave ingresada es correcta
      
   memoria_int();
   memoria_ext();
   teclado();
   if(clave[0]==0x43 & clave[1]==0x89)  // Clave 4389
   {
menu:
      lcd_putc('\f');
      lcd_gotoxy(2,1);
      lcd_putc("BIENVENIDOS");
      lcd_gotoxy(3,2);
      lcd_putc("MENU PRINCIPAL");
      delay_ms(3000);
         
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("SELECCIONE MENU");
      lcd_gotoxy(3,2);
      lcd_putc("1. MENU JEIMY");
      delay_ms(3000);
      
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("2. MENU INVITADO");
      lcd_gotoxy(3,2);
      lcd_putc("3. SALIR");
      delay_ms(3000);
      
   un_digito();
   if(datom==0x01)
    {
      goto jeimy;
    }
   if(datom==0x02)
    {
      goto invitado;
    }
   if(datom==0x03)
    {
      reset_cpu();
    }
  // Mensaje de error seleccionando la opcion
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("ESTA OPCION");
      lcd_gotoxy(3,2);
      lcd_putc("NO ESTA EN EL MENU");
      delay_ms(3000);
      lcd_putc('\f');
      lcd_gotoxy(0,1);
      lcd_putc("POR FAVOR VUELVE");
      lcd_gotoxy(1,2);
      lcd_putc("A INTENTARLO");
      delay_ms(3000);
      goto menu;
      
   }
   
   if(clave[0]==datoi[1] && clave[1]==datoi[0])
   {
jeimy:
      direccion=0x00;
      //Opciones del menu
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("MENU DE JEIMY");
      lcd_gotoxy(0,2);
      lcd_putc("1. CAMBIAR CLAVE");
      delay_ms(3000);
        
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("2. FECHA Y HORA");
      lcd_gotoxy(0,2);
      lcd_putc("3. SALIR");
      delay_ms(3000);
    
      un_digito();

    if(datom==0x01)
    {
cambio:
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("MENU PARA");
      lcd_gotoxy(0,2);
      lcd_putc("CAMBIAR CLAVE");
      delay_ms(3000);
   
      lcd_putc('\f');
      lcd_gotoxy(0,1);
      lcd_putc("POR FAVOR DIGITE");
      lcd_gotoxy(1,2);
      lcd_putc("SU NUEVA CLAVE");
      delay_ms(3000);
        
      teclado();
      dato1=clave[0];
      dato2=clave[1];
        
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("DIGITE LA CLAVE");
      lcd_gotoxy(2,2);
      lcd_putc("NUEVAMENTE");
      delay_ms(3000);
      teclado();

    if(clave[0]==dato1 && clave[1]==dato2)
    {
      //Llamamos al metodo para escribir la EEPROM interna y guarde la clave ahi
        
       escribir_int();
       lcd_putc('\f');
       lcd_gotoxy(1,1);
       lcd_putc("CLAVE MODIFICADA");
       lcd_gotoxy(0,2);
       lcd_putc("EXITOSAMENTE");
       delay_ms(3000);
       }
       else
       {
       lcd_putc('\f');
       lcd_gotoxy(1,1);
       lcd_putc("ERROR AL CAMBIAR");
       lcd_gotoxy(0,2);
       lcd_putc("LA CLAVE");
       delay_ms(3000);
       goto cambio;
       }
      }//Fin de la opcion cambiar clave
       if(datom==0x02)
       {
       lcd_putc('\f');
       lcd_gotoxy(1,1);
       lcd_putc("MENU FECHA, HORA");
       lcd_gotoxy(0,2);
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto jeimy;
       }
       if(datom==0x03)
       {
        goto menu;
       }//Fin del menu jeimy
   }

   if (clave[0]==datoe[2] && clave[1]==datoe[3])
   {
        //Inicio menu del invitado
invitado:
      direccion=0x02;
        //Opciones del menu
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("MENU DE INVITADO");
      lcd_gotoxy(0,2);
      lcd_putc("1. CAMBIAR CLAVE");
      delay_ms(3000);
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("2. FECHA Y HORA");
      lcd_gotoxy(0,2);
      lcd_putc("3. SALIR");
      delay_ms(3000);
      un_digito();

    if(datom==0x01)
    {
cambio1:
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("MENU PARA");
      lcd_gotoxy(0,2);
      lcd_putc("CAMBIAR CLAVE");
      delay_ms(3000);
   
      lcd_putc('\f');
      lcd_gotoxy(0,1);
      lcd_putc("POR FAVOR DIGITE");
      lcd_gotoxy(1,2);
      lcd_putc("SU NUEVA CLAVE");
      delay_ms(3000);
         
      teclado();
      dato1=clave[0];
      dato2=clave[1];
         
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      lcd_putc("DIGITE LA CLAVE");
      lcd_gotoxy(2,2);
      lcd_putc("NUEVAMENTE");
      delay_ms(3000);
      teclado();

     if(clave[0]==dato1 & clave[1]==dato2)
     {
      //Llamamos al metodo para escribir la EEPROM externa y guarde la clave ahi
       escribir_ext();
       lcd_putc('\f');
       lcd_gotoxy(1,1);
       lcd_putc("CLAVE MODIFICADA");
       lcd_gotoxy(0,2);
       lcd_putc("EXITOSAMENTE");
       delay_ms(3000);
     }
     else
     {
       lcd_putc('\f');
       lcd_gotoxy(1,1);
       lcd_putc("ERROR AL CAMBIAR");
       lcd_gotoxy(0,2);
       lcd_putc("LA CLAVE");
       delay_ms(3000);
       goto cambio1;
     }
       goto invitado;
    }//Fin de la opcion cambiar clave
       
     if(datom==0x02)
     {
       lcd_putc('\f');
       lcd_gotoxy(1,1);
       lcd_putc("MENU FECHA, HORA");
       lcd_gotoxy(0,2);
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto invitado;
     }
     if(datom==0x03)
     {
       goto menu;
    }// Fin del menu del invitado      
   }
       //Error de clave
       lcd_putc('\f');
       lcd_gotoxy(4,1);
       lcd_putc("CLAVE");
       lcd_gotoxy(2,2);
       lcd_putc("INCORRECTA");
       delay_ms(3000);
       reset_cpu();
}
