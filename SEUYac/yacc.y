%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;
%}

%token INTEGER
%token ID
%token ( + - * / ) , { } = > <
%left '+' '-'
%left '*' '/'


%%
S:E

E:T
 |E A T


T:F
|T M F


F:( E )
 |INTEGER
 |ID

A:+
 |-

M:*
 |/

%%

main()
{
	if(!yyparse())
		printf("completed parse\n");
	else
		printf("illegal expression\n");
}