class Server {

  public:
    Server(short port = 3000);
    
    void start();
  
  private:
    short m_port;
  
};