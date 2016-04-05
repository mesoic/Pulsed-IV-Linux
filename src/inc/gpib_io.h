extern int gpibADDR; 

int _write(int ud, char* command);
int _initialize(int gpib);
int _status(char* command);
int _close(int ud);

char* _read(int ud, char* command, int max_num_bytes);






