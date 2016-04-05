#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpib/ib.h"
#include "gpib_io.h"
#include "pulse_utils.h"

int pulseON(int ud){_write(ud, "D0");}
int pulseOFF(int ud){_write(ud, "D1");}
int pulsePOS(int ud){_write(ud, "C0");} 
int pulseNEG(int ud){_write(ud, "C1");} 

int pulseWidth(int ud, char* var){
  char cmd[32]; 
  strcpy(cmd, "WID ");
  strcat(cmd, var);
  strcat(cmd, "us"); 
  _write(ud, cmd); 
  return 0; 
}

int pulsePeriod(int ud, char* var){ 
  char cmd[32]; 
  strcpy(cmd, "PER ");
  strcat(cmd, var);
  strcat(cmd, "ms"); 
  _write(ud, cmd); 
  return 0; 
}

int pulseLevel(int ud, char* var1, char* var2){ 

  char cmda[32]; 
  // Low Level
  strcpy(cmda, "LOL ");
  strcat(cmda, var1);
  strcat(cmda, "V"); 
  _write(ud, cmda);

  // High Level
  char cmdb[32]; 
  strcpy(cmdb, "HIL ");
  strcat(cmdb, var2);
  strcat(cmdb, "V"); 
  _write(ud, cmdb);
  return 0; 
}


