// ----------------------------------------------
// IMPORT LIBRARIES
// ----------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// ----------------------------------------------
// CONSTANTS
// ----------------------------------------------
// file extensions
#define txtExtension        ".txt"
#define htmlExtension       ".html"
// flags/switches in cmd
#define reportSwitch        "-r"
#define binarySwitch        "-b"
#define paragraphSwitch     "-p"

// ----------------------------------------------
// FUNCTION PROTOTYPES
// ----------------------------------------------
/**
 * Prints the use direction of program.
 */
void printProgramInfo();

/**
 * Given a file name such as "document.txt",
 * returns the index of "." in name in 0-based.
 */
int findIndexOfDotInFileName(string fileName);

/**
 * Injects <br> tags into out stream, given some context determined by "inParagraphMode."."
 */
void printBrTagsResetCounter(ofstream* os, int* counts, bool inParagraphMode);

/**
 * Prints report as user requests with -r tag.
 */
void printReport(bool inBinaryMode, bool inParagraphMode, int totalNewLines, int totalParagraphs);
