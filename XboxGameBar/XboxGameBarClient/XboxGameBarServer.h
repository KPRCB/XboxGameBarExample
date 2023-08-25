#pragma once
#include <Windows.h>
#include <vector>
#include <variant>
#include "objectTypes.h"

class XboxGameBarServer
{
private:
	HANDLE mutex;
	HANDLE fileMapping;

	const std::string SID = "S-1-15-2-3692403734-2460610335-2548121685-3930139087-1390453771-1376261131-1831030302"; //the SID changes, use winObj, process hacker or api call from within UWP app to get it

	const std::string mutexName = "XboxGameBarServerMutex";
	const std::string mutexPath = "AppContainerNamedObjects\\" + SID + "\\" + mutexName; //concatenate strings to form the path for mutex

	const std::string fileMappingName = "XboxGameBarServerFileMapping";
	const std::string fileMappingPath = "AppContainerNamedObjects\\" + SID + "\\" + fileMappingName; //concatenate strings to form the path for fileMapping

	void* mappedData;

	int fileMappingOffset = 0; //this denotes the relative offset of the fileMapping

	void addObjectToFileMapping(const void* source, rsize_t sourceSize);
	void addShapeToFileMapping(const void* source, rsize_t sourceSize, const void* source2, rsize_t source2Size);

public:
	XboxGameBarServer();
	void beginFrame();
	void endFrame();
	Color getColorFromArgb(int a, int r, int g, int b);
	void addLine(float x1, float y1, float x2, float y2, Color color);
	void addRectangle(float x, float y, float width, float height, Color color);
	void addCircle(float x, float y, float radius, Color color);
	void addText(std::string text, float x, float y, Color color, float fontSize);
};

