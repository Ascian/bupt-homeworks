%{
    #include<stdio.h>
    #include<ctype.h>
    #include "lex.yy.c"
%}
%token DIGIT
%%
line:expr'\n'   {printf("ACCEPT\n"); return 0;}
;
expr:expr'+'term    {printf("E->E+T\n");}
    |expr'-'term    {printf("E->E-F\n");}
    |term           {printf("E->T\n");}
    ;
term:term'*'factor  {printf("T->T*F\n");}
    |term'/'factor  {printf("T->T/F\n");}
    |factor         {printf("T->F\n");}
    ;
factor:'('expr')'   {printf("F->(E)\n");}
    |DIGIT          {printf("F->num\n");}
    ;
%%
main()
{
    return yyparse();
}

void yyerror(char* s)
{
    printf("%s\n", s);
}
