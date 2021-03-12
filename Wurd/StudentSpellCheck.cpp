#include "StudentSpellCheck.h"
#include <string>
#include <vector>

// STUDENT ADDITIONS
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
    if (m_head != nullptr) delete m_head;
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    ifstream infile(dictionaryFile);
    if (!infile) return false;
    
    // clear old dictionary
    if (m_head != nullptr) delete m_head;
    m_head = new Node('.'); // initialize new dummy node
    
    string s;
    while (getline(infile, s)) {
        addWordToDict(s);
    }
    
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    if (inDict(word)) return true; // word in the dictionary so no need to find suggestions
    
    suggestions.clear();
    
    int numAdded = 0; // number of suggestions added
    int len = 27; // # of valid substitution characters
    for (int pos = 0; pos < word.size(); pos++) {
        for (int i = 0; i < len; i++) {
            char letter = 'a' + i; // try substituting 'a'-'z'
            if (i == len-1) letter = '\''; // handle apostrophe case
            
            string suggestion = word.substr(0,pos) + letter + word.substr(pos+1); // substitute a different letter at pos in word
            if (numAdded < max_suggestions && inDict(suggestion)) { // add the valid suggestion if needed
                numAdded++;
                suggestions.push_back(suggestion);
            }
        }
    }
    
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    int start = 0;
    for (int end = 0; end <= line.size(); end++) { // 2-pointers approach to partitioning words
        if (end == line.size() || !isWordChar(line.at(end))) { // split words by non-word characters (or the end of the line)
            string word = line.substr(start, end-start);
            
            if (!inDict(word)) {
                Position p;
                p.start = start;
                p.end = end-1;
                problems.push_back(p); // adds a copy of p to problems
            }
            
            start = end+1; // update the start pointer
        }
    }
}

void StudentSpellCheck::addWordToDict(string word) {
    string w = getFormattedWord(word);
    Node* curr = m_head;
    for (auto c : w) {
        int index = getCharIndex(c);
        if (curr->children[index] == nullptr) {
            curr->children[index] = new Node(c);
        }
        curr = curr->children[index];
    }
    curr->m_ending = true; // mark the ending of a valid word
}

bool StudentSpellCheck::inDict(string word) {
    string w = getFormattedWord(word);
    Node* curr = m_head;
    for (auto c : w) {
        int index = getCharIndex(c);
        if (curr->children[index] == nullptr) return false; // next char cannot follow current char
        curr = curr->children[index];
    }
    return curr->m_ending; // check if word is actually a word
}

int StudentSpellCheck::getCharIndex(char c) {
    return c == '\'' ? 26 : (c - 'a');
}

bool StudentSpellCheck::isWordChar(char c) {
    return isalpha(c) || c == '\'';
}

// strip the invalid (non a-z or apostrophe) characters from a word
string StudentSpellCheck::getFormattedWord(string s) {
    string lower = "";
    for (char c : s) {
        if (isWordChar(c)) {
            lower += tolower(c);
        }
    }
    return lower;
}
