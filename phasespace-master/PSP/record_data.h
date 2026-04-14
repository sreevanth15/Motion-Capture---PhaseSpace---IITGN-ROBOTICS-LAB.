#ifndef RECORD_DATA_H
#define RECORD_DATA_H

#pragma once
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <memory>

#include <owl/owl.hpp>
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>

class RecordData
{
public:
	RecordData();
	~RecordData();
	void OpenJsonFile(const std::string& filename);
	void SetCurFrameNum(const int& frameNum);
	void SetCurData(const int& id, const float& x, const float& y, const float& z);
	void SetDataPerFrame();
	void WriteToJsonFile();

private:
	int mCurFrameNum;
	std::ofstream mFile;
	std::string mJsonFilename;
	Json::Value mIDValues;
	Json::Value mPerFrameData;
	Json::Value mJsonRoot;
};

#endif // RECORD_DATA_H