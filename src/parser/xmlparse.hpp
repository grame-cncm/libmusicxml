/* A Bison parser, made by GNU Bison 3.6.4.  */

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

#ifndef YY_LIBMXML_XMLPARSE_HPP_INCLUDED
# define YY_LIBMXML_XMLPARSE_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int libmxmldebug;
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
    XMLDECL = 258,                 /* XMLDECL  */
    VERSION = 259,                 /* VERSION  */
    ENCODING = 260,                /* ENCODING  */
    STANDALONE = 261,              /* STANDALONE  */
    YES = 262,                     /* YES  */
    NO = 263,                      /* NO  */
    ENDXMLDECL = 264,              /* ENDXMLDECL  */
    DOCTYPE = 265,                 /* DOCTYPE  */
    PUBLIC = 266,                  /* PUBLIC  */
    SYSTEM = 267,                  /* SYSTEM  */
    COMMENT = 268,                 /* COMMENT  */
    PI = 269,                      /* PI  */
    NAME = 270,                    /* NAME  */
    DATA = 271,                    /* DATA  */
    QUOTEDSTR = 272,               /* QUOTEDSTR  */
    SPACE = 273,                   /* SPACE  */
    LT = 274,                      /* LT  */
    GT = 275,                      /* GT  */
    ENDXMLS = 276,                 /* ENDXMLS  */
    ENDXMLE = 277,                 /* ENDXMLE  */
    EQ = 278                       /* EQ  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE libmxmllval;

int libmxmlparse (void);

#endif /* !YY_LIBMXML_XMLPARSE_HPP_INCLUDED  */
