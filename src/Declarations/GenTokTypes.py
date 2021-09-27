#!/bin/python

write_to = "TokType.h"

# [TokType, literal]

pcase = lambda s: [s.upper(), s.lower()]

keywords = [
    # Pragma
    pcase("native"),
    pcase("ctype"),

    # Types
    ["BOOL", "Bool"],
    ["CHAR", "Char"],
    ["UCHAR", "UChar"],
    ["SHORT", "Short"],
    ["USHORT", "UShort"],
    ["INT", "Int"],
    ["UINT", "UInt"],
    ["LONG", "Long"],
    ["FLOAT", "Float"],
    ["DOUBLE", "Double"],
    ["VOID", "Void"],

    # Control
    pcase('if'),
    pcase('else'),

    # Loops
    pcase('for'),
    pcase('while'),
    pcase('continue'),
    pcase('break'),
    pcase('in'),

    # Exceptions
    # makeLower('try'),
    # makeLower('catch'),
    # makeLower('finally'),

    # Classes
    pcase('class'),
    pcase('this'),
    pcase('operator'),
    pcase('extends'),

    # Interfaces
    pcase('trait'),
    pcase('impl'),

    # Other containers
    pcase('enum'),

    # Access Modifiers
    pcase('private'),
    pcase('protected'),
    pcase('public'),

    # Builtin functions
    pcase('super'),
    pcase('instanceof'),
    pcase('sizeof'),
    pcase('assert'),

    # Separators
    ["LPAREN", "("],
    ["RPAREN", ")"],
    ["LBRACE", "{"],
    ["RBRACE", "}"],
    ["LBRACK", "["],
    ["RBRACK", "]"],
    ["LARROW", "<"],
    ["RARROW", ">"],
    ["SEMI", ";"],
    ["COMMA", ","],
    ["DOT", "."],
    ["STAR", "*"],
    ["EQUALS", "="],

    # Operators
    ["BANG", "!"],
    ["TILDE", "~"],
    ["QUESTION", "?"],
    ["COLON", ":"],
    ["EQUAL", "=="],
    ["LE", "<="],
    ["GE", ">="],
    ["NOTEQUAL", "!="],
    ["AND", "&&"],
    ["OR", "||"],
    ["INC", "++"],
    ["DEC", "--"],
    ["ADD", "+"],
    ["SUB", "-"],
    ["DIV", "/"],
    ["AMP", "&"],
    ["BITOR", "|"],
    ["CARET", "^"],
    ["MOD", "%"],
    ["ARROW", "->"],

    ["ADD_ASSIGN", "+="],
    ["SUB_ASSIGN", "-="],
    ["MUL_ASSIGN", "*="],
    ["DIV_ASSIGN", "/="],
    ["AND_ASSIGN", "&="],
    ["OR_ASSIGN", "|="],
    ["XOR_ASSIGN", "^="],
    ["MOD_ASSIGN", "%="],
]

custom = ['INVALID', 'END_OF_FILE', 'IMPORT', 'COMMENT', 'IDENT', ]

names =  custom + [k[0] for k in keywords]

custom_functions = """
/**********/
/* Custom */
/**********/


static inline TokType validImport(char *str) {
  return String_equals(str, "include") || String_equals(str, "import")
             ? IMPORT
             : INVALID;
}

static inline bool potentialImport(char *str) {
  return apaz_str_startsWith("import", str) ||
         apaz_str_startsWith("include", str);
}

static inline TokType validIdent(char *str) {
  for (size_t i = 0; i < String_len(str); i++) {
    if ((str[i] != ' ') &
        (str[i] != '\\t') &
        (str[i] != '\\r') &
        (str[i] != '\\n'))
      return INVALID;
  }
  return IDENT;
}

static inline bool potentialIdent(char *str) { }

static inline TokType validComment(char *str) {
  if (apaz_strlen(str) < 3)
    return INVALID;
  else if (String_startsWith(str, "/*") && String_endsWith(str, "*/")) {
    size_t search_end = apaz_strlen(str) - 4; // Prefix/suffix
    str = str + 2;
    for (size_t i = 0; i < search_end; i++)
      if (str[i] == '*' & str[i + 1] == '/')
        return INVALID;
    return COMMENT;
  } else if (String_startsWith(str, "//") && String_endsWith(str, "\\n")) {
    size_t search_end = apaz_strlen(str) - 3; // -3 for prefix/suffix
    str = str + 2;
    for (size_t i = 0; i < search_end; i++)
      if (str[i] == '\\n')
        return INVALID;
    return COMMENT;
  }
  return INVALID;
}

static inline bool potentialComment(char *str) {
  bool sl = apaz_str_startsWith(str, "//");
  bool ml = apaz_str_startsWith(str, "/*");
  if (!(sl | ml))
    return false;

  size_t len = apaz_strlen(str);
  if (sl) {
    for (size_t i = 2; i < len - 1; i++)
      if (str[i] == '\\n')
        return false;
    return true;
  } else {
    for (size_t i = 2; i < len - 2; i++)
      if (str[i] == '*' & str[i + 1] == '/')
        return false;
    return true;
  }
}

"""




def writeValid(name, literal):
    f.write(
        f"static inline TokType valid_{name}(char* str) {{ static const char* tok = \"{literal}\"; const char *s = tok; while (*s) ++s; size_t toklen = (size_t)(s - str); for (size_t i = 0; i < toklen; i++) if (str[i] != tok[i]) return INVALID; return {name}; }}\n")


def writePotential(name, literal):
    f.write(
        f"static inline bool potential_{name}(char* str) {{ static const char* tok = \"{literal}\"; while (*str) if (*str++ != *tok++) return false; return true; }}\n")


with open(write_to, 'w') as f:
    # Header
    f.write('// THIS FILE GENERATED BY GenTokType.py. DO NOT EDIT.\n')
    f.write('#ifndef INCLUDE_TOKENS\n')
    f.write('#define INCLUDE_TOKENS\n')
    f.write('#include <apaz-libc.h>\n\n')

    # Declare token types
    f.write(f'#define NUM_TOKTYPES {len(names)}\n')
    f.write(f'#define MAX_TOKTYPE_NAME_LEN {max([len(n) for n in names])}\n')
    f.write('enum TokType {\n')
    [f.write(f"  {c},\n") for c in custom]
    [f.write(f"  {e[0]},\n") for e in keywords]
    f.write('};\ntypedef enum TokType TokType;\n')

    # Declare reverse map from TokType to TOKNAME as a static array of string.
    dq = '"';nl = '\n'
    f.write(f'static const char* TokNameMap[] = {{{nl}{f", {nl}".join([f"  {dq}{name}{dq}" for name in names])}\n}};{nl}{nl}')
    
    # Write methods for tokenizaton that can be generated
    for e in keywords:
        writeValid(e[0], e[1])
    for e in keywords:
        writePotential(e[0], e[1])
    
    # Write the ones that can't be automatically generated.
    f.write(custom_functions)

    # Footer
    f.write("\n#endif // INCLUDE_TOKENS")