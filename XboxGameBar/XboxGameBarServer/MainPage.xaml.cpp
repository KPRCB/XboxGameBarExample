//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "MainPage.xaml.h"

using namespace XboxGameBarServer;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::UI;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Windows::UI::ViewManagement;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

enum {
	_endFrame,
	rectangle,
	line,
	circle,
	_text
};

typedef struct Rectangle {
	Rect rect;
	Color color;
};

typedef struct Line {
	float2 point1;
	float2 point2;
	Color color;
};

typedef struct Circle {
	float2 point;
	float radius;
	Color color;
};

typedef struct TextData {
	uint32 textLength;
	float2 point;
	Color color;
	float fontSize;
};

void* mappedData;

MainPage::MainPage()
{
	InitializeComponent();
}

String^ ConvertStdStringToPlatformString(const std::string& str)
{
	std::wstring wstr(str.begin(), str.end());
	return ref new String(wstr.c_str());
}

void readObjectFromFileMapping(void* destination, rsize_t destinationSize, int& offset) {
	memcpy_s(destination, destinationSize, (void*)((char*)mappedData + offset), destinationSize);
	offset += destinationSize;
}

void initiateNamedObjects(HANDLE& mutex, HANDLE& fileMapping) {

	mutex = CreateMutex(NULL, FALSE, L"XboxGameBarServerMutex");

	fileMapping = CreateFileMappingFromApp(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 4096, L"XboxGameBarServerFileMapping");
}

void handleObject(int objectID, int& offset, CanvasDrawingSession^ drawingSession) {
	switch (objectID) {
		case rectangle:
		{
			Rectangle rectangle;

			readObjectFromFileMapping(&rectangle, sizeof(Rectangle), offset);

			drawingSession->DrawRectangle(rectangle.rect, rectangle.color);


			break; 
		}	
		case line:
		{
			Line line;

			readObjectFromFileMapping(&line, sizeof(Line), offset);

			drawingSession->DrawLine(line.point1, line.point2, line.color);

			break;
		}		
		case circle:
		{
			Circle circle;

			readObjectFromFileMapping(&circle, sizeof(Circle), offset);

			drawingSession->DrawCircle(circle.point, circle.radius, circle.color);

			break;
		}
		case _text:
		{
			TextData textData;

			readObjectFromFileMapping(&textData, sizeof(TextData), offset);

			std::string text;

			text.resize(textData.textLength);

			readObjectFromFileMapping(&text[0], textData.textLength, offset);

			CanvasTextFormat^ textFormat = ref new CanvasTextFormat();

			textFormat->FontSize = textData.fontSize;

			drawingSession->DrawText(ConvertStdStringToPlatformString(text), textData.point, textData.color, textFormat);

			break;
		}	
	}

}


void drawingThread(CanvasSwapChainPanel^ canvas) {

	HANDLE mutex;

	HANDLE fileMapping;

	initiateNamedObjects(mutex, fileMapping);

	if (!mutex || !fileMapping) goto error;

	static auto drawingSession = canvas->SwapChain->CreateDrawingSession(Colors::Transparent);

	mappedData = MapViewOfFileFromApp(fileMapping, FILE_MAP_ALL_ACCESS, 0, 4096);

	if (!mappedData) goto error;

	for (;;) {
		int offset = 0;

		WaitForSingleObject(mutex, INFINITE);

		drawingSession->Clear(Colors::Transparent);

		while (true) {
			int8 objectID; 
			
			memcpy_s(&objectID, sizeof(int8), (void*)((char*)mappedData + offset), sizeof(int8));

			if (objectID == _endFrame) break;
			offset += sizeof(int8);
			
			handleObject(objectID, offset, drawingSession);
		}

		drawingSession->Flush();
		canvas->SwapChain->Present();
		ReleaseMutex(mutex);
	}
error:
	CloseHandle(fileMapping);
	CloseHandle(mutex);
	drawingSession->Clear(Colors::Transparent);
	drawingSession->DrawText("Error has Occured", 0, 0, Colors::Red);
	drawingSession->Flush();
	canvas->SwapChain->Present();
}

void MainPage::OnCanvasSwapChainLoaded(Object^ sender, RoutedEventArgs^ e)
{
	CanvasSwapChainPanel^ canvas;

	canvasSwapChainPanel->SwapChain = ref new CanvasSwapChain(CanvasDevice::GetSharedDevice(), (float)Window::Current->CoreWindow->Bounds.Width, (float)Window::Current->CoreWindow->Bounds.Height, 96);

	canvas = canvasSwapChainPanel;

	std::thread drawingThread(drawingThread, canvas);

	drawingThread.detach();
}
