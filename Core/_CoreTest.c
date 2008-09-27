//svn checkout https://stla.googlecode.com/svn/trunk/ stla --username oicawa
#include <stdio.h>
#include <stdlib.h>
#include "Core.h"


void Test_IO();
void Test_Integer();
void Test_String_List();
void Test_StringBuffer();
void Test_File();


int main(int argc, char ** argv)
{
    Logger_set(LOG_LEVEL_INFORMATION, NULL);
    
    //Test_IO();
    
    
    //Test_Integer();
    
    
    //Logger_set(LOG_LEVEL_DEBUG, NULL);
    //Test_String_List();
    
    
    Logger_set(LOG_LEVEL_CORE, "stringBuffer_appendChar,stringBuffer_append");
    Test_StringBuffer();
    
    
    //Test_File();
    
    
    
    
    
    
    printf("---Memory dump blocks --------------------------------------\n");
    Memory_dumpBlocks(stdout);
    printf("---Memory_dispose();----------------------------------------\n");
    Memory_dispose();
    Memory_dumpBlocks(stdout);
    return 0;
}

void Test_IO()
{
    //*
    printf("\n\n=== <IO_write> =============================================\n");
    String value0 = String_new("hogehoge");
    Integer value1 = Integer_new("725");
    Integer value2 = Integer_new("726");
    Integer value3 = Integer_new("727");
    //★初回呼び出しで引数不足だとSegmentation faultが発生。一度正しい値でセットするとOKみたいだが。。理由は不明。
    Native_print("[Test] %s\n", value0);
    Native_print("[Test] %s %s\n", value0, value1);
    Native_print("[Test] %s %s %s\n", value0, value1, value2);
    Native_print("[Test] %s %s %s %s\n", value0, value1, value2, value3);
    
    String_dispose(value0);
    Integer_dispose(value1);
    Integer_dispose(value2);
    Integer_dispose(value3);
    
    Memory_dumpBlocks(stdout);
    printf("!!! Memory_dispose() !!!\n");
    Memory_dispose();
    //*/
}




void TestInteger()
{
    //*
    printf("\n\n=== <Integer> ==============================================\n");
    
    
    printf("--- integer0:Integer = 123 ---------------------------------\n");
    Integer integer0 = Integer_new("123");
    Native_print("integer0 = %s\n", integer0);
    
    
    printf("--- integer1:Integer = 0 -----------------------------------\n");
    Integer integer1 = Integer_new("0");
    Native_print("integer1 = %s\n", integer1);
    
    
    printf("--- integer2:Integer = -5 ----------------------------------\n");
    Integer integer2 = Integer_new("-5");
    Native_print("integer2 = %s\n", integer2);
    
    
    printf("--- integer3:Integer = integer0 ----------------------------\n");
    Integer integer3 = Integer_clone(integer0);
    Native_print("integer3 = %s\n", integer3);
    

    printf("--- integer3 = integer2 ------------------------------------\n");
    Integer_dispose(integer3);
    integer3 = Integer_clone(integer2);
    Native_print("integer3 = %s\n", integer3);
    
    
    printf("--- integer4:Integer = integer2 + integer0 -----------------\n");
    Integer integer4 = Integer_plus(integer2, integer0);
    Native_print("integer4 = %s\n", integer4);
    
    
    printf("--- integer5:Integer = integer4 * integer0 -----------------\n");
    Integer integer5 = Integer_multiply(integer4, integer0);
    Native_print("integer5 = %s\n", integer5);
    
    
    printf("--- integer6:Integer = integer5 / integer2 -----------------\n");
    Integer integer6 = Integer_divide(integer5, integer2);
    Native_print("integer6 = %s\n", integer6);
    
    
    printf("--- integer7:Integer = integer5 %% integer2 ----------------\n");
    Integer integer7 = Integer_modulo(integer5, integer2);
    Native_print("integer7 = %s\n", integer7);
    
    
    Integer_dispose(integer0);
    Integer_dispose(integer1);
    Integer_dispose(integer2);
    Integer_dispose(integer3);
    Integer_dispose(integer4);
    Integer_dispose(integer5);
    Integer_dispose(integer6);
    Integer_dispose(integer7);
    
    Memory_dumpBlocks(stdout);
    printf("!!! Memory_dispose() !!!\n");
    Memory_dispose();
}



void Test_String_List()
{
    //*
    printf("\n\n=== <String> ===============================================\n");
    
    
    printf("--- string0:String = \"hogehoge,fugafuga\" -----------------\n");
    String string0 = String_new("hogehoge,fugafuga");
    Integer string0length = String_length(string0);
    Native_print("string0 = '%s', length = %s\n", string0, string0length);


    printf("--- string1:String = \",mogamoga,ponyoponyo\" --------------\n");
    String string1 = String_new(",mogamoga,ponyoponyo");
    Integer string1length = String_length(string1);
    Native_print("string1 = '%s', length = %s\n", string1, string1length);


    printf("--- string2:String = string0 -------------------------------\n");
    String string2 = String_clone(string0);
    Integer string2length = String_length(string2);
    Native_print("string2 = '%s', length = %s\n", string2, string2length);
    
    
    printf("--- string2 = string1 --------------------------------------\n");
    String_dispose(string2);
    string2 = String_clone(string1);
    Integer_dispose(string2length);
    string2length = String_length(string2);
    Native_print("string2 = '%s', length = %s\n", string2, string2length);
    
    
    printf("--- string3:String = string0 + string1 ---------------------\n");
    String string3 = String_plus(string0, string1);
    Integer string3length = String_length(string3);
    Native_print("string3 = '%s', length = %s\n", string3, string3length);
    
    
    printf("--- index0:Integer = string3.find(\",\", 0) ----------------\n");
    printf("--- index1:Integer = string3.find(\",\", index0 + 1) -------\n");
    printf("--- index2:Integer = string3.find(\",\", index1 + 1) -------\n");
    printf("--- index3:Integer = string3.find(\",\", index2 + 1) -------\n");
    Integer const_1 = Integer_new("1");
    String constComma = String_new(",");
    
    Integer offset0 = Integer_new("0");
    Integer index0 = String_find(string3, constComma, offset0);
    Native_print("offset0 = %s / index0 = %s\n", offset0, index0);

    Integer offset1 = Integer_plus(index0, const_1);
    Integer index1 = String_find(string3, constComma, offset1);
    Native_print("offset1 = %s / index1 = %s\n", offset1, index1);
    
    Integer offset2 = Integer_plus(index1, const_1);
    Integer index2 = String_find(string3, constComma, offset2);
    Native_print("offset2 = %s / index2 = %s\n", offset2, index2);
    
    Integer offset3 = Integer_plus(index2, const_1);
    Integer index3 = String_find(string3, constComma, offset3);
    Native_print("offset3 = %s / index3 = %s\n", offset3, index3);
    
    
    printf("--- fields:List = string3.split(\",\") ------------------\n");
    String separator = String_new(",");
    List fields = String_split(string3, separator);
    Native_print("fields = %s\n", fields);
    //ここでメモリリークが発生するのは仕方がない。。
    //公開関数のみのインタフェースでは、Integerをロングに変換できないので、
    //ループをまわすこともできない状態。
    //やるとするならば非公開関数を駆使したItem全てを開放できる
    //List_disposeAll（仮）メソッドを実装してそれを呼び出すとか。かな。
    //とりあえずこのテストプログラムでは後始末することで対応しよう。
    
    
    
    printf("--- sub0:String = string3.subString(offset0, index0) -------\n");
    printf("--- sub1:String = string3.subString(offset1, index1) -------\n");
    printf("--- sub2:String = string3.subString(offset2, index2) -------\n");
    printf("--- sub3:String = string3.subString(offset3, index3) -------\n");
    Integer size0 = Integer_minus(index0, offset0);
    String sub0 = String_subString(string3, offset0, size0);
    Native_print("sub0 = %s\n", sub0);
    
    Integer size1 = Integer_minus(index1, offset1);
    String sub1 = String_subString(string3, offset1, size1);
    Native_print("sub1 = %s\n", sub1);
    
    Integer size2 = Integer_minus(index2, offset2);
    String sub2 = String_subString(string3, offset2, size2);
    Native_print("sub2 = %s\n", sub2);
    
    Integer size3 = Integer_minus(index3, offset3);
    String sub3 = String_subString(string3, offset3, size3);
    Native_print("sub3 = %s / offset3 = %s / size3 = %s\n", sub3, offset3, size3);
    
    
    
    
    printf("\n\n=== <List> =================================================\n");
    
    
    printf("--- list0:List = List() ------------------------------------\n");
    printf("--- list0.add(sub0) ----------------------------------------\n");
    List list0 = List_new();
    List_add(list0, sub0);
    Integer list0count = List_count(list0);
    Native_print("list0 count = %s\n", list0count);
    
    printf("--- list0.add(sub1) ----------------------------------------\n");
    List_add(list0, sub1);
    Integer_dispose(list0count);
    list0count = List_count(list0);
    Native_print("list0 count = %s\n", list0count);

    printf("--- list0.add(sub2) ----------------------------------------\n");
    List_add(list0, sub2);
    Integer_dispose(list0count);
    list0count = List_count(list0);
    Native_print("list0 count = %s\n", list0count);
    Integer_dispose(list0count);
    
    
    Integer list0index0 = Integer_new("0");
    String item0 = (String)List_get(list0, list0index0);
    Native_print("list0[%s] = %s\n", list0index0, item0);
    
    Integer list0index1 = Integer_new("1");
    String item1 = (String)List_get(list0, list0index1);
    Native_print("list0[%s] = %s\n", list0index1, item1);
    
    Integer list0index2 = Integer_new("2");
    String item2 = (String)List_get(list0, list0index2);
    Native_print("list0[%s] = %s\n", list0index2, item2);

    Integer list0index3 = Integer_new("3");
    String item3 = (String)List_get(list0, list0index3);
    Native_print("list0[%s] = %s\n", list0index3, item3);
    
    
    printf("--- list1:List = List() ------------------------------------\n");
    printf("--- list1.add(sub2) ----------------------------------------\n");
    List list1 = List_new();
    List_add(list1, sub2);
    Integer list1count = List_count(list1);
    Native_print("list1 count = %s\n", list1count);
    
    printf("--- list1.add(size1) ---------------------------------------\n");
    List_add(list1, size1);
    Integer_dispose(list1count);
    list1count = List_count(list1);
    Native_print("list1 count = %s\n", list1count);

    printf("--- list1.add(list0) ---------------------------------------\n");
    List_add(list1, list0);
    Integer_dispose(list1count);
    list1count = List_count(list1);
    Native_print("list1 count = %s\n", list1count);
    Integer_dispose(list1count);
    
    Integer list1index0 = Integer_new("0");
    Object object = List_get(list1, list1index0);
    Native_print("list1[%s] = %s\n", list1index0, object);
    
    Integer list1index1 = Integer_new("1");
    object = List_get(list1, list0index1);
    Native_print("list1[%s] = %s\n", list1index1, object);
    
    //Logger_set(LOG_LEVEL_DEBUG, NULL);
    Integer list1index2 = Integer_new("2");
    object = List_get(list1, list1index2);
    Native_print("list1[%s] = %s\n", list1index2, object);

    Integer list1index3 = Integer_new("3");
    object = List_get(list1, list1index3);
    Native_print("list1[%s] = %s\n", list1index3, object);

    Native_print("list1     = %s\n", list1);
    
    
    String_dispose(string0);
    String_dispose(string1);
    String_dispose(string2);
    String_dispose(string3);
    Integer_dispose(string0length);
    Integer_dispose(string1length);
    Integer_dispose(string2length);
    Integer_dispose(string3length);
    Integer_dispose(const_1);
    String_dispose(constComma);
    Integer_dispose(index0);
    Integer_dispose(index1);
    Integer_dispose(index2);
    Integer_dispose(index3);
    Integer_dispose(offset0);
    Integer_dispose(offset1);
    Integer_dispose(offset2);
    Integer_dispose(offset3);
    String_dispose(separator);
    List_dispose(fields);
    Integer_dispose(size0);
    Integer_dispose(size1);
    Integer_dispose(size2);
    Integer_dispose(size3);
    String_dispose(sub0);
    String_dispose(sub1);
    String_dispose(sub2);
    String_dispose(sub3);
    
    Integer_dispose(list0index0);
    Integer_dispose(list0index1);
    Integer_dispose(list0index2);
    Integer_dispose(list0index3);
    
    Integer_dispose(list1index0);
    Integer_dispose(list1index1);
    Integer_dispose(list1index2);
    Integer_dispose(list1index3);
    
    List_dispose(list0);
    List_dispose(list1);
}



void Test_StringBuffer()
{
    printf("\n\n=== <StringBuffer> =========================================\n");
    
    
    printf("--- buffer:StringBuffer = StringBuffer() -----------------------\n");
    printf("--- buffer.append(\"field0,\") ---------------------------------\n");
    printf("--- buffer.append(\"field1,\") ---------------------------------\n");
    printf("--- buffer.append(\"field2,\") ---------------------------------\n");
    printf("--- buffer.toString() ------------------------------------------\n");
    String field0 = String_new("field0,");
    String field1 = String_new("field1,");
    String field2 = String_new("fild2");
    StringBuffer buffer = StringBuffer_new();
    StringBuffer_append(buffer, field0);
    StringBuffer_append(buffer, field1);
    StringBuffer_append(buffer, field2);
    String result = StringBuffer_toString(buffer);
    Native_print("buffer = [%s]\n", result);
    
    String_dispose(field0);
    String_dispose(field1);
    String_dispose(field2);
    String_dispose(result);
    StringBuffer_dispose(buffer);
    
    
    printf("--- bufferEmpty:StringBuffer = StringBuffer() -----------------------\n");
    printf("--- [Native]StringBuffer_dispose() ----------------------------------\n");
    StringBuffer bufferEmpty = StringBuffer_new();
    StringBuffer_dispose(bufferEmpty);
}



void Test_File()
{
    printf("\n\n=== <File> =================================================\n");
    
    
    printf("--- file0:File = File(\"../../little.txt\", READ) ------------\n");
    File file0 = File_new("../../little.txt", FILE_OPEN_MODE_READ);
    if (file0 != NULL)
    {
        printf(">>>\n");
        String charactor = NULL;
        //Logger_set(LOG_LEVEL_DEBUG, NULL);
        while ((charactor = File_read(file0)) != NULL)
        {
            //IO_writeNative("%s", charactor);
            String_dispose(charactor);
        }
        printf("<<<\n");
        File_dispose(file0);
    }
    
    printf("--- file1:File = File(\"../Sample2.s\", READ) ------------\n");
    File file1 = File_new("../Sample2.s", FILE_OPEN_MODE_READ);
    if (file1 != NULL)
    {
        printf(">>>\n");
        String file1Line = NULL;
        
        
        Logger_set(LOG_LEVEL_INFORMATION, NULL);
        //Logger_set(LOG_LEVEL_DEBUG, NULL);
        /*
        while ((file1Line = File_readLine(file1)) != NULL)
        {
            String message = NULL;
            if (File_isEndOfFile(file1) == TRUE)
            {
                message = String_new("[EOF]");
            }
            else
            {
                message = String_new("[   ]");
            }
            IO_writeNative("%s%s", message, file1Line);
            String_dispose(file1Line);
            String_dispose(message);
        }
        */
        while (File_isEndOfFile(file1) == FALSE)
        {
            String message = NULL;
            if (File_isEndOfFile(file1) == TRUE)
            {
                message = String_new("[EOF]");
            }
            else
            {
                message = String_new("[   ]");
            }
            if ((file1Line = File_readLine(file1)) == NULL)
            {
                break;
            }
            Native_print("%s%s", message, file1Line);
            String_dispose(file1Line);
            String_dispose(message);
        }
        
        if (File_isEndOfFile(file1) == TRUE)
        {
            Native_print(">>> File is 'EOF'\n");
        }
        else
        {
            Native_print(">>> File is NOT 'EOF'\n");
        }
        printf("[EOF]\n");
        File_dispose(file1);
    }
}



