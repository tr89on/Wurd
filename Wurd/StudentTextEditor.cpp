#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

// STUDENT ADDITIONS
#include <iostream>
#include <fstream>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
     
     m_lines.push_back("");
     resetCursorPos();
     m_is_undo = false;
}

StudentTextEditor::~StudentTextEditor()
{
    reset();
}

bool StudentTextEditor::load(std::string file) {
    ifstream infile(file);
    if (!infile) return false;
    
    reset();
    
    string s;
    while (getline(infile, s)) {
        // strip \r character
        if (s.find('\r') != string::npos) {
            s = s.substr(0, s.find('\r'));
        }
        
        string newline = "";
        for (char c : s) {
            if (c == '\t') { // handle tab characters
                for (int i = 0; i < 4; i++) {
                    newline += " ";
                }
            } else {
                newline += c;
            }
        }
        
        m_lines.push_back(newline);
    }
    
    if (m_lines.empty()) m_lines.push_back(""); // Handle empty file case

    resetCursorPos();
    
	return true;
}

bool StudentTextEditor::save(std::string file) {
    ofstream outfile(file);
    if (!outfile) return false;
    
    for (auto line : m_lines) {
        string s = line + '\n';
        outfile << s;
    }
    
    return true;
}

void StudentTextEditor::reset() {
    m_lines.clear(); // clear all the lines of text (no dynamic allocation)
    resetCursorPos();
    
    getUndo()->clear();
}

void StudentTextEditor::move(Dir dir) {
    if (dir == Dir::UP) {
        if (m_row == 0) return; // at the top row
        decRow();
        if (m_pos->size() < m_col) m_col = m_pos->size();
    } else if (dir == Dir::DOWN) {
        if (m_row == m_lines.size()-1) return; // at the bottom row
        incRow();
        if (m_pos->size() < m_col) m_col = m_pos->size();
    } else if (dir == Dir::LEFT) {
        if (m_col == 0) { // at the very left
            if (m_row != 0) { // not at the top row
                decRow();
                m_col = m_pos->size();
            }
            return;
        }
        m_col--;
    } else if (dir == Dir::RIGHT) {
        if (m_col >= m_pos->size()) { // at the very right
            if (m_row != m_lines.size() - 1) { // not at the bottom row
                incRow();
                m_col = 0;
            }
            return;
        }
        m_col++;
    } else if (dir == Dir::HOME) {
        m_col = 0;
    } else if (dir == Dir::END) {
        m_col = m_pos->size();
    }
}

void StudentTextEditor::del() {
    if (m_col == m_pos->size()) { // at the very right
        if (m_row != m_lines.size()-1) { // not at the bottom row
            string line = *m_pos;
            m_pos = m_lines.erase(m_pos);
            *m_pos = line + *m_pos;
            
            // push JOIN to undo
            if (!m_is_undo) getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\n');
        }
        return;
    }
    
    string line = *m_pos;
    char deleted_char = line.at(m_col);
    *m_pos = line.substr(0, m_col) + line.substr(m_col+1);
    
	// push DELETE to undo only if not currently undoing
    if (!m_is_undo) getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deleted_char);
}

void StudentTextEditor::backspace() {
    if (m_col == 0) { // at the very left
        if (m_row != 0) { // not at the top row
            string line = *m_pos;
            m_pos = m_lines.erase(m_pos);
            decRow();
            m_col = m_pos->length();
            *m_pos += line;
            
            // push JOIN to undo
            if (!m_is_undo) getUndo()->submit(Undo::Action::JOIN, m_row, m_col, '\n');
        }
        
        return;
    }
    
    string line = *m_pos;
    char deleted_char = line.at(m_col-1);
    *m_pos = line.substr(0, m_col-1) + line.substr(m_col);
    m_col--;
    
    // push BACKSPACE to undo only if not currently undoing
    if (!m_is_undo) getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deleted_char);
}

void StudentTextEditor::insert(char ch) {
    if (ch == '\t') {
        for (int i = 0; i < 4; i++) {
            insert(' ');
        }
        return;
    }
    
    string line = *m_pos;
    *m_pos = line.substr(0, m_col) + ch + line.substr(m_col);
    m_col++;
    
    // push INSERT to undo only if not currently undoing
    if (!m_is_undo) getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() {
    string line = *m_pos;
    string newline = "" + line.substr(m_col); // split line at the current character
    *m_pos = line.substr(0, m_col); // 1st half remains on current line
    
    // push SPLIT to undo only if not currently undoing (push BEFORE enter pressed)
    if (!m_is_undo) getUndo()->submit(Undo::Action::SPLIT, m_row, m_col, '\n');
    
    m_lines.insert(std::next(m_pos), "");
    incRow();
    m_col = 0;
    *m_pos += newline; // 2nd half ends up on new line
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row = m_row;
    col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if (startRow < 0 || numRows < 0) return -1;
    if (startRow > m_lines.size()) return -1;
    
    lines.clear(); // clear the old lines
    
    int r = m_row;
    auto it = m_pos;
    while (r < startRow) {
        r++;
        it++;
    }
    while (r > startRow) {
        r--;
        it--;
    }
    
    for ( ; it != m_lines.end() && r < startRow+numRows; it++, r++) {
        lines.push_back(*it);
    }
    
    return lines.size(); // return number of lines in lines parameter
}

void StudentTextEditor::undo() {
    int count;
    string text;
    int start_row;
    Undo::Action act = getUndo()->get(start_row, m_col, count, text);
    if (act == Undo::Action::ERROR) return;
    int orig_col = m_col; // keep track of original col
    
    m_is_undo = true;
    
    while (m_row < start_row) {
        incRow();
    }
    while (m_row > start_row) {
        decRow();
    }
    
    if (act == Undo::Action::INSERT) {
        for (char c : text) {
            insert(c);
        }
        m_col = orig_col;
    } else if (act == Undo::Action::DELETE) {
        for (int i = 0; i < count; i++) {
            backspace();
        }
    } else if (act == Undo::Action::SPLIT) {
        enter();
        if (m_row != 0) {
            decRow();
            m_col = m_pos->size();
        }
    } else if (act == Undo::Action::JOIN) {
        del();
    }
    
    m_is_undo = false;
}

void StudentTextEditor::resetCursorPos() {
    m_row = 0;
    m_col = 0;
    m_pos = m_lines.begin();
}

void StudentTextEditor::incRow() {
    m_row++;
    m_pos++;
}

void StudentTextEditor::decRow() {
    m_row--;
    m_pos--;
}
