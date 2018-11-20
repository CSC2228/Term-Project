#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
// Simply gets the file size of file.
int getFileSize(ifstream *file) {
	file->seekg(0,ios::end);
	int filesize = file->tellg();
	file->seekg(ios::beg);
	return filesize;
}

using namespace std; 
// Finds chunks by "chunkName" and creates file specified in fileOutput
void join_file(char *chunkName, char *fileOutput) {
	string fileName;

	// Create our output file
	ofstream outputfile;
	outputfile.open(fileOutput, ios::out | ios::binary);

	// If successful, loop through chunks matching chunkName
	if (outputfile.is_open()) {
		bool filefound = true;
		int counter = 1;
		int fileSize = 0;

		while (filefound) {
			filefound = false;

			// Build the filename
			fileName.clear();
			fileName.append(chunkName);
			fileName.append(".");

			char intBuf[10];
//			_itoa(counter,intBuf,10);
			snprintf(intBuf, 10, "%d", counter);
			fileName.append(intBuf);

			// Open chunk to read
			ifstream fileInput;
			fileInput.open(fileName.c_str(), ios::in | ios::binary);

			// If chunk opened successfully, read it and write it to 
			// output file.
			if (fileInput.is_open()) {
				filefound = true;
				fileSize = getFileSize(&fileInput);
				char *inputBuffer = new char[fileSize];

				fileInput.read(inputBuffer,fileSize);
				outputfile.write(inputBuffer,fileSize);
				delete(inputBuffer);

				fileInput.close();
			}
			counter++;
		}

		// Close output file.
		outputfile.close();

		cout << "File assembly complete!" << endl;
	}
	else { cout << "Error: Unable to open file for output." << endl; }

}


int main(int argc, char** argv)
{

	std::string filename = argv[1];
        std::string input_path = argv[2];
        std::string output_path = argv[3];

        char *c_input_path = const_cast<char*>(input_path.append(filename).c_str());
        char *c_output_path = const_cast<char*>(output_path.append(filename).c_str());

        join_file(c_input_path, c_output_path);

//	join_file(argv[1], argv[2]);
//	join_file("test1", "test1_reassembled");
	return 0;

}
