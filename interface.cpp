#include <algorithm>
#include "interface.h"

Variant::Variant(int argc, char** argv)
{
    int opt;
    while( (opt = getopt(argc, argv, "T:H:S:h")) != -1) {
        switch (opt) {
        case 'T' :
            type = optarg;
            break;
        case 'H' :
            hash = optarg;
            break;
        case 'S' :
            side = optarg;
            break;
        case 'h' :
            print_help_and_exit(0);
        case '?' :
            throw option_error("");
        case ':' :
            throw option_error("Ошибка параметра: пропущено значение");
        }
    }

    // Set default values if not provided
    if (type.empty()) {
        type = "int64_t";
    }
    if (hash.empty()) {
        hash = "SHA1";
    }
    if (side.empty()) {
        side = "server";
    }

    // Validate the values
    if (std::find(Types.cbegin(), Types.cend(), type) == Types.cend())
        throw option_error("Операционная ошибка: неизвестный тип данных `" + type + '\'');
    if (std::find(Hashes.cbegin(), Hashes.cend(), hash) == Hashes.cend())
        throw option_error("Операционная ошибка: неизвестный тип хэша `" + hash + '\'');
    if (std::find(Sides.cbegin(), Sides.cend(), side) == Sides.cend())
        throw option_error("Операционная ошибка: неизвестна сторона создания соли `" + side + '\'');
}

void Variant::print_help_and_exit(int exit_code)
{
    std::cerr << "Usage: server -T data_type -H hash_type -S salt_side \n";
    std::cerr << "where\tdata_type is:\n";
    for (auto t: Types) {
        std::cerr << "\t\t" << t << std::endl;
    }
    std::cerr << "\thash_type is:\n";
    for (auto h: Hashes) {
        std::cerr << "\t\t" << h << std::endl;
    }
    std::cerr << "\tsalt_side is:\n";
    for (auto s: Sides) {
        std::cerr << "\t\t" << s << std::endl;
    }
    std::quick_exit(exit_code);
}
