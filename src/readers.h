/*
 * readers.h
 *
 *  Created on: Aug 8, 2013
 *      Author: smaintz
 */

#ifndef READERS_H_
#define READERS_H_

#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/scoped_ptr.hpp>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

void readControlFileToMemory();
streambuf* readingBuffer(const string& filename);

bool checkForKeywordInFile(const string& filename, const string& keyword);
bool checkForInsensitiveKeywordInFile(const string& filename, const string& keyword);

map<string, set<string> > readManualBasisSelection(const string& filename, const string& keyword);

string readSingleLineFromFile(const string& filename, const int linenumber);
string readStringAfterInsensitiveKeywordFromFile(const string& filename, const string& keyword);

template<typename T> T readKeywordFromFile(const string& filename, const string& keyword);
template<typename T> T readKeywordFromFileAfterSkippingToKeyword(const string& filename, const string& keyword, const string& skipWord);
template<typename T> T readCaseInsensitiveKeywordFromFile(const string& filename, const string& keyword);
template<typename T> T stringToValues(const string& s);
template<> string stringToValues<string>(const string& s);

string forwardIfstreamUntilKeyword(ifstream &file, const string& keyword);
list<string> readCustomBasisFunctionStringsFile(const string& filename, const string& keyword);


class cpReaders
{
public:
	void getNumberOfAtoms();
	void getNumberOfBands();
	void getTotalNumberOfBands();
	void writeNewIncar();
protected:
	VectorXi _numberOfAtoms;	// Number of atoms, per species
	VectorXi _basisFunctions;	// Number of basis functions per species
	int _totalNumberOfBands;
	int _nElements;
};

#endif /* READERS_H_ */
