# Search Engine Project

## Overview

This project is a C++ command-line search engine that processes text documents, builds an inverted index, and allows users to search for matching web pages using simple and compound queries.

The program reads a data file containing pairs of URLs and page text. It then tokenizes and cleans the page text, stores each unique word in an inverted index, and allows the user to search for pages containing specific terms.

The main implementation work for this project was completed in `search.cpp`.

## Features

- Cleans and normalizes search tokens
- Extracts unique words from page text
- Builds an inverted index using C++ maps and sets
- Supports searching across multiple indexed pages
- Handles compound search queries
- Supports union, intersection, and difference query operations
- Provides an interactive command-line search interface

## Technologies Used

- C++
- STL `map`
- STL `set`
- STL `string`
- STL file streams
- Makefile
- GoogleTest for testing

## Project Structure

```text
Search/
├── search.cpp
├── search_main.cpp
├── search_tests.cpp
├── include/
│   └── search.h
├── data/
├── Makefile
├── README.md
└── .gitignore
```

## File Descriptions

| File/Folder | Description |
|---|---|
| `search.cpp` | Main implementation file containing the search engine logic |
| `search_main.cpp` | Runs the program by asking the user for a filename and starting the search engine |
| `search_tests.cpp` | Test cases for validating search engine functionality |
| `include/search.h` | Header file containing function declarations |
| `data/` | Contains text data files used by the search engine |
| `Makefile` | Builds and runs the project |

## Main Functions

### `cleanToken`

Cleans an individual token by:

- Converting letters to lowercase
- Removing punctuation from the beginning and end
- Preserving punctuation inside the token
- Returning an empty string if the token contains no letters

### `gatherTokens`

Extracts unique cleaned tokens from a string of text and stores them in a `set`.

### `buildIndex`

Builds an inverted index from a file containing URL and page-content pairs.

The inverted index maps each word to the set of URLs where that word appears.

### `findQueryMatches`

Processes search queries and returns the set of matching URLs.

The function supports:

| Query Type | Meaning |
|---|---|
| `word` | Pages containing the word |
| `word1 word2` | Pages containing `word1` OR `word2` |
| `word1 +word2` | Pages containing `word1` AND `word2` |
| `word1 -word2` | Pages containing `word1` but NOT `word2` |

### `searchEngine`

Runs the full command-line search engine by:

1. Building the index from a data file
2. Reporting how many pages and unique terms were indexed
3. Prompting the user for search queries
4. Printing matching URLs
5. Exiting when the user enters an empty query

## How the Inverted Index Works

An inverted index maps words to the pages where those words appear.

For example, if the word `pointer` appears on two pages, the index stores:

```text
pointer -> {page1.com, page2.com}
```

This makes searching faster because the program can look up a word directly instead of scanning every page each time the user enters a query.

## How to Build

From the project directory, run:

```bash
make
```

To build the main executable, run:

```bash
make search_main
```

## How to Run

Run the search engine with:

```bash
make run_search
```

Or manually run the executable after building:

```bash
./search_main
```

The program will ask for a filename:

```text
Enter a filename:
```

Enter a data file path, such as:

```text
data/tiny.txt
```

Then enter search queries when prompted.

## Example Usage

```text
Enter a filename: data/tiny.txt
Stand by while building index...
Indexed 4 pages containing 32 unique terms
Enter query sentence (press enter to quit): fish
Found 2 matching pages
www.dr.seuss.net
www.shoppinglist.com
```

## Example Queries

```text
fish
```

Finds pages containing `fish`.

```text
red fish
```

Finds pages containing `red` OR `fish`.

```text
red +fish
```

Finds pages containing both `red` AND `fish`.

```text
red -fish
```

Finds pages containing `red` but not `fish`.

## Testing

This project includes tests for the major search engine functions.

To build and run all tests:

```bash
make test_all
```

To run specific test groups:

```bash
make test_clean_token
make test_gather_tokens
make test_build_index
make test_find_query_matches
make test_search_engine
```

## What I Learned

Through this project, I practiced:

- Using C++ sets and maps
- Building an inverted index
- Processing and cleaning text input
- Working with file input/output
- Implementing search query logic
- Using set operations such as union, intersection, and difference
- Writing and running test cases
- Designing a command-line application
- Debugging larger C++ programs

## Notes

This repository is intended to showcase the core implementation and functionality of the project.

Generated build files, compiled executables, editor-specific files, and course handouts are intentionally excluded from the repository.


## Author

Created by Justin Skariah.

Primary implementation work completed in `search.cpp`.
