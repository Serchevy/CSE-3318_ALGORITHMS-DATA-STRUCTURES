#include <stdio.h>
#include <stdlib.h>

struct node {
  int data;
  struct node *next;
};
typedef struct node *nodePT;

nodePT list_from_array(int N, int A[N]);
nodePT list_from_list(nodePT L);
void destroy_list(nodePT L);
nodePT reverse(nodePT L);
nodePT deleteDuplicates(nodePT L);
nodePT deleteByValue(nodePT nd, int val);

void display(nodePT L);

int main(void) {
  int N = 4, M = 9, Q = 6;
  int A[] = {5, 2, 4, 9};
  int B[] = {1, 3, 3, 5, 5, 5, 6, 8, 8};
  int C[] = {5, 8, 20, 9, 20, 7};

  nodePT node = NULL, node2 = NULL, node2_1 = NULL, node3 = NULL,
         node3_1 = NULL, node4 = NULL, node4_1 = NULL;

  node = list_from_array(N, A);
  display(node);

  node2 = list_from_list(node);
  display(node2);

  node2_1 = reverse(node2);
  display(node2_1);

  node3 = list_from_array(M, B);
  display(node3);
  node3_1 = deleteDuplicates(node3);
  display(node3_1);

  node4 = list_from_array(Q, C);
  display(node4);
  node4_1 = deleteByValue(node4, 20);
  display(node4_1);

  destroy_list(node);
  destroy_list(node2_1);
  destroy_list(node3_1);
  destroy_list(node4_1);

  return 0;
}

void display(nodePT node) {
  while (node != NULL) {
    printf("%d -> ", node->data);
    node = node->next;
  }
  printf("NULL\n");
}

nodePT list_from_array(int N, int A[N]) {
  nodePT head = NULL;
  nodePT temp = NULL;
  nodePT current = NULL;

  for (int i; i < N; i++) {
    temp = (struct node *)malloc(sizeof(struct node));
    temp->data = A[i];
    temp->next = NULL;
    if (head == NULL) {
      head = temp;
      current = temp;
    } else {
      current->next = temp;
      current = current->next;
    }
  }
  return head;
}

nodePT list_from_list(nodePT L) {
  if (!L) {
    return NULL;
  } else {
    nodePT dup = (struct node *)malloc(sizeof(struct node));
    dup->data = L->data;
    dup->next = list_from_list(L->next);
    return dup;
  }
}

void destroy_list(nodePT head) {
  nodePT temp = NULL;

  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
    //printf("Deleted Node\n");
  }
}

nodePT reverse(nodePT head) {
  nodePT prev = NULL;
  nodePT current = head;
  nodePT next = NULL;

  while (current != NULL) {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  return prev;
}

nodePT deleteDuplicates(nodePT head) {
  nodePT current = head;

  while (current != NULL && current->next != NULL) {
    nodePT Next = NULL;

    if (current->data == current->next->data) {
      Next = current->next->next;
      free(current->next);
      current->next = Next;
    } else {
      current = current->next;
    }
  }
  return head;
}

nodePT deleteByValue(nodePT head, int val) {
/*//nodePT current = head;
//nodePT prev = NULL;
  while (head && head->data == val) {
    head = head->next;
    //free(current);
    //return head;
  }*/

  nodePT current = head;
  nodePT prev = NULL;

  while (current) {
    if (current->data == val) {
    //if(current->data != val){
      prev->next = current->next;
      free(current);
    } else {
      prev = current;
    }
    current = current->next;
  }
  return head;
}
