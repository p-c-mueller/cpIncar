#ifndef KEYWORDS_H_
#define KEYWORDS_H_
#include <string>

using namespace std;

// Strictly needed and set by LOBSTER
const string NBANDSkeyword = "NBANDS";	// Number of bands; default: # of occupied valence orbitals
const string ISYMkeyword = "ISYM";
const string LWAVEkeyword = "LWAVE";	// Write wavefunction; Needed by lobster

// Strictly needed or at least highly recommended
const string NWRITEkeyword = "NWRITE";	// Sets verbosity of OUTCAR; 0|1|2|3|4; default: 1
const string PRECkeyword = "PREC";		// Precision Low|Medium|High|Normal|Single|Accurate; default: Accurate
const string ISPINkeyword = "ISPIN";	// Spin-polarized? 1|2; default: 1
const string ENCUTkeyword = "ENCUT";	// cut-off kinetic energy of PW; default: 500
const string NPARkeyword = "NPAR";		// Number of cores; default: 4
const string EDIFFkeyword = "EDIFF";	// Convergence criterion for electronic relaxation; default: 1e-6
const string ALGOkeyword = "ALGO";		// Algorithm; exchangeable with IALGO; default:
const string NELMINkeyword = "NELMIN";	// Minimum electronic steps; default: 4
const string NELMkeyword = "NELM";		// Maximum electronic steps; default: 200
const string NELMDkeyword = "NELMD";	// Number of non-selfconsistent steps at the beginning; default: -5
const string LCHARGkeyword = "LCHARG";	// Write charge density files; default .FALSE.
const string NSWkeyword = "NSW";		// Number of ionic relaxation steps; default: 0
const string EMINkeyword = "EMIM";		// Minimum energy of DOS
const string EMAXkeyword = "EMAX";		// Maximum energy of DOS
const string ISMEARkeyword = "ISMEAR";	// Determines integration method; default: 0 for Gaussian smearing; -5 for tetrahetron with Blöchl correction; >0 Methfessel-Paxton of Order N, used for metals
const string SIGMAkeyword = "SIGMA";	// Smearing factor; default: 0.05 for ISMEAR=-5|0; 0.15 for ISMEAR=1|2

// Optional keywords
const string LREALkeyword = "LREAL";	//

// Special functionals
const string GGAkeyword = "GGA";		// Set a specific xc-functional
const string METAGGAkeyword = "METAGGA";	//
const string IVDWkeyword = "IVDW";		// Van-der-Waals correction terms for molecular crystals

// todo: Magnetism

// todo: U-Parameters

// Non-necessary keywords
const string EDIFFGkeyword = "EDIFFG";	// Convergence criterion for ionic relaxation; not needed in a static simulation...
const string LPETIMkeyword = "LPETIM";	// write-flag & timer; NOT DOCUMENTED!!!!; default: F
const string IALGOkeyword = "IALGO";	// Algorithm; exchangeable with ALGO;
const string POTIMkeyword = "POTIM";	// time-step for ion-motion; not needed in a static simulation...
const string IBRIONkeyword = "IBRION";	// Determines how ions are moved; not needed in a static simulation...
const string ISIFkeyword = "ISIF";		// Determines degrees of freedom; not needed in a static simulation...
//# SYMPREC = 1E-03


#endif /* KEYWORDS_H_ */
