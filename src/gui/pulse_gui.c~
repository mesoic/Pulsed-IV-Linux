#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include "gpib_io.h"

#include "source_utils.h"
#include "pulse_utils.h"
#include "scope_utils.h"

#include "linkedlist.h"
#include "stringutils.h"

// GPIB BUFFER SIZE 32kb 
#define BUFFERSIZE 16000

// GUI Window SIZE
#define WIDTH 900
#define HEIGHT 500

// BASIC Widget SIZE
#define BWIDTH 120
#define BHEIGHT 45

// WIDGET PLACEMENT 
#define XSPACE 150
#define YSPACE 70


#define X1 15
#define X2 X1+XSPACE
#define X3 X2+XSPACE
#define X4 X3+XSPACE
#define X5 X4+XSPACE
#define X6 X5+XSPACE

#define Y1 15
#define Y2 Y1+YSPACE+25
#define Y3 Y2+YSPACE
#define Y4 Y3+YSPACE+25
#define Y5 Y4+YSPACE
#define Y6 Y5+YSPACE+25
#define Y7 Y6+YSPACE

int gpibADDR;
int gpibHANDLE;

typedef struct{ 
  int a_keithley; 
  int a_source; 
  int a_scope;  
  int a_pulse; 
}gpib; 


// Linked List for bias grid
typedef struct point{
  struct point *next;
  float vg;
  float vd;
  char* c1; 
  char* c2; 
  char* c3;
  char* c4; 
} point_t;

void print_grid(point_t * head) {
  point_t * current = head;
  while (current != NULL) {
    printf("%f %f %p \n", current->vg, current->vd, current->c1, current->c1);
    current = current->next;
  }
}

// The application is held in a gigantic 
// struct which is called "state"
typedef struct{
  ////////////////////////////
  // THE APPLICATION ITSELF //
  ////////////////////////////
  GtkApplication *app;

  // the main window
  GtkWidget *window;
  GtkWidget *fixed;
  GdkWindow *gd;

  // MODE LABEL (SWEEP/SAMPLING) 
  GtkWidget *MODELABEL;  
  int NUMBER;
  int MODE;

  ////////////////////////////
  // INITIALIZATION WIDGETS //
  ////////////////////////////
  // Initialize and Measure
  gpib      *GPIB;   
  GtkWidget **gpibBUTTONS;
  GtkWidget **gpibADDRS; 

  ////////////////////////
  // PULSE MODE WIDGETS // 
  ////////////////////////
  GtkWidget **gBIAS; 
  GtkWidget **dBIAS; 
  GtkWidget **gLABELS; 
  GtkWidget **dLABELS;
  GtkWidget *setGRID;
  point_t   *grid;


  ////////////////////////
  //     SET PARAMS     //
  ////////////////////////
  GtkWidget **PARAMS; 
  GtkWidget **pLABELS;  
  GtkWidget *MEAS;
  GtkWidget *dialog;

  ////////////////////////
  //     TEST ENV       //
  ////////////////////////
  GtkWidget *resetScope; 
  GtkWidget **TEST; 
  GtkWidget **tLABELS;
    
  ////////////////////////
  // Save Data widgets  //
  ////////////////////////
  GtkWidget **chSELECT;

  char* listSTR;
  GtkWidget *saveWINDOW;
  GtkWidget *saveBUTTON;
  GtkWidget *saveENTRY;
  GtkWidget *saveDATA;
  GtkWidget *saveINC; 
  GtkWidget *saveLABEL;
  char* filename;
  int increment; 

}GTKwrapper;


static void generateMODELABEL(GTKwrapper* state, char* str){
  state->MODELABEL = gtk_label_new(NULL);
  gtk_label_set_use_markup (GTK_LABEL (state->MODELABEL),TRUE);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->MODELABEL, (int)X1,  (int)Y1+10);

  const char* format;
  format = "<span font=\"20.0\" weight=\"bold\" foreground=\"#81369E\">%s</span>";
  
  char *markup;
  markup = g_markup_printf_escaped (format, str);
  gtk_label_set_markup (GTK_LABEL (state->MODELABEL),markup);
  g_free (markup);
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////
//      INITIALIZE GPIB CALLBACKS     //
////////////////////////////////////////
static void SETKEITHLEY(GtkWidget *gpibADDR, GTKwrapper* state)
{ 
  int tmp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(state->gpibADDRS[0]));
  state->GPIB->a_keithley = _initialize(tmp); 
  return;
}
static void SETSOURCE(GtkWidget *gpibADDR, GTKwrapper* state)
{ 
  int tmp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(state->gpibADDRS[1]));
  state->GPIB->a_source = _initialize(tmp); 
  return;
}
static void SETPULSE(GtkWidget *gpibADDR,  GTKwrapper* state)
{ 
  int tmp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(state->gpibADDRS[2]));
  state->GPIB->a_pulse = _initialize(tmp); 
  return;
}
static void SETSCOPE(GtkWidget *gpibADDR,  GTKwrapper* state)
{ 
  int tmp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(state->gpibADDRS[3]));
  state->GPIB->a_scope = _initialize(tmp); 
  return;
}

////////////////////////////////
//  INITIALIZE GPIB CONTROLS  //
////////////////////////////////
static void gpib_INIT(GTKwrapper* state){
  state->gpibBUTTONS = g_new(GtkWidget*, 4);
  state->gpibADDRS   = g_new(GtkWidget*, 4);
  const char *labels[4];
  labels[0]="Keithley GPIB";
  labels[1]="Source GPIB";
  labels[2]="Pulse GPIB";
  labels[3]="Scope GPIB";
  int defaults[4] = {1, 6, 9, 7}; 
  
  void (*gpibSetters[4]) (GtkWidget *gpibADDR, GTKwrapper* state); 
  gpibSetters[0] = SETKEITHLEY;
  gpibSetters[1] = SETSOURCE;
  gpibSetters[2] = SETPULSE; 
  gpibSetters[3] = SETSCOPE;

  int i;
  for(i=0; i<4; i++){
    state->gpibBUTTONS[i] = gtk_button_new_with_label(labels[i]);
    g_signal_connect(state->gpibBUTTONS[i],"clicked", G_CALLBACK(gpibSetters[i]),state);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->gpibBUTTONS[i], X1, Y2 + YSPACE*i);
    gtk_widget_set_size_request(state->gpibBUTTONS[i], BWIDTH, BHEIGHT);

    /* GPIB address selector */
    GtkWidget* adj = (GtkWidget*)gtk_adjustment_new(defaults[i],0,30,1,1,0);
    state->gpibADDRS[i] = gtk_spin_button_new(GTK_ADJUSTMENT(adj),1,2);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON (state->gpibADDRS[i]),0);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->gpibADDRS[i], X2, Y2+ YSPACE*i);
    gtk_widget_set_size_request(state->gpibADDRS[i], 100 , BHEIGHT);
  }
}

static void PULSE_SWITCH (char* MODE, GTKwrapper* state){
  char msg[32]; 
  strcpy(msg, "Switch Output to ");
  strcat(msg, MODE);
  state->dialog = gtk_message_dialog_new( (GtkWindow*)state->window,
					  GTK_DIALOG_DESTROY_WITH_PARENT,
					  GTK_MESSAGE_ERROR,
					  GTK_BUTTONS_OK,
					  (gchar*)msg);
  g_signal_connect(state->dialog, "response", G_CALLBACK (gtk_widget_destroy), state->dialog);
  gtk_dialog_run (GTK_DIALOG (state->dialog));
  // This prevents the dialog from hanging ...
  while (gtk_events_pending())
    gtk_main_iteration();
  sourceON(state->GPIB->a_source);
}


static void MEASURE(GtkWidget *measBUTTON, GTKwrapper* state)
{

  scopeMeasureMode(state->GPIB->a_scope, (char*)gtk_entry_get_text((GtkEntry*)state->PARAMS[2]));
  int PULSEMODE = (int)gtk_switch_get_active ((GtkSwitch*)state->gBIAS[3]);
  point_t* current = state->grid;
  char* DATA;

  // Protect measurement to avoid unwanted biasing/pulsing
  if ( current->vg < 0.0){ 
    sourceBias(state->GPIB->a_source, "0.0"); 
    sourceOFF(state->GPIB->a_source);
    pulseOFF(state->GPIB->a_pulse);
    pulseNEG(state->GPIB->a_pulse);
    PULSE_SWITCH("NEG", state);
  }
  else{
    sourceBias(state->GPIB->a_source, "0.0"); 
    sourceOFF(state->GPIB->a_source);
    pulseOFF(state->GPIB->a_pulse);
    pulsePOS(state->GPIB->a_pulse);
    PULSE_SWITCH("POS", state); 
  }

  // First Dialog
  if (PULSEMODE) {
    char _vg[8];
    char _vd[8];
    while (current != NULL) {
      sprintf(_vg, "%.2f", current->vg);
      sprintf(_vd, "%.2f", current->vd);
      scopeRUN(state->GPIB->a_scope);
      sourceON(state->GPIB->a_source); 
      if ( current->vg < 0.0 )  {
	pulseLevel(state->GPIB->a_pulse, _vg, "0.0");
	pulseNEG(state->GPIB->a_pulse);
	pulseON(state->GPIB->a_pulse);
	sourceBias(state->GPIB->a_source, _vd); 

	usleep(500000);
	current->c1 = (char*)scopeDATA(state->GPIB->a_scope, 1, BUFFERSIZE); 
	current->c2 = (char*)scopeDATA(state->GPIB->a_scope, 2, BUFFERSIZE); 
	current->c3 = (char*)scopeDATA(state->GPIB->a_scope, 3, BUFFERSIZE); 
	current->c4 = (char*)scopeDATA(state->GPIB->a_scope, 4, BUFFERSIZE); 
	
	// Switch mode if necessary
	if (current->next != NULL && current->next->vg >= 0.0){
	  sourceBias(state->GPIB->a_source, "0.0"); 
	  sourceOFF(state->GPIB->a_source);
	  pulseOFF(state->GPIB->a_pulse);
	  pulsePOS(state->GPIB->a_pulse);
	  PULSE_SWITCH("POS", state); 
	}
      }
      if ( current->vg > 0.0 ){
	pulseLevel(state->GPIB->a_pulse, "0.0", _vg);
	pulsePOS(state->GPIB->a_pulse);
	pulseON(state->GPIB->a_pulse);
	sourceBias(state->GPIB->a_source, _vd);

	usleep(500000);
	current->c1 = (char*)scopeDATA(state->GPIB->a_scope, 1, BUFFERSIZE); 
	current->c2 = (char*)scopeDATA(state->GPIB->a_scope, 2, BUFFERSIZE); 
	current->c3 = (char*)scopeDATA(state->GPIB->a_scope, 3, BUFFERSIZE); 
	current->c4 = (char*)scopeDATA(state->GPIB->a_scope, 4, BUFFERSIZE); 
	
	if (current->next != NULL && current->next->vg <= 0.0){
	  sourceBias(state->GPIB->a_source, "0.0"); 
	  sourceOFF(state->GPIB->a_source);
	  pulseOFF(state->GPIB->a_pulse);
	  pulseNEG(state->GPIB->a_pulse);
	  PULSE_SWITCH("NEG", state); 
	}
      }
      if ( current->vg == 0.0){
	pulseOFF(state->GPIB->a_pulse);
	pulseLevel(state->GPIB->a_pulse,  "0.0", "0.1");
	pulsePOS(state->GPIB->a_pulse);
	sourceBias(state->GPIB->a_source, _vd);

	usleep(500000);
	current->c1 = (char*)scopeDATA(state->GPIB->a_scope, 1, BUFFERSIZE); 
	current->c2 = (char*)scopeDATA(state->GPIB->a_scope, 2, BUFFERSIZE); 
	current->c3 = (char*)scopeDATA(state->GPIB->a_scope, 3, BUFFERSIZE); 
	current->c4 = (char*)scopeDATA(state->GPIB->a_scope, 4, BUFFERSIZE); 
      }
      current = current->next;
    }
    sourceOFF(state->GPIB->a_source); 
    pulseOFF(state->GPIB->a_pulse);
  }
  else {
    g_print("Keithley mode \n");
    /////////////////////////
    // KEITHLEY MODE HERE  //
    /////////////////////////
    //measurePULSED(state->gpib, state);
  }
  print_grid(state->grid); 
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
static void RESETSCOPE(GtkWidget *BUTTON, GTKwrapper* state){
  scopeDefaults(state->GPIB->a_scope);
  gtk_toggle_button_set_active ((GtkToggleButton*)state->chSELECT[0], TRUE);
  gtk_toggle_button_set_active ((GtkToggleButton*)state->chSELECT[1], TRUE);
  gtk_toggle_button_set_active ((GtkToggleButton*)state->chSELECT[2], TRUE);
  gtk_toggle_button_set_active ((GtkToggleButton*)state->chSELECT[3], TRUE);
}

static void PULSETEST(GtkWidget *BUTTON, GTKwrapper* state){ 

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(state->TEST[2]))){
    if (atof((char*)gtk_entry_get_text((GtkEntry*)state->TEST[0]))  > 0.0){
      pulseLevel(state->GPIB->a_pulse, "0.0", (char*)gtk_entry_get_text((GtkEntry*)state->TEST[0]));
      sourceBias(state->GPIB->a_source, (char*)gtk_entry_get_text((GtkEntry*)state->TEST[1]));
      pulsePOS(state->GPIB->a_pulse);
      pulseON(state->GPIB->a_pulse);
      sourceON(state->GPIB->a_source); 
    }
    else if (atof((char*)gtk_entry_get_text((GtkEntry*)state->TEST[0])) == 0.0){
      pulseLevel(state->GPIB->a_pulse,  "0.0", "0.001");
      sourceBias(state->GPIB->a_source, (char*)gtk_entry_get_text((GtkEntry*)state->TEST[1]));
      pulsePOS(state->GPIB->a_pulse);
      pulseON(state->GPIB->a_pulse);
      sourceON(state->GPIB->a_source); 
    }
    else{
      pulseLevel(state->GPIB->a_pulse, (char*)gtk_entry_get_text((GtkEntry*)state->TEST[0]), "0.0");
      sourceBias(state->GPIB->a_source,(char*)gtk_entry_get_text((GtkEntry*)state->TEST[1]));
      pulseNEG(state->GPIB->a_pulse);
      pulseON(state->GPIB->a_pulse);
      sourceON(state->GPIB->a_source); 
    }
  }
  else {
    sourceOFF(state->GPIB->a_source); 
    pulseOFF(state->GPIB->a_pulse);
  } 
}

static void test_INIT(GTKwrapper* state){ 

  state->TEST    = g_new(GtkWidget*, 3);
  state->tLABELS = g_new(GtkWidget*, 3);
  
  // GATE Test
  state->TEST[0] = gtk_entry_new();
  gtk_editable_set_editable(GTK_EDITABLE(state->TEST[0]), TRUE);
  gtk_entry_set_width_chars((GtkEntry*)state->TEST[0],14);
  gtk_entry_set_text(GTK_ENTRY(state->TEST[0]),"2.0");
  gtk_widget_set_size_request(state->TEST[0], BWIDTH, BHEIGHT);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->TEST[0], X6, Y2);
  state->tLABELS[0] = gtk_label_new("Gate Level (V)");
  gtk_fixed_put(GTK_FIXED(state->fixed), state->tLABELS[0], X6, (int)Y2-20);

  // DRAIN Test
  state->TEST[1] = gtk_entry_new();
  gtk_editable_set_editable(GTK_EDITABLE(state->TEST[1]), TRUE);
  gtk_entry_set_width_chars((GtkEntry*)state->TEST[1],14);
  gtk_entry_set_text(GTK_ENTRY(state->TEST[1]),"5.0");
  gtk_widget_set_size_request(state->TEST[1], BWIDTH, BHEIGHT);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->TEST[1], X6, Y3);
  state->tLABELS[1] = gtk_label_new("Drain Level (V)");
  gtk_fixed_put(GTK_FIXED(state->fixed), state->tLABELS[1], X6, (int)Y3-20);

  state->TEST[2] = gtk_check_button_new_with_label("Test Pulses");
  gtk_fixed_put(GTK_FIXED(state->fixed), state->TEST[2], X6, Y3+45);
  g_signal_connect(state->TEST[2],"toggled", G_CALLBACK(PULSETEST), state);

  // PULSE testing
  state->resetScope  = gtk_button_new_with_label("Reset Scope");
  g_signal_connect(state->resetScope,"clicked", G_CALLBACK(RESETSCOPE), state);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->resetScope, X6, Y4);
  gtk_widget_set_size_request(state->resetScope, BWIDTH, BHEIGHT); 
}

static void SETPARAMS(GtkWidget *paramsBUTTON, GTKwrapper* state){
  pulseWidth(state->GPIB->a_pulse, (char*)gtk_entry_get_text((GtkEntry*)state->PARAMS[0]));
  pulsePeriod(state->GPIB->a_pulse, (char*)gtk_entry_get_text((GtkEntry*)state->PARAMS[1]));
}

static void params_INIT(GTKwrapper* state){
  state->PARAMS  = g_new(GtkWidget*, 4);
  state->pLABELS = g_new(GtkWidget*, 4);

  const char *plabels[3];
  plabels[0]="Pulse Width (us)";
  plabels[1]="Duty Cycle (ms)";
  plabels[2]="Averages (#)";

  const char *defaults[3];
  defaults[0]="10.0";
  defaults[1]="1.0";
  defaults[2]="16";

  int i;
  for (i=0; i<3; i++){
    state->PARAMS[i] = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(state->PARAMS[i]), TRUE);
    gtk_entry_set_width_chars((GtkEntry*)state->PARAMS[i],14);
    gtk_widget_set_size_request(state->PARAMS[i], BWIDTH, BHEIGHT);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->PARAMS[i], X1+XSPACE*i, Y2);
    gtk_entry_set_text(GTK_ENTRY(state->PARAMS[i]),defaults[i]);
    state->pLABELS[i] = gtk_label_new(plabels[i]);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->pLABELS[i], X1+XSPACE*i, (int)Y2-20);
  }
 
  // SET PARAMS
  state->PARAMS[3]  = gtk_button_new_with_label("Set PARAMS");
  g_signal_connect(state->PARAMS[3],"clicked", G_CALLBACK(SETPARAMS), state);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->PARAMS[3], X4, Y2);
  gtk_widget_set_size_request(state->PARAMS[3], BWIDTH, BHEIGHT);

  // MEASURE
  state->MEAS  = gtk_button_new_with_label("Measure");
  g_signal_connect(state->MEAS,"clicked", G_CALLBACK(MEASURE), state);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->MEAS, X6, Y1);
  gtk_widget_set_size_request(state->MEAS, BWIDTH, BHEIGHT);
}

static void SETGRID(GtkWidget *gpibADDR,  GTKwrapper* state){

  // Gate Biases
  float vgm = atof( (char*)gtk_entry_get_text((GtkEntry*)state->gBIAS[0]) );  
  float vgx = atof( (char*)gtk_entry_get_text((GtkEntry*)state->gBIAS[1]) );  
  float vgs = atof( (char*)gtk_entry_get_text((GtkEntry*)state->gBIAS[2]) );  

  // Drain Biases
  float vdm = atof( (char*)gtk_entry_get_text((GtkEntry*)state->dBIAS[0]) );  
  float vdx = atof( (char*)gtk_entry_get_text((GtkEntry*)state->dBIAS[1]) );  
  float vds = atof( (char*)gtk_entry_get_text((GtkEntry*)state->dBIAS[2]) );  

  point_t* grid = malloc(sizeof(point_t));
  grid->vg   = 0.0;
  grid->vd   = 0.0; 
  grid->c1   = NULL;
  grid->c2   = NULL;
  grid->c3   = NULL;
  grid->c4   = NULL;
  grid->next = NULL; 

  point_t* current = grid;
  float vg, vd;
  for (vg = vgm; vg<= vgx+0.001; vg+=vgs){
    for (vd = vdm; vd<= vdx+0.001; vd+=vds){

      current->next = malloc(sizeof(point_t));
      current->next->vg   = vg;
      current->next->vd   = vd;
      current->next->c1   = NULL;
      current->next->c2   = NULL;
      current->next->c3   = NULL;
      current->next->c4   = NULL;
      current->next->next = NULL;
      current = current->next;
    }
  }
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(state->dBIAS[3]))){
    for (vg = vgx-vgs; vg>= vgm-0.001; vg-=vgs){
      for (vd = vdx-vds; vd>= vdm-0.001; vd-=vds){
  	current->next = malloc(sizeof(node_t));
  	current->next->vg = vg;
  	current->next->vd = vd;
	current->next->c1   = NULL;
	current->next->c2   = NULL;
	current->next->c3   = NULL;
	current->next->c4   = NULL;
  	current = current->next;
      }
    }
  }
  state->grid = grid->next;
  print_grid(state->grid); 
}

static void grid_INIT(GTKwrapper* state){ 

  state->gBIAS   = g_new(GtkWidget*, 4);
  state->dBIAS   = g_new(GtkWidget*, 4); 
  state->gLABELS = g_new(GtkWidget*, 4);
  state->dLABELS = g_new(GtkWidget*, 4);

  const char *glabels[4];
  glabels[0]="Vg Min";
  glabels[1]="Vg Max";
  glabels[2]="Vg Step";
  glabels[3]="Pulsed Vg";

  const char *defaults[3];
  defaults[0]="0.0";
  defaults[1]="5.0";
  defaults[2]="1.0";

  int i;
  // GATE grid
  for (i=0; i<3; i++){
    state->gBIAS[i] = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(state->gBIAS[i]), TRUE);
    gtk_entry_set_width_chars((GtkEntry*)state->gBIAS[i],14);
    gtk_widget_set_size_request(state->gBIAS[i], BWIDTH, BHEIGHT);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->gBIAS[i], X1+XSPACE*i, Y4);
    gtk_entry_set_text(GTK_ENTRY(state->gBIAS[i]),defaults[i]);
    state->gLABELS[i] = gtk_label_new(glabels[i]);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->gLABELS[i], X1+XSPACE*i, (int)Y4-20);
  }
  state->gBIAS[3] =  gtk_switch_new();
  gtk_widget_set_size_request(state->gBIAS[3], BWIDTH, BHEIGHT);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->gBIAS[3], X4, Y4);
  gtk_switch_set_active ((GtkSwitch*)state->gBIAS[3], TRUE);
  state->gLABELS[3] = gtk_label_new(glabels[3]);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->gLABELS[3], X4, (Y4-20));

  // DRAIN grid
  const char *dlabels[3];
  dlabels[0]="Vd Min";
  dlabels[1]="Vd Max";
  dlabels[2]="Vd Step";
  for (i=0; i<3; i++){
    state->dBIAS[i] = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(state->dBIAS[i]), TRUE);
    gtk_entry_set_width_chars((GtkEntry*)state->dBIAS[i],14);
    gtk_widget_set_size_request(state->dBIAS[i], BWIDTH, BHEIGHT);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->dBIAS[i], X1+XSPACE*i, Y5);
    gtk_entry_set_text(GTK_ENTRY(state->dBIAS[i]),defaults[i]);
    state->dLABELS[i] = gtk_label_new(dlabels[i]);
    gtk_fixed_put(GTK_FIXED(state->fixed), state->dLABELS[i], X1+XSPACE*i, (int)Y5-20);
  }
 
  // SET GRID
  state->setGRID  = gtk_button_new_with_label("Set GRID");
  g_signal_connect(state->setGRID,"clicked", G_CALLBACK(SETGRID), state);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->setGRID, X4, Y5);
  gtk_widget_set_size_request(state->setGRID, BWIDTH, BHEIGHT);

  state->dBIAS[3] = gtk_check_button_new_with_label("Hysteresis");
  gtk_fixed_put(GTK_FIXED(state->fixed), state->dBIAS[3], X4, Y5-25);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////
// SAVE CONTROL CALLBACKS //
////////////////////////////
static void set_CH1(GtkWidget *chSELECT, GTKwrapper* state){
  
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chSELECT))){
    scopeSetCH(state->GPIB->a_scope, "1", "1");
  }else{scopeSetCH(state->GPIB->a_scope, "1", "0");}
}
static void set_CH2(GtkWidget *chSELECT, GTKwrapper* state){
  
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chSELECT))){
    scopeSetCH(state->GPIB->a_scope, "2", "1");
  }else{scopeSetCH(state->GPIB->a_scope, "2", "0");}
}
static void set_CH3(GtkWidget *chSELECT, GTKwrapper* state){
  
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chSELECT))){
    scopeSetCH(state->GPIB->a_scope, "3", "1");
  }else{scopeSetCH(state->GPIB->a_scope, "3", "0");}
}
static void set_CH4(GtkWidget *chSELECT, GTKwrapper* state){
  
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chSELECT))){
    scopeSetCH(state->GPIB->a_scope, "4", "1");
  }else{scopeSetCH(state->GPIB->a_scope, "4", "0");}
}

static void chSelect_INIT(GTKwrapper* state){
  state->chSELECT = g_new(GtkWidget*, 4);
  const char *labels[4];
  labels[0]="Channel 1 (Vg)";
  labels[1]="Channel 2 (Vd)";
  labels[2]="Channel 3 (Id)";
  labels[3]="Channel 4 (Ig)";

  void (*chSetters[4]) (GtkWidget *gpibADDR, GTKwrapper* state); 
  chSetters[0] = set_CH1;
  chSetters[1] = set_CH2;
  chSetters[2] = set_CH3; 
  chSetters[3] = set_CH4;

  int i;
  for (i = 0; i<4; i++){
      state->chSELECT[i] = gtk_check_button_new_with_label(labels[i]);
      gtk_fixed_put(GTK_FIXED(state->fixed), state->chSELECT[i], X1+XSPACE*i, Y3);
      g_signal_connect(state->chSELECT[i],"toggled", G_CALLBACK(chSetters[i]), state);
  }
}

static void SAVEPATH(GtkWidget* saveBUTTON, GTKwrapper* state){

  GtkWidget *chooser;
  chooser = gtk_file_chooser_dialog_new ("Open File...",
					 (GtkWindow*)state->window,
					 GTK_FILE_CHOOSER_ACTION_SAVE,
					 (gchar*)"_Cancel", 
					 GTK_RESPONSE_CANCEL,
					 (gchar*)"_Open", 
					 GTK_RESPONSE_OK,
					 NULL);
	
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (chooser), TRUE);
  if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_OK)
  {
      state->filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));
      gtk_entry_set_text(GTK_ENTRY(state->saveENTRY), state->filename);
  }
  gtk_widget_destroy (chooser);
}

static void tokenize(char** a, char* tmp){
  int i = 0;
  char *tok = strtok (tmp, ",");
  while (tok != NULL){a[i++] = tok; tok = strtok (NULL, ",");}
}

static void savedata(char* filename, GTKwrapper* state){ 


  if(state->grid == NULL){   
    gtk_entry_set_text(GTK_ENTRY(state->saveENTRY),"NO DATA IN BUFFER !! Measure Something ...");
    return;
  }

  point_t* current = state->grid;
  if(current->c1 == NULL){   
    gtk_entry_set_text(GTK_ENTRY(state->saveENTRY),"NO DATA IN BUFFER !! Measure Something ...");
    return;
  }

  int i;
  FILE *file;   
  file = fopen(filename,"w+"); 

  char header[256];
  char buffer[256];
  char tmp[BUFFERSIZE];

  while (current->next != NULL){
    sprintf(header, "## %f %f \n", current->vg,  current->vd);
    fprintf(file, header);
    sprintf(header, "** CH1\t\tCH2\t\tCH3\t\tCH4\n");
    fprintf(file, header);
   
    char *a1[1000];
    strcpy(tmp, current->c1);
    tokenize(a1, tmp); 
    
    char *a2[1000];
    strcpy(tmp, current->c2);
    tokenize(a2, tmp); 
    
    char *a3[1000];
    strcpy(tmp, current->c3);
    tokenize(a3, tmp); 
    
    char *a4[1000];
    strcpy(tmp, current->c4);
    tokenize(a4, tmp); 
    
    for (i=1; i<999; i++){
      fprintf(file, "%s\t\t%s\t\t%s\t\t%s\n", a1[i], a2[i],a3[i],a4[i]);
    }
    current = current->next;
  }
  fclose(file);
}


static void SAVEDATA(GtkWidget* saveBUTTON, GTKwrapper* state){

  if (!strcmp(state->filename,"")){
    gtk_entry_set_text(GTK_ENTRY(state->saveENTRY),"ERROR .. FILENAME NOT SET!!");
    return;
  }
  int INC = (int)gtk_switch_get_active ((GtkSwitch*)state->saveINC);
  if (INC) {
    // Append the incrementor to the filename
    // e.g. you will get <path.dat.0> etc. We 
    // need some string manipulation for this. 
    char tmpPath[100]; 
    char incPath[3];

    strcpy(tmpPath,state->filename);
    int len = strlen(tmpPath);
    sprintf(incPath, "%d", state->increment);

    // add a . to the pathname
    tmpPath[len]   = '.';
    tmpPath[len+1] = '\0';
    strcat(tmpPath, incPath);

    // update the file name in the entry field
    gtk_entry_set_text(GTK_ENTRY(state->saveENTRY), tmpPath);
    state->increment++;
    savedata(tmpPath, state);
  }
  else {
    // if the incrementor is deselected then reset.
    state->increment = 0;
    savedata(state->filename, state);
  }
}

/////////////////////////////
// SAVE CONTROL GENERATION //
/////////////////////////////
static void save_INIT(GTKwrapper *state)
{
  /* set SMU control */
  state->saveBUTTON = gtk_button_new_with_label("<filename>");
  g_signal_connect(state->saveBUTTON,"clicked", G_CALLBACK(SAVEPATH), state);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->saveBUTTON, X1, Y6);
  gtk_widget_set_size_request(state->saveBUTTON, BWIDTH, BHEIGHT);

  // entry box to display filename
  state->saveENTRY = gtk_entry_new();
  gtk_editable_set_editable(GTK_EDITABLE(state->saveENTRY), FALSE);
  gtk_widget_set_size_request(state->saveENTRY, 3*XSPACE-25, BHEIGHT);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->saveENTRY,X2,Y6);
  
  // Incrementor. This allows data to be saved in rapid sucsession 
  // e.g. data.dat.0 .... data.dat.n. If toggled, it will increment 
  // the save path ... otherwise it will overwrite the data
  state->saveINC =  gtk_switch_new();
  gtk_widget_set_size_request(state->saveINC, BWIDTH, BHEIGHT);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->saveINC, X5, Y6);
  gtk_switch_set_active ((GtkSwitch*)state->saveINC, TRUE);
  state->saveLABEL = gtk_label_new("File Incrementor");
  gtk_fixed_put(GTK_FIXED(state->fixed), state->saveLABEL, X5, (Y6-20));

  // Save Button
  state->saveDATA = gtk_button_new_with_label("SAVE");
  g_signal_connect(state->saveDATA,"clicked", G_CALLBACK(SAVEDATA), state);
  gtk_fixed_put(GTK_FIXED(state->fixed), state->saveDATA, X6, Y6);
  gtk_widget_set_size_request(state->saveDATA, BWIDTH, BHEIGHT);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//                     SWEEP AND SAMPLING MODE DESTRUCTORS                           // 
///////////////////////////////////////////////////////////////////////////////////////
static void destroyPULSE(GTKwrapper* state){

  // Protection ... so we do not try to destroy on startup
  if (state->MODE == 0){return;}
  if ((state->saveLABEL != NULL)) { 
    int i; 
    for (i=0; i<4; i++){
      gtk_widget_destroy(state->gLABELS[i]); 
      gtk_widget_destroy(state->gBIAS[i]); 
      gtk_widget_destroy(state->dBIAS[i]); 
      gtk_widget_destroy(state->PARAMS[i]); 
      gtk_widget_destroy(state->chSELECT[i]);
      gtk_widget_destroy(state->setGRID); 
    }
    for (i=0; i<3; i++){
      gtk_widget_destroy(state->dLABELS[i]); 
      gtk_widget_destroy(state->pLABELS[i]); 
      gtk_widget_destroy(state->TEST[i]); 
    }

    for (i=0; i<2; i++){
      gtk_widget_destroy(state->tLABELS[i]); 
    }

    gtk_widget_destroy(state->MEAS);
    gtk_widget_destroy(state->resetScope);
    gtk_widget_destroy(state->saveDATA); 
    gtk_widget_destroy(state->saveINC); 
    gtk_widget_destroy(state->saveLABEL); 
    gtk_widget_destroy(state->saveENTRY); 
    gtk_widget_destroy(state->saveBUTTON);
  }
}

static void destroyGPIB(GTKwrapper* state){

  // Protection ... so we do not try to destroy on startup
  if (state->MODE == 0){return;}

  // DESTROY GPIB BUTTONS
  int i; 
  for (i=0; i<4; i++){
    gtk_widget_destroy(state->gpibBUTTONS[i]); 
    gtk_widget_destroy(state->gpibADDRS[i]); 
  }
}

///////////////////////////////////////////////////////////////////////////////////////
//                            PULSE MODE CONTROL                                     // 
///////////////////////////////////////////////////////////////////////////////////////
static void generatePULSE(GSimpleAction *action, GVariant*parameter,  void* gui_state)
{  
  GTKwrapper* _state = (GTKwrapper*)malloc(sizeof(GTKwrapper*));
  _state = gui_state; 

  if (_state->MODE != 1){
    destroyGPIB(_state);
    _state->MODE = 1;
    
    chSelect_INIT(_state);
    params_INIT(_state);
    grid_INIT(_state);
    test_INIT(_state);
    save_INIT(_state);
  }
  gtk_widget_show_all(GTK_WIDGET(_state->window)); 
}

///////////////////////////////////////////////////////////////////////////////////////
//                            GPIB MODE CONTROL                                      // 
///////////////////////////////////////////////////////////////////////////////////////
static void generateGPIB(GSimpleAction *action, GVariant*parameter, void* gui_state)
{
  GTKwrapper* _state = (GTKwrapper*)malloc(sizeof(GTKwrapper*));
  _state = gui_state; 
  if (_state->MODE != 2){
    destroyPULSE(_state);
    gpib_INIT(_state);
    _state->MODE = 2;
  }

  
  gtk_widget_show_all(GTK_WIDGET(_state->window)); 
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//                    APPLICATION GENERATION                           //
/////////////////////////////////////////////////////////////////////////
static void quit(GSimpleAction *action, GVariant *parameter, void* gui_state)
{
  GTKwrapper* _state = (GTKwrapper*)malloc(sizeof(GTKwrapper));
  _state = gui_state; 
  g_application_quit((GApplication*)_state->app);
}

static void startup(GtkApplication* app, GTKwrapper* state)
{
  static const GActionEntry actions[] = {
    {"generatePULSE",generatePULSE},
    {"generateGPIB",generateGPIB},
    {"quit", quit}
  };
 
  GMenu *menu;
  GMenu *measMENU;
  menu = g_menu_new ();
  measMENU = g_menu_new ();
  g_menu_append (measMENU, "Measurement", "app.generatePULSE");
  g_menu_append (measMENU, "GPIB Setup", "app.generateGPIB");
  g_menu_append_submenu (menu,"Pulsed IV",G_MENU_MODEL(measMENU));
  g_menu_append (menu, "Quit", "app.quit");

  g_action_map_add_action_entries (G_ACTION_MAP(app), actions, G_N_ELEMENTS(actions), state);
  gtk_application_set_menubar (app, G_MENU_MODEL (menu));
  g_object_unref(menu);
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, void* gui_state)
{ 
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_rgba(cr, 0.85, 0.85, 0.85, 1.0);
  cairo_paint (cr);
}

static void activate (GtkApplication *app, GTKwrapper* state)
{
  state->NUMBER = 1;
  state->window = (GtkWidget*)gtk_application_window_new (app);
  gtk_window_set_application (GTK_WINDOW (state->window), GTK_APPLICATION (app));
  gtk_window_set_default_size(GTK_WINDOW(state->window), WIDTH, HEIGHT);
  gtk_window_set_title (GTK_WINDOW (state->window), "Pulse GUI");
  gtk_window_set_position(GTK_WINDOW(state->window), GTK_WIN_POS_CENTER);
  gtk_widget_set_app_paintable(state->window, TRUE);
  gtk_window_set_decorated(GTK_WINDOW(state->window), TRUE);
  gtk_widget_set_opacity(GTK_WIDGET(state->window),0.9);

  state->fixed = gtk_fixed_new();
  gtk_widget_set_size_request (state->fixed, WIDTH, HEIGHT);
  gtk_container_add(GTK_CONTAINER(state->window), state->fixed);

  g_signal_connect(G_OBJECT(state->window), "draw", G_CALLBACK(draw_cb), NULL);
  generateMODELABEL(state,"Pulsemaster v1.00");
  gpib_INIT(state);
  state->MODE=2;
  gtk_widget_show_all (GTK_WIDGET(state->window));
}

///////////////////////////////////////////////////////////////////////////
//                    ------- MAIN LOOP -------                          //
///////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
  GTKwrapper* state = (GTKwrapper*)malloc(sizeof(GTKwrapper));
  state->filename   = malloc(100);
  state->listSTR    = malloc(100);
  state->increment  = 0;
  state->MODE       = 0;

  state->GPIB             = (gpib*)malloc(sizeof(gpib)); 
  state->GPIB->a_keithley = 0;
  state->GPIB->a_source   = 0; 
  state->GPIB->a_scope    = 0;
  state->GPIB->a_pulse    = 0; 

  state->app = gtk_application_new ("org.gtk.example",G_APPLICATION_FLAGS_NONE);
  g_signal_connect (state->app, "startup", G_CALLBACK (startup), state);
  g_signal_connect (state->app, "activate", G_CALLBACK (activate), state);
  g_application_run (G_APPLICATION (state->app), argc, argv);
  g_object_unref (state->app);
  return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////












