#pragma once

#include <vector>
#include <string>
#include <algorithm>

class Utils
{
public:

	/* unsigned int split
	 * split a string (txt), everywhere are certain character is found
	 * store the resulting substrings in a vector
	 * return the size of the vector
	 */

	static unsigned int split(const std::string& txt, std::vector<std::string>& strs, char ch) {
		int pos = txt.find(ch);
		int initialPos = 0;
		strs.clear();

		while (pos != std::string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos + 1));		// substr(initialPos, numberOfChars)
			initialPos = pos + 1;
			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

		return strs.size();

	}





};

