/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/main.y"

    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );

#line 79 "src/main.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  206

#define YYUNDEFTOK  2
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    41,    41,    43,    44,    50,    51,    56,    59,    62,
      68,    78,    88,   100,   106,   116,   126,   134,   144,   152,
     163,   174,   188,   192,   196,   201,   211,   217,   223,   228,
     232,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   253,   254,   258,   269,   280,   289,   301,   320,   341,
     342,   351,   361,   368,   379,   384,   387,   394,   397,   400,
     407,   414,   424,   427,   434,   441,   451,   454,   465,   468,
     476,   487,   490,   501,   504,   515,   518,   529,   532,   543,
     546,   555,   556,   557,   562,   593,   598,   606,   610,   614,
     618,   625,   629,   637,   642,   645,   646,   647,   648,   651,
     652,   660,   661,   662,   665,   666,   667,   668,   671,   672,
     675,   676,   677,   678,   679
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_CHAR", "T_INT", "T_STRING", "T_BOOL",
  "T_VOID", "KEY_IF", "KEY_ELSE", "KEY_FOR", "KEY_WHILE", "KEY_CONTINUE",
  "KEY_BREAK", "KEY_RETURN", "KEY_SCANF", "KEY_PRINTF", "KEY_CONST",
  "KEY_STRUCT", "KEY_PUBLIC", "KEY_PRIVATE", "LOP_POINT", "LOP_MINUS",
  "LOP_PLUS", "LOP_NOT", "LOP_MUL", "LOP_DIV", "LOP_MOD", "LOP_LESS",
  "LOP_GREA", "LOP_LE", "LOP_GE", "LOP_EEQ", "LOP_NEQ", "LOP_LAND",
  "LOP_LOR", "LOP_ASSIGN", "LOP_PLUSEQ", "LOP_MINUSEQ", "LOP_MODEEQ",
  "LOP_MULEQ", "LOP_DIVEQ", "LOP_COMMA", "LOP_LPAREN", "LOP_RPAREN",
  "LOP_LBRACE", "LOP_RBRACE", "SEMICOLON", "IDENTIFIER", "HEX_INTEGER",
  "DEC_INTEGER", "OCT_INTEGER", "CHAR", "BOOL", "STRING", "LOP_LBRKET",
  "LOP_RBRKET", "LOP_UMINUS", "LOP_UPLUS", "LOP_EQ", "LOP_MODEQ",
  "$accept", "Program", "CompUnits", "CompUnit", "decl", "constdeclstmt",
  "constdefitem", "constinitval", "vardeclstmt", "declareitem",
  "assignstmt", "funcdef", "funcparams", "funcparam", "block", "blocklist",
  "statement", "funccall", "scanfstmt", "printfstmt", "spflist", "forstmt",
  "returnstmt", "whilestmt", "ifstmt", "expr", "PrimaryExp", "LValExp",
  "UnaryExp", "MulExp", "AddExp", "RelExp", "EqExp", "LandExp", "LorExp",
  "CommaExp", "INTEGER", "structdeclstmt", "structdeclist",
  "structdecitem", "structinitlist", "structinititem", "T", "AutT",
  "MulOp", "RelOp", "EqOp", "AssignEqOp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315
};
# endif

#define YYPACT_NINF (-137)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     165,  -137,  -137,  -137,  -137,   -29,   141,     0,    51,   165,
    -137,   -24,    43,    53,  -137,  -137,    48,    61,    64,    68,
    -137,  -137,  -137,    64,    69,   -22,  -137,   141,    86,  -137,
     160,  -137,   112,  -137,   105,   141,    88,  -137,    93,   105,
    -137,  -137,   104,   134,    56,  -137,   165,   105,   105,   105,
     131,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,    73,
      21,   145,    22,   155,   156,  -137,    91,   141,   147,  -137,
    -137,   156,  -137,  -137,   148,  -137,   150,   151,  -137,  -137,
    -137,   152,  -137,  -137,  -137,   105,   105,   105,  -137,  -137,
    -137,  -137,   105,  -137,  -137,   105,   105,   105,   147,  -137,
      76,  -137,  -137,   153,  -137,  -137,  -137,  -137,  -137,    73,
      73,    21,   145,    22,   155,  -137,   158,   159,   161,   162,
     163,   105,   164,   168,  -137,   172,   166,   167,  -137,    23,
    -137,  -137,   169,   170,  -137,   171,  -137,  -137,    78,    69,
     148,   105,     4,   105,  -137,  -137,   156,   157,   140,   149,
     173,  -137,  -137,  -137,   175,  -137,  -137,  -137,  -137,   105,
    -137,  -137,  -137,  -137,  -137,   105,  -137,   116,   -27,   176,
     142,   105,   177,   178,  -137,  -137,   156,   156,    95,   105,
     105,    95,   156,   105,   105,   196,   -25,   -17,  -137,   143,
    -137,   146,    95,   179,   179,   105,  -137,  -137,  -137,   180,
     181,  -137,    95,    95,  -137,  -137
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    96,    95,    98,    97,     0,     0,     0,     0,     2,
       3,     0,     8,     7,     6,     9,     0,     0,     0,     0,
       1,     4,     5,     0,     0,    17,    14,    24,     0,    10,
       0,    11,    17,    15,     0,    24,     0,    22,     0,     0,
      99,   100,     0,     0,     0,    85,     0,     0,     0,     0,
      55,    81,    82,    83,    58,    59,    16,    62,    66,    68,
      71,    73,    75,    77,    54,    57,     0,     0,     0,    25,
      12,    13,    87,    88,    94,    86,     0,     0,    64,    63,
      65,     0,   101,   102,   103,     0,     0,     0,   104,   105,
     106,   107,     0,   108,   109,     0,     0,     0,     0,    23,
       0,    21,    93,    84,    91,    89,    90,    56,    67,    70,
      69,    72,    74,    76,    78,    20,     0,     0,     0,     0,
       0,    49,     0,     0,    31,    60,     0,     0,    32,     0,
      28,    40,     0,     0,    34,     0,    35,    33,     0,     0,
      94,     0,     0,     0,    38,    37,    79,    50,     0,     0,
       0,    27,    39,    26,     0,    30,    41,    42,    36,     0,
     110,   111,   113,   114,   112,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    61,    29,    18,    19,     0,     0,
       0,     0,    80,     0,     0,    52,     0,     0,    51,     0,
      45,     0,     0,     0,     0,     0,    43,    44,    53,     0,
       0,    46,     0,     0,    47,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,  -137,   197,   -28,  -137,   185,  -137,    70,   191,
    -136,    90,   193,   174,   -56,  -137,  -125,  -137,  -137,  -137,
      42,  -137,  -137,  -137,  -137,   -33,  -137,  -137,    -2,   -21,
     137,   135,   136,   138,   -34,  -130,  -137,  -137,  -137,   187,
    -137,    94,    -3,  -137,  -137,  -137,  -137,  -137
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    11,    12,    29,    70,    13,    26,
     127,    14,    36,    37,   128,   129,   130,   131,   132,   133,
     189,   134,   135,   136,   137,   190,    57,   138,    58,    59,
      60,    61,    62,    63,   146,   147,    65,    15,    44,    45,
     103,   104,    16,    46,    85,    92,    95,   165
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      64,    56,    42,    18,   155,    71,   169,     1,     2,     3,
       4,   167,   101,   170,    34,    24,    42,   171,    76,    17,
     179,    35,   193,    22,    38,   171,     1,     2,     3,     4,
     194,   116,    38,   117,   118,   119,   120,   121,   122,   123,
       6,     7,   115,    86,    87,    78,    79,    80,    19,   186,
     187,    20,   125,   185,    93,    94,   188,   199,   200,     1,
       2,     3,     4,     5,    38,   109,   110,   198,   100,   153,
     124,   125,   126,     6,     7,    40,    41,   204,   205,     1,
       2,     3,     4,   108,   116,    23,   117,   118,   119,   120,
     121,   122,   123,     6,     7,    24,    25,   139,    82,    83,
      84,   154,    74,   116,    27,   117,   118,   119,   120,   121,
     122,   123,    28,    30,   159,   160,   161,    32,   162,   163,
      43,   100,    39,   124,   125,   176,   139,    47,    48,    49,
      67,   177,    68,    67,    43,    98,    77,   182,   164,   139,
     100,    69,   124,   125,     1,     2,     3,     4,    34,    64,
      64,    72,    81,    50,    51,    52,    53,    54,   171,    55,
     178,    64,   201,     1,     2,     3,     4,     5,     1,     2,
       3,     4,     5,    88,    89,    90,    91,     6,     7,    40,
      41,    73,     6,     7,   171,   195,   181,   196,   195,    96,
     197,    97,   100,   150,   172,   140,   102,   105,   106,   171,
     107,   141,   142,   173,   143,   192,    21,   148,    31,   144,
     145,   149,   168,   151,   152,    33,   156,   157,   158,   183,
     184,   174,   175,   180,   202,   203,   191,   125,    66,   111,
     112,    75,   113,     0,   166,   114,     0,     0,     0,     0,
       0,    99
};

static const yytype_int16 yycheck[] =
{
      34,    34,    30,     6,   129,    39,   142,     3,     4,     5,
       6,   141,    68,   143,    36,    42,    44,    42,    46,    48,
      47,    43,    47,    47,    27,    42,     3,     4,     5,     6,
      47,     8,    35,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    98,    22,    23,    47,    48,    49,    48,   179,
     180,     0,    48,   178,    32,    33,   181,   193,   194,     3,
       4,     5,     6,     7,    67,    86,    87,   192,    45,    46,
      47,    48,   100,    17,    18,    19,    20,   202,   203,     3,
       4,     5,     6,    85,     8,    42,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    42,    48,   100,    25,    26,
      27,   129,    46,     8,    43,    10,    11,    12,    13,    14,
      15,    16,    48,    45,    36,    37,    38,    48,    40,    41,
      30,    45,    36,    47,    48,   159,   129,    22,    23,    24,
      42,   165,    44,    42,    44,    44,    46,   171,    60,   142,
      45,    48,    47,    48,     3,     4,     5,     6,    36,   183,
     184,    47,    21,    48,    49,    50,    51,    52,    42,    54,
      44,   195,   195,     3,     4,     5,     6,     7,     3,     4,
       5,     6,     7,    28,    29,    30,    31,    17,    18,    19,
      20,    47,    17,    18,    42,    42,    44,    44,    42,    34,
      44,    35,    45,    21,    54,    42,    48,    47,    47,    42,
      48,    43,    43,    54,    43,     9,     9,    43,    23,    47,
      47,    43,   142,    47,    47,    24,    47,    47,    47,    42,
      42,    48,    47,    47,    44,    44,   184,    48,    35,    92,
      95,    44,    96,    -1,   140,    97,    -1,    -1,    -1,    -1,
      -1,    67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    17,    18,    62,    63,
      64,    65,    66,    69,    72,    98,   103,    48,   103,    48,
       0,    64,    47,    42,    42,    48,    70,    43,    48,    67,
      45,    67,    48,    70,    36,    43,    73,    74,   103,    36,
      19,    20,    65,    72,    99,   100,   104,    22,    23,    24,
      48,    49,    50,    51,    52,    54,    86,    87,    89,    90,
      91,    92,    93,    94,    95,    97,    73,    42,    44,    48,
      68,    95,    47,    47,    46,   100,    65,    72,    89,    89,
      89,    21,    25,    26,    27,   105,    22,    23,    28,    29,
      30,    31,   106,    32,    33,   107,    34,    35,    44,    74,
      45,    75,    48,   101,   102,    47,    47,    48,    89,    90,
      90,    91,    92,    93,    94,    75,     8,    10,    11,    12,
      13,    14,    15,    16,    47,    48,    65,    71,    75,    76,
      77,    78,    79,    80,    82,    83,    84,    85,    88,   103,
      42,    43,    43,    43,    47,    47,    95,    96,    43,    43,
      21,    47,    47,    46,    65,    77,    47,    47,    47,    36,
      37,    38,    40,    41,    60,   108,   102,    96,    69,    71,
      96,    42,    54,    54,    48,    47,    95,    95,    44,    47,
      47,    44,    95,    42,    42,    77,    96,    96,    77,    81,
      86,    81,     9,    47,    47,    42,    44,    44,    77,    71,
      71,    86,    44,    44,    77,    77
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    61,    62,    63,    63,    64,    64,    65,    65,    65,
      66,    66,    67,    68,    69,    69,    70,    70,    71,    71,
      72,    72,    73,    73,    73,    74,    75,    76,    76,    76,
      76,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    78,    78,    79,    80,    81,    81,    82,    82,    83,
      83,    84,    85,    85,    86,    87,    87,    87,    87,    87,
      88,    88,    89,    89,    89,    89,    90,    90,    91,    91,
      91,    92,    92,    93,    93,    94,    94,    95,    95,    96,
      96,    97,    97,    97,    98,    99,    99,   100,   100,   100,
     100,   101,   101,   102,   102,   103,   103,   103,   103,   104,
     104,   105,   105,   105,   106,   106,   106,   106,   107,   107,
     108,   108,   108,   108,   108
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     1,     1,
       3,     3,     3,     1,     2,     3,     3,     1,     3,     3,
       6,     6,     1,     3,     0,     2,     3,     2,     1,     3,
       2,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       1,     2,     2,     6,     6,     1,     3,     9,     9,     1,
       2,     5,     5,     7,     1,     1,     3,     1,     1,     1,
       1,     3,     1,     2,     2,     2,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     6,     1,     2,     2,     2,     3,
       3,     1,     3,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 41 "src/main.y"
                  {root = new TreeNode(0,NODE_PROG); root->addChild(yyvsp[0]);}
#line 1528 "src/main.tab.cpp"
    break;

  case 3:
#line 43 "src/main.y"
                    {yyval=yyvsp[0];}
#line 1534 "src/main.tab.cpp"
    break;

  case 4:
#line 44 "src/main.y"
                     {
    yyval=yyvsp[-1]; yyval->addSibling(yyvsp[0]);
}
#line 1542 "src/main.tab.cpp"
    break;

  case 5:
#line 50 "src/main.y"
                {yyval = yyvsp[-1];}
#line 1548 "src/main.tab.cpp"
    break;

  case 6:
#line 51 "src/main.y"
          {yyval = yyvsp[0];}
#line 1554 "src/main.tab.cpp"
    break;

  case 7:
#line 56 "src/main.y"
             {
    yyval = yyvsp[0];
}
#line 1562 "src/main.tab.cpp"
    break;

  case 8:
#line 59 "src/main.y"
               {
    yyval = yyvsp[0];
}
#line 1570 "src/main.tab.cpp"
    break;

  case 9:
#line 62 "src/main.y"
                {
    yyval = yyvsp[0];
}
#line 1578 "src/main.tab.cpp"
    break;

  case 10:
#line 68 "src/main.y"
                          {
    TreeNode* node = new TreeNode(yyvsp[-1]->lineno, NODE_STMT);
    node->stype = STMT_CONSTDECL;
    node->addChild(yyvsp[-1]);
    node->addChild(yyvsp[0]);
    yyval = node;

    // add type to first declare node
    yyval->type = yyvsp[0]->type = yyvsp[-1]->type;
}
#line 1593 "src/main.tab.cpp"
    break;

  case 11:
#line 78 "src/main.y"
                                      {
    yyvsp[-2]->addChild(yyvsp[0]);
    yyval = yyvsp[-2];

    // add type to first declare node
    yyvsp[0]->type = yyval->type;
}
#line 1605 "src/main.tab.cpp"
    break;

  case 12:
#line 88 "src/main.y"
                                    {
    TreeNode* node = new TreeNode(yyvsp[-2]->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    yyvsp[-2]->is_dec = true; 
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 1618 "src/main.tab.cpp"
    break;

  case 13:
#line 100 "src/main.y"
        {
    yyval = yyvsp[0];
}
#line 1626 "src/main.tab.cpp"
    break;

  case 14:
#line 106 "src/main.y"
               {  // declare and init
    TreeNode* node = new TreeNode(yyvsp[-1]->lineno, NODE_STMT);
    node->stype = STMT_VARDECL;
    node->addChild(yyvsp[-1]);
    node->addChild(yyvsp[0]);
    yyval = node;

    // add type to first declare node
    yyval->type = yyvsp[0]->type = yyvsp[-1]->type;
}
#line 1641 "src/main.tab.cpp"
    break;

  case 15:
#line 116 "src/main.y"
                                    {
    yyvsp[-2]->addChild(yyvsp[0]);
    yyval = yyvsp[-2];

    // add type to first declare node
    yyvsp[0]->type = yyval->type;
}
#line 1653 "src/main.tab.cpp"
    break;

  case 16:
#line 126 "src/main.y"
                            {
    TreeNode* node = new TreeNode(yyvsp[-2]->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    yyvsp[-2]->is_dec=true;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 1666 "src/main.tab.cpp"
    break;

  case 17:
#line 134 "src/main.y"
            {
    TreeNode* node = new TreeNode(yyvsp[0]->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    yyvsp[0]->is_dec=true;
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 1678 "src/main.tab.cpp"
    break;

  case 18:
#line 144 "src/main.y"
                           {
    TreeNode* node = new TreeNode(lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_EQ;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 1691 "src/main.tab.cpp"
    break;

  case 19:
#line 152 "src/main.y"
                           {
    TreeNode* node = new TreeNode(lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = yyvsp[-1]->optype;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 1704 "src/main.tab.cpp"
    break;

  case 20:
#line 163 "src/main.y"
                                                      {
    TreeNode* node = new TreeNode(yyvsp[-5]->lineno,NODE_FUNC);
    node -> type = new Type(COMPOSE_FUNCTION); 
    node -> type -> retType = yyvsp[-5] -> type -> type; // retType
    node -> var_name = yyvsp[-4] -> var_name; // funcname
    yyvsp[-4] -> is_dec = true;
    node -> addChild(yyvsp[-4]); // this line can be ignored further
    node -> addChild(yyvsp[-2]); // params
    node -> addChild(yyvsp[0]);
    yyval = node;
}
#line 1720 "src/main.tab.cpp"
    break;

  case 21:
#line 174 "src/main.y"
                                                           {
    TreeNode* node = new TreeNode(yyvsp[-5]->lineno,NODE_FUNC);
    node -> type = new Type(COMPOSE_FUNCTION); 
    node -> type -> retType = TYPE_VOID -> type; // retType
    node -> var_name = yyvsp[-4] -> var_name; // funcname
    yyvsp[-4] -> is_dec = true;
    node -> addChild(yyvsp[-4]); // this line can be ignored further
    node -> addChild(yyvsp[-2]); // params
    node -> addChild(yyvsp[0]);
    yyval = node;
}
#line 1736 "src/main.tab.cpp"
    break;

  case 22:
#line 188 "src/main.y"
            {
    yyval = new TreeNode(lineno, NODE_LIST);
    yyval -> addChild(yyvsp[0]);
}
#line 1745 "src/main.tab.cpp"
    break;

  case 23:
#line 192 "src/main.y"
                                 {
    yyval = yyvsp[-2];
    yyval -> addChild(yyvsp[0]);
}
#line 1754 "src/main.tab.cpp"
    break;

  case 24:
#line 196 "src/main.y"
  {yyval = new TreeNode(lineno, NODE_LIST);}
#line 1760 "src/main.tab.cpp"
    break;

  case 25:
#line 201 "src/main.y"
               {
    TreeNode* node = new TreeNode(lineno, NODE_ITEM);
    node->itype=ITEM_UFUNC;
    node->type=yyvsp[-1]->type;
    node->addChild(yyvsp[0]);
    yyval=node;
}
#line 1772 "src/main.tab.cpp"
    break;

  case 26:
#line 211 "src/main.y"
                                 {
    yyval = yyvsp[-1];
}
#line 1780 "src/main.tab.cpp"
    break;

  case 27:
#line 217 "src/main.y"
                 {
    //maybe further replace by NODE_LIST
    yyval = new TreeNode(yyvsp[-1]->lineno, NODE_STMT);
    yyval->stype = STMT_BLOCK;
    yyval->addChild(yyvsp[-1]);
}
#line 1791 "src/main.tab.cpp"
    break;

  case 28:
#line 223 "src/main.y"
           {
    yyval = new TreeNode(yyvsp[0]->lineno, NODE_STMT);
    yyval->stype = STMT_BLOCK;
    yyval->addChild(yyvsp[0]);
}
#line 1801 "src/main.tab.cpp"
    break;

  case 29:
#line 228 "src/main.y"
                           {
    yyval = yyvsp[-2];
    yyval->addChild(yyvsp[-1]);
}
#line 1810 "src/main.tab.cpp"
    break;

  case 30:
#line 232 "src/main.y"
                     {
    yyval = yyvsp[-1];
    yyval->addChild(yyvsp[0]);
}
#line 1819 "src/main.tab.cpp"
    break;

  case 31:
#line 240 "src/main.y"
             {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_SKIP;}
#line 1825 "src/main.tab.cpp"
    break;

  case 32:
#line 241 "src/main.y"
        {yyval = yyvsp[0];}
#line 1831 "src/main.tab.cpp"
    break;

  case 33:
#line 242 "src/main.y"
         {yyval = yyvsp[0];}
#line 1837 "src/main.tab.cpp"
    break;

  case 34:
#line 243 "src/main.y"
          {yyval = yyvsp[0];}
#line 1843 "src/main.tab.cpp"
    break;

  case 35:
#line 244 "src/main.y"
            {yyval = yyvsp[0];}
#line 1849 "src/main.tab.cpp"
    break;

  case 36:
#line 245 "src/main.y"
                       {yyval = yyvsp[-1];}
#line 1855 "src/main.tab.cpp"
    break;

  case 37:
#line 246 "src/main.y"
                      {yyval= new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_BREAK;}
#line 1861 "src/main.tab.cpp"
    break;

  case 38:
#line 247 "src/main.y"
                         {yyval= new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_CONTINUE;}
#line 1867 "src/main.tab.cpp"
    break;

  case 39:
#line 248 "src/main.y"
                       {yyval = yyvsp[-1];}
#line 1873 "src/main.tab.cpp"
    break;

  case 40:
#line 249 "src/main.y"
           {yyval=yyvsp[0];}
#line 1879 "src/main.tab.cpp"
    break;

  case 41:
#line 253 "src/main.y"
                      {yyval=yyvsp[-1];}
#line 1885 "src/main.tab.cpp"
    break;

  case 42:
#line 254 "src/main.y"
                       {yyval=yyvsp[-1];}
#line 1891 "src/main.tab.cpp"
    break;

  case 43:
#line 258 "src/main.y"
                                                          {
    TreeNode* node = new TreeNode(lineno, NODE_FUNC);
    node->type = TYPE_VOID;
    node->addChild(yyvsp[-3]);
    node->addChild(yyvsp[-1]);
    node->var_name="scanf";
    yyval = node;
}
#line 1904 "src/main.tab.cpp"
    break;

  case 44:
#line 269 "src/main.y"
                                                           {
    TreeNode* node = new TreeNode(lineno, NODE_FUNC);
    node->type = TYPE_VOID;
    node->addChild(yyvsp[-3]);
    node->addChild(yyvsp[-1]);
    node->var_name="printf";
    yyval = node;
}
#line 1917 "src/main.tab.cpp"
    break;

  case 45:
#line 280 "src/main.y"
       {
    TreeNode* expnode = new TreeNode(lineno, NODE_ITEM);
    expnode->itype = ITEM_SPF;
    expnode->addChild(yyvsp[0]);
    
    TreeNode* node = new TreeNode(lineno, NODE_LIST);
    node->addChild(expnode);
    yyval = node;
}
#line 1931 "src/main.tab.cpp"
    break;

  case 46:
#line 289 "src/main.y"
                        {
    TreeNode* expnode = new TreeNode(lineno, NODE_ITEM);
    expnode->itype = ITEM_SPF;
    expnode->addChild(yyvsp[0]);
    
    yyval = yyvsp[-2];
    yyval->addChild(expnode);
}
#line 1944 "src/main.tab.cpp"
    break;

  case 47:
#line 301 "src/main.y"
                                                                                             {
    TreeNode* node = new TreeNode(yyvsp[-6]->lineno, NODE_STMT);
    node->stype = STMT_FOR;

    // check if $9 is a block, if not, create a block and put $9 into this block.
    if(yyvsp[0]->nodeType==NODE_STMT&&yyvsp[0]->stype==STMT_BLOCK){
        node->addChild(yyvsp[0]);
    }
    else{
        TreeNode* blocknode = new TreeNode(yyvsp[-6]->lineno, NODE_STMT);
        blocknode -> stype = STMT_BLOCK;
        blocknode -> addChild(yyvsp[0]);
        node -> addChild(blocknode);
    }

    node->addChild(yyvsp[-6]); node->addChild(yyvsp[-4]); node->addChild(yyvsp[-2]);

    yyval = node;
}
#line 1968 "src/main.tab.cpp"
    break;

  case 48:
#line 320 "src/main.y"
                                                                                            {
    TreeNode* node = new TreeNode(yyvsp[-6]->lineno, NODE_STMT);
    node->stype = STMT_FOR;

    // check if $9 is a block, if not, create a block and put $9 into this block.
    if(yyvsp[0]->nodeType==NODE_STMT&&yyvsp[0]->stype==STMT_BLOCK){
        node->addChild(yyvsp[0]);
    }
    else{
        TreeNode* blocknode = new TreeNode(yyvsp[-6]->lineno, NODE_STMT);
        blocknode -> stype = STMT_BLOCK;
        blocknode -> addChild(yyvsp[0]);
        node -> addChild(blocknode);
    }

    node->addChild(yyvsp[-6]); node->addChild(yyvsp[-4]); node->addChild(yyvsp[-2]);
    yyval = node;
}
#line 1991 "src/main.tab.cpp"
    break;

  case 49:
#line 341 "src/main.y"
             {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_RETURN;}
#line 1997 "src/main.tab.cpp"
    break;

  case 50:
#line 342 "src/main.y"
                      {
    TreeNode* node = new TreeNode(yyvsp[0]->lineno, NODE_STMT);
    node->stype = STMT_RETURN;
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2008 "src/main.tab.cpp"
    break;

  case 51:
#line 351 "src/main.y"
                                                    {
    TreeNode* node = new TreeNode(yyvsp[-2]->lineno, NODE_STMT);
    node->stype = STMT_WHILE;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2020 "src/main.tab.cpp"
    break;

  case 52:
#line 361 "src/main.y"
                                                  {
    TreeNode* node = new TreeNode(yyvsp[-2]->lineno, NODE_STMT);
    node->stype = STMT_IF;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2032 "src/main.tab.cpp"
    break;

  case 53:
#line 368 "src/main.y"
                                                                     {
    TreeNode* node = new TreeNode(yyvsp[-4]->lineno, NODE_STMT);
    node->stype = STMT_IFELSE;
    node->addChild(yyvsp[-4]);
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2045 "src/main.tab.cpp"
    break;

  case 54:
#line 379 "src/main.y"
        {yyval=yyvsp[0];}
#line 2051 "src/main.tab.cpp"
    break;

  case 55:
#line 384 "src/main.y"
             {
    yyval = yyvsp[0];
}
#line 2059 "src/main.tab.cpp"
    break;

  case 56:
#line 387 "src/main.y"
                                 {
    yyval = new TreeNode(yyvsp[-2]->lineno,NODE_EXPR);
    yyval -> stype = STMT_EXP;
    yyval -> optype = OP_POINT;
    yyval -> addChild(yyvsp[-2]); yyval -> addChild(yyvsp[0]);
    // TODO: miss struct's type expr
}
#line 2071 "src/main.tab.cpp"
    break;

  case 57:
#line 394 "src/main.y"
          {
    yyval = yyvsp[0];
}
#line 2079 "src/main.tab.cpp"
    break;

  case 58:
#line 397 "src/main.y"
       {
    yyval = yyvsp[0];
}
#line 2087 "src/main.tab.cpp"
    break;

  case 59:
#line 400 "src/main.y"
         {
    yyval = yyvsp[0];
}
#line 2095 "src/main.tab.cpp"
    break;

  case 60:
#line 407 "src/main.y"
             {
    TreeNode* node = new TreeNode(yyvsp[0]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LVAL;
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2107 "src/main.tab.cpp"
    break;

  case 61:
#line 414 "src/main.y"
                                  {
    // miss structure's type determine
    yyval = new TreeNode(yyvsp[-2]->lineno,NODE_EXPR);
    yyval -> stype = STMT_EXP;
    yyval -> optype = OP_POINT;
    yyval -> addChild(yyvsp[-2]); yyval -> addChild(yyvsp[0]);
}
#line 2119 "src/main.tab.cpp"
    break;

  case 62:
#line 424 "src/main.y"
            {
    yyval = yyvsp[0];
}
#line 2127 "src/main.tab.cpp"
    break;

  case 63:
#line 427 "src/main.y"
                                   {
    TreeNode* node = new TreeNode(yyvsp[0]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_FPLUS;
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2139 "src/main.tab.cpp"
    break;

  case 64:
#line 434 "src/main.y"
                                     {
    TreeNode* node = new TreeNode(yyvsp[0]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_FMINUS;
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2151 "src/main.tab.cpp"
    break;

  case 65:
#line 441 "src/main.y"
                  {
    TreeNode* node = new TreeNode(yyvsp[0]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_NOT;
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2163 "src/main.tab.cpp"
    break;

  case 66:
#line 451 "src/main.y"
          {
    yyval = yyvsp[0];
}
#line 2171 "src/main.tab.cpp"
    break;

  case 67:
#line 454 "src/main.y"
                       {
    TreeNode* node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = yyvsp[-1] -> optype;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2184 "src/main.tab.cpp"
    break;

  case 68:
#line 465 "src/main.y"
        {
    yyval = yyvsp[0];
}
#line 2192 "src/main.tab.cpp"
    break;

  case 69:
#line 468 "src/main.y"
                        {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_PLUS;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2205 "src/main.tab.cpp"
    break;

  case 70:
#line 476 "src/main.y"
                         {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_MINUS;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2218 "src/main.tab.cpp"
    break;

  case 71:
#line 487 "src/main.y"
        {
    yyval = yyvsp[0];
}
#line 2226 "src/main.tab.cpp"
    break;

  case 72:
#line 490 "src/main.y"
                     {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = yyvsp[-1]->optype;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2239 "src/main.tab.cpp"
    break;

  case 73:
#line 501 "src/main.y"
        {
    yyval = yyvsp[0];
}
#line 2247 "src/main.tab.cpp"
    break;

  case 74:
#line 504 "src/main.y"
                   {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = yyvsp[-1]->optype;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2260 "src/main.tab.cpp"
    break;

  case 75:
#line 515 "src/main.y"
       {
    yyval = yyvsp[0];
}
#line 2268 "src/main.tab.cpp"
    break;

  case 76:
#line 518 "src/main.y"
                        {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LAND;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2281 "src/main.tab.cpp"
    break;

  case 77:
#line 529 "src/main.y"
         {
    yyval = yyvsp[0];
}
#line 2289 "src/main.tab.cpp"
    break;

  case 78:
#line 532 "src/main.y"
                        {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LOR;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2302 "src/main.tab.cpp"
    break;

  case 79:
#line 543 "src/main.y"
        {
    yyval = yyvsp[0];
}
#line 2310 "src/main.tab.cpp"
    break;

  case 80:
#line 546 "src/main.y"
                           {
    TreeNode *node = new TreeNode(yyvsp[-2]->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_COMMA;
    node->addChild(yyvsp[-2]);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2323 "src/main.tab.cpp"
    break;

  case 81:
#line 555 "src/main.y"
                     {yyval=yyvsp[0];}
#line 2329 "src/main.tab.cpp"
    break;

  case 82:
#line 556 "src/main.y"
              {yyval=yyvsp[0];}
#line 2335 "src/main.tab.cpp"
    break;

  case 83:
#line 557 "src/main.y"
              {yyval=yyvsp[0];}
#line 2341 "src/main.tab.cpp"
    break;

  case 84:
#line 562 "src/main.y"
                                                                          {
    yyval = new TreeNode(yyvsp[-4]->lineno, NODE_STMT);
    yyval -> stype = STMT_STRUCTDECL;

    // record the struct params' types
    TreeNode* tnode = new TreeNode(yyvsp[-4]->lineno, NODE_TYPE);
    tnode->type = new Type(COMPOSE_STRUCT);

    // find all it's sons to add into a NODE_TYPE
    TreeNode* ptr_item = yyvsp[-2]->child;
    for(;ptr_item!=nullptr;ptr_item=ptr_item->sibling){
        if(ptr_item->nodeType==NODE_ITEM){
            TreeNode* ptr_stmt = ptr_item -> child;
            if(ptr_stmt->nodeType==NODE_STMT){
                tnode->type->addChild(ptr_stmt->type);
            }
            else continue;
        }
    }
    yyval->addChild(tnode);

    yyval -> addChild(yyvsp[-4]); yyval -> var_name = yyvsp[-4] -> var_name; // has to record struct name
    yyval -> addChild(yyvsp[-2]);
    yyval -> addChild(yyvsp[0]);

    // to record the type in the first init node
    yyvsp[-2]->type = yyval->type;
}
#line 2374 "src/main.tab.cpp"
    break;

  case 85:
#line 593 "src/main.y"
               {
    TreeNode* node = new TreeNode(lineno, NODE_LIST);
    node->addChild(yyvsp[0]);
    yyval = node;
}
#line 2384 "src/main.tab.cpp"
    break;

  case 86:
#line 598 "src/main.y"
                              {
    yyval = yyvsp[-1];
    yyval -> addChild(yyvsp[0]);
}
#line 2393 "src/main.tab.cpp"
    break;

  case 87:
#line 606 "src/main.y"
                {
    yyval = new TreeNode(yyvsp[-1]->lineno,NODE_ITEM);
    yyval -> addChild(yyvsp[-1]); yyval->authtype = AUTH_PUBLIC;
}
#line 2402 "src/main.tab.cpp"
    break;

  case 88:
#line 610 "src/main.y"
                   {
    yyval = new TreeNode(yyvsp[-1]->lineno,NODE_ITEM);
    yyval -> addChild(yyvsp[-1]); yyval->authtype = AUTH_PUBLIC;
}
#line 2411 "src/main.tab.cpp"
    break;

  case 89:
#line 614 "src/main.y"
                     {
    yyval = new TreeNode(yyvsp[-1]->lineno,NODE_ITEM);
    yyval -> addChild(yyvsp[-1]); yyval->authtype = yyvsp[-2]->authtype;
}
#line 2420 "src/main.tab.cpp"
    break;

  case 90:
#line 618 "src/main.y"
                        {
    yyval = new TreeNode(yyvsp[-1]->lineno,NODE_ITEM);
    yyval -> addChild(yyvsp[-1]); yyval->authtype = yyvsp[-2]->authtype;
}
#line 2429 "src/main.tab.cpp"
    break;

  case 91:
#line 625 "src/main.y"
                 {
    yyval = new TreeNode(yyvsp[0]->lineno,NODE_LIST);
    yyval->addChild(yyvsp[0]);
}
#line 2438 "src/main.tab.cpp"
    break;

  case 92:
#line 629 "src/main.y"
                                         {
    yyval = yyvsp[-2];
    yyval->addChild(yyvsp[0]);
}
#line 2447 "src/main.tab.cpp"
    break;

  case 93:
#line 637 "src/main.y"
            {
    yyval = new TreeNode(yyvsp[0]->lineno,NODE_ITEM);
    yyvsp[0]->is_dec = true;
    yyval-> addChild(yyvsp[0]);
}
#line 2457 "src/main.tab.cpp"
    break;

  case 94:
#line 642 "src/main.y"
  {yyval=new TreeNode(lineno,NODE_ITEM);}
#line 2463 "src/main.tab.cpp"
    break;

  case 95:
#line 645 "src/main.y"
         {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_INT;}
#line 2469 "src/main.tab.cpp"
    break;

  case 96:
#line 646 "src/main.y"
         {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_CHAR;}
#line 2475 "src/main.tab.cpp"
    break;

  case 97:
#line 647 "src/main.y"
         {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_BOOL;}
#line 2481 "src/main.tab.cpp"
    break;

  case 98:
#line 648 "src/main.y"
           {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_STRING;}
#line 2487 "src/main.tab.cpp"
    break;

  case 99:
#line 651 "src/main.y"
                  {yyval = new TreeNode(lineno, NODE_AUTH); yyval->authtype = AUTH_PUBLIC;}
#line 2493 "src/main.tab.cpp"
    break;

  case 100:
#line 652 "src/main.y"
              {yyval = new TreeNode(lineno, NODE_AUTH); yyval->authtype = AUTH_PRIVATE;}
#line 2499 "src/main.tab.cpp"
    break;

  case 101:
#line 660 "src/main.y"
               {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_MUL;}
#line 2505 "src/main.tab.cpp"
    break;

  case 102:
#line 661 "src/main.y"
          {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_DIV;}
#line 2511 "src/main.tab.cpp"
    break;

  case 103:
#line 662 "src/main.y"
          {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_MOD;}
#line 2517 "src/main.tab.cpp"
    break;

  case 104:
#line 665 "src/main.y"
                {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_LESS;}
#line 2523 "src/main.tab.cpp"
    break;

  case 105:
#line 666 "src/main.y"
           {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_GREA;}
#line 2529 "src/main.tab.cpp"
    break;

  case 106:
#line 667 "src/main.y"
         {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_LE;}
#line 2535 "src/main.tab.cpp"
    break;

  case 107:
#line 668 "src/main.y"
         {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_GE;}
#line 2541 "src/main.tab.cpp"
    break;

  case 108:
#line 671 "src/main.y"
              {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_EEQ;}
#line 2547 "src/main.tab.cpp"
    break;

  case 109:
#line 672 "src/main.y"
          {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_NEQ;}
#line 2553 "src/main.tab.cpp"
    break;

  case 110:
#line 675 "src/main.y"
                       {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_PLUSEQ;}
#line 2559 "src/main.tab.cpp"
    break;

  case 111:
#line 676 "src/main.y"
              {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_MINUSEQ;}
#line 2565 "src/main.tab.cpp"
    break;

  case 112:
#line 677 "src/main.y"
            {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_MODEQ;}
#line 2571 "src/main.tab.cpp"
    break;

  case 113:
#line 678 "src/main.y"
            {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_MULEQ;}
#line 2577 "src/main.tab.cpp"
    break;

  case 114:
#line 679 "src/main.y"
            {yyval = new TreeNode(lineno,NODE_OP); yyval->optype=OP_DIVEQ;}
#line 2583 "src/main.tab.cpp"
    break;


#line 2587 "src/main.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 682 "src/main.y"


int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}
