/*
  rfreceive.cpp
  by Sarrailh Remi
  Description : This code receives RF codes and trigger actions
*/

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

  RCSwitch mySwitch;
  int pin; //Pin of the RF receiver
  int codereceived; // Code received
  char buffer[200]; // Buffer for the command
  time_t T_recep, T_recep_ant;
  char actlz_xively[BUFSIZ];  // almacena la llamada a xively
  int valor_recibido;
  int sensor;
  char sensor_c[7];
  float valor_sensor;
  char valor_sensor_c[8];
  char pres15[7] = "pres15";
  char temp10[7] = "temp10";
  char temp20[7] = "temp20";
  char temp08[7] = "temp08";
  char temp13[7] = "temp13";
  char error[7] = "error";

  int main(int argc, char *argv[]) {
	  
	if(argc == 2) { //Verify if there is an argument
      pin = atoi(argv[1]); //Convert first argument to INT
      printf("PIN SELECTED :%i\n",pin);
    }
  else {
    printf("No PIN Selected\n");
    printf("Usage: rfreceive PIN\n");
    printf("Example: rfreceive 0\n");
    exit(1);
  }

    if(wiringPiSetup() == -1) { //Initialize WiringPI
      printf("Wiring PI is not installed\n"); //If WiringPI is not installed give indications
      printf("You can install it with theses command:\n");
      printf("apt-get install git-core\n");
      printf("git clone git://git.drogon.net/wiringPi\n");
      printf("cd wiringPi\n"); 
      printf("git pull origin\n");
      printf("./build\n");
      exit(1);
    }

   mySwitch = RCSwitch(); //Settings RCSwitch (with first argument as pin)
   mySwitch.enableReceive(pin);

          printf("xxx entro en el loop\n") ;

   while(1) { //Infinite loop
    if (mySwitch.available()) { //If Data is detected.
         codereceived = mySwitch.getReceivedValue(); //Get data in decimal
          printf("Recibido %i en %u\n", codereceived,time(NULL) );
  
    T_recep = time (NULL); // escribo solo si es una lectura diferente, no estan en el mismo seg 
    if (T_recep != T_recep_ant) {
	  valor_recibido =  mySwitch.getReceivedValue();
          printf("Recibido otra vez %i en %u\n", valor_recibido,time(NULL) );
          T_recep_ant = T_recep;
// y ahora actualizamos en la nube:
          sensor = valor_recibido / 1000000;
          switch (sensor) {
             case 6:
                strcpy(sensor_c, " hum06");
                break;
             case 7:
                strcpy(sensor_c, "temp07");
                break;
             case 10:
                strcpy(sensor_c, "temp10");
                break;
             case 11: 
                strcpy(sensor_c, " hum11");
                break;
             case 12:
                strcpy(sensor_c, " bat12");
                break;
             case 13:
                strcpy(sensor_c, "temp13");
                break;
             case 14:
                strcpy(sensor_c, " hum14");
                break;
             case 15:
                strcpy(sensor_c, "pres15");
                break;
             case 16:
                strcpy(sensor_c, "P_Te16");
                break;
             case 8:
                strcpy(sensor_c, "temp08");
                break;
             case 9:
                strcpy(sensor_c, " luz09");
                break;
             default:
                strcpy(sensor_c, "otro00");
                           }
          if (strcmp(sensor_c,error) != 0)  {
              valor_sensor = ( (float)valor_recibido - sensor * 1000000) / 100;

              if (strcmp(sensor_c, pres15) == 0) {
                  valor_sensor = valor_sensor + 76;  // valor de calibración para el sensor de presion
                                      }
              if (strcmp(sensor_c, temp10) == 0) {
                  valor_sensor = valor_sensor + 0.3;  // valor de calibración para el sensor de presion
                                      }
              if (strcmp(sensor_c, temp20) == 0) {
                  valor_sensor = valor_sensor - 0.6;  // valor de calibración para el sensor de presion
                                      }
              if (strcmp(sensor_c, temp13) == 0 and valor_sensor > 100) {
                  valor_sensor = valor_sensor - 200;  // comprobamos si es una temperatura negativa 
                                      }
              if (strcmp(sensor_c, temp13) == 0) {
                  valor_sensor = valor_sensor - 0.3;  // valor de calibracion 
                                      }
              if (strcmp(sensor_c, temp08) == 0) {
                  valor_sensor = valor_sensor - 0.3;  // valor de calibración 
                                      }
              sprintf(valor_sensor_c, "%4.2f", valor_sensor);
              strcpy (actlz_xively, "/home/pi/libxively/src/xively/actualiz_xively update ");
              strcat (actlz_xively, sensor_c);
              strcat (actlz_xively, " ");
              strcat (actlz_xively, valor_sensor_c);
  
 //             printf ("Executing xyvely %s\n", actlz_xively);
 //             system (actlz_xively);


              strcpy (actlz_xively, "/usr/bin/python /home/pi/actualiz_GS.py ");
              strcat (actlz_xively, sensor_c);
              strcat (actlz_xively, " ");
              strcat (actlz_xively, valor_sensor_c);

              printf ("Executing GS  %s\n", actlz_xively);
              system (actlz_xively);



                           }
          else {
              strcpy (actlz_xively, "/usr/bin/python /home/pi/actualiz_GS.py ");
              strcat (actlz_xively, sensor_c);
              strcat (actlz_xively, " ");
              sprintf(valor_sensor_c, "%d", valor_recibido);
              strcat (actlz_xively, valor_sensor_c);
              printf ("Executing GS por excepcion  %s\n", actlz_xively);
              system (actlz_xively);
               }
              


 }
          //Want to execute something when a code is received ?
          //When 12345 is received this will execute program_to_execute for exemple)
          /*
          if (codereceived == 12345)
          {
            system("program_to_execute");
          }
          */
    }
  mySwitch.resetAvailable();
  }
}
