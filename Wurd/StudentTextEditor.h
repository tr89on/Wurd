#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"

class Undo;

#include <iostream>
#include <list>

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
    
    std::list<std::string> m_lines;
    std::list<std::string>::iterator m_pos;
    int m_row;
    int m_col;
    
    bool m_is_undo;
};

#endif // STUDENTTEXTEDITOR_H_
