#include "mimeTypes.hpp"

namespace Server {
namespace MimeTypes {

struct mapping
{
  const char* extension;
  const char* mimeType;
} mappings[] = {
  { "gif", "image/gif" },
  { "htm", "text/html" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "png", "image/png" },
  { 0, 0 } // Marks end of list.
};

std::string extensionToType(const std::string& extension) {
  for (mapping* m = mappings; m->extension; ++m)
  {
    if (m->extension == extension)
    {
      return m->mimeType;
    }
  }
  return "text/plain";
}

} // namespace mimeTypes
} // namespace server
