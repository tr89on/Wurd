#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    // batching
    if (!m_undos.empty()) {
        Edit top = m_undos.top();
        if (action == top.m_action && row == top.m_row) { // batching only happens on the same row
            if (action == Action::INSERT && col == top.m_col+1) { // if the action was insert() then the cursor moved right 1
                m_undos.pop();
                top.m_count++;
                top.m_row = row;
                top.m_col = col;
                m_undos.push(top); // push the batched INSERT
                return;
            } else if (action == Action::DELETE) {
                if (col == top.m_col) { // if the action was del() then the cursor stays in the same place
                    top.m_text = top.m_text + ch;
                    m_undos.pop();
                    top.m_row = row;
                    top.m_col = col;
                    m_undos.push(top);
                    return;
                } else if (col == top.m_col-1) { // if the action was backspace() then the cursor moved back 1
                    top.m_text = ch + top.m_text;
                    m_undos.pop();
                    top.m_row = row;
                    top.m_col = col;
                    m_undos.push(top);
                    return;
                }
            }
        }
    }
    
    std::string text = "";
    text += ch;
    Edit newEdit(action, row, col, text, 1);
    
    m_undos.push(newEdit);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if (m_undos.empty()) return Action::ERROR;
    
    Edit top = m_undos.top();
    m_undos.pop();
    
    // set the appropriate Undo action
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
    
    return act; 
}

void StudentUndo::clear() {
    while (!m_undos.empty()) {
        m_undos.pop();
    }
}
