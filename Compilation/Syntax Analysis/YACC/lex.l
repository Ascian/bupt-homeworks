%{
    #include"y.tab.h"
%}

digit   [0-9]
%%
{digit}+ {return DIGIT;}
"+"      {return '+';}
"-"      {return '-';}
"*"      {return '*';}
"/"      {return '/';}
"("      {return '(';}
")"      {return ')';}
"\n"     {return '\n';}
%%
int yywrap()
{
    return 1;
}