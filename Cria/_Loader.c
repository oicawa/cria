#include <unistd.h>
#include <memory.h>

#include "Memory.h"
#include "Logger.h"

#include "String.h"
#include "StringBuffer.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Parser.h"

#include "_Loader.h"


Loader
Loader_parse(
    Parser parser
)
{
    Logger_trc("[ START ]%s", __func__);
    Loader loader = NULL;
    Item position = Parser_getPosition(parser);
    Token token = NULL;
    StringBuffer package_name = NULL;
    StringBuffer library_name = NULL;
    
    Logger_dbg("Check TOKEN_TYPE_LOAD.");
    token = Parser_getCurrent(parser);
    if (Token_type(token) != TOKEN_TYPE_LOAD)
    {
        Logger_dbg("Not load token.");
        Parser_setPosition(parser, position);
        goto END;
    }
    
    
    package_name = StringBuffer_new();
    library_name = StringBuffer_new();
    while (TRUE)
    {
        Logger_dbg("Check package field");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) != TOKEN_TYPE_CLASS_LITERAL)
        {
            Logger_dbg("Not load token.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        StringBuffer_append(package_name, Token_buffer(token));
        StringBuffer_append(library_name, Token_buffer(token));
        
        
        Logger_dbg("Check colon or NEW_LINE");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) == TOKEN_TYPE_NEW_LINE)
        {
            break;
        }
        else if (Token_type(token) != TOKEN_TYPE_COLON)
        {
            Logger_dbg("Not colon token.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        Logger_dbg("Check second colon");
        Parser_next(parser);
        token = Parser_getCurrent(parser);
        if (Token_type(token) != TOKEN_TYPE_COLON)
        {
            Logger_dbg("Not colon token.");
            Parser_setPosition(parser, position);
            Parser_error(token);
            goto END;
        }
        
        
        StringBuffer_append(package_name, "::");
        StringBuffer_append(library_name, ".");
    }
    
    
    Parser_next(parser);
    
    
    Logger_dbg("Create 'Loader'");
    loader = Memory_malloc(sizeof(struct LoaderTag));
    loader->package_name = StringBuffer_toString(package_name);
    loader->library_name = StringBuffer_toString(library_name);
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return loader;
}



String
Loader_get_package_name(
    Loader loader
)
{
    return loader->package_name;
}


void
Loader_load(
    Loader loader,
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    char buffer[MAX_PATH_LENGTH];
    StringBuffer cria_home_path = StringBuffer_new();
    StringBuffer current_path = StringBuffer_new();
    
    
    //カレントディレクトリのパスを取得
    memset(buffer, 0x00, MAX_PATH_LENGTH);
    getcwd(buffer, MAX_PATH_LENGTH);
    StringBuffer_append(current_path, buffer);
    StringBuffer_append(current_path, "/");
    StringBuffer_append(current_path, loader->library_name);
    printf("*** [%s] ***\n", StringBuffer_toString(current_path));
    
    
    //実行ファイルが存在するパスを取得
    //（※これで実行ファイルのフルパスは取得できるので、
    //　　あとはこのフルパスからディレクトリパスだけ取得すればいい。）
    memset(buffer, 0x00, MAX_PATH_LENGTH);
    readlink("/proc/self/exe", buffer, MAX_PATH_LENGTH);
    

    //ここにディレクトリパスだけ取得する処理を。。
    size_t length = strlen(buffer);
    char* pointer = strrchr(buffer, '/');
    size_t index = pointer - buffer;
    memset(pointer + 1, 0x00, length - index);
    
    
    StringBuffer_append(cria_home_path, buffer);
    StringBuffer_append(cria_home_path, loader->library_name);
    printf("*** [%s] ***\n", StringBuffer_toString(cria_home_path));
    
    
    //★問題発生★
    //$PATHに、criaの実行ファイルのありかを追加して、
    //任意のディレクトリからcriaを実行しようとすると、
    //libcria.so共有ライブラリのリンクに失敗する。
    //で、これをcriaを実行した時のカレントディレクトリに置くと正常に実行ができる。
    //どうやらコンパイル時の-rpathオプションは、
    //実行時の「カレントディレクトリ」に対するライブラリのありかを指定しているらしい。
    //実行時に「実行ファイルと同じディレクトリ」にあるライブラリをリンクさせるオプションはないものか。。
    //
    //！！あったっぽい！！
    //ldコマンドに、「-Wl,-rpath,$ORIGIN」つーオプションを指定するとまさにやりたいことができるらしい。
    //んが、試してみたらダメだった。指定するのはリンクのときだよな〜。なんでダメなんだろう。。
    //「-z origin」つーオプションもあるみたいなので試してみたけどダメだった。
    //もちっとネットで調査が必要かな。
    //
    //もしないようなら、現在libcria.soとしているライブラリも、
    //実行時にdlopenとかdlclose使ってダイナミックにロードするしかないなぁ。
    //でも絶対オプションでできるはず。。
    //
    //このこだわりはcriaのコンセプトとしては非常に重要なのです。
    //インストールしてアーカイブを展開したらそのまま使えるというのが。
    //環境変数の設定したくないし。
    //あ、でもPATHのどっかにcria実行ファイルへのリンクを作る必要があるか。。
    
    
    //ライブラリの存在チェック。
    
    //dlopen(
    
    
    
    
//END:
    Logger_trc("[  END  ]%s", __func__);
    return;
}

