/*** write GPIB order ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpib/ib.h"
#include "gpib_io.h"

/*** GPIB INPUT OUTPUT COMMANDS ***/
int _write(int ud, char* command)
{
  // WRITE A COMMAND
  // (device, data, len(data))

  ibwrt(ud, command, strlen(command));
  usleep(100000);
  _status(command);
  return 0;
}

char* _read(int ud, char* command, int max_num_bytes)        
{     
  // Write the command
  char *buffer;
  int buffer_size = max_num_bytes + 1;
  buffer = malloc(2*buffer_size*sizeof(char));
  int is_string;
  int i;
  if(buffer == NULL){
    fprintf(stderr, "%s: failed to allocate buffer.\n", __FUNCTION__);
    return ((char*)0);
  }
  memset(buffer, 0, buffer_size);
  
  ibwrt(ud, command, strlen(command));
  _status(command);
  printf("trying to read %i bytes from device...\n", max_num_bytes);
  ibrd(ud, buffer, buffer_size - 1);
  _write(ud,"*WAI");
  usleep(500000);
  is_string = 1;
  for(i = 0; i < ThreadIbcntl(); ++i){
    if(isascii(buffer[i]) == 0){
      is_string = 0;
      break;
    }
  }
  if(is_string){
    printf("received string: '%s'\n", buffer);
  }
  // Set up a buffer to recieve data and initialize a 
  // Pointer to the first element of the string
  return buffer;
}

int _status(char* command)
{
  // Prints GPIB status info (for debug)
  printf("--------------------------\n");
  printf("cmd = %s\n",command);
  printf("ibsta = 0x%x\n", ThreadIbsta());
  printf("iberr = %i\n", ThreadIberr());
  printf("ibcnt = %i\n", ThreadIbcnt());
  return 0;
}

int _initialize(int gpib)
{ 
  // OPEN A DEVICE 
  // ibdev(board index, primary address, secondary address, timeout, err, err)  
  // T1s = 1 second timeout
  int ud; 
  ud = ibdev(0, gpib, 0, T1s, 1, 0); 
  _write(ud,"*CLS"); 
  _write(ud,"*RST");
  return ud;   
}

int _close(int ud)
{
  // CLOSE THE DEVICE  ibonl(device, number)
  // number == 0 ----> deallocates everything associated with device
  // number != 0 ----> resets to 'default values' 
  ibonl(ud, 1);
  return 0;
}


/* int main(void) */
/* { */
/*   int gpib = 1;  */
/*   int HP4156B = initialize(gpib); */

/*   // Setup device */
/*   _write(HP4156B,"*CLS"); */
/*   _write(HP4156B,"*RST"); */
/*   _write(HP4156B,":FORM:DATA ASCii"); */
  
/*   // Sweep mode variables */
/*   _write(HP4156B,":PAGE:CHAN:MODE SWE"); */
/*   _write(HP4156B,":PAGE:MEAS:MSET:ITIM SHORT"); */

/*   // Set up the SMU channels */
/*   const char *SMU1[] = {"SMU1","'VS'","'IS'","COMM","CONS"}; */
/*   setSMU(HP4156B,SMU1); */
/*   const char *SMU2[] = {"SMU2","'VD'","'ID'","V","VAR1"}; */
/*   setSMU(HP4156B,SMU2); */
/*   const char *SMU3[] = {"SMU3","'VG'","'IG'","COMM","CONS"}; */
/*   setSMU(HP4156B,SMU3); */
/*   disableSMU(HP4156B,"SMU4"); */
/*   disableSMU(HP4156B,"SMU5"); */

/*   // Set up VAR1  */
/*   const char *VAR1[] = {"LIN","-0.1","1.5","0.1","100mA"}; */
/*   setVAR(HP4156B,1,VAR1); */

/*   // Perform Measurement */
/*   _write(HP4156B,":PAGE:SCON:SING"); */
/*   _write(HP4156B,"*WAI"); */
/*   sleep(2); */

/*   // Read Measurement Data */
/*   int buffer_size = 8192; */

/*   char* vd_data; */
/*   vd_data = malloc(buffer_size); */

/*   char* id_data; */
/*   vd_data = malloc(buffer_size); */

/*   vd_data = _read(HP4156B, ":DATA? 'VD'", buffer_size); */
/*   id_data = _read(HP4156B, ":DATA? 'ID'", buffer_size); */

/*   // _WRITE TO FILE */
/*   // Format output data (first remove newlines at end) */
/*   FILE *file;  */
/*   file = fopen("test.dat","w+");  */
 
/*   vd_data[strlen(vd_data)-1] = 0;  */
/*   id_data[strlen(id_data)-1] = 0; */

/*   // Populate buf_1, buf_2, and buf_3 */
/*   char *save_ptr1, *save_ptr2; */
/*   char *vd_tok, *id_tok; */

/*   // get the initial tokens */
/*   vd_tok = strtok_r(vd_data, ",", &save_ptr1); */
/*   id_tok = strtok_r(id_data, ",", &save_ptr2); */

/*   fprintf(file, "VD              ID\n"); */
/*   while(vd_tok && id_tok) { */
/*     // get next tokens */
/*     fprintf(file, "%s\t\%s\n", vd_tok, id_tok); */
/*     vd_tok = strtok_r(NULL, ",", &save_ptr1); */
/*     id_tok = strtok_r(NULL, ",", &save_ptr2); */
/*   } */

/*   fclose(file); /\*done!*\/  */

/*   // Format and dump data to file */
/*   close(HP4156B); */
/*   return 0; */
/* } */

