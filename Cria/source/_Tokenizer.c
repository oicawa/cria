#include <string.h>
#include <ctype.h>

#include "Memory.h"
#include "Logger.h"

#include "List.h"
#include "StringBuffer.h"

#include "_Tokenizer.h"


#define TOKEN_LITERAL_PARENTHESIS_LEFT      "("
#define TOKEN_LITERAL_PARENTHESIS_RIGHT     ")"
#define TOKEN_LITERAL_BRACKET_LEFT          "["
#define TOKEN_LITERAL_BRACKET_RIGHT         "]"
#define TOKEN_LITERAL_BRACE_LEFT            "{"
#define TOKEN_LITERAL_BRACE_RIGHT           "}"
#define TOKEN_LITERAL_GENERICS_LEFT         "<"
#define TOKEN_LITERAL_GENERICS_RIGHT        ">"
#define TOKEN_LITERAL_COLON                 ":"
#define TOKEN_LITERAL_ATMARK                "@"
#define TOKEN_LITERAL_PERIOD                "."
#define TOKEN_LITERAL_NEW_LINE              "\n"
#define TOKEN_LITERAL_COMMA                 ", "
#define TOKEN_LITERAL_MONADIC_MINUS         "-"
#define TOKEN_LITERAL_UNDER                 "_"
#define TOKEN_LITERAL_COMMENT               "#"


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
#define TOKEN_LITERAL_GOTO                  "goto "
#define TOKEN_LITERAL_LOAD                  "load "
#define TOKEN_LITERAL_THROW                 "throw "
#define TOKEN_LITERAL_WHILE                 "while "
#define TOKEN_LITERAL_RETURN_VALUE          "return "

#define TOKEN_LITERAL_NULL                  "null"
#define TOKEN_LITERAL_ELSE                  "else"
#define TOKEN_LITERAL_TRUE                  "true"
#define TOKEN_LITERAL_FALSE                 "false"
#define TOKEN_LITERAL_CLASS                 "class"
#define TOKEN_LITERAL_BREAK                 "break"
#define TOKEN_LITERAL_CATCH                 "catch"
#define TOKEN_LITERAL_BLOCK                 "block"
#define TOKEN_LITERAL_RETURN                "return"
#define TOKEN_LITERAL_FINALLY               "finally"
#define TOKEN_LITERAL_CONTINUE              "continue"



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
    long size = length;
    TokenType type = TOKEN_TYPE_DUMMY;
    
    char c = start[length];
    if (c != ' ' && c != '\0')
        goto END;
    
    if (strncmp(start, TOKEN_LITERAL_NULL, size) == 0)
    {
        type = TOKEN_TYPE_NULL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_BREAK, size) == 0)
    {
        type = TOKEN_TYPE_BREAK;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_BLOCK, size) == 0)
    {
        type = TOKEN_TYPE_BLOCK;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_ELSE, size) == 0)
    {
        type = TOKEN_TYPE_ELSE;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_TRUE, size) == 0)
    {
        type = TOKEN_TYPE_BOOLEAN_LITERAL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_FALSE, size) == 0)
    {
        type = TOKEN_TYPE_BOOLEAN_LITERAL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_CLASS, size) == 0)
    {
        type = TOKEN_TYPE_CLASS;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_RETURN, size) == 0)
    {
        type = TOKEN_TYPE_RETURN;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_CONTINUE, size) == 0)
    {
        type = TOKEN_TYPE_CONTINUE;
        goto END;
    }
    
    
    size++;
    
    if (strncmp(start, TOKEN_LITERAL_IF, size) == 0)
    {
        type = TOKEN_TYPE_IF;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_ELIF, size) == 0)
    {
        type = TOKEN_TYPE_ELIF;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_WHILE, size) == 0)
    {
        type = TOKEN_TYPE_WHILE;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_LOAD, size) == 0)
    {
        type = TOKEN_TYPE_LOAD;
        goto END;
    }
    
	goto END;
	
END:
    if (type != TOKEN_TYPE_DUMMY)
    {
        token = Token_new(type, String_sub(start, 0, size));
    }
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
    long length = 0;
    TokenType type = TOKEN_TYPE_DUMMY;
    

    //Check 1 charactor.
    if (*start != ' ')
        goto END;
    
    //Check 2 charactor.
    length = 2;
    if (strncmp(start, TOKEN_LITERAL_UNDER_WITH_SPACE, length) == 0)
    {
        type = TOKEN_TYPE_JOIN;
        goto END;
    }
    
    //Check 3 charactor.
    length = 3;
    if (strncmp(start, TOKEN_LITERAL_SUBSTITUTE, length) == 0)
    {
        type = TOKEN_TYPE_SUBSTITUTE;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_PLUS, length) == 0)
    {
        type = TOKEN_TYPE_PLUS;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_MINUS, length) == 0)
    {
        type = TOKEN_TYPE_MINUS;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_MULTIPLY, length) == 0)
    {
        type = TOKEN_TYPE_MULTIPLY;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_DEVIDE, length) == 0)
    {
        type = TOKEN_TYPE_DEVIDE;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_MODULO, length) == 0)
    {
        type = TOKEN_TYPE_MODULO;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_LESS_THAN, length) == 0)
    {
        type = TOKEN_TYPE_LESS_THAN;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_GREATER_THAN, length) == 0)
    {
        type = TOKEN_TYPE_GREATER_THAN;
        goto END;
    }
    
    //Check 4 charactor.
    length = 4;
    if (strncmp(start, TOKEN_LITERAL_EQUAL, length) == 0)
    {
        type = TOKEN_TYPE_EQUAL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_INCREMENT, length) == 0)
    {
        type = TOKEN_TYPE_INCREMENT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_DECREMENT, length) == 0)
    {
        type = TOKEN_TYPE_DECREMENT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_LESS_EQUAL, length) == 0)
    {
        type = TOKEN_TYPE_LESS_EQUAL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_NOT_EQUAL, length) == 0)
    {
        type = TOKEN_TYPE_NOT_EQUAL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_OR, length) == 0)
    {
        type = TOKEN_TYPE_OR;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_AND, length) == 0)
    {
        type = TOKEN_TYPE_AND;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_GREATER_EQUAL, length) == 0)
    {
        type = TOKEN_TYPE_GREATER_EQUAL;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_EXTENDS, length) == 0)
    {
        type = TOKEN_TYPE_EXTENDS;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_INDENT, length) == 0)
    {
        type = TOKEN_TYPE_INDENT;
        goto END;
    }
    
END:
    if (type != TOKEN_TYPE_DUMMY)
    {
        token = Token_new(type, String_sub(start, 0, length));
    }
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
    length = 1;
    if (strncmp(start, TOKEN_LITERAL_PARENTHESIS_LEFT, length) == 0)
    {
        type = TOKEN_TYPE_PARENTHESIS_LEFT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_PARENTHESIS_RIGHT, length) == 0)
    {
        type = TOKEN_TYPE_PARENTHESIS_RIGHT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_BRACKET_LEFT, length) == 0)
    {
        type = TOKEN_TYPE_BRACKET_LEFT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_BRACKET_RIGHT, length) == 0)
    {
        type = TOKEN_TYPE_BRACKET_RIGHT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_BRACE_LEFT, length) == 0)
    {
        type = TOKEN_TYPE_BRACE_LEFT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_BRACE_RIGHT, length) == 0)
    {
        type = TOKEN_TYPE_BRACE_RIGHT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_GENERICS_LEFT, length) == 0)
    {
        type = TOKEN_TYPE_GENERICS_LEFT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_GENERICS_RIGHT, length) == 0)
    {
        type = TOKEN_TYPE_GENERICS_RIGHT;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_COLON, length) == 0)
    {
        type = TOKEN_TYPE_COLON;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_ATMARK, length) == 0)
    {
        type = TOKEN_TYPE_ATMARK;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_PERIOD, length) == 0)
    {
        type = TOKEN_TYPE_PERIOD;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_MONADIC_MINUS, length) == 0)
    {
        type = TOKEN_TYPE_MONADIC_MINUS;
        goto END;
    }
    
    if (strncmp(start, TOKEN_LITERAL_UNDER, length) == 0)
    {
        type = TOKEN_TYPE_JOIN;
        goto END;
    }
    
    //Check 2 charactor
    length = 2;
    if (strncmp(start, TOKEN_LITERAL_COMMA, length) == 0)
    {
        type = TOKEN_TYPE_COMMA;
        goto END;
    }
    
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
    
    
    while (cursor != NULL)
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


