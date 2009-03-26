#include "Memory.h"
#include "Logger.h"

#include "Definition.h"
#include "CriaIO.h"
#include "CriaFile.h"
#include "CriaList.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Statement.h"

#include "_Interpreter.h"

void
Interpreter_addFunction(
    Interpreter interpreter,
    char* functionName,
    CriaNativeFunction* functionPoint
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionFunction definition = NULL;
    definition = DefinitionFunction_new(functionName, TRUE, TRUE, NULL, NULL, functionPoint);
    Hash_put(interpreter->functions, functionName, definition);
    Logger_trc("[  END  ]%s", __func__);
}



void
Interpreter_addClass(
    Interpreter interpreter,
    char* className,
    CriaNativeClassLoader* classLoader
)
{
    Logger_trc("[ START ]%s", __func__);
    DefinitionClass definition = NULL;
    definition = DefinitionClass_new(className, TRUE, NULL, NULL, NULL, NULL, classLoader);
    Hash_put(interpreter->classes, className, definition);
    Logger_trc("[  END  ]%s", __func__);
}



void
Interpreter_loadCore(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    Interpreter_addFunction(interpreter, "write", CriaIO_write);
    Interpreter_addFunction(interpreter, "read", CriaIO_read);
    Interpreter_addClass(interpreter, "File", CriaFile_loadClass);
    Interpreter_addClass(interpreter, "List", CriaList_loadClass);
    Logger_trc("[  END  ]%s", __func__);
}



void
Interpreter_load_imports(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    
    //TODO: [2009/03/26] パッケージの概念追加によってInterpreter構造体の再設計が必要？
    //現状では、インタープリタの構造体自身に、
    //------------------------------
    //・グローバル変数
    //・関数
    //・クラス
    //------------------------------
    //の登録を行っているが、パッケージの概念を取り入れる場合、
    //インタープリタ自身に登録しない方がよい気がする。
    //
    //と言うのは、今まで通りの構成だと何が問題になるかというと、
    //Parserが構文解析する段階で、そのクラスや関数が、
    //どのパッケージに所属するかまで解析しなければならなくなるから。
    //
    //つまり、ソースコード上は"List()"と記載されていても、構文解析の段階で、
    //"Cria::Core::List()"と解釈しておかなければならなくなってしまう。）
    
    //ではどのような構成がよいか。
    //例えば、Package」構造体をInterpreterに複数登録可にして、
    //上記の３要素はこのPackageに追加するという方法。
    //
    //そうすると、実行時にどのパッケージに存在しているかをさがしに行くような形になる。
    //必要に応じて、構文オブジェクトに、どこのパッケージのクラス、関数なのかという情報を付加し、
    //次回呼び出し時の速度を早めてもいいと思う。
    //
    //なにより実装がイメージしやすい。→とにかく動くものが作れる。これ大事。
    //
    //★問題点★
    //別パッケージで同じ名称のクラスや関数があった場合、
    //競合してしまう。これは実行時にアナウンスしたいなぁ。
    //でないと、最初に見つかったクラス、関数を問答無用で呼び出すことになってしまう。。
    //
    //あ、そうすると、いままで通りの構成にしておいて、
    //関数やクラスに「どこのパッケージに所属」って情報を付加しておいた方がいい？もしかして。
    //関数、クラスのオブジェクトは、今Hashに格納する形にしている。
    //で、キーに名前、値に実際のオブジェクトで登録しているけど、
    //これを値にHashを登録させて、そのHashにはキーにパッケージ名、
    //値に実際のオブジェクト、つー構成にしたらどうだろう。
    //そうすりゃクラスや関数が競合してても分かるぞ。
    //あ、これいいかもしれない。こうしようかな。
    //
    //この方針でちょっと考察してみよう。
    //
    //[前提]
    //まず、import宣言で指定する値は、パッケージ名のみ。
    //つまり、Javaのように使用するクラス名のフルパスとは根本的に違う。
    //理由は、Cのライブラリとしてパッケージをロードしたい場合、
    //どうせメモリに全部がロードされるから。一部だけのロードってできないっしょ。いや詳しく知らんけど。
    //あと、こういう仕様にしておけば実装が楽だし。
    //と言うわけで、この仕様は変更しない。小技は不要。
    //
    //[構文解析時]
    //さて、ではパーシングするときは何をすればよいか。
    //必ずInterpreterオブジェクトを引き連れて、
    //該当するimport構文を確認したらinterpreterオブジェクトに登録していくだけ。
    //importしたパッケージが再度importされそうになったら無視。
    //
    //・・・でも、importしたパッケージがさらに別のパッケージをimportしていた場合ってどうするんだ？？
    //importオブジェクトを入れ子構造にすべきなのか？頭こんがらがってきた。
    //もし入れ子構造にしないのであれば、それはそれで楽だけど、
    //先にどこかでimportされていたら、パッケージ内でimport宣言してなくても使えちゃうってことだし、
    //そうすると、別のパッケージで同名の関数やクラスが存在したら、意図したのとは別の関数・クラスを
    //使用するという恐ろしい可能性があるなぁ。
    //
    //これはまずい。
    //これを解決するには多分、
    //--------------------
    //１．ライブラリ毎に独立したロードエリアが必要。
    //２．パーシング時に、クラスや関数の名称を、
    //　　そのライブラリ中で宣言されているimportパッケージ名を含めたフルパスに変換する必要がある。
    //３．そうすると、ライブラリの依存関係とか考えると、気が遠くなるね。
    //--------------------
    //
    //並列な感じで構文解析したいなら、構文解析中にロード処理を行う必要がある。
    //これは避けられないかもな〜。
    //そうすると、宣言した側で使われているクラスや関数が、
    //importによってロードされたライブラリ内に存在するかどうかチェックができる。
    //というかどうしてもしなければならない。
    //そのかわり、このフェーズで苦労しておけば、実行時に苦労しなくてすむし、
    //どのクラスを使用しているのかは明確になる。
    //criaで開発している最中におかしな不具合が発生しても、criaの処理系の思想自体はシンプルに保っておけるので、
    //原因の解析は楽かも。
    //今こういう感じに処理系の実装がイメージできているのが何よりの証かもなぁ。
    //
    //ということで、importつーより、loadっていう予約語の方がしっくりくるかも。
    //むしろ関数化すべき？
    //いやいや、好きなところでloadされるのはちょっと見た目どうなのよ。
    //でも関数化しておいて、構文的にそれを許さないようにしておけばいいか。
    //
    //話を元に戻すと、構文解析時にloadを見つけた時点でその後の構文解析のために、
    //指定されたライブラリをメモリ上にロードする必要がある。
    //ここだけは実行時ロードってわけにはいかないな。
    //まぁでも考えてみたら構文解析は解析しながら関数やクラスをロードしてるわけだから仕方ないか。
    //よし、この方針で実装していってみよう。
    
    Interpreter_addFunction(interpreter, "write", CriaIO_write);
    Interpreter_addFunction(interpreter, "read", CriaIO_read);
    Interpreter_addClass(interpreter, "File", CriaFile_loadClass);
    Interpreter_addClass(interpreter, "List", CriaList_loadClass);
    Logger_trc("[  END  ]%s", __func__);
}



Interpreter
Interpreter_new(
    void
)
{
    Interpreter interpreter = NULL;
    
    interpreter = Memory_malloc(sizeof(struct InterpreterTag));
    interpreter->statements = List_new();
    Logger_dbg("interpreter->statements is [%p]", interpreter->statements);
    interpreter->imports = Hash_new(16);
    interpreter->variables = Hash_new(32);
    interpreter->functions = Hash_new(64);
    interpreter->classes = Hash_new(64);
    interpreter->row = 0;
    interpreter->column = 0;
    interpreter->indentLevel = 0;
    
    return interpreter;
}



Boolean
Interpreter_compile(
    Interpreter interpreter,
    char* filePath
)
{
    Logger_trc("[ START ]%s", __func__);
    Boolean result = FALSE;
    List tokens = NULL;
    
    tokens = Tokenizer_create_tokens(filePath);
    
    if (Parser_create_syntax_tree(tokens, interpreter) == FALSE)
    {
        Logger_err("syntax parse error.");
        goto END;
    }
    
    result = TRUE;
    
END:
    Logger_trc("[  END  ]%s", __func__);
    return result;
}



void
Interpreter_run(
    Interpreter interpreter
)
{
    Logger_trc("[ START ]%s", __func__);
    
    Interpreter_loadCore(interpreter);
    Interpreter_load_imports(interpreter);
    
    Statement_executeList(interpreter, NULL, NULL, interpreter->statements);
    
    Logger_trc("[  END  ]%s", __func__);
}



int
Interpreter_row(
	Interpreter interpreter
)
{
	return interpreter->row;
}



Hash
Interpreter_classes(
	Interpreter interpreter
)
{
	return interpreter->classes;
}



Hash
Interpreter_functions(
	Interpreter interpreter
)
{
	return interpreter->functions;
}



Hash
Interpreter_variables(
	Interpreter interpreter
)
{
	return interpreter->variables;
}



List
Interpreter_statements(
	Interpreter interpreter
)
{
	return interpreter->statements;
}



void
Interpreter_setRow(
	Interpreter interpreter,
	int row
)
{
	interpreter->row = row;
}



Hash
Interpreter_get_imports(
    Interpreter interpreter
)
{
    return interpreter->imports;
}
