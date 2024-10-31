#pragma once
#include <stdexcept>
#include <string>

class auth_error: public std::runtime_error {
public:
    auth_error(const std::string& s) : std::runtime_error(s) {}
    auth_error(const char * s) : std::runtime_error(s) {}
};

class vector_error: public std::runtime_error {
public:
    vector_error(const std::string& s) : std::runtime_error(s) {}
    vector_error(const char * s) : std::runtime_error(s) {}
};

class option_error: public std::runtime_error {
public:
    option_error(const std::string& s) : std::runtime_error(s) {}
    option_error(const char * s) : std::runtime_error(s) {}
};
