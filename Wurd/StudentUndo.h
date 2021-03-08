#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

// STUDENT ADDED THINGS
#include <stack>
using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    struct Edit {
        Edit(Action action, int r, int c,  int count, string text) {
            m_action = action;
            m_row = r;
            m_col = c;
            m_count = count;
            m_text = text;
        }
        Action m_action;
        int m_row;
        int m_col;
        int m_count;
        string m_text;
    }; 
    stack<Edit> m_edits;
};

#endif // STUDENTUNDO_H_
