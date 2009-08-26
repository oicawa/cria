#include "Cria.h"
#include "Memory.h"
#include "Logger.h"
#include "String.h"
#include "Definition.h"
#include "Interpreter.h"
#include "CriaBlock.h"



CriaBlock
CriaBlock_new(
    Interpreter interpreter,
    CriaId object,
    List parameterList,
    DefinitionFunction function
)
{
    Logger_trc("[ START ]%s", __func__);
    CriaBlock block = NULL;
    
    block = Memory_malloc(sizeof(struct CriaBlockTag));
    block->id.name = String_new("<<block>>");
    block->id.type = CRIA_DATA_TYPE_BLOCK;
    block->interpreter = interpreter;
    block->object = object;
    block->parameterList = parameterList;
    block->function = function;
    
    Logger_trc("[  END  ]%s", __func__);
    return block;
}


CriaId
CriaBlock_evaluate(
    CriaBlock block,
    List parameters
)
{
    CriaId id = NULL;
    Interpreter interpreter = block->interpreter;
    CriaId object = block->object;
    List parameterList = block->parameterList;
    DefinitionFunction function = block->function;
    
    
    //TODO: ここで、parameterListをスコープチェインのようにしたい〜。
    //＜本来やりたいこと＞
    //DefinitionFunction_evaluateの第３引数、parameterListを、
    //例えば「Scope」という構造体に変える。
    //このScope構造体は、メンバに「List parameterList」と「Scope parent」をもつ。
    //で、通常はこのparameterListを、従来のparameterListとして扱う。
    //今いるこの場所「CriaBlock_evaluate」は、「クロージャを実行する」場所であり、
    //ここでは新しい「List parameterList」を「block->function->of.cria->parameterList」から生成し、
    //今まで持ち回ってきたScopeを「Scope next」にポイントしてやる。
    //これでスコープチェインの出来上がり。
    //・・・
    //でも大手術すぎるので何ちゃって修正で回避する。どうせ書き直すし。
    //で、どうやるかというと、現在の親スコープであるparameterListに、
    //クロージャ用の新しいスコープを連結してしまう。
    //で、新しいスコープの大元になる引数定義は、block->function->of.cria->parameterListに存在している！
    //なので、これに格納されている変数定義（DefinitionVariableのリスト）に、
    //渡されてきたparametersのCriaIdオブジェクトをループ回してぶっこんでやる。
    //その後、親スコープのparameterListに、今ぶっこんだ「〜.cria->parameterList」を連結。
    //更に、ここがかなりウンコな実装になってしまうのだが、
    //その直後の「DefinitionFunction_evaluate内でparametersが再度ループされないためだけ」に、
    //第６引数に何も格納されていない、生成しただけのListオブジェクトをセットする。
    //・・・
    //まぁなんて醜いコードなのでしょう！
    List new_parameterList = DefinitionFunction_getParameterList(function);
    int i = 0;
    for (i = 0; i < List_count(parameters); i++)
    {
    	id = (CriaId)List_get(parameters, i);
    	DefinitionVariable definition = (DefinitionVariable)List_get(new_parameterList, i);
    	DefinitionVariable_set(definition, id);
    }
    parameterList = List_cat(parameterList, new_parameterList);
    
    id = DefinitionFunction_evaluate(interpreter, object, parameterList, NULL, function, List_new(), NULL);
    
    return id;
}

