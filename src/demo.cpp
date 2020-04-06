#include <iostream>
#include <cstddef>
#include <algorithm>
#include <thread>

#include "sargparse/Parameter.h"
#include "sargparse/File.h"

#include "cndl/Request.h"
#include "cndl/Response.h"
#include "cndl/Error.h"
#include "cndl/Route.h"
#include "cndl/ConnectionHandler.h"

#include "cndl/Server.h"

#include "simplyfile/socket/Socket.h"
#include "simplyfile/socket/Host.h"
#include "simplyfile/Epoll.h"
#include "simplyfile/INotify.h"

#include "qrqma/template.h"

#include <fstream>


namespace 
{

void demo();
sargp::Task demo_task{demo};
auto optTemplPat = sargp::Parameter<sargp::Directory>("templates", "template_dir", "where to find the templates");

cndl::GlobalRoute globalRoute {"/global/?", [](cndl::Request const&) -> cndl::OptResponse {
    static qrqma::Template templ{qrqma::defaultLoader()("templates/global.html")};
    return templ();
}, {.methods={"GET", "POST"}}};

auto serveFile(std::filesystem::path file) -> cndl::OptResponse {
	auto ifs = std::ifstream(file.string(), std::ios::binary);
	std::stringstream buffer;
	buffer << ifs.rdbuf();

	auto response = cndl::Response{buffer.str()};
	if (file.extension() == ".css") {
		response.fields["Content-Type"] = "text/css; charset=utf-8";
		return response;
	} else if (file.extension() == ".js") {
		response.fields["Content-Type"] = "text/javascript; charset=utf-8";
		return response;
	} else if (file.extension() == ".png") {
		response.fields["Content-Type"] = "image/png";
		return response;
	} else {
		response.fields["Content-Type"] = "text/html; charset=utf-8";
		return response;
	}
	std::cout << "not found " << file << "\n";
	return std::nullopt;
};

cndl::GlobalRoute template_route{"/(.*)?", [](cndl::Request const& request, std::string const& req_pat) -> cndl::OptResponse {
	return serveFile("www/" + req_pat);
}, {.methods={"GET", "POST"}}};

struct : cndl::WebsocketHandler {
    using Websocket = cndl::Websocket;
    using Request = cndl::Request;
    void onMessage([[maybe_unused]] Websocket& ws, [[maybe_unused]] AnyMessage msg) override {
        if (std::holds_alternative<TextMessage>(msg)) {
            auto txt = std::get<TextMessage>(msg);
            std::cout << txt << std::endl;
            if (txt == "close") {
                ws.close();
            }
            if (txt == "term") {
                cndl::Server::getGlobalServer().stop_looping();
            }
        }
        ws.send(msg);
    }

    bool onOpen([[maybe_unused]] Request const& request, [[maybe_unused]] Websocket& ws) {
        std::cout << "socket connected " << request.header.url << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        using namespace std::literals::chrono_literals;
//        ws.setAutoPing(1000ms, 100ms);
        return true;
    }

    void onClose([[maybe_unused]] Websocket& ws) override {
        std::cout << "socket closed" << std::endl;
    }

    void onPong([[maybe_unused]] Websocket& ws, [[maybe_unused]] BinMessage message) override {
        std::cout << "socket ponged" << std::endl;
    }
} echo_handler{};

void demo()
{
    cndl::Server& server = cndl::Server::getGlobalServer();
    server.listen(simplyfile::getHosts("localhost", "8080"));
    
    cndl::WSRoute wsroute{std::regex{R"(/ws)"}, echo_handler};
    server.getDispatcher().addRoute(&wsroute);

	auto timer1 = simplyfile::Timer{std::chrono::milliseconds{10}};
	auto timer2 = simplyfile::Timer{std::chrono::seconds{1}};

	simplyfile::Epoll epoll;
	epoll.addFD(timer1, [&](int) {
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
		timer1.getElapsed(); // clear timer
	});
	epoll.addFD(timer2, [&](int) {
		timer2.getElapsed(); // clear timer;
	});

    epoll.addFD(server, [&](int) {
        server.work();
        epoll.modFD(server, EPOLLIN | EPOLLONESHOT);
    }, EPOLLIN | EPOLLONESHOT);

    while(true) {
        epoll.work();
    }

    server.loop_forever();
}

}
