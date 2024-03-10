#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <zmq.hpp>

const int MAIN_PORT = 4040;

inline void send_message(zmq::socket_t &socket, const std::string &string_message) {
  zmq::message_t message(string_message.size());
  memcpy(message.data(), string_message.c_str(), string_message.size());
  socket.send(message);
}

inline std::string receive_message(zmq::socket_t &socket) {
  zmq::message_t message;
  int chars_number;

  try {
    chars_number = (int)socket.recv(&message);
  } catch (...) {
    chars_number = 0;
  }

  if (chars_number == 0) {
    throw -1;
  }

  std::string received_message(static_cast<char *>(message.data()), message.size());
  return received_message;
}

inline void connect(zmq::socket_t &socket, int port) {
  std::string address = "tcp://127.0.0.1:" + std::to_string(port);
  socket.connect(address);
}

inline void disconnect(zmq::socket_t &socket, int port) {
  std::string address = "tcp://127.0.0.1:" + std::to_string(port);
  socket.disconnect(address);
}

inline int bind(zmq::socket_t &socket, int id) {
  int port = MAIN_PORT + id;
  std::string address = "tcp://127.0.0.1:" + std::to_string(port);

  while (1) {
    try {
      socket.bind(address);
      break;
    } catch (...) {
      ++port;
    }
  }

  return port;
}

inline void unbind(zmq::socket_t &socket, int port) {
  std::string address = "tcp://127.0.0.1:" + std::to_string(port);
  socket.unbind(address);
}
