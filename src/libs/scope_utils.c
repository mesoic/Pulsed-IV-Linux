/*** write GPIB order ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpib/ib.h"
#include "gpib_io.h"
#include "scope_utils.h"

int scopeDefaults(int ud){
  _write(ud, "CHAN1:COUP DC");
  _write(ud, "CHAN1:DISP 1");
  _write(ud, "CHAN2:COUP DC");
  _write(ud, "CHAN2:DISP 1");
  _write(ud, "CHAN3:COUP DC");
  _write(ud, "CHAN3:DISP 1");
  _write(ud, "CHAN4:COUP DC");
  _write(ud, "CHAN4:DISP 1");

  _write(ud, "CHAN1:UNIT VOLT");
  _write(ud, "CHAN2:UNIT VOLT");
  _write(ud, "CHAN3:UNIT VOLT");
  _write(ud, "CHAN4:UNIT VOLT");

  _write(ud, "CHAN1:SCAL 2");
  _write(ud, "CHAN2:SCAL 2");
  _write(ud, "CHAN3:SCAL 0.2");
  _write(ud, "CHAN4:SCAL 0.02");

  _write(ud, "CHAN1:IMP FIFT");
  _write(ud, "CHAN2:IMP ONEM");
  _write(ud, "CHAN3:IMP FIFT");
  _write(ud, "CHAN4:IMP FIFT");

  _write(ud, "TIM:SCAL 10us");
  _write(ud, "TRIG:SLOP POS");
  _write(ud, "TRIG:SOUR IMM");
  
  _write(ud, "TRIG:SOUR EXT");
  _write(ud, "TRIG:SLOP POS");
  _write(ud, "ACQ:MODE RTIM");
  _write(ud, "RUN");
  return 0;
}

int scopeMeasureMode(int ud, char* var1){
  _write(ud, "TRIG:SOUR EXT");
  _write(ud, "TRIG:SLOP POS");
  _write(ud, "ACQ:TYPE AVER");
 
  // Averaging
  char cmd[32]; 
  strcpy(cmd, "ACQ:COUN ");
  strcat(cmd, var1);
  _write(ud, cmd);
  return 0;
}

int scopeSetCH(int ud, char* ch, char* state){
  char cmd[32]; 
  strcpy(cmd, "CHAN"); 
  strcat(cmd, ch); 
  strcat(cmd, ":DISP ");
  strcat(cmd, state);
  _write(ud, cmd);
  return 0;
}

char* scopeDATA(int ud, int cx, int BUFFERSIZE){

  char cmd[32]; 
  _write(ud, "WAV:POIN 1000");
  sprintf(cmd, "WAV:SOUR CHAN%d",cx);
  _write(ud, cmd);
  _write(ud, "WAV:FORM ASCII");
  return _read(ud, "WAV:DATA?", BUFFERSIZE);
}

int scopeRUN(int ud){_write(ud,"RUN");} 
int scopeSTOP(int ud){_write(ud,"STOP");} 









