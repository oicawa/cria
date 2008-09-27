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
    token->type = type;
    token->row = row;
    token->column = column;
    token->buffer = buffer;
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
        Logger_err("File open error. (%s)", filePath);
        goto END;
    }
    tokenizer = Memory_malloc(sizeof(struct TokenizerTag));
    memset(tokenizer, 0x00, sizeof(struct TokenizerTag));
    tokenizer->file = file;
    tokenizer->row = 0;
    tokenizer->column = 0;
    tokenizer->next = '\0';
    tokenizer->buffer = NULL;
    tokenizer->error = NULL;
    tokenizer->indentLevel = 0;
    tokenizer->tokens = list_new();
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return tokenizer;
}



//Interpreterを破棄
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
    
    Logger_dbg("Check 'tokenizer->buffer'");
    if (tokenizer->buffer != NULL)
    {
        Logger_dbg("Free 'tokenizer->buffer'");
        stringBuffer_dispose(tokenizer->buffer);
        tokenizer->buffer = NULL;
    }
    
    Logger_dbg("Check 'tokenizer->error'");
    if (tokenizer->error != NULL)
    {
        Logger_dbg("Free 'tokenizer->error'");
        string_dispose(tokenizer->error);
        tokenizer->error = NULL;
    }
    
    
    Logger_dbg("Check 'tokeniser->tokens'");
    if (tokenizer->tokens != NULL)
    {
        Logger_dbg("Free 'tokenizer->tokens'");
        list_dispose(tokenizer->tokens);
        tokenizer->tokens = NULL;
    }
    
    
    Logger_dbg("Free 'tokenizer'");
    Memory_free(tokenizer);
    tokenizer = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
}



void
add(
    List    list,
    Token   token
)
{
    list_add(list, token);
}



//Fileの現在の文字を返す
Boolean
read(
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



Boolean
parseNumber(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Token token = NULL;
    tokenizer->buffer = stringBuffer_new();
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    //まずは内部バッファに登録
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    
    
    //次の文字を読み出す。
    Logger_dbg("Loop start.");
    while (read(tokenizer) == TRUE)
    {
        //数字以外の文字を読み込んだ場合は読み込み中止。
        if (isdigit(tokenizer->next) == 0)
        {
            Logger_dbg("Not number. (%c)", tokenizer->next);
            break;
        }
        //数字ならばバッファに追記。
        stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    }
    Logger_dbg("Loop end.");
    
    
    //トークン登録
    String tmp = stringBuffer_toString(tokenizer->buffer);
    Logger_dbg("Add token. (%s)", tmp->pointer);
    token = token_new(TOKEN_TYPE_INTEGER_LITERAL, row, column, tmp);
    add(tokenizer->tokens, token);
    
    
    //使用したバッファを開放
    stringBuffer_dispose(tokenizer->buffer);
    tokenizer->buffer = NULL;
    
    Logger_trc("[  END  ]%s", __func__);
    return TRUE;
}



Boolean
parseReserved(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    String  tmp = NULL;
    char*   buffer = NULL;
    Token   token = NULL;
    int    row = tokenizer->row;
    int    column = tokenizer->column;
    
    
    //※！！通常の識別子の可能性があるので、一文字先読みをバッファに入れてはダメ！！
    //　まずは手元の文字列から確認すること！！
    
    //現在のバッファ内の文字列を取得し、予約語に合致した場合はそれでトークンを生成
    tmp = stringBuffer_toString(tokenizer->buffer);
    buffer = tmp->pointer;
    if (strcmp(buffer, TOKEN_LITERAL_NULL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_NULL");
        token = token_new(TOKEN_TYPE_NULL, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_BREAK) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_BREAK");
        token = token_new(TOKEN_TYPE_BREAK, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_CATCH) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_CATCH");
        token = token_new(TOKEN_TYPE_CATCH, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_ELSE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_ELSE");
        token = token_new(TOKEN_TYPE_ELSE, row, column, NULL);
    }
    else if (strcmp(buffer, TOKEN_LITERAL_RETURN) == 0)
    {
        //retrunについては、値を返すケースである、RETURN_VALUEがありうるので、
        //次の文字が' 'でなかった場合のみ、トークンを生成する。
        //' 'だった場合は後述のRETURN_VALUEで引っ掛ける。
        if (tokenizer->next != ' ')
        {
            Logger_dbg("create TOKEN_TYPE_RETURN");
            token = token_new(TOKEN_TYPE_RETURN, row, column, tmp);
            goto ADD_TOKEN;
        }
    }
    else if (strcmp(buffer, TOKEN_LITERAL_FINALLY) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_FINALLY");
        token = token_new(TOKEN_TYPE_FINALLY, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_CONTINUE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_CONTINUE");
        token = token_new(TOKEN_TYPE_CONTINUE, row, column, tmp);
        goto ADD_TOKEN;
    }
    
    
    //次の１文字が半角スペースでなかった場合は予約語ではない。
    if (tokenizer->next != ' ')
    {
        Logger_dbg("It is not reserved word.");
        goto END;
    }
    
    
    //次の１文字が半角スペースであった場合は予約語の可能性があるのでチェック。
    if (strncmp(buffer, TOKEN_LITERAL_IF, strlen(TOKEN_LITERAL_IF) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_IF");
        token = token_new(TOKEN_TYPE_IF, row, column, tmp);
    }
    else if (strncmp(buffer, TOKEN_LITERAL_FOR, strlen(TOKEN_LITERAL_FOR) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_FOR");
        token = token_new(TOKEN_TYPE_FOR, row, column, tmp);
    }
    else if (strncmp(buffer, TOKEN_LITERAL_ELIF, strlen(TOKEN_LITERAL_ELIF) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_ELIF");
        token = token_new(TOKEN_TYPE_ELIF, row, column, tmp);
    }
    else if (strncmp(buffer, TOKEN_LITERAL_WHILE, strlen(TOKEN_LITERAL_WHILE) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_WHILE");
        token = token_new(TOKEN_TYPE_WHILE, row, column, tmp);
    }
    else if (strncmp(buffer, TOKEN_LITERAL_THROW, strlen(TOKEN_LITERAL_THROW) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_THROW");
        token = token_new(TOKEN_TYPE_THROW, row, column, tmp);
    }
    else if (strncmp(buffer, TOKEN_LITERAL_RETURN_VALUE, strlen(TOKEN_LITERAL_RETURN_VALUE) - 1) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_RETURN_VALUE");
        token = token_new(TOKEN_TYPE_RETURN_VALUE, row, column, tmp);
    }
    else
    {
        //予約語でなかった場合はスペース追加前の文字列を識別子トークンを生成
        Logger_dbg("Not reserved word with space. (%s)", tmp->pointer);
        goto END;
    }
    
    //予約語だった場合は、今読んだ文字をクリアしてからトークン追加処理へ
    if (token != NULL)
    {
        tokenizer->next = '\0';
        goto ADD_TOKEN;
    }
    
    goto ADD_TOKEN;
    
    
ADD_TOKEN:
    //トークンの登録
    add(tokenizer->tokens, token);
    result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
    
}



Boolean
parseVariableOrFunction(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    Token   token = NULL;
    int    row = tokenizer->row;
    int    column = tokenizer->column;
    String  tmp = NULL;
    char*   buffer = NULL;

    
    //現状でのバッファ内文字列を取得。
    tmp = stringBuffer_toString(tokenizer->buffer);
    buffer = tmp->pointer;
    
    
    //先頭の文字列は小文字でなければ対象外。
    if ((*buffer < 'a') || ('z' < *buffer))
    {
        Logger_dbg("First charactor is not lower alphabet. ('%c')", *buffer);
        goto END;
    }


    //２文字以降はアルファベットまたは数字であることをチェック
    buffer += 1;
    while (*buffer != '\0')
    {
        //文字がアルファベットまたは数字、またはアンダーバーでない場合は対象外
        if (isalnum(*buffer) == 0)
        {
            Logger_dbg("Not alphabet or underbar.");
            goto END;
        }

        //次の文字へ        
        buffer += 1;
    }

    //トークンを生成して登録
    token = token_new(TOKEN_TYPE_IDENTIFIER, row, column, tmp);
    add(tokenizer->tokens, token);
    result = TRUE;
    
END:
    //返却
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
parseClassLiteralOrConstant(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    Token   token = NULL;
    int    row = tokenizer->row;
    int    column = tokenizer->column;
    String  tmp = NULL;
    char*   buffer = NULL;

    
    //現状でのバッファ内文字列を取得。
    TokenType type = TOKEN_TYPE_CONSTANT;
    tmp = stringBuffer_toString(tokenizer->buffer);
    buffer = tmp->pointer;
    
    
    //文字列長が２文字未満の場合は対象外
    if (strlen(buffer) < 2)
    {
        Logger_dbg("String length is less than 2.");
        goto END;
    }
    
    
    //先頭の文字列は大文字でなければ対象外。
    if ((*buffer < 'A') || ('Z' < *buffer))
    {
        Logger_dbg("First charactor is not upper alphabet. ('%c')", *buffer);
        goto END;
    }

    //２文字目が大文字の場合は定数、小文字の場合はクラスとしてチェック。
    buffer += 1;
    if ((('A' <= *buffer) && (*buffer <= 'Z')) || (*buffer == '_'))
    {
        type = TOKEN_TYPE_CONSTANT;
    }
    else if (('a' <= *buffer) && (*buffer <= 'z'))
    {
        type = TOKEN_TYPE_CLASS_LITERAL;
    }
    else
    {
        Logger_dbg("It is not a Class literal or Constant.");
        goto END;
    }
    
    //以降の文字のチェック
    buffer += 1;
    while (*buffer != '\0')
    {
        //文字がアルファベットでも数字でもアンダーバーでもない場合は対象外
        if ((isalnum(*buffer) == 0) && (*buffer != '_'))
        {
            Logger_dbg("Not alphabet or underbar. (%c)", *buffer);
            goto END;
        }
        
        //クラスリテラルなのに、アンダーバーが出現した場合は対象外。
        if ((type == TOKEN_TYPE_CLASS_LITERAL) && (*buffer == '_'))
        {
            Logger_dbg("Not class literal.");
            goto END;
        }
        
        //定数リテラルなのに、小文字が出現した場合は対象外。
        if ((type == TOKEN_TYPE_CONSTANT) && (('a' <= *buffer) && (*buffer <= 'z')))
        {
            Logger_dbg("Not constant literal.");
            goto END;
        }

        //次の文字へ        
        buffer += 1;
    }

    //ここまできた場合は前半でチェックした識別子（クラスリテラル、または定数リテラル）という認識でOK。
    //トークンを生成して登録
    token = token_new(type, row, column, tmp);
    add(tokenizer->tokens, token);
    result = TRUE;
    
END:
    //返却
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
parseIdentifier(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    tokenizer->buffer = stringBuffer_new();
    
    
    //まずは内部バッファに登録
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    
    
    //次の文字を読み出す。
    Logger_dbg("read next charactor");
    while (read(tokenizer) == TRUE)
    {
        //アルファベット、または数字だった場合は追記して継続
        Logger_dbg("Is alphabet or number?");
        if ((isalnum(tokenizer->next) != 0)
            || tokenizer->next == '_')
        {
            //アルファベット、または数字ならばバッファに追記して継続
            stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
            continue;
        }
        //アルファベット、数字、アンダーバー以外だった場合は識別子以外のゾーンに入ったということなのでブレーク。
        Logger_dbg("next is not alphabet, number, or underbar. '%c'", tokenizer->next);
        break;
    }
    
    
    //ここで、「予約語」「変数名または関数名」「定数名」「クラス名」の判別を行う。
    if (parseReserved(tokenizer) == TRUE)
    {
        Logger_dbg("created reserved word.");
        result = TRUE;
    }
    else if (parseVariableOrFunction(tokenizer) == TRUE)
    {
        Logger_dbg("created variable or function identifier.");
        result = TRUE;
    }
    else if (parseClassLiteralOrConstant(tokenizer) == TRUE)
    {
        Logger_dbg("created class literal or constant literal.");
        result = TRUE;
    }
    else
    {
        //エラー
        Logger_err("this is not any identifier or reserved word.");
        result = FALSE;
    }
    
    //使用したバッファを開放
    stringBuffer_dispose(tokenizer->buffer);
    tokenizer->buffer = NULL;
    
    //返却
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
skipSpaceAndNewLine(
    Tokenizer   tokenizer
)
{
    Boolean result = FALSE;
    
    while (TRUE)
    {
        if (read(tokenizer) == FALSE)
        {
            Logger_dbg("No next charactor.");
            result = FALSE;
            break;
        }
        if (tokenizer->next != '\n' && tokenizer->next != ' ')
        {
            Logger_dbg("Analysis next token.");
            result = TRUE;
            break;
        }
    }
    return result;
}



Boolean
parseSpace(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    Token token = NULL;
    tokenizer->buffer = stringBuffer_new();
    String tmp = NULL;
    char* buffer = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    //まずは内部バッファに登録
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    Logger_dbg("tokenizer->next = '%c'", tokenizer->next);
    
    
    //--------------------------------------------------
    //２文字目
    //--------------------------------------------------
    if (read(tokenizer) == FALSE)
    {
        Logger_dbg("No 2nd charactor.");
        goto NO_TOKEN;
    }
    Logger_dbg("2nd charactor = '%c'", tokenizer->next);
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    tmp = stringBuffer_toString(tokenizer->buffer);
    if (tokenizer->next == '_')
    {
        Logger_dbg("skip connection literal.");
        //改行、空白意外が出現するまで読み飛ばし
        result = skipSpaceAndNewLine(tokenizer);
        goto NO_TOKEN;
    }
    
    
    //--------------------------------------------------
    //３文字目
    //--------------------------------------------------
    Logger_dbg("read 3rd charactor");
    if (read(tokenizer) == FALSE)
    {
        Logger_dbg("No 3rd charactor.");
        goto NO_TOKEN;
    }
    Logger_dbg("3rd charactor = '%c'", tokenizer->next);
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    tmp = stringBuffer_toString(tokenizer->buffer);
    buffer = tmp->pointer;
    if (strcmp(buffer, TOKEN_LITERAL_SUBSTITUTE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_SUBSTITUTE");
        token = token_new(TOKEN_TYPE_SUBSTITUTE, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_PLUS) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_PLUS");
        token = token_new(TOKEN_TYPE_PLUS, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_MINUS) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_MINUS");
        token = token_new(TOKEN_TYPE_MINUS, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_MULTIPLY) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_MULTIPLY");
        token = token_new(TOKEN_TYPE_MULTIPLY, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_DEVIDE) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_DEVIDE");
        token = token_new(TOKEN_TYPE_DEVIDE, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_MODULO) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_MODULO");
        token = token_new(TOKEN_TYPE_MODULO, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_LESS_THAN) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_LESS_THAN");
        token = token_new(TOKEN_TYPE_LESS_THAN, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_GREATER_THAN) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_GREATER_THAN");
        token = token_new(TOKEN_TYPE_GREATER_THAN, row, column, tmp);
        goto END;
    }
    
    
    //使用した一時領域を開放
    string_dispose(tmp);
    tmp = NULL;
    buffer = NULL;
    
    
    //--------------------------------------------------
    //４文字目
    //--------------------------------------------------
    Logger_dbg("read 4th charactor");
    if (read(tokenizer) == FALSE)
    {
        Logger_dbg("No 4th charactor.");
        goto NO_TOKEN;
    }
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    tmp = stringBuffer_toString(tokenizer->buffer);
    buffer = tmp->pointer;
    if (strcmp(buffer, TOKEN_LITERAL_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_EQUAL");
        token = token_new(TOKEN_TYPE_EQUAL, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_INCREMENT) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_INCREMENT");
        token = token_new(TOKEN_TYPE_INCREMENT, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_DECREMENT) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_DECREMENT");
        token = token_new(TOKEN_TYPE_DECREMENT, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_LESS_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_LESS_EQUAL");
        token = token_new(TOKEN_TYPE_LESS_EQUAL, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_NOT_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_NOT_EQUAL");
        token = token_new(TOKEN_TYPE_NOT_EQUAL, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_OR) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_OR");
        token = token_new(TOKEN_TYPE_OR, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_AND) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_AND");
        token = token_new(TOKEN_TYPE_AND, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_GREATER_EQUAL) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_GREATER_EQUAL");
        token = token_new(TOKEN_TYPE_GREATER_EQUAL, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_EXTENDS) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_EXTENDS");
        token = token_new(TOKEN_TYPE_EXTENDS, row, column, tmp);
        goto END;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_IN) == 0)
    {
        Logger_dbg("create TOKEN_TYPE_IN");
        token = token_new(TOKEN_TYPE_IN, row, column, tmp);
        goto END;
    }
    else
    {
        Logger_dbg("No token. (%s)", buffer);
        goto NO_TOKEN;
    }
    
END:
    //一時読み込み文字をリセット
    tokenizer->next = '\0';
    add(tokenizer->tokens, token);
    result = TRUE;
    
    
NO_TOKEN:
    
    //使用したバッファを開放
    stringBuffer_dispose(tokenizer->buffer);
    tokenizer->buffer = NULL;
    
    //返却
    Logger_dbg("result = %d", result);
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
parseStringLiteral(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    Token token = NULL;
    String tmp = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    //読み込みループ
    Logger_dbg("tokenizer->next = '%c'",  tokenizer->next);
    while (read(tokenizer) == TRUE)
    {
        //まずは読んだ文字列を追記
        stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
        
        
        //「\」（エスケープシーケンス）だった場合は、
        //もう一文字読み込み、チェックせずにバッファに登録
        if (tokenizer->next == '\\')
        {
            if (read(tokenizer) == FALSE)
            {
                Logger_dbg("No string literal");
                goto NO_TOKEN;
            }
            stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
            continue;
        }
        
        
        //終端の「"」だった場合はトークンを生成してループを抜ける。
        if (tokenizer->next == '"')
        {
            tmp = stringBuffer_toString(tokenizer->buffer);
            Logger_dbg("string literal = [%s]", tmp->pointer);
            token = token_new(TOKEN_TYPE_STRING_LITERAL, row, column, tmp);
            tokenizer->next = '\0';
            goto END;
        }
    }
    
    
END:
    add(tokenizer->tokens, token);
    result = TRUE;
    
    
NO_TOKEN:
    
    //返却
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
parseIndentDedent(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = FALSE;
    Token token = NULL;
    String tmp = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    tokenizer->buffer = stringBuffer_new();
    
    
    //読み込みループ
    Logger_dbg("[Before read] tokenizer->next = '%c'", tokenizer->next);
    Logger_dbg("Loop start.");
    while (read(tokenizer) == TRUE)
    {
        Logger_dbg("tokenizer->next = '%c'", tokenizer->next);
        
        //' 'だった場合は追記して継続
        if (tokenizer->next == ' ')
        {
            Logger_dbg("Add space charactor.");
            stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
            continue;
        }
        
        //'\n'だった場合はバッファをクリアして次の行のチェックを開始。
        Logger_dbg("Check new line charactor.");
        if (tokenizer->next == '\n')
        {
            Logger_dbg("New line charactor is found.");
            stringBuffer_dispose(tokenizer->buffer);
            tokenizer->buffer = stringBuffer_new();
            continue;
        }
        
        //上記いずれでも無かった場合はループを抜け、インデント・ディデントトークンを生成する。
        Logger_dbg("Not space.");
        result = TRUE;
        break;
    }
    Logger_dbg("Loop end.");
    
    
    //文字列抽出インデント数のチェック
    tmp = stringBuffer_toString(tokenizer->buffer);
    long length = string_length(tmp);
    Logger_dbg("Space length = %d", length);
    if (length % 4 != 0)
    {
        Logger_dbg("Indent is not 4 times spaces. (modulo = %d)", length % 4);
        goto END;
    }
    
    
    //インデントレベルのチェック
    Logger_dbg("Indent level = %d.", length / 4);
    int which = (length / 4) - tokenizer->indentLevel;
    Logger_dbg("which = %d.", which);
    int i = 0;
    if (which < 0)
    {
        which = which * (-1);
        for (i = 0; i < which; i++)
        {
            Logger_dbg("create TOKEN_TYPE_DEDENT");
            tokenizer->indentLevel -= 1;
            token = token_new(TOKEN_TYPE_DEDENT, row, column, string_new("<<DEDENT>>"));
            add(tokenizer->tokens, token);
        }
        result = TRUE; 
    }
    else if (which == 1)
    {
        Logger_dbg("create TOKEN_TYPE_INDENT");
        tokenizer->indentLevel += 1;
        token = token_new(TOKEN_TYPE_INDENT, row, column, string_new("<<INDENT>>"));
        add(tokenizer->tokens, token);
        result = TRUE; 
    }
    else if (which == 0)
    {
        //インデントなし。
        Logger_dbg("Indent level is same above line.");
        result = TRUE; 
    }
    else
    {
        //一度に複数のインデントは発生してはいけない。
        Logger_dbg("Multi indent is not permited.");
        result = FALSE; 
    }
    
    
END:
    
    //返却
    Logger_dbg("result = %d", result);
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
parseOther(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    //初期化
    Boolean result = TRUE;
    Token token = NULL;
    tokenizer->buffer = stringBuffer_new();
    String tmp = NULL;
    char* buffer = NULL;
    int row = tokenizer->row;
    int column = tokenizer->column;
    
    
    //--------------------------------------------------
    //１文字目
    //--------------------------------------------------
    Logger_trc("Check first charactor.(%c)", tokenizer->next);
    stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
    tmp = stringBuffer_toString(tokenizer->buffer);
    buffer = tmp->pointer;
    if (strcmp(buffer, TOKEN_LITERAL_PARENTHESIS_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_PARENTHESIS_LEFT'");
        token = token_new(TOKEN_TYPE_PARENTHESIS_LEFT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_PARENTHESIS_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_PARENTHESIS_RIGHT'");
        token = token_new(TOKEN_TYPE_PARENTHESIS_RIGHT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_BRACKET_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACKET_LEFT'");
        token = token_new(TOKEN_TYPE_BRACKET_LEFT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_BRACKET_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACKET_RIGHT'");
        token = token_new(TOKEN_TYPE_BRACKET_RIGHT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_BRACE_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACE_LEFT'");
        token = token_new(TOKEN_TYPE_BRACE_LEFT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_BRACE_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_BRACE_RIGHT'");
        token = token_new(TOKEN_TYPE_BRACE_RIGHT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_GENERICS_LEFT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_GENERICS_LEFT'");
        token = token_new(TOKEN_TYPE_GENERICS_LEFT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_GENERICS_RIGHT) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_GENERICS_RIGHT'");
        token = token_new(TOKEN_TYPE_GENERICS_RIGHT, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_COLON) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_COLON'");
        token = token_new(TOKEN_TYPE_COLON, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_MEMBER) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_MEMBER'");
        token = token_new(TOKEN_TYPE_MEMBER, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_PERIOD) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_PERIOD'");
        token = token_new(TOKEN_TYPE_PERIOD, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_MONADIC_MINUS) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_MONADIC_MINUS'");
        token = token_new(TOKEN_TYPE_MONADIC_MINUS, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (strcmp(buffer, TOKEN_LITERAL_UNDER) == 0)
    {
        Logger_trc("Create 'TOKEN_TYPE_UNDER'");
        result = skipSpaceAndNewLine(tokenizer);
        goto END;
    }
    else if (tokenizer->next == ',')
    {
        Logger_trc("Try comma literal.");
        //コンマの場合はその次の文字もチェック
        if (read(tokenizer) == FALSE)
        {
            result = FALSE;
            goto END;
        }
        
        if (tokenizer->next != ' ')
        {
            //リセット
            result = FALSE;
            tokenizer->next = '\0';
            goto END;
        }

        string_dispose(tmp);
        tmp = NULL;
        stringBuffer_appendChar(tokenizer->buffer, tokenizer->next, __FILE__, __LINE__);
        tmp = stringBuffer_toString(tokenizer->buffer);
        Logger_trc("Create 'TOKEN_TYPE_COMMA'");
        token = token_new(TOKEN_TYPE_COMMA, row, column, tmp);
        goto ADD_TOKEN;
    }
    else if (tokenizer->next == '"')
    {
        //文字列リテラルの解析
        Logger_trc("Try string literal.");
        result = parseStringLiteral(tokenizer);
        goto END;
    }
    else if (tokenizer->next == '\n')
    {
        //改行トークンを生成
        Logger_trc("Create 'TOKEN_TYPE_NEW_LINE'");
        string_dispose(tmp);
        tmp = string_new("<<NEW_LINE>>");
        token = token_new(TOKEN_TYPE_NEW_LINE, row, column, tmp);
        add(tokenizer->tokens, token);
        stringBuffer_dispose(tokenizer->buffer);
        tokenizer->buffer = NULL;
        
        //インデント・ディデントの解析
        Logger_trc("Try indent/dedent literal.");
        result = parseIndentDedent(tokenizer);
        goto END;
    }
    
    
    //何れにも合致しなかった場合はエラー
    result = FALSE;
    goto END;
    
    
ADD_TOKEN:
    //リセット
    tokenizer->next = '\0';
    add(tokenizer->tokens, token);
    result = TRUE;
    
    
END:
    //使用したバッファを開放
    stringBuffer_dispose(tokenizer->buffer);
    tokenizer->buffer = NULL;
    
    //返却
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



Boolean
isEndOfFile(
    Tokenizer   tokenizer
)
{
    if (feof(tokenizer->file) == 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



void
token_log(
    Token token
)
{
    char* buffer = NULL;
    
    if (token == NULL)
    {
        Logger_dbg("Token type=NULL.");
        return;
    }
    
    if (token->buffer != NULL)
        buffer = token->buffer->pointer;
    
    Logger_dbg("Token type=%2d(%3d, %2d) [%s]", token->type, token->row, token->column, buffer);
}



void
logAllTokens(
    Tokenizer   tokenizer
)
{
    long index = 0;
    long count = tokenizer->tokens->count;
    
    
    while (index < count)
    {
        Token token = (Token)list_get(tokenizer->tokens, index);
        token_log(token);
        index += 1;
    }
}



Boolean
tokenizer_parse(
    Tokenizer   tokenizer
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    
    
    while (TRUE)
    {
        if (isEndOfFile(tokenizer) == TRUE)
        {
            Logger_dbg("Reached at the end of file.");
            result = TRUE;
            break;
        }
        
        
        //未読み込み状態ならば一文字読み込み。読み込めなかった場合はNULLを返却
        if (tokenizer->next == '\0')
        {
            if (read(tokenizer) == FALSE)
            {
                Logger_dbg("Reached at the end of file.");
                result = TRUE;
                goto END;
            }
        }
        
        //読み込んだ文字で適切なパーサーをコール
        if (isdigit(tokenizer->next) != 0)
        {
            //数字で始まるトークンの解析
            result = parseNumber(tokenizer);
        }
        else if (isalpha(tokenizer->next) != 0)
        {
            //アルファベットで始まるトークンの解析
            result = parseIdentifier(tokenizer);
        }
        else if (tokenizer->next == ' ')
        {
            //スペースで始まるトークンの解析
            result = parseSpace(tokenizer);
        }
        else
        {
            //上記以外の記号で始まるトークンの解析
            result = parseOther(tokenizer);
        }
        
        
        if (result == FALSE)
        {
            goto END;
        }
    }
    
    
END:
    //トークンを全て出力
    logAllTokens(tokenizer);
    
    Logger_trc("result = %d", result);
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



