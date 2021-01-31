#include "infix.h"
#include <stdbool.h>

bool is_operand(char * ch);
bool is_left_bracket(char * ch);
bool is_operator(char * ch);
bool operator_of_ge_precedence(struct double_stack * this, char * ch);
bool is_right_bracket(char * ch);

char ** args;

// evaluate expression stored as an array of string tokens
double evaluate_infix_expression(char ** argss, int nargs) {
  args = argss;
  char ** postfix = malloc(sizeof(char *) * nargs);
  int postfix_length = 0;
  struct double_stack * stack = double_stack_new(nargs);
  for (int i = 0; i < nargs; i++) {
    if (is_operand(args[i])) {
      postfix[postfix_length] = args[i];
      postfix_length++;
    } else if (is_left_bracket(args[i])) {
      double_stack_push(stack, i);
    } else if (is_operator(args[i])) {
      while (operator_of_ge_precedence(stack, args[i])) {
        int op = double_stack_pop(stack);
        postfix[postfix_length] = args[op];
        postfix_length++;
      }
      double_stack_push(stack, i);
    } else if (is_right_bracket(args[i])) {
      while (!is_left_bracket(args[(int)stack->items[stack->top - 1]])) {
        int op = double_stack_pop(stack);
        postfix[postfix_length] = args[op];
        postfix_length++;
      }
      int op = double_stack_pop(stack);
      assert(is_left_bracket(args[op]));
    } else {
      assert(false);
    }
  }
  while (stack->top > 0) {
    int op = double_stack_pop(stack);
    postfix[postfix_length] = args[op];
    postfix_length++;
  }
  return evaluate_postfix_expression(postfix, postfix_length);
}

bool is_operand(char * ch) {
  return (atof(ch) != 0);
}

bool is_left_bracket(char * ch) {
  return (strcmp(ch, "(") == 0);
}

bool is_operator(char * ch) {
  return (strcmp(ch, "+") == 0 || 
          strcmp(ch, "-") == 0 || 
          strcmp(ch, "X") == 0 || 
          strcmp(ch, "/") == 0 || 
          strcmp(ch, "^") == 0);
}

bool operator_of_ge_precedence(struct double_stack * this, char * ch) {
  return (this->top > 0 &&
          ((strcmp(args[(int)this->items[this->top - 1]], "^") == 0) ||
          (strcmp(args[(int)this->items[this->top - 1]], "X") == 0 && (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || strcmp(ch, "X") == 0 || strcmp(ch, "/") == 0)) ||
          (strcmp(args[(int)this->items[this->top - 1]], "/") == 0 && (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || strcmp(ch, "X") == 0 || strcmp(ch, "/") == 0)) ||
          (strcmp(args[(int)this->items[this->top - 1]], "+") == 0 && (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0)) ||
          (strcmp(args[(int)this->items[this->top - 1]], "-") == 0 && (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0))));
}

bool is_right_bracket(char * ch) {
  return (strcmp(ch, ")") == 0);
}
