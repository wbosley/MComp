#include "ProteinLoader.h"


ProteinLoader::ProteinLoader() {
	this->data = new DataTables();
	//from S.Ls code
	this->listOfColours = data->getColourList();
	this->listOfRadii = data->getVDWRadii();
}

//PDB FILE: ATOM LINES EXPLAINED vvvvv
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

bool ProteinLoader::loadProtein(const char* path) {

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
		//make a substring of the line, and find the identifier that appears before the first whitespace.
		std::string identifier = line.substr(0, line.find(" "));
		//stream  -stream of input. we are taking the line, bitwising shifting into offload. we are deliminating by default delimter (space) - its etting rid of the ATOM, and getting the atom serial number. ATOM and the first space go into offload, line turns into just the other details
		//std::stringstream ss(line);
		//ss >> offload;

		std::stringstream ss(line);

		if (identifier == "ATOM") {
			
			std::string atomName = getAtomNameFromLine(line);
			atomNames.push_back(atomName);
			//std::cout << atomName << std::endl;

			glm::vec3 coordinate = getAtomCoordinatesFromLine(line);
			atomCoordinates.push_back(coordinate);
			//std::cout << "Coordinates: " << coordinate.x << " " << coordinate.y << " " << coordinate.z << std::endl;

			//get the colour of the atom
			glm::vec3 colourVector = getColorFromAtomName(atomName);
			atomColours.push_back(colourVector);
			//std::cout << "Colour: " << colourVector.x << " " << colourVector.y << " " << colourVector.z << std::endl;

			//et the radius of the atom
			float radius = getRadiusFromAtomName(atomName);
			atomRadius.push_back(radius);;
			//std::cout << "Radius: " << radius << std::endl;
		}
	}
	return false;
}

std::string ProteinLoader::getAtomNameFromLine(std::string line) {
	//although we use string streams in most cases where we read from a file in this program, pdb files arent deliminated using anything - the meaning of a character is determined by its index in the line. Therefore, it is more effective to use substrings and remove the whitespace.
	std::string atomName = line.substr(12, 4);
	atomName.erase(std::remove_if(atomName.begin(), atomName.end(), isspace), atomName.end());
	return atomName;
}

glm::vec3 ProteinLoader::getAtomCoordinatesFromLine(std::string line) {
	glm::vec3 coordinate;
	std::string coordString;

	coordString = line.substr(30, 8);
	coordString.erase(std::remove_if(coordString.begin(), coordString.end(), isspace), coordString.end());
	coordinate.x = std::stof(coordString);

	coordString = line.substr(38, 8);
	coordString.erase(std::remove_if(coordString.begin(), coordString.end(), isspace), coordString.end());
	coordinate.y = std::stof(coordString);

	coordString = line.substr(46, 8);
	coordString.erase(std::remove_if(coordString.begin(), coordString.end(), isspace), coordString.end());
	coordinate.z = std::stof(coordString);

	return coordinate;
}

//ALmost the same as code from stephen. need to get who to credit for this and datatable.
glm::vec3 ProteinLoader::getColorFromAtomName(std::string atomName){

	glm::vec3 colourVector;
	std::map <std::string, std::vector<float>>::iterator itr;
	std::vector<float> current_colour;
	itr = listOfColours.find(atomName);
	if (itr == listOfColours.end())
	{

		current_colour.push_back(0.5);
		current_colour.push_back(0.5);
		current_colour.push_back(0.5);
	}
	else
	{
		current_colour = itr->second;
	}

	colourVector.x = current_colour[0];
	colourVector.y = current_colour[1];
	colourVector.z = current_colour[2];

	return colourVector;
}

//ALmost the same as code from stephen. need to get who to credit for this and datatable.
float ProteinLoader::getRadiusFromAtomName(std::string atomName) {
	float radius;

	std::map <std::string, float>::iterator itrRad = listOfRadii.find(atomName);

	if (itrRad == listOfRadii.end())
	{
		radius = 10.0f;
	}
	else
	{
		radius = itrRad->second;
	}

	return radius;
}

//GITHUB COPILOT MADE THIS OUT OF THE BLUE RANDOMLY. WE NEED TO TEST WHETHER THIS WORKS.

//void ProteinLoader::drawProtein() {
//	//draw the protein
//	for (int i = 0; i < coordinates.size(); i++) {
//		glm::mat4 model = glm::mat4(1.0f);
//		model = glm::translate(model, coordinates[i]);
//		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
//		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0
//	}
//}