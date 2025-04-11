#include <stdio.h>
#include "general.h"
#include <stdint.h>
//#include <crtdbg.h>
//#define _CRTDBG_MAP_ALLOC
#define SUCCESS 1
#define FAIL 0

typedef enum {FALSE, TRUE} bool_t;

int ParseParameters (int, char**, FILE**);      //parsing arguments of main function
void PrintErrorMessage(error_t);                //printing error message
void ProcessStream (FILE*);                     //reading from stream, making expression string and calling Calculate function
int LenCheck (char*, size_t, char*);            //check whether string length is large enough to get next part of string from stream
error_t StrIncrease (char**, size_t*, char**);  //increasing of string length
void PrintExpression(char const*);              //printing string with input expression
void PrintResult(double);                       //printing " == " and result
void ClearStr (char*, size_t);                  //clearing string with 0
int EndOfStr (char*);                           //check that string is not finished
int IsEmptyOrCommentStr (char*);                //check that string is not a comment string or an empty string
int OmitCurrentStr(FILE* in);
int AllocateMemory(char** pExp, char** pTmp, size_t len, dict_t** pDict, size_t dictLen, FILE*);

int main(int argc, char** argv)
{
  FILE *in = stdin;
  int flag;
  //FILE* in;
  //fopen_s(&in, "in.txt", "r");

  //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

  flag = ParseParameters(argc, argv, &in);
  if (flag == FAIL)
    exit(1);

  ProcessStream(in);

  fclose(in);
  return 0;
}

int ParseParameters (int argc, char** argv, FILE** ptrin)
{
  //if amount of parameters is too large print error message
  if (argc >= 3) 
  {
	  PrintErrorMessage(TOO_MANY_PARAMETERS);
	  return FAIL;
  }
  if (argc == 2)
  {
    //if failed to open file print error message
    //if ( fopen_s(ptrin, argv[1], "r") )
    *ptrin = fopen(argv[1], "r");
    if (*ptrin == NULL)
    {
      PrintErrorMessage(FAILED_TO_OPEN_FILE);
      return FAIL;
    }
  }
  return SUCCESS;
}
void PrintErrorMessage (error_t error)
{
  printf("ERROR: ");
  switch (error)
  {
  case INCORRECT_BRACKETS: printf("brackets\n"); break;
  case INVALID_TOKEN: printf("invalid token\n"); break;
  case INCORRECT_EXPRESSION: printf("incorrect expression\n"); break;
  case DIVISION_BY_ZERO: printf("division by zero\n"); break;
  case NOT_IN_TONERANCE_RANGE: printf("value of argument is not in tolerance range\n"); break;
  case MEMORY_SHORTAGE_GENERAL: printf("not enough memory\n"); break;
  case MEMORY_SHORTAGE_WHILE_CALC: printf("not enough memory (while doing calculations)\n"); break;
  case FAILED_TO_OPEN_FILE: printf("failed to open input file\n"); break;
  case TOO_MANY_PARAMETERS: printf("too many parameters\n"); break;
  case NO_EXPRESSION: printf("no expression to caclculate\n"); break;
  }
}
void ProcessStream (FILE* in)
{
  char *inputStr = NULL, *tmp = NULL;
  size_t len = 10, dictLen = 20; //basic length of expression
  double result; //result of input expression
  error_t error;
  int flagEOF;
  dict_t* dict = NULL;

  /*allocate memory for expression, temporary buffer and dictionary
  if memory wasn't allocated omit current string*/
  flagEOF = AllocateMemory(&inputStr, &tmp, len, &dict, dictLen, in);
  if (flagEOF == EOF)
    return;
  ClearStr(inputStr, 1);

  //read from stream until stream is not finished
  while (fgets(tmp, len, in))
  {
  	//if expression is not large enough to contain tmp increase length of expression (call StrIncrease)
  	if (LenCheck(inputStr, len, tmp))
    {
      error = StrIncrease(&inputStr, &len, &tmp);
      //if StrIncrease failed to increase expression and tmp length omit current string in stream
      if (error)
      {
        //if end of file was reached stop processing stream
        if (!EndOfStr(tmp))
        {
          flagEOF = OmitCurrentStr(in);
          if (flagEOF == EOF)
          {
            free(inputStr);
            free(tmp);
            free(dict);
            return;
          }
        }
        //else print error message
        PrintErrorMessage(error);
        ClearStr(inputStr, 1);
        ClearStr(tmp, 1);
        continue;
      }
    }

    //strcat_s(inputStr, len, tmp);
    strcat(inputStr, tmp);
	  //if string or stream is finished
    if (EndOfStr(tmp) || feof(in))
    {
      //if expression is empty write expression without any changes
      if (IsEmptyOrCommentStr(inputStr))
        fwrite(inputStr, sizeof(char), strlen(inputStr), stdout);
      //else calculate and print result
      else
      {
        char varName;
        /*inputStr - full input string
          exp - current expression (different expressions in string divided by semicolons)*/
        char *exp = inputStr, *pSemicolon;
        size_t expLen;
        int indDict = 0; // indDict - index in dictionary
        bool_t emptyFlag = FALSE;

        do
        {
          varName = 0;
          //find semicolon (';')
          pSemicolon = strchr(exp, ';');
          //set length of expression
          if (pSemicolon != NULL)
            expLen = (uintptr_t)pSemicolon - (uintptr_t)exp;
          else
            expLen = strlen(exp);
          assert(expLen <= strlen(exp));
          //calculate expression result
          //printf("expLen: %i\n", expLen);
          error = Calculate(exp, expLen, &result, dict, indDict, &varName);
          if (error)
            break;
          assert(varName >= 0);
          //if variable was detected (varName != 0) in expression add it in dictionary with it's value (result)
          if (varName != 0)
          {
            dict[indDict].name = varName;
            dict[indDict].value = result;
            //printf("dict[i]: %c %lf\n", dict[indDict].name, dict[indDict].value);
            ++indDict;
          }
          //move pointer to the next expression
          if (expLen < strlen(exp))
            exp += expLen + 1;
          //printf("%c\n", *exp);
          if (IsEmptyOrCommentStr(exp))
          {
            emptyFlag = TRUE;
            break;
          }
          else
            emptyFlag = FALSE;
        } while (pSemicolon != NULL);

        //printf("%c == %lf\n", dict[0].name, dict[0].value);
        PrintExpression(inputStr);
        if (!error)
          if (emptyFlag == TRUE)
            PrintErrorMessage(NO_EXPRESSION);
          else
            PrintResult(result);
        //if emptyFlag == TRUE do not print anything
        else
          PrintErrorMessage(error);
	   }
      //clear inputStr with 0 when it was processed
	  ClearStr(inputStr, len);
    }
  }
  free(tmp);
  free(inputStr);
  free(dict);
  return;
}
int LenCheck (char* str1, size_t len, char* str2)
{
  //if str1 is not large enough to contain str2 return 1
  if (len - strlen(str1) - 1 < strlen(str2))
  	return 1;
  return 0;
}
error_t StrIncrease(char** pStr1, size_t* pLen, char** pStr2)
{
  //increase length of str1
  if (BufferIncrease((void**)pStr1, *pLen, sizeof(char), 2) == FAIL)
    return MEMORY_SHORTAGE_GENERAL;

  //increase length of str2
  if (BufferIncrease((void**)pStr2, *pLen, sizeof(char), 2) == FAIL)
    return MEMORY_SHORTAGE_GENERAL;

  //return SUCCESS if size of both strings was increased successfully
  *pLen *= 2;
  return NO_ERROR;
}
void PrintExpression(char const* expression)
{
  int i, len = strlen(expression);
  
  //print expression without end of string characters
  for (i = 0; i < len; ++i)
    if (expression[i] != '\r' && expression[i] != '\n')
  	  printf("%c", expression[i]);

  printf(" == ");
}
void PrintResult(double res)
{
  printf("%g\n", res);
}
void ClearStr (char* str, size_t len)
{
  unsigned i;
  for (i = 0; i < len; ++i) 
  	str[i] = (char)0;
}
int EndOfStr (char* str)
{
  //if the last character of string is end of string return 1
  if ('\n' == str[strlen(str)-1]) 
  	return 1;
  return 0;
}
int IsEmptyOrCommentStr (char* str)
{
  int i, len = strlen(str);

  //repeat until string is not finished
  for (i = 0; i < len-1; ++i)
  {
  	//if we found comments and before coments were only space characters return 1
  	if ((str[i] == '/') && (str[i+1] == '/')) 
  	  return 1;
  	//if we found nonspace character and string is not finished return 0
  	if (str[i] < 0)
      return 0;
    else
      if (!isspace(str[i]) && str[i] != '\n')
        return 0;
  }
  return 1;
}
int OmitCurrentStr(FILE* in)
{
  char c;

  do
  {
    c = (char)fgetc(in);
  } while (c != '\n' && c != EOF);

  if (c == EOF)
    return EOF;
  return 0;
}
int AllocateMemory(char** pExp, char** pTmp, size_t len, dict_t** pDict, size_t dictLen, FILE* in)
{
  do
  {
    if (BufferIncrease((void**)pExp, len, sizeof(char), 1) == FAIL)
    {
      PrintErrorMessage(MEMORY_SHORTAGE_GENERAL);
      if (EOF == OmitCurrentStr(in))
        return EOF;
    }
  } while (*pExp == NULL);

  do
  {
    if (BufferIncrease((void**)pTmp, len, sizeof(char), 1) == FAIL)
    {
      PrintErrorMessage(MEMORY_SHORTAGE_GENERAL);
      if (OmitCurrentStr(in) == EOF)
      {
        free(*pExp);
        return EOF;
      }
    }
  } while (*pTmp == NULL);

  do
  {
    if (BufferIncrease((void**)pDict, dictLen, sizeof(dict_t), 1) == FAIL)
    {
      PrintErrorMessage(MEMORY_SHORTAGE_GENERAL);
      if (OmitCurrentStr(in) == EOF)
      {
        free(*pExp);
        free(*pTmp);
        return EOF;
      }
    }
  } while (*pDict == NULL);

  return SUCCESS;
}

void* realloc_dbg(void* ptr, size_t size)
{
  if (rand() * 10 / RAND_MAX == 3)
    return NULL;
  return realloc(ptr, size);
}