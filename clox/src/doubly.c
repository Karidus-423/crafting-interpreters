// Double Linked list of Heap Allocated Strings.
// Implement - Insert,Add,Delete.

#include <stdio.h>
#include <stdlib.h>

struct listNode {
  struct listNode *next;
  struct listNode *prev;
  char *val;
};
typedef struct listNode listNode;

listNode *CreateNode(char *input) {
  listNode *new_node = malloc(sizeof(listNode));
  new_node->val = input;
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

void HeadInsert(listNode *target_ptr, listNode **head_ptr) {
  target_ptr->next = *head_ptr;

  if (*head_ptr != NULL) {
    (*head_ptr)->prev = target_ptr;
  }

  *head_ptr = target_ptr;
  target_ptr->prev = NULL;
}

void TailInsert(listNode *target_ptr, listNode *tail_ptr) {
  target_ptr->next = tail_ptr->prev;
  tail_ptr = target_ptr;
}

void PrintList(listNode *head_node, listNode *tail_node) {
  printf("Head to Tail : ");
  listNode *tracker = head_node;

  while (tracker != NULL) {
    printf("%s<->", tracker->val);
    tracker = tracker->next;
  }

  printf("\n");

  printf("Tail to Head: ");
  tracker = tail_node;

  while (tracker != NULL) {
    printf("%s<->", tracker->val);
    tracker = tracker->prev;
  }

  printf("\n");
}

int main() {
  listNode *head_ptr, *tail_ptr, *tmp_ptr;
  head_ptr = NULL;
  tail_ptr = NULL;

  for (int i = 0; i < 10; i++) {
    tmp_ptr = CreateNode("HIYA!");
    HeadInsert(tmp_ptr, &head_ptr);
  }

  PrintList(head_ptr, tail_ptr);
}
