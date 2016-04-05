#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////
//         HELPER FUNCTIONS           //
////////////////////////////////////////
// Function which takes a string and puts single 
// quotes around it (for GPIB control)
char* stringify(char* str){ 
  char* tmp;
  tmp = (char*)malloc(8);
  strcpy(tmp, "'");
  const char* matches = "'";
  if ( strchr(matches, str[0]) == NULL ) {
    strcat(tmp,str);
    strcat(tmp,"'");
  }
  return tmp;
}

char* remove_char(char* source, char c){
  char* dest; 
  dest =  (char*)malloc(256);
  char* h=dest;
  while (*source){
    if(*source != c){
      *dest = *source; 
      dest++;
    }
    source++;
  }
  *dest = '\0';
  return h;
}


/* int main(void) */
/* { */
/*   char *str;  */
/*   str = "this is my string"; */
/*   printf("%s\n",stringify(str)); */
/* } */
