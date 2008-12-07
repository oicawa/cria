#include <memory.h>

#include "../Memory/Memory.h"
#include "../Logger/Logger.h"

#include "List.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "DefinitionVariable.h"
#include "Expression.h"


#include "_Statement.h"


Statement
Statement_new(
    StatementKind   kind
)
{
    Statement statement = Memory_malloc(sizeof(struct StatementTag));
    memset(statement, 0x00, sizeof(struct StatementTag));
    statement->kind = kind;
    return statement;
}



Statement
StatementWhile_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementWhile whileStatement = NULL;
    Expression condition = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_WHILE)
    {
        Logger_dbg("token type is not 'WHILE'.");
        parser_setPosition(parser, position);
        goto END;
    }
    parser_next(parser);
    
    
    condition = parser_expression(parser);
    if (condition == NULL)
    {
        Logger_dbg("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token)  != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token_type(token)  == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'WhileStatement'");
    whileStatement = Memory_malloc(sizeof(struct StatementWhileTag));
    memset(whileStatement, 0x00, sizeof(struct StatementWhileTag));
    whileStatement->condition = condition;
    whileStatement->statements = statements;
    
    statement = Statement_new(STATEMENT_KIND_WHILE);
    statement->of._while_ = whileStatement;
    statement->line = token_row(token) ;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



Statement
StatementGoto_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementGoto gotoStatement = NULL;
    Item position = parser_getPosition(parser);
    GotoType type = GOTO_TYPE_LABEL;
    String label = NULL;
    Expression expression = NULL;
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    switch (token_type(token))
    {
    case TOKEN_TYPE_BREAK:
    	type = GOTO_TYPE_BREAK;
	    parser_next(parser);
        break;
    case TOKEN_TYPE_CONTINUE:
    	type = GOTO_TYPE_CONTINUE;
	    parser_next(parser);
        break;
    case TOKEN_TYPE_GOTO:
    	type = GOTO_TYPE_LABEL;
	    parser_next(parser);
	    token = parser_getCurrent(parser);
	    label = token_buffer(token);
        break;
    case TOKEN_TYPE_RETURN:
    	type = GOTO_TYPE_RETURN;
	    parser_next(parser);
        break;
    case TOKEN_TYPE_RETURN_VALUE:
    	type = GOTO_TYPE_RETURN;
	    parser_next(parser);
	    expression = parser_expression(parser);
        break;
    default:
    	parser_setPosition(parser, position);
    	goto END;
    }
    
    token = parser_getCurrent(parser);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
    	parser_setPosition(parser, position);
    	parser_error(token);
    	goto END;
    }
	parser_next(parser);
    
    Logger_dbg("Create 'GotoStatement'");
    gotoStatement = Memory_malloc(sizeof(struct StatementGotoTag));
    memset(gotoStatement, 0x00, sizeof(struct StatementGotoTag));
    gotoStatement->type = type;
    if (label != NULL)
    	gotoStatement->of.label = string_clone(label);
    gotoStatement->of.expression = expression;
    
    statement = Statement_new(STATEMENT_KIND_GOTO);
    statement->of._goto_ = gotoStatement;
    statement->line = token_row(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



Statement
StatementFunctionCall_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementFunctionCall  function = NULL;
    ExpressionReference expression = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    
    Logger_dbg("Check 'FunctionCallExpression'");
    expression = parser_expression_reference(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not reference expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("Not new line token.");
        token_log(token);
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    parser_next(parser);
    
    
    Logger_dbg("Create 'FunctionCallStatement'");
    function = Memory_malloc(sizeof(struct StatementFunctionCallTag));
    memset(function, 0x00, sizeof(struct StatementFunctionCallTag));
    function->expression = expression;
    
    statement = Statement_new(STATEMENT_KIND_FUNCTION_CALL);
    statement->of._functionCall_ = function;
    statement->line = token_row(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



StatementIf
StatementIf_parseElseBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_ELSE)
    {
        Logger_err("token type is not 'ELSE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is not 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct StatementIfTag));
    memset(ifStatement, 0x00, sizeof(struct StatementIfTag));
    ifStatement->statements = statements;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



StatementIf
StatementIf_parseElifBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    Expression condition = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_ELIF)
    {
        Logger_err("token type is not 'ELIF'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    condition = parser_expression(parser);
    if (condition == NULL)
    {
        Logger_err("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is not 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct StatementIfTag));
    memset(ifStatement, 0x00, sizeof(struct StatementIfTag));
    ifStatement->condition = condition;
    ifStatement->statements = statements;
    
    
    token = parser_getCurrent(parser);
    if (token_type(token) == TOKEN_TYPE_ELSE)
    {
        Logger_dbg("token type is 'ELSE'.");
        ifStatement->_if_ = StatementIf_parseElseBlock(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



StatementIf
StatementIf_parseIfBlock(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    Expression condition = NULL;
    List statements = list_new();
    Item position = parser_getPosition(parser);
    Token token = NULL;
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_IF)
    {
        Logger_dbg("token type is not 'IF'.");
        parser_setPosition(parser, position);
        goto END;
    }
    parser_next(parser);
    
    
    condition = parser_expression(parser);
    if (condition == NULL)
    {
        Logger_dbg("condition expression is NULL.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_err("token type is not 'NEW LINE'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    token = parser_getCurrent(parser);
    token_log(token);
    if (token_type(token) != TOKEN_TYPE_INDENT)
    {
        Logger_err("token type is not 'INDENT'.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    parser_next(parser);
    
    
    while(1)
    {
        token = parser_getCurrent(parser);
        token_log(token);
        if (token_type(token) == TOKEN_TYPE_DEDENT)
        {
            Logger_dbg("token type is 'DEDENT'.");
            break;
        }
        
        statement = Statement_parse(parser);
        if (statement == NULL)
        {
            Logger_err("statement parse error.");
            parser_setPosition(parser, position);
            parser_error(token);
            goto END;
        }
        
        Logger_dbg("Add statement.");
        list_add(statements, statement);
    }
    parser_next(parser);
    
    
    Logger_dbg("Create 'IfStatement'");
    ifStatement = Memory_malloc(sizeof(struct StatementIfTag));
    memset(ifStatement, 0x00, sizeof(struct StatementIfTag));
    ifStatement->condition = condition;
    ifStatement->statements = statements;
    
    
    token = parser_getCurrent(parser);
    if (token_type(token) == TOKEN_TYPE_ELIF)
    {
        Logger_dbg("token type is 'ELIF'.");
        ifStatement->_if_ = StatementIf_parseElifBlock(parser);
    }
    else if (token_type(token) == TOKEN_TYPE_ELSE)
    {
        Logger_dbg("token type is 'ELSE'.");
        ifStatement->_if_ = StatementIf_parseElseBlock(parser);
    }
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return ifStatement;
}



Statement
StatementIf_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementIf ifStatement = NULL;
    Item position = parser_getPosition(parser);
    Token token = parser_getCurrent(parser);
    
    
    ifStatement = StatementIf_parseIfBlock(parser);
    if (ifStatement == NULL)
    {
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    statement = Statement_new(STATEMENT_KIND_IF);
    statement->of._if_ = ifStatement;
    statement->line = token_row(token);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



Statement
StatementSubstitute_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    StatementSubstitute substitute = NULL;
    Item position = parser_getPosition(parser);
    Token token = NULL;
    Reference reference = NULL;
    Expression expression = NULL;
    
    
    Logger_dbg("Check reference expression.");
    reference = parser_reference(parser);
    if (reference == NULL)
    {
        Logger_dbg("Not reference expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    token = parser_getCurrent(parser);
    if (token_type(token) != TOKEN_TYPE_SUBSTITUTE)
    {
        Logger_dbg("Not substitution token.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    
    Logger_dbg("Check 'Expression'");
    parser_next(parser);
    expression = parser_expression(parser);
    if (expression == NULL)
    {
        Logger_dbg("Not expression.");
        parser_setPosition(parser, position);
        goto END;
    }
    
    token = parser_getCurrent(parser);
    if (token_type(token) != TOKEN_TYPE_NEW_LINE)
    {
        Logger_dbg("Not new line token.");
        parser_setPosition(parser, position);
        parser_error(token);
        goto END;
    }
    
    
    parser_next(parser);
    
    
    Logger_dbg("Create 'SubstituteStatement'");
    substitute = Memory_malloc(sizeof(struct StatementSubstituteTag));
    memset(substitute, 0x00, sizeof(struct StatementSubstituteTag));
    substitute->reference = reference;
    substitute->expression = expression;
    
    statement = Statement_new(STATEMENT_KIND_SUBSTITUTE);
    statement->of._substitute_ = substitute;
    statement->line = token_row(token);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



void
StatementSubstitute_execute(
    Interpreter         interpreter,
    CriaId object,
	List parameters,
    StatementSubstitute statement
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionVariable definition = NULL;
    Reference reference = statement->reference;
    CriaId id = NULL;
    
    id = Expression_evaluate(interpreter, object, parameters, statement->expression);
    
    
    definition = evaluator_reference(interpreter, object, parameters, reference);
    
    
    definition_variable_set(definition, id);
    
    
    Logger_trc("[  END  ]%s", __func__);
}



void
Statement_dispose(
    Statement   statement
)
{
    Logger_trc("[ START ]%s", __func__);
    if (statement == NULL)
    {
        goto END;
    }
    
    switch (statement->kind)
    {
    case STATEMENT_KIND_SUBSTITUTE:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_FUNCTION_CALL:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_IF:
        //ifStatement_dispose(statement->of._if_);
        break;
    case STATEMENT_KIND_WHILE:
        //whileStatement_dispose(statement->of._while_);
        break;
    case STATEMENT_KIND_FOR:
        //forStatement_dispose(statement->of._for_);
        break;
    case STATEMENT_KIND_GOTO:
        //returnStatement_dispose(statement->of._return_);
        break;
    case STATEMENT_KIND_CATCH:
        //finallyStatement_dispose(statement->of._catch_);
        break;
    case STATEMENT_KIND_FINALLY:
        //finallyStatement_dispose(statement->of._finally_);
        break;
    default:
        Logger_err("Illegal statement type. (%d)", statement->kind);
        break;
    }
    Memory_free(statement);
    statement = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Statement
Statement_parse(
    Parser  parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Statement statement = NULL;
    
    statement = StatementSubstitute_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created SubstituteStatement.");
        goto END;
    }
    
    
    statement = StatementFunctionCall_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created FunctionCallStatement.");
        goto END;
    }
    
    
    statement = StatementIf_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created IfStatement.");
        goto END;
    }
    
    
    statement = StatementWhile_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created WhileStatement.");
        goto END;
    }
    
    
    statement = StatementGoto_parse(parser);
    if (statement != NULL)
    {
        Logger_dbg("Created BreakStatement.");
        goto END;
    }
    
    
    Logger_dbg("No Statement.");
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return statement;
}



