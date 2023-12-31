﻿//
// App.xaml.cpp
// Implementation of the App class.
//
#include "pch.h"
#include "MainPage.xaml.h"

using namespace XboxGameBarServer;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

void App::OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs^ e)
{
    XboxGameBarWidgetActivatedEventArgs^ widgetArgs = nullptr;

    if (e->Kind == ActivationKind::Protocol)
    {
        auto protocolArgs = dynamic_cast<IProtocolActivatedEventArgs^>(e);
        if (protocolArgs)
        {
            const wchar_t* scheme = protocolArgs->Uri->SchemeName->Data();
            if (0 == wcscmp(scheme, L"ms-gamebarwidget")) widgetArgs = dynamic_cast<XboxGameBarWidgetActivatedEventArgs^>(e);
        }
    }

    if (widgetArgs)
    {
        if (widgetArgs->IsLaunchActivation)
        {
            auto rootFrame = ref new Frame();
            rootFrame->NavigationFailed += ref new NavigationFailedEventHandler(this, &App::OnNavigationFailed);
            Window::Current->Content = rootFrame;

            overlayWidget = ref new XboxGameBarWidget(widgetArgs, Window::Current->CoreWindow, rootFrame);

            overlayWidget->VerticalResizeSupported = true;
            overlayWidget->HorizontalResizeSupported = true;

            float width = GetSystemMetrics(SM_CXSCREEN);
            float height = GetSystemMetrics(SM_CYSCREEN);

            overlayWidget->MinWindowSize = Size(width, height - 40);
            overlayWidget->MaxWindowSize = Size(7680, 4320);

            overlayWidget->CenterWindowAsync();
            overlayWidget->TryResizeWindowAsync(Size(width, height - 40));

            rootFrame->Navigate(TypeName(MainPage::typeid), nullptr);

            overlayWidgetWindowClosedHandlerToken = Window::Current->Closed += ref new WindowClosedEventHandler(this, &App::OverlayWidgetWindowClosedHandler);
            Window::Current->Activate();
        }
    }
}

void App::OverlayWidgetWindowClosedHandler(Platform::Object^ /*sender*/, Windows::UI::Core::CoreWindowEventArgs^ /*e*/)
{
    overlayWidget = nullptr;
    Window::Current->Closed -= overlayWidgetWindowClosedHandlerToken;
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        rootFrame = ref new Frame();

        rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete

        }

        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Place the frame in the current Window
            Window::Current->Content = rootFrame;
            // Ensure the current window is active
            Window::Current->Activate();
        }
    }
    else
    {
        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Ensure the current window is active
            Window::Current->Activate();
        }
    }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void) sender;  // Unused parameter
    (void) e;   // Unused parameter

    //TODO: Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}