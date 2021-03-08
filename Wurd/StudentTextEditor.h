#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"

class Undo;

#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
    void resetCursorPos();
    void incRow();
    void decRow();
    
    list<string> m_lines;
    list<string>::iterator m_pos;
    int m_row;
    int m_col;
};

#endif // STUDENTTEXTEDITOR_H_
