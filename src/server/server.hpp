#pragma once 

namespace Server {

class Server {  
  public:
    static Server& getInstance(short port = 3000);
    virtual void run() = 0;
};

} // namespace Server