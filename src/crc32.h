/*
 * crc32.cpp
 *
 *  Created on: Dec 31, 2018
 *      Author: compi
 */

#pragma once

#include <cstdint>
#include <cstddef>

struct Crc32_table
{
	uint32_t m_table[256];

	Crc32_table()
	{
		uint32_t polynomial = 0xEDB88320;
		for(uint32_t i = 0; i < 256; i++)
		{
			uint32_t c = i;
			for(std::size_t j = 0; j < 8; j++) {
				if(c & 1)
					c = polynomial ^ (c >> 1);
				else
					c >>= 1;
			}
			m_table[i] = c;
		}
	}
};

class Crc32
{
public:
	void Update(const void* buf, size_t len)
	{

		const uint8_t* u = static_cast<const uint8_t*>(buf);
		for(std::size_t i = 0; i < len; ++i)
			m_crc = m_t.m_table[(m_crc ^ u[i]) & 0xFF] ^ (m_crc >> 8);
	}

	operator uint32_t() { return m_crc ^ 0xFFFFFFFF; }
private:
	static Crc32_table m_t;
	uint32_t m_crc = 0xFFFFFFFF;
};
