#include "ProteinLoader.h"
#include <fstream>
#include <iostream>

ProteinLoader::ProteinLoader() {
}

bool ProteinLoader::loadProtein(const char* path) {


	/*PDB FILE NOTES / REFERENCE
	* - lines beginning with ATOM are the atoms
	* Example:
	*	(IGNORE THE FIRST 2 LINES. I WROTE THESE SO WE CAN SEE WHICH CHARACTER IN THE STRING IS WHICH)
	*  !         1         2         3         4         5         6         7         8
	*  !12345678901234567890123456789012345678901234567890123456789012345678901234567890
	*	ATOM      1  N   SER A   1      -4.264  27.700  -4.649  1.00 19.42           N  
	* 
	* 
	* EXPLANATION OF COLUMNS: (Useful columns have been marked with an "!". Ignore the rest)
	*  !col 1, chars 1-4   - record name "ATOM  "
	*	col 2, chars 7-11  - atom serial number "1"
	*  !col 3, chars 13-16 - atom name "n"
	*   col 4, char 17     - alternate location indicator " " PROBABLY IGNORE THIS, there isnt one in this example. digit 17 is blank
	*   col 5, chars 18-20 - residue name "SER"
	*   col 6, char 22     - chain identifier "A" PROBABLY IGNORE THIS
	*   col 7, chars 23-26 - residue sequence number "1" Could be useful if we have to spot diff residues in an atom for selection?
	*   col 8, char 27     - code for insertion of residues " " PROBABLY IGNORE THIS, nothing in example
	*  !col 9, chars 31-38 - x coordinate "-4.264"
	*  !col 10, chars 39-46 - y coordinate "27.700"
	*  !col 11, chars 47-54 - z coordinate "-4.649"
	* 
	*	We can use column 1, "ATOM", to find which lines in the pdb files are atoms.
	*   The protein loader contains a list of atom names, e.g. "N" for nitrogen, and links them to colours e.g. "N" is blue.
	*   The protein loader also contains a list of atom names, e.g. "N" for nitrogen, and links them to radii e.g. "N" is 1.0.
	*   We can link these to the atoms name in column 3, alongside the coordinates of the atoms, to draw the protein. We could use the coordinates to adjust 
	    the model matrix to place each atom, and then draw a sphere with the correct radius and colour.
	* 
	* There are some alternatives to lines beginning with ATOM which we can probably ignore for now., we just need something that looks like an atom more than anything.
	* - lines beginning with HETATM are the ligands
	* - lines beginning with TER are the end of a chain
	* - lines beginning with CONECT are the bonds
	
	
	
	*/


	std::ifstream file;
	std::string line;
	std::string offload;
	file.open(path, std::ios::in);

	if (file.is_open()) {
		std::cout << "File opened successfully." << std::endl;
	}
	else {
		std::cout << "File failed to open." << std::endl;
		return false;
	}
	while (getline(file, line)) {

	}


	return false;
}