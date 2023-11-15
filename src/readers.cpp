#include <boost/foreach.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/newline.hpp>
#include <boost/iostreams/copy.hpp>
#include <sstream>
#include "readers.h"
//#include "../utils/lobsterCout.h"
#include "keywords.h"
#include "parameters.h"
#include <iostream>

void cpReaders::getNumberOfAtoms ()	// gets number of elements per species
{
	fstream posFile;
	posFile.open("POSCAR");

	string line;
	for ( int i=0; i<6; i++ )
	{
		getline (posFile, line);
	}
	stringstream ss;
	ss << line;
	string inString;
	int nElements=0;
	while (ss >> inString)
		nElements++;
//	cout << line << " " << nElements << " " << inString << endl;
	_nElements=nElements;
	_numberOfAtoms.resize(nElements);

	getline(posFile, line);
	ss.clear();
	ss.str("");
	ss << line;
	for( int i=0; i< nElements; i++ )
	{
		ss >> inString;
		_numberOfAtoms(i)=stoi(inString);
	}

//	cout << _numberOfAtoms << endl;
	posFile.close();
}

void cpReaders::getNumberOfBands ()	// gets number of bands per species
{
	_basisFunctions.resize(_nElements);
	_basisFunctions.setZero();
	fstream ppFile;
	ppFile.open("POTCAR");
	stringstream ss;
	string inString;
	string line;

	for ( int currEl = 0; currEl < _nElements; currEl++ )
	{
		getline(ppFile, line);	// name of pp
		getline(ppFile, line);	// number of electrons
		ss.clear();
		ss.str("");
		ss << line;
		ss >>inString;
		const int nElec = stoi(inString);

		while ( inString !="Atomic" )
		{
			getline(ppFile, line);
			ss.clear();
			ss.str("");
			ss << line;
			ss >> inString;
		}
		getline (ppFile, line);
		ss.clear();
		ss.str("");
		ss << line;
		ss >> inString;
		const int nEntries = stoi(inString);
		getline (ppFile, line);

		ArrayXXi atomConf;
		atomConf.resize(nEntries,2);
		for ( int entry = 0; entry < nEntries; entry++ )	// Read the atom entries
		{
			getline(ppFile, line);
			ss.clear();
			ss.str("");
			ss << line;
			ss >> inString;	// n
			ss >> inString;	// l
			atomConf(entry,0) = stoi(inString);
			ss >> inString;	// j
			ss >> inString;	// E
			ss >> inString;	// occ
			atomConf(entry,1) = stoi(inString);
		}
		//cout << atomConf << endl;

		int currElec = 0;
		int currEntry = nEntries-1;
		do
		{
			if ( atomConf(currEntry,1) > 0 )
			{
				_basisFunctions(currEl)+=2*atomConf(currEntry,0)+1;
				currElec += atomConf(currEntry,1);
			}
			currEntry--;
		} while ( currElec < nElec );
//		cout << _basisFunctions(currEl) << " basis functions for element " << currEl << endl;

		// Skip to the next pp
		while ( inString != "End" )
		{
			getline(ppFile, line);
			ss.clear();
			ss.str("");
			ss << line;
			ss >> inString;
		}
	}
	ppFile.close();

}

void cpReaders::getTotalNumberOfBands()
{
	_totalNumberOfBands = 0;
	for ( int currEl = 0; currEl < _nElements; currEl++ )
		_totalNumberOfBands += _basisFunctions(currEl)*_numberOfAtoms(currEl);

//	cout << _totalNumberOfBands << " bands in total" << endl;
}

void cpReaders::writeNewIncar()
{
	fstream inFile;
	inFile.open("INCAR");
	ofstream outFile;
	outFile.open("INCAR_lobster");

	string line;
	stringstream ss;
	string inString;

	outFile << "INCAR file generated for LOBSTER" << endl;
	outFile << " NBANDS = " << _totalNumberOfBands << endl;
	outFile << " LWAVE = .TRUE." << endl;
	outFile << " ISYM = 0" << endl;
	outFile << " NSW = 0" << endl;

	while ( getline(inFile, line) )
	{
		ss.clear();
		ss.str("");
		ss << line;
		ss >> inString;
		if ( inString !=NBANDSkeyword && inString != ISYMkeyword && inString != LWAVEkeyword && inString != NSWkeyword )
			outFile << line << endl;
	}

	inFile.close();
	outFile.close();


}
