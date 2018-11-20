#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>         // std::string
#include <cstddef>         // std::size_t

using namespace std; 
// Chunks a file by breaking it up into chunks of "chunkSize" bytes.
void split_file(char *fullFilePath, char *chunkName, unsigned long chunkSize) {
	ifstream fileStream;
	fileStream.open(fullFilePath, ios::in | ios::binary);

	// File open a success
	if (fileStream.is_open()) {
		ofstream output;
		int counter = 1;

		string fullChunkName;

		// Create a buffer to hold each chunk
		char *buffer = new char[chunkSize];

		// Keep reading until end of file
		while (!fileStream.eof()) {

			// Build the chunk file name. Usually drive:\\chunkName.ext.N
			// N represents the Nth chunk
			fullChunkName.clear();
			fullChunkName.append(chunkName);
			fullChunkName.append(".");

			// Convert counter integer into string and append to name.
			char intBuf[10];
			snprintf(intBuf, 10, "%d", counter);
			//itoa(counter,intBuf,10);
			fullChunkName.append(intBuf);

			// Open new chunk file name for output
			output.open(fullChunkName.c_str(),ios::out | ios::trunc | ios::binary);

			// If chunk file opened successfully, read from input and 
			// write to output chunk. Then close.
			if (output.is_open()) { 
				fileStream.read(buffer,chunkSize);
				// gcount() returns number of bytes read from stream.
				output.write(buffer,fileStream.gcount());
				output.close();

				counter++;
			}
		}

		// Cleanup buffer
		delete(buffer);

		// Close input file stream.
		fileStream.close();
		//cout << "Chunking complete! " << counter - 1 << " files created." << endl;
		cout << counter - 1 << endl;
	}
	else { cout << "Error opening file!" << endl; }
}


using namespace std;
int main(int argc, char** argv) 
{
	/*
	std::string filename = argv[1];
	std::string input_path = argv[2];
	std::string output_path = argv[3];
	unsigned long chunk_size = atol(argv[4]);

	char *c_input_path = const_cast<char*>(input_path.append(filename).c_str());
	char *c_output_path = const_cast<char*>(output_path.append(filename).c_str());

	split_file(c_input_path, c_output_path, chunk_size);
	*/

	char *c_input_path = argv[1];
	char *c_output_path = argv[2];
	unsigned long chunk_size = atol(argv[3]);

        split_file(c_input_path, c_output_path, chunk_size);
	return 0;
}
