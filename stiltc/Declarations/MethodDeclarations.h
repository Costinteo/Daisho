#ifndef METHOD_DECLARATIONS
#define METHOD_DECLARATIONS
#include "Generated/Automata.h"
#include "Generated/TokType.h"
#include "StructDeclarations.h"

/*****************************/
/*    METHOD DECLARATIONS    */
/*****************************/

/*************/
/* Tokenizer */
/*************/

static inline void Token_print(Token tok);
static inline Token* Token_new_on(Arena arena, String content, TokType type, Target* source,
                                  size_t line, size_t pos);

static inline StiltsTokenizer* Tokenizer_init(StiltsTokenizer* tokenizer, Target* target);
static inline bool Tokenizer_nextToken(StiltsTokenizer* tokenizer, TokenStream stream);
static inline TokenStream Tokenizer_tokenize(StiltsTokenizer* tokenizer);
static inline void Tokenizer_destroy(StiltsTokenizer* tokenizer);

/*******/
/* AST */
/*******/

// Generated by GenNodeTypes.py
#include "Generated/ASTNodeMethods.h"

/**********/
/* Parser */
/**********/

static inline void StiltsParser_init(StiltsParser* parser);
static inline void StiltsParser_destroy(StiltsParser* parser);
static inline AST StiltsParser_parse(StiltsParser* parser, TokenStream tokens);
static inline void next_token(StiltsParser* parser);
static inline void parser_stack_trace(StiltsParser* parser);
static inline void parse_error(StiltsParser* parser, char* message);
static inline bool accept(StiltsParser* parser, TokType s);
static inline bool expect(StiltsParser* parser, TokType s);

/*************/
/* ASTWalker */
/*************/

static inline void ASTWalker_init(ASTWalker* walker, AST ast, TraversalOrder order);
static inline void ASTWalker_walk(ASTWalker* walker, walk_fn onwalk);
static inline void ASTWalker_destroy(ASTWalker* walker);

// Walks:
// 1. Build a list of all traits and their requirements.
// 2. Build a list of all types, the traits they implement, and the methods
// defined.
// 3. Build a list of all functions.

/*********************/
/* Semantic Analysis */
/*********************/

static inline List_Trait getTraits(AST ast);  // Names only
static inline void buildTraitHierarchy(List_Trait traits);
static inline void validateTraitHierarchy(List_Trait traits);

static inline List_Type getTypes(AST ast);
static inline void buildTypeHierarchy(List_Type types, List_Trait traits);
static inline void validateTypeHierarchy(List_Type types);

static inline List_Method getMethods();

/************/
/* Compiler */
/************/

int main();

// Flags are global.
static inline void parseFlags(int argc, char** argv);
static inline void destroyFlags();
static inline void printFlags();              // For debugging
static inline void usage();                   // --help and exit
static inline void arg_err(const char* msg);  // Error parsing args

// Pipeline:
static inline void runTokenizers(void);
static inline void runParsers(void);
static inline void runLowering(void);
static inline void runSemanticAnalysis(void);
static inline void runCodegen(void);

#endif  // METHOD_DECLARATIONS