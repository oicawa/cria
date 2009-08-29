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

 
#ifndef PUBLIC_HASH_H_INCLUDED
#define PUBLIC_HASH_H_INCLUDED


#include "Cria.h"
#include "List.h"



struct HashTag
{
	int size;
    int count;
    List keys;
    Entry* entrys;
};



struct EntryTag
{
    unsigned int hash_value;
    char* key;
    void* object;
    Entry next;
};



int
Hash_get_count(
	Hash hash
);



List
Hash_get_keys(
	Hash hash
);



void*
Hash_get(
	Hash hash,
	char* key
);



#define Hash_log_key(hash) \
	do { \
		int i = 0; \
		Logger_dbg("Hash keys:%p", hash); \
		for (i = 0; i < List_count(Hash_get_keys(hash)); i++) \
		{ \
			String name = List_get(Hash_get_keys(hash), i); \
			Logger_dbg(" >> %s", name); \
		} \
	} while(0) \



#endif
