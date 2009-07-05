#include <string.h>
#include <ctype.h>

#include "Memory.h"
#include "Logger.h"

#include "List.h"
#include "StringBuffer.h"

#include "_Tokenizer.h"



static struct TokenCheckerTag TOKEN_TABLE_1[] =
{
    { TOKEN_TYPE_PARENTHESIS_LEFT,  "(",  FALSE },
    { TOKEN_TYPE_PARENTHESIS_RIGHT, ")",  FALSE },
    { TOKEN_TYPE_BRACKET_LEFT,      "[",  FALSE },
    { TOKEN_TYPE_BRACKET_RIGHT,     "]",  FALSE },
    { TOKEN_TYPE_BRACE_LEFT,        "{",  FALSE },
    { TOKEN_TYPE_BRACE_RIGHT,       "}",  FALSE },
    { TOKEN_TYPE_GENERICS_LEFT,     "<",  FALSE },
    { TOKEN_TYPE_GENERICS_RIGHT,    ">",  FALSE },
    { TOKEN_TYPE_COLON,             ":",  FALSE },
    { TOKEN_TYPE_ATMARK,            "@",  FALSE },
    { TOKEN_TYPE_PERIOD,            ".",  FALSE },
    { TOKEN_TYPE_MONADIC_MINUS,     "-",  FALSE },
    { TOKEN_TYPE_JOIN,              "_",  FALSE },
    { TOKEN_TYPE_DUMMY,             NULL, FALSE }
};


static struct TokenCheckerTag TOKEN_TABLE_2[] =
{
    { TOKEN_TYPE_COMMA,             ", ", FALSE },
    { TOKEN_TYPE_DUMMY,             NULL, FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_1[] =
{
    { TOKEN_TYPE_NULL,                  "null",     TRUE },
    { TOKEN_TYPE_BREAK,                 "break",    TRUE },
    { TOKEN_TYPE_ELSE,                  "else",     TRUE },
    { TOKEN_TYPE_BOOLEAN_LITERAL_TRUE,  "true",     TRUE },
    { TOKEN_TYPE_BOOLEAN_LITERAL_FALSE, "false",    TRUE },
    { TOKEN_TYPE_CLASS,                 "class",    TRUE },
    { TOKEN_TYPE_RETURN,                "return",   TRUE },
    { TOKEN_TYPE_CONTINUE,              "continue", TRUE },
    { TOKEN_TYPE_DUMMY,                 NULL,       FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_2[] =
{
    { TOKEN_TYPE_IF,           "if ",     FALSE },
    { TOKEN_TYPE_ELIF,         "elif ",   FALSE },
    { TOKEN_TYPE_WHILE,        "while ",  FALSE },
    { TOKEN_TYPE_LOAD,         "load ",   FALSE },
    { TOKEN_TYPE_RETURN_VALUE, "return ", FALSE },
    { TOKEN_TYPE_DUMMY,        NULL,      FALSE }
};

static struct TokenCheckerTag TOKENS_START_WITH_SPACE[] =
{
    { TOKEN_TYPE_AND,           " && ", FALSE },
    { TOKEN_TYPE_DECREMENT,     " -= ", FALSE },
    { TOKEN_TYPE_DEVIDE,        " / ",  FALSE },
    { TOKEN_TYPE_EQUAL,         " == ", FALSE },
    { TOKEN_TYPE_EXTENDS,       " -> ", FALSE },
    { TOKEN_TYPE_GREATER_EQUAL, " >= ", FALSE },
    { TOKEN_TYPE_GREATER_THAN,  " > ",  FALSE },
    { TOKEN_TYPE_INCREMENT,     " += ", FALSE },
    { TOKEN_TYPE_INDENT,        "    ", FALSE },
    { TOKEN_TYPE_JOIN,          " _",   TRUE },
    { TOKEN_TYPE_LESS_THAN,     " < ",  FALSE },
    { TOKEN_TYPE_LESS_EQUAL,    " <= ", FALSE },
    { TOKEN_TYPE_MINUS,         " - ", FALSE },
    { TOKEN_TYPE_MODULO,        " % ", FALSE },
    { TOKEN_TYPE_MULTIPLY,      " * ", FALSE },
    { TOKEN_TYPE_NOT_EQUAL,     " != ", FALSE },
    { TOKEN_TYPE_OR,            " || ", FALSE },
    { TOKEN_TYPE_PLUS,          " + ", FALSE },
    { TOKEN_TYPE_SUBSTITUTE,    " = ", FALSE },
    { TOKEN_TYPE_DUMMY,         NULL,  FALSE }
};



Token
Token_new(
    TokenType type,
    String value
)
{
    Token token = Memory_malloc(sizeof(struct TokenTag));
    token->type = type;
    token->value = value;
    return token;
}


Token
Token_parse(
    char* start,
    struct TokenCheckerTag *table
)
{
    Token token = NULL;
    struct TokenCheckerTag *checker = NULL;
    char c = '0';
    
    while (table != NULL)
    {
        checker = table;
        if (checker->type == TOKEN_TYPE_DUMMY)
            break;
        
        if (strncmp(start, checker->value, strlen(checker->value)) != 0)
        {
            table++;
            continue;
        }
        
        if (checker->terminate == FALSE)
        {
            token = Token_new(checker->type, checker->value);
            break;
        }
        
        c = start[strlen(checker->value)];
        if (isalnum(c) != 0)
        {
            token = Token_new(checker->type, checker->value);
            break;
        }
        
        table++;
    }

    return token;
}



Token
Tokenizer_number(
    char* cursor
)
{
    Token token = NULL;
    char* start = cursor;
    Boolean point = FALSE;
    long size = 0;

    
    if (isdigit(*cursor) == 0)
        goto END;
    
    while (TRUE)
    {
        if (isdigit(*cursor) != 0)
        {
            cursor++;
            continue;
        }
        
        if (*cursor == '.' && point == FALSE)
        {
            cursor++;
            point = TRUE;
            continue;
        }
        
        goto END;
    }
    
    size = cursor - start;
    token = Token_new(TOKEN_TYPE_INTEGER_LITERAL, String_sub(start, 0, size));
    
END:
    return token;
}


//TODO: Make tokens table. Get this function logic more simple.
Token
Tokenizer_reserved(
    char* start,
    long length
)
{
    Token token = NULL;
    
    char c = start[length];
    if (c != ' ' && c != '\0')
        goto END;
    
    token = Token_parse(start, TOKENS_RESERVED_2);
    if (token != NULL)
        goto END;
    
    token = Token_parse(start, TOKENS_RESERVED_1);
    if (token != NULL)
        goto END;
    
END:
    return token;
    
}


Token
Tokenizer_identifier(
    char* start,
    long length
)
{
    Token token = NULL;
    char* cursor = start;
    int i = 0;
    
    if (islower(*cursor) == 0)
        goto END;


    cursor++;
    for (i = 0; i < length - 1; i++)
    {
        if (islower(*cursor) != 0)
            goto NEXT;
        
        if (isdigit(*cursor) != 0)
            goto NEXT;
        
        if (*cursor == '_')
            goto NEXT;
        
		goto END;
NEXT:
        cursor += 1;
    }

    token = Token_new(TOKEN_TYPE_IDENTIFIER, String_sub(start, 0, length));
    
END:
    return token;
}



Token
Tokenizer_class_or_constant(
    char* start,
    long length
)
{
    Token token = NULL;
    char* cursor = start;
    TokenType type = TOKEN_TYPE_CONSTANT;
    int i = 0;
    
    if (length < 2)
        goto END;
    
    //Check 1st charactor.
    if (isupper(*cursor) == 0)
        goto END;

    //Check 2nd charactor.
    cursor++;
    if ((isupper(*cursor) != 0) || (*cursor == '_'))
    {
        type = TOKEN_TYPE_CONSTANT;
    }
    else if (islower(*cursor) != 0)
    {
        type = TOKEN_TYPE_CLASS_LITERAL;
    }
    else
    {
        goto END;
    }
    
    cursor++;
    for (i = 0; i < length - 2; i++)
    {
    	if ((type == TOKEN_TYPE_CLASS_LITERAL) && (isalnum(*cursor) != 0))
    	{
    		cursor++;
    		continue;
    	}
    	
    	if ((type == TOKEN_TYPE_CONSTANT)
            && ((isupper(*cursor) != 0)
                || *cursor == '_'
                || isdigit(*cursor) != 0))
    	{
    		cursor++;
    		continue;
    	}
    	
		goto END;
    }

    token = Token_new(type, String_sub(start, 0, length));
    
END:
    return token;
}



Token
Tokenizer_word(
    char* cursor
)
{
    Token token = NULL;
    char* start = cursor;
    long length = 0;
    
    if (isalpha(*cursor) == 0)
        goto END;
    
    while (TRUE)
    {
        if ((isalnum(*cursor) != 0)
            || *cursor == '_')
        {
            cursor++;
            continue;
        }
        break;
    }
    
    length = cursor - start;
    
    token = Tokenizer_reserved(start, length);
    if (token != NULL)
        goto END;
    
    token = Tokenizer_identifier(start, length);
    if (token != NULL)
    	goto END;
    
    token = Tokenizer_class_or_constant(start, length);
    if (token != NULL)
    	goto END;
    
END:
    return token;
}



Token
Tokenizer_space(
    char* start
)
{
    Token token = NULL;
    

    //Check 1 charactor.
    if (*start != ' ')
        goto END;
    
    //Check 2 charactor.
    token = Token_parse(start, TOKENS_START_WITH_SPACE);
    if (token != NULL)
        goto END;
    
    
END:
    return token;
}

Token
Tokenizer_string(
    char* start
)
{
    Token token = NULL;
    char* cursor = start;
    Boolean is_end = FALSE;
    long length = 0;
    
    if (*start != '"')
        goto END;
    
    cursor++;
    while (*cursor != '\0')
    {
        if (*cursor == '\\')
        {
            cursor += 2;
            continue;
        }
        
        if (*cursor == '"')
        {
            is_end = TRUE;
            cursor++;
            break;
        }
        
        cursor++;
    }
    
    
    if (is_end == FALSE)
        goto END;
    
    length = cursor - start;
    token = Token_new(TOKEN_TYPE_STRING_LITERAL, String_sub(start, 0, length));
    
END:
    return token;
}


Token
Tokenizer_other(
    char* start
)
{
    Token token = NULL;
    TokenType type = TOKEN_TYPE_DUMMY;
    long length = 0;
    
    //Check 1 charactor
    token = Token_parse(start, TOKEN_TABLE_1);
    if (token != NULL)
        goto END;
    
    
    //Check 2 charactor
    token = Token_parse(start, TOKEN_TABLE_2);
    if (token != NULL)
        goto END;
    
END:
    if (type != TOKEN_TYPE_DUMMY)
    {
        token = Token_new(type, String_sub(start, 0, length));
    }
    return token;
}



List
Tokenizer_split(
    Interpreter interpreter,
    String file_path,
    int line,
    String target
)
{
    Logger_trc("[ START ]%s", __func__);
    Token token = NULL;
    List tokens = List_new();
    char* cursor = &target[0];
    
    
    while (*cursor != '\0')
    {
        token = Tokenizer_number(cursor);
        if (token != NULL)
            goto ADD_TOKEN;
        
        token = Tokenizer_string(cursor);
        if (token != NULL)
            goto ADD_TOKEN;
        
        token = Tokenizer_word(cursor);
        if (token != NULL)
            goto ADD_TOKEN;
        
        token = Tokenizer_space(cursor);
        if (token != NULL)
            goto ADD_TOKEN;
        
        token = Tokenizer_other(cursor);
        if (token != NULL)
            goto ADD_TOKEN;
        
        break;
        
ADD_TOKEN:
        List_add(tokens, token);
        cursor += strlen(token->value);
    }

    Logger_trc("[  END  ]%s", __func__);
    return tokens;
}


