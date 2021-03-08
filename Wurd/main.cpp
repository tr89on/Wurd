#include "EditorGui.h"
#include "TextIO.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

// Do not change anything in this file other than these initializer values
const char* DICTIONARYPATH = "dictionary.txt";
const int FOREGROUND_COLOR = COLOR_WHITE;
const int BACKGROUND_COLOR = COLOR_BLACK;
const int HIGHLIGHT_COLOR  = COLOR_RED;
// Choices are COLOR_x, where x is WHITE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN

bool neededToRelaunchAndDid() {
	char* needToRelaunch = getenv("NEEDTORELAUNCH");
	if (needToRelaunch == nullptr  ||  strcmp(needToRelaunch, "yes") != 0) {
		return false;
	}
	int status = std::system("/usr/bin/osascript << END_END_END 2>&1 | grep -v 'Falling back'\n"
		"tell application \"Terminal\"\n"
		"  set T to do script \""
				"unset NEEDTORELAUNCH; "
				"cd \\\"$WURDDIR\\\"  ||  { "
					"echo Error: Can not change to directory \\\"$WURDDIR\\\"; "
					"exit 1; "
				"}; "
				"./Wurdspawner; "
				"exit\"\n"
		"  set W to the id of window 1 where its tab 1 = T\n"
		"  delay 2\n"
		"  repeat until T is not busy\n"
		"    delay 2\n"
		"  end repeat\n"
		"  delay 1\n"  // change this to a larger number of seconds if you want to keep the window open longer
		"  close window id W\n"
		"end tell\nEND_END_END\n");
	std::cout << "Terminal window ended with exit code: " << status << std::endl;
	return true;
}

int main(int argc, char* argv[]) {
	if (neededToRelaunchAndDid())
		return 0;
	char* term = std::getenv("TERM");
	if (term == nullptr  ||  std::strncmp(term, "xterm", 5) != 0) {
		std::cout << "TERM variable does not start with 'xterm'" << std::endl;
		return 1;
	}

	TextIO ti(FOREGROUND_COLOR, BACKGROUND_COLOR, HIGHLIGHT_COLOR);

	EditorGui editor(LINES, COLS);

	if (!editor.loadDictionary(DICTIONARYPATH)) {
		editor.writeStatus(std::string("Error: Can not load dictionary ") + DICTIONARYPATH);
	}
	if (argc == 2) {
		editor.loadFileToEdit(argv[1]);
	}
	editor.run();
}
