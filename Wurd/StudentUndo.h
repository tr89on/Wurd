#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

// STUDENT ADDED THINGS
#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    struct Edit {
        Edit(Action action, int r, int c, std::string text, int count) {
            m_action = action;
            m_row = r;
            m_col = c;
            m_text = text;
            m_count = count;
        }
        Action m_action;
        int m_row;
        int m_col;
        int m_count;
        std::string m_text;
    }; 
    std::stack<Edit> m_undos;
};

#endif // STUDENTUNDO_H_
