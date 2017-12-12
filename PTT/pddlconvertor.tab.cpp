/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "pddlconvertor.ypp" /* yacc.c:339  */

//	#include <actiondef.h>
//	#include <FlexLexer.h>
	#include <iostream>
//	#include <fstream>
	#include <list>
	#include <algorithm>
	#include "common.h"
	//#include "pddlfile.h"
	
	int yylex(void);
	int yyerror(char *);
	int yyparse(void);

	extern CDomain *dom;

	extern CProblem *prob;
	
//	yyFlexLexer lexer;
//	PDDLFile pddlfile;		

#line 88 "pddlconvertor.tab.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "pddlconvertor.tab.hpp".  */
#ifndef YY_YY_PDDLCONVERTOR_TAB_HPP_INCLUDED
# define YY_YY_PDDLCONVERTOR_TAB_HPP_INCLUDED
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
    DEFINE = 258,
    DOMAIN = 259,
    DDOMAIN = 260,
    PROBLEM = 261,
    ACTION = 262,
    IDENTIFIER = 263,
    VARIABLE = 264,
    PARAMETERS = 265,
    PRECONDITION = 266,
    EFFECT = 267,
    AND = 268,
    NOT = 269,
    PREDICATES = 270,
    TYPES = 271,
    REQUIREMENTS = 272,
    REQUIREMENT = 273,
    TOTAL_COST = 274,
    NUMBER_TYPE = 275,
    INCREASE = 276,
    OBJECTS = 277,
    INIT = 278,
    GOAL = 279,
    EITHER = 280,
    FUNCTIONS = 281,
    CONSTANTS = 282,
    NUMBER = 283,
    METRIC = 284,
    MINIMIZE = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 23 "pddlconvertor.ypp" /* yacc.c:355  */

//	RequirementEnum requirement;
	char str[255];
	void* pointer;
	int integer;

	/*if here comes during compilaton error, that the type doesn't exist
	  than remember, that the include must be not only in this .y file,
	  but also in .l file.
	*/

	deque<string>* pstringlist;
	TypesRecord*	  tr;
	CTypes*		  ptypes;
	ParamRecord*	  pr;
	CParameter*		  pparams;
	CPredicate*		  ppred;
	CPredicateList*		  ppredlist;
	CAction*		  pact;
	CActionList*		  pactlist;


/*	ActionDef* pactiondef;
	Parameter* pparameter;
	Predicate* ppredicate;
	PredicateDef* ppredicatedef;
	TypedList* ptypedlist;
	Operator* poperator;
	OperatorDef* poperatordef;
	
	list<Parameter*>* pparameterlist;
	list<string>* pstringlist;
	list<Predicate*>* ppredicatelist;
	list<PredicateDef*>* ppredicatedeflist;
	list<RequirementEnum>* prequirementlist;
	list<ActionDef*>* pactiondeflist;
	list<TypedList*>* ptypedlistlist;
	list<Object*>* pobjectlist;
	list<Operator*>* poperatorlist;
	list<OperatorDefParam*>* poperatordefparamlist;
	list<OperatorVariable*>* poperatorvariablelist;
	list<TypeDef*>* ptypedeflist;*/

#line 203 "pddlconvertor.tab.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PDDLCONVERTOR_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 218 "pddlconvertor.tab.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  196

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      32,    31,     2,     2,     2,    33,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   119,   119,   120,   204,   208,   216,   223,   234,   245,
     250,   256,   261,   270,   276,   282,   288,   298,   304,   307,
     313,   319,   325,   331,   335,   338,   345,   351,   355,   362,
     367,   378,   382,   389,   395,   399,   406,   409,   415,   422,
     426,   432,   435,   438,   441,   494,   498,   506,   509,   516,
     524,   528,   536,   541,   549,   552,   556,   576,   596,   599,
     611,   616,   623,   627,   635,   645,   650,   654,   663,   670,
     673,   686,   693,   697,   704,   728,   735
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DEFINE", "DOMAIN", "DDOMAIN", "PROBLEM",
  "ACTION", "IDENTIFIER", "VARIABLE", "PARAMETERS", "PRECONDITION",
  "EFFECT", "AND", "NOT", "PREDICATES", "TYPES", "REQUIREMENTS",
  "REQUIREMENT", "TOTAL_COST", "NUMBER_TYPE", "INCREASE", "OBJECTS",
  "INIT", "GOAL", "EITHER", "FUNCTIONS", "CONSTANTS", "NUMBER", "METRIC",
  "MINIMIZE", "')'", "'('", "'-'", "$accept", "pddl", "domaindef",
  "domainhead", "envpart", "problemdef", "metric", "goal", "init",
  "objectdeclaration", "constantsdef", "requirementdefs",
  "requirementlist", "typedefs", "typedlists", "typedlist", "identifiers",
  "predicatedefs", "predicatedeflist", "predicatedef", "actioncostdef",
  "actioncostlist", "actioncostatom", "actiondefs", "actiondef",
  "actparameters", "actpreconditions", "acteffects", "parameters",
  "parameter", "types", "type", "operators", "operator",
  "operatorvariables", "variables", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    41,    40,    45
};
# endif

#define YYPACT_NINF -114

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-114)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -20,    57,    62,  -114,    33,  -114,    52,  -114,    89,    69,
      91,   104,  -114,     9,   106,    88,   119,     4,   107,    61,
    -114,    92,   104,  -114,    44,   108,  -114,   133,   134,   135,
      79,  -114,  -114,    18,  -114,     2,  -114,    64,   136,   113,
      80,    81,  -114,  -114,   108,  -114,    88,    94,  -114,   115,
     116,    37,  -114,  -114,  -114,  -114,  -114,   140,  -114,  -114,
     139,    75,   117,    83,  -114,  -114,  -114,    20,  -114,   120,
    -114,  -114,    41,  -114,     0,  -114,   121,   105,   123,   131,
    -114,  -114,  -114,   150,  -114,  -114,     0,  -114,     6,   147,
     125,   126,   148,   128,   129,  -114,   153,  -114,   138,     6,
    -114,    47,    53,  -114,    67,  -114,   137,  -114,   144,   141,
      88,  -114,  -114,    10,   142,   142,   143,   142,  -114,  -114,
     145,    21,  -114,  -114,  -114,    78,    14,    55,    87,  -114,
     149,   146,    93,    -1,   151,   152,  -114,   154,    23,  -114,
    -114,  -114,  -114,  -114,  -114,   155,  -114,    88,   156,   157,
     159,   151,   160,   162,    71,    24,   142,   158,    95,   157,
     161,   163,   164,   165,  -114,    97,   166,  -114,   167,   168,
      99,  -114,  -114,  -114,   112,  -114,    77,   101,   170,  -114,
    -114,   171,    16,   142,  -114,   169,  -114,   172,   103,   174,
    -114,   173,   175,  -114,   176,  -114
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     0,     3,     0,     1,     0,     0,
       0,     0,     7,     0,     0,     0,     0,     0,     0,     0,
      45,     0,     0,     8,     0,     0,    11,     0,     0,     0,
       0,    34,    31,     0,    27,    30,    24,     0,     0,     0,
       0,     0,     4,    46,     0,     9,     0,     0,    12,     0,
       0,     0,    33,    35,    26,    28,    32,     0,    25,    23,
       0,     0,    44,     0,    39,     5,    10,     0,     6,     0,
      75,    37,     0,    54,    56,    29,     0,     0,     0,     0,
      38,    40,    22,     0,    36,    55,     0,    76,     0,     0,
       0,     0,     0,     0,    42,    43,     0,    60,     0,    57,
      58,     0,     0,    50,     0,    52,     0,    48,     0,     0,
       0,    59,    49,     0,     0,     0,     0,     0,    47,    41,
       0,     0,    72,    71,    65,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,    61,     0,     0,    74,
      73,    64,    51,    63,    66,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,    14,     0,     0,
       0,    69,    70,    68,     0,    20,     0,     0,     0,    16,
      13,     0,     0,     0,    19,     0,    15,     0,     0,     0,
      67,     0,     0,    18,     0,    17
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -114,  -114,  -114,  -114,  -114,  -114,    -4,     8,    19,    36,
     183,    49,  -114,   177,   -45,   -33,   -52,    -5,  -114,    34,
    -114,  -114,   178,   179,    38,  -114,  -114,    84,   102,   -65,
    -114,   109,  -113,   -88,    35,   -64
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,     9,     5,   169,   158,   149,   134,
      25,    10,    37,    11,    33,    34,    35,    12,    30,    62,
      18,    63,    64,    19,    20,    77,    92,    93,    72,    73,
      99,   100,   128,   129,   126,    74
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      55,    67,   103,   105,   132,    23,    26,    85,    86,    87,
      56,    38,     1,    27,    97,    28,    16,    45,   122,   123,
      48,   147,   139,   140,   139,   140,    32,   130,    32,    56,
      39,    56,    32,    88,    55,    57,    85,    86,    98,    66,
     143,   124,   125,   165,   143,   141,    70,   187,    31,    54,
      70,    82,   136,   177,   153,   164,    70,    43,   121,    14,
       6,   113,     7,   113,    53,     8,   114,   115,    71,   115,
     188,    46,    84,   138,   116,   113,   116,   143,   112,    43,
     117,   115,    58,    51,    13,   113,    32,    38,   116,   143,
     183,   115,    42,    40,    78,    59,    32,   137,   116,   162,
     143,    17,   155,   163,    14,    15,    16,    14,    15,    14,
      52,    29,    65,    40,    80,    61,    90,    91,   142,   127,
     122,   123,    55,    21,   146,   127,   167,   168,   175,   127,
     180,   168,   184,   127,   191,   127,    24,    36,    29,    40,
      47,    49,    50,    51,    60,    61,    68,    69,    75,    76,
      79,    95,    83,    89,    94,    96,    70,   102,   104,   107,
      91,   109,   108,   110,   119,   145,   181,   170,   118,   135,
     159,   151,   120,   174,   127,   131,   106,   133,     0,   156,
     144,   147,   166,   148,   150,   152,   154,    22,   160,   157,
     161,   101,   171,   192,   172,   173,   178,    41,   176,   179,
     185,   189,   186,   190,   193,    44,   194,   195,   111,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    81
};

static const yytype_int16 yycheck[] =
{
      33,    46,    90,    91,   117,    10,    11,    72,    72,     9,
       8,     7,    32,     4,     8,     6,    17,    22,     8,     9,
      25,    22,     8,     9,     8,     9,     8,   115,     8,     8,
      26,     8,     8,    33,    67,    33,   101,   101,    32,    44,
     128,    31,    32,   156,   132,    31,     9,    31,    14,    31,
       9,    31,    31,   166,    31,    31,     9,    19,   110,    15,
       3,     8,     0,     8,    30,    32,    13,    14,    31,    14,
     183,    27,    31,   125,    21,     8,    21,   165,    31,    41,
      13,    14,    18,     8,    32,     8,     8,     7,    21,   177,
      13,    14,    31,    32,    19,    31,     8,    19,    21,    28,
     188,    32,   147,    32,    15,    16,    17,    15,    16,    15,
      31,    32,    31,    32,    31,    32,    11,    12,    31,    32,
       8,     9,   155,    32,    31,    32,    31,    32,    31,    32,
      31,    32,    31,    32,    31,    32,    32,    18,    32,    32,
      32,     8,     8,     8,     8,    32,    31,    31,     8,    10,
      33,    20,    32,    32,    31,     5,     9,    32,    32,    31,
      12,     8,    33,    25,    20,    19,   170,   159,    31,   120,
     151,   135,    31,     8,    32,    32,    92,    32,    -1,    23,
      31,    22,    24,    32,    32,    31,    31,    10,    28,    32,
      28,    89,    31,    19,    31,    31,    29,    18,    32,    31,
      30,    32,    31,    31,    31,    22,    31,    31,    99,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    32,    35,    36,    37,    39,     3,     0,    32,    38,
      45,    47,    51,    32,    15,    16,    17,    32,    54,    57,
      58,    32,    47,    51,    32,    44,    51,     4,     6,    32,
      52,    53,     8,    48,    49,    50,    18,    46,     7,    26,
      32,    57,    31,    58,    44,    51,    27,    32,    51,     8,
       8,     8,    31,    53,    31,    49,     8,    33,    18,    31,
       8,    32,    53,    55,    56,    31,    51,    48,    31,    31,
       9,    31,    62,    63,    69,     8,    10,    59,    19,    33,
      31,    56,    31,    32,    31,    63,    69,     9,    33,    32,
      11,    12,    60,    61,    31,    20,     5,     8,    32,    64,
      65,    62,    32,    67,    32,    67,    61,    31,    33,     8,
      25,    65,    31,     8,    13,    14,    21,    13,    31,    20,
      31,    50,     8,     9,    31,    32,    68,    32,    66,    67,
      67,    32,    66,    32,    43,    45,    31,    19,    50,     8,
       9,    31,    31,    67,    31,    19,    31,    22,    32,    42,
      32,    43,    31,    31,    31,    48,    23,    32,    41,    42,
      28,    28,    28,    32,    31,    66,    24,    31,    32,    40,
      41,    31,    31,    31,     8,    31,    32,    66,    29,    31,
      31,    40,    68,    13,    31,    30,    31,    31,    66,    32,
      31,    31,    19,    31,    31,    31
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    34,    35,    35,    36,    36,    37,    38,    38,    38,
      38,    38,    38,    39,    39,    39,    39,    40,    41,    41,
      42,    43,    44,    45,    46,    46,    47,    48,    48,    49,
      49,    50,    50,    51,    52,    52,    53,    53,    54,    55,
      55,    56,    56,    56,    56,    57,    57,    58,    58,    59,
      60,    60,    61,    61,    62,    62,    62,    63,    64,    64,
      65,    65,    66,    66,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    68,    68,    69,    69
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     4,     5,     6,     1,     2,     3,
       4,     2,     3,    15,    14,    16,    15,     7,     7,     4,
       4,     4,     4,     4,     1,     2,     4,     1,     2,     3,
       1,     1,     2,     4,     1,     2,     4,     3,     4,     1,
       2,     5,     3,     3,     1,     1,     2,     7,     6,     4,
       2,     5,     2,     5,     1,     2,     1,     3,     1,     2,
       1,     4,     1,     2,     4,     3,     4,    10,     7,     7,
       7,     1,     1,     2,     2,     1,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
#line 119 "pddlconvertor.ypp" /* yacc.c:1646  */
    {}
#line 1433 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 120 "pddlconvertor.ypp" /* yacc.c:1646  */
    {}
#line 1439 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 205 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	   dom->SetActions((yyvsp[-1].pactlist));
	}
#line 1447 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 209 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	   dom->SetFunctions((yyvsp[-2].ppredlist));
	   dom->SetActions((yyvsp[-1].pactlist));
	   dom->SetActionCost(true);
	}
#line 1457 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 217 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
            dom->SetName((yyvsp[-1].str));
        }
#line 1465 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 224 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
        CTypes *t = new CTypes();
		TypesRecord *r=new TypesRecord();
		r->plistchilds=new deque<string>();
		r->plistchilds->push_back("object");
		r->parent="xobject";
		t->AddRecord(r);
		dom->SetTypes(t);
		dom->SetPredicates((yyvsp[0].ppredlist));
       }
#line 1480 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 235 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
        CTypes *t = new CTypes();
		TypesRecord *r=new TypesRecord();
		r->plistchilds=new deque<string>();
		r->plistchilds->push_back("object");
		r->parent="xobject";
		t->AddRecord(r);
		dom->SetTypes(t);
		dom->SetPredicates((yyvsp[0].ppredlist));
       }
#line 1495 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 246 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
        dom->SetTypes((yyvsp[-1].ptypes));
        dom->SetPredicates((yyvsp[0].ppredlist));
       }
#line 1504 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 251 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
        dom->SetTypes((yyvsp[-2].ptypes));
        dom->SetConstants((yyvsp[-1].ptypes));
        dom->SetPredicates((yyvsp[0].ppredlist));
       }
#line 1514 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 257 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
        dom->SetTypes((yyvsp[-1].ptypes));
        dom->SetPredicates((yyvsp[0].ppredlist));
       }
#line 1523 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 262 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
        dom->SetTypes((yyvsp[-2].ptypes));
        dom->SetConstants((yyvsp[-1].ptypes));
        dom->SetPredicates((yyvsp[0].ppredlist));
       }
#line 1533 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 270 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		prob->SetName((yyvsp[-10].str));
		prob->SetObjects((yyvsp[-3].ptypes));
		prob->SetInit((yyvsp[-2].ppredlist));
		prob->SetGoal((yyvsp[-1].ppredlist));
	}
#line 1544 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 276 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		prob->SetName((yyvsp[-9].str));
		prob->SetObjects((yyvsp[-3].ptypes));
		prob->SetInit((yyvsp[-2].ppredlist));
		prob->SetGoal((yyvsp[-1].ppredlist));
	}
#line 1555 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 282 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		prob->SetName((yyvsp[-11].str));
		prob->SetObjects((yyvsp[-4].ptypes));
		prob->SetInit((yyvsp[-3].ppredlist));
		prob->SetGoal((yyvsp[-2].ppredlist));
	}
#line 1566 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 288 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		prob->SetName((yyvsp[-10].str));
		prob->SetObjects((yyvsp[-4].ptypes));
		prob->SetInit((yyvsp[-3].ppredlist));
		prob->SetGoal((yyvsp[-2].ppredlist));	
	}
#line 1577 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 298 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		
	}
#line 1585 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 304 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-2].ppredlist);
	}
#line 1593 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 307 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
	}
#line 1601 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 313 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
	}
#line 1609 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 319 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ptypes)=(yyvsp[-1].ptypes);
	}
#line 1617 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 325 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ptypes)=(yyvsp[-1].ptypes);
	}
#line 1625 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 331 "pddlconvertor.ypp" /* yacc.c:1646  */
    { /* $$=$3; */}
#line 1631 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 335 "pddlconvertor.ypp" /* yacc.c:1646  */
    {

	}
#line 1639 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 338 "pddlconvertor.ypp" /* yacc.c:1646  */
    {

	}
#line 1647 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 345 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ptypes)=(yyvsp[-1].ptypes);
	}
#line 1655 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 351 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ptypes)=new CTypes();
		(yyval.ptypes)->AddRecord((yyvsp[0].tr));
	}
#line 1664 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 355 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-1].ptypes)->AddRecord((yyvsp[0].tr));
		(yyval.ptypes)=(yyvsp[-1].ptypes);
	}
#line 1673 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 362 "pddlconvertor.ypp" /* yacc.c:1646  */
    {	
		(yyval.tr)=new TypesRecord();
		(yyval.tr)->plistchilds=(yyvsp[-2].pstringlist);
		(yyval.tr)->parent=(yyvsp[0].str);
	}
#line 1683 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 367 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.tr)=new TypesRecord();
		(yyval.tr)->plistchilds=(yyvsp[0].pstringlist);
		(yyval.tr)->parent=find((yyvsp[0].pstringlist)->begin(),(yyvsp[0].pstringlist)->end(),"object")==(yyvsp[0].pstringlist)->end() ? "object" : "xobject";  //object is a default value if no parent defined
	}
#line 1693 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 378 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pstringlist)=new deque<string>();
		(yyval.pstringlist)->push_back((yyvsp[0].str));
	}
#line 1702 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 382 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyvsp[-1].pstringlist)->push_back((yyvsp[0].str));
		(yyval.pstringlist)=(yyvsp[-1].pstringlist);
	}
#line 1711 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 389 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
	}
#line 1719 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 395 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=new CPredicateList();
		(yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 1728 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 399 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
		(yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 1737 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 406 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppred)=new CPredicate((yyvsp[-2].str),(yyvsp[-1].pparams));
	}
#line 1745 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 409 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppred)=new CPredicate((yyvsp[-1].str),new CParameter());
	}
#line 1753 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 415 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	   (yyval.ppredlist)=(yyvsp[-1].ppredlist);
	}
#line 1761 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 422 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	    (yyval.ppredlist)=new CPredicateList();
	    (yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 1770 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 426 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	    (yyval.ppredlist)=(yyvsp[-1].ppredlist);
	    (yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 1779 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 432 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	    (yyval.ppred)=new CPredicate("total-cost",new CParameter());
        }
#line 1787 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 435 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	    (yyval.ppred)=new CPredicate("total-cost",new CParameter());
        }
#line 1795 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 438 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	    (yyval.ppred)=(yyvsp[-2].ppred);
        }
#line 1803 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 441 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	    (yyval.ppred)=(yyvsp[0].ppred);
        }
#line 1811 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 494 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pactlist)=new CActionList();
		(yyval.pactlist)->AddRecord((yyvsp[0].pact));
	}
#line 1820 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 498 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pactlist)=(yyvsp[-1].pactlist);
		(yyval.pactlist)->AddRecord((yyvsp[0].pact));	
	}
#line 1829 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 506 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pact)=new CAction((yyvsp[-4].str),(yyvsp[-3].pparams),(yyvsp[-2].ppredlist),(yyvsp[-1].ppredlist));
	}
#line 1837 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 509 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pact)=new CAction((yyvsp[-3].str),(yyvsp[-2].pparams),NULL,(yyvsp[-1].ppredlist));	
	}
#line 1845 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 516 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pparams)=(yyvsp[-1].pparams);
	}
#line 1853 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 524 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	        (yyval.ppredlist)=new CPredicateList();
		(yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 1862 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 528 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
	}
#line 1870 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 536 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=new CPredicateList();
		(yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
		
	}
#line 1880 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 541 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
	}
#line 1888 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 549 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pparams)=(yyvsp[0].pparams);
	}
#line 1896 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 552 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		*((yyvsp[-1].pparams))+=*((yyvsp[0].pparams));
		(yyval.pparams)=(yyvsp[-1].pparams);
	}
#line 1905 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 556 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		ParamRecord *ppr;
		CParameter *pcparam;
		string s;
		
		pcparam=new CParameter();

		while(!(yyvsp[0].pstringlist)->empty()){
			s=(yyvsp[0].pstringlist)->front();
			ppr=new ParamRecord;
			ppr->name=s.substr(1);
			ppr->plisttypes=new deque<string>(1,"object");	
			pcparam->AddRecord(ppr);
			(yyvsp[0].pstringlist)->pop_front();
		}
		(yyval.pparams)=pcparam;
	}
#line 1927 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 576 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		ParamRecord *ppr;
		CParameter *pcparam;
		string s;
		
		pcparam=new CParameter();

		while(!(yyvsp[-2].pstringlist)->empty()){
			s=(yyvsp[-2].pstringlist)->front();
			ppr=new ParamRecord;
			ppr->name=s.substr(1);
			ppr->plisttypes=(yyvsp[0].pstringlist);	
			pcparam->AddRecord(ppr);
			(yyvsp[-2].pstringlist)->pop_front();
		}
		(yyval.pparams)=pcparam;
	}
#line 1949 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 596 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pstringlist)=(yyvsp[0].pstringlist);
	}
#line 1957 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 599 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		string s;
		(yyval.pstringlist)=(yyvsp[-1].pstringlist);
		while (!(yyvsp[0].pstringlist)->empty()){
			s=(yyvsp[0].pstringlist)->front();
			(yyval.pstringlist)->push_back(s);
			(yyvsp[0].pstringlist)->pop_front();
		}
	}
#line 1971 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 611 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pstringlist)=new deque<string>();
		(yyval.pstringlist)->push_back((yyvsp[0].str));
		
	}
#line 1981 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 616 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.pstringlist)=(yyvsp[-1].pstringlist);
	}
#line 1989 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 623 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=new CPredicateList();
		(yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 1998 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 627 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppredlist)=(yyvsp[-1].ppredlist);
		(yyval.ppredlist)->AddRecord((yyvsp[0].ppred));
	}
#line 2007 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 635 "pddlconvertor.ypp" /* yacc.c:1646  */
    { //example: (lifting ?x ?y) (carry hand ?x) (at truck0 distributor1) ...
		CParameter *pars;
		pars=new CParameter((yyvsp[-1].pstringlist));
		(yyval.ppred)=new CPredicate((yyvsp[-2].str),pars);
	}
#line 2017 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 645 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		CParameter *pars;
		pars=new CParameter();
		(yyval.ppred)=new CPredicate((yyvsp[-1].str),pars);		
	}
#line 2027 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 650 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		(yyval.ppred)=(yyvsp[-1].ppred);
		(yyval.ppred)->Negate();
	}
#line 2036 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 654 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	       CParameter *p=new CParameter((yyvsp[-2].pstringlist));
	       CPredicate *tmp=new CPredicate((yyvsp[-3].str),p);
	       CParameter *pars;
	       deque<string> x;
	       x.push_back(tmp->ToString(false));
		pars=new CParameter(&x,true);
		(yyval.ppred)=new CPredicate("total-cost",pars);	
	}
#line 2050 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 663 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	       CParameter *pars;
	       deque<string> x;
	       x.push_back((yyvsp[-1].str));
		pars=new CParameter(&x,true);
		(yyval.ppred)=new CPredicate("total-cost",pars);	
	}
#line 2062 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 670 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	      (yyval.ppred)=NULL;	
	}
#line 2070 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 673 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	     CParameter *pars;
	     
	     (yyvsp[-3].pstringlist)->push_front("(");
	     (yyvsp[-3].pstringlist)->push_back(")");
	     (yyvsp[-3].pstringlist)->push_back((yyvsp[-1].str));
	     pars=new CParameter((yyvsp[-3].pstringlist),true);
	     (yyval.ppred)=new CPredicate((yyvsp[-5].str),pars);
	}
#line 2084 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 686 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		string s=(yyvsp[0].str);
		//s=$1;
		//s=s.substr(1);
		(yyval.pstringlist)=new deque<string>();
		(yyval.pstringlist)->push_back(s);
	}
#line 2096 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 693 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	        (yyval.pstringlist)=new deque<string>();
		(yyval.pstringlist)->push_back((yyvsp[0].str));
	}
#line 2105 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 697 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	        string s=(yyvsp[0].str);
		//s=$2;
		//s=s.substr(1);
		(yyvsp[-1].pstringlist)->push_back(s);
		(yyval.pstringlist)=(yyvsp[-1].pstringlist);
	}
#line 2117 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 704 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
	       (yyvsp[-1].pstringlist)->push_back((yyvsp[0].str));
		(yyval.pstringlist)=(yyvsp[-1].pstringlist);
	}
#line 2126 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 728 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		string s=(yyvsp[0].str);
		//s=$1;
		//s=s.substr(1);
		(yyval.pstringlist)=new deque<string>();
		(yyval.pstringlist)->push_back(s);
	}
#line 2138 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 735 "pddlconvertor.ypp" /* yacc.c:1646  */
    {
		string s=(yyvsp[0].str);
		//s=$2;
		//s=s.substr(1);
		(yyvsp[-1].pstringlist)->push_back(s);
		(yyval.pstringlist)=(yyvsp[-1].pstringlist);
	}
#line 2150 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
    break;


#line 2154 "pddlconvertor.tab.cpp" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
                  yystos[*yyssp], yyvsp);
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
#line 744 "pddlconvertor.ypp" /* yacc.c:1906  */


int yyerror(char *chybka)
{
  	printf("%s\n", chybka);
  	return 0;
}

extern int yylex(void);

/*
int main (int argc, char **argv)
{
	ofstream out("pddlconvertor.log");
	streambuf* oldclog;
	
	if (out){
		oldclog = clog.rdbuf();
		clog.rdbuf(out.rdbuf());	
	}
	
  	yyparse();
//  	pddlfile.OutputTo(cout);

  	pddlfile.OutputToGrounded(cout);
  	
  	if (out){
  		clog.rdbuf(oldclog);
  	}
  	
  	return 0;
}
*/

