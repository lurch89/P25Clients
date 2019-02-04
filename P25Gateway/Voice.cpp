/*
*   Copyright (C) 2017,2018,2019 by Jonathan Naylor G4KLX
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "Voice.h"
#include "Log.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include <sys/stat.h>

const unsigned char REC62[] = {
    0x62U, 0x02U, 0x02U, 0x0CU, 0x0BU, 0x12U, 0x64U, 0x00U, 0x00U, 0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

const unsigned char REC63[] = {
    0x63U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC64[] = {
    0x64U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC65[] = {
    0x65U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC66[] = {
    0x66U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC67[] = {
    0x67U, 0xF0U, 0x9DU, 0x6AU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC68[] = {
    0x68U, 0x19U, 0xD4U, 0x26U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC69[] = {
    0x69U, 0xE0U, 0xEBU, 0x7BU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC6A[] = {
    0x6AU, 0x00U, 0x00U, 0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

const unsigned char REC6B[] = {
    0x6BU, 0x02U, 0x02U, 0x0CU, 0x0BU, 0x12U, 0x64U, 0x00U, 0x00U, 0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

const unsigned char REC6C[] = {
    0x6CU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC6D[] = {
    0x6DU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC6E[] = {
    0x6EU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC6F[] = {
    0x6FU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC70[] = {
    0x70U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC71[] = {
    0x71U, 0xACU, 0xB8U, 0xA4U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC72[] = {
    0x72U, 0x9BU, 0xDCU, 0x75U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U };

const unsigned char REC73[] = {
    0x73U, 0x00U, 0x00U, 0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

const unsigned char REC80[] = {
    0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

const unsigned char SILENCE[] = { 0xF0U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x78U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

const unsigned int P25_FRAME_TIME = 20U;

const unsigned int SILENCE_LENGTH = 4U;
const unsigned int IMBE_LENGTH = 11U;

CVoice::CVoice(const std::string& directory, const std::string& language, unsigned int srcId) :
m_language(language),
m_indxFile(),
m_imbeFile(),
m_srcId(srcId),
m_status(VS_NONE),
m_timer(1000U, 1U),
m_stopWatch(),
m_sent(0U),
m_n(0x62U),
m_imbe(NULL),
m_voiceData(NULL),
m_voiceLength(0U),
m_positions()
{
	assert(!directory.empty());
	assert(!language.empty());

#if defined(_WIN32) || defined(_WIN64)
	m_indxFile = directory + "\\" + language + ".indx";
	m_imbeFile = directory + "\\" + language + ".p25";
#else
	m_indxFile = directory + "/" + language + ".indx";
	m_imbeFile = directory + "/" + language + ".p25";
#endif

	// Approximately 10 seconds worth
	m_voiceData = new unsigned char[120U * NXDN_FRAME_LENGTH];
}

CVoice::~CVoice()
{
	for (std::unordered_map<std::string, CPositions*>::iterator it = m_positions.begin(); it != m_positions.end(); ++it)
		delete it->second;

	m_positions.clear();

	delete[] m_imbe;
	delete[] m_voiceData;
}

bool CVoice::open()
{
	FILE* fpindx = ::fopen(m_indxFile.c_str(), "rt");
	if (fpindx == NULL) {
		LogError("Unable to open the index file - %s", m_indxFile.c_str());
		return false;
	}

	struct stat statStruct;
	int ret = ::stat(m_imbeFile.c_str(), &statStruct);
	if (ret != 0) {
		LogError("Unable to stat the AMBE file - %s", m_imbeFile.c_str());
		::fclose(fpindx);
		return false;
	}

	FILE* fpimbe = ::fopen(m_imbeFile.c_str(), "rb");
	if (fpimbe == NULL) {
		LogError("Unable to open the AMBE file - %s", m_imbeFile.c_str());
		::fclose(fpindx);
		return false;
	}

	m_imbe = new unsigned char[statStruct.st_size];

	size_t sizeRead = ::fread(m_imbe, 1U, statStruct.st_size, fpimbe);
	if (sizeRead != 0U) {
		char buffer[80U];
		while (::fgets(buffer, 80, fpindx) != NULL) {
			char* p1 = ::strtok(buffer, "\t\r\n");
			char* p2 = ::strtok(NULL, "\t\r\n");
			char* p3 = ::strtok(NULL, "\t\r\n");

			if (p1 != NULL && p2 != NULL && p3 != NULL) {
				std::string symbol  = std::string(p1);
				unsigned int start  = ::atoi(p2) * IMBE_LENGTH;
				unsigned int length = ::atoi(p3) * IMBE_LENGTH;

				CPositions* pos = new CPositions;
				pos->m_start = start;
				pos->m_length = length;

				m_positions[symbol] = pos;
			}
		}
	}

	::fclose(fpindx);
	::fclose(fpimbe);

	LogInfo("Loaded the audio and index file for %s", m_language.c_str());

	return true;
}

void CVoice::linkedTo(unsigned int tg)
{
	char letters[10U];
	::sprintf(letters, "%u", tg);

	std::vector<std::string> words;
	if (m_positions.count("linkedto") == 0U) {
		words.push_back("linked");
		words.push_back("2");
	} else {
		words.push_back("linkedto");
	}

	for (unsigned int i = 0U; letters[i] != 0x00U; i++)
		words.push_back(std::string(1U, letters[i]));

	createVoice(tg, words);
}

void CVoice::unlinked()
{
	std::vector<std::string> words;
	words.push_back("notlinked");

	createVoice(9999U, words);
}

void CVoice::createVoice(unsigned int tg, const std::vector<std::string>& words)
{
	m_voiceLength = 0U;
	for (std::vector<std::string>::const_iterator it = words.begin(); it != words.end(); ++it) {
		if (m_positions.count(*it) > 0U) {
			CPositions* position = m_positions.at(*it);
			m_voiceeLength += position->m_length;
		} else {
			LogWarning("Unable to find character/phrase \"%s\" in the index", (*it).c_str());
		}
	}

	// Add space for silence before and after the voice
	m_voiceLength += SILENCE_LENGTH * IMBE_LENGTH;
	m_voiceLength += SILENCE_LENGTH * IMBE_LENGTH;

	// Fill the IMBE data with silence
	unsigned int offset = 0U;
	for (unsigned int i = 0U; i < (m_voiceLength / IMBE_LENGTH); i++, offset += IMBE_LENGTH)
		::memcpy(m_voiceData + offset, SILENCE, IMBE_LENGTH);

	// Put offset in for silence at the beginning
	unsigned int pos = SILENCE_LENGTH * IMBE_LENGTH;
	for (std::vector<std::string>::const_iterator it = words.begin(); it != words.end(); ++it) {
		if (m_positions.count(*it) > 0U) {
			CPositions* position = m_positions.at(*it);
			unsigned int start  = position->m_start;
			unsigned int length = position->m_length;
			::memcpy(m_voiceData + pos, m_imbe + start, length);
			pos += length;
		}
	}
}

unsigned int CVoice::read(unsigned char* data)
{
	assert(data != NULL);

	if (m_status != VS_SENDING)
		return 0U;

	unsigned int count = m_stopWatch.elapsed() / P25_FRAME_TIME;

	if (m_sent < count) {
		unsigned int offset = m_sent * NXDN_FRAME_LENGTH;
		::memcpy(data, m_voiceData + offset, NXDN_FRAME_LENGTH);

		offset += NXDN_FRAME_LENGTH;
		m_sent++;

		if (offset >= m_voiceLength) {
			m_timer.stop();
			m_voiceLength = 0U;
			m_status = VS_NONE;
		}

		return NXDN_FRAME_LENGTH;
	}

	return 0U;
}

void CVoice::eof()
{
	if (m_voiceLength == 0U)
		return;

	m_status = VS_WAITING;

	m_timer.start();
}

void CVoice::clock(unsigned int ms)
{
	m_timer.clock(ms);
	if (m_timer.isRunning() && m_timer.hasExpired()) {
		if (m_status == VS_WAITING) {
			m_stopWatch.start();
			m_status = VS_SENDING;
			m_sent = 0U;
			m_n = 0x62U;
		}
	}
}