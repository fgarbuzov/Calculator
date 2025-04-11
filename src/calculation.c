#include "general.h"
#include "stack.h"
#include <math.h>
//#include <stdio.h>

typedef enum
{
  P_NO_OPERATION,
  P_ADDITION,
  P_MULTIPLICATION,
  P_UNARY_MINUS,
  P_POWER,
  P_FUNCTION
} priority_t;
typedef enum
{
  T_NO_OPERATION,
  T_OPERAND,
  T_COMMA,
  T_OPERATION,
  T_FUNCTION,
  T_LEFT_BRACKET,
  T_RIGHT_BRACKET
} tokentype_t;
typedef enum
{
  NO_OPERATION,
  ADDITION,
  SUBSTRACTION,
  UNARY_MINUS,
  MULTIPLICATION,
  DIVISION,
  POWER,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  SINUS,
  COSINUS,
  TANGENT,
  COTANGENT,
  ARCSINUS,
  ARCCOSINUS,
  ARCTANGENT,
  NAT_LOGARITHM,
  LOGARITHM,
  FLOOR,
  CEIL,
  SQRT
} operation_t;

static double pi = 3.14159265358979323846, e = 2.71828182845904523536;

int IsNumber(char c)
{
  if ('0' <= c && c <= '9')
    return 1;
  return 0;
}
int IsArithmeticOperation(char c)
{
  if (strchr("+-*/^", c))
    return 1;
  return 0;
}
int IsLetter(char c)
{
  if ('a' <= c && c <= 'z')
    return 1;
  if ('A' <= c && c <= 'Z')
    return 1;
  return 0;
}
int IsSinus(char const* str)
{
  if (str[0] == 's' && str[1] == 'i' && str[2] == 'n')
    return 1;
  return 0;
}
int IsCosinus(char const* str)
{
  if (str[0] == 'c' && str[1] == 'o' && str[2] == 's')
    return 1;
  return 0;
}
int IsTangent(char const* str)
{
  if (str[0] == 't' && str[1] == 'g')
    return 1;
  return 0;
}
int IsCotangent(char const* str)
{
  if (str[0] == 'c' && str[1] == 't' && str[2] == 'g')
    return 1;
  return 0;
}
int IsCeil(char const* str)
{
  if (str[0] == 'c' && str[1] == 'e' && str[2] == 'i' && str[3] == 'l')
    return 1;
  return 0;
}
int IsFloor(char const* str)
{
  if (str[0] == 'f' && str[1] == 'l' && str[2] == 'o' && str[3] == 'o' && str[4] == 'r')
    return 1;
  return 0;
}
int IsArc(char const* str)
{
  if (str[0] == 'a' && str[1] == 'r' && str[2] == 'c')
    return 1;
  return 0;
}
int IsNatLogarithm(char const* str)
{
  if (str[0] == 'l' && str[1] == 'n')
    return 1;
  return 0;
}
int IsLogarithm(char const* str)
{
  if (str[0] == 'l' && str[1] == 'o' && str[2] == 'g')
    return 1;
  return 0;
}
int IsSqrt(char const* str)
{
  if (str[0] == 's' && str[1] == 'q' && str[2] == 'r' && str[3] == 't')
    return 1;
  return 0;
}
int IsBracket(char character)
{
  if (character == '(' || character == ')')
    return 1;
  return 0;
}
int IsUnaryMinus(char character, tokentype_t prevTokenType)
{
  if (character == '-' && (prevTokenType == T_OPERATION || prevTokenType == T_LEFT_BRACKET || \
      prevTokenType == T_COMMA || prevTokenType == T_NO_OPERATION))
    return 1;
  return 0;
}
int IsBinaryOperation(operation_t operation)
{
  if (operation == ADDITION || operation == SUBSTRACTION || operation == MULTIPLICATION || \
      operation == DIVISION || operation == POWER)
    return 1;
  return 0;
}
int IsPi(char const* str)
{
  if (str[0] == 'p' && str[1] == 'i' && !IsLetter(str[2]))
    return 1;
  return 0;
}
int IsE(char const* str)
{
  if (str[0] == 'e' && !IsLetter(str[1]))
    return 1;
  return 0;
}
int IsInDict(char const c, dict_t* dict, int dictLen, int* pIndDict)
{
  int i;
  for (i = 0; i < dictLen; ++i)
    if (dict[i].name == c)
    {
      *pIndDict = i;
      return 1;
    }
  return 0;
}

int CheckCongruence(double x, double a)
{
  double d, eps = 0.00000001;
  if (x >= 0)
  {
    a += 2 * pi * floor(x / (2*pi));
    while (fabs(x-a) > eps)
    {
      d = x - a;
      a += 2 * pi;
      if (fabs(x-a) > fabs(d))
        return 0;
    }
  }
  if (x < 0)
  {
    a -= 2 * pi * ceil(x / (2*pi));
    while (fabs(x-a) > eps)
    {
      d = x - a;
      a -= 2 * pi;
      if (fabs(x-a) > fabs(d))
        return 0;
    }
  }
  return 1;
}
int CheckTangentToleranceRange(double x)
{
  if (CheckCongruence(x, pi/2) || CheckCongruence(x, -pi/2))
    return 0;
  return 1;
}
int CheckCotangentToleranceRange(double x)
{
  if (CheckCongruence(x, pi) || CheckCongruence(x, 0))
    return 0;
  return 1;
}
int CheckArcsinusToleranceRange(double x)
{
  if (-1 <= x && x <= 1)
    return 1;
  return 0;
}
int CheckNatLogarithmToleranceRange(double x)
{
  if (x > 0)
    return 1;
  return 0;
}
int CheckLogarithmToleranceRange(double a, double b)
{
  if (a > 0 && a != 1 && b > 0)
    return 1;
  return 0;
}
int CheckSqrtToleranceRange(double x)
{
  if (x >= 0)
    return 1;
  return 0;
}

void MoveIndex(int* pIndex, operation_t function)
{
  if (SINUS <= function && function <= COSINUS || function == COTANGENT || function == LOGARITHM)
    *pIndex += 2;
  if (ARCSINUS <= function && function <= ARCCOSINUS)
    *pIndex += 5;
  if (NAT_LOGARITHM == function || TANGENT == function)
    *pIndex += 1;
  if (FLOOR == function || ARCTANGENT == function)
    *pIndex += 4;
  if (CEIL == function || SQRT == function)
    *pIndex += 3;
}
void SearchForEquationSign(const char** ppEquationSign, const char* str, int len)
{
  int i;
  *ppEquationSign = NULL;
  for (i = 1; i < len; ++i)
  {
    if (str[i] < 0)
    {
      *ppEquationSign = NULL;
      break;
    }
    if (str[i] == '=')
    {
      *ppEquationSign = str + i;
      break;
    }
    if (!isspace(str[i]))
    {
      *ppEquationSign = NULL;
      break;
    }
  }
}

operation_t DefineArithmeticOperation(char c, tokentype_t prevTokenType)
{
  switch (c)
  {
  case '+': 
    return ADDITION;
  case '-': 
    if (IsUnaryMinus('-', prevTokenType))
      return UNARY_MINUS;  
    return SUBSTRACTION;
  case '*': 
    return MULTIPLICATION;
  case '/': 
    return DIVISION;
  case '^': 
    return POWER;
  default: 
    return NO_OPERATION;
  }
}
operation_t DefineFunction(char const* str)
{
  switch (*str)
  {
  case 's':
    if (IsSinus(str))
      return SINUS;
    if (IsSqrt(str))
      return SQRT;
  case 'c':
    if (IsCosinus(str))
      return COSINUS;
    if (IsCotangent(str))
      return COTANGENT;
    if (IsCeil(str))
      return CEIL;
  case 't':
    if (IsTangent(str))
      return TANGENT;
  case 'a':
    if (IsArc(str))
    {
      if (IsSinus(str+3))
        return ARCSINUS;
      if (IsCosinus(str+3))
        return ARCCOSINUS;
      if (IsTangent(str+3))
        return ARCTANGENT;
    }
  case 'l':
    if (IsNatLogarithm(str))
      return NAT_LOGARITHM;
    if (IsLogarithm(str))
      return LOGARITHM;
  case 'f':
    if (IsFloor(str))
      return FLOOR;
  default: 
    return NO_OPERATION;
  }
}
operation_t DefineBracket(char c)
{
  if (c == '(')
    return LEFT_BRACKET;
  if (c == ')')
    return RIGHT_BRACKET;
  return NO_OPERATION;
}

priority_t SetPriority(operation_t operation)
{
  switch (operation)
  {
  case ADDITION:
  case SUBSTRACTION: return P_ADDITION;
  case MULTIPLICATION:
  case DIVISION: return P_MULTIPLICATION;
  case UNARY_MINUS: return P_UNARY_MINUS;
  case POWER: return P_POWER;
  }
  if (operation >= SINUS)
    return P_FUNCTION;
  return P_NO_OPERATION;
}

double AmountOfPiInNumber(double x)
{
  double d;
  if (!CheckTangentToleranceRange(x) || !CheckCotangentToleranceRange(x))
  {
    d = x / pi * 2;
    if (fabs(d - floor(d)) < fabs(d - ceil(d)))
      d = floor(d);
    else
      d = ceil(d);
    return d/2;
  }
  return 0;
}

error_t CalculateResult(operation_t operation, double a, double b, double* pResult)
{
  switch (operation)
  {
  case ADDITION: 
    assert(IsBinaryOperation(operation));
    *pResult = a + b;
    break;
  case SUBSTRACTION: 
    assert(IsBinaryOperation(operation));
    *pResult = a - b;
    break;
  case UNARY_MINUS:
    *pResult = -b;
    break;
  case MULTIPLICATION:
    assert(IsBinaryOperation(operation));
    *pResult = a * b; 
    break;
  case DIVISION:
    assert(IsBinaryOperation(operation));
    if (b != 0.0)
    {
      *pResult = a / b; 
      break;
    }
    else
      return DIVISION_BY_ZERO;
  case POWER:
    assert(IsBinaryOperation(operation));
    *pResult = pow(a,b);
    break;
  case RIGHT_BRACKET:
  case LEFT_BRACKET:
    return INCORRECT_BRACKETS;
  case SINUS:
    *pResult = sin(b);
    break;
  case COSINUS:
    *pResult = cos(b);
    break;
  case TANGENT:
    if (CheckTangentToleranceRange(b))
    {
      *pResult = tan(b);
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  case COTANGENT:
    if (CheckCotangentToleranceRange(b))
    {
      if (CheckTangentToleranceRange(b))
        *pResult = 1 / tan(b);
      else
        *pResult = 0;
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  case ARCSINUS:
    if (CheckArcsinusToleranceRange(b))
    {
      *pResult = asin(b);
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  case ARCCOSINUS:
    if (CheckArcsinusToleranceRange(b))
    {
      *pResult = acos(b);
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  case ARCTANGENT:
    *pResult = atan(b);
    break;
  case NAT_LOGARITHM:
    if (CheckNatLogarithmToleranceRange(b))
    {
      *pResult = log(b);
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  case LOGARITHM:
    if (CheckLogarithmToleranceRange(a, b))
    {
      *pResult = log(b) / log(a);
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  case FLOOR:
    *pResult = floor(b);
    break;
  case CEIL:
    *pResult = ceil(b);
    break;
  case SQRT:
    if (CheckSqrtToleranceRange(b))
    {
      *pResult = sqrt(b);
      break;
    }
    else
      return NOT_IN_TONERANCE_RANGE;
  default:
    return INCORRECT_EXPRESSION;
  }

  return NO_ERROR;
}
error_t DoOldAtithmeticOperation(stack_t** pStNum, stack_t** pStOper)
{
  double a = 0, b, result;
  operation_t operation;
  error_t error;

  //get operation from stack of operations
  assert(!IsEmpty(*pStOper));
  operation = *(operation_t*)GetTop(*pStOper);
  pop(pStOper);

  //get an operand from stack of numbers
  if (!IsEmpty(*pStNum))
    b = *(double*)GetTop(*pStNum);
  else
    return INCORRECT_EXPRESSION;
  pop(pStNum);

  //if operation is binary get another operand from stack of numbers
  if (IsBinaryOperation(operation) || operation == LOGARITHM)
  {
    if (!IsEmpty(*pStNum))
      a = *(double*)GetTop(*pStNum);
    else
      return INCORRECT_EXPRESSION;
    pop(pStNum);
  }

  //calculate result
  error = CalculateResult(operation, a, b, &result);
  if (error)
      return error;

  //push result on stack
  error = push(pStNum, (void*)&result, sizeof(double));
  if (error)
      return error;
  return NO_ERROR;
}
error_t DoOldOperationsWithHigherPriority(priority_t priorityCurrent, priority_t priorityOld, stack_t** pStNum, stack_t** pStOper)
{
  error_t error;

  //NOTE: priorityCurrent can be equal to P_NO_OPERATION (lowest priority) if operation in stack is left bracket
  while (priorityCurrent <= priorityOld)
  {
    //unary minus, functions and power has right-to-left associativity therefore we don't need to push it out from stack
    if ((priorityOld == priorityCurrent) && (priorityCurrent == P_UNARY_MINUS || priorityCurrent == P_FUNCTION || priorityCurrent == P_POWER))
      break;

    //unary minus doesn't push out power to accept situation a^-b 
    if (priorityCurrent == P_UNARY_MINUS && priorityOld == P_POWER)
      break;

    //make operation in stack (old operation)
    error = DoOldAtithmeticOperation(pStNum, pStOper);
    if (error)
      return error;

    //set priority of the next operation in stack
    if (!IsEmpty(*pStOper))
      priorityOld = SetPriority(*((operation_t*)GetTop(*pStOper)));
    else
      priorityOld = P_NO_OPERATION;
  }
  return NO_ERROR;
}

error_t ProcessNumberToken(char const* expression, int* pCurrentIndex, stack_t** pStNum, tokentype_t prevTokenType)
{
  error_t error;
  char* pTokenEnd;
  //define number (function "strtod")
  double numToken = strtod(expression + *pCurrentIndex, &pTokenEnd);

  //if number wasn't defined by function "strtod" return error
  if (pTokenEnd == expression + *pCurrentIndex)
    return INVALID_TOKEN;

  //protection from situation <func><number> and <rightBracket><number>
  if (prevTokenType == T_FUNCTION || prevTokenType == T_RIGHT_BRACKET)
    return INCORRECT_EXPRESSION;

  //push defined number on stack
  error = push(pStNum, (void*)&numToken, sizeof(double));
  if (error)
    return error;

  //move index to the end of defined number
  *pCurrentIndex += (int)(pTokenEnd - expression) - *pCurrentIndex - 1;

  return NO_ERROR;
}

error_t ProcessOperationToken(operation_t operation, stack_t** pStNum, stack_t** pStOper, tokentype_t prevTokenType)
{
  /*priorityCurrent - priority of current operation 
  priorityOld - priority of operation in stack*/
  priority_t priorityCurrent, priorityOld;
  error_t error;

  //protection from situation <func><func>
  if (prevTokenType == T_FUNCTION)
    return INCORRECT_EXPRESSION;

  //protection from situation <func><bin.oper.> or ...<left bracket><bin.oper.>...
  if (IsBinaryOperation(operation) && (prevTokenType == T_FUNCTION || prevTokenType == T_LEFT_BRACKET || prevTokenType == T_COMMA))
  {
    //printf("exit: bin.oper.\n");
    return INCORRECT_EXPRESSION;
  }

  //set priority of operation in stack (old operation)
  if (!IsEmpty(*pStOper))
    priorityOld = SetPriority(*((operation_t*)GetTop(*pStOper)));
  else
    priorityOld = P_NO_OPERATION;
  //set priority of current operation
  priorityCurrent = SetPriority(operation);
  
  /*operation with lower priority pushes out operations with higher priority from stack
  therefore we have to do all operations in stack with higher priority than current operation*/
  error = DoOldOperationsWithHigherPriority(priorityCurrent, priorityOld, pStNum, pStOper);
  if (error)
  {
    //printf("exit: do old oper.\n");
    return error;
  }

  //push current operation on stack
  error = push(pStOper, (void*)&operation, sizeof(operation_t));
  if (error)
    return error;

  return NO_ERROR;
}

error_t ProcessBracketToken(operation_t bracket, stack_t** pStNum, stack_t** pStOper, tokentype_t* pPrevTokenType)
{
  error_t error;

  //if current operation is left bracket
  if (bracket == LEFT_BRACKET)
  {
    //protection from situation ...)(... and <operand><leftBracket>
    if (*pPrevTokenType == T_RIGHT_BRACKET || *pPrevTokenType == T_OPERAND)
      return INCORRECT_EXPRESSION;
    //push bracket on stack
    error = push(pStOper, (void*)&bracket, sizeof(operation_t));
    *pPrevTokenType = T_LEFT_BRACKET;
    return error;
  }

  /*if current operation is right bracket 
  make all operations which are above left bracket in stack opetations and remove right bracket*/
  while (!IsEmpty(*pStOper) && *(operation_t*)GetTop(*pStOper) != LEFT_BRACKET)
  {
    error = DoOldAtithmeticOperation(pStNum, pStOper);
    if (error)
      return error;
  }

  //return error if there was a right bracket but no left bracket
  if (IsEmpty(*pStOper))
    return INCORRECT_BRACKETS;

  //pop left bracket corresponding to right bracket which was processed
  assert(*(operation_t*)GetTop(*pStOper) == LEFT_BRACKET);
  pop(pStOper);
  if (!IsEmpty(*pStOper))
    if(*(operation_t*)GetTop(*pStOper) == LOGARITHM)
    {
      error = DoOldAtithmeticOperation(pStNum, pStOper);
      if (error)
        return error;
    }
  *pPrevTokenType = T_RIGHT_BRACKET;
  return NO_ERROR;
}

error_t ProcessVarToken(char const* expression, int* pCurrentIndex, int len, dict_t* dict, int indDict, stack_t** pStNum, char* pVarName)
{
  int indOfVarInDict;
  const char* pEquationSign;
  error_t error;

  // if current letter is in dictionary push it's value on stack
  if (IsInDict(expression[*pCurrentIndex], dict, indDict, &indOfVarInDict))
  {
    double value;
    assert(0 <= indOfVarInDict && indOfVarInDict < indDict);
    value = dict[indOfVarInDict].value;
    //printf("is in dict %lf\n", value);
    error = push(pStNum, (void*)&value, sizeof(double));
    return error;
  }

  /* if current letter is not in dictionary set varName with this letter, 
  find equation sign, set *pCurrentIndex to this sign and continue to calculate*/
  *pVarName = expression[*pCurrentIndex];
  SearchForEquationSign(&pEquationSign, &expression[*pCurrentIndex], len);
  if (pEquationSign == NULL)
    return INCORRECT_EXPRESSION;
  assert(pEquationSign - expression < strlen(expression));
  //printf("equation sign index: %i\n", (int)pEquationSign - (int)expression);
  *pCurrentIndex = pEquationSign - expression;

  return NO_ERROR;
}

error_t ProcessCommaToken(stack_t** pStNum, stack_t** pStOper, tokentype_t* pPrevTokenType)
{
  error_t error;

  if(*pPrevTokenType != T_OPERAND && *pPrevTokenType != T_RIGHT_BRACKET)
    return INCORRECT_EXPRESSION;

  while (!IsEmpty(*pStOper) && *(operation_t*)GetTop(*pStOper) != LEFT_BRACKET)
  {
    error = DoOldAtithmeticOperation(pStNum, pStOper);
    if (error)
      return error;
  }

  if (IsEmpty(*pStOper))
    return INCORRECT_EXPRESSION;

  *pPrevTokenType = T_COMMA;
  return NO_ERROR;
}

error_t ProcessChar(char const* expression, int* pCurrentIndex, int len, stack_t** pStNum, stack_t** pStOper, \
                    tokentype_t* pPrevTokenType, dict_t* dict, int indDict, char* pVarName)
{
  error_t error;

  //protection from nonlatin (cyrillic) characters
  if (expression[*pCurrentIndex] < 0)
    return INVALID_TOKEN;

  if (IsNumber(expression[*pCurrentIndex]))
  {
    //process number
    error = ProcessNumberToken(expression, pCurrentIndex, pStNum, *pPrevTokenType);
    assert(*pCurrentIndex < (int)strlen(expression));
    //set previuos token type
    *pPrevTokenType = T_OPERAND;
    return error;
  }

  //omit space characters 
  if (isspace((int)expression[*pCurrentIndex]))
    return NO_ERROR;

  if (IsArithmeticOperation(expression[*pCurrentIndex]))
  {
    // define which operation it is, process defined operation and set previous token type as operand
    operation_t operation = DefineArithmeticOperation(expression[*pCurrentIndex], *pPrevTokenType);
    assert(operation != NO_OPERATION);
    error = ProcessOperationToken(operation, pStNum, pStOper, *pPrevTokenType);
    *pPrevTokenType = T_OPERATION;
    return error;
  }

  if (IsLetter(expression[*pCurrentIndex]))
  {
    operation_t function;
    // check is it pi or e
    if (IsPi(&expression[*pCurrentIndex]))
    {
      error = push(pStNum, (void*)&pi, sizeof(double));
      *pCurrentIndex += 1;
      *pPrevTokenType = T_OPERAND;
      return error;
    }
    if (IsE(&expression[*pCurrentIndex]))
    {
      error = push(pStNum, (void*)&e, sizeof(double));
      *pPrevTokenType = T_OPERAND;
      return error;
    }
    // if it's not pi or e define which function it is
    function = DefineFunction(&expression[*pCurrentIndex]);
    //printf("function: %i (17 - log)\n", function);
    // if no function defined process letter token (variable token)
    if (function == NO_OPERATION)
    {
      error = ProcessVarToken(expression, pCurrentIndex, len, dict, indDict, pStNum, pVarName);
      *pPrevTokenType = T_OPERAND;
      return error;
    }
    /*if function was detected move current index to the end of the function token,
    process defined fuction and set previous token type as function*/
    MoveIndex(pCurrentIndex, function);
    error = ProcessOperationToken(function, pStNum, pStOper, *pPrevTokenType);
    *pPrevTokenType = T_FUNCTION;
    return error;
  }

  if (expression[*pCurrentIndex] == ',')
  {
    error = ProcessCommaToken(pStNum, pStOper, pPrevTokenType);
    return error;
  }

  if (IsBracket(expression[*pCurrentIndex]))
  {
    //define which bracket it is left or right
    operation_t bracket = DefineBracket(expression[*pCurrentIndex]);
    assert(bracket != NO_OPERATION);
    //process defined bracket
    error = ProcessBracketToken(bracket, pStNum, pStOper, pPrevTokenType);
    return error;
    //NOTE: previuos token type is set in function ProcessBracketToken
  }

  //if character isn't space or number or arithmetic operation or letter or bracket return error
  return INVALID_TOKEN;
}

error_t Calculate (char const* expression, int len, double* pResult, dict_t* dict, int indDict, char* pVarName)
{
  int indCurrent;
  tokentype_t prevTokenType = T_NO_OPERATION;
  stack_t *stNum = NULL, *stOper = NULL;
  error_t error;
  
  //process every character in expression
  for (indCurrent = 0; indCurrent < len; ++indCurrent)
  {
    error = ProcessChar(expression, &indCurrent, len, &stNum, &stOper, &prevTokenType, dict, indDict, pVarName);
    if (error)
    {
      clear(&stNum);
      clear(&stOper);
      return error;
    }
    assert(indCurrent <= len);
  }

  //if stack of numbers is empty return error
  if (IsEmpty(stNum))
  {
    clear(&stNum);
    clear(&stOper);
    return INCORRECT_EXPRESSION;
  }

  //do last operations if they weren't done
  while (!IsEmpty(stOper))
  {
    error = DoOldAtithmeticOperation(&stNum, &stOper);
    if (error)
    {
      clear(&stNum);
      clear(&stOper);
      return error;
    }
  }
  
  //last number in stNum is result
  assert(!IsEmpty(stNum));
  *pResult = *(double*)GetTop(stNum);

  /*pop top of stack and check is it empty or not;
  not empty stack means that there were two nubers in stack after doing all operations
  therefore return error*/
  pop(&stNum);
  if (!IsEmpty(stNum))
  {
    clear(&stNum);
    clear(&stOper);
    return INCORRECT_EXPRESSION;
  }

  //if everything is OK
  clear(&stNum);
  clear(&stOper);
  return NO_ERROR;
}