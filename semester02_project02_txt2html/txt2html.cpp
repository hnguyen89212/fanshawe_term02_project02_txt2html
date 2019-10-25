// ----------------------------------------------
// IMPORT LIBRARIES
// ----------------------------------------------
#include "Tools.h"

// ----------------------------------------------
// DOCUMENTATION
// ----------------------------------------------
/**
 * Author:          Hai Nguyen
 * Student ID:      0904995
 * Program:         CPA2, Winter 2019 Intake
 * Start date:      July 5th, 2019
 * Complete date:   July 14th, 2019
 * Project:         #2 Text To Html
 * Professor:       Janice Manning
 * Purpose:         A C++ console app (txt2html.exe) that converts a text file into a formatted html page.
 * Dependencies:    Tools.h and Tools.cpp
 * Software used:   Visual Studio 2019, Visual Studio Code
 * Usage:           txt2html [-r(-b|-p)] sourceFileName.txt [destFileName.html]
 */

 // ----------------------------------------------
 // ENTRY POINT -- MAIN PROGRAM
 // ----------------------------------------------
int main(int argc, char** argv)
{
	// ------------------------------------------
	// PART 0: DECLARE VARIABLES
	// ------------------------------------------
	int indexOfTxtFile = -1;
	int indexOfHtmlFile = -1;

	bool inReportMode = false;
	bool inBinaryMode = false;
	bool inParagraphMode = false;

	string pureName("");
	string inFileName("");
	string outFileName("");

	// ------------------------------------------
	// PART 1: HANDLE THE CMD LINE ARGUMENTS
	// ------------------------------------------
	// the most simple test, check for legal/appropriate numbers of parameters
	// at least 2 and at most 5
	if (argc < 2 || argc > 5)
	{
		cerr << "Error: Incorrect number of parameters allowed." << endl;
		printProgramInfo();
	}

	// declare a vector of strings, to store all string literals in cmd line
	vector<string> cmdArgs;

	// populate the vector "cmdArgs"
	for (int i = 0; i < argc; i++)
	{
		string temp(argv[i]);
		cmdArgs.push_back(temp);
	}

	// determine the index of .txt and .html file
	for (int i = 0; i < argc; i++)
	{
		string temp = cmdArgs[i];
		if (temp.find(txtExtension) != string::npos)
		{
			inFileName.append(temp);
			indexOfTxtFile = i;
		}
		if (temp.find(htmlExtension) != string::npos)
		{
			outFileName.append(temp);
			indexOfHtmlFile = i;
		}
	}

	// in case source file is missing from cmd
	// i.e, indexOfTxtFile == -1, terminates program.
	if (indexOfTxtFile == -1)
	{
		cerr << "Error: .txt file (source file) is missing. There must be one to convert to .html file." << endl;
		printProgramInfo();
		return EXIT_FAILURE;
	}

	// in case both .txt and .html files appear in cmd arguments
	// makes sure that .txt file does not appear after .html.
	// if then, throws error, exits.
	if (indexOfTxtFile > indexOfHtmlFile && indexOfTxtFile >= 0 && indexOfHtmlFile >= 0)
	{
		cerr << "Error: .txt file (source file) cannot be specified after .html file (destination file)." << endl;
		printProgramInfo();
		return EXIT_FAILURE;
	}

	// if "indexOfHtmlFile" remains -1 (i.e, user does not specify .html name)
	// set the outFileName exact to inFileName, just modify extension.
	if (indexOfHtmlFile == -1)
	{
		int indexOfDot = findIndexOfDotInFileName(inFileName);
		pureName.append(inFileName.begin(), inFileName.begin() + indexOfDot);
		outFileName.append(pureName);
		outFileName.append(htmlExtension);
	}
	// of the .html name is specified, save it in "purename" to instantiate the ofstream object
	else
	{
		int indexOfDot = findIndexOfDotInFileName(outFileName);
		pureName.append(outFileName.begin(), outFileName.begin() + indexOfDot);
	}
	// from this point on, the source file and dest file both have legal full names.
	// those names are stored in "inFileName" and "outFileName".

	// detect all switches/modes (if any)
	for (int i = 0; i < argc; i++)
	{
		string temp = cmdArgs[i];
		if (temp.compare(reportSwitch) == 0)
			inReportMode = true;
		else if (temp.compare(binarySwitch) == 0)
			inBinaryMode = true;
		else if (temp.compare(paragraphSwitch) == 0)
			inParagraphMode = true;
	}

	// ensure that -b and -p cannot co-exist. If then, ends program.
	if (inBinaryMode && inParagraphMode)
	{
		cerr << "Error: cannot have both \"binary\" (-b) and \"paragraph\" (-p) switches on together." << endl;
		printProgramInfo();
		return EXIT_FAILURE;
	}

	// ------------------------------------------
	// PART 2: PROCESS INPUT FILE
	// ------------------------------------------
	// group 1: io stream variables
	ifstream inFile;
	if (inBinaryMode)
		inFile.open(inFileName, ios::binary);
	else
		inFile.open(inFileName, ifstream::in);
	ofstream outFile;

	// check if such a file exists
	if (!inFile)
	{
		cerr << "Error: file not found." << endl;
		printProgramInfo();
		return EXIT_FAILURE;
	}

	// group 2: counter variables (for later report)
	char ch;
	int totalNewLines = 0;
	int countNewLines = 0;
	int totalParagraphs = 0;
	// ------------------------------------------
	// in binary mode, if we catch a '\r', that means there is a new line.
	const char NEW_LINE_CHAR = (inBinaryMode) ? '\r' : '\n';
	// ------------------------------------------

	// declare output stream to write out data
	// ofstream outFile(outFileName);
	outFile.open(outFileName);

	// append preliminary tags
	outFile << "<html>\n<head>\n<title>";
	outFile << pureName << "</title>\n</head>\n<body>\n";

	// right from the beginning of the .txt file, there is a line, i.e, a paragraph must be in place.
	if (inParagraphMode)
		outFile << "<p>\n";

	while (inFile.get(ch))
	{
		// in binary mode, if we catch \r, then there is surely \n after it
		// we are not interested in \n as \r is enough, thus, skip \n
		if (inBinaryMode && ch == '\n')
			continue;
		if (ch == NEW_LINE_CHAR)
		{
			// cout << "carriage return!" << endl;
			countNewLines++;
			totalNewLines++;
		}
		// catch a character other than \n
		else if (ch != NEW_LINE_CHAR)
		{
			// if there is pnly one \n char in sequence, drop it, reset count
			if (countNewLines == 1)
				countNewLines = 0;
			// else, insert as many <br> tags
			else if (countNewLines > 1)
			{
				if (inParagraphMode)
					outFile << "\n</p>\n";
				printBrTagsResetCounter(&outFile, &countNewLines, inParagraphMode);
				totalParagraphs++;
				if (inParagraphMode)
					outFile << "<p>\n";
			}
			// finally, output the character
			outFile << ch;
		}
	}

	// after all character is read, close the final paragraph with "</p>"
	if (inParagraphMode)
		outFile << "\n</p>\n";

	// deal with some remaining \n chars before EOF is reached
	if (countNewLines > 1)
	{
		printBrTagsResetCounter(&outFile, &countNewLines, inParagraphMode);
		totalParagraphs++;
	}

	// wrap things up
	outFile << "</body>\n</html>";

	// ------------------------------------------
	// PART 2.5: PRINT REPORT IF NEEDED
	// ------------------------------------------
	if (inReportMode)
		printReport(inBinaryMode, inParagraphMode, totalNewLines, totalParagraphs);

	// ------------------------------------------
	// PART 3: CLEAN UP
	// ------------------------------------------
	inFile.close();
	outFile.close();
	cmdArgs.clear();

	return EXIT_SUCCESS;
}
