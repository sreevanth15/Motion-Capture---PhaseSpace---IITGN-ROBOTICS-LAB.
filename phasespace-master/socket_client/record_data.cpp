/* Author: Yu Ting Lai
// This code will record the data in JSON format
// It will only record the time frame that has data in it
// Note that the data is recorded per "frame"
// To convert it to seconds, please divided by the frequency (960) of motion capture
*/

#include "record_data.h"
#include <iostream>

RecordData::RecordData()
	: mJsonFilename("data_received.json")
{
	//OpenJsonFile(mJsonFilename);
}

RecordData::~RecordData()
{
	//WriteToJsonFile();
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

void RecordData::SetCurData(const std::string& data)
{
	std::string delimiter = ",";
	std::string token;
	std::string trimmedData(data.c_str());
	int count = 0;
	size_t pos = 0;
	while ((pos = trimmedData.find(delimiter)) != std::string::npos)
	{
		token = trimmedData.substr(0, pos);
		if (count == 0)
		{
			mCurFrameNum = token;
			std::cout << "[Record] Frame num = " << mCurFrameNum << std::endl;
		}
		else if (count == 1)
		{
			mID = token;
			std::cout << "[Record] ID = " << mID << std::endl;
		}
		else
		{
			mPosition.assign(trimmedData);
			std::cout << "[Record] Position = " << mPosition << std::endl;
			break;
		}
		trimmedData.erase(0, pos + delimiter.length());
		count++;
	}
	mIDValues[mID] = mPosition;
	SetDataPerFrame();
}

void RecordData::SetDataPerFrame()
{
	if (mIDValues.size())
	{
		mJsonRoot[mCurFrameNum] = mIDValues;
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