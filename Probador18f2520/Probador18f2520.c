#include <18f2520.h>
#device ADC = 10
#use i2c(Master,Fast=100000, sda=PIN_C4, scl=PIN_C3,force_sw)
#include <i2c_Flex_LCD.c>
#fuses intrc,nowdt
#use delay(clock=8000000)
//#include <lcd.c>
#use fast_io(all)
#priority rtcc, ext2  // Establecer prioridad si es necesario
float Lecturaad620;
int32 valor_recarga = 42098; 
//int32 valor_recarga = 0;
int direccionI2C = 0x4E;
int filas = 2;
int columnas = 16;
int pulsos_555 =  0;
int1 bandera_prueba_555;
int1 bandera_estado_555;
int32 value;
int opambuenos = 0;
int1 NPN_estado;
int1 PNP_estado;
int Compuerta = 0;
/* suponiendo que genere problemas lo de setear de esta manera los pines del puerto C, se puede probar colocar en 11 C3 y C4 así o en su defecto se puede setear de manera individual:
Definir como entradas los pines:
set_tris_c(0b00011000); // RC3 y RC4 en entrada, los demás en salida
set_tris_c((input_c() & 0b00011000) | 0b00000000);
output_c(input_c() & 0b00011000);// colocar puerto c 0000 0000

bit_set(TRISC, 3);  // RC3 como entrada
bit_set(TRISC, 4);  // RC4 como entrada

bit_clear(TRISC, 0); // RC0 como salida
bit_clear(TRISC, 1); // RC1 como salida
bit_clear(TRISC, 2); // RC2 como salida
bit_clear(TRISC, 5); // RC5 como salida
bit_clear(TRISC, 6); // RC6 como salida
bit_clear(TRISC, 7); // RC7 como salida

para output es:
bit_clear(PORTC, 0);  // RC0 en bajo
bit_clear(PORTC, 1);  // RC1 en bajo
bit_clear(PORTC, 2);  // RC2 en bajo
bit_clear(PORTC, 5);  // RC5 en bajo
bit_clear(PORTC, 6);  // RC6 en bajo
bit_clear(PORTC, 7);  // RC7 en bajo
*/


#INT_EXT2
void pulsos() {
   pulsos_555 = pulsos_555 + 1;
   CLEAR_INTERRUPT(INT_EXT2);
}

#INT_RTCC 
void tiempo_final_555() {
   DISABLE_INTERRUPTS(INT_EXT2);// Inhabilitar las interrupciones del timer0
   DISABLE_INTERRUPTS(INT_RTCC);
   bandera_prueba_555 = 0;
   if (pulsos_555 > 9) {
      bandera_estado_555 = 1;
   }
   
   else {
      bandera_estado_555 = 0;
   }
   set_timer0(valor_recarga);
   CLEAR_INTERRUPT(INT_RTCC);
}


void limpiar_puertos()
{
    output_b(0x00); // Limpiar puerto D (poner todos los pines en bajo)
    output_c(0x00); // Limpiar puerto C (poner todos los pines en bajo)
}
void configurar_puertos()
{
    set_tris_b(0x00); // Configurar puerto D como salida
    set_tris_c(0x3F); // Configurar puerto C como entrada C6 y C7 son salidas
    
}
void configurar_puertos2()
{
    set_tris_b(0x51); // Configurar puerto D como salida 0101 0001
    set_tris_c(0x40); // Configurar puerto C como salida 0100 0000
    output_b(0x00); // Colocar puerto D 0000 0000
    output_c(0x00);
}
void configurar_puertos3()
{
    set_tris_b(0x6a); // Configurar puerto D como salida 0110 1010
    set_tris_c(0x46); // Configurar puerto C como salida 0100 0110
    output_b(0x00); // Colocar puerto D 10101010
    output_c(0x00);// colocar puerto c 0110110
}
void configurar_puertos4()
{
    set_tris_b(0x6a); // Configurar puerto D como salida 0110 1010
    set_tris_c(0x46); // Configurar puerto C como salida 0100 0110
    output_b(0x95); // Colocar puerto D 1001 0101
    output_c(0x82);// colocar puerto c 1000 0010
}

void configurar_puertos5()
{
    set_tris_b(0x28); // Configurar puerto D como salida 0010 1000 
    set_tris_c(0x04); // Configurar puerto C como salida 0000 0100
    output_b(0x00); // Colocar puerto D 1001 0101
    output_c(0x00);// colocar puerto c 1000 0010
}
void configurar_puertos6()
{
    set_tris_b(0x28); // Configurar puerto D como salida 0010 1000 
    set_tris_c(0x04); // Configurar puerto C como salida 0000 0100
    output_b(0xD7); // Colocar puerto D 1101 0111
    output_c(0xFB);// colocar puerto c 1111 1011
}
void configurar_puertos7()
{
    set_tris_b(0x08); // Configurar puerto D como salida 0000 1000 
    set_tris_c(0x04); // Configurar puerto C como salida 0000 0100
    output_b(0x00); // Colocar puerto D 0000 0000
    output_c(input_c() & 0b00011000);// colocar puerto c 0000 0000
}
void configurar_puertos8()
{
    set_tris_b(0x08); // Configurar puerto D como salida 0000 1000 
    set_tris_c(0x04); // Configurar puerto C como salida 0000 0100
    output_b(0xF7); // Colocar puerto D 1111 0111
    output_c(0xFB);// colocar puerto c 1111 1011
}
void configurar_puertoslm224()
{
    set_tris_b(0x00);// puerto c entrada 0000 0000
    set_tris_c(0x02);// puerto c entrada 0011 0010
    output_b(0x00); // Colocar en alto el puerto D
    output_c(0x00);
}
void configurar_puertosPNP()
{
    set_tris_b(0x20); // Configurar puerto D como salida 0000 0000
    output_b(0x00); // Colocar puerto D 000 0001
}
void configurar_puertosNPN()
{
    set_tris_b(0x20); // Configurar puerto D como salida 0010 0000         CALTO  B4 ALTO E B5 BAJO ENTRADA
    output_b(0x10); // Colocar puerto D 000 0001
    //output_c(0x02);
    
}
void configurar_puertos358()
{
    set_tris_b(0x11); // Configurar puerto D como salida 0001 0001
    set_tris_c(0x00); // Configurar puerto C como salida 0000 0000
    output_b(0x04); // Colocar puerto D 0000 0100
    output_c(0x12); // Colocar puerto D 0001 0010
}
void configurar_puertos741()
{
    set_tris_b(0x20); // Configurar puerto D como salida 0010 0000
    set_tris_c(0x00); // Configurar puerto C como salida 0000 0000
    output_b(0x10); // Colocar puerto D 0001 0001
    output_c(0x02); // Colocar puerto D 0100 0010
}
void configurar_puertosOPTO()
{
    set_tris_b(0x04); // Configurar puerto D como salida 0000 0100
    set_tris_c(0x00); // Configurar puerto C como salida 0001 0000
    output_b(0x12); // Colocar puerto D 0001 0010
    output_c(0x00); // Colocar puerto D 0100 0000
}
void configurar_puertosNAND()
{
    set_tris_b(0x0c);// puerto c entrada 0000 1100
    set_tris_c(0x3f);// puerto c entrada 0011 1111
    output_b(0x00); // Colocar en alto el puerto D
    output_c(0x00);
}



void reiniciar()
{
    limpiar_puertos();
    configurar_puertos();
}
void SetearPuertosModo2()
{
    limpiar_puertos();
    configurar_puertos2();
    
}
void SetearPuertosModo3()
{
    limpiar_puertos();
    configurar_puertos3();
    
}
void SetearPuertosModo4()
{
    limpiar_puertos();
    configurar_puertos4();
    
}
void SetearPuertosModo5()
{
    limpiar_puertos();
    configurar_puertos5();
    
}
void SetearPuertosModo6()
{
    limpiar_puertos();
    configurar_puertos6();
    
}
void SetearPuertosModo7()
{
    limpiar_puertos();
    configurar_puertos7();
    
}
void SetearPuertosModo8()
{
    limpiar_puertos();
    configurar_puertos8();
    
}
void SetearPuertoslm224()
{
    limpiar_puertos();
    configurar_puertoslm224();
    
}
void SetearPuertosPNP()
{
    limpiar_puertos();
    configurar_puertosPNP();
    
}
void SetearPuertosNPN()
{
    limpiar_puertos();
    configurar_puertosNPN();
    
}
void SetearPuertos358()
{
    limpiar_puertos();
    configurar_puertos358();
    
}
void SetearPuertos741()
{
    limpiar_puertos();
    configurar_puertos741();
    
}
void SetearPuertosOPTO()
{
    limpiar_puertos();
    configurar_puertosOPTO();
    
}
void SetearPuertosNAND()
{
    limpiar_puertos();
    configurar_puertosNAND();
    
}


int Boton(){
         if(input(PIN_A3)) {
            return 1;
            } else if(input(PIN_A4)) {
            return 2;
            } else if(input(PIN_A5)) {
            return 3;
            } else if (input(PIN_C0)) {
            return 4;
            }
            return 0;
         }
         

void main(){
      setup_oscillator(OSC_4MHZ);
      set_tris_a(0xff);// puerto a entrada
      set_tris_b(00001100);// puerto d salida
      set_tris_c(0xff);// puerto c entrada
      
      SETUP_COUNTERS(RTCC_INTERNAL,RTCC_DIV_128);
      ENABLE_INTERRUPTS(GLOBAL);// Habilita las interrupciones globales
      ext_int_edge(H_TO_L); //Inicia el flanco de la interruccion de alto abajo
      lcd_init(direccionI2C,filas,columnas);
      setup_adc_ports(0x0e);
      setup_adc(adc_clock_internal);
      reiniciar();
      
      printf(lcd_putc,"\f");
      lcd_gotoxy(0,1);
      printf(lcd_putc,"UNEXPO");
      lcd_gotoxy(0,2);
      printf(lcd_putc,"BARQUISIMETO");
      delay_ms(200);
      printf(lcd_putc,"\f");
      lcd_gotoxy(0,1);
      printf(lcd_putc,"Daniela Cañas");
      lcd_gotoxy(0,2);
      printf(lcd_putc,"[Developer]");
      delay_ms(200);
      printf(lcd_putc,"\f");
      lcd_gotoxy(0,1);
      printf(lcd_putc,"Gabriel Gimenez");
      lcd_gotoxy(0,2);
      printf(lcd_putc,"[Developer]");
      delay_ms(200);
      printf(lcd_putc,"\f");
      lcd_gotoxy(0,1);
      printf(lcd_putc,"Carlos Guedez");
      lcd_gotoxy(0,2);
      printf(lcd_putc,"[Developer]");
      delay_ms(200);
      printf(lcd_putc,"\f");
      lcd_gotoxy(0,1);
      printf(lcd_putc,"Carlos Zambrano");
      lcd_gotoxy(0,2);
      printf(lcd_putc,"[Tutor]");
      delay_ms(200);
      

MENUPRINCIPAL:
      while(true){
          printf(lcd_putc,"\f");
          lcd_gotoxy(0,1);
          printf(lcd_putc,"[MENU]  2>TTL");
          lcd_gotoxy(0,2);
          printf(lcd_putc,"1>AD620 3>Next");
          delay_ms(200);
          while(1){
             if(boton()==2){
MENUTTL:
      while(true){
          output_b(0x00); 
          printf(lcd_putc,"\f");
          lcd_gotoxy(0,1);
          printf(lcd_putc,"MENU TTL");
          lcd_gotoxy(0,2);
          printf(lcd_putc,"1>7408 3>Next");
          delay_ms(200);
          while(1){
             if(Boton()==1){
               while(1){
                            reiniciar();
                            set_tris_b(0x0c);// puerto c entrada 0000 1100
                            set_tris_c(0x3f);// puerto c entrada 0011 0110
                            output_b(0xf3); // 1111 0011
                            output_c(0xc0);// 1100 0000
                            printf(lcd_putc,"\f"); //Limpiar la pantalla
                            lcd_gotoxy(0,1);
                            printf(lcd_putc,"74LS08 Testing..."); //  escribir eso
                            lcd_gotoxy(0,2);
                            printf(lcd_putc,"2>Volver");
                            delay_ms(1000);
                            if(input(PIN_C1)==1){
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"AND-1 Bueno!"); //  escribir eso
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"74LS08 Testing...");
                                 delay_ms(500);
                                 Compuerta = Compuerta + 1;
                            }
                            if(input(PIN_C2)==1){
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"AND-2 Bueno!"); //  escribir eso
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"74LS08 Testing...");
                                 delay_ms(500);
                                 Compuerta = Compuerta + 1;
                            }
                            if(input(PIN_B2)==1){
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"AND-3 Bueno!"); //  escribir eso
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"74LS08 Testing...");
                                 delay_ms(500);
                                 Compuerta = Compuerta + 1;
                            } 
                            if(input(PIN_B3)==1){
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"AND-4 Bueno!"); //  escribir eso
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"74LS08 Testing...");
                                 delay_ms(500);
                                 Compuerta = Compuerta + 1;
                            }   
                            if(Compuerta>=1){
                                 reiniciar();
                                 printf(lcd_putc,"\f"); //Limpiar la pantalla
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"Buenos %i", Compuerta);
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"Regresando..");
                                 delay_ms(1000);
                                 Compuerta = 0;
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"MENU TTL");
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"1>7408 3>Next");
                                break;
                            }       
                  if(Boton()==2){
                     reiniciar();
                     printf(lcd_putc,"\f");
                     lcd_gotoxy(0,1);
                     printf(lcd_putc,"MENU PRINCIPAL");
                     lcd_gotoxy(0,2);
                     printf(lcd_putc,"1>7408 3>Next");
                     delay_ms(1000);
                     break;
                  }
                }     
             }
              if(Boton()==3){
MENUDOS:
                  printf(lcd_putc,"\f");
                  lcd_gotoxy(0,1);
                  printf(lcd_putc,"1>7402 2>7486");
                  lcd_gotoxy(0,2);
                  printf(lcd_putc,"4>7432 3>Next");
                  delay_ms(200);
                  while(1){
                     if(Boton()==1){
                               SetearPuertosModo2();
                               printf(lcd_putc,"\f"); //Limpiar la pantalla
                               lcd_gotoxy(0,1);
                               printf(lcd_putc,"74LS02 Testing..."); //  escribir eso
                               lcd_gotoxy(0,2);
                               printf(lcd_putc,"2>Volver");
                               delay_ms(1000);
                               while(1){
                                  if(input(PIN_B0)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOR-1 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS02 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }
                                  if(input(PIN_C6)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOR-2 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS02 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }
                                  if(input(PIN_B6)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOR-3 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS02 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  } 
                                  if(input(PIN_B4)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOR-4 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS02 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }   
                                  if(Compuerta>=1){
                                       printf(lcd_putc,"\f"); //Limpiar la pantalla
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"Buenos %i", Compuerta);
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando..");
                                       reiniciar(); 
                                       delay_ms(1000);
                                       Compuerta = 0;
                                       goto MENUDOS;
                                  }
                                  if(Boton()==2){
                                       reiniciar();
                                       Compuerta = 0;
                                       delay_ms(1000);
                                       goto MENUDOS;
                                 }
                              }
                     }
                   
                     if(Boton()==2){
                            set_tris_b(0x0c);// puerto c entrada 0000 1100
                            set_tris_c(0x36);// puerto c entrada 0011 0110
                            output_b(0x51); // 0101 0001
                            output_c(0x40);// 0100 0000
                            output_low(PIN_C2); // C2 bajo
                            printf(lcd_putc,"\f"); //Limpiar la pantalla
                            lcd_gotoxy(0,1);
                            printf(lcd_putc,"74LS86 Testing..."); //  escribir eso
                            lcd_gotoxy(0,2);
                            printf(lcd_putc,"2>Volver");
                            delay_ms(1000);
                            while(1){
                               if(input(PIN_C1)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"XOR-1 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS86 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(input(PIN_C2)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"XOR-2 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS86 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(input(PIN_B3)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"XOR-3 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS86 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               } 
                               if(input(PIN_B2)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"XOR-4 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS86 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }   
                               if(Compuerta>=1){
                                    reiniciar();
                                    printf(lcd_putc,"\f"); //Limpiar la pantalla
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"Buenos %i", Compuerta);
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"Regresando..");
                                    delay_ms(1000);
                                    Compuerta = 0;
                                    goto MENUDOS;
                               }
                               if(Boton()==2){
                                    reiniciar();
                                    Compuerta = 0;
                                    goto MENUDOS;
                               }
                            }
                     }
                     if(Boton()==4){ 
                            set_tris_b(0x0c);// puerto c entrada 0000 1100
                            set_tris_c(0x36);// puerto c entrada 0011 0110
                            output_b(0x51); // 0101 0001
                            output_c(0x40);// 0100 0000
                            output_low(PIN_C2); // C2 bajo
                            printf(lcd_putc,"\f");
                            lcd_gotoxy(0,1);
                            printf(lcd_putc,"74LS32 Testing..."); //  escribir eso
                            lcd_gotoxy(0,2);
                            printf(lcd_putc,"2>Volver");
                            delay_ms(1000);
                            while(1){
                               if(input(PIN_C1)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"OR-1 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS32 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(input(PIN_C2)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"OR-2 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS32 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(input(PIN_B3)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"OR-3 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS32 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               } 
                               if(input(PIN_B2)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"OR-4 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS32 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(Compuerta>=1){
                                    reiniciar();
                                    printf(lcd_putc,"\f"); //Limpiar la pantalla
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"Buenos %i", Compuerta);
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"Regresando..");
                                    delay_ms(1000);
                                    Compuerta = 0;
                                    goto MENUDOS;
                               }
                               if(Boton()==2){
                                    reiniciar();
                                    Compuerta = 0;
                                    goto MENUDOS;
                               }
                            }
                     }
                     if(Boton()==3){
MENUTRES: 
                           printf(lcd_putc,"\f");
                           lcd_gotoxy(0,1);
                           printf(lcd_putc,"1>7400 2>7401");
                           lcd_gotoxy(0,2);
                           printf(lcd_putc,"4>7404 3>Next");
                           delay_ms(200);
                           while(1){
                           if(Boton()==1){
                               SetearPuertosNAND();
                               printf(lcd_putc,"\f"); //Limpiar la pantalla
                               lcd_gotoxy(0,1);
                               printf(lcd_putc,"74LS00 Testing..."); //  escribir eso
                               lcd_gotoxy(0,2);
                               printf(lcd_putc,"2>Volver");
                               delay_ms(1000);
                               while(1){
                                  if(input(PIN_C1)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NAND-1 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS00 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }
                                  if(input(PIN_C2)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NAND-2 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS00 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }
                                  if(input(PIN_B2)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NAND-3 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS00 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  } 
                                  if(input(PIN_B3)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NAND-4 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS00 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }   
                                  if(Compuerta>=1){
                                       reiniciar();
                                       printf(lcd_putc,"\f"); //Limpiar la pantalla
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"Buenos %i", Compuerta);
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando..");
                                       delay_ms(1000);
                                       Compuerta = 0;
                                       goto MENUTRES;
                                  }
                                  if(Boton()==2){
                                       reiniciar();
                                       printf(lcd_putc,"\f"); //Limpiar la pantalla
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando..");
                                       delay_ms(1000);
                                       Compuerta = 0;
                                       goto MENUTRES;
                                  }
                               } 
                           }
                           
                           if(Boton()==2){
                               SetearPuertosModo2();
                               printf(lcd_putc,"\f"); //Limpiar la pantalla
                               lcd_gotoxy(0,1);
                               printf(lcd_putc,"74LS01 Testing..."); //  escribir eso
                               lcd_gotoxy(0,2);
                               printf(lcd_putc,"2>Volver");
                               delay_ms(1000);
                               while(1) {
                               if(input(PIN_B0)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"NAND-1 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS01 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(input(PIN_C6)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"NAND-2 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS01 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }
                               if(input(PIN_B6)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"NAND-3 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS01 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               } 
                               if(input(PIN_B4)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"NAND-4 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"74LS01 Testing...");
                                    delay_ms(500);
                                    Compuerta = Compuerta + 1;
                               }   
                               if(Compuerta>=1){
                                    reiniciar();
                                    printf(lcd_putc,"\f"); //Limpiar la pantalla
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"Buenos %i", Compuerta);
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"Regresando..");
                                    delay_ms(1000);
                                    Compuerta = 0;
                                    goto MENUTRES;
                               }
                               if(Boton()==2){
                                    reiniciar();
                                    printf(lcd_putc,"\f"); //Limpiar la pantalla
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"Regresando..");
                                    Compuerta = 0;
                                    delay_ms(1000);
                                    goto MENUTRES;
                              }
                               }
                            }
                        
                           if(Boton()==4){
                               SetearPuertosModo3();
                               printf(lcd_putc,"\f"); //Limpiar la pantalla
                               lcd_gotoxy(0,1);
                               printf(lcd_putc,"74LS04 Testing..."); //  escribir eso
                               lcd_gotoxy(0,2);
                               printf(lcd_putc,"2>Volver");
                               delay_ms(500);
                               while(1){
                                  if(input(PIN_B1)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOT-1 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS04 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }
                                  if(input(PIN_C6)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOT-2 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS04 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }
                                  if(input(PIN_C2)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOT-3 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS04 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  } 
                                  if(input(PIN_B3)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOT-4 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS04 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }   
                                  if(input(PIN_B6)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOT-5 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS04 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }  
                                  if(input(PIN_B5)==1){
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"NOT-6 Bueno!"); //  escribir eso
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"74LS04 Testing...");
                                       delay_ms(500);
                                       Compuerta = Compuerta + 1;
                                  }  
                                  if(Compuerta>=1){
                                       reiniciar();
                                       printf(lcd_putc,"\f"); //Limpiar la pantalla
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"Buenos %i", Compuerta);
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando..");
                                       delay_ms(1000);
                                       Compuerta = 0;
                                       goto MENUTRES;
                                  }
                                  if(Boton()==2){
                                       printf(lcd_putc,"\f"); //Limpiar la pantalla
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando..");
                                       reiniciar();
                                       Compuerta = 0;
                                       delay_ms(1000);
                                       goto MENUTRES;
                                  }
                               }
                        }
                           if(Boton() == 3) {
MENUCUATRO: 
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"1>7407 2>7430");
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"4>7410 3>Next");
                                 delay_ms(200);
                                 while(1){
                                    if(Boton()==1){
                                        SetearPuertosModo4();
                                        printf(lcd_putc,"\f"); //Limpiar la pantalla
                                        lcd_gotoxy(0,1);
                                        printf(lcd_putc,"74LS07 Testing..."); //  escribir eso
                                        lcd_gotoxy(0,2);
                                        printf(lcd_putc,"2>Volver");
                                        delay_ms(1000);
                                        while(1){
                                           if(input(PIN_B1)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"BUFFER-1 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS07 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_C6)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"BUFFER-2 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS07 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_C2)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"BUFFER-3 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS07 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           } 
                                           if(input(PIN_B3)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"BUFFER-4 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS07 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }   
                                           if(input(PIN_B6)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"BUFFER-5 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS07 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }  
                                           if(input(PIN_B5)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"BUFFER-6 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS07 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }  
                                           if(Compuerta>=1){
                                                reiniciar();
                                                printf(lcd_putc,"\f"); //Limpiar la pantalla
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"Buenos %i", Compuerta);
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"Regresando..");
                                                delay_ms(1000);
                                                Compuerta = 0;
                                                goto MENUCUATRO;
                                           }
                                           if(Boton()==2){
                                                reiniciar();
                                                Compuerta = 0;
                                                delay_ms(1000);
                                                goto MENUCUATRO;
                                          }
                                       }
                                 }
                                    if(Boton()==4){
                                        SetearPuertosModo5();
                                        printf(lcd_putc,"\f"); //Limpiar la pantalla
                                        lcd_gotoxy(0,1);
                                        printf(lcd_putc,"74LS10 Testing..."); //  escribir eso
                                        lcd_gotoxy(0,2);
                                        printf(lcd_putc,"2>Volver");
                                        delay_ms(1000);
                                        while(1){
                                           if(input(PIN_B5)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"NAND-1 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS10 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_B3)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"NAND-2 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS10 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_C2)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"NAND-3 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS10 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(Compuerta>=1){
                                                reiniciar();
                                                printf(lcd_putc,"\f"); //Limpiar la pantalla
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"Buenos %i", Compuerta);
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"Regresando..");
                                                delay_ms(1000);
                                                Compuerta = 0;
                                                goto MENUCUATRO;
                                           }
                                           if(Boton()==2){
                                                reiniciar();
                                                Compuerta = 0;
                                                delay_ms(1000);
                                                goto MENUCUATRO;
                                          }
                                       }
                                    }
                                    
                                    if(Boton()==2){
                                       reiniciar();
                                        set_tris_b(0x08);// puerto c entrada 0000 1000
                                        set_tris_c(0x00);// puerto c entrada 0000 0000
                                        output_b(0x00); // Colocar en alto el puerto D
                                        output_c(0x00);
                                        printf(lcd_putc,"\f"); //Limpiar la pantalla
                                        lcd_gotoxy(0,1);
                                        printf(lcd_putc,"74LS30 Testing..."); //  escribir eso
                                        lcd_gotoxy(0,2);
                                        printf(lcd_putc,"2>Volver");
                                        delay_ms(1000);
                                        while(1){
                                        if(input(PIN_B3)==1){
                                             printf(lcd_putc,"\f");
                                             lcd_gotoxy(0,1);
                                             printf(lcd_putc,"74LS30 Bueno!"); //  escribir eso
                                             lcd_gotoxy(0,2);
                                             printf(lcd_putc,"Regresando...");
                                             delay_ms(1000);
                                             reiniciar();
                                             goto MENUCUATRO;
                                        }
                                        if(Boton()==2){
                                             printf(lcd_putc,"\f");
                                             lcd_gotoxy(0,2);
                                             printf(lcd_putc,"Regresando...");
                                             delay_ms(1000);
                                             reiniciar();
                                             goto MENUCUATRO;
                                        }
                                     } 
                                    }
                                    if(Boton() == 3) {
MENUCINCO: 
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"1>7420 2>7421");
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"4>7411 3>[Menu]");
                                    delay_ms(200);
                                    while(1){
                                       if(Boton()==4){
                                        SetearPuertosModo6();
                                        printf(lcd_putc,"\f"); //Limpiar la pantalla
                                        lcd_gotoxy(0,1);
                                        printf(lcd_putc,"74LS11 Testing..."); //  escribir eso
                                        lcd_gotoxy(0,2);
                                        printf(lcd_putc,"2>Volver");
                                        delay_ms(1000);
                                        while(1){
                                           if(input(PIN_B5)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"AND-1 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS11 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_B3)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"AND-2 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS11 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_C2)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"AND-3 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS11 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(Compuerta>=1){
                                                reiniciar();
                                                printf(lcd_putc,"\f"); //Limpiar la pantalla
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"Buenos %i", Compuerta);
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"Regresando..");
                                                delay_ms(1000);
                                                Compuerta = 0;
                                                goto MENUCINCO;
                                           }
                                           if(Boton()==2){
                                                reiniciar();
                                                Compuerta = 0;
                                                delay_ms(1000);
                                                goto MENUCINCO;
                                          }
                                       }
                                    }
                                       
                                       if(Boton()==1){
                                        SetearPuertosModo7();
                                        printf(lcd_putc,"\f"); //Limpiar la pantalla
                                        lcd_gotoxy(0,1);
                                        printf(lcd_putc,"74LS20 Testing..."); //  escribir eso
                                        lcd_gotoxy(0,2);
                                        printf(lcd_putc,"2>Volver");
                                        delay_ms(1000);
                                        while(1){
                                           if(input(PIN_B3)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"NAND-1 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS20 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(input(PIN_C2)==1){
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"NAND-2 Bueno!"); //  escribir eso
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"74LS20 Testing...");
                                                delay_ms(500);
                                                Compuerta = Compuerta + 1;
                                           }
                                           if(Compuerta>=1){
                                                reiniciar();
                                                printf(lcd_putc,"\f"); //Limpiar la pantalla
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"Buenos %i", Compuerta);
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"Regresando..");
                                                delay_ms(1000);
                                                Compuerta = 0;
                                                goto MENUCINCO;
                                           }
                                           if(Boton()==2){
                                                reiniciar();
                                                Compuerta = 0;
                                                delay_ms(1000);
                                                goto MENUCINCO;
                                          }
                                       }
                                    }
                                    
                                       if(Boton()==2){
                                           SetearPuertosModo8();
                                           printf(lcd_putc,"\f"); //Limpiar la pantalla
                                           lcd_gotoxy(0,1);
                                           printf(lcd_putc,"74LS21 Testing..."); //  escribir eso
                                           lcd_gotoxy(0,2);
                                           printf(lcd_putc,"2>Volver");
                                           delay_ms(1000);
                                           while(1){
                                              if(input(PIN_B3)==1){
                                                   printf(lcd_putc,"\f");
                                                   lcd_gotoxy(0,1);
                                                   printf(lcd_putc,"AND-1 Bueno!"); //  escribir eso
                                                   lcd_gotoxy(0,2);
                                                   printf(lcd_putc,"74LS21 Testing...");
                                                   delay_ms(500);
                                                   Compuerta = Compuerta + 1;
                                              }
                                              if(input(PIN_C2)==1){
                                                   printf(lcd_putc,"\f");
                                                   lcd_gotoxy(0,1);
                                                   printf(lcd_putc,"AND-2 Bueno!"); //  escribir eso
                                                   lcd_gotoxy(0,2);
                                                   printf(lcd_putc," 74LS21 Testing...");
                                                   delay_ms(500);
                                                   Compuerta = Compuerta + 1;
                                              }
                                              if(Compuerta>=1){
                                                   reiniciar();
                                                   printf(lcd_putc,"\f"); //Limpiar la pantalla
                                                   lcd_gotoxy(0,1);
                                                   printf(lcd_putc,"Buenos %i", Compuerta);
                                                   lcd_gotoxy(0,2);
                                                   printf(lcd_putc,"Regresando..");
                                                   delay_ms(1000);
                                                   Compuerta = 0;
                                                   goto MENUCINCO;
                                              }
                                              if(Boton()==2){
                                                   reiniciar();
                                                   Compuerta = 0;
                                                   delay_ms(1000);
                                                   goto MENUCINCO;
                                             }
                                          }
                                       }
                                          
                                       if(Boton() == 3) { 
                                          goto MENUPRINCIPAL;
                                       }
                                    }
                                    }
                                 }
                            }
                        }
                     }
                  }
               }  
            }
      }
  
             }
             if(Boton()==1){
               reiniciar();
               SetearPuertos741();
               set_adc_channel(0);
               delay_us(20);
               while(1){
                  float valor_adc = read_adc();
                  float lectura= valor_adc*0.004886;
                  float Lecturaad620=lectura;
                  printf(lcd_putc,"\f"); //Limpiar la pantalla
                  lcd_gotoxy(0,1);
                  printf(lcd_putc," AD620 Testing..."); //  escribir eso
                  lcd_gotoxy(0,2);
                  printf(lcd_putc," V:%0.2f. 2>Ret",Lecturaad620);
                  delay_ms(500);
                  if(Boton()==2){
                     reiniciar();
                     printf(lcd_putc,"\f"); //Limpiar la pantalla
                     lcd_gotoxy(0,2);
                     printf(lcd_putc,"Regresando..");
                     delay_ms(500);
                     goto MENUPRINCIPAL;
                  }
                }     
             }
             if(Boton()==3){
MENUBOTON3:
               printf(lcd_putc,"\f");
               lcd_gotoxy(0,1);
               printf(lcd_putc,"1>555 2>LM224");
               lcd_gotoxy(0,2);
               printf(lcd_putc,"4>741 3>Next");
               delay_ms(200);
               while(1){
                  if(Boton()==1){
                     reiniciar();
                     set_tris_b(0x04);  // RB2 como entrada (0b11111011), otros pines según necesidad
                     output_b(0x01); //GND y VCC
                     bandera_prueba_555 = 1;
                     CLEAR_INTERRUPT(INT_RTCC);
                     SET_TIMER0(valor_recarga);
                     value = get_timer0();
                     ENABLE_INTERRUPTS(INT_RTCC);
                     ENABLE_INTERRUPTS(INT_EXT2);  // Habilita las interrupciones externas
                     printf(lcd_putc,"\f"); //Limpiar la pantalla
                     lcd_gotoxy(0,1);
                     printf(lcd_putc,"555 Testing..."); //  escribir eso
                     lcd_gotoxy(0,2);
                     printf(lcd_putc,"2>Volver");
                     while(bandera_prueba_555 == 1) {
                        if (Boton() == 2) {                        
                           reiniciar();
                           printf(lcd_putc,"\f"); //Limpiar la pantalla
                           lcd_gotoxy(0,2);
                           printf(lcd_putc,"Regresando..");
                           delay_ms(1000);
                           pulsos_555 = 0;
                           goto MENUBOTON3;
                        }
                        // Mostrar tiempo transcurrido en la pantalla 
                        lcd_gotoxy(0, 1); 
                        printf(lcd_putc, "Tiempo: %lu", 65536 - get_timer0()); 
                        delay_ms(100);
                     }
                     
                     if (bandera_prueba_555 == 0) {
                        if (bandera_estado_555 == 0) {
                           printf(lcd_putc,"\f"); //Limpiar la pantalla
                           lcd_gotoxy(0,1);
                           printf(lcd_putc,"Malo %i", pulsos_555); //  escribir eso
                           lcd_gotoxy(0,2);
                           output_b(0x00); //GND y VCC
                           printf(lcd_putc,"2>Volver");
                           while(1) {
                              if(Boton() == 2) {
                                 bandera_estado_555 == 1;
                                 CLEAR_INTERRUPT(INT_RTCC);
                                 set_timer0(valor_recarga);
                                 reiniciar();
                                 printf(lcd_putc,"\f"); //Limpiar la pantalla
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"Regresando..");
                                 delay_ms(1000);
                                 pulsos_555 = 0;
                                 goto MENUBOTON3;
                              }
                           }
                        }
                        if (bandera_estado_555 == 1) {
                           printf(lcd_putc,"\f"); //Limpiar la pantalla
                           lcd_gotoxy(0,1);
                           printf(lcd_putc,"Bueno %i", pulsos_555); //  escribir eso
                           lcd_gotoxy(0,2);
                           output_b(0x00); //GND y VCC
                           printf(lcd_putc,"2>Volver");
                           while(1) {
                              if(Boton() == 2) {
                                 reiniciar();
                                 bandera_estado_555 == 1;
                                 pulsos_555 = 0;
                                 CLEAR_INTERRUPT(INT_RTCC);
                                 set_timer0(valor_recarga);
                                 printf(lcd_putc,"\f"); //Limpiar la pantalla
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"Regresando..");
                                 delay_ms(1000);
                                 goto MENUBOTON3;
                              }
                           }
                        }
                     }
                  }
                  if(Boton()==2){
                      SetearPuertoslm224();
                      printf(lcd_putc,"\f"); //Limpiar la pantalla
                      lcd_gotoxy(0,1);
                      printf(lcd_putc,"LM224 Testing..."); //  escribir eso
                      lcd_gotoxy(0,2);
                      printf(lcd_putc,"2>Volver");
                      delay_ms(500);
                      while(1){
                        if(input(PIN_C1)==1){
                          printf(lcd_putc,"\f");
                          lcd_gotoxy(0,1);
                          printf(lcd_putc,"AmpOp-1 Bueno!"); //  escribir eso
                          lcd_gotoxy(0,2);
                          printf(lcd_putc,"LM224 Testing...");
                          delay_ms(500);
                          opambuenos = opambuenos + 1;
                        } 
                        if(input(PIN_C2)==1){
                          printf(lcd_putc,"\f");
                          lcd_gotoxy(0,1);
                          printf(lcd_putc,"AmpOp-2 Bueno!"); //  escribir eso
                          lcd_gotoxy(0,2);
                          printf(lcd_putc,"LM224 Testing...");
                          delay_ms(500);
                          opambuenos = opambuenos + 1;
                        }
                        if(input(PIN_B3)==1){
                          printf(lcd_putc,"\f");
                          lcd_gotoxy(0,1);
                          printf(lcd_putc,"AmpOp-3 Bueno!"); //  escribir eso
                          lcd_gotoxy(0,2);
                          printf(lcd_putc,"LM224 Testing...");
                          delay_ms(500);
                          opambuenos = opambuenos + 1;
                        }
                        if(input(PIN_B4)==1){
                          printf(lcd_putc,"\f");
                          lcd_gotoxy(0,1);
                          printf(lcd_putc,"AmpOp-4 Bueno!"); //  escribir eso
                          lcd_gotoxy(0,2);
                          printf(lcd_putc,"LM224 Testing...");
                          delay_ms(500);
                          opambuenos = opambuenos + 1;
                        }
                        if(opambuenos>=1){
                          reiniciar();
                          output_c(0x00); // Colocar puerto D 0101 0010
                          printf(lcd_putc,"\f"); //Limpiar la pantalla
                          lcd_gotoxy(0,1);
                          printf(lcd_putc,"Buenos %i", opambuenos);
                          lcd_gotoxy(0,2);
                          printf(lcd_putc,"Regresando..");
                          delay_ms(1000);
                          opambuenos = 0;
                          goto MENUBOTON3;
                        }
                        if(Boton()==2){
                         reiniciar();
                         opambuenos = 0;
                         printf(lcd_putc,"\f"); //Limpiar la pantalla
                         lcd_gotoxy(0,2);
                         printf(lcd_putc,"Regresando...");
                         delay_ms(500);
                         goto MENUBOTON3;
                        }
                      } 
                  }
                  if(Boton()==4){
                        reiniciar();
                        SetearPuertos741();
                        printf(lcd_putc,"\f"); //Limpiar la pantalla
                        lcd_gotoxy(0,1);
                        printf(lcd_putc,"741 Testing..."); //  escribir eso
                        lcd_gotoxy(0,2);
                        printf(lcd_putc,"2>Volver");
                        delay_ms(1000);
                        while(1){
                           if(input(PIN_B5)==1){
                                    reiniciar();
                                    printf(lcd_putc,"\f"); //Limpiar la pantalla
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"AmpOp Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"Regresando...");
                                    delay_ms(500);
                                    goto MENUBOTON3;
                           }
                           if(Boton()==2){
                             reiniciar();
                             printf(lcd_putc,"\f"); //Limpiar la pantalla
                             lcd_gotoxy(0,2);
                             printf(lcd_putc,"Regresando...");
                             delay_ms(500);
                             goto MENUBOTON3;
                           }
                        }   
                  }
                  if(Boton()==3){
MENULM324:                  
                     printf(lcd_putc,"\f");
                     lcd_gotoxy(0,1);
                     printf(lcd_putc,"1>LM358 2>LM348");
                     lcd_gotoxy(0,2);
                     printf(lcd_putc,"4>LM324 3>Next");
                     delay_ms(200);
                     while(1){
                     if(Boton()==1){
                         SetearPuertos358();
                         printf(lcd_putc,"\f"); //Limpiar la pantalla
                         lcd_gotoxy(0,1);
                         printf(lcd_putc,"LM358 Testing..."); //  escribir eso
                         lcd_gotoxy(0,2);
                         printf(lcd_putc,"2>Volver");
                         delay_ms(1000);
                         while(1){
                            if(input(PIN_B0)==1){
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"AmpOp-1 Bueno!"); //  escribir eso
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"LM358 Testing...");
                                 delay_ms(500);
                                 opambuenos = opambuenos + 1;
                            }
                            if(input(PIN_B4)==1){
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"AmpOp-2 Bueno!"); //  escribir eso
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"LM358 Testing...");
                                 delay_ms(500);
                                 opambuenos = opambuenos + 1;
                            }
                            if(opambuenos>=1){
                                 reiniciar();
                                 printf(lcd_putc,"\f"); //Limpiar la pantalla
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"Buenos %i", opambuenos);
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"Regresando..");
                                 delay_ms(1000);
                                 opambuenos = 0;
                                 goto MENULM324;
                            }
                            if(Boton()==2){
                                reiniciar();
                                opambuenos = 0;
                                printf(lcd_putc,"\f"); //Limpiar la pantalla
                                lcd_gotoxy(0,2);
                                printf(lcd_putc,"Regresando...");
                                delay_ms(500);
                                goto MENULM324;
                            }
                        } 
                     }
                     
                     if(Boton()==2){
                      SetearPuertoslm224();
                      printf(lcd_putc,"\f"); //Limpiar la pantalla
                      lcd_gotoxy(0,1);
                      printf(lcd_putc,"LM348 Testing..."); //  escribir eso
                      lcd_gotoxy(0,2);
                      printf(lcd_putc,"2>Volver");
                      delay_ms(1000);
                      while(1){
                         if(input(PIN_C1)==1){
                              printf(lcd_putc,"\f");
                              lcd_gotoxy(0,1);
                              printf(lcd_putc,"AmpOp-1 Bueno!"); //  escribir eso
                              lcd_gotoxy(0,2);
                              printf(lcd_putc,"LM348 Testing...");
                              delay_ms(500);
                              opambuenos = opambuenos + 1;
                         }
                         if(input(PIN_C2)==1){
                              printf(lcd_putc,"\f");
                              lcd_gotoxy(0,1);
                              printf(lcd_putc,"AmpOp-2 Bueno!"); //  escribir eso
                              lcd_gotoxy(0,2);
                              printf(lcd_putc,"LM348 Testing...");
                              delay_ms(500);
                              opambuenos = opambuenos + 1;
                         }
                         if(input(PIN_B3)==1){
                              printf(lcd_putc,"\f");
                              lcd_gotoxy(0,1);
                              printf(lcd_putc,"AmpOp-3 Bueno!"); //  escribir eso
                              lcd_gotoxy(0,2);
                              printf(lcd_putc,"LM348 Testing...");
                              delay_ms(500);
                              opambuenos = opambuenos + 1;
                         } 
                         if(input(PIN_B4)==1){
                              printf(lcd_putc,"\f");
                              lcd_gotoxy(0,1);
                              printf(lcd_putc,"AmpOp-4 Bueno!"); //  escribir eso
                              lcd_gotoxy(0,2);
                              printf(lcd_putc," LM348 Testing...");
                              delay_ms(500);
                              opambuenos = opambuenos + 1;
                         }   
                         if(opambuenos>=1){
                              reiniciar();
                              printf(lcd_putc,"\f"); //Limpiar la pantalla
                              lcd_gotoxy(0,1);
                              printf(lcd_putc,"Buenos %i", opambuenos);
                              lcd_gotoxy(0,2);
                              printf(lcd_putc,"Regresando..");
                              delay_ms(1000);
                              opambuenos = 0;
                              goto MENULM324;
                         }
                         if(Boton()==2){
                                reiniciar();
                                opambuenos = 0;
                                printf(lcd_putc,"\f"); //Limpiar la pantalla
                                lcd_gotoxy(0,2);
                                printf(lcd_putc,"Regresando...");
                                delay_ms(500);
                                goto MENULM324;
                            }
                     } 
               }
                     if(Boton()==4){
                            SetearPuertoslm224();
                            printf(lcd_putc,"\f"); //Limpiar la pantalla
                            lcd_gotoxy(0,1);
                            printf(lcd_putc,"LM324 Testing..."); //  escribir eso
                            lcd_gotoxy(0,2);
                            printf(lcd_putc,"2>Volver");
                            delay_ms(1000);
                            while(1){
                               if(input(PIN_C1)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"AmpOp-1 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"LM324 Testing...");
                                    delay_ms(500);
                                    opambuenos = opambuenos + 1;
                                    
                               }
                               if(input(PIN_C2)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"AmpOp-2 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"LM324 Testing...");
                                    delay_ms(500);
                                    opambuenos = opambuenos + 1;
                               }
                               if(input(PIN_B3)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"AmpOp-3 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"LM324 Testing...");
                                    delay_ms(500);
                                    opambuenos = opambuenos + 1;
                               } 
                               if(input(PIN_B4)==1){
                                    printf(lcd_putc,"\f");
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"AmpOp-4 Bueno!"); //  escribir eso
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"LM324 Testing...");
                                    delay_ms(500);
                                    opambuenos = opambuenos + 1;
                               }   
                               if(opambuenos>=1){
                                    reiniciar();
                                    printf(lcd_putc,"\f"); //Limpiar la pantalla
                                    lcd_gotoxy(0,1);
                                    printf(lcd_putc,"Buenos %i", opambuenos);
                                    lcd_gotoxy(0,2);
                                    printf(lcd_putc,"Regresando...");
                                    delay_ms(1000);
                                    opambuenos = 0;
                                    goto MENULM324;
                               }
                               if(Boton()==2){
                                   reiniciar();
                                   opambuenos = 0;
                                   printf(lcd_putc,"\f"); //Limpiar la pantalla
                                   lcd_gotoxy(0,2);
                                   printf(lcd_putc,"Regresando...");
                                   delay_ms(500);
                                   goto MENULM324;
                               }
                           }
                     }
                     if(Boton()==3){
MENUTRANSISTORES:                     
                        printf(lcd_putc,"\f");
                        lcd_gotoxy(0,1);
                        printf(lcd_putc,"1>NPN 2>PNP");
                        lcd_gotoxy(0,2);
                        printf(lcd_putc,"3>Next");
                        delay_ms(100);
                        while(1){
                           while(1){
                              if(Boton()==1) {
                                 SetearPuertosNPN();
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"NPN Testing...");
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"2>Volver");
                                 delay_ms(1000);
                                 while(1) {
                                    if (input(PIN_B5) == 1) {
                                          reiniciar();
                                          printf(lcd_putc,"\f");
                                          lcd_gotoxy(0,1);
                                          printf(lcd_putc,"NPN BUENO");
                                          lcd_gotoxy(0,2);
                                          printf(lcd_putc,"Regresando...");
                                          delay_ms(1000);
                                          goto MENUTRANSISTORES;
                                     }
                                    if(Boton() == 2){
                                       reiniciar();
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando...");
                                       delay_ms(1000);
                                       goto MENUTRANSISTORES;
                                     }
                                  }
                              }
                              
                              if(Boton() == 2) {
                                 SetearPuertosPNP();
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"PNP Testing...");
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"2>Volver");
                                 delay_ms(1000);
                                 while(1) {
                                    if (input(PIN_B5) == 1) {
                                       reiniciar();
                                       printf(lcd_putc,"\f");
                                       lcd_gotoxy(0,1);
                                       printf(lcd_putc,"PNP BUENO");
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando...");
                                       delay_ms(1000);
                                       goto MENUTRANSISTORES;
                                    }
                                    if(Boton() == 2){
                                       reiniciar();
                                       lcd_gotoxy(0,2);
                                       printf(lcd_putc,"Regresando...");
                                       delay_ms(1000);
                                       goto MENUTRANSISTORES;
                                    }
                                 }
                              }
                                 
                              if(Boton() == 3) {
MENUMOSFET:                              
                                 printf(lcd_putc,"\f");
                                 lcd_gotoxy(0,1);
                                 printf(lcd_putc,"1>MOSF N");
                                 lcd_gotoxy(0,2);
                                 printf(lcd_putc,"2>MOSF P 3>Next");
                                 delay_ms(100);
                                 while(1) {
                                       if(Boton()==1) {
                                         
                                          SetearPuertosNPN();
                                          printf(lcd_putc,"\f");
                                          lcd_gotoxy(0,1);
                                          printf(lcd_putc,"MOSFET Testing...");
                                          lcd_gotoxy(0,2);
                                          printf(lcd_putc,"2>Volver");
                                          delay_ms(1000);
                                           while(1){
                                          if (input(PIN_B5) == 1) {
                                             reiniciar();
                                             printf(lcd_putc,"\f");
                                             lcd_gotoxy(0,1);
                                             printf(lcd_putc,"MOSFET BUENO");
                                             lcd_gotoxy(0,2);
                                             printf(lcd_putc,"Regresando...");
                                             delay_ms(1000);
                                             goto MENUMOSFET;
                                          }
                                          if(Boton() == 2){
                                             reiniciar();
                                             lcd_gotoxy(0,2);
                                             printf(lcd_putc,"Regresando...");
                                             delay_ms(1000);
                                             goto MENUMOSFET;
                                          }
                                          }
                                       }
                              
                                       if(Boton() == 2) {
                                          SetearPuertosPNP();
                                          printf(lcd_putc,"\f");
                                          lcd_gotoxy(0,1);
                                          printf(lcd_putc,"MOSFET Testing...");
                                          lcd_gotoxy(0,2);
                                          printf(lcd_putc,"2>Volver");
                                          delay_ms(1000);
                                          while(1) {
                                             if (input(PIN_B5) == 1) {
                                                reiniciar();
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"MOSFET BUENO");
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"Regresando...");
                                                delay_ms(1000);
                                                goto MENUMOSFET;
                                             }
                                             if(Boton() == 2){
                                                printf(lcd_putc,"\f");
                                                reiniciar();
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"Regresando...");
                                                delay_ms(1000);
                                                goto MENUMOSFET;
                                             }
                                          }
                                       }
                                       
                                       if(Boton() == 3) {
MENUOPTO:   
                                          printf(lcd_putc,"\f");
                                          lcd_gotoxy(0,1);
                                          printf(lcd_putc,"1>MOC3020/3010 ");
                                          lcd_gotoxy(0,2);                                        
                                          printf(lcd_putc,"3>Next");
                                          delay_ms(100);
                                          while(1){
                                             if(Boton() == 1) {
                                                SetearPuertosOPTO();
                                                printf(lcd_putc,"\f");
                                                lcd_gotoxy(0,1);
                                                printf(lcd_putc,"MOC Testing...");
                                                lcd_gotoxy(0,2);
                                                printf(lcd_putc,"2>Volver");
                                                delay_ms(1000);
                                                while(1) {
                                                   if (input(PIN_B2) == 1) {
                                                      reiniciar();
                                                      printf(lcd_putc,"\f");
                                                      lcd_gotoxy(0,1);
                                                      printf(lcd_putc,"MOC BUENO");
                                                      lcd_gotoxy(0,2);
                                                      printf(lcd_putc,"Regresando...");
                                                      delay_ms(1000);
                                                      goto MENUOPTO;
                                                   }
                                                   if(Boton() == 2){
                                                      printf(lcd_putc,"\f");
                                                      reiniciar();
                                                      lcd_gotoxy(0,2);
                                                      printf(lcd_putc,"Regresando...");
                                                      delay_ms(1000);
                                                      goto MENUOPTO;
                                                   }
                                                }
                                             }
                                             if(Boton() == 3) {
                                                   reiniciar();
                                                   goto MENUPRINCIPAL;
                                             }
                                          }
                                       }
                                 }
                              }
                           }
                        }
                     }
                     }
                  }
               }
            }
         }
      } 
   }
