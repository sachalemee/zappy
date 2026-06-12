/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** network_client
*/

#ifndef NETWORK_CLIENT_HPP_
#define NETWORK_CLIENT_HPP_

#include <string>
#include <vector>

struct addrinfo;

class network_client {
public:
    network_client() = default;
    ~network_client();

    bool connectTo(const std::string &host, int port);
    void disconnect();
    bool sendLine(const std::string &msg);
    std::vector<std::string> poll();
    bool isConnected() const { return _fd >= 0; }

private:
    bool openSocket(const addrinfo &info);
    std::vector<std::string> extractLines();

    int _fd = -1;
    std::string _buffer;
};

#endif /* NETWORK_CLIENT_HPP_ */