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

#ifndef YY_PLPGSQL_YY_PL_GRAM_HPP_INCLUDED
# define YY_PLPGSQL_YY_PL_GRAM_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int plpgsql_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENT = 258,
    FCONST = 259,
    SCONST = 260,
    BCONST = 261,
    XCONST = 262,
    Op = 263,
    CmpOp = 264,
    COMMENTSTRING = 265,
    ICONST = 266,
    PARAM = 267,
    TYPECAST = 268,
    ORA_JOINOP = 269,
    DOT_DOT = 270,
    COLON_EQUALS = 271,
    PARA_EQUALS = 272,
    T_WORD = 273,
    T_CWORD = 274,
    T_DATUM = 275,
    T_PLACEHOLDER = 276,
    T_VARRAY = 277,
    T_ARRAY_FIRST = 278,
    T_ARRAY_LAST = 279,
    T_ARRAY_COUNT = 280,
    T_ARRAY_EXTEND = 281,
    T_VARRAY_VAR = 282,
    T_RECORD = 283,
    LESS_LESS = 284,
    GREATER_GREATER = 285,
    T_REFCURSOR = 286,
    T_SQL_ISOPEN = 287,
    T_SQL_FOUND = 288,
    T_SQL_NOTFOUND = 289,
    T_SQL_ROWCOUNT = 290,
    T_CURSOR_ISOPEN = 291,
    T_CURSOR_FOUND = 292,
    T_CURSOR_NOTFOUND = 293,
    T_CURSOR_ROWCOUNT = 294,
    K_ABSOLUTE = 295,
    K_ALIAS = 296,
    K_ALL = 297,
    K_ALTER = 298,
    K_ARRAY = 299,
    K_BACKWARD = 300,
    K_BEGIN = 301,
    K_BY = 302,
    K_CASE = 303,
    K_CLOSE = 304,
    K_COLLATE = 305,
    K_COMMIT = 306,
    K_CONSTANT = 307,
    K_CONTINUE = 308,
    K_CURRENT = 309,
    K_CURSOR = 310,
    K_DEBUG = 311,
    K_DECLARE = 312,
    K_DEFAULT = 313,
    K_DELETE = 314,
    K_DETAIL = 315,
    K_DIAGNOSTICS = 316,
    K_DUMP = 317,
    K_ELSE = 318,
    K_ELSIF = 319,
    K_END = 320,
    K_ERRCODE = 321,
    K_ERROR = 322,
    K_EXCEPTION = 323,
    K_EXECUTE = 324,
    K_EXIT = 325,
    K_FETCH = 326,
    K_FIRST = 327,
    K_FOR = 328,
    K_FORALL = 329,
    K_FOREACH = 330,
    K_FORWARD = 331,
    K_FROM = 332,
    K_GET = 333,
    K_GOTO = 334,
    K_HINT = 335,
    K_IF = 336,
    K_IMMEDIATE = 337,
    K_IN = 338,
    K_INFO = 339,
    K_INSERT = 340,
    K_INTO = 341,
    K_IS = 342,
    K_LAST = 343,
    K_LOG = 344,
    K_LOOP = 345,
    K_MERGE = 346,
    K_MESSAGE = 347,
    K_MESSAGE_TEXT = 348,
    K_MOVE = 349,
    K_NEXT = 350,
    K_NO = 351,
    K_NOT = 352,
    K_NOTICE = 353,
    K_NULL = 354,
    K_OF = 355,
    K_OPEN = 356,
    K_OPTION = 357,
    K_OR = 358,
    K_OUT = 359,
    K_PERFORM = 360,
    K_PG_EXCEPTION_CONTEXT = 361,
    K_PG_EXCEPTION_DETAIL = 362,
    K_PG_EXCEPTION_HINT = 363,
    K_PRAGMA = 364,
    K_PRIOR = 365,
    K_QUERY = 366,
    K_RAISE = 367,
    K_RECORD = 368,
    K_REF = 369,
    K_RELATIVE = 370,
    K_RESULT_OID = 371,
    K_RETURN = 372,
    K_RETURNED_SQLSTATE = 373,
    K_REVERSE = 374,
    K_ROLLBACK = 375,
    K_ROWTYPE = 376,
    K_ROW_COUNT = 377,
    K_SAVEPOINT = 378,
    K_SELECT = 379,
    K_SCROLL = 380,
    K_SLICE = 381,
    K_SQLSTATE = 382,
    K_STACKED = 383,
    K_STRICT = 384,
    K_SYS_REFCURSOR = 385,
    K_THEN = 386,
    K_TO = 387,
    K_TYPE = 388,
    K_UPDATE = 389,
    K_USE_COLUMN = 390,
    K_USE_VARIABLE = 391,
    K_USING = 392,
    K_VARIABLE_CONFLICT = 393,
    K_VARRAY = 394,
    K_WARNING = 395,
    K_WHEN = 396,
    K_WHILE = 397,
    K_WITH = 398
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 186 "gram.y" /* yacc.c:1909  */

        core_YYSTYPE			core_yystype;
        /* these fields must match core_YYSTYPE: */
        int						ival;
        char					*str;
        const char				*keyword;

        PLword					word;
        PLcword					cword;
        PLwdatum				wdatum;
        bool					boolean;
        Oid						oid;
        struct
        {
            char *name;
            int  lineno;
        }						varname;
        struct
        {
            char *name;
            int  lineno;
            PLpgSQL_datum   *scalar;
            PLpgSQL_rec		*rec;
            PLpgSQL_row		*row;
        }						forvariable;
        struct
        {
            char *label;
            int  n_initvars;
            int  *initvarnos;
            bool autonomous;
        }						declhdr;
        struct
        {
            List *stmts;
            char *end_label;
            int   end_label_location;
        }						loop_body;
        List					*list;
        PLpgSQL_type			*dtype;
        PLpgSQL_datum			*datum;
        PLpgSQL_var				*var;
        PLpgSQL_expr			*expr;
        PLpgSQL_stmt			*stmt;
        PLpgSQL_condition		*condition;
        PLpgSQL_exception		*exception;
        PLpgSQL_exception_block	*exception_block;
        PLpgSQL_nsitem			*nsitem;
        PLpgSQL_diag_item		*diagitem;
        PLpgSQL_stmt_fetch		*fetch;
        PLpgSQL_case_when		*casewhen;
        PLpgSQL_rec_attr	*recattr;

#line 252 "pl_gram.hpp" /* yacc.c:1909  */
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


extern THR_LOCAL YYSTYPE plpgsql_yylval;
extern THR_LOCAL YYLTYPE plpgsql_yylloc;
int plpgsql_yyparse (void);

#endif /* !YY_PLPGSQL_YY_PL_GRAM_HPP_INCLUDED  */
