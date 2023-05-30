#pragma once
#include "pch.h"
class WinToastHandler : public WinToastLib::IWinToastHandler
{
public:
	WinToastHandler()
	{
	}

	// Public interfaces
	void toastActivated() const override
	{
	}

	void toastActivated(int actionIndex) const override
	{	}

	void toastDismissed(WinToastDismissalReason state) const override
	{
	}

	void toastFailed() const override
	{
	}
};
