// ----------------------------------------------
// LINK TO HEADER FILE
// ----------------------------------------------
#include "Tools.h"

// ----------------------------------------------
// FUNCTIONS IMPLEMENTATION
// ----------------------------------------------
void printProgramInfo()
{
	cout << "txt2html.exe (C) 2019 by Hai Phuc Nguyen" << endl;
	cout << "Usage: txt2html [-r(-b|-p)] sourceFileName.txt [destFileName.html]" << endl;
}

int findIndexOfDotInFileName(string fileName)
{
	for (size_t i = 0; i < fileName.length(); i++)
		if (fileName[i] == '.')
			return i;
	return -1;
}

void printBrTagsResetCounter(ofstream* os, int* counts, bool inParagraphMode)
{
	/**
	 * Why subtract 2 from *counts?
	 * Assume there are 2 consecutive \n in a row.
	 * Then, under perspective of paragraphs, there are only 2 paragraphs.
	 * If we just subtract 1, then there is still a <br> between 2 paragraphs --> more spaces than expected!
	 */
	int times = (inParagraphMode) ? *counts - 2 : *counts;
	for (int i = 0; i < times; i++)
		* os << "<br>\n";
	// reset counter
	*counts = 0;
}

void printReport(bool inBinaryMode, bool inParagraphMode, int totalNewLines, int totalParagraphs)
{
	if (inBinaryMode)
		// as we interpret a single \n as \r + \n
		cout << "Input lines processed, counting untranslated newline characters = " << (totalNewLines * 2) << endl;
	else
		cout << "Input lines processed, counting translated newline characters = " << totalNewLines << endl;

	if (inParagraphMode)
		cout << "Output paragraphs processed, counting <p></p> tags = " << (totalParagraphs * 2) << endl;
	else
		cout << "Output paragraphs processed = " << totalParagraphs << endl;
}
