#include "_Cria.h"


#define TOKEN_LITERAL_PARENTHESIS_LEFT      "("
#define TOKEN_LITERAL_PARENTHESIS_RIGHT     ")"
#define TOKEN_LITERAL_BRACKET_LEFT          "["
#define TOKEN_LITERAL_BRACKET_RIGHT         "]"
#define TOKEN_LITERAL_BRACE_LEFT            "{"
#define TOKEN_LITERAL_BRACE_RIGHT           "}"
#define TOKEN_LITERAL_GENERICS_LEFT         "<"
#define TOKEN_LITERAL_GENERICS_RIGHT        ">"
#define TOKEN_LITERAL_COLON                 ":"
#define TOKEN_LITERAL_MEMBER                "@"
#define TOKEN_LITERAL_PERIOD                "."
#define TOKEN_LITERAL_NEW_LINE              "\n"
#define TOKEN_LITERAL_COMMA                 ", "
#define TOKEN_LITERAL_MONADIC_MINUS         "-"
#define TOKEN_LITERAL_UNDER                 "_"


#define TOKEN_LITERAL_UNDER_WITH_SPACE      " _"

#define TOKEN_LITERAL_SUBSTITUTE            " = "
#define TOKEN_LITERAL_EQUAL                 " == "
#define TOKEN_LITERAL_NOT_EQUAL             " != "

#define TOKEN_LITERAL_PLUS                  " + "
#define TOKEN_LITERAL_INCREMENT             " += "

#define TOKEN_LITERAL_MINUS                 " - "
#define TOKEN_LITERAL_DECREMENT             " -= "
#define TOKEN_LITERAL_EXTENDS               " -> "

#define TOKEN_LITERAL_OR                    " || "
#define TOKEN_LITERAL_AND                   " && "

#define TOKEN_LITERAL_MULTIPLY              " * "
#define TOKEN_LITERAL_DEVIDE                " / "
#define TOKEN_LITERAL_MODULO                " % "

#define TOKEN_LITERAL_LESS_THAN             " < "
#define TOKEN_LITERAL_LESS_EQUAL            " <= "

#define TOKEN_LITERAL_NOT_EQUAL             " != "
#define TOKEN_LITERAL_GREATER_THAN          " > "
#define TOKEN_LITERAL_GREATER_EQUAL         " >= "
#define TOKEN_LITERAL_IN                    " in "

#define TOKEN_LITERAL_INDENT                "    "


#define TOKEN_LITERAL_IF                    "if "
#define TOKEN_LITERAL_FOR                   "for "
#define TOKEN_LITERAL_ELIF                  "elif "
#define TOKEN_LITERAL_THROW                 "throw "
#define TOKEN_LITERAL_WHILE                 "while "
#define TOKEN_LITERAL_RETURN_VALUE          "return "

#define TOKEN_LITERAL_NULL                  "null"
#define TOKEN_LITERAL_ELSE                  "else"
#define TOKEN_LITERAL_TRUE                  "true"
#define TOKEN_LITERAL_FALSE                 "false"
#define TOKEN_LITERAL_BREAK                 "break"
#define TOKEN_LITERAL_CATCH                 "catch"
#define TOKEN_LITERAL_RETURN                "return"
#define TOKEN_LITERAL_FINALLY               "finally"
#define TOKEN_LITERAL_CONTINUE              "continue"



Token
token_new(
    TokenType   type,
    int         row,
    int         column,
    String      buffer
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = Memory_malloc(sizeof(struct TokenTag));
    memset(token, 0x00, sizeof(struct TokenTag));
    token->type = type;
    token->row = row;
    token->column = column;
    if (buffer != NULL)
    {
        token->buffer = string_clone(buffer);
        Logger_trc(buffer->pointer);
    }
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



void
token_dispose(
    Token   token
)
{
    Logger_trc("[ START ]%s", __func__);
    if (token == NULL)
    {
        Logger_dbg("'token' is NULL");
        goto END;
    }
    
    Logger_dbg("Check 'token->buffer'");
    if (token->buffer != NULL)
    {
        Logger_dbg("Free 'token->buffer'");
        string_dispose(token->buffer);
        token->buffer = NULL;
    }
    
    Logger_dbg("Free 'token'");
    Memory_free(token);
    token = NULL;
    
END:
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
tokenizer_readChar(
    Tokenizer   tokenizer
)
{
    int c = fgetc(tokenizer->file);
    if (tokenizer->next == 0x0A)
    {
        tokenizer->row += 1;
        tokenizer->column = 1;
    }
    else
    {
        tokenizer->column += 1;
    }
    
    
    if (c == EOF)
    {
        return FALSE;
    }
    
    
    tokenizer->next = c;
    return TRUE;
}



Tokenizer
tokenizer_new(
    char*   filePath
)
{
    Logger_trc("[ START ]%s", __func__);
    Tokenizer tokenizer = NULL;
    
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        Logger_err("File open error. (%s)", filePath);  //★終了させたい
        goto END;
    }
    
    tokenizer = Memory_malloc(sizeof(struct TokenizerTag));
    memset(tokenizer, 0x00, sizeof(struct TokenizerTag));
    tokenizer->file = file;
    tokenizer->row = 1;
    tokenizer->column = 0;
    tokenizer->next = '\0';
    tokenizer->error = NULL;
    tokenizer->indentLevel = 0;
    
    tokenizer_readChar(tokenizer);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return tokenizer;
}



void
tokenizer_dispose(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    Logger_dbg("Check 'tokenizer'");
    if (tokenizer == NULL)
    {
        Logger_dbg("'tokenizer' is NULL");
        return;
    }
    
    Logger_dbg("Check 'tokenizer->file'");
    if (tokenizer->file != NULL)
    {
        Logger_dbg("Free 'tokenizer->file'");
        fclose(tokenizer->file);
        tokenizer->file = NULL;
    }
    
    Logger_dbg("Check 'tokenizer->error'");
    if (tokenizer->error != NULL)
    {
        Logger_dbg("Free 'tokenizer->error'");
        string_dispose(tokenizer->error);
        tokenizer->error = NULL;
    }
    
    
    Logger_dbg("Free 'tokenizer'");
    Memory_free(tokenizer);
    tokenizer = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
}



Token
tokenizer_parseNumber(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    StringBuffer buffer = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    if (isdigit(tokenizer->next) == 0)
        goto END;
    
    
    buffer = stringBuffer_new();
    stringBuffer_appendChar(buffer, tokenizer->next);
    
    
    while (tokenizer_readChar(tokenizer) == TRUE)
    {
        if (isdigit(tokenizer->next) == 0)
        {
            Logger_dbg("Not number. (%c)", tokenizer->next);
            break;
        }
        stringBuffer_appendChar(buffer, tokenizer->next);
    }
    
    
    String tmp = stringBuffer_toString(buffer);
    Logger_dbg("Add token. (%s)", tmp->pointer);
    token = token_new(TOKEN_TYPE_INTEGER_LITERAL, row, column, tmp);
    
    
    stringBuffer_dispose(buffer);
    string_dispose(tmp);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Token
tokenizer_parseReserved(
    Tokenizer   tokenizer,
    StringBuffer buffer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    String  tmp = NULL;
    char*   value = NULL;
    Token   token = NULL;
    int    row = tokenizer->row;
    int    column = tokenizer->column;
    
    tmp = stringBuffer_toString(buffer);
    value = tmp->pointer;
    if (strcmp(value, TOKEN_LITERAL_NULL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_NULL");
        token = token_new(TOKEN_TYPE_NULL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_BREAK) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_BREAK");
        token = token_new(TOKEN_TYPE_BREAK, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_CATCH) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_CATCH");
        token = token_new(TOKEN_TYPE_CATCH, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_ELSE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_ELSE");
        token = token_new(TOKEN_TYPE_ELSE, row, column, NULL);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_TRUE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_BOOLEAN_LITERAL");
        token = token_new(TOKEN_TYPE_BOOLEAN_LITERAL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_FALSE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_BOOLEAN_LITERAL");
        token = token_new(TOKEN_TYPE_BOOLEAN_LITERAL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_RETURN) == 0)
    {
		Logger_dbg("create TOKEN_TYPE_RETURN");
		token = token_new(TOKEN_TYPE_RETURN, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_FINALLY) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_FINALLY");
        token = token_new(TOKEN_TYPE_FINALLY, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_CONTINUE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_CONTINUE");
        token = token_new(TOKEN_TYPE_CONTINUE, row, column, tmp);
        goto END;
    }
    
    
    if (tokenizer->next != ' ')
    {
        Logger_dbg("It is not reserved word.");
        goto END;
    }
    
    
    if (strncmp(value, TOKEN_LITERAL_IF, strlen(TOKEN_LITERAL_IF) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_IF");
        token = token_new(TOKEN_TYPE_IF, row, column, tmp);
        goto READ;
    }
    
    if (strncmp(value, TOKEN_LITERAL_FOR, strlen(TOKEN_LITERAL_FOR) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_FOR");
        token = token_new(TOKEN_TYPE_FOR, row, column, tmp);
        goto READ;
    }
    
    if (strncmp(value, TOKEN_LITERAL_ELIF, strlen(TOKEN_LITERAL_ELIF) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_ELIF");
        token = token_new(TOKEN_TYPE_ELIF, row, column, tmp);
        goto READ;
    }
    
    if (strncmp(value, TOKEN_LITERAL_WHILE, strlen(TOKEN_LITERAL_WHILE) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_WHILE");
        token = token_new(TOKEN_TYPE_WHILE, row, column, tmp);
        goto READ;
    }
    
    if (strncmp(value, TOKEN_LITERAL_THROW, strlen(TOKEN_LITERAL_THROW) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_THROW");
        token = token_new(TOKEN_TYPE_THROW, row, column, tmp);
        goto READ;
    }
    
	Logger_dbg("Not reserved word with space. (%s)", tmp->pointer);
	goto END;
	
READ:
	tokenizer_readChar(tokenizer);
    
END:
	string_dispose(tmp);
    Logger_trc("[  END  ]%s", __func__);
    return token;
    
}



Token
tokenizer_parseVariableOrFunction(
    Tokenizer   	tokenizer,
    StringBuffer	buffer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token   token = NULL;
    String  tmp = stringBuffer_toString(buffer);
    char*   identifier = tmp->pointer;
    
    
    if (islower(*identifier) == 0)
    {
        Logger_dbg("First charactor is not lower alphabet. ('%s')", identifier);
        goto END;
    }


    identifier += 1;
    while (*identifier != '\0')
    {
        if (isalnum(*identifier) == 0)
        {
            Logger_dbg("Not alphabet or number.");
            goto END;
        }
        identifier += 1;
    }

    token = token_new(TOKEN_TYPE_IDENTIFIER, tokenizer->row, tokenizer->column, tmp);
    
END:
    string_dispose(tmp);
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Token
tokenizer_parseClassLiteralOrConstant(
    Tokenizer   	tokenizer,
    StringBuffer	buffer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token   token = NULL;
    String  tmp = stringBuffer_toString(buffer);
    char*   value = tmp->pointer;
    TokenType type = TOKEN_TYPE_CONSTANT;
    
    
    if (strlen(value) < 2)
    {
        Logger_err("String length is less than 2.");
        tokenizer_error(tmp->pointer, tokenizer->row, tokenizer->column);
        goto END;
    }
    
    
    if (isupper(*value) == 0)
    {
        Logger_dbg("First charactor is not upper alphabet. ('%c')", *buffer);
        tokenizer_error(tmp->pointer, tokenizer->row, tokenizer->column);
        goto END;
    }

    value += 1;
    if ((isupper(*value) != 0) || (*value == '_'))
    {
        type = TOKEN_TYPE_CONSTANT;
    }
    else if (islower(*value) != 0)
    {
        type = TOKEN_TYPE_CLASS_LITERAL;
    }
    else
    {
        Logger_err("It is not a Class literal or Constant.");
        tokenizer_error(tmp->pointer, tokenizer->row, tokenizer->column);
        goto END;
    }
    
    value += 1;
    while (*value != '\0')
    {
    	if ((type == TOKEN_TYPE_CLASS_LITERAL) && (isalnum(*value) != 0))
    	{
    		value += 1;
    		continue;
    	}
    	
    	if ((type == TOKEN_TYPE_CONSTANT) && ((isupper(*value) != 0) || *value == '_'))
    	{
    		value += 1;
    		continue;
    	}
    	
		Logger_dbg("Not alphabet or underbar. (%c)", *buffer);
		tokenizer_error(tmp->pointer, tokenizer->row, tokenizer->column);
		goto END;
    }

    token = token_new(type, tokenizer->row, tokenizer->column, tmp);
    
END:
    string_dispose(tmp);
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Token
tokenizer_parseIdentifier(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    StringBuffer buffer = stringBuffer_new();
    stringBuffer_appendChar(buffer, tokenizer->next);
    
    if (isalpha(tokenizer->next) == 0)
        goto END;
    
    Logger_dbg("read next charactor");
    while (tokenizer_readChar(tokenizer) == TRUE)
    {
        Logger_dbg("Is alphabet or number?");
        if ((isalnum(tokenizer->next) != 0)
            || tokenizer->next == '_')
        {
            stringBuffer_appendChar(buffer, tokenizer->next);
            continue;
        }
        Logger_dbg("next is not alphabet, number, or underbar. '%c'", tokenizer->next);
        break;
    }
    
    token = tokenizer_parseReserved(tokenizer, buffer);
    if (token != NULL)
    	goto END;
    
    token = tokenizer_parseVariableOrFunction(tokenizer, buffer);
    if (token != NULL)
    	goto END;
    
    token = tokenizer_parseClassLiteralOrConstant(tokenizer, buffer);
    if (token != NULL)
    	goto END;

	String tmp = stringBuffer_toString(buffer);
	tokenizer_error(tmp->pointer, tokenizer->row, tokenizer->column);
	string_dispose(tmp);
	
END:
    stringBuffer_dispose(buffer);
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Token
tokenizer_parseDummy(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    Boolean hasNewLine = FALSE;
    
    while (tokenizer_readChar(tokenizer) == TRUE)
    {
        if (tokenizer->next == '\n')
        {
            hasNewLine = TRUE;
            continue;
        }
        
        if (tokenizer->next != ' ')
            break;
    }
    
    if (hasNewLine == TRUE)
        token = token_new(TOKEN_TYPE_DUMMY, 0, 0, NULL);
    else
        tokenizer_error(" _...", tokenizer->row, tokenizer->column);
    
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Token
tokenizer_parseSpace(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    StringBuffer buffer = stringBuffer_new();
    stringBuffer_appendChar(buffer, tokenizer->next);
    String tmp = stringBuffer_toString(buffer);
    char* value = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    if (tokenizer->next != ' ')
        goto END;
    
    //--------------------------------------------------
    //2nd charactor.
    //--------------------------------------------------
    if (tokenizer_readChar(tokenizer) == FALSE)
    {
        Logger_dbg("No 2nd charactor.");
        goto END;
    }
    Logger_dbg("2nd charactor = '%c'", tokenizer->next);
    stringBuffer_appendChar(buffer, tokenizer->next);
    if (tokenizer->next == '_')
    {
        token = tokenizer_parseDummy(tokenizer);
        goto END;
    }
    string_dispose(tmp);
    tmp = stringBuffer_toString(buffer);
    value = tmp->pointer;
    
    
    //--------------------------------------------------
    //3rd charactor.
    //--------------------------------------------------
    Logger_dbg("read 3rd charactor");
    if (tokenizer_readChar(tokenizer) == FALSE)
    {
        Logger_dbg("No 3rd charactor.");
        tokenizer_error(value, row, column);
        goto END;
    }
    Logger_dbg("3rd charactor = '%c'", tokenizer->next);
    stringBuffer_appendChar(buffer, tokenizer->next);
    string_dispose(tmp);
    tmp = stringBuffer_toString(buffer);
    value = tmp->pointer;
    
    if (strcmp(value, TOKEN_LITERAL_SUBSTITUTE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_SUBSTITUTE");
        token = token_new(TOKEN_TYPE_SUBSTITUTE, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_PLUS) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_PLUS");
        token = token_new(TOKEN_TYPE_PLUS, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_MINUS) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_MINUS");
        token = token_new(TOKEN_TYPE_MINUS, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_MULTIPLY) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_MULTIPLY");
        token = token_new(TOKEN_TYPE_MULTIPLY, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_DEVIDE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_DEVIDE");
        token = token_new(TOKEN_TYPE_DEVIDE, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_MODULO) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_MODULO");
        token = token_new(TOKEN_TYPE_MODULO, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_LESS_THAN) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_LESS_THAN");
        token = token_new(TOKEN_TYPE_LESS_THAN, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_GREATER_THAN) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_GREATER_THAN");
        token = token_new(TOKEN_TYPE_GREATER_THAN, row, column, tmp);
        goto END;
    }
    
    
    //--------------------------------------------------
    //４文字目
    //--------------------------------------------------
    Logger_dbg("read 4th charactor");
    if (tokenizer_readChar(tokenizer) == FALSE)
    {
        Logger_dbg("No 4th charactor.");
        tokenizer_error(value, row, column);
        goto END;
    }
    stringBuffer_appendChar(buffer, tokenizer->next);
    string_dispose(tmp);
    tmp = stringBuffer_toString(buffer);
    value = tmp->pointer;
    
    if (strcmp(value, TOKEN_LITERAL_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_EQUAL");
        token = token_new(TOKEN_TYPE_EQUAL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_INCREMENT) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_INCREMENT");
        token = token_new(TOKEN_TYPE_INCREMENT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_DECREMENT) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_DECREMENT");
        token = token_new(TOKEN_TYPE_DECREMENT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_LESS_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_LESS_EQUAL");
        token = token_new(TOKEN_TYPE_LESS_EQUAL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_NOT_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_NOT_EQUAL");
        token = token_new(TOKEN_TYPE_NOT_EQUAL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_OR) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_OR");
        token = token_new(TOKEN_TYPE_OR, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_AND) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_AND");
        token = token_new(TOKEN_TYPE_AND, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_GREATER_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_GREATER_EQUAL");
        token = token_new(TOKEN_TYPE_GREATER_EQUAL, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_EXTENDS) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_EXTENDS");
        token = token_new(TOKEN_TYPE_EXTENDS, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_IN) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_IN");
        token = token_new(TOKEN_TYPE_IN, row, column, tmp);
        goto END;
    }
    
    tokenizer_error(value, row, column);
    
END:
    if (token != NULL)
        tokenizer_readChar(tokenizer);
    string_dispose(tmp);
    stringBuffer_dispose(buffer);
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Token
tokenizer_parseStringLiteral(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    StringBuffer buffer = stringBuffer_new();
    String tmp = NULL;
    Boolean isEnd = FALSE;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    if (tokenizer->next != '"')
        goto END;
    
    
    stringBuffer_appendChar(buffer, tokenizer->next);
    while (tokenizer_readChar(tokenizer) == TRUE)
    {
        stringBuffer_appendChar(buffer, tokenizer->next);
        
        
        if (tokenizer->next == '\\')
        {
            if (tokenizer_readChar(tokenizer) == FALSE)
            {
                Logger_dbg("No string literal");
                break;
            }
            stringBuffer_appendChar(buffer, tokenizer->next);
            continue;
        }
        
        
        if (tokenizer->next == '"')
        {
            isEnd = TRUE;
            break;
        }
    }
    
    
    tmp = stringBuffer_toString(buffer);
    
    if (isEnd == FALSE)
    {
        tokenizer_error(tmp->pointer, row, column);
        goto END;
    }
    
    token = token_new(TOKEN_TYPE_STRING_LITERAL, row, column, tmp);
    
END:
    string_dispose(tmp);
    stringBuffer_dispose(buffer);
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



void
tokenizer_parseIndentDedent(
    Tokenizer   tokenizer,
    List        tokens
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    String tmp = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    StringBuffer buffer = stringBuffer_new();
    
    
    while (tokenizer_readChar(tokenizer) == TRUE)
    {
        if (tokenizer->next == ' ')
        {
            stringBuffer_appendChar(buffer, tokenizer->next);
            continue;
        }
        
        if (tokenizer->next == '\n')
        {
        	String tmp1 = stringBuffer_toString(buffer);
		    Logger_dbg("Buffer clear. ('%s')", tmp1->pointer);
		    string_dispose(tmp1);
            stringBuffer_dispose(buffer);
            buffer = stringBuffer_new();
            continue;
        }
        
	    Logger_dbg("Illegal charactor = '%c'", tokenizer->next);
        break;
    }
    
    
    tmp = stringBuffer_toString(buffer);
    Logger_dbg("Next charactor = '%c'", tokenizer->next);
    Logger_dbg("buffer = '%s'", tmp->pointer);
    long length = string_length(tmp);
    if (length % 4 != 0)
    {
        Logger_err("Indent is not 4 times spaces. (modulo = %d)", length % 4);
        tokenizer_error("<<Indent/Dedent>>", tokenizer->row, tokenizer->column);
        goto END;
    }
    
    
    int which = (length / 4) - tokenizer->indentLevel;
    int i = 0;
    if (which < 0)
    {
        which = which * (-1);
        for (i = 0; i < which; i++)
        {
            Logger_dbg("create TOKEN_TYPE_DEDENT");
            tokenizer->indentLevel -= 1;
            token = token_new(TOKEN_TYPE_DEDENT, row, column, string_new("<<DEDENT>>"));
            list_add(tokens, token);
        }
    }
    else if (which == 1)
    {
        Logger_dbg("create TOKEN_TYPE_INDENT");
        tokenizer->indentLevel += 1;
        token = token_new(TOKEN_TYPE_INDENT, row, column, string_new("<<INDENT>>"));
        list_add(tokens, token);
    }
    else if (which == 0)
    {
        //インデントなし。
        Logger_dbg("Indent level is same above line.");
    }
    else
    {
        //一度に複数のインデントは発生してはいけない。
        Logger_err("Multi indent is not permited.");
        tokenizer_error("<<Indent/DEDENT>>", tokenizer->row, tokenizer->column);
    }
END:
    string_dispose(tmp);
    stringBuffer_dispose(buffer);
    Logger_trc("[  END  ]%s", __func__);
}



Boolean
tokenizer_parseNewLineIndentDedent(
    Tokenizer   tokenizer,
    List        tokens
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Boolean result = FALSE;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    if (tokenizer->next != '\n')
        goto END;
    
    
    Logger_trc("Create 'TOKEN_TYPE_NEW_LINE'");
    String tmp = string_new("<<NEW_LINE>>");
    Token token = token_new(TOKEN_TYPE_NEW_LINE, row, column, tmp);
    string_dispose(tmp);
    
    
    list_add(tokens, token);
    result = TRUE;
    
    
    Logger_trc("Try indent/dedent literal.");
    tokenizer_parseIndentDedent(tokenizer, tokens);
    
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Token
tokenizer_parseOther(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s('%c')", __func__, tokenizer->next);
    Token token = NULL;
    StringBuffer buffer = stringBuffer_new();
    String tmp = NULL;
    char* value = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    Logger_trc("Check first charactor.(%c)", tokenizer->next);
    stringBuffer_appendChar(buffer, tokenizer->next);
    tmp = stringBuffer_toString(buffer);
    value = tmp->pointer;
    if (strcmp(value, TOKEN_LITERAL_PARENTHESIS_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_PARENTHESIS_LEFT'");
        token = token_new(TOKEN_TYPE_PARENTHESIS_LEFT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_PARENTHESIS_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_PARENTHESIS_RIGHT'");
        token = token_new(TOKEN_TYPE_PARENTHESIS_RIGHT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_BRACKET_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACKET_LEFT'");
        token = token_new(TOKEN_TYPE_BRACKET_LEFT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_BRACKET_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACKET_RIGHT'");
        token = token_new(TOKEN_TYPE_BRACKET_RIGHT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_BRACE_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACE_LEFT'");
        token = token_new(TOKEN_TYPE_BRACE_LEFT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_BRACE_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACE_RIGHT'");
        token = token_new(TOKEN_TYPE_BRACE_RIGHT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_GENERICS_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_GENERICS_LEFT'");
        token = token_new(TOKEN_TYPE_GENERICS_LEFT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_GENERICS_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_GENERICS_RIGHT'");
        token = token_new(TOKEN_TYPE_GENERICS_RIGHT, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_COLON) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_COLON'");
        token = token_new(TOKEN_TYPE_COLON, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_MEMBER) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_MEMBER'");
        token = token_new(TOKEN_TYPE_MEMBER, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_PERIOD) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_PERIOD'");
        token = token_new(TOKEN_TYPE_PERIOD, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_MONADIC_MINUS) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_MONADIC_MINUS'");
        token = token_new(TOKEN_TYPE_MONADIC_MINUS, row, column, tmp);
        goto END;
    }
    
    if (strcmp(value, TOKEN_LITERAL_UNDER) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_UNDER'");
        token = tokenizer_parseDummy(tokenizer);
        goto END;
    }
    
    if (tokenizer->next == '"')
    {
        //文字列リテラルの解析
        Logger_trc("Try string literal.");
        token = tokenizer_parseStringLiteral(tokenizer);
        goto END;
    }
    
    if (tokenizer->next == ',')
    {
        Logger_trc("Try comma literal.");
        if (tokenizer_readChar(tokenizer) == FALSE)
        {
            tokenizer_error(value, row, column);
            goto END;
        }
        
        string_dispose(tmp);
        stringBuffer_appendChar(buffer, tokenizer->next);
        tmp = stringBuffer_toString(buffer);
        value = tmp->pointer;
        
        if (tokenizer->next != ' ')
        {
            tokenizer_error(value, row, column);
            goto END;
        }
        
        Logger_trc("Create 'TOKEN_TYPE_COMMA'");
        token = token_new(TOKEN_TYPE_COMMA, row, column, tmp);
        goto END;
    }
    
    
    tokenizer_error(value, row, column);
    
    
END:
    tokenizer_readChar(tokenizer);
    stringBuffer_dispose(buffer);
    string_dispose(tmp);
    Logger_trc("[  END  ]%s", __func__);
    return token;
}



Boolean
tokenizer_isEndOfFile(
    Tokenizer   tokenizer
)
{
    if (feof(tokenizer->file) == 0)
        return FALSE;
    
	return TRUE;
}



void
tokenizer_logAllTokens(
    List	tokens
)
{
    long index = 0;
    long count = tokens->count;
    
    
    while (index < count)
    {
        Token token = (Token)list_get(tokens, index);
        token_log(token);
        index += 1;
    }
}



List
tokenizer_createTokens(
    char*   filePath
)
{
    Logger_trc("[ START ]%s", __func__);
    Tokenizer tokenizer = tokenizer_new(filePath);
    Token token = NULL;
    List tokens = list_new();
    
    
    while (tokenizer_isEndOfFile(tokenizer) == FALSE)
    {
        token = tokenizer_parseNumber(tokenizer);
        if (token != NULL)
            goto ADD_TOKEN;
        
        token = tokenizer_parseIdentifier(tokenizer);
        if (token != NULL)
            goto ADD_TOKEN;
        
        token = tokenizer_parseSpace(tokenizer);
        if (token != NULL)
            goto ADD_TOKEN;
        
        if (tokenizer_parseNewLineIndentDedent(tokenizer, tokens) == TRUE)
            continue;
        
        token = tokenizer_parseOther(tokenizer);
        if (token != NULL)
            goto ADD_TOKEN;
        
        Logger_err("Not supported token.");
        tokenizer_error("???", tokenizer->row, tokenizer->column);
        break;
        
ADD_TOKEN:
        list_add(tokens, token);
        continue;        
    }
    
    
    tokenizer_logAllTokens(tokens);
    
    Logger_trc("[  END  ]%s", __func__);
    return tokens;
}



