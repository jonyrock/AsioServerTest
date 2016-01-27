
#include "Reply.hpp"

#include <boost/lexical_cast.hpp>

#include <string>


namespace Server {
  
namespace StatusStrings {

const std::string ok =
  "HTTP/1.0 200 OK\r\n";
const std::string created =
  "HTTP/1.0 201 Created\r\n";
const std::string accepted =
  "HTTP/1.0 202 Accepted\r\n";
const std::string noContent =
  "HTTP/1.0 204 No Content\r\n";
const std::string multipleChoices =
  "HTTP/1.0 300 Multiple Choices\r\n";
const std::string movedPermanently =
  "HTTP/1.0 301 Moved Permanently\r\n";
const std::string movedTemporarily =
  "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string notModified =
  "HTTP/1.0 304 Not Modified\r\n";
const std::string badRequest =
  "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized =
  "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden =
  "HTTP/1.0 403 Forbidden\r\n";
const std::string notFound =
  "HTTP/1.0 404 Not Found\r\n";
const std::string internalServerError =
  "HTTP/1.0 500 Internal Server Error\r\n";
const std::string notImplemented =
  "HTTP/1.0 501 Not Implemented\r\n";
const std::string badGateway =
  "HTTP/1.0 502 Bad Gateway\r\n";
const std::string serviceUnavailable =
  "HTTP/1.0 503 Service Unavailable\r\n";

boost::asio::const_buffer toBuffer(Reply::StatusType status)
{
  switch (status)
  {
  case Reply::Ok:
    return boost::asio::buffer(ok);
  case Reply::Created:
    return boost::asio::buffer(created);
  case Reply::Accepted:
    return boost::asio::buffer(accepted);
  case Reply::NoContent:
    return boost::asio::buffer(noContent);
  case Reply::MultipleChoices:
    return boost::asio::buffer(multipleChoices);
  case Reply::MovedPermanently:
    return boost::asio::buffer(movedPermanently);
  case Reply::MovedTemporarily:
    return boost::asio::buffer(movedTemporarily);
  case Reply::NotModified:
    return boost::asio::buffer(notModified);
  case Reply::BadRequest:
    return boost::asio::buffer(badRequest);
  case Reply::Unauthorized:
    return boost::asio::buffer(unauthorized);
  case Reply::Forbidden:
    return boost::asio::buffer(forbidden);
  case Reply::NotFound:
    return boost::asio::buffer(notFound);
  case Reply::InternalServerError:
    return boost::asio::buffer(internalServerError);
  case Reply::NotImplemented:
    return boost::asio::buffer(notImplemented);
  case Reply::BadGateway:
    return boost::asio::buffer(badGateway);
  case Reply::ServiceUnavailable:
    return boost::asio::buffer(serviceUnavailable);
  default:
    return boost::asio::buffer(internalServerError);
  }
}

} // namespace StatusStrings

namespace MiscStrings {

const char nameValueSeparator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace MiscStrings

std::vector<boost::asio::const_buffer> Reply::toBuffers() {
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(StatusStrings::toBuffer(status));
  for (std::size_t i = 0; i < headers.size(); ++i) {
    header& h = headers[i];
    buffers.push_back(boost::asio::buffer(h.name));
    buffers.push_back(
      boost::asio::buffer(MiscStrings::nameValueSeparator)
    );
    buffers.push_back(boost::asio::buffer(h.value));
    buffers.push_back(boost::asio::buffer(MiscStrings::crlf));
  }
  buffers.push_back(boost::asio::buffer(MiscStrings::crlf));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

namespace StockReplies {

const char ok[] = "";
const char created[] =
  "<html>"
  "<head><title>Created</title></head>"
  "<body><h1>201 Created</h1></body>"
  "</html>";
const char accepted[] =
  "<html>"
  "<head><title>Accepted</title></head>"
  "<body><h1>202 Accepted</h1></body>"
  "</html>";
const char noContent[] =
  "<html>"
  "<head><title>No Content</title></head>"
  "<body><h1>204 Content</h1></body>"
  "</html>";
const char multipleChoices[] =
  "<html>"
  "<head><title>Multiple Choices</title></head>"
  "<body><h1>300 Multiple Choices</h1></body>"
  "</html>";
const char movedPermanently[] =
  "<html>"
  "<head><title>Moved Permanently</title></head>"
  "<body><h1>301 Moved Permanently</h1></body>"
  "</html>";
const char movedTemporarily[] =
  "<html>"
  "<head><title>Moved Temporarily</title></head>"
  "<body><h1>302 Moved Temporarily</h1></body>"
  "</html>";
const char notModified[] =
  "<html>"
  "<head><title>Not Modified</title></head>"
  "<body><h1>304 Not Modified</h1></body>"
  "</html>";
const char badRequest[] =
  "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>";
const char unauthorized[] =
  "<html>"
  "<head><title>Unauthorized</title></head>"
  "<body><h1>401 Unauthorized</h1></body>"
  "</html>";
const char forbidden[] =
  "<html>"
  "<head><title>Forbidden</title></head>"
  "<body><h1>403 Forbidden</h1></body>"
  "</html>";
const char notFound[] =
  "<html>"
  "<head><title>Not Found</title></head>"
  "<body><h1>404 Not Found</h1></body>"
  "</html>";
const char internalServerError[] =
  "<html>"
  "<head><title>Internal Server Error</title></head>"
  "<body><h1>500 Internal Server Error</h1></body>"
  "</html>";
const char notImplemented[] =
  "<html>"
  "<head><title>Not Implemented</title></head>"
  "<body><h1>501 Not Implemented</h1></body>"
  "</html>";
const char badGateway[] =
  "<html>"
  "<head><title>Bad Gateway</title></head>"
  "<body><h1>502 Bad Gateway</h1></body>"
  "</html>";
const char serviceUnavailable[] =
  "<html>"
  "<head><title>Service Unavailable</title></head>"
  "<body><h1>503 Service Unavailable</h1></body>"
  "</html>";

std::string toString(Reply::StatusType status) {
  switch (status) {
  case Reply::Ok:
    return ok;
  case Reply::Created:
    return created;
  case Reply::Accepted:
    return accepted;
  case Reply::NoContent:
    return noContent;
  case Reply::MultipleChoices:
    return multipleChoices;
  case Reply::MovedPermanently:
    return movedPermanently;
  case Reply::MovedTemporarily:
    return movedTemporarily;
  case Reply::NotModified:
    return notModified;
  case Reply::BadRequest:
    return badRequest;
  case Reply::Unauthorized:
    return unauthorized;
  case Reply::Forbidden:
    return forbidden;
  case Reply::NotFound:
    return notFound;
  case Reply::InternalServerError:
    return internalServerError;
  case Reply::NotImplemented:
    return notImplemented;
  case Reply::BadGateway:
    return badGateway;
  case Reply::ServiceUnavailable:
    return serviceUnavailable;
  default:
    return internalServerError;
  }
}

} // namespace StockReplies

Reply Reply::stockReply(Reply::StatusType status) {
  Reply rep;
  rep.status = status;
  rep.content = StockReplies::toString(status);
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(
                           rep.content.size()
                         );
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
  return rep;
}

} // namespace Server
