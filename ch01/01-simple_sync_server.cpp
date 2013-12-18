#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock);

int main()
{
    io_service service;
    ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
    ip::tcp::acceptor acc(service, ep);
    while (true) {
        socket_ptr sock(
            new ip::tcp::socket(service)
        );
        acc.accept(*sock);
        boost::thread(
            boost::bind(client_session, sock)
        );
    }
    return 0;       
}

void client_session(socket_ptr sock) 
{
    while (true) {
        char data[512];
        boost::system::error_code error;
        size_t len = sock->read_some(buffer(data), error);
        if (error == error::eof)
           return;
        if (len > 0)
            write(*sock, buffer("ok", 2));
    } 
}
