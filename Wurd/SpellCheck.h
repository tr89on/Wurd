#ifndef SPELLCHECK_H_
#define SPELLCHECK_H_

#include <string>
#include <vector>

class SpellCheck {
public:
	struct Position {
		int start;
		int end; // inclusive
	};

	SpellCheck() { }
	virtual ~SpellCheck() { }

	virtual bool load(std::string dictionaryFile) = 0;
	virtual bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions) = 0;
	virtual void spellCheckLine(const std::string& line, std::vector<Position>& problems) = 0;

private:

};

SpellCheck* createSpellCheck();

#endif  // SPELLCHECK_H_
