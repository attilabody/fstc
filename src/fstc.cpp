//============================================================================
// Name        : fstc.cpp
// Author      : Attila BODY
// Version     :
// Copyright   : LGPLv3
// Description : File tree compare
//============================================================================

#include <iostream>
#include <map>
#include <experimental/filesystem>

#include "crc32.h"
#include "fileobjects.h"

using namespace std;
namespace fs = std::experimental::filesystem;

string g_path1;
string g_path2;
bool g_full = true;
map<string,unique_ptr<FileInfo>>	g_files;

void parsearg(int &arg, char*argv[])
{
	switch(arg)
	{
	case 1:
		g_path1 = argv[arg++];
		break;

	case 2:
		g_path2 = argv[arg++];
		break;

	default:
		++arg;
	}
}

bool iterate_dir(map<string, uint32_t> files, string const &path)
{
	return true;
}

int main(int argc, char *argv[])
{
	int arg = 1;

	while(arg <= argc)
		parsearg(arg, argv);

	size_t baseLength = g_path1.length() + ((*g_path1.rend() == '/') ? 0 : 1);

	for( auto &p: fs::recursive_directory_iterator(g_path1))
	{
		if(fs::is_regular_file(p.status())) {
			string path(p.path());
			RegularFileInfo *fi = new RegularFileInfo(path);
			path.erase(0, baseLength);
			cout << static_cast<string const&>(path) << ": " << fi->GetLength() << ", " << fi->GetCrc() << endl;
			g_files.emplace(path, fi);
		}
	}
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
