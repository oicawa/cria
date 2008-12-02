#ifndef PRIVATE_CRIA_H_INCLUDED
#define PRIVATE_CRIA_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>



#include "Cria.h"



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
    List    statements;
    List    variables;
    List    functions;
    List    classes;
    int     row;
    int     column;
    int     indentLevel;
};



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
    int             indentLevel;
};



struct ParserTag
{
    List    tokens;
    Item    current;
    Item    next;
    Item    mark;
};



struct StatementTag
{
    StatementKind   kind;
    int             line;
    union {
        StatementFunctionCall   _functionCall_;
        StatementSubstitute     _substitute_;
        StatementIf             _if_;
        StatementWhile          _while_;
        StatementGoto           _goto_;
    } of;
};



typedef enum
{
	GOTO_TYPE_CONTINUE,
	GOTO_TYPE_BREAK,
	GOTO_TYPE_LABEL,
	GOTO_TYPE_RETURN,
} GotoType;



struct StatementGotoTag
{
	GotoType type;
	union
	{
	    String  label;
	    Expression expression;
	} of;
};



struct StatementIfTag
{
    Expression  condition;
    List        statements;
    StatementIf _if_;
};



struct StatementWhileTag
{
    Expression  condition;
    List        statements;
};



struct StatementSubstituteTag
{
    Reference   reference;
    Expression  expression;
};



struct StatementFunctionCallTag
{
    ReferenceExpression  expression;
};



struct VariableDefinitionTag
{
    String      name;
    Boolean     isStatic;
    CriaId      object;
};



struct FunctionDefinitionTag
{
    char*                       name;
    Boolean                     isNative;
    Boolean                    isStatic;
    union
    {
        struct
        {
            List                parameterList;
            List                statementList;
        } cria;
        struct
        {
            CriaNativeFunction* function;
        } native;
    } of;
    CriaObject                  returnValue;
};



struct ClassDefinitionTag
{
    String      name;
    Boolean     isNative;
    AccessLevel access;
    List        baseList;
    List        fieldList;
    List        methodList;
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
        GenerateExpression          _generate_;
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
    List    list;
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
        GenerateExpression      generate;
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
    STATEMENT_RESULT_LABEL,
} StatementResultType;



typedef struct {
    StatementResultType type;
    union {
        String label;
        CriaId id;
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



String
string_cloneFunction(
    String  source,
    char    *fileName,
    int     line
);
#define string_clone(source)\
    (string_cloneFunction(source, __FILE__, __LINE__))



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



void
list_addFunction(
    List    list,
    void    *object,
    char    *fileName,
    int     line
);
#define list_add(list, object)\
    (list_addFunction(list, object, __FILE__, __LINE__))



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



StringBuffer
stringBuffer_newFunction(
    char    *fileName,
    int     line
);
#define stringBuffer_new()\
    (stringBuffer_newFunction(__FILE__, __LINE__))



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



void
stringBuffer_appendFunction(
    StringBuffer    stringBuffer,
    char*           string,
    char*           fileName,
    int             line
);
#define stringBuffer_append(buffer, string)\
    (stringBuffer_appendFunction(buffer, string, __FILE__, __LINE__))



String
stringBuffer_toStringFunction(
    StringBuffer    stringBuffer,
    char*           fileName,
    int             line
);
#define stringBuffer_toString(buffer)\
    (stringBuffer_toStringFunction(buffer, __FILE__, __LINE__))



List
tokenizer_create_tokens(
    char*   filePath
);



Boolean
parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter
);



Statement
statement_new(
    StatementKind   kind
);



void
statement_dispose(
    Statement   statement
);



Expression
expression_new(
    ExpressionKind  kind
);



void
expression_dispose(
    Expression  expression
);



Reference
reference_new(
    ReferenceType   type
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



FunctionDefinition
functionDefinition_parse(
    Parser parser
);



CriaId
functionDefinition_evaluate(
	Interpreter interpreter,
    CriaId id,
	List parameterList,
	FunctionDefinition function,
	List parameters
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
ClassDefinition
classDefinition_new(
	Interpreter interpreter,
    char*               name,
    Boolean             isNative,
    List                fieldList,
    List                methodList,
    CriaNativeClassLoader* classLoader
);



CriaId
classDefinition_evaluate(
    Interpreter interpreter,
    CriaId  id,
    List parameterList,
    char*   name,
    ClassDefinition klass,
    List parameters
);




ClassDefinition
classDefinition_search(
    List classList,
    char* name
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
    CriaId object,
    List        args
);



CriaId
io_read(
    Interpreter interpreter,
    CriaId object,
    List        args
);



//==================================================
//Executor
//==================================================
StatementResult
executor_executeStatementList(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    List        statements
);



StatementResult
executor_executeStatement(
    Interpreter interpreter,
    CriaId object, 
    List parameters,
    Statement   statement
);



void
executor_executeFunctionCallStatement(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    StatementFunctionCall statement
);



void
executor_executeSubstituteStatement(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    StatementSubstitute statement
);



//==================================================
//Evaluator
//==================================================
CriaId
evaluator_expression(
    Interpreter             interpreter,
    CriaId object,
    List parameters,
    Expression              expression
);



VariableDefinition
evaluator_reference(
    Interpreter interpreter,
    CriaId object,
    List parameters,
    Reference   reference
);



CriaId
evaluator_referenceExpression(
    Interpreter         interpreter,
    CriaId object,
    List parameters,
    ReferenceExpression expression
);



CriaId
evaluator_functionCall(
    Interpreter interpreter,
    CriaId object,
    List parameters,
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



#define token_log(token) \
    do \
    { \
    	Logger_dbg("token is [%p]", token); \
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
        fprintf(stderr, "Syntax error near '%s'. [%s, %d]\n", buffer, __FILE__, __LINE__); \
        Logger_err("Token error near '%s'. (line:%d, column:%d) [%s, %d]\n", buffer, row, column, __FILE__, __LINE__); \
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
        Logger_err("Runtime error. (line:%d) [%s, %d]\n", (interpreter)->row, __FILE__, __LINE__); \
        fprintf(stderr, "Runtime error. (line:%d) [%s, %d]\n", (interpreter)->row, __FILE__, __LINE__); \
        Memory_dispose(); \
        exit(1); \
    } while(0) \



#endif /* PRIVATE_CRIA_H_INCLUDED */
