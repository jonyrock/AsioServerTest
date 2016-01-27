#pragma once

#include "header.hpp"

#include <string>
#include <vector>


namespace Server {

/// A request received from a client.
struct Request {
  std::string method;
  std::string uri;
  int httpVersionMajor;
  int httpVersionMinor;
  std::vector<header> headers;
};

} // namespace Server
