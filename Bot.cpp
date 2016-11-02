//
// Created by aidan on 11/1/16.
//

#include "Bot.h"

#include <functional>

using namespace discordpp;

Bot::Bot(std::string token)
        : token_(token) {
}

Bot::~Bot() {

}

void Bot::setResponse(std::string event, std::function<void(Bot*, json)> response){
    if(event != "READY"){
        eventResponses_[event] = std::bind(response, this, std::placeholders::_1);
    }
}

void Bot::start() {
    asio::io_service asio_ios_;

    discordpp::Client discordClient(asio_ios_, token_, eventResponses_);

    // Construct a signal set registered for process termination.
    boost::asio::signal_set signals(asio_ios_, SIGINT, SIGTERM);

    // Start an asynchronous wait for one of the signals to occur.
    signals.async_wait([&asio_ios_](const error_code &error, int signal_number) {
        if (!error) {
            std::cerr << "User terminated program.\n";
            std::cerr << "Logging out...\n";
            //discordpp::DiscordAPI::auth::logout();
            std::cerr << "Logged out.\n";
            asio_ios_.stop();
        }
    });

    asio_ios_.run();
}