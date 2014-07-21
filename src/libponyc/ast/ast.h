#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stddef.h>
#include "token.h"

/*

PROGRAM: {PACKAGE}
symtab: path -> PACKAGE

PACKAGE: {MODULE}
data: package_t
symtab: ID -> TYPE | TRAIT | CLASS | ACTOR

MODULE: {USE} {TYPE | TRAIT | CLASS | ACTOR}
data: source
symtab: ID -> PACKAGE | TYPE | TRAIT | CLASS | ACTOR

USE: PATH [ID]

TYPE: ID [TYPEPARAMS] NONE [TYPES] MEMBERS
TRAIT: ID [TYPEPARAMS] cap [TYPES] MEMBERS
CLASS: ID [TYPEPARAMS] cap [TYPES] MEMBERS
ACTOR: ID [TYPEPARAMS] NONE [TYPES] MEMBERS
data: typechecking state
symtab: ID -> TYPEPARAM | FVAR | FVAL | NEW | FUN | BE

MEMBERS: {FVAR | FLET | NEW | FUN | BE}

FVAR: ID [type] [SEQ]
FLET: ID [type] [SEQ]

NEW: NONE ID [TYPEPARAMS] [PARAMS | TYPES] NONE [QUESTION] [SEQ]
BE: NONE ID [TYPEPARAMS] [PARAMS | TYPES] NONE NONE [SEQ]
FUN: cap ID [TYPEPARAMS] [PARAMS | TYPES] [type] [QUESTION] [SEQ]
symtab: ID -> TYPEPARAM | PARAM

TYPEPARAMS: {TYPEPARAM}

TYPEPARAM: ID [type] [type]

TYPES: {type}

type: (UNIONTYPE | ISECTTYPE | TUPLETYPE | NOMINAL | STRUCTURAL | THISTYPE |
  ARROW | TYPEPARAMREF)
cap: (ISO | TRN | REF | VAL | BOX | TAG)

ARROW: type type
UNIONTYPE: {type}
ISECTTYPE: {type}
TUPLETYPE: {type}
TYPEPARAMREF: ID [cap] [HAT]

NOMINAL: [ID] ID [TYPEARGS] cap [HAT]
data: definition

STRUCTURAL: MEMBERS cap [HAT]
symtab: ID -> NEW | FUN | BE

FUNTYPE: [TYPEPARAMS] [TYPES] type

TYPEARGS: {type}

PARAMS: {PARAM}

PARAM: ID type [SEQ]

IDSEQ: {ID}

SEQ: {expr}
symtab: ID -> VAR | VAL

RAWSEQ: {expr}

expr
----
data: can error or not

term: local | prefix | postfix | control | infix

CONTINUE

ERROR

BREAK: infix

RETURN: infix

infix
-----
MULTIPLY term term
DIVIDE term term
MOD term term
PLUS term term
MINUS term term
LSHIFT term term
RSHIFT term term
LT term term
LE term term
GE term term
GT term term
IS term term
EQ term term
NE term term
IS term term
ISNT term term
AND term term
XOR term term
OR term term
ASSIGN term term

local
-----
VAR: IDSEQ [type]
LET: IDSEQ [type]

prefix
------
CONSUME: term
RECOVER: term
NOT: term
MINUS: term

postfix
-------
atom
DOT postfix (ID | INT)
BANG postfix INT
QUALIFY postfix TYPEARGS
CALL postfix [POSITIONALARGS] [NAMEDARGS]

control
-------
IF: RAWSEQ SEQ [SEQ]
symtab: ID -> VAR | VAL

MATCH: RAWSEQ CASES [SEQ]

CASES: {CASE}

CASE: [SEQ] [AS] [SEQ] [SEQ]
symtab: ID -> VAR | VAL

AS: IDSEQ type

WHILE: RAWSEQ SEQ [SEQ]
symtab: ID -> VAR | VAL

REPEAT: RAWSEQ SEQ
symtab: ID -> VAR | VAL

FOR: IDSEQ [type] SEQ SEQ [SEQ]

TRY: SEQ [SEQ] [SEQ]

atom
----
TUPLE: [POSITIONALARGS] [NAMEDARGS]

ARRAY: [POSITIONALARGS] [NAMEDARGS]

OBJECT: [TYPES] MEMBERS

POSITIONALARGS: {SEQ}

NAMEDARGS: {NAMEDARG}

NAMEDARG: term SEQ

THIS
ID

INT
FLOAT
STRING

ast type
--------
NEW, BE, FUN
  when expecting a CALL
  change this?

type

*/

typedef enum
{
  AST_OK,
  AST_ERROR,
  AST_FATAL
} ast_result_t;

typedef struct ast_t ast_t;

ast_t* ast_new(token_t* t, token_id id);
ast_t* ast_blank(token_id id);
ast_t* ast_token(token_t* t);
ast_t* ast_from(ast_t* ast, token_id id);
ast_t* ast_from_string(ast_t* ast, const char* id);
ast_t* ast_dup(ast_t* ast);
void ast_scope(ast_t* ast);
bool ast_has_scope(ast_t* ast);
void ast_setid(ast_t* ast, token_id id);

token_id ast_id(ast_t* ast);
size_t ast_line(ast_t* ast);
size_t ast_pos(ast_t* ast);
void* ast_data(ast_t* ast);
void ast_setdata(ast_t* ast, void* data);
bool ast_canerror(ast_t* ast);
void ast_seterror(ast_t* ast);
void ast_inheriterror(ast_t* ast);
const char* ast_get_print(ast_t* ast);
const char* ast_name(ast_t* ast);
double ast_float(ast_t* ast);
size_t ast_int(ast_t* ast);
ast_t* ast_type(ast_t* ast);
void ast_settype(ast_t* ast, ast_t* type);

ast_t* ast_nearest(ast_t* ast, token_id id);
ast_t* ast_enclosing_type(ast_t* ast);
ast_t* ast_enclosing_method(ast_t* ast);
ast_t* ast_enclosing_method_type(ast_t* ast);
ast_t* ast_enclosing_method_body(ast_t* ast);
ast_t* ast_enclosing_loop(ast_t* ast);
ast_t* ast_enclosing_constraint(ast_t* ast);

ast_t* ast_parent(ast_t* ast);
ast_t* ast_child(ast_t* ast);
ast_t* ast_childidx(ast_t* ast, size_t idx);
ast_t* ast_childlast(ast_t* ast);
size_t ast_childcount(ast_t* ast);
ast_t* ast_sibling(ast_t* ast);
ast_t* ast_previous(ast_t* ast);
size_t ast_index(ast_t* ast);

void* ast_get(ast_t* ast, const char* name);
bool ast_set(ast_t* ast, const char* name, void* value);
bool ast_merge(ast_t* dst, ast_t* src);
void ast_clear(ast_t* ast);

ast_t* ast_add(ast_t* parent, ast_t* child);
ast_t* ast_add_sibling(ast_t* older_sibling, ast_t* new_sibling);
ast_t* ast_pop(ast_t* ast);
ast_t* ast_append(ast_t* parent, ast_t* child);
void ast_swap(ast_t* prev, ast_t* next);
void ast_replace(ast_t** prev, ast_t* next);
void ast_reverse(ast_t* ast);
void ast_print(ast_t* ast, size_t width);
void ast_free(ast_t* ast);
void ast_free_unattached(ast_t* ast);

void ast_error(ast_t* ast, const char* fmt, ...)
  __attribute__ ((format (printf, 2, 3)));

typedef ast_result_t (*ast_visit_t)(ast_t** astp);

ast_result_t ast_visit(ast_t** ast, ast_visit_t pre, ast_visit_t post);

void ast_get_children(ast_t* parent, size_t child_count,
  ast_t*** out_children);

#define AST_GET_CHILDREN(parent, ...) \
  { \
    ast_t** children[] = {__VA_ARGS__}; \
    ast_get_children(parent, sizeof(children)/sizeof(ast_t**), children); \
  }

void ast_extract_children(ast_t* parent, size_t child_count,
  ast_t*** out_children);

#define AST_EXTRACT_CHILDREN(parent, ...) \
  { \
    ast_t** children[] = {__VA_ARGS__}; \
    ast_extract_children(parent, sizeof(children)/sizeof(ast_t**), children); \
  }


// Macros for building ASTs
// TODO: Move these somewhere else?

/** The macros below allow for building arbitrarily complex ASTs with a simple,
 * S-expression like syntax.
 *
 * At the tope level there must be exactly one of:
 * BUILD      builds an AST and creates a variable in which to store it.
 * REPLACE    builds an AST with which it replaces the specified existing tree.
 *
 * Within these macros the following are used to build up the tree:
 * NODE       creates a node with a specified token ID and optionally children.
 * TREE       inserts an already built subtree.
 * ID         adds a TK_ID node with the given ID name.
 * NONE       is syntactic sugar to add a TK_NONE node.
 * AST_SCOPE  adds a symbol table to the enclosing node.
 */

/** Builds an AST to replace the specified existing tree.
 * A variable with the name given by var is defined and the created tree is
 * stored in it.
 * An existing node must be provided in basis_ast, all created nodes are based
 * on this.
 */
#define BUILD(var, basis_ast, ...) \
  ast_t* var; \
  { \
    ast_t* parent = NULL; \
    ast_t* last_sibling = NULL; \
    ast_t* node = NULL; \
    __VA_ARGS__ \
    var = parent; \
  }

/** Builds an AST to replace the specified existing tree.
 * The provided existing must be an ast_t**.
 */
#define REPLACE(existing, ...) \
  { \
    ast_t* basis_ast = *existing; \
    ast_t* parent = NULL; \
    ast_t* last_sibling = NULL; \
    ast_t* node = NULL; \
    __VA_ARGS__ \
    ast_replace(existing, parent); \
  }


/** Add an existing subtree.
 * If the given tree is already part of another tree it will be copied
 * automatically. If it is a complete tree it will not.
 */
#define TREE(tree) \
  { \
    if(parent == NULL) parent = tree; \
    else if(last_sibling == NULL) last_sibling = ast_add(parent, tree); \
    else last_sibling = ast_add_sibling(last_sibling, tree); \
  }

/// Add a new node with the specified token ID and optionally children
#define NODE(id, ...) \
  { \
    node = ast_from(basis_ast, id); \
    TREE(node); \
    { \
      ast_t* parent = node; \
      ast_t* last_sibling = NULL; \
      ast_t* node = NULL; \
      __VA_ARGS__ \
      (void)parent; \
      (void)last_sibling; \
      (void)node; \
    } \
  }

/// Add a TK_NONE node with no children
#define NONE TREE(ast_from(basis_ast, TK_NONE));

/// Add a TK_ID node with the given ID name
#define ID(name) TREE(ast_from_string(basis_ast, name));

/// Add a symbol table to the enclosing node
#define AST_SCOPE ast_scope(parent);

#endif
