// Метод Рутисхаузера

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 20

int n[MAX_BUFFER];
int s[MAX_BUFFER];
int oper[MAX_BUFFER];

void printc(int *arr, int *arr2, int length, const char *fmt, int pad);
void copy(int *, int *, int k);
int operation(int a, int b, int op);

int main(int argc, char **argv) {
  argv[argc - 1] = argv[argc - 1];
  char c;
  int tokenCounter = 0;
  int maxDepth = 0;
  int maxOperandLength = 0;
  int operand = 0;
  int operandLength = 0;
  int inOperand = 0;
  n[tokenCounter] = 0;
  s[tokenCounter] = ' ';
  while ((c = (char)getchar()) != '\n') {
    if (!inOperand && c != ' ')
      tokenCounter++;
    switch (c) {
    case ' ':
      continue;
      break;
    case '+':
    case '-':
    case '/':
    case '*':
    case ')':
      if (inOperand) {
        inOperand = 0;
        n[tokenCounter] = n[tokenCounter - 1] + 1;
        if (n[tokenCounter] > maxDepth)
          maxDepth = n[tokenCounter];
        oper[tokenCounter] = operand;
        s[tokenCounter] = '\0';
        operand = 0;
        if (operandLength > maxOperandLength)
          maxOperandLength = operandLength;
        operandLength = 0;
        tokenCounter++;
      }
      n[tokenCounter] = n[tokenCounter - 1] - 1;
      s[tokenCounter] = c;
      break;
    case '(':
    default:
      if ('0' <= c && c <= '9') {
        inOperand = 1;
        operandLength++;
        operand = operand * 10 + c - '0';
      } else {
        n[tokenCounter] = n[tokenCounter - 1] + 1;
        s[tokenCounter] = c;
      }
      break;
    }
  }
  n[++tokenCounter] = 0;
  s[tokenCounter] = ' ';

  printc(s, oper, tokenCounter, "%*c", maxOperandLength);
  printc(n, NULL, tokenCounter, "%*d", maxOperandLength);
  printf("\n");

  const int shift = 4;
  while (maxDepth > 0) {
    int maxFound = 0;
    int i = 0;
    while (i <= tokenCounter) {
      if (maxFound == 1) {
        n[i - shift] = n[i];
        s[i - shift] = s[i];
        oper[i - shift] = oper[i];
      }
      if (n[i + 1] == maxDepth) {
        assert(n[i] == maxDepth - 1);
        assert(n[i + 1] == maxDepth);
        assert(n[i + 2] == maxDepth - 1);
        assert(n[i + 3] == maxDepth);
        assert(n[i + 4] == maxDepth - 1);
        oper[i] = operation(oper[i + 1], oper[i + 3], s[i + 2]);
        s[i] = 0;
        maxFound = 1;
        i += shift;
      }
      i++;
    }
    if (maxFound == 0)
      maxDepth--;
    else {
      tokenCounter -= shift;

      printc(s, oper, tokenCounter, "%*c", maxOperandLength);
      printc(n, NULL, tokenCounter, "%*d", maxOperandLength);
      printf("\n");
    }
  }
  return 0;
}

void printc(int *arr, int *arr2, int k, const char *fmt, int pad) {
  for (int i = 0; i <= k; i++) {
    if (arr[i] == 0 && arr2 != NULL)
      printf("%*d", pad + 2, arr2[i]);
    else
      printf(fmt, pad + 2, arr[i]);
  }
  printf("\n");
}

void copy(int *from, int *to, int k) {
  for (int i = 0; i <= k; i++)
    to[i] = from[i];
}

int operation(int a, int b, int op) {
  switch (op) {
  case '+':
    return a + b;
    break;
  case '-':
    return a - b;
    break;
  case '*':
    return a * b;
    break;
  case '/':
    return a / b;
    break;
  default:
    abort();
    break;
  }
  return 0;
}
