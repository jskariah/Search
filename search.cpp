#include "include/search.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  string s = token;
  for (int i = 0; i < (int)s.length(); i++) {
    s[i] = tolower(s[i]);
  }

  bool hasLetter = false;
  for (int i = 0; i < (int)s.length(); i++) {
    if (isalpha(s[i])) {
      hasLetter = true;
      break;
    }
  }
  if (!hasLetter) {
    return "";
  }

  int left = 0;
  while (left < (int)s.length() && ispunct(s[left])) left++;

  int right = (int)s.length() - 1;
  while (right >= left && ispunct(s[right])) right--;

  return s.substr(left, right - left + 1);
}

set<string> gatherTokens(const string& text) {
  set<string> tokens;

  int n = (int)text.size();
  int i = 0;

  while (i < n) {
    while (i < n && isspace(text[i])) i++;
    if (i >= n) break;

    int j = i;
    while (j < n && !isspace(text[j])) j++;

    string cleaned = cleanToken(text.substr(i, j - i));
    if (!cleaned.empty()) tokens.insert(cleaned);

    i = j;
  }

  return tokens;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  string pageText;
  string pageUrl;
  int pageCount = 0;

  ifstream inFile(filename);
  while (getline(inFile, pageUrl)) {
    getline(inFile, pageText);

    set<string> tokens = gatherTokens(pageText);
    for (string word : tokens) {
      index[word].insert(pageUrl);
    }

    pageCount++;
  }

  return pageCount;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  istringstream input(sentence);
  string rawWord;
  bool firstTerm = true;
  set<string> finalResult;

  while (input >> rawWord) {
    char change = ' ';

    if ((rawWord[0] == '+') || (rawWord[0] == '-')) {
      change = rawWord[0];
      rawWord = rawWord.substr(1);
    }

    string cleanedString = cleanToken(rawWord);

    set<string> pages;
    auto page = index.find(cleanedString);
    if (page != index.end()) {
      pages = page->second;
    }

    if (firstTerm) {
      finalResult = pages;
      firstTerm = false;
      continue;
    }

    set<string> temp;

    if (change == '+') {
      set_intersection(finalResult.begin(), finalResult.end(), pages.begin(),
                       pages.end(), inserter(temp, temp.begin()));
    } else if (change == '-') {
      set_difference(finalResult.begin(), finalResult.end(), pages.begin(),
                     pages.end(), inserter(temp, temp.begin()));
    } else {
      set_union(finalResult.begin(), finalResult.end(), pages.begin(),
                pages.end(), inserter(temp, temp.begin()));
    }

    finalResult = temp;
  }

  return finalResult;
}

void searchEngine(const string& filename) {
  map<string, set<string>> index;

  ifstream file(filename);
  if (!file.is_open()) {
    cout << "Invalid filename." << endl;
  }

  cout << "Stand by while building index..." << endl;

  int webPages = buildIndex(filename, index);
  cout << "Indexed " << webPages << " pages containing " << index.size()
       << " unique terms" << endl;

  while (true) {
    cout << "Enter query sentence (press enter to quit): ";

    string query;
    getline(cin, query);

    if (query == "") {
      cout << "Thank you for searching!" << endl;
      break;
    }

    set<string> matches = findQueryMatches(index, query);

    cout << "Found " << matches.size() << " matching pages" << endl;

    for (const string& url : matches) {
      cout << url << endl;
    }
  }
}