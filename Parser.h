#ifndef ___parser_h
#define ___parser_h

#include"Token.h"

extern Token token;
extern int errF;

typedef struct Node{
	Kind kind;
	struct Node* lhs;
	struct Node* rhs;
	int val;
}Node;

Node* statement(void);

Node* expression(void);

Node* term(void);

Node* factor(void);

void chkTkn(Kind op);

Node* new_node(Kind kind,Node* lhs,Node* rhs);

Node* new_node_num(Kind kind,int val);

void printNode(Node *node);

#endif
