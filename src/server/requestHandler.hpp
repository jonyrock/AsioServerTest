//@formatter:off

#pragma once


#include <boost/noncopyable.hpp>
#include <boost/array.hpp>

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

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool urlDecode(const std::string& in, std::string& out);
};

} // namespace server
