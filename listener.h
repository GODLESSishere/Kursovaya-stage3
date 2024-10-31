#pragma once
#include <memory>
#include <netinet/in.h>
#include "worker.h"

static constexpr int default_port = 33333;
static constexpr int default_qlen = 10;

class Listener
{
private:
    int sock;
    std::unique_ptr<sockaddr_in> self_addr;
    std::unique_ptr<sockaddr_in> foreign_addr;
    int queueLen;

public:
    Listener(unsigned short port=default_port, int qlen=default_qlen);
    ~Listener();
    void Run(Worker & worker);
};
