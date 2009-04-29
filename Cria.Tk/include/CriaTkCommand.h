#ifndef PUBLIC_CRIA_TK_H_INCLUDED
#define PUBLIC_CRIA_TK_H_INCLUDED


#include "CriaId.h"
#include "CriaObject.h"
#include "List.h"
#include "Interpreter.h"
#include "Definition.h"



enum TkCommandType
{
    TK_COMMAND_TYPE_FIELDS,
    TK_COMMAND_TYPE_COMMANDS,
}



struct TkCommandTag
{
    TkCommandType tyep;
    union
    {
        List fields;
        List commands;
    } of;
}


typedef TkCommandTag* TkCommand;
typedef TkCommand CreateCommands(Interpreter interpreter, CriaObject object);



#endif
