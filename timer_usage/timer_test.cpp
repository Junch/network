/*
 * basic_usage.cpp
 *
 *  Created on: Nov 1, 2018
 *      Author: frank
 */
#include <functional>
#include <iostream>
#include <asio.hpp>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("using a timer synchronously", "[syn]")
{
    asio::io_context ioc;
    asio::steady_timer t(ioc, std::chrono::seconds(1));
    t.wait();
    std::cout << "timeout after 1 seconds\n";
}

TEST_CASE("using a timer asynchronously", "[asyn]")
{
    asio::io_context ioc;
    asio::steady_timer timer1(ioc, std::chrono::seconds(1));
    timer1.async_wait([](const std::error_code &ec)
                      {
                          if (!ec) // asio::error::operation_aborted
                              std::cout << "timer1 timeout after 1 seconds\n";
                      });

    SECTION("With the timer")
    {
    }

    SECTION("Add another timer")
    {
        asio::steady_timer timer2(ioc);
        timer2.expires_after(std::chrono::milliseconds(100));
        timer2.async_wait([&timer1](const std::error_code &)
                          { std::cout << "timer2 timeout after 100ms\n"; });
    }

    SECTION("Cancle one timer")
    {
        asio::steady_timer timer2(ioc);
        timer2.expires_after(std::chrono::milliseconds(100));
        timer2.async_wait([&timer1](const std::error_code &)
                          {
                              timer1.cancel();
                              std::cout << "timer2 timeout after 100ms\n";
                          });
    }

    ioc.run();
    std::cout << "----\n";
}
