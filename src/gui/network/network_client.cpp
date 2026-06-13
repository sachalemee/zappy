/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** network_client
*/

#include "network/network_client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

network_client::~network_client()
{
    disconnect();
}

bool network_client::openSocket(const addrinfo &info)
{
    int flags = 0;

    _fd = socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (_fd < 0)
        return false;
    if (connect(_fd, info.ai_addr, info.ai_addrlen) < 0) {
        disconnect();
        return false;
    }
    flags = fcntl(_fd, F_GETFL, 0);
    fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
    return true;
}

bool network_client::connectTo(const std::string &host, int port)
{
    struct addrinfo hints = {};
    struct addrinfo *res = nullptr;
    bool connected = false;

    disconnect();
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(),
        &hints, &res) != 0)
        return false;
    connected = openSocket(*res);
    freeaddrinfo(res);
    return connected;
}

void network_client::disconnect()
{
    if (_fd >= 0)
        close(_fd);
    _fd = -1;
    _buffer.clear();
}

bool network_client::sendLine(const std::string &msg)
{
    ssize_t total = 0;
    ssize_t len = static_cast<ssize_t>(msg.size());
    ssize_t sent = 0;

    if (_fd < 0)
        return false;
    while (total < len) {
        sent = send(_fd, msg.c_str() + total, len - total, 0);
        if (sent <= 0)
            return false;
        total += sent;
    }
    return true;
}

std::vector<std::string> network_client::extractLines()
{
    std::vector<std::string> lines;
    std::size_t pos = _buffer.find('\n');

    while (pos != std::string::npos) {
        std::string line = _buffer.substr(0, pos);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        lines.push_back(line);
        _buffer.erase(0, pos + 1);
        pos = _buffer.find('\n');
    }
    return lines;
}

std::vector<std::string> network_client::poll()
{
    char tmp[4096] = {};
    ssize_t n = 0;

    if (_fd < 0)
        return {};
    n = recv(_fd, tmp, sizeof(tmp), 0);
    while (n > 0) {
        _buffer.append(tmp, static_cast<size_t>(n));
        n = recv(_fd, tmp, sizeof(tmp), 0);
    }
    if (n == 0)
        disconnect();
    return extractLines();
}
