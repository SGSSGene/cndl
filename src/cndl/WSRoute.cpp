// SPDX-FileCopyrightText: 2020 Lutz Freitag
// SPDX-License-Identifier: MIT
#include "WSRoute.h"
#include "Server.h"

namespace cndl
{

void registerRouteGlobally(WSRouteBase& route) {
    Server::getGlobalServer().getDispatcher().addRoute(route);
}
void deregisterRouteGlobally(WSRouteBase& route) {
    Server::getGlobalServer().getDispatcher().removeRoute(route);
}

}
