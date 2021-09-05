parser grammar StiltsParser;

options {
	tokenVocab = StiltsLexer;
}

compilationUnit: topLevelDeclaration+;

topLevelDeclaration:
	globalVariableDeclaration
	| functionDeclaration
	| classDeclaration
	| interfaceDeclaration
	| enumDeclaration
	| ctypeDeclaration;

classDeclaration: CLASS;
interfaceDeclaration: INTERFACE IDENT;

// Enum
enumDeclaration: ENUM LBRACE enumMember* RBRACE;
enumMember: varIdent EQUALS enumValue SEMI;
enumValue: NullLiteral | IntegerLiteral | BooleanLiteral;

// ctype
ctypeDeclaration: CTYPE IDENT SEMI;

globalVariableDeclaration: // Disallow tuple unpacking
	typeIdent varIdent SEMI
	| typeIdent varIdent EQUALS expr SEMI;

genericTypeDecl: LARROW genTypeDecList RARROW;
genTypeDecList: genDeclType genTypeDecList |;
genDeclType:
	varIdent // For type generics (<T>)s
	| typeIdent; // For value-type generics

/***************/
/* Identifiers */
/***************/
// Normal letters, underscores, etc.
varIdent: IDENT;
fnIdent: IDENT;

// With generic types and pointers
typeIdent: IDENT genericTypeDecl? | typeIdent STAR;

/***************/
/* Expressions */
/***************/
expr:
	IDENT
	| literal
	| lambdaExpr
	| functionCall
	| LPAREN expr RPAREN;

lambdaExpr: LPAREN lambdaArgList RPAREN ARROW statement;
lambdaArgList: varIdent lambdaArgList |;
// or empty

/**************/
/* Statements */
/**************/
statement: typeIdent? varIdent EQUALS expr | blockStatement;
blockStatement: LBRACE statement* RBRACE;

/*************/
/* Functions */
/*************/

// declaration
functionDeclaration:
	typeIdent? fnIdent LPAREN fnDeclarationArgList RPAREN statement;
fnDeclarationArgList:
	fnDeclarationArg fnDeclarationArgList
	| fnDeclarationArg;
fnDeclarationArg: typeIdent varIdent defaultArg?;
defaultArg: EQUALS literal;

// call
functionCall: fnIdent LPAREN callArgList RPAREN;
callArgList: varIdent COMMA callArgList | varIdent;

// lambdas

/**************/
/* Misc Rules */
/**************/
literal:
	NullLiteral
	| IntegerLiteral
	| FloatLiteral
	| StringLiteral
	| BooleanLiteral;