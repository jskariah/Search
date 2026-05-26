#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

// cleanToken tests

TEST(CleanToken, PunctuationOnLeftEnd) {
  EXPECT_THAT(cleanToken("......a"), StrEq("a"));
  EXPECT_THAT(cleanToken("!!!Hello"), StrEq("hello"));
  EXPECT_THAT(cleanToken("...123abc"), StrEq("123abc"));
}

TEST(CleanToken, PunctuationOnRightEnd) {
  EXPECT_THAT(cleanToken("a......"), StrEq("a"));
  EXPECT_THAT(cleanToken("Hello!!!"), StrEq("hello"));
  EXPECT_THAT(cleanToken("abc123!!!"), StrEq("abc123"));
}

TEST(CleanToken, PunctuationInMiddleOnly) {
  EXPECT_THAT(cleanToken("he-llo"), StrEq("he-llo"));
  EXPECT_THAT(cleanToken("don't"), StrEq("don't"));
  EXPECT_THAT(cleanToken("a_b*c"), StrEq("a_b*c"));
}

TEST(CleanToken, PunctuationMiddleAndLeft) {
  EXPECT_THAT(cleanToken("...he-llo"), StrEq("he-llo"));
  EXPECT_THAT(cleanToken("!!!rock-n-roll"), StrEq("rock-n-roll"));
}

TEST(CleanToken, PunctuationMiddleAndRight) {
  EXPECT_THAT(cleanToken("he-llo!!!"), StrEq("he-llo"));
  EXPECT_THAT(cleanToken("rock-n-roll!!!"), StrEq("rock-n-roll"));
}

TEST(CleanToken, PunctuationMiddleBothEnds) {
  EXPECT_THAT(cleanToken("...he-llo!!!"), StrEq("he-llo"));
  EXPECT_THAT(cleanToken("!!!rock-n-roll!!!"), StrEq("rock-n-roll"));
}

TEST(CleanToken, UppercaseLetters) {
  EXPECT_THAT(cleanToken("HeLLo"), StrEq("hello"));
  EXPECT_THAT(cleanToken("WORLD"), StrEq("world"));
}

TEST(CleanToken, UppercaseWithPunctuation) {
  EXPECT_THAT(cleanToken("...HeLLo!!!"), StrEq("hello"));
  EXPECT_THAT(cleanToken("!!!WoRLd!!!"), StrEq("world"));
}

TEST(CleanToken, NoLetters) {
  EXPECT_THAT(cleanToken("!!!...???"), StrEq(""));
  EXPECT_THAT(cleanToken("$$$%%%"), StrEq(""));
  EXPECT_THAT(cleanToken("12345"), StrEq(""));
  EXPECT_THAT(cleanToken("007"), StrEq(""));
}

TEST(CleanToken, LettersAndNumbers) {
  EXPECT_THAT(cleanToken("123abc456"), StrEq("123abc456"));
  EXPECT_THAT(cleanToken("a1b2c3"), StrEq("a1b2c3"));
}

TEST(CleanToken, SingleLetter) {
  EXPECT_THAT(cleanToken("x"), StrEq("x"));
  EXPECT_THAT(cleanToken("Z"), StrEq("z"));
}

TEST(CleanToken, SingleLetterWithPunctuation) {
  EXPECT_THAT(cleanToken("...x!!!"), StrEq("x"));
  EXPECT_THAT(cleanToken("!!!Y!!!"), StrEq("y"));
}

TEST(CleanToken, NumbersAtEnds) {
  EXPECT_THAT(cleanToken("123hello456"), StrEq("123hello456"));
  EXPECT_THAT(cleanToken("99bottles99"), StrEq("99bottles99"));
}

TEST(CleanToken, EmptyString) {
  EXPECT_THAT(cleanToken(""), StrEq(""));
}

TEST(CleanToken, MixedSymbolsAndLetters) {
  EXPECT_THAT(cleanToken("$$$a$b$c$$$"), StrEq("a$b$c"));
  EXPECT_THAT(cleanToken("###x-y-z###"), StrEq("x-y-z"));
}

// gatherTokens tests

TEST(GatherTokens, SimpleSentence) {
  set<string> result = gatherTokens("hello world");
  EXPECT_THAT(result, UnorderedElementsAre("hello", "world"));
}

TEST(GatherTokens, DuplicateWords) {
  set<string> result = gatherTokens("hello hello HELLO");
  EXPECT_THAT(result, UnorderedElementsAre("hello"));
}

TEST(GatherTokens, LeadingSpaces) {
  set<string> result = gatherTokens("   hello world");
  EXPECT_THAT(result, UnorderedElementsAre("hello", "world"));
}

TEST(GatherTokens, TrailingSpaces) {
  set<string> result = gatherTokens("hello world   ");
  EXPECT_THAT(result, UnorderedElementsAre("hello", "world"));
}

TEST(GatherTokens, MultipleSpacesBetweenWords) {
  set<string> result = gatherTokens("hello    world     cs251");
  EXPECT_THAT(result, UnorderedElementsAre("hello", "world", "cs251"));
}

TEST(GatherTokens, OnlySpaces) {
  set<string> result = gatherTokens("      ");
  EXPECT_TRUE(result.empty());
}

TEST(GatherTokens, PunctuationAndCase) {
  set<string> result = gatherTokens("Hello, world!!! HELLO world.");
  EXPECT_THAT(result, UnorderedElementsAre("hello", "world"));
}

TEST(GatherTokens, TokensThatCleanToEmptyAreIgnored) {
  set<string> result = gatherTokens("!!! ??? hello 12345");
  EXPECT_THAT(result, UnorderedElementsAre("hello"));
}

TEST(GatherTokens, LettersAndNumbers) {
  set<string> result = gatherTokens("cs251 cs251 123abc456");
  EXPECT_THAT(result, UnorderedElementsAre("cs251", "123abc456"));
}

TEST(GatherTokens, SingleLetterTokens) {
  set<string> result = gatherTokens("a b c A B");
  EXPECT_THAT(result, UnorderedElementsAre("a", "b", "c"));
}

TEST(GatherTokens, EmptyString) {
  set<string> result = gatherTokens("");
  EXPECT_TRUE(result.empty());
}

// buildIndex tests

TEST(BuildIndex, TinyFile) {
  map<string, set<string>> index;

  int pages = buildIndex("data/tiny.txt", index);
  EXPECT_EQ(pages, 4);

  EXPECT_THAT(index["bread"], UnorderedElementsAre("www.shoppinglist.com"));

  EXPECT_THAT(index["fish"],
              UnorderedElementsAre("www.shoppinglist.com", "www.dr.seuss.net"));

  EXPECT_THAT(index["gre-en"], UnorderedElementsAre("www.rainbow.org"));

  EXPECT_THAT(index["indigo"], UnorderedElementsAre("www.rainbow.org"));

  EXPECT_THAT(index["two"], UnorderedElementsAre("www.dr.seuss.net"));

  EXPECT_THAT(index["red"],
              UnorderedElementsAre("www.rainbow.org", "www.dr.seuss.net"));

  EXPECT_THAT(index["eat"], UnorderedElementsAre("www.bigbadwolf.com"));
}

TEST(BuildIndex, FileNotFound) {
  map<string, set<string>> index;

  int pages = buildIndex("data/does_not_exist.txt", index);

  EXPECT_EQ(pages, 0);
  EXPECT_TRUE(index.empty());
}

// findQueryMatches tests

TEST(FindQueryMatches, SingleTerm) {
  map<string, set<string>> index = {{"apple", {"a.com", "b.com"}}};

  set<string> result = findQueryMatches(index, "apple");

  EXPECT_THAT(result, UnorderedElementsAre("a.com", "b.com"));
}

TEST(FindQueryMatches, FirstTermNotInIndex) {
  map<string, set<string>> index = {{"apple", {"a.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "orange");

  EXPECT_TRUE(result.empty());
}

TEST(FindQueryMatches, LaterTermNotInIndexUnion) {
  map<string, set<string>> index = {{"apple", {"a.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "apple orange");

  EXPECT_THAT(result, UnorderedElementsAre("a.com"));
}

TEST(FindQueryMatches, LaterTermNotInIndexIntersection) {
  map<string, set<string>> index = {{"apple", {"a.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "apple +orange");

  EXPECT_TRUE(result.empty());
}

TEST(FindQueryMatches, LaterTermNotInIndexDifference) {
  map<string, set<string>> index = {{"apple", {"a.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "apple -orange");

  EXPECT_THAT(result, UnorderedElementsAre("a.com"));
}

TEST(FindQueryMatches, UnionBehavior) {
  map<string, set<string>> index = {{"apple", {"a.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "apple banana");

  EXPECT_THAT(result, UnorderedElementsAre("a.com", "b.com"));
}

TEST(FindQueryMatches, IntersectionBehavior) {
  map<string, set<string>> index = {{"apple", {"a.com", "b.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "apple +banana");

  EXPECT_THAT(result, UnorderedElementsAre("b.com"));
}

TEST(FindQueryMatches, DifferenceBehavior) {
  map<string, set<string>> index = {{"apple", {"a.com", "b.com"}},
                                    {"banana", {"b.com"}}};

  set<string> result = findQueryMatches(index, "apple -banana");

  EXPECT_THAT(result, UnorderedElementsAre("a.com"));
}

TEST(FindQueryMatches, LeftToRightEvaluation) {
  map<string, set<string>> index = {{"tasty", {"a.com", "b.com"}},
                                    {"mushrooms", {"b.com"}},
                                    {"simple", {"c.com"}},
                                    {"cheap", {"a.com", "c.com"}}};

  set<string> result =
      findQueryMatches(index, "tasty -mushrooms simple +cheap");

  EXPECT_THAT(result, UnorderedElementsAre("a.com", "c.com"));
}

TEST(FindQueryMatches, CleansQueryTerms) {
  map<string, set<string>> index = {{"apple", {"a.com"}}};

  set<string> result = findQueryMatches(index, "Apple!!!");

  EXPECT_THAT(result, UnorderedElementsAre("a.com"));
}