//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace XboxGameBarServer
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(LaunchActivatedEventArgs^ e) override;
		virtual void OnActivated(IActivatedEventArgs^ e) override;
	internal:
		App();

	private:
		void OnSuspending(Object^ sender, SuspendingEventArgs^ e);
		void OnNavigationFailed(Object^ sender, NavigationFailedEventArgs^ e);
		void OverlayWidgetWindowClosedHandler(Object^ sender, CoreWindowEventArgs^ e);
		EventRegistrationToken overlayWidgetWindowClosedHandlerToken{};
		XboxGameBarWidget^ overlayWidget{ nullptr };
	};
}
