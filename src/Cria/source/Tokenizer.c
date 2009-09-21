/*
 *  $Id$
 *
 *  ===============================================================================
 *
 *   Copyright (C) 2008-2009  Masamitsu Oikawa  <oicawa@gmail.com>
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 *
 *  ===============================================================================
 */

 
#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "List.h"

#include "Tokenizer.h"



static struct TokenCheckerTag TOKEN_TABLE_1[] =
{
    { TOKEN_TYPE_PARENTHESIS_LEFT,  L"(",  FALSE },
    { TOKEN_TYPE_PARENTHESIS_RIGHT, L")",  FALSE },
    { TOKEN_TYPE_BRACKET_LEFT,      L"[",  FALSE },
    { TOKEN_TYPE_BRACKET_RIGHT,     L"]",  FALSE },
    { TOKEN_TYPE_BRACE_LEFT,        L"{",  FALSE },
    { TOKEN_TYPE_BRACE_RIGHT,       L"}",  FALSE },
    { TOKEN_TYPE_GENERICS_LEFT,     L"<",  FALSE },
    { TOKEN_TYPE_GENERICS_RIGHT,    L">",  FALSE },
    { TOKEN_TYPE_COLON,             L":",  FALSE },
    { TOKEN_TYPE_ATMARK,            L"@",  FALSE },
    { TOKEN_TYPE_PERIOD,            L".",  FALSE },
    { TOKEN_TYPE_MONADIC_MINUS,     L"-",  FALSE },
    { TOKEN_TYPE_JOIN,              L"_",  TRUE },
    { TOKEN_TYPE_DUMMY,             NULL, FALSE }
};


static struct TokenCheckerTag TOKEN_TABLE_2[] =
{
    { TOKEN_TYPE_COMMA,             L", ", FALSE },
    { TOKEN_TYPE_DUMMY,             NULL, FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_0[] =
{
    { TOKEN_TYPE_NULL,                  L"null",     TRUE },
    { TOKEN_TYPE_BOOLEAN_LITERAL_TRUE,  L"true",     TRUE },
    { TOKEN_TYPE_BOOLEAN_LITERAL_FALSE, L"false",    TRUE },
    { TOKEN_TYPE_BLOCK,                 L"block",    TRUE },
    { TOKEN_TYPE_CLASS,                 L"class",    TRUE },
    { TOKEN_TYPE_DUMMY,                 NULL,       FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_1[] =
{
    { TOKEN_TYPE_BREAK,                 L"break",    TRUE },
    { TOKEN_TYPE_ELSE,                  L"else",     TRUE },
    { TOKEN_TYPE_RETURN,                L"return",   TRUE },
    { TOKEN_TYPE_CONTINUE,              L"continue", TRUE },
    { TOKEN_TYPE_DUMMY,                 NULL,       FALSE }
};

static struct TokenCheckerTag TOKENS_RESERVED_2[] =
{
    { TOKEN_TYPE_IF,           L"if ",     FALSE },
    { TOKEN_TYPE_ELIF,         L"elif ",   FALSE },
    { TOKEN_TYPE_WHILE,        L"while ",  FALSE },
    { TOKEN_TYPE_LOAD,         L"load ",   FALSE },
    { TOKEN_TYPE_RETURN_VALUE, L"return ", FALSE },
    { TOKEN_TYPE_DUMMY,        NULL,      FALSE }
};

static struct TokenCheckerTag TOKENS_START_WITH_SPACE[] =
{
    { TOKEN_TYPE_AND,           L" && ", FALSE },
    { TOKEN_TYPE_DECREMENT,     L" -= ", FALSE },
    { TOKEN_TYPE_DEVIDE,        L" / ",  FALSE },
    { TOKEN_TYPE_EQUAL,         L" == ", FALSE },
    { TOKEN_TYPE_EXTENDS,       L" -> ", FALSE },
    { TOKEN_TYPE_GREATER_EQUAL, L" >= ", FALSE },
    { TOKEN_TYPE_GREATER_THAN,  L" > ",  FALSE },
    { TOKEN_TYPE_INCREMENT,     L" += ", FALSE },
    { TOKEN_TYPE_INDENT,        L"    ", FALSE },
    { TOKEN_TYPE_JOIN,          L" _",   TRUE },
    { TOKEN_TYPE_LESS_THAN,     L" < ",  FALSE },
    { TOKEN_TYPE_LESS_EQUAL,    L" <= ", FALSE },
    { TOKEN_TYPE_MINUS,         L" - ", FALSE },
    { TOKEN_TYPE_MODULO,        L" % ", FALSE },
    { TOKEN_TYPE_MULTIPLY,      L" * ", FALSE },
    { TOKEN_TYPE_NOT_EQUAL,     L" != ", FALSE },
    { TOKEN_TYPE_OR,            L" || ", FALSE },
    { TOKEN_TYPE_PLUS,          L" + ", FALSE },
    { TOKEN_TYPE_SUBSTITUTE,    L" = ", FALSE },
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
    String start,
    struct TokenCheckerTag *table
)
{
    Token token = NULL;
    struct TokenCheckerTag *checker = NULL;
    wchar_t c = L'0';
    
    while (table != NULL)
    {
        checker = table;
        if (checker->type == TOKEN_TYPE_DUMMY)
            break;
        
        if (wcsncmp(start, checker->value, wcslen(checker->value)) != 0)
        {
            table++;
            continue;
        }
        
        if (checker->terminate == FALSE)
        {
            token = Token_new(checker->type, checker->value);
            break;
        }
        
        c = start[wcslen(checker->value)];
        if (iswalnum(c) == 0)
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
    String cursor
)
{
    Token token = NULL;
    String start = cursor;
    Boolean point = FALSE;
    Boolean is_real = FALSE;
    long length = 0;

    
    if (iswdigit(*cursor) == 0)
        goto END;
    
    while (TRUE)
    {
        if (iswdigit(*cursor) != 0)
        {
            is_real = point;
            cursor++;
            continue;
        }
        
        if (*cursor != L'.')
            break;
        
        if (point == FALSE)
        {
            cursor++;
            point = TRUE;
            continue;
        }
        
        break;
    }
    
    if (is_real == TRUE)
    {
        //TODO: It hasn't implemented 'Real Number'.
        goto END;
    }
    
    if (point == TRUE)
        cursor--;
    
    length = wcslen(start) - wcslen(cursor);
    token = Token_new(TOKEN_TYPE_INTEGER_LITERAL, String_sub(start, 0, length));
    
END:
    return token;
}


Token
Tokenizer_reserved(
    String start,
    long length
)
{
    Token token = NULL;

    
    token = Token_parse(start, TOKENS_RESERVED_0);
    if (token != NULL)
        goto END;
    
    wchar_t c = start[length];
    if (c != L' ' && c != L'\0')
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
    String start,
    long length
)
{
    Token token = NULL;
    String cursor = start;
    int i = 0;
    
    if (iswlower(*cursor) == 0)
        goto END;


    cursor++;
    for (i = 0; i < length - 1; i++)
    {
        if (iswlower(*cursor) != 0)
            goto NEXT;
        
        if (iswdigit(*cursor) != 0)
            goto NEXT;
        
        if (*cursor == L'_')
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
    String start,
    long length
)
{
    Token token = NULL;
    String cursor = start;
    TokenType type = TOKEN_TYPE_CONSTANT;
    int i = 0;
    
    if (length < 2)
        goto END;
    
    //Check 1st charactor.
    if (iswupper(*cursor) == 0)
        goto END;

    //Check 2nd charactor.
    cursor++;
    if ((iswupper(*cursor) != 0) || (*cursor == L'_'))
    {
        type = TOKEN_TYPE_CONSTANT;
    }
    else if (iswlower(*cursor) != 0)
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
    	if ((type == TOKEN_TYPE_CLASS_LITERAL) && (iswalnum(*cursor) != 0))
    	{
    		cursor++;
    		continue;
    	}
    	
    	if ((type == TOKEN_TYPE_CONSTANT)
            && ((iswupper(*cursor) != 0)
                || *cursor == L'_'
                || iswdigit(*cursor) != 0))
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
    String cursor
)
{
    Token token = NULL;
    String start = cursor;
    long length = 0;
    
    if (iswalpha(*cursor) == 0)
        goto END;
    
    while (TRUE)
    {
        if ((iswalnum(*cursor) != 0)
            || *cursor == L'_')
        {
            cursor++;
            continue;
        }
        break;
    }
    
    length = wcslen(start) - wcslen(cursor);
    
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
    String start
)
{
    Token token = NULL;
    

    //Check 1 charactor.
    if (*start != L' ')
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
    String start
)
{
    Token token = NULL;
    String cursor = start;
    Boolean is_end = FALSE;
    long length = 0;
    
    if (*start != L'"')
        goto END;
    
    cursor++;
    while (*cursor != L'\0')
    {
        if (*cursor == L'\\')
        {
            cursor += 2;
            continue;
        }
        
        if (*cursor == L'"')
        {
            is_end = TRUE;
            cursor++;
            break;
        }
        
        cursor++;
    }
    
    
    if (is_end == FALSE)
        goto END;
    
    length = wcslen(start) - wcslen(cursor);
    token = Token_new(TOKEN_TYPE_STRING_LITERAL, String_sub(start, 0, length));
    
END:
    return token;
}


Token
Tokenizer_other(
    String start
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
    String file_path,
    int line,
    String target,
    String cursor,
    char* format,
    ...
)
{
    va_list argp;
    va_start(argp, format);
    long column = 0;

    fprintf(stderr, "Illegal token error. [%s, %d]\n", String_wcsrtombs(file_path), line);
    
    vfprintf(stderr, format, argp );
    fprintf(stderr, "\n");
    
    
    fprintf(stderr, "--------------------\n");
    fprintf(stderr, "%s\n", String_wcsrtombs(target));
    column = wcslen(cursor) - wcslen(target);
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
    String target,
    Boolean skip,
    String file_path,
    int line
)
{
    List tokens = List_new();
    Token token = NULL;
    String cursor = &target[0];
    
    
    if (skip == TRUE)
    {
        while (*cursor == L' ')
            cursor++;
    }
    
    
    while (*cursor != L'\0')
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
        cursor += wcslen(token->value);
    }
    
    return tokens;
}



String
Tokenizer_read_line(FILE* file)
{
    char buffer[80 + 1 + 1];
    char* lf = NULL;
    String string = NULL;

    memset(buffer, 0x00, sizeof(buffer));
    
    fgets(buffer, sizeof(buffer) - 1, file);
    lf = strchr(buffer, '\n'); 
    if (lf != NULL)
    {
        *lf = '\0';
    }
    else if (feof(file) == FALSE)
    {
        goto END;
    }
    
    string = String_mbsrtowcs(buffer);
END:
    return string;
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
        token = Token_new(TOKEN_TYPE_INDENT, L"<<INDENT>>");
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
        token = Token_new(TOKEN_TYPE_DEDENT, L"<<DEDENT>>");
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
        token = Token_new(TOKEN_TYPE_NEW_LINE, L"<<NEW_LINE>>");
        token->row = line;
        token->column = column;
        List_add(tokens, token);
    }
    
    return skip;
}



Boolean
Tokenizer_is_blank_line(String buffer)
{
    Boolean result = TRUE;
    wchar_t* cursor = buffer;
    
    while (*cursor == L' ')
        cursor++;
    
    if (*cursor == L'\0')
        goto END;
    
    if (*cursor == L'#')
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
        token = Token_new(TOKEN_TYPE_DEDENT, L"<<DEDENT>>");
        token->row = line;
        token->column = indent_current * 4;
        List_add(tokens, token);
        indent_current--;
    }
    
    token = Token_new(TOKEN_TYPE_TERMINATE, L"<<TERMINATE>>");
    token->row = line;
    token->column = 0;
    List_add(tokens, token);
    
    return indent_current;
}


void
Tokenizer_print_all_tokens(
    List tokens,
    String filePath
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
            printf("[%s]%3d : %s\n", String_wcsrtombs(filePath), token->row, String_wcsrtombs(token->value));
    }
}

List
Tokenizer_create_tokens(
    String filePath,
    Boolean is_main
)
{
    FILE* file = NULL;
    int line = 1;
    int indent_current = 0;
    List tokens = NULL;
    
    String buffer = NULL;
    List tmp = NULL;
    Boolean skip = FALSE;
    
    
    file = fopen(String_wcsrtombs(filePath), "r");
    if (file == NULL)
    {
        if (!is_main)
            goto END;

        fprintf(stderr, "File open error. [%s]\n\n", String_wcsrtombs(filePath));
        exit(0);
    }
    
    tokens = List_new();
    
    
    while (feof(file) == 0)
    {
        buffer = Tokenizer_read_line(file);
        
        
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
                fprintf(stderr, "[%s] line %3d : Indent level error.\n\n", String_wcsrtombs(filePath), line);
                goto END;
            }
        }
        
        skip = Tokenizer_check_join(tmp, line, wcslen(buffer));
        
        
        List_append(tokens, tmp);
        
        
        line++;
    }
    
    Tokenizer_terminate(tokens, indent_current, line);

END:
    if (file != NULL)
        fclose(file);
    
        
    return tokens;
}

/*
void
Tokenizer_reverce(String filePath, List tokens)
{
    char path[1024];
    
    memset(path, 0x00, sizeof(path));
    strcat(path, String_wcsrtombs(filePath));
    strcat(path, ".rev");
    FILE* file = fopen(path, "w");
    if (file == NULL)
    {
        Runtime_error("Reverced file open error.");
    }
    
    long count = List_count(tokens);
    long i = 0;
    for (i = 0; i < count; i++)
    {
    }
    
    fclose(file);
}
*/
