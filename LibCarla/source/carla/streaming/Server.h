// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/streaming/detail/AsioThreadPool.h"
#include "carla/streaming/detail/tcp/Server.h"
#include "carla/streaming/low_level/Server.h"

#include <boost/asio/io_service.hpp>

namespace carla {
namespace streaming {

  /// A streaming server. Each new stream has a token associated, this token can
  /// be used by a client to subscribe to the stream.
  class Server {
    using underlying_server = low_level::Server<detail::tcp::Server>;
    using protocol_type = low_level::Server<detail::tcp::Server>::protocol_type;
  public:

    explicit Server(uint16_t port)
      : _server(_service.service(), make_endpoint<protocol_type>(port)) {}

    explicit Server(const std::string &address, uint16_t port)
      : _server(_service.service(), make_endpoint<protocol_type>(address, port)) {}

    explicit Server(
        const std::string &address, uint16_t port,
        const std::string &external_address, uint16_t external_port)
      : _server(
          _service.service(),
          make_endpoint<protocol_type>(address, port),
          make_endpoint<protocol_type>(external_address, external_port)) {}

    ~Server() {
      _service.Stop();
    }

    void SetTimeout(time_duration timeout) {
      _server.SetTimeout(timeout);
    }

    Stream MakeStream() {
      return _server.MakeStream();
    }

    MultiStream MakeMultiStream() {
      return _server.MakeMultiStream();
    }

    void Run() {
      _service.Run();
    }

    void AsyncRun(size_t worker_threads) {
      _service.AsyncRun(worker_threads);
    }

  private:

    // The order of these two arguments is very important.

    detail::AsioThreadPool _service;

    underlying_server _server;
  };

} // namespace streaming
} // namespace carla
