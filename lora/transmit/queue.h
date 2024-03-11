struct queue_node {
  uint8_t *content;
  struct queue_node *next;
  size_t length;
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

bool enqueue(struct queue *q, uint8_t *message, size_t message_size) {
  if (q == NULL || q->size > 10000 || message_size > 255) {
    return false;
  }
  struct queue_node *new_node = (struct queue_node*) malloc(sizeof(struct queue_node));
  new_node->content = (uint8_t *) malloc(message_size);
  memcpy(new_node->content, message, message_size);
  new_node->next = NULL;
  new_node->length = message_size;
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

size_t dequeue(struct queue *q, uint8_t *buffer) {
  struct queue_node *old;
  if (q == NULL || q->head == NULL) {
      return -1;
  }
  old = q->head;
  if (buffer != NULL) {
    memcpy(buffer, old->content, old->length);
  }
  size_t length = old->length;
  q->head = q->head->next;
  if (q->head == NULL) {
      q->tail = NULL;
  }
  free(old->content);
  free(old);
  q->size--;
  return length;
  // Code reused from the course 'Operating Systems and C'
}

int size(struct queue *q) {
  if (q == NULL) {
    return -1;
  }
  return q->size;
}