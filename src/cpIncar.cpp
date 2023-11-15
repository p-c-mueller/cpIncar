// Just a little script that automatically writes the INCAR for lobster calculations
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include "readers.h"
#include "keywords.h"
#include "parameters.h"
#include <Eigen/Core>
#include <vector>
#include <utility>


using namespace std;
using namespace Eigen;
VectorXi _numberOfAtoms;      // Number of atoms, per species
VectorXi _basisFunctions;       // Number of basis functions per species
vector<vector<pair<int,int>>> _localFunctions;     // Explicit basis functions, n, l, j
int _totalNumberOfBands;
int _nElements;
vector<string> _elements;

void getNumberOfAtoms ()	// gets number of elements per species
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
	{
		_elements.push_back(inString);
		++nElements;
	}
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

void getNumberOfBands ()	// gets number of bands per species
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

		vector<pair<int, int>> orbConfTmp, orbConf;

		ArrayXXi atomConf;
		atomConf.resize(nEntries,2);
		for ( int entry = 0; entry < nEntries; entry++ )	// Read the atom entries
		{
			int n, l;

			getline(ppFile, line);
			ss.clear();
			ss.str("");
			ss << line;
			ss >> inString;	// n
			n = stoi(inString);
			ss >> inString;	// l
			l = stoi(inString);
			atomConf(entry,0) = stoi(inString);
			ss >> inString;	// j
			ss >> inString;	// E
			ss >> inString;	// occ
			atomConf(entry,1) = stoi(inString);
			
				orbConfTmp.push_back( make_pair(n,l) );
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
				orbConf.push_back( orbConfTmp[currEntry] );
//				cout << orbConfTmp[currEntry].first << " " << orbConfTmp[currEntry].second << endl;
			}
			--currEntry;
		} while ( currElec < nElec );
//		cout << _basisFunctions(currEl) << " basis functions for element " << currEl << endl;
/*		for ( size_t i = 0; i < orbConf.size(); ++i )
			cout << "Orbital configuration of atom " << get<0>(orbConf[i]) << " " <<  get<1>(orbConf[i]) << " " << get<2>(orbConf[i]) << flush;
*/
		_localFunctions.push_back(orbConf);

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

void getTotalNumberOfBands()
{
	_totalNumberOfBands = 0;
	for ( int currEl = 0; currEl < _nElements; currEl++ )
		_totalNumberOfBands += _basisFunctions(currEl)*_numberOfAtoms(currEl);

//	cout << _totalNumberOfBands << " bands in total" << endl;
}

void writeNewIncar()
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

string getBasisFunctions( size_t atom )
{
	string result;

	for ( size_t orb = 0; orb < _localFunctions[atom].size(); ++orb )
	{
		stringstream ss;
		ss.str("");
		ss << _localFunctions[atom][orb].first;
		result += ss.str();
//		cout << "result = \"" << result << "\"" << endl;
		
		switch( _localFunctions[atom][orb].second )
		{
			case 0: 
				result += "s";
				break;
			case 1: 
				result += "p";
				break;
			case 2: 
				result += "d";
				break;
			case 3: 
				result += "f";
				break;
			default:
				cout << "ERROR: Unknown value for l" << endl;
		}

		result += " ";
	}

	return result;
}

string getEnergyRange()
{
        fstream controlFile;
        controlFile.open("DOSCAR");

	string result;

        if( controlFile.fail() )
        {
                cout << "No DOSCAR in the current directory.\nUsing default energy range." << endl;
                return "cohpStartEnergy -20\ncohpEndEnergy 10";
        }

	double emin, emax;

	string line, inString;
	stringstream ss;

	for ( size_t i = 0; i < 6; ++i)
	{
		getline(controlFile, line);
	}

	ss << line;
	ss >> inString;

	emax = stod( inString );
	ss >> inString;

	emin = stod( inString );
	ss >> inString;
	ss >> inString;

	emax -= stod(inString);
	emin -= stod(inString);

	result += "cohpStartEnergy ";
	result += to_string(static_cast<int>(floor(emin)));
	result += "\ncohpEndEnergy ";
	result += to_string(static_cast<int>(ceil(emax)));

        return result;
}


void writeLobsterin()
{
	ofstream linfile;

	linfile.open("lobsterin");

	linfile << getEnergyRange() << endl << endl;

	linfile << "basisSet pbeVASPfit2015" << endl;

	for ( size_t atom = 0; atom < _numberOfAtoms.size(); ++atom )
	{
		linfile << "basisFunctions " << _elements[atom] << " " << getBasisFunctions(atom) << endl;
	}

	linfile << endl << "saveProjectionToFile" << endl << "loadProjectionFromFile";

	linfile << endl << "cohpGenerator from 1.0 to 4.0";

	linfile << endl << endl << "skipCOOP" << endl << "skipCOHP" << endl << "skipCAR" << endl << "skipDOS" << endl << "skipMadelungEnergy";

	linfile.close();
}

int main()
{
	// Read original INCAR from structural optimization
	fstream controlFile;
	controlFile.open("INCAR");
	if( controlFile.fail() )
	{
		cout << "INCAR does not exist." << endl;
		return 1;
	}

//	cout << "getting total number of atoms" << endl;
	getNumberOfAtoms();
//	cout << "getting number of bands" << endl;
	getNumberOfBands();
//	cout << "getting total number of bands" << endl;
	getTotalNumberOfBands();
//	cout << "writing new incar" << endl;
	writeNewIncar();
//	cout << "writing lobsterin" << endl;
	writeLobsterin();
	return 0;
}
