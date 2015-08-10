#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;


class Path{
  public:
	string name;
	int elements;
	short* coordX;
	short* coordY;
	
	void buildArrays(){
		coordX = new short[elements];
			for(int i = 0; i < elements; i++){
				coordX[i] = 0;
			}
		coordY = new short[elements];
			for(int i = 0; i < elements; i++){
				coordY[i] = 0;
			}
	}
	
	~Path(){
		delete [] coordX;
		delete [] coordY;
	}
};

class Process{
	string currentFile;
	string headerFile;
	char** charDump;
	int charDumpCount;
  public:

	void dumpFile(){
		ifstream svg;
		svg.open(currentFile.c_str());
		
		int count = 0;
		
		while(!svg.eof()){
			char buf[100];
			svg.getline(buf, 100);
			
			count++;
		}
			
		charDumpCount = count;
			
		charDump = new char*[count];
		for(int i = 0; i < count; i++)
			charDump[i] = new char[100];
			
		for(int i = 0; i < count; i++)
			for(int j = 0; j < 100; j++)
				charDump[i][j] = ' ';
			
		svg.clear();
		svg.seekg(0, ios::beg);
		
		for(int i = 0; i < count; i++){
			svg.getline(charDump[i], 100);
		}
		
		svg.close();
	}

	int getElements(int occurrence){
		occurrence++;
		int seen = 0;
		int eleCount = 0;
		
		
		char idKey[12] = "  <path id=";
		char startKey[13] = "           C";
		bool hasStarted = false;
		bool foundC = false;
		bool addC = true;
		
		
		for(int i = 0; i < charDumpCount && (seen == occurrence ||
			hasStarted == false || foundC == false); i++){
			
			bool check = true;
			
			for(int j = 0; j < 11; j++){
				if(charDump[i][j] != idKey[j]){
					check = false;
					if(foundC == true)
						addC = true;
				}
			}

			
			if(addC){
				eleCount++;
				addC = false;
			}
			
			if (check == true){
				seen++;
			}
			
			if(seen == occurrence){
				hasStarted = true;
				if(charDump[i][0] == '<'){
					seen++;
					
					eleCount--;
				}
			}
			
			bool check2 = true;
			
			if(hasStarted){
				for(int j = 0; j < 12; j++){
					if(charDump[i][j] != startKey[j])
						check2 = false;
				}
				if(check2 == true){
					foundC = true;
				}
			}
		}
		cout << "[" << eleCount << "]" << endl;
		
		return eleCount;
	}
 
	string getName(int occurrence){
		
		char key[12] = "  <path id=";
		
		int eleCount = 0;
		string lastEncountered = "";
		
		for(int i = 0; i < charDumpCount && eleCount <= occurrence; i++){
			bool check = true;
			
			for(int j = 0; j < 11; j++){
				if(charDump[i][j] != key[j])
					check = false;
			}
			
			if (check == true){
				lastEncountered = "";
				for(int j = 12; charDump[i][j] != '"'; j++)
					lastEncountered += charDump[i][j];
				
				eleCount++;
			}
		}
		
		cout << "    :: " << lastEncountered;
		return lastEncountered;
	}

	void getVars(Path &tempPath, int occurrence){
		int seen = 0;
		int currentIndex = 0;
		occurrence++;
		
		char idKey[12] = "  <path id=";
		char startKey[13] = "           C";
		
		bool isSeeking = true;
		bool foundC = false;
		bool foundZ = false;
				
		for(int i = 0; i < charDumpCount && !foundZ; i++){
			
			if(isSeeking){
				bool check = true;
				
				for(int j = 0; j < 11; j++){
					if(charDump[i][j] != idKey[j]){
						check = false;
					}
				}
				
				if (check == true){
					seen++;
				}
			}
			
			if(seen == occurrence){
				bool check2 = true;
				isSeeking = false;
				
				for(int j = 0; j < 12; j++){
					if(charDump[i][j] != startKey[j])
						check2 = false;
				}
				if(check2 == true){
					foundC = true;
				}
			}
			
			if(foundC){
				if(currentIndex < tempPath.elements){
					string tempNum = "";
					bool firstNum = 1;
					bool skipToC = 0;
					bool secondNum = 0;
					
					for(int k = 13; charDump[i][k] != ' '; k++){
						if(firstNum && charDump[i][k] != '.'){
							tempNum+=charDump[i][k];
						}else if(firstNum && charDump[i][k] == '.'){
							firstNum = 0;
							tempPath.coordX[currentIndex] =
								atoi(tempNum.c_str());
							tempNum="";
							skipToC = 1;
						}else if(skipToC && charDump[i][k] == ','){
							skipToC = 0;
							secondNum = 1;
						}else if(secondNum && charDump[i][k] != '.'){
							tempNum+= charDump[i][k];
						}else if(secondNum && charDump[i][k] == '.'){
							secondNum = 0;
							tempPath.coordY[currentIndex] =
								atoi(tempNum.c_str());
						}
					}
					
					secondNum = 0;
					currentIndex++;
				}
			}
			
		}
			
	}
  
	int getIdCount(){
		
		int eleCount = 0;
		
		char key[12] = "  <path id=";
		
		for(int i = 0; i < charDumpCount; i++){
			bool check = true;
			
			for(int j = 0; 
				j < 11; 
					j++){
				if(charDump[i][j] != key[j])
					check = false;
			}
			
			if (check == true)
				eleCount++;
		}
		
		cout << "  - " << eleCount << " paths will be converted" << endl;
				
		return eleCount;
	}

	Process(string c, string h){
		//pull in file names
		currentFile = c;
		headerFile = h;
		
		dumpFile();
		
		int idCount = getIdCount();
		Path path[idCount];
		
		for(int i = 0; i < idCount; i++){
			path[i].name = getName(i);
			path[i].elements = getElements(i);
			path[i].buildArrays();
			getVars(path[i], i);
		}
		
		cout << "\n";
		
		ofstream head;
		head.open(headerFile.c_str());
		head << "\n\n";
		
		for(int i = 0; i < idCount; i++){
			head << path[i].name << "X[" <<
				    path[i].elements << "] = { ";
			for(int j = 0; j < path[i].elements; j++){
				head << path[i].coordX[j];
				if (j != (path[i].elements-1)){
					head << ", ";
				}
			}
			head << " };\n";
			
			
			head << path[i].name << "Y[" <<
				    path[i].elements << "] = { ";
			for(int j = 0; j < path[i].elements; j++){
				head << path[i].coordY[j];
				if (j != (path[i].elements-1)){
					head << ", ";
				}
			}
			head << " };\n\n\n";
		}
		
		head.close();
		
		
	}
	
	~Process(){
		for(int i = 0; i < charDumpCount; i++)
			delete [] charDump[i];
		delete [] charDump;
	}
	
};

int main(int argc, char* argv[]){

	cout << "\n" << argv[ 0 ] << " is converting the following files: \n\n";
       
	for (int n = 1; n < argc; n++){
		cout << setw( 2 ) << n << ": " << argv[ n ] << " ==> ";
		
		string currentFile = argv[n];
		int lastindex = currentFile.find_last_of(".");
		string headerFile = (currentFile.substr(0, lastindex) + ".h");
		cout << headerFile << '\n';
		
		
		
		Process process(currentFile, headerFile);
		
			
	}

	return 0;
}
