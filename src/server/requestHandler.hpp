//@formatter:off

#pragma once

#include "sha1Builder.hpp"

#include <boost/noncopyable.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include <string>


namespace Server {

struct Reply;
struct Request;

/// The common handler for all incoming requests.
class RequestHandler
  : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit RequestHandler();

  /// Handle a request and produce a reply.
  void handleRequestGet(const Request& req, Reply& rep);
  
  void handleRequestPostBegin(const Request& req);
  
  void handleRequestPostSome(const Request& req);
  
  void handleRequestPostEnd(const Request& req, Reply& rep);

private:
  
  boost::shared_ptr<Crypto::Sha1Builder> m_sha1BuilderPtr;
  
  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool urlDecode(const std::string& in, std::string& out);
};

} // namespace server
