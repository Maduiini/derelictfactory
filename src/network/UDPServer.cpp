
#include "UDPServer.h"
#include "../Log.h"

#include <windows.h>

namespace der
{

    bool UDPServer::initialize()
    {
        WSADATA wsa_data;
        WORD version = MAKEWORD(2, 0);

        // Initialize WinSock
        if (::WSAStartup(version, &wsa_data) != 0)
        {
            log::error("Failed to initialize WinSock. Error code: %", ::WSAGetLastError());
            return false;
        }

        m_need_cleanup = true;

        // Check WinSock version
        if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 0)
        {
            log::error("Incorrect WinSock version.");
            return false;
        }

        // Create socket
        m_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

        // Start server
        struct sockaddr_in local_addr;
        char hostname[100];
        ::gethostname(hostname, 100);
        hostent* localhost = ::gethostbyname(hostname);
        const char* local_ip = ::inet_ntoa(*(in_addr*)*localhost->h_addr_list);
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = ::inet_addr(local_ip);
        local_addr.sin_port = ::htons(m_port);

        log::debug("Binding server to %:%.", local_ip, m_port);

        if (::bind(m_socket, (struct sockaddr*)&local_addr, sizeof(local_addr)) == SOCKET_ERROR)
        {
            log::error("Failed to bind socket to port %. Error code: %", m_port, ::WSAGetLastError());
            return false;
        }

        m_initialized = true;
        return true;
    }

    UDPServer::~UDPServer()
    {
        if (m_need_cleanup)
        {
            ::WSACleanup();
            if (m_socket)
            {
                log::debug("Closing socket.");
                ::closesocket(m_socket);
            }
        }
    }

    void UDPServer::pump()
    {
        if (!m_initialized) return;

        struct timeval timev;
        timev.tv_sec = 0;
        timev.tv_usec = 5;

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(m_socket, &fds);

        int n = ::select(m_socket, &fds, NULL, NULL, &timev);

        if (n == 0)
        {
            // Select timeout
            return;
        }
        else if (n == -1)
        {
            log::error("UDPServer: Select error.");
            return;
        }

        struct sockaddr_in remote_addr;
        int length = sizeof(remote_addr);
        char buffer[m_buffer_size];

        int received = ::recvfrom(m_socket, buffer, m_buffer_size, 0, (SOCKADDR*)&remote_addr, &length);
        buffer[received] = '\0'; // null-terminate buffer
        message_received(buffer);
    }

} // der
