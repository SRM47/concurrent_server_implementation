#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void enqueue(struct Queue* q, int element){
       struct Node* elem = (struct Node*)malloc(sizeof(struct Node));
       elem->next = NULL;
       elem->val = element;
       
       q->tail->next = elem;
       q->tail = elem;
}

int dequeue(struct Queue* q){
       if (q->head->next == NULL){
              return -1;
       }
       
       struct Node* temp = q->head->next;
       int res = temp->val;
       free(temp);
       q->head->next = q->head->next->next;
       if (q->head->next == NULL){
              q->tail = q->head;
       }
       return res;
}


