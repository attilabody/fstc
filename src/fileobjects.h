/*
 * fileobjects.h
 *
 *  Created on: Dec 31, 2018
 *      Author: compi
 */

#ifndef FILEOBJECTS_H_
#define FILEOBJECTS_H_

#include <iostream>
#include <string>
#include "crc32.h"

struct FileInfo
{
	virtual bool operator==(FileInfo const &other) = 0;
	virtual ~FileInfo() = default;
};

struct RegularFileInfo : public FileInfo
{
	RegularFileInfo(std::string const &path);
	virtual bool operator==(FileInfo const &other);
	std::streampos GetLength() { return m_length; }
	uint32_t GetCrc() { return m_crc; }
private:
	std::streampos	m_length;
	uint32_t		m_crc;
};

#endif /* FILEOBJECTS_H_ */
