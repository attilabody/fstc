/*
 * fileobjects.cpp
 *
 *  Created on: Dec 31, 2018
 *      Author: compi
 */

#include <fstream>
#include <vector>
#include <algorithm>
#include "fileobjects.h"
#include "crc32.h"


using namespace std;
namespace fs = std::experimental::filesystem;

RegularFileInfo::RegularFileInfo(fs::path const &path)
: FileInfo(path)
{
	ifstream f(path.c_str(), ios::binary | ios::ate);
	m_length = f.tellg();
	f.seekg(0, ios_base::beg);
	streampos pos = 0;

	Crc32	hasher;
	vector<char>	buffer(4096);
	while(pos < m_length) {
		streampos count = min(static_cast<streampos>(buffer.size()), static_cast<streampos>(m_length-pos));
		f.read(&buffer[0], count);
		hasher.Update(&buffer[0], count);
		pos += count;
	}
	m_crc = (uint32_t)hasher;
}

bool RegularFileInfo::operator==(const FileInfo &other)
{
	RegularFileInfo const *_other(dynamic_cast<RegularFileInfo const *>(&other));
	return _other
		&& m_status.permissions() == _other->m_status.permissions()
		&& m_crc == _other->m_crc
		&& m_length == _other->m_length;
}

SymLinkInfo::SymLinkInfo(fs::path const &path)
: FileInfo(path)
, m_target( fs::read_symlink(path))
{
}

bool SymLinkInfo::operator==(const FileInfo &other)
{
	SymLinkInfo const *_other(dynamic_cast<SymLinkInfo const *>(&other));
	return _other
		&& m_status.permissions() == _other->m_status.permissions()
		&& m_target == _other->m_target;
}
