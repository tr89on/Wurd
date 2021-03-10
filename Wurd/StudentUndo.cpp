#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    Edit top = m_undos.top();
    // TODO: implement batching
    if (action == Action::INSERT) {
        if (top.m_action == Action::INSERT) {
            
        }
    }
    
    string text = "";
    text += ch;
    Edit newEdit(action, row, col, text, 1);
    
    m_undos.push(newEdit);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if (m_undos.empty()) return Action::ERROR;
    
    Edit top = m_undos.top();
    m_undos.pop();
    Action act;
    if (top.m_action == Action::INSERT) act = Action::DELETE;
    else if (top.m_action == Action::DELETE) act = Action::INSERT;
    else if (top.m_action == Action::SPLIT) act = Action::JOIN;
    else if (top.m_action == Action::JOIN) act = Action::SPLIT;
    else return Action::ERROR;
    
    row = top.m_row;
    col = top.m_col;
    count = top.m_count;
    text = top.m_text;
    
    return act;  // TODO
}

void StudentUndo::clear() {
    while (!m_undos.empty()) {
        m_undos.pop();
    }
}
