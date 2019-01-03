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
map<string,unique_ptr<FileInfo>>	g_files1;
map<string,unique_ptr<FileInfo>>	g_files2;

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

void LoadTree(map<string,unique_ptr<FileInfo>> &dst, string const &treePath)
{
	size_t baseLength = treePath.length() + ((*treePath.rend() == fs::path::preferred_separator) ? 0 : 1);

	for( auto &entry: fs::recursive_directory_iterator(treePath))
	{
		fs::path path(entry.path());
		FileInfo *fi;

		if(fs::is_symlink(entry.symlink_status())) {
			fi = new SymLinkInfo(path);
		} else if(fs::is_regular_file(entry.status())) {
			fi = new RegularFileInfo(path);
		} else continue;

		string s(path.c_str()+baseLength, path.c_str()+path.string().length());
		dst.emplace(s, fi);
	}
}

int main(int argc, char *argv[])
{
	int arg = 1;

	while(arg < argc)
		parsearg(arg, argv);

	LoadTree(g_files1, g_path1);
	LoadTree(g_files2, g_path2);

	for(auto const& entry: g_files1) {
		auto it(g_files2.find(entry.first));
		if(it == end(g_files2))
			cerr << "File " << entry.first << " not found in second tree." << endl;
		else if(*entry.second.get() != *it->second.get())
			cerr << entry.first << " is different." << endl;
	}

	for(auto const& entry: g_files2) {
		auto it(g_files1.find(entry.first));
		if(it == end(g_files1)) cerr << "File " << entry.first << " not found in first tree." << endl;
	}

	return 0;
}
