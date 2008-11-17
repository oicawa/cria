#ifndef PRIVATE_CRIA_H_INCLUDED
#define PRIVATE_CRIA_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>



#include "Cria.h"
//#include "../Core/Core.h"



struct StringTag {
    char*       pointer;
};



struct ItemTag {
    void    *object;
    Item    next;
};



struct ListTag {
    Item        item;
    Item*       last;
    int         count;
};



struct InterpreterTag
{
    List    statementList;
    List    variableList;       //VariableDefinition
    List    functionList;       //FunctionDefinition
    //List    classList;        //ClassDefinition
    //File    file;
    String  buffer;
    int     lineNumber;
    int     column;
    int     indentLevel;
    int     error;
};



//==================================================
//Tokenizer
//==================================================
struct TokenTag
{
    TokenType   type;
    int         row;
    int         column;
    String      buffer;
};



struct TokenizerTag
{
    FILE            *file;
    int             row;
    int             column;
    char            next;
    String          error;
    int             indentLevel;
};



//==================================================
//Parser
//==================================================
struct ParserTag
{
    List    tokens;
    Item    current;
    Item    next;
    Item    mark;
};




//Statement構造体
struct StatementTag
{
    StatementKind   kind;
    int             line;
    union {
        ReturnStatement         _return_;
        CatchStatement          _catch_;
        FinallyStatement        _finally_;
        IfStatement             _if_;
        WhileStatement          _while_;
        ForStatement            _for_;
        FunctionCallStatement   _functionCall_;
        SubstituteStatement     _substitute_;
    } of;
};



struct ReturnStatementTag
{
    Expression  expression;
};



struct CatchStatementTag
{
    List    statements;
};



struct FinallyStatementTag
{
    List    statements;
};



struct IfStatementTag
{
    Expression  condition;
    List        statements;
    IfStatement _if_;
};



struct WhileStatementTag
{
    Expression  condition;
    List        statements;
};



struct VariableDefinitionTag
{
    String      name;
    CriaId      object;
};



//*
struct FunctionDefinitionTag
{
    char*                       name;
    Boolean                     isNative;
    AccessLevel                 access;
    union {
        struct {
            List                parameterList;
            List                statementList;
        } cria;
        struct {
            CriaNativeFunction* function;
        } native;
    } of;
    CriaObject                  returnValue;
};
//*/



struct ClassDefinitionTag
{
    char*   name;
    List    baseList;
    List    fieldList;
    List    methodList;
};



struct ExpressionTag
{
    ExpressionKind  kind;
    union {
        StringLiteralExpression     _stringLiteral_;
        IntegerLiteralExpression    _integerLiteral_;
        //RealLiteralExpression       _realLiteral_;
        BooleanLiteralExpression    _booleanLiteral_;
        //NullLiteralExpression       _nullLiteral_;
        OperationExpression         _operation_;
        //GenerateExpression          _generate_;
        FunctionCallExpression      _functionCall_;
        VariableExpression          _variable_;
        ReferenceExpression         _reference_;
    } of;
};



struct OperationExpressionTag
{
    OperationKind   kind;
    Expression      left;
    Expression      right;
};



struct SubstituteStatementTag
{
    Reference   reference;
    Expression  expression;
};



struct FunctionCallStatementTag
{
    ReferenceExpression  expression;
};



struct FunctionCallExpressionTag
{
    String                  name;
    ParametersExpression    parameters;
};



struct GenerateExpressionTag
{
    String                  name;
    ParametersExpression    parameters;
};



struct ParametersExpressionTag
{
    List    list;   //Expression
};



struct StringLiteralExpressionTag
{
    String  value;
};



struct IntegerLiteralExpressionTag
{
    int     value;
};



struct BooleanLiteralExpressionTag
{
    Boolean value;
};



struct VariableExpressionTag
{
    String  name;
};



struct ClassExpressionTag
{
    String  name;
};



typedef enum
{
    REFERENCE_EXPRESSION_TYPE_SELF,
    REFERENCE_EXPRESSION_TYPE_VARIABLE,
    REFERENCE_EXPRESSION_TYPE_FUNCTION_CALL,
    REFERENCE_EXPRESSION_TYPE_CLASS,
    REFERENCE_EXPRESSION_TYPE_GENERATE,
} ReferenceExpressionType;



struct ReferenceExpressionTag
{
    ReferenceExpressionType     type;
    union {
        VariableExpression      variable;
        FunctionCallExpression  function;
    } of;
    ReferenceExpression         next;
};



struct ReferenceVariableTag
{
    String  name;
};



struct ReferenceFunctionCallTag
{
    String                  name;
    ParametersExpression    parameters;
};



struct ReferenceClassTag
{
    String  name;
};



typedef enum
{
    REFERENCE_TYPE_SELF,
    REFERENCE_TYPE_VARIABLE,
    REFERENCE_TYPE_FUNCTION_CALL,
    REFERENCE_TYPE_CLASS,
} ReferenceType;



struct ReferenceTag
{
    ReferenceType               type;
    union {
        ReferenceVariable       variable;
        ReferenceFunctionCall   function;
        ReferenceClass          klass;
    } of;
    Reference                   next;
};



typedef enum {
    STATEMENT_RESULT_NORMAL = 1,
    STATEMENT_RESULT_RETURN,
    STATEMENT_RESULT_BREAK,
    STATEMENT_RESULT_CONTINUE,
} StatementResultType;



typedef struct {
    StatementResultType type;
    union {
        CriaObject  object;
    } returns;
} StatementResult;



//==============================
//String
//==============================
String
string_newFunction(
    char    *input,
    char    *fileName,
    int     line
);
#define string_new(string)\
    (string_newFunction(string, __FILE__, __LINE__))



void
string_dispose(
    String  string
);



long
string_length(
    String  string
);



String
string_cloneFunction(
    String  source,
    char    *fileName,
    int     line
);
#define string_clone(source)\
    (string_cloneFunction(source, __FILE__, __LINE__))



String
string_subStringFunction(
    String  source,
    long    start,
    long    length,
    char    *fileName,
    int     line
);
#define string_subString(source, start, length)\
    (string_subStringFunction(source, start, length, __FILE__, __LINE__))



int
string_toInteger(
    String  source
);



Boolean
string_toBoolean(
    String  source
);



//==============================
//Item
//==============================
Item
item_newFunction(
    void    *object,
    char    *fileName,
    int     line
);



void
item_dispose(
    Item    item
);



//==============================
//List
//==============================
List
list_newFunction(
    char    *fileName,
    int     line
);
#define list_new()\
    (list_newFunction(__FILE__, __LINE__))



void
list_dispose(
    List    list
);



//オブジェクトを追加
void
list_addFunction(
    List    list,
    void    *object,
    char    *fileName,
    int     line
);
#define list_add(list, object)\
    (list_addFunction(list, object, __FILE__, __LINE__))



//オブジェクトを追加
int
list_countFunction(
    List    list,
    char    *fileName,
    int     line
);
#define list_count(list)\
    (list_countFunction(list, __FILE__, __LINE__))




//Listの内容を文字列で返却
void*
list_get(
    List    list,
    int     index
);



char*
list_toStringFunction(
    List    list,
    char*   fileName,
    int     line
);
#define list_toString(list)\
    (list_toStringFunction(list, __FILE__, __LINE__))



//==============================
//StringBuffer
//==============================
//StringBufferを生成
StringBuffer
stringBuffer_newFunction(
    char    *fileName,
    int     line
);
#define stringBuffer_new()\
    (stringBuffer_newFunction(__FILE__, __LINE__))



//StringBufferの破棄
void
stringBuffer_dispose(
    StringBuffer    stringBuffer
);



void
stringBuffer_appendCharFunction(
    StringBuffer    stringBuffer,
    char            charactor,
    char*           fileName,
    int             line
);
#define stringBuffer_appendChar(buffer, charctor)\
    (stringBuffer_appendCharFunction(buffer, charctor, __FILE__, __LINE__))



//文字列を追記
void
stringBuffer_appendFunction(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
);
#define stringBuffer_append(buffer, string)\
    (stringBuffer_appendFunction(buffer, string, __FILE__, __LINE__))



//StringBufferが保持している文字列群を一つの文字列として返却
String
stringBuffer_toStringFunction(
    StringBuffer    stringBuffer,
    char*           fileName,
    int             line
);
#define stringBuffer_toString(buffer)\
    (stringBuffer_toStringFunction(buffer, __FILE__, __LINE__))



//==================================================
//Interpreter
//==================================================
char*
interpreter_readLine(
    Interpreter interpreter
);



//==================================================
//Token
//==================================================
void
token_log(
    Token token
);



//==================================================
//Tokenizer
//==================================================
List
tokenizer_createTokens(
    char*   filePath
);



//==================================================
//Parser
//==================================================
Parser
parser_new(
    List    list
);



void
parser_dispose(
    Parser  parser
);



Boolean
parser_parse(
    Parser      parser,
    Interpreter interpreter
);



Token
parser_getCurrent(
    Parser  parser
);



Boolean
parser_next(
    Parser  parser
);



Token
parser_getNext(
    Parser  parser
);



void
parser_returnToMark(
    Parser  parser,
    Item    mark
);



Item
parser_getPosition(
    Parser  parser
);



void
parser_setPosition(
    Parser  parser,
    Item    position
);



void
parser_error(
    Token token
);



//==================================================
//Statement
//==================================================
Statement
statement_parse(
    Parser  parser
);



Boolean
commentStatement_parse(
    char*       buffer,
    int         offset
);




//==================================================
//ReturnStatement
//==================================================
ReturnStatement
returnStatement_parse(
    char*       buffer,
    int         offset
);



void
returnStatement_dispose(
    ReturnStatement statement
);



//==================================================
//WhileStatement
//==================================================
WhileStatement
whileStatement_parse(
    Interpreter interpreter,
    char*       buffer,
    int         offset
);



void
whileStatement_dispose(
    WhileStatement  statement
);



//==================================================
//FunctionCallStatement
//==================================================
void
functionCallStatement_dispose(
    FunctionCallStatement  statement
);



FunctionCallStatement
functionCallStatement_parse(
    Parser  parser
);



//==================================================
//FunctionCallStatement
//==================================================
SubstituteStatement
substituteStatement_parse(
    Parser parser
);



Boolean
substituteStatement_isMatch(
    Parser  parser
);



//==================================================
//Expression
//==================================================
void
expression_dispose(
    Expression expression
);



Expression
expression_parse(
    Parser  parser
);



ReferenceExpression
expression_parseReferenceExpression(
    Parser  parser
);



ParametersExpression
expression_parseParametersExpression(
    Parser  parser
);



//==================================================
//ObjectExpression
//==================================================
ObjectExpression
objectExpression_parse(
    char*       buffer,
    int         offset
);



void
objectExpression_dispose(
    ObjectExpression  expression
);



//==================================================
//OperationExpression
//==================================================
OperationExpression
operationExpression_new(
    OperationKind   kind,
    Expression      left,
    Expression      right
);



void
operationExpression_dispose(
    OperationExpression     expression
);



//==================================================
//StringLiteralExpression
//==================================================
void
stringLiteralExpression_dispose(
    StringLiteralExpression expression
);



StringLiteralExpression
stringLiteralExpression_parse(
    Parser  parser
);



//==================================================
//FunctionCallExpression
//==================================================
void
functionCallExpression_dispose(
    FunctionCallExpression  expression
);



FunctionCallExpression
functionCallExpression_parse(
    Parser  parser
);



//==================================================
//ParametersExpression
//==================================================
ParametersExpression
parametersExpression_parse(
    Parser  parser
);



void
parametersExpression_dispose(
    ParametersExpression    expression
);



//==================================================
//VariableExpression
//==================================================
VariableExpression
variableExpression_new(
    String                  name
);



void
variableExpression_dispose(
    VariableExpression   expression
);



VariableExpression
variableExpression_parse(
    Parser  parser
);



//==================================================
//FunctionDefinition
//==================================================
FunctionDefinition
functionDefinition_new(
    char*               name,
    Boolean             isNative,
    AccessLevel         access,
    List                parameterList,
    List                statementList,
    CriaNativeFunction* nativeFunctionPoint
);



Boolean
functionDefinition_isMatch(
    Parser parser
);



FunctionDefinition
functionDefinition_search(
    List    functions,
    char*   name
);



//==================================================
//VariableDefinition
//==================================================
VariableDefinition
variableDefinition_new(
    String  name
);



VariableDefinition
variableDefinition_search(
    List    variableList,
    String  name
);



//==================================================
//ClassDefinition
//==================================================
Boolean
classDefinition_isMatch(
    Parser parser
);



//==================================================
//IO
//==================================================
CriaId
io_write(
    Interpreter interpreter,
    List        args
);



CriaId
io_read(
    Interpreter interpreter,
    List        args
);



//==================================================
//Executor
//==================================================
StatementResult
executor_executeStatementList(
    Interpreter interpreter,
    List        statements
);



StatementResult
executor_executeStatement(
    Interpreter interpreter,
    Statement   statement
);



void
executor_executeFunctionCallStatement(
    Interpreter             interpreter,
    FunctionCallStatement   statement
);



void
executor_executeSubstituteStatement(
    Interpreter         interpreter,
    SubstituteStatement statement
);



//==================================================
//Evaluator
//==================================================
CriaId
evaluator_expression(
    Interpreter             interpreter,
    Expression              expression
);



VariableDefinition
evaluator_reference(
    Interpreter interpreter,
    Reference   reference
);



CriaId
evaluator_referenceExpression(
    Interpreter         interpreter,
    ReferenceExpression expression
);



CriaId
evaluator_functionCall(
    Interpreter             interpreter,
    FunctionCallExpression  expression
);



//==================================================
//VariableDefinition
//==================================================
VariableDefinition
variableDefinition_search(
    List    variableList,
    String  name
);



Reference
reference_parse(
    Parser  parser
);



Reference
reference_getLast(
    Reference reference
);



#define token_log(token) \
    do \
    { \
        char* buffer = NULL; \
        if (token == NULL) \
            Logger_dbg("Token type=NULL."); \
        else if (token->buffer != NULL) \
        { \
            buffer = token->buffer->pointer; \
            Logger_dbg("Token type=%2d(%3d, %2d) [%s]", token->type, token->row, token->column, buffer); \
        } \
    } while(0) \




#define tokenizer_error(buffer, row, column) \
    do { \
        Logger_err("Token error near '%s'. (line:%d, column:%d) [%s, %d]\n", buffer, row, column, __FILE__, __LINE__); \
        fprintf(stderr, "Syntax error near '%s'. [%s, %d]\n", buffer, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#define parser_error(token) \
    do { \
        Logger_err("Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", (token)->buffer->pointer, (token)->row, (token)->column, __FILE__, __LINE__); \
        fprintf(stderr, "Syntax error near '%s'. (line:%d, column:%d) [%s, %d]\n", (token)->buffer->pointer, (token)->row, (token)->column, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#define runtime_error(interpreter) \
    do { \
        fprintf(stderr, "Runtime error. (line:%d)\n", (interpreter)->lineNumber); \
        Logger_err("Runtime error. (line:%d)\n", (interpreter)->lineNumber); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#endif /* PRIVATE_CRIA_H_INCLUDED */
