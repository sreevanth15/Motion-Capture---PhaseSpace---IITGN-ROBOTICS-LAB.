/* Author: Yu Ting Lai
// This code will record the data in JSON format
// It will only record the time frame that has data in it
// Note that the data is recorded per "frame"
// To convert it to seconds, please divided by the frequency (960) of motion capture
*/

#include "record_data.h"
#include <iostream>

RecordData::RecordData()
	: mJsonFilename("data.json")
{
	OpenJsonFile(mJsonFilename);
}

RecordData::~RecordData()
{
	WriteToJsonFile();
	mFile.close();
}

void RecordData::OpenJsonFile(const std::string& filename)
{
	mFile.open(filename, std::ofstream::out);
	if (!mFile.is_open())
	{
		std::cout << "[Record] File creation incomplete!" << std::endl;
	}
	return;
}

void RecordData::SetCurFrameNum(const int& frameNum)
{
	mCurFrameNum = frameNum;
}

void RecordData::SetCurData(const int& id, 
	const float& x, const float& y, const float& z)
{
	std::string value = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	mIDValues[std::to_string(id)] = value;
}

void RecordData::SetDataPerFrame()
{
	if (mIDValues.size())
	{
		mJsonRoot[std::to_string(mCurFrameNum)] = mIDValues;
		mIDValues.clear();
	}
}

void RecordData::WriteToJsonFile()
{
	if (mJsonRoot)
	{
		mFile << mJsonRoot;
	}
}