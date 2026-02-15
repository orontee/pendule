#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

enum PlaybackState {
  UNKNOWN,
  PLAYING,
  PAUSED,
  STOPPED
};

struct PlaybackStateChange {
  PlaybackState new_state;

  std::string track_uri;
  std::string track_name;

  std::string album_uri;
  std::string album_name;

  std::string artist_uri;
  std::string artist_name;

  static PlaybackState from_string(const std::string &str) {
    if (str == "playing") {
      return PLAYING;
    } else if (str == "paused") {
      return PAUSED;
    } else if (str == "stopped") {
      return STOPPED;
    }
    return UNKNOWN;
  }
};

class MopidyClient {
  public:
  MopidyClient(const std::string &host, const std::string &port) :
    host{host}, port{port}
  {}

  void start_listening() {
    this->websocket_thread = std::thread(this->run, this->host, this->port);
  }

  void stop_listening() {
    this->websocket_thread.join();
  }

private:
  const std::string host;
  const std::string port;
  std::thread websocket_thread;

  static void run(std::string host, std::string port) {
    asio::io_context ioc;
    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { ioc.stop(); });

    auto spawned = asio::spawn(ioc, [&](asio::yield_context yield) {
      websocket_client(host, port, yield);
    });
    ioc.run();
  }

  static void websocket_client(const std::string& host, const std::string& port, asio::yield_context yield) {
    tcp::resolver resolver(yield.get_executor());
    websocket::stream<beast::tcp_stream> ws(yield.get_executor());

    auto const results = resolver.async_resolve(host, port, yield);
    beast::get_lowest_layer(ws).async_connect(results, yield);

    ws.async_handshake(host, "/mopidy/ws", yield);

    for (;;) {
      beast::flat_buffer buffer;
      ws.async_read(buffer, yield);
      std::cout << beast::make_printable(buffer.data()) << std::endl;
    }
  }
};
