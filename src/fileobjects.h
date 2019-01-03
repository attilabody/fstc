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
#include <experimental/filesystem>
#include "crc32.h"

struct FileInfo
{
	FileInfo(std::experimental::filesystem::path const &path) : m_status(std::experimental::filesystem::symlink_status(path)) {}
	virtual bool operator==(FileInfo const &other) = 0;
	bool operator!=(FileInfo const &other) { return !operator==(other); }
	virtual ~FileInfo() = default;
protected:
	std::experimental::filesystem::file_status	m_status;
};

struct RegularFileInfo : public FileInfo
{
	RegularFileInfo(std::experimental::filesystem::path const &path);
	virtual bool operator==(FileInfo const &other);
	std::streampos GetLength() const { return m_length; }
	uint32_t GetCrc() const { return m_crc; }
private:
	std::streampos	m_length;
	uint32_t		m_crc;
};

struct SymLinkInfo : public FileInfo
{
	SymLinkInfo(std::experimental::filesystem::path const &path);
	virtual bool operator==(FileInfo const &other);
private:
	std::string m_target;
};
#endif /* FILEOBJECTS_H_ */
