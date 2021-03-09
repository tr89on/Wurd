#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    void addWordToDict(std::string word);
    bool inDict(std::string word);
    int getCharIndex(char c);
    bool isWordChar(char c);
    std::string getFormattedWord(std::string s);
    
    struct Node {
        Node(char c) {
            m_val = c;
            m_ending = false;
            for (int i = 0; i < 27; i++) {
                children[i] = nullptr;
            }
        }
        // TODO: free up dynamically allocated memory
//        ~Node() {
//            for (int i = 0; i < 27; i++) {
//                if (children[i] != nullptr) delete children[i];
//            }
//        }
        char m_val;
        bool m_ending;
        Node* children[27];
    };
    
    Node* m_head = new Node('.');
};

#endif  // STUDENTSPELLCHECK_H_
