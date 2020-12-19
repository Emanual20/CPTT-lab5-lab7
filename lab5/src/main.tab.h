/* A Bison parser, made by GNU Bison 3.5.1.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SRC_MAIN_TAB_H_INCLUDED
# define YY_YY_SRC_MAIN_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_CHAR = 258,
    T_INT = 259,
    T_STRING = 260,
    T_BOOL = 261,
    T_VOID = 262,
    KEY_IF = 263,
    KEY_ELSE = 264,
    KEY_FOR = 265,
    KEY_WHILE = 266,
    KEY_CONTINUE = 267,
    KEY_BREAK = 268,
    KEY_RETURN = 269,
    KEY_SCANF = 270,
    KEY_PRINTF = 271,
    KEY_CONST = 272,
    KEY_STRUCT = 273,
    KEY_PUBLIC = 274,
    KEY_PRIVATE = 275,
    LOP_POINT = 276,
    LOP_MINUS = 277,
    LOP_PLUS = 278,
    LOP_NOT = 279,
    LOP_MUL = 280,
    LOP_DIV = 281,
    LOP_MOD = 282,
    LOP_LESS = 283,
    LOP_GREA = 284,
    LOP_LE = 285,
    LOP_GE = 286,
    LOP_EEQ = 287,
    LOP_NEQ = 288,
    LOP_LAND = 289,
    LOP_LOR = 290,
    LOP_ASSIGN = 291,
    LOP_PLUSEQ = 292,
    LOP_MINUSEQ = 293,
    LOP_MODEEQ = 294,
    LOP_MULEQ = 295,
    LOP_DIVEQ = 296,
    LOP_COMMA = 297,
    LOP_LPAREN = 298,
    LOP_RPAREN = 299,
    LOP_LBRACE = 300,
    LOP_RBRACE = 301,
    SEMICOLON = 302,
    IDENTIFIER = 303,
    HEX_INTEGER = 304,
    DEC_INTEGER = 305,
    OCT_INTEGER = 306,
    CHAR = 307,
    BOOL = 308,
    STRING = 309,
    LOP_LBRKET = 310,
    LOP_RBRKET = 311,
    LOP_UMINUS = 312,
    LOP_UPLUS = 313,
    LOP_EQ = 314,
    LOP_MODEQ = 315
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_MAIN_TAB_H_INCLUDED  */
