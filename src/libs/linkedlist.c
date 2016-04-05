#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

node_t* initialize_list(void){
  node_t *list = malloc(sizeof(*list));
  list->data = (char*)malloc(16*sizeof(char));
  strcpy(list->data,"\0");
  list->next = NULL;
  list->head = list;
  return list;
}

int print_list(node_t *list){
 
  if ((list->data[0] == '\0') && (list->next == NULL)){
    printf("[EMPTY LIST]\n");
    return -1;
  }
  
  node_t *current;
  current = list;
  while (current->next != NULL){
    printf("(list = %x) %x %s ----> %x\n", current->head, 
	   current, current->data, current->next);
    current = current->next;
  }
  // rewind pointer to head of list
  current = current->head;
  return 0;
}

char* print_list_to_string(node_t *list){
  
  char* str; 
  str = (char*)malloc(32);

  strcpy(str,"");
  node_t *current = list;  
  if ((list->data[0] == '\0') && (list->next == NULL)){
    strcpy(str, "[EMPTY LIST]\0");
    current = current->head;
    return str;
  }

  while (current->next != NULL){
    strcat(str,strdup(current->data));
    strcat(str," ");
    
    current = current->next;
  }
  // rewind pointer to head of list
  current = current->head;
  return str;
}

int count_list(node_t *list){
  int result = 0;
  node_t *current = list;  
  if ((list->data[0] == '\0') && (list->next == NULL)){
    return result;
  }
  
  while (current->next != NULL){
    current = current->next;
    result++;
  }
  return result;
}

int add_to_list(node_t *list, char* data){
  
  if ((list->data[0] == '\0') && (list->next == NULL)){
    list->head = list;
    list->data = data; 
    list->next = NULL;
  }
  node_t *current = list;
  while (current->next != NULL){
    current = current->next;
  }
  current->next = malloc(sizeof(*list));
  current->data = strdup(data);
  current->next->head = list;
  current->next->next = NULL;
  return 0;
}

int add_to_list_unique(node_t *list, char* data){
  
  if ((list->data == '\0') && (list->next == NULL)){
    list->data = data; 
    list->next = NULL;
  }
  node_t *current = list;
  while (current->next != NULL){
    if (!(strcmp(current->data, data)))
      return -1;
    current = current->next;
  }
  current->next = malloc(sizeof(*list));
  current->data = strdup(data);
  current->next->head = list;
  current->next->next = NULL;
  return 0;
}

// remove from list may need to modify the head.
// in C all things are PASSED BY VALUE ... so 
// we need to pass a pointer to a pointer here 
// to have the function be able to MODIFY the 
// head pointer.
int remove_from_list(node_t **list, char* data){

  // EMPTY LIST CASE
  if ((*list)->data[0] == '\0' )  {
    return 0;
  }

  int not_found   = 1;
  node_t *curr    = malloc(sizeof(node_t*));
  node_t **match  = malloc(sizeof(node_t**));

  if (count_list( (*list) ) < 1){
    remove_all_from_list(list);
  }

  // CHECK FOR MATCH
  curr = (*list)->head;
  while (curr->next != NULL){
    if (!(strcmp(curr->data, data))){
      memcpy(match,&curr, sizeof(&curr) );
      not_found = 0;
      break;
    }
    curr = curr->next;
  }
  
  if (not_found){
    return 0;
  }

  // Perfrom deletion and linking. In the case
  // where we match at the head, then we need 
  // to move the head to the next list item.
  // for ALLLL list elements
  curr = curr->head;
  if (curr == *match){ 
    curr->head = (*match)->next;
    while(curr->next != NULL){
      curr->next->head = (*match)->next;
      curr = curr->next; 
    }
    *list = (*match)->next;
    return 0;
  } 

  while (curr->next != NULL){
    if (curr->next == *match){
      curr->next = curr->next->next;
      curr->head = curr->next->head;
    }
    curr = curr->next;
  }
  return 0;
}

int destroy_list(node_t** list){
 
  // this is a tricky fuction. It starts at node_i
  // moves the list pointer to node_(i+1) after saving
  // the address of node_i somewhere else. Then it 
  // frees node_i and continues through the list. 
  // all references to list are complelty gone.

  // This is really needed for large lists -- so 
  // they do not get orphaned in memory (e.g. like 
  // during the save data operations. we allocate 
  // a huge amount of memory for the data .. and then
  // we need to free this memory later after dumping 
  // the data to a file. 
  node_t*  current = malloc(sizeof(node_t*));
  node_t** current_free = malloc(sizeof(node_t**));;

  // rewind just in case
  current = (*list)->head;
  while (current->next != NULL){
    memcpy(current_free, &current, sizeof(&current));
    current = current->next;
    free(*current_free);
  }
  free(current);
  free(current_free);
}

// this is the same as destroy ... but a new 
// list is initialized at the end. Again we 
// need to pass the address of list becase 
// we will modify it when we reinitialize !!
int remove_all_from_list(node_t** list){
  
  if (( (*list)->next == NULL) && ((*list)->data[0] == '\0'))  {
    return 0;
  }

  node_t*  current = malloc(sizeof(node_t*));
  node_t** current_free = malloc(sizeof(node_t**));;

  // rewind just in case
  current = (*list)->next;
  while (current->next){
    memcpy(current_free, &current, sizeof(&current));
    current = current->next;
    free(*current_free);
  }
  free(current);
  free(current_free);
  (*list) = initialize_list();
}

/* int main(){ */
/*   node_t* list = initialize_list(); */

/*   int i; */
/*   for (i=0; i<50000; i++){ */
/*     add_to_list(list,"a");  */
/*   } */
/*   print_list(list); */
/*   remove_all_from_list(&list); */
/*   add_to_list(list,"b"); */
/*   print_list(list); */


/*   add_to_list(list,"a"); */
/*   add_to_list(list,"b"); */
/*   add_to_list(list,"c"); */
/*   printf("%s\n",print_list_to_string(list)); */
/*   print_list(list); */

/*   printf("\n"); */
/*   remove_from_list(&list,"a"); */
/*   remove_from_list(&list,"b"); */
/*   remove_from_list(&list,"c"); */
/*   printf("%s\n",print_list_to_string(list)); */
/*   print_list(list); */

/*   add_to_list(list,"a"); */
/*   add_to_list(list,"b"); */
/*   add_to_list(list,"c"); */
/*   printf("%s\n",print_list_to_string(list)); */
/*   print_list(list); */

/*   remove_all_from_list(&list); */
/*   add_to_list(list,"a"); */
/*   printf("%s\n",print_list_to_string(list)); */
/*   print_list(list); */
  
/*   printf("\n"); */
/*   remove_from_list(&list,"a"); */
/*   printf("%s\n",print_list_to_string(list)); */
/* } */


