/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_REPLICATION_YY_REPL_GRAM_HPP_INCLUDED
# define YY_REPLICATION_YY_REPL_GRAM_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int replication_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SCONST = 258,
    IDENT = 259,
    RECPTR = 260,
    ICONST = 261,
    K_BASE_BACKUP = 262,
    K_IDENTIFY_SYSTEM = 263,
    K_IDENTIFY_VERSION = 264,
    K_IDENTIFY_MODE = 265,
    K_IDENTIFY_MAXLSN = 266,
    K_IDENTIFY_CONSISTENCE = 267,
    K_IDENTIFY_CHANNEL = 268,
    K_IDENTIFY_AZ = 269,
    K_LABEL = 270,
    K_PROGRESS = 271,
    K_FAST = 272,
    K_NOWAIT = 273,
    K_BUILDSTANDBY = 274,
    K_WAL = 275,
    K_TABLESPACE_MAP = 276,
    K_DATA = 277,
    K_START_REPLICATION = 278,
    K_FETCH_MOT_CHECKPOINT = 279,
    K_ADVANCE_REPLICATION = 280,
    K_CREATE_REPLICATION_SLOT = 281,
    K_DROP_REPLICATION_SLOT = 282,
    K_ADVANCE_CATALOG_XMIN = 283,
    K_PHYSICAL = 284,
    K_LOGICAL = 285,
    K_SLOT = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 60 "repl_gram.y" /* yacc.c:1909  */

		replication_scanner_YYSTYPE yy_core;
		char					*str;
		bool					boolval;
		int						ival;

		XLogRecPtr				recptr;
		Node					*node;
		List					*list;
		DefElem					*defelt;

#line 98 "repl_gram.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int replication_yyparse (replication_scanner_yyscan_t yyscanner);

#endif /* !YY_REPLICATION_YY_REPL_GRAM_HPP_INCLUDED  */
