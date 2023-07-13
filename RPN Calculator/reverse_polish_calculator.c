#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100       // max size of operands and operators
#define NUMBER '0'      // flag that a number was found
#define MAXVAL 100      // max size for stack

int sp = 0;             // next free stack position
double val[MAXVAL];     // value stack

void push(double num);
double pop();


int main() {


    return 0;
}

// get next operator or numeric operand
int getop(char s[]) {
    int i, c;

    while( (s[0] = c = getch()) == ' ' || c == '\t' ) { }
    s[1] = '\0';
    // case 1: input is not a number
    if(!isdigit(c) && c != '.') {
        return c;
    }
    i = 0;
    // case 2: input is an integer
    if(isdigit(c)) {
        while(isdigit(s[++i] = c = getch())) { }
    }
    //case 3: input is a fraction
    if(c == '.') {
        while(isdigit(s[++i] = c = getch())) { }
    }
    s[i] = '\0';
    if(c != EOF) {
        ungetch(c);
    }
    return NUMBER;
}

void push(double num) {
    if(sp < MAXVAL) {
        val[sp++] = num;
    }
    else {
        printf("Stack full.\n");
        return;
    }
}

double pop() {
    if(sp > 0) {
        return val[--sp];
    }
    else {
        printf("Stack empty.\n");
        return 0;
    }
}