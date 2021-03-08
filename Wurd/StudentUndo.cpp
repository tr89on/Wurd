#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
	// TODO
}
