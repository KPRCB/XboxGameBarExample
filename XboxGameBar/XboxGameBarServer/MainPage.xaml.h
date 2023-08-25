//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include <Windows.h>
#include <thread>
#include <string>
#include <sstream>
#include <Windowsnumerics.h>

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;

namespace XboxGameBarServer
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
		void OnCanvasSwapChainLoaded(Object^ sender, RoutedEventArgs^ e);
	};
}
