/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"

using namespace std;

Program::Program() {
    // Replace this stub with your own code
}

Program::~Program() {
    // Replace this stub with your own code
}

void Program::clear() {
    lines.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    lines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    lines.erase(lineNumber);
    // Replace this stub with your own code
}

string Program::getSourceLine(int lineNumber) {
    return lines[lineNumber];    // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber) {
    return NULL;  // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
    auto iter=lines.begin();
    return iter->first;// Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
    auto iter=lines.begin();
    while(iter->first!=lineNumber){
        iter++;
    }
    iter++;
    return iter->first;
}

bool Program::nextEnd(int number){
    auto iter=lines.begin();
    while(iter->first!= number){
        iter++;
    }
    if(++iter==lines.end())return true;
    else return false;
}

void Program::list(){
    auto iter=lines.begin();
    while(iter!=lines.end()){
        cout<<iter->first<<" "<<iter->second<<'\n';
        iter++;
    }
}

bool Program::numberExist(int number){
    if(lines[number].size()!=0)return true;
    else return false;
}
