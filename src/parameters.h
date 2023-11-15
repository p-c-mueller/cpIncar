/*
 * parameters.h
 *
 *  Created on: Feb 27, 2020
 *      Author: pemueller
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>
#include <vector>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

const string controlFilename = "INCAR";
const string commentSymbols = "#";
const string equalsSymbol = "=";

// Define default values for LOBSTER-specific settings and tags that are rigorously overwritten
const int defaultISYM = -1;
const string defaultLWAVE = ".TRUE.";
const int defaultNELMIN = 4;
const int defaultNELM = 200;
const int defaultNSW = 0;
const string defaultLCHARG = ".FALSE.";

/*
VectorXi _numberOfAtoms;      // Number of atoms, per species
VectorXi _basisFunctions;       // Number of basis functions per species
vector<vector<tuple<int,int,int>>> _localFunctions;     // Explicit basis functions, n, l, j
int _totalNumberOfBands;
int _nElements;
*/

#endif /* PARAMETERS_H_ */
