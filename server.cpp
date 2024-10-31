#include <iostream>
#include <system_error>
#include "listener.h"
#include "worker.h"
#include "server_error.h"
#include "interface.h"

int main(int argc, char **argv)
{
    try {
        Variant v(argc, argv);
        Worker w(v.getType(), v.getHash(), v.getSide());
        Listener server;
        server.Run(w);
    } catch (std::system_error &e) {
        std::cerr << e.what() << std::endl;
        std::quick_exit(2);
    } catch (option_error &e) {
        std::cerr << e.what() << std::endl;
        Variant::print_help_and_exit(1);
    }
    return 0;
}
