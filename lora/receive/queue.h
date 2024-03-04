struct queue_node {
  char *content;
  struct queue_node *next;
};

struct queue {
  struct queue_node *head;
  struct queue_node *tail;
  int size;
};

queue *createQueue() {
  struct queue *q = (struct queue*) malloc(sizeof(struct queue));
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
  return q;
}

bool enqueue(struct queue *q, char *message, size_t message_size) {
  if (q == NULL || q->size > 250 || message_size > 255) {
    return false;
  }
  struct queue_node *new_node = (struct queue_node*) malloc(sizeof(struct queue_node));
  new_node->content = (char *) malloc(message_size+1);
  strcpy(new_node->content, message);
  new_node->next = NULL;
  q->size++;
  if (q->head == NULL) {
    q->head = new_node;
    q->tail = new_node;
    return true;
  }
  q->tail->next = new_node;
  q->tail = new_node;
  return true;
}

bool dequeue(struct queue *q, char *buffer, size_t buffer_size) {
  struct queue_node *old;
  if (q == NULL || q->head == NULL) {
      return false;
  }
  old = q->head;
  if (buffer != NULL) {
    strncpy(buffer, old->content, buffer_size-1);
    buffer[buffer_size-1] = '\0';
  }
  q->head = q->head->next;
  if (q->head == NULL) {
      q->tail = NULL;
  }
  free(old->content);
  free(old);
  q->size--;
  return true;
  // Code reused from the course 'Operating Systems and C'
}

int size(struct queue *q) {
  if (q == NULL) {
    return -1;
  }
  return q->size;
}