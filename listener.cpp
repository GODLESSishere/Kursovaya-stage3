#include <iostream>
#include <system_error>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "listener.h"
#include "server_error.h"

Listener::Listener(unsigned short port, int qlen):
    sock(socket(AF_INET, SOCK_STREAM, 0)),				
    self_addr(new sockaddr_in),							
    foreign_addr(new sockaddr_in),						
    queueLen(qlen)
{
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "Ошибка сокета");
    int on = 1;
    int rc = setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category(), "Ошибка сокета");
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = htons(port);
    self_addr->sin_addr.s_addr = 0;
    if (bind(sock,
             reinterpret_cast<const sockaddr*>(self_addr.get()),
             sizeof (sockaddr_in)) == -1)
        throw std::system_error(errno, std::generic_category(), "Ошибка соединения");
}

Listener::~Listener()
{
    close(sock);
}

void Listener::Run(Worker & worker)
{
    if (listen(sock, queueLen) == -1)
        throw std::system_error(errno, std::generic_category(), "Ошибка прослушивания");
    socklen_t socklen = sizeof (sockaddr_in);
    while(true) {
        int work_sock = accept(sock,
                               reinterpret_cast<sockaddr*>(foreign_addr.get()),
                               &socklen);
        if (work_sock == -1)
            throw std::system_error(errno, std::generic_category(), "Accept error");
        std::string ip_addr(inet_ntoa(foreign_addr->sin_addr));
        std::clog << "log: Connection established with " << ip_addr <<std::endl;
        try {
            worker(work_sock);	
        } catch (std::system_error &e) {
            std::cerr << e.what() << "\nСоединение с " << ip_addr << " прервано\n";
        } catch (auth_error &e) {
            std::cerr << e.what() << "\nСоединение с  " << ip_addr << " прервано\n";
            send(work_sock, "ERR", 3, 0);
        } catch (vector_error &e) {
            std::cerr << e.what() << "\nСоединение с " << ip_addr << " прервано\n";
        } catch (std::bad_alloc &e) {
            std::cerr << e.what() << ": Размер вектора слишком велик\nСоединение с " << ip_addr << " прервано\n";
        }
        close(work_sock);
        std::clog << "log: Соединение закрыто\n==============================\n";
    }
}
