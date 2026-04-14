#ifndef RECORD_DATA_H
#define RECORD_DATA_H

#pragma once
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <memory>

#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>

#define DEBUG

class RecordData
{
public:
	RecordData();
	~RecordData();
	void OpenJsonFile(const std::string& filename);
	void SetCurData(const std::string& data);
	void SetDataPerFrame();
	void WriteToJsonFile();

private:
	std::string mCurFrameNum;
	std::string mID;
	std::string mPosition;
	std::ofstream mFile;
	std::string mJsonFilename;
	Json::Value mIDValues;
	Json::Value mPerFrameData;
	Json::Value mJsonRoot;
};

#endif // RECORD_DATA_H