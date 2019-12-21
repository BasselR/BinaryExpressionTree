#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4

char *makeString(char *s1, char *s2, char *s3) {
  int totalLength = strlen(s1) + strlen(s2) + strlen(s3);
  char *res = malloc((totalLength + 1) * sizeof(char));
  if (res == NULL) {
    return NULL;
  }
  strcat(strcat(strcpy(res, s1), s2), s3);
  return res;
}

Node *createNode(char *s, double val) {
  Node *nPtr;
  nPtr = malloc(sizeof(Node));
  if (nPtr == NULL) {
    return NULL;
  }
  if (s != NULL) {
    nPtr->expr_string = makeString(s, "", "");
  }
  nPtr->left = NULL;
  nPtr->right = NULL;
  nPtr->num_parents = 0;
  nPtr->value = val;
  return nPtr;
}

Node *binop(Operation op, Node *a, Node *b) {
  if (a->num_parents == 1 || b->num_parents == 1) {
    return NULL;
  }
  Node *nPtr = createNode(0, 0);
  nPtr->left = a;
  nPtr->right = b;
  nPtr->operation = op;
  a->num_parents += 1;
  b->num_parents += 1;
  char *tmp1;
  char *tmp2;
  tmp1 = makeString("(", a->expr_string, ")");
  tmp2 = makeString("(", b->expr_string, ")");

  switch (op) {
  case addop:
    nPtr->expr_string = makeString(a->expr_string, "+", b->expr_string);
    break;
  case subop:
    nPtr->expr_string = makeString(a->expr_string, "-", b->expr_string);
    break;
  case mulop:
    nPtr->expr_string = makeString(tmp1, "*", tmp2);
    break;
  case divop:
    nPtr->expr_string = makeString(tmp1, "/", tmp2);
    break;
  }

  free(tmp1);
  free(tmp2);
  return nPtr;
}

double evalTree(Node *root) {
  if (root == NULL) {
    return 0;
  }
  if (root->left == NULL && root->right == NULL) {
    return root->value;
  }
  switch (root->operation) {
  case addop:
    root->value = evalTree(root->left) + evalTree(root->right);
    return root->value;
    break;
  case subop:
    root->value = evalTree(root->left) - evalTree(root->right);
    return root->value;
    break;
  case mulop:
    root->value = evalTree(root->left) * evalTree(root->right);
    return root->value;
    break;
  case divop:
    break;
  }
  root->value = evalTree(root->left) / evalTree(root->right);
  return root->value;
}

void freeTree(Node *root) {
  if (root == NULL) {
    return;
  }
  free(root->expr_string);
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

Node *duplicateHelper(Node *root, int numParents) {
  if (root == NULL) {
    return NULL;
  }
  Node *dupeNode = createNode(root->expr_string, root->value);
  dupeNode->operation = root->operation;
  dupeNode->num_parents = numParents;
  dupeNode->left = duplicateHelper(root->left, 1);
  dupeNode->right = duplicateHelper(root->right, 1);
  return dupeNode;
}

Node *duplicateTree(Node *root) {
  if (root == NULL) {
    return NULL;
  }
  return duplicateHelper(root, 0);
}

void printTree(Node *root) {
  if (root == NULL) {
    return;
  }
  printf(
      " Node\n   expr_string = %s\n   value       = %g\n   num_parents = %d\n",
      root->expr_string, root->value, root->num_parents);
  printTree(root->left);
  printTree(root->right);
}

int main() {
  Node *a[4];
  a[0] = createNode("a", .1);
  a[1] = createNode("b", .2);
  a[2] = createNode("c", .3);
  a[3] = createNode("d", .4);
  Node *t1 = binop(mulop, a[0], a[1]); /* a* b */
  Node *t2 = binop(addop, a[2], t1);   /* c + a* b */
  Node *t3 = binop(mulop, a[3], t2);   /* d *( c + a * b ) */
  Node *t4 = duplicateTree(t3);        /* d *( c + a * b ) */
  Node *t5 = binop(subop, t4, t3);
  Node *f = t5;
  printf("--- Tree at t4\n");
  printTree(t4);
  double val = evalTree(f);
  printf("\n--- Value at root of f %g\n", val);
  printf("--- Evaluated Tree at f\n");
  printTree(f);
  freeTree(f);
  return 0;
}