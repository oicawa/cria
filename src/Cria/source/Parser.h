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

 
#ifndef PUBLIC_PARSER_H_INCLUDED
#define PUBLIC_PARSER_H_INCLUDED


#include "Cria.h"
#include "List.h"
#include "Tokenizer.h"
#include "Interpreter.h"



struct ParserTag
{
    Interpreter interpreter;
    List tokens;
    Item current;
    Item next;
    Item mark;
    String path;
};



Interpreter
Parser_getInterpreter(
    Parser parser
);



Token
Parser_getCurrent(
    Parser  parser
);



Item
Parser_getPosition(
    Parser  parser
);



void
Parser_setPosition(
    Parser  parser,
    Item    position
);



Boolean
Parser_next(
    Parser  parser
);



Boolean
Parser_eat(
	Parser parser,
	TokenType type,
	Boolean isNessesally
);



Boolean
Parser_create_syntax_tree(
    List tokens,
    Interpreter interpreter,
    String path
);



void
Parser_errorFunction(
    Parser parser,
	Token token,
	char* file,
	int line
);
#define Parser_error(parser, token) \
    Parser_errorFunction(parser, token, __FILE__, __LINE__)



void
Parser_insert(
    Parser parser,
    TokenType type
);



#endif
