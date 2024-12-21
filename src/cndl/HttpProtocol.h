// SPDX-FileCopyrightText: 2020 Lutz Freitag
// SPDX-License-Identifier: MIT
#pragma once

#include "ProtocolHandler.h"
#include "Request.h"

#include <optional>

namespace cndl {

struct HttpProtocol : ProtocolHandler {
    using ByteView = ProtocolHandler::ByteView;
    using ByteBuf  = ProtocolHandler::ByteBuf;
    using ConsumeResult = ProtocolHandler::ConsumeResult;

    using ProtocolHandler::ProtocolHandler;
    virtual ~HttpProtocol() = default;

    ConsumeResult onDataReceived(ByteView received);

private:
    std::optional<Request::Header> header;
};


}
