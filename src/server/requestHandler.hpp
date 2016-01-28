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
  explicit RequestHandler(const std::string& docRoot);

  /// Handle a request and produce a reply.
  void handleGetRequest(const Request &req, Reply &rep);

  void handlePostSomeRequest(
    const boost::array<char, 8192>& buffer,
    std::size_t bytesTransferred
  );

  void handlePostEndRequest(Reply &rep);

private:

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool urlDecode(const std::string& in, std::string& out);
};

} // namespace server
