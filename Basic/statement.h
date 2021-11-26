/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <iostream>
#include <string>
#include <cstring>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"

using namespace std;
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */
enum StatementType {
    PRINT, QUIT, CLEAR, LIST, REM, LET, GOTO, IF, END
};


class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(string line, Program &program, EvalState &state, TokenScanner &scanner) = 0;

};


class Print : public Statement {
private:
    string str;
public:
    Print(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Clear : public Statement {
    string str;
public:
    Clear(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Quit : public Statement {
private:
    string str;
public:
    Quit(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class List : public Statement {
    string str;
public:
    List(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Rem : public Statement {
    string str;
public:
    Rem(string str);


    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Let : public Statement {
    string str;
public:
    Let(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Input : public Statement {
    string str;
public:
    Input(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Run : public Statement {
    string str;
public:
    Run(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class GoTo : public Statement {
    string str;
public:
    GoTo(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class End : public Statement {
    string str;
public:
    End(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class If : public Statement {
    string str;
public:
    If(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

class Error : public Statement {
    string str;
public:
    Error(string str);

    void execute(string line, Program &program, EvalState &state, TokenScanner &scanner);
};

Statement *getStateTpye(string cmd);
/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */



#endif
