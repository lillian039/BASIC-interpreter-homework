/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include <cstring>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);

/* Main program */
bool flag_run = false;
int number_run;

int main() {
    EvalState state;
    Program program;
/*    cout << "Stub implementation of BASIC" << endl;*/
    while (true) {
        try {
            string input = getLine();
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string cmd = scanner.nextToken();
    if (0 <= cmd[0] - 'A' && cmd[0] - 'A' <= 26) {
        if (cmd == "PRINT") {
            Expression *exp = parseExp(scanner);
            int value = exp->eval(state);
            cout << value << endl;
            delete exp;
        } else if (cmd == "QUIT") {
            exit(0);
        } else if (cmd == "CLEAR") {
            program.clear();
            state.clearValue();
        } else if (cmd == "RUN") {
            flag_run = true;
            number_run = program.getFirstLineNumber();
            line = program.getSourceLine(number_run);
            processLine(line, program, state);
            while (!program.nextEnd(number_run) && flag_run) {
                number_run = program.getNextLineNumber(number_run);
                line = program.getSourceLine(number_run);
                processLine(line, program, state);
            }
            flag_run = false;
        } else if (cmd == "LIST") {
            program.list();
        } else if (cmd == "REM") {
            return;
        } else if (cmd == "LET") {
            string name = scanner.nextToken();
            if (name == "LET") {
                cout << "SYNTAX ERROR\n";
                throw ErrorException("SYNTAX ERROR");
            }
            int len = name.size();
            line.erase(0, len + 6);//have blankspace:6
            scanner.setInput(line);
            Expression *exp = parseExp(scanner);
            int value = exp->eval(state);
            state.setValue(name, value);
        } else if (cmd == "INPUT") {
            string name = scanner.nextToken();
            cout << " ? ";
            string num;
            num = getLine();
            int len = num.size();
            bool integer = true;
            if ((num[0] < '0' || num[0] > '9') && num[0] != '-')integer = false;
            for (int i = 1; i < len; i++) {
                if (num[i] < '0' || num[i] > '9') {
                    integer = false;
                    break;
                }
            }
            while (!integer) {
                integer = true;
                cout << "INVALID NUMBER\n";
                cout << " ? ";
                num = getLine();
                len = num.size();
                if ((num[0] < '0' || num[0] > '9') && num[0] != '-')integer = false;
                for (int i = 1; i < len; i++) {
                    if (num[i] < '0' || num[i] > '9') {
                        integer = false;
                        break;
                    }
                }
            }
            int number = stringToInteger(num);
            state.setValue(name, number);
        } else if (cmd == "END" && flag_run) {
            flag_run = false;
        } else if (cmd == "GOTO" && flag_run) {
            int number_tmp = stringToInteger(scanner.nextToken());
            if (program.numberExist(number_tmp)) {
                number_run = number_tmp;
                line = program.getSourceLine(number_run);
                processLine(line, program, state);
                while (!program.nextEnd(number_run) && flag_run) {
                    number_run = program.getNextLineNumber(number_run);
                    line = program.getSourceLine(number_run);
                    processLine(line, program, state);
                }
                flag_run = false;
            } else cout << "LINE NUMBER ERROR\n";
        } else if (cmd == "IF" && flag_run) {
            int len = 0;
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '=' || line[i] == '<' || line[i] == '>')break;
                len++;
            }
            char optr = line[len];
            string sum_1 = line.substr(3, len - 3);
            /*       cout<<sum_1;*/
            TokenScanner scanner_1;
            scanner_1.ignoreWhitespace();
            scanner_1.scanNumbers();
            scanner_1.setInput(sum_1);
            Expression *exp_1 = parseExp(scanner_1);
            int value_1 = exp_1->eval(state);
            /*     cout << value_1 << endl;*/
            int len_2 = 0;
            for (int i = len; i < line.size(); i++) {
                if (line[i] == 'T' && line[i + 1] == 'H')break;
                len_2++;
            }
            string sum_2 = line.substr(len + 1, len_2 - 1);
            /*         cout<<sum_2;*/
            TokenScanner scanner_2;
            scanner_2.ignoreWhitespace();
            scanner_2.scanNumbers();
            scanner_2.setInput(sum_2);
            Expression *exp_2 = parseExp(scanner_2);
            int value_2 = exp_2->eval(state);
            /*       cout << value_2 << endl;*/
            int line_num = stringToInteger(line.substr((len + len_2 + 4), line.size()));
            /*          cout<<line_num;//已获得所有需要的字符*/
            if ((optr == '=' && value_1 == value_2) || (optr == '<' && value_1 < value_2) ||
                (optr == '>' && value_1 > value_2)) {
                int number_tmp = line_num;
                if (program.numberExist(number_tmp)) {
                    number_run = number_tmp;
                    line = program.getSourceLine(number_run);
                    processLine(line, program, state);
                    while (!program.nextEnd(number_run) && flag_run) {
                        number_run = program.getNextLineNumber(number_run);
                        line = program.getSourceLine(number_run);
                        processLine(line, program, state);
                    }
                    flag_run = false;
                } else cout << "LINE NUMBER ERROR\n";
            }
        } else cout << "SYNTAX ERROR\n";
        //错误输出
    } else if (0 <= cmd[0] - '0' && cmd[0] - '0' <= 9) {
        int line_num = stringToInteger(cmd);
        int line_len = line.size();
        int len_cmd = cmd.size();
        if (len_cmd == line_len) {
            program.removeSourceLine(line_num);
            return;
        }
        cmd = line.erase(0, len_cmd + 1);
        program.addSourceLine(line_num, cmd);
    }
}
