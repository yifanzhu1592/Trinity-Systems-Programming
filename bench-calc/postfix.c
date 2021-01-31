#include "postfix.h"

// evaluate expression stored as an array of string tokens
double evaluate_postfix_expression(char ** args, int nargs) {
  struct double_stack * stack = double_stack_new(nargs);
  
  for (int i = 0; i < nargs; i++) {
    if ((args[i][0] >= '0' &&
         args[i][0] <= '9') ||
        (args[i][1] >= '0' &&
         args[i][1] <= '9')) {
      double operand = atof(args[i]);
      double_stack_push(stack, operand);
    } else if (strlen(args[i]) == 1) {
    	double op1, op2, result;
		  op2 = double_stack_pop(stack);
		  op1 = double_stack_pop(stack);
      switch (args[i][0]) {
		    case '+':
		      result = op1 + op2;   
		      break;
		    case '-':
		      result = op1 - op2;
		      break;
		    case 'X':
		      result = op1 * op2;
		      break;
		    case '/':
		      result = op1 / op2;
		      break;
		    case '^':
		      result = pow(op1, op2);
		      break;
      }
      double_stack_push(stack, result);
    }
  }
  
  return double_stack_pop(stack);
}
