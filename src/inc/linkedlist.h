typedef struct node{
  char *data;
  struct node *next;
  struct node *head;
}node_t;

node_t* initialize_list(void);
int print_list(node_t *list);
int count_list(node_t *list);
int add_to_list(node_t *list, char* data);
int add_to_list_unique(node_t *list, char* data);
int remove_from_list(node_t **list, char* data);
int remove_all_from_list(node_t** list);
int destroy_list(node_t** list);
