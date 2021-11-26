/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <iostream>
#include <string>
#include <cstring>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"
#include "../StanfordCPPLib/simpio.h"

using namespace std;

bool flag_run = false;
int number_run;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

Statement *getStateTpye(string cmd) {
    if (cmd == "PRINT")return new Print(cmd);
    else if (cmd == "QUIT")return new Quit(cmd);
    else if (cmd == "LIST")return new List(cmd);
    else if (cmd == "REM")return new Rem(cmd);
    else if (cmd == "LET")return new Let(cmd);
    else if (cmd == "CLEAR")return new Clear(cmd);
    else if (cmd == "INPUT")return new Input(cmd);
    else if (cmd == "RUN")return new Run(cmd);
    else if (cmd == "GOTO" && flag_run)return new GoTo(cmd);
    else if (cmd == "IF" && flag_run)return new If(cmd);
    else if (cmd == "END" && flag_run)return new End(cmd);
    else return new Error(cmd);
}

void ProcessLine(string line, Program &program, EvalState &state);

Print::Print(string str) {
    this->str = str;
}

void Print::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
    Expression *exp = parseExp(scanner);
    int value = exp->eval(state);
    cout << value << endl;
    delete exp;
}

Quit::Quit(string str) {
    this->str = str;
}

void Quit::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
    exit(0);
}

List::List(string str) {
    this->str = str;
}

void List::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
    program.list();
}

Rem::Rem(string str) {
    this->str = str;
}

void Rem::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
    //empty
}

Let::Let(string str) {
    this->str = str;
}

void Let::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
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
}

Clear::Clear(string str) {
    this->str = str;
}

void Clear::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
    program.clear();
    state.clearValue();
}

Input::Input(string str) {
    this->str = str;
}

void Input::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
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
}

Run::Run(string str) {
    this->str = str;
}

void Run::execute(string line, Program &program, EvalState &state, TokenScanner &scanner) {
    flag_run = true;
    number_run = program.getFirstLineNumber();
    line = program.getSourceLine(number_run);
    ProcessLine(line, program, state);
    while (!program.nextEnd(number_run) && flag_run) {
        number_run = program.getNextLineNumber(number_run);
        line = program.getSourceLine(number_run);
        ProcessLine(line, program, state);
    }
    flag_run = false;
}

GoTo::GoTo(string str) {
    this->str = str;
}

void GoTo::execute(string line, Program &program, EvalState &state, TokenScanner &scanner){
    int number_tmp = stringToInteger(scanner.nextToken());
    if (program.numberExist(number_tmp)) {
        number_run = number_tmp;
        line = program.getSourceLine(number_run);
        ProcessLine(line, program, state);
        while (!program.nextEnd(number_run) && flag_run) {
            number_run = program.getNextLineNumber(number_run);
            line = program.getSourceLine(number_run);
            ProcessLine(line, program, state);
        }
        flag_run = false;
    } else cout << "LINE NUMBER ERROR\n";
}

End::End(string str) {
    this->str = str;
}

void End::execute(string line, Program &program, EvalState &state, TokenScanner &scanner){
    flag_run = false;
}

If::If(string str) {
    this->str = str;
}

void If::execute(string line, Program &program, EvalState &state, TokenScanner &scanner){
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
            ProcessLine(line, program, state);
            while (!program.nextEnd(number_run) && flag_run) {
                number_run = program.getNextLineNumber(number_run);
                line = program.getSourceLine(number_run);
                ProcessLine(line, program, state);
            }
            flag_run = false;
        } else cout << "LINE NUMBER ERROR\n";
    }
}

Error::Error(string str) {
    this->str = str;
}

void Error::execute(string line, Program &program, EvalState &state, TokenScanner &scanner){
  /*  cout<<"SYNTAX ERROR\n";
}

void ProcessLine(string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string cmd = scanner.nextToken();
    Statement *cmd_parse = getStateTpye(cmd);
    cmd_parse->execute(line, program, state, scanner);
}