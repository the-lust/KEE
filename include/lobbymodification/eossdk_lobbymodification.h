#pragma once

#include "common_includes.h"
#include "callback_manager.h"

namespace sdk
{
    class EOSSDK_Lobbymodification : public IRunCallback
    {
    public:
        EOSSDK_Lobbymodification() {}
        ~EOSSDK_Lobbymodification() {}

        virtual bool CBRunFrame() { return false; }
        virtual bool RunCallbacks(pFrameResult_t res) { return false; }
        virtual void FreeCallback(pFrameResult_t res) {}
    };
}
