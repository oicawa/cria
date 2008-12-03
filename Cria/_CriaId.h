#ifndef PRIVATE_CRIAID_H_INCLUDED
#define PRIVATE_CRIAID_H_INCLUDED


#include "_String.h"



typedef enum
{
    CRIA_DATA_TYPE_NULL,
    CRIA_DATA_TYPE_VOID,
    CRIA_DATA_TYPE_BOOLEAN,
    CRIA_DATA_TYPE_INTEGER,
    CRIA_DATA_TYPE_STRING,
	CRIA_DATA_TYPE_FILE,
    CRIA_DATA_TYPE_LIST,
    CRIA_DATA_TYPE_FUNCTION,
    CRIA_DATA_TYPE_CRIA_OBJECT,
    CRIA_DATA_TYPE_CRIA_FUNCTION,
} CriaDataType;



typedef struct CriaIdTag
{
    String          name;
    CriaDataType    type;
    int             refCount;
} *CriaId;



CriaId
CriaId_new(
    String          name,
    CriaDataType    type
);



#endif
