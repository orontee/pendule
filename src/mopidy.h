#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

enum PlaybackState { UNKNOWN, PLAYING, PAUSED, STOPPED };

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

typedef std::function<void(PlaybackStateChange)> PlaybackStateChangeHandler;

class MopidyClient {
public:
  MopidyClient(const std::string &host, const std::string &port)
      : host{host}, port{port} {}

  void start_listening() {
    this->websocket_thread =
        std::thread(this->run, this->host, this->port, this->handler);
  }

  void stop_listening() { this->websocket_thread.join(); }

  void set_playback_state_change_handler(PlaybackStateChangeHandler handler) {
    this->handler = handler;
  }

private:
  const std::string host;
  const std::string port;
  std::thread websocket_thread;
  PlaybackStateChangeHandler handler;

  static void run(std::string host, std::string port,
                  PlaybackStateChangeHandler handler) {
    asio::io_context ioc;
    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { ioc.stop(); });

    asio::spawn(ioc, [&](asio::yield_context yield) {
      websocket_client(host, port, yield, handler);
    });
    ioc.run();
  }

  static void websocket_client(const std::string &host, const std::string &port,
                               asio::yield_context yield,
                               PlaybackStateChangeHandler handler) {
    if (not handler) {
      return;
    }
    tcp::resolver resolver(yield.get_executor());
    websocket::stream<beast::tcp_stream> ws(yield.get_executor());

    auto const results = resolver.async_resolve(host, port, yield);
    beast::get_lowest_layer(ws).async_connect(results, yield);

    ws.async_handshake(host, "/mopidy/ws", yield);

    for (;;) {
      beast::flat_buffer buffer;
      ws.async_read(buffer, yield);

      std::stringstream payload;
      payload << beast::make_printable(buffer.data());

      boost::json::error_code error_code;
      boost::json::value jv = boost::json::parse(payload, error_code);
      if (error_code) {
        std::cerr << "Failed to parse payload: " << error_code.message() << '\n';
        return;
      }
      const std::string event{jv.at("event").as_string()};
      if (event == "playback_state_changed") {
        const std::string raw_state{jv.at("new_state").as_string()};
        PlaybackStateChange state_change;
        state_change.new_state = PlaybackStateChange::from_string(raw_state);

        handler(state_change);
      } else {
        std::cout << jv;
      }
    }
  }
};
