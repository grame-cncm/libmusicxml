/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_MSDL_MSDLPARSER_HPP_INCLUDED
# define YY_MSDL_MSDLPARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int msdldebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PARENTHESIZED_COMMENT = 258,   /* PARENTHESIZED_COMMENT  */
    COMMENT_TO_END_OF_LINE = 259,  /* COMMENT_TO_END_OF_LINE  */
    LEFT_BRACKET = 260,            /* LEFT_BRACKET  */
    RIGHT_BRACKET = 261,           /* RIGHT_BRACKET  */
    IDENTIFIER = 262,              /* IDENTIFIER  */
    INTEGER = 263,                 /* INTEGER  */
    DOUBLE = 264,                  /* DOUBLE  */
    SINGLE_QUOTED_STRING = 265,    /* SINGLE_QUOTED_STRING  */
    DOUBLE_QUOTED_STRING = 266,    /* DOUBLE_QUOTED_STRING  */
    SPACES = 267,                  /* SPACES  */
    END_OF_LINE = 268,             /* END_OF_LINE  */
    EQUAL_SIGN = 269,              /* EQUAL_SIGN  */
    COLON = 270,                   /* COLON  */
    SEMI_COLON = 271,              /* SEMI_COLON  */
    DOT = 272,                     /* DOT  */
    END_OF_BAR = 273,              /* END_OF_BAR  */
    OTHER_CHARACTER = 274          /* OTHER_CHARACTER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE msdllval;

int msdlparse (void);

#endif /* !YY_MSDL_MSDLPARSER_HPP_INCLUDED  */
