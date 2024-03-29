#include <18F4550.h>
#fuses     HS,NOPROTECT,NOBROWNOUT,NOWDT,PUT,NOCPD,NODEBUG,NOLVP,NOMCLR
#use       delay(clock=20000000)
#include   <stdlib.h>          //LIBRERIAS MATEMATICAS IGUAL LA DE ABAJO
#include   <math.h>
#define   LCD_E    PIN_E2
#define   LCD_CK   PIN_E1
#define   LCD_DAT  PIN_E0
#include <LCD4x20_3PIN.c>       //libreria LCD
#include <KBD18F.c>             //LIBRERIA TECLADO
#include <DS1307A.c>            //LIBRERIA RELOJ
#include <24C512.c>
#ROM     0xf00000={0x4523}     //colocar 0 para usuario 1, estan al reves primero esta los dos digitos ultimos y luego los otros
#ROM     0xf00002={0x5634}     //esto es asignacion de claves para los usuarios
#define SW PIN A5
//Vamos a nombrar los registros de proposito GRAL
int i, b, clave[4], datoi[4], datoe[4], adres,  dato1, dato2, cantidad;
char datom;
float valor;
short flag, espacio_vacio;
long segundos, auxsegundos;
int ide, hour, min, day, mth, year, dow, sec, time[8] ;
int contador, demora=20;

/**************************
SECCION DE METODOS
************************/

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
                   lcd_gotoxy(6,1);
                   lcd_putc(datom);
                   //printf(lcd_putc,datom);
                   clave[0]=datom-0X30;
                   swap(clave[0]);
                   lcd_gotoxy(6,2);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
          
                   case 2:
                   lcd_gotoxy(7,1);
                   lcd_putc(datom);
                  // printf(lcd_putc,datom);
                   clave[1]=datom-0X30;
                   clave[0]=clave[0]+clave[1];
                   lcd_gotoxy(7,2);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
             
                   case 3:
                   lcd_gotoxy(8,1);
                   lcd_putc(datom);
                   //printf(lcd_putc,datom);
                   clave[2]=datom-0X30;
                   swap(clave[2]);
                   lcd_gotoxy(8,2);
                   printf(lcd_putc,"*");
                   delay_ms(50);
                   break;
             
                   case 4:
                   lcd_gotoxy(9,1);
                   lcd_putc(datom);
                  // printf(lcd_putc,datom);
                   clave[3]=datom-0X30;
                   clave[1]=clave[2]+clave[3];
                   lcd_gotoxy(9,2);
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

void hora_fecha(){
   //ds1307_get_date(day, mth, year, dow);
   ds1307_get_date(time[3], time[4], time[5], time[6]);
   lcd_putc('\f');
   lcd_gotoxy(1,1);
   printf(lcd_putc,"DATE:");
   
   for(i=3;i<6;i++){
      printf(lcd_putc,"%2d/",time[i]);
      delay_ms(200);
      
   }
   lcd_gotoxy(16,1);
   lcd_putc("D ");
   
    for(i=0;i<10;i++){
         //ds1307_get_time(hour, min, sec);
         ds1307_get_time(time[0],time[1],time[2]);
         lcd_gotoxy(1,2);
         printf(lcd_putc,"HORA:");
    for(i=0;i<3;i++){
        printf(lcd_putc,"%2d:",time[i]);
        delay_ms(200);
    }
    
      lcd_gotoxy(16,2);
      printf(lcd_putc,"%1d:",time[6]);
      delay_ms(995);
    }
   
}

void actualizar(){
    day=0x0A;          //dia27
    mth=0x0A;          //mes
    year=0x15;         //a�os21
    dow=0x01;          //dow lun
    hour=0x014;        //hora 7
    min=0x6;           //min   50
    sec=0x00;          //sec
    ds1307_set_date_time(day, mth, year, dow, hour, min,sec);
   }


void buscar_pos(){
       espacio_vacio=false;
       adres=0x08;
       contador=0;
     
    do{
         valor=read_ext_eeprom(adres);
         delay_ms(3);
        
        if(valor==0xFF){
            espacio_vacio=true;
           
        }else{
              adres=adres+0x08;
        }
      contador=contador+1;
    }while((adres<=0xF0)  && (espacio_vacio==false));
    //time[0]=contador;
}

void hacer_reporte(){
   ds1307_get_date(day, mth, year, dow );
   ds1307_get_time(hour, min, sec);
    time[0]=contador;
    time[1]= day;
    time[2]= mth;
    time[3]= year;
    time[4]= hour;
    time[5]= min;
    time[6]= sec;
    time[7]= ide;
}




void mostrar_reporte(){
     adres=0x08;
     cantidad=0;
 do{
       for(i=0;i<8;i++){
          time[i]=read_ext_eeprom(adres);
          delay_ms(3);
          adres++;
        }
             lcd_putc('\f');
             lcd_gotoxy(1,1);
             printf(lcd_putc,"##DDMMAAHHMMSSID");
             lcd_gotoxy(1,2);
             printf(lcd_putc,"%2d",time[0]);
             lcd_gotoxy(3,2);
             printf(lcd_putc,"%2d",time[1]);
             lcd_gotoxy(5,2);
             printf(lcd_putc,"%2d",time[2]);
             lcd_gotoxy(7,2);
             printf(lcd_putc,"%2d",time[3]);
             lcd_gotoxy(9,2);
             printf(lcd_putc,"%2d",time[4]);
             lcd_gotoxy(11,2);
             printf(lcd_putc,"%2d",time[5]);
             lcd_gotoxy(13,2);
             printf(lcd_putc,"%2d",time[6]);
             lcd_gotoxy(15,2);
             printf(lcd_putc,"%2d",time[7]);
             delay_ms(3000);
             
       }while(cantidad<=50);
}

void borrar(){
   for(i=0;i<80;i++){   
      write_ext_eeprom(i, 0xFF);
      delay_ms(2);
    } 
  }
  
void motorderecha(){
  for(i=0;i<256;i++){
  
     output_high(PIN_D0);           //paso 1   
     output_high(PIN_D1);          
     output_low(PIN_D2);           
     output_low(PIN_D3);           
     delay_ms(demora);
     
     output_low(PIN_D0);            //paso 2
     output_high(PIN_D1);          
     output_high(PIN_D2);          
     output_low(PIN_D3);           
     delay_ms(demora);
     
     output_low(PIN_D0);           //paso 3
     output_low(PIN_D1);          
     output_high(PIN_D2);          
     output_high(PIN_D3);          
     delay_ms(demora);
     
     
     output_high(PIN_D0);          //paso 4
     output_low(PIN_D1);           
     output_low(PIN_D2);           
     output_high(PIN_D3);          
     delay_ms(demora);
     
   }
   
     output_low(PIN_D0);          
     output_low(PIN_D1);           
     output_low(PIN_D2);           
     output_low(PIN_D3);          
     delay_ms(5000);
 }
      
void motorizquierda(){
   for(i=0;i<256;i++)
   {
     output_low(PIN_D0);          //paso 4
     output_low(PIN_D1);           
     output_low(PIN_D2);           
     output_high(PIN_D3);          
     delay_ms(demora);
     
     output_low(PIN_D0);           //paso 3
     output_low(PIN_D1);          
     output_high(PIN_D2);          
     output_low(PIN_D3);          
     delay_ms(demora);
     
     output_low(PIN_D0);            //paso 2
     output_high(PIN_D1);          
     output_low(PIN_D2);          
     output_low(PIN_D3);           
     delay_ms(demora);
     
     output_high(PIN_D0);           //paso 1   
     output_low(PIN_D1);          
     output_low(PIN_D2);           
     output_low(PIN_D3);           
     delay_ms(demora);
     
     }
        output_low(PIN_D0);          
        output_low(PIN_D1);           
        output_low(PIN_D2);           
        output_low(PIN_D3);          
        delay_ms(5000);
}     
  
/**************************
PROGRAMA PRINCIPAL
************************/  

void main()
{#zero_ram
inicio:                       //etiquetas siempre a la izquierda
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
  
    //actualizar();
    // borrar();
menu:
     lcd_putc('\f');                            //Limpiar pantalla comando
     lcd_gotoxy(1,1);                           //escribir en a primera fila primera posicion 
     lcd_putc("   BIENVENIDOS  ");              //lo que se escribe
     lcd_gotoxy(1,2);                           //escribir en a primera fila primera posicion 
     lcd_putc("ARQ. COMP. TARDE");
     delay_ms(3000);
   
     lcd_putc('\f');                            //Limpiar pantalla comando
     lcd_gotoxy(1,1);                           //escribir en a primera fila primera posicion 
     lcd_putc("MIGUEL MENDEZ");                 //lo que se escribe
     lcd_gotoxy(1,2);                           //escribir en a primera fila primera posicion 
     lcd_putc(" DAVID MARTINEZ ");
     //printf(lcd_putc,"\f   MIGUEL MENDEZ\n DAVID MARTINEZ ");// /n pase a sigueinet linea
     delay_ms(3000);
   
     lcd_putc('\f');                           //Limpiar pantalla comando
     lcd_gotoxy(3,1);                          //escribir en a primera fila primera posicion 
     lcd_putc("GRUPO");                        //lo que se escribe
     lcd_gotoxy(2,2);                          //escribir en a primera fila primera posicion 
     lcd_putc(" NUMERO");
     //printf(lcd_putc,"\f        GRUPO   \n       NUMERO    ");// /n pase a sigueinet linea
     delay_ms(3000);
   
     lcd_putc('\f');                          //Limpiar pantalla comando
     lcd_gotoxy(2,1);                         //escribir en a primera fila primera posicion 
     lcd_putc("POR FAVOR");                   //lo que se escribe
     lcd_gotoxy(1,2);                         //escribir en a primera fila primera posicion 
     lcd_putc("INGRESE CLAVE");
     //printf(lcd_putc,"\f    POR FAVOR   \n INGRESE CLAVE");// /n pase a sigueinet linea
     delay_ms(3000);
   
   
   
     leer();                                    //datoi[0]= clave que le demos datoi[1]=clave que le demos
     leer_men_ext();
     teclado();                                 //clave [0]=0x34, clave[1]=0x78
     
 if(clave[0]==0x12 & clave[1]==0x34)
   {
admin: 
      lcd_putc('\f');                           //Limpiar pantalla comando
      lcd_gotoxy(1,1);                          //escribir en a primera fila primera posicion 
      lcd_putc("BIENVENIDOS AL");               //lo que se escribe
      lcd_gotoxy(4,2);                          //escribir en a primera fila primera posicion 
      lcd_putc(" MENU");
      //printf(lcd_putc,"\f    POR FAVOR   \n INGRESE CLAVE");// /n pase a sigueinet linea
      delay_ms(3000);
     
      lcd_putc('\f');                         //Limpiar pantalla comando
      lcd_gotoxy(1,1);                        //escribir en a primera fila primera posicion 
      lcd_putc("MARQUE 1 PARA EL");           //lo que se escribe
      lcd_gotoxy(1,2);                        //escribir en a primera fila primera posicion 
      lcd_putc("MENU DE DAVID");
      //printf(lcd_putc,"\f    POR FAVOR   \n INGRESE CLAVE");// /n pase a sigueinet linea
      delay_ms(3000);
   
      lcd_putc('\f');                         //Limpiar pantalla comando
      lcd_gotoxy(1,1);                        //escribir en a primera fila primera posicion 
      lcd_putc("MARQUE 2 PARA EL");           //lo que se escribe
      lcd_gotoxy(1,2);                        //escribir en a primera fila primera posicion 
      lcd_putc("MENU DE MIGUEL");
      //printf(lcd_putc,"\f    POR FAVOR   \n INGRESE CLAVE");// /n pase a sigueinet linea
      delay_ms(3000);
    
      lcd_putc('\f');                         //Limpiar pantalla comando
      lcd_gotoxy(1,1);                        //escribir en a primera fila primera posicion 
      lcd_putc("MARQUE 3 PARA ");             //lo que se escribe
      lcd_gotoxy(1,2);                        //escribir en a primera fila primera posicion 
      lcd_putc("SALIR");
      //printf(lcd_putc,"\f    POR FAVOR   \n INGRESE CLAVE");// /n pase a sigueinet linea
       delay_ms(3000);
   
   
       //bienvenidos al menu del dise�ador
       //marque 1 para menu de MIGUEL
       //marque 2 para menu de David
       //marque 3 salir  */
       
       
     un_digito();
    if(datom==0x01)
     {
        goto David;
     }
     if(datom==0x02)
     {
        goto Miguel;
     }
     if(datom==0x03)
     {
        reset_cpu();                        //salir
     }
     lcd_putc('\f');                        //Limpiar pantalla comando
     lcd_gotoxy(1,2);                       //escribir en a primera fila primera posicion 
     lcd_putc("ERROR DE NUMERO");           //lo que se escribe
     delay_ms(3000);
     goto admin;
     } 
     else if(clave[0]==datoe[0] & clave[1]==datoe[1])
      {
Miguel:adres=0x00;
       /*escribirle al lcd
       bienvenidos al menu del dise�ador
       marque 1 para cambiar clave
       marque 2 para fecha y hora
       marque 3 para reportes
       marque 4 para motor paso a paso
       marque 5 para servomotor
       marque 6 para cad
       marque 7 para DTMF
       marque 8 para sensores
       marque 9 para salir*/
     
    lcd_putc('\f');                       //Limpiar pantalla comando
    lcd_gotoxy(1,1);                      //escribir en a primera fila primera posicion 
    lcd_putc("MENU USUAR.");              //lo que se escribe
    lcd_gotoxy(1,2);                      //escribir en a primera fila primera posicion 
    lcd_putc("MIGUEL");
    delay_ms(3000); 
       
    lcd_putc('\f');                       //Limpiar pantalla comando
    lcd_gotoxy(1,1);                      //escribir en a primera fila primera posicion 
    lcd_putc("MARQUE NUM PARA");          //lo que se escribe
    lcd_gotoxy(1,2);                      //escribir en a primera fila primera posicion 
    lcd_putc("1 CAMBIAR CLAVE");
    delay_ms(3000);  
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("2 FECHA Y HORA");          //lo que se escribe
    lcd_gotoxy(1,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("3 REPORTES");
    delay_ms(3000);
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("4 MOTOR PASAPAS");        //lo que se escribe
    lcd_gotoxy(1,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("5 SERVOMOTOR");
    delay_ms(3000);
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("6 CAD");                   //lo que se escribe
    lcd_gotoxy(1,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("7 DTMF");
    delay_ms(3000);
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("8 SENSORES");              //lo que se escribe
    lcd_gotoxy(3,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("9 SALIR");
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
         escribir_memoria_ext();
         lcd_putc('\f');                   //Limpiar pantalla comando
         lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
         lcd_putc("CLAVE MODIFICADA");     //lo que se escribe
         lcd_gotoxy(3,2);                  //escribir en a primera fila primera posicion 
         lcd_putc("CON EXITO");   
         delay_ms(3000);
         goto Miguel;
       }
       else{
            lcd_putc('\f');                   //Limpiar pantalla comando
            lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
            lcd_putc("ERROR DE CLAVE");     //lo que se escribe
            delay_ms(3000);
            goto Miguel;
            }
      
      
     }
     else if(datom==0x02)
     {
       lcd_putc('\f');                  //Limpiar pantalla comando
       lcd_gotoxy(1,1);                 //escribir en a primera fila primera posicion 
       lcd_putc("MEN FECHA Y HORA");    //lo que se escribe
       delay_ms(3000);
       hora_fecha();
       delay_ms(10000);
       goto Miguel;
     }
     else if(datom==0x03)/////////////////////////////////////////
     {
       ide=0x13;
       lcd_putc('\f');                                         //Limpiar pantalla comando
       lcd_gotoxy(1,1);                                        //escribir en a primera fila primera posicion 
       lcd_putc("MENU REPORTES");                              //lo que se escribe
       delay_ms(3000);
       
       lcd_putc('\f');                                         //Limpiar pantalla comando
       lcd_gotoxy(1,1);                                        //escribir en a primera fila primera posicion 
       lcd_putc("1 GENERAR.REPORT");                           //lo que se escribe
       lcd_gotoxy(1,2);                                        //escribir en a primera fila primera posicion 
       lcd_putc("2 MOSTRAR.REPORT");                           //lo que se escribe
       delay_ms(3000);
       un_digito();
     if(datom==0x01){
         buscar_pos();                                           //CONTADOR, ADDRESS, ESPACIO_VACIO=TRU
       if(espacio_vacio==true){
          
            hacer_reporte();
          for(i=0;i<8;i++){
               write_ext_eeprom(adres, time[i]);
               adres++;
               delay_ms(5);
               
          }
           lcd_putc('\f');                    //Limpiar pantalla comando
               lcd_gotoxy(1,1);               //escribir en a primera fila primera posicion 
               lcd_putc("REPORTE GUARDADO");  //lo que se escribe
               lcd_gotoxy(1,2);               //escribir en a primera fila primera posicion 
               lcd_putc("CON EXITO");         //lo que se escribe
               delay_ms(3000);
               goto Miguel;
        }else{
               lcd_putc('\f');               //Limpiar pantalla comando
               lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
               lcd_putc("MEMORIA LLENA");    //lo que se escribe
               delay_ms(3000);
               goto Miguel;
               
              }
     }else if (datom==0x02) {
                mostrar_reporte();
                goto Miguel;
                }
                 else{ lcd_gotoxy(1,1);               //escribir en a primera fila primera posicion 
                       lcd_putc("NUMERO");            //lo que se escribe
                       lcd_gotoxy(1,2);               //escribir en a primera fila primera posicion 
                       lcd_putc("ERROR");             //lo que se escribe
                       delay_ms(3000);
                       goto Miguel;}
              
               
     }
     else if(datom==0x04)
     {
       lcd_putc('\f');                        //Limpiar pantalla comando
       lcd_gotoxy(1,1);                       //escribir en a primera fila primera posicion 
       lcd_putc("MEN MOTOR PAS A.");          //lo que se escribe
       delay_ms(3000);
       
       lcd_putc('\f');                        //Limpiar pantalla comando
       lcd_gotoxy(1,1);                       //escribir en a primera fila primera posicion 
       lcd_putc("1 PARA DERECHA");            //lo que se escribe
       lcd_gotoxy(1,2);                       //escribir en a primera fila primera posicion 
       lcd_putc("2 PARA IZQUIERDA");          //lo que se escribe
       delay_ms(3000);
       
       un_digito();
       if(datom==1)
       {  
          motorderecha();
          goto Miguel;
        }
        else if(datom==2)
          {motorizquierda();
           goto Miguel;
          }else {lcd_putc('\f');                        //Limpiar pantalla comando
                 lcd_gotoxy(1,1);                       //escribir en a primera fila primera posicion 
                 lcd_putc("ERROR DE NUMERO");            //lo que se escribe
                  delay_ms(3000);
                  goto Miguel;
                  }
       
       
       goto Miguel;
     }
     else if(datom==0x05)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SERVOMOTOR");  //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto Miguel;
     }
     else if(datom==0x06)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MENU CAD");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto Miguel;
     }
     else if(datom==0x07)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MEN DTMF");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
      goto Miguel;
     }
     else if(datom==0x08)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SENSORES");    //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto Miguel;
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
  
    else if(clave[0]==datoe[2] & clave[1]==datoe[3])
    {
David:adres=0x02;
       /*escribirle al lcd
       bienvenidos al menu del dise�ador
       marque 1 para cambiar clave
       marque 2 para fecha y hora
       marque 3 para reportes
       marque 4 para motor paso a paso
       marque 5 para servomotor
       marque 6 para cad
       marque 7 para DTMF
       marque 8 para sensores
       marque 9 para salir*/
     
    lcd_putc('\f');                       //Limpiar pantalla comando
    lcd_gotoxy(1,1);                      //escribir en a primera fila primera posicion 
    lcd_putc("MENU USUAR.");              //lo que se escribe
    lcd_gotoxy(4,2);                      //escribir en a primera fila primera posicion 
    lcd_putc("DAVID");
    delay_ms(3000); 
       
    lcd_putc('\f');                       //Limpiar pantalla comando
    lcd_gotoxy(1,1);                      //escribir en a primera fila primera posicion 
    lcd_putc("MARQUE NUM PARA");          //lo que se escribe
    lcd_gotoxy(1,2);                      //escribir en a primera fila primera posicion 
    lcd_putc("1 CAMBIAR CLAVE");
    delay_ms(3000);  
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("2 FECHA Y HORA");          //lo que se escribe
    lcd_gotoxy(1,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("3 REPORTES");
    delay_ms(3000);
    
    lcd_putc('\f');                     //Limpiar pantalla comando
    lcd_gotoxy(1,1);                    //escribir en a primera fila primera posicion 
    lcd_putc("4 MOTOR PAS A PA");       //lo que se escribe
    lcd_gotoxy(1,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("5 SERVOMOTOR");
    delay_ms(3000);
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("6 CAD");                   //lo que se escribe
    lcd_gotoxy(1,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("7 DTMF");
    delay_ms(3000);
    
    lcd_putc('\f');                      //Limpiar pantalla comando
    lcd_gotoxy(1,1);                     //escribir en a primera fila primera posicion 
    lcd_putc("8 SENSORES");              //lo que se escribe
    lcd_gotoxy(3,2);                     //escribir en a primera fila primera posicion 
    lcd_putc("9 SALIR");
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
        escribir_memoria_ext();
        lcd_putc('\f');                   //Limpiar pantalla comando
        lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
        lcd_putc("CLAVE MODIFICADA");     //lo que se escribe
        lcd_gotoxy(3,2);                  //escribir en a primera fila primera posicion 
        lcd_putc("CON EXITO");   
        delay_ms(3000);
        goto David;
       }
       else{
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("ERROR DE CLAVE");       //lo que se escribe
       delay_ms(3000);
       goto David;
       }
       
     }
     else if(datom==0x02)
     {
      lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                 //escribir en a primera fila primera posicion 
       lcd_putc("MEN FECHA Y HORA");    //lo que se escribe
       delay_ms(3000);
       hora_fecha();
       delay_ms(3000);
       goto David;
     }
     else  if(datom==0x03)
     {
       ide=0x10;
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("REPORTE");              //lo que se escribe
       delay_ms(3000);
       
       lcd_putc('\f');                   //Limpiar pantalla comando
       lcd_gotoxy(1,1);                  //escribir en a primera fila primera posicion 
       lcd_putc("1 GENERAR.REPORT");     //lo que se escribe
       lcd_gotoxy(1,2);                  //escribir en a primera fila primera posicion 
       lcd_putc("2 MOSTRAR.REPORT");     //lo que se escribe
       delay_ms(3000);
       un_digito();
     if(datom==0x01){
       buscar_pos();                     //CONTADOR, ADDRESS, ESPACIO_VACIO=TRU
       if(espacio_vacio==true){
          
          hacer_reporte();
          for(i=0;i<8;i++){
               write_ext_eeprom(adres, time[i]);
               adres++;
               delay_ms(5);
          }
           lcd_putc('\f');                    //Limpiar pantalla comando
               lcd_gotoxy(1,1);               //escribir en a primera fila primera posicion 
               lcd_putc("REPORTE GUARDADO");  //lo que se escribe
               lcd_gotoxy(1,2);               //escribir en a primera fila primera posicion 
               lcd_putc("CON EXITO");         //lo que se escribe
               delay_ms(3000);
               goto David;
        }else{
               lcd_putc('\f');               //Limpiar pantalla comando
               lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
               lcd_putc("MEMORIA LLENA");    //lo que se escribe
               delay_ms(3000);
               goto David;
               
              }
     }else if (datom==0x02) {
                  mostrar_reporte();
                  goto David;
                }
                   else{ lcd_gotoxy(1,1);               //escribir en a primera fila primera posicion 
                       lcd_putc("NUMERO");              //lo que se escribe
                       lcd_gotoxy(1,2);                //escribir en a primera fila primera posicion 
                       lcd_putc("ERROR");              //lo que se escribe
                       delay_ms(3000);
                       goto David;}
               
     }
     else if(datom==0x04)
     {
       lcd_putc('\f');                        //Limpiar pantalla comando
       lcd_gotoxy(1,1);                       //escribir en a primera fila primera posicion 
       lcd_putc("MEN MOTOR PAS A.");          //lo que se escribe
       delay_ms(3000);
       
       lcd_putc('\f');                        //Limpiar pantalla comando
       lcd_gotoxy(1,1);                       //escribir en a primera fila primera posicion 
       lcd_putc("1 PARA DERECHA");            //lo que se escribe
       lcd_gotoxy(1,2);                       //escribir en a primera fila primera posicion 
       lcd_putc("2 PARA IZQUIERDA");          //lo que se escribe
       delay_ms(3000);
       
       un_digito();
       if(datom==1)
       
       { motorderecha();
       }else if(datom==2)
          {motorizquierda();
          }else {lcd_putc('\f');                        //Limpiar pantalla comando
                 lcd_gotoxy(1,1);                       //escribir en a primera fila primera posicion 
                 lcd_putc("ERROR DE NUMERO");            //lo que se escribe
                  delay_ms(3000);
                  goto David;
                  }
       
       
       goto David;
     }
     else if(datom==0x05)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SERVOMOTOR");  //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto David;
     }
     else if(datom==0x06)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MENU CAD");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto David;
     }
     else if(datom==0x07)
     {
       lcd_putc('\f');               //Limpiar pantalla comando
       lcd_gotoxy(1,1);              //escribir en a primera fila primera posicion 
       lcd_putc("MEN DTMF");         //lo que se escribe
       lcd_gotoxy(3,2);              //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto David;
     }
     else if(datom==0x08)
     {
       lcd_putc('\f');              //Limpiar pantalla comando
       lcd_gotoxy(1,1);             //escribir en a primera fila primera posicion 
       lcd_putc("MEN SENSORES");    //lo que se escribe
       lcd_gotoxy(3,2);             //escribir en a primera fila primera posicion 
       lcd_putc("EN CONSTRUCCION");
       delay_ms(3000);
       goto David;
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
