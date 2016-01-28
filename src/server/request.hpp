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
  std::vector<Header> headers;
  
  bool headerIsReady = false;
  
  std::string boundary;
  // correspond to ConnectionManager::m_buffer
  char postChunk[8192];
  size_t postChunkSize = 0;
};

} // namespace Server
