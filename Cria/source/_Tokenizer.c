#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "Cria.h"
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
    { TOKEN_TYPE_JOIN,              "_",  TRUE },
    { TOKEN_TYPE_DUMMY,             NULL, FALSE }
};


static struct TokenCheckerTag TOKEN_TABLE_2[] =
{
    { TOKEN_TYPE_COMMA,             ", ", FALSE },
    { TOKEN_TYPE_DUMMY,             NULL, FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_0[] =
{
    { TOKEN_TYPE_NULL,                  "null",     TRUE },
    { TOKEN_TYPE_BOOLEAN_LITERAL_TRUE,  "true",     TRUE },
    { TOKEN_TYPE_BOOLEAN_LITERAL_FALSE, "false",    TRUE },
    { TOKEN_TYPE_BLOCK,                 "block",    TRUE },
    { TOKEN_TYPE_CLASS,                 "class",    TRUE },
    { TOKEN_TYPE_DUMMY,                 NULL,       FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_1[] =
{
    { TOKEN_TYPE_BREAK,                 "break",    TRUE },
    { TOKEN_TYPE_ELSE,                  "else",     TRUE },
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
        if (isalnum(c) == 0)
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
        
        break;
    }
    
    size = cursor - start;
    token = Token_new(TOKEN_TYPE_INTEGER_LITERAL, String_sub(start, 0, size));
    
END:
    return token;
}


Token
Tokenizer_reserved(
    char* start,
    long length
)
{
    Token token = NULL;

    
    token = Token_parse(start, TOKENS_RESERVED_0);
    if (token != NULL)
        goto END;
    
    char c = start[length];
    if (c != ' ' && c != '\0')
        goto END;

    //It's important that checking order of TOKENS_RESERVED_2, TOKENS_RESERVED_1.
    
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
    
    //Check 1 charactor
    token = Token_parse(start, TOKEN_TABLE_1);
    if (token != NULL)
        goto END;
    
    
    //Check 2 charactor
    token = Token_parse(start, TOKEN_TABLE_2);
    if (token != NULL)
        goto END;
    
END:
    return token;
}



void
Tokenizer_error(
    char* file_path,
    int line,
    char* target,
    char* cursor,
    char* format,
    ...
)
{
    va_list argp;
    va_start(argp, format);
    long column = 0;

    fprintf(stderr, "Illegal token error. [%s, %d]\n", file_path, line);
    
    vfprintf(stderr, format, argp );
    fprintf(stderr, "\n");
    
    
    fprintf(stderr, "--------------------\n");
    fprintf(stderr, "%s\n", target);
    column = cursor - target;
    while (0 < column)
    {
        fprintf(stderr, " ");
        column--;
    }
    fprintf(stderr, "^\n");
    
    exit(1);
}



TokenType
Token_type(
	Token token
)
{
    if (token == NULL)
    {
        Logger_err("token is nil.");
    }
	return token->type;
}



List
Tokenizer_split(
    char* target,
    Boolean skip,
    String file_path,
    int line
)
{
    List tokens = List_new();
    Token token = NULL;
    char* cursor = &target[0];
    
    
    if (skip == TRUE)
    {
        while (*cursor == ' ')
            cursor++;
    }
    
    
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

        Tokenizer_error(file_path, line, target, cursor, "Illegal token.");
        break;
        
ADD_TOKEN:
        token->row = line;
        token->column = cursor - &target[0];
        token->file_path = file_path;
        List_add(tokens, token);
        cursor += strlen(token->value);
    }
    
    return tokens;
}


Boolean
Tokenizer_read_line(FILE* file, char* buffer, long size)
{
    memset(buffer, 0x00, size);
    char* lf = NULL;
    Boolean result = FALSE;
    
    fgets(buffer, size, file);
    lf = strchr(buffer, '\n'); 
    if (lf != NULL)
    {
        *lf = '\0';
    }
    else if (feof(file) == FALSE)
    {
        goto END;
    }
    
    result = TRUE;
END:
    return result;
}



int
Tokenizer_get_indent(List tokens, int indent_current, int line)
{
    int indent_new = -1;
    int count = 0;
    int total = List_count(tokens);
    int indent = 0;
    Token token = NULL;
    
    
    for (count = 0; count < total - 1; count++)
    {
        token = List_get(tokens, 0);
        if (token->type != TOKEN_TYPE_INDENT)
            break;
        
        List_delete(tokens, 0);
    }

    
    indent = count - indent_current;
    
    
    if (1 < indent)
    {
        indent_new = -1;
        goto END;
    }
    
    
    indent_new = indent_current + indent;
    
    
    if (indent == 1)
    {
        token = Token_new(TOKEN_TYPE_INDENT, "<<INDENT>>");
        token->row = line;
        token->column = indent_new * 4;
        List_insert(tokens, 0, token);
        goto END;
    }
    
    if (indent == 0)
    {
        goto END;
    }
    
    //  indent < 0
    while (indent < 0)
    {
        token = Token_new(TOKEN_TYPE_DEDENT, "<<DEDENT>>");
        token->row = line;
        token->column = indent_new * 4;
        List_insert(tokens, 0, token);
        indent++;
    }
    
END:
    return indent_new;
}


Boolean
Tokenizer_check_join(List tokens, int line, int column)
{
    Token token = NULL;
    Boolean skip = FALSE;
    int count = 0;
    
    count = List_count(tokens);
    token = List_get(tokens, count - 1);
    if (token->type == TOKEN_TYPE_JOIN)
    {
        List_delete(tokens, count - 1);
        skip = TRUE;
    }
    else
    {
        token = Token_new(TOKEN_TYPE_NEW_LINE, "<<NEW_LINE>>");
        token->row = line;
        token->column = column;
        List_add(tokens, token);
    }
    
    return skip;
}



Boolean
Tokenizer_is_blank_line(char* buffer)
{
    Boolean result = TRUE;
    char* cursor = buffer;
    
    while (*cursor == ' ')
        cursor++;
    
    if (*cursor == '\0')
        goto END;
    
    if (*cursor == '#')
        goto END;
    
    result = FALSE;
    
END:
    return result;
}



int
Tokenizer_terminate(List tokens, int indent_current, int line)
{
    Token token = NULL;
    
    while (0 < indent_current)
    {
        token = Token_new(TOKEN_TYPE_DEDENT, "<<DEDENT>>");
        token->row = line;
        token->column = indent_current * 4;
        List_add(tokens, token);
        indent_current--;
    }
    
    token = Token_new(TOKEN_TYPE_TERMINATE, "<<TERMINATE>>");
    token->row = line;
    token->column = 0;
    List_add(tokens, token);
    
    return indent_current;
}


void
Tokenizer_print_all_tokens(
    List tokens,
    char* filePath
)
{
    int c = List_count(tokens);
    int i = 0;
    Token token = NULL;
    for (i = 0; i < c; i++)
    {
        token = List_get(tokens, i);
        if (token == NULL)
            printf("(!!null!!)\n");
        else
            printf("[%s]%3d : %s\n", filePath, token->row, token->value);
    }
}

#define BUFFER_SIZE 80
List
Tokenizer_create_tokens(
    char*   filePath,
    Boolean is_main
)
{
    FILE* file = NULL;
    int line = 1;
    int indent_current = 0;
    List tokens = NULL;
    
    char buffer[BUFFER_SIZE + 1];
    int size = sizeof(buffer);
    List tmp = NULL;
    Boolean skip = FALSE;
    
    
    file = fopen(filePath, "r");
    if (file == NULL)
    {
        if (!is_main)
            goto END;

        fprintf(stderr, "File open error. [%s]\n\n", filePath);
        exit(0);
    }
    
    tokens = List_new();
    
    
    while (feof(file) == 0)
    {
        if (!Tokenizer_read_line(file, buffer, size))
        {
            fprintf(stderr, "[%s] line %3d : Too long sentence. Please get the line under %d columns.\n\n", filePath, line, size);
            goto END;
        }
        
        
        if (Tokenizer_is_blank_line(buffer))
        {
            line++;
            continue;
        }
        
        
        tmp = Tokenizer_split(buffer, skip, filePath, line);
        
        
        if (skip == FALSE)
        {
            indent_current = Tokenizer_get_indent(tmp, indent_current, line);
            if (indent_current < 0)
            {
                fprintf(stderr, "[%s] line %3d : Indent level error.\n\n", filePath, line);
                goto END;
            }
        }
        
        skip = Tokenizer_check_join(tmp, line, strlen(buffer));
        
        
        List_cat(tokens, tmp);
        
        
        line++;
    }
    
    Tokenizer_terminate(tokens, indent_current, line);

END:
    if (file != NULL)
        fclose(file);
    
        
    return tokens;
}

