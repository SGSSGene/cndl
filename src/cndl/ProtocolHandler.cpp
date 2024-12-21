// SPDX-FileCopyrightText: 2020 Lutz Freitag
// SPDX-License-Identifier: MIT
#include "ProtocolHandler.h"

#include "ConnectionHandler.h"

namespace cndl {

size_t ProtocolHandler::getOutBufferSize() const {
    return connection_handler->getOutBufferSize();
}

}
