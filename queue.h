struct Node {
       struct Node *next;
       int val;
};

struct Queue{
       struct Node *head;
       struct Node *tail;
};

void enqueue(struct Queue* q, int element);

int dequeue(struct Queue* q);