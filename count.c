#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum States {
    inText,
    inString,
    goingToEnterParenthesis,
    inParenthesis,
    goingToLeaveParenthesis,
    inBrace,
    goingToEnterDouble,
    inDouble,  
};

int main() {
    enum States curState = inText;
    int8_t curChar = 0;

    uint32_t braceCounter       = 0;
    uint32_t stringCounter      = 0;
    uint32_t parenthesisCounter = 0;
    uint32_t doubleCounter      = 0;


    while ((curChar = getchar()) != EOF) {
        switch(curState) {
            case inText:
                switch (curChar) {
                    case '\'':
                        curState = inString;
                        break;
                    case '(':
                        curState = goingToEnterParenthesis;
                        break;
                    case '{':
                        curState = inBrace;
                        break;
                    case '/':
                        curState = goingToEnterDouble;
                        break;
                    default:
                        break;
                }
                break;
            case inString:
                if (curChar == '\'') {
                    curState = inText;
                    stringCounter++;
                }
                break;
            case goingToEnterParenthesis:
                switch (curChar) {
                    case '\'':
                        curState = inString;
                        break;
                    case '*':
                        curState = inParenthesis;
                        break;
                    default:
                        curState = inText;
                        break;
                }
                break;
            case inParenthesis:
                if (curChar == '*') 
                    curState = goingToLeaveParenthesis;
                break;
            case goingToLeaveParenthesis:
                if (curChar == ')') {
                    parenthesisCounter++;
                    curState = inText;
                }
                else
                    curState = inParenthesis;
                break;
            case inBrace:
                if (curChar == '}') {
                    braceCounter++;
                    curState = inText;
                }
                break;
            case goingToEnterDouble:
                switch (curChar) {
                    case '\'':
                        curState = inString;
                        break;
                    case '/':
                        curState = inDouble;
                        break;
                    default:
                        curState = inText;
                        break;
                }
                break;
            case inDouble:
                if (curChar == '\n') {
                    doubleCounter++;
                    curState = inText;
                }
                break;
        }
    }

    printf("%u %u %u %u\n", parenthesisCounter, braceCounter, doubleCounter, stringCounter);
}