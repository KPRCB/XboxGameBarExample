#include "XboxGameBarServer.h"

void XboxGameBarServer::addObjectToFileMapping(const void* source, rsize_t sourceSize) {

	memcpy_s((void*)((char*)mappedData + fileMappingOffset), sourceSize, source, sourceSize);

	fileMappingOffset += sourceSize;
}

void XboxGameBarServer::addShapeToFileMapping(const void* objectID, rsize_t sourceSize, const void* shape, rsize_t source2Size) {
	addObjectToFileMapping(objectID, sourceSize);
	addObjectToFileMapping(shape, source2Size);
}

XboxGameBarServer::XboxGameBarServer() {
	while (!(mutex = OpenMutex(SYNCHRONIZE, FALSE, mutexPath.c_str()))) Sleep(1000); //initialise mutex
	while (!(fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileMappingPath.c_str()))) Sleep(1000); //initialise filemapping

	mappedData = MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 4096);
}

void XboxGameBarServer::beginFrame() {

	WaitForSingleObject(mutex, INFINITE); //wait for the mutex to be freed
	fileMappingOffset = 0; //reset the fileMapping offset 
}

void XboxGameBarServer::endFrame() {
	int8_t endFrameID = _endFrame; //add end code to last to signal the end of the list of objects
	addObjectToFileMapping(&endFrameID, sizeof(int8_t));
	ReleaseMutex(mutex); //release the mutex so xboxGameBarServer can operate on the store data
}

Color XboxGameBarServer::getColorFromArgb(int a, int r, int g, int b) {
	return Color{ (int8_t)a, (int8_t)r, (int8_t)g, (int8_t)b };
}
void XboxGameBarServer::addLine(float x1, float y1, float x2, float y2, Color color) {

	Line lineObject = { float2{x1, y1}, float2{x2, y2}, color };
	int8_t objectID = line;
	addShapeToFileMapping(&objectID, sizeof(int8_t), &lineObject, sizeof(Line));
}
void XboxGameBarServer::addRectangle(float x, float y, float width, float height, Color color) {

	_Rectangle rectObject = { Rect{height, width, x, y}, color };
	int8_t objectID = rectangle;
	addShapeToFileMapping(&objectID, sizeof(int8_t), &rectObject, sizeof(_Rectangle));
}
void XboxGameBarServer::addCircle(float x, float y, float radius, Color color) {

	Circle circleObject = { float2{x, y}, radius, color };
	int8_t objectID = circle;
	addShapeToFileMapping(&objectID, sizeof(int8_t), &circleObject, sizeof(Circle));
}

void XboxGameBarServer::addText(std::string text, float x, float y, Color color, float fontSize) {

	TextData textData = { text.length(), float2{x, y}, color, fontSize };
	int8_t objectID = _text;

	addShapeToFileMapping(&objectID, sizeof(int8_t), &textData, sizeof(TextData));

	addObjectToFileMapping(text.c_str(), text.length());
}
