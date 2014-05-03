
#ifndef H_DER_UDPSERVER_H
#define H_DER_UDPSERVER_H

#include <winsock2.h>

namespace der
{

    class UDPServer
    {
    public:
        UDPServer(unsigned int port, unsigned int buffer_size)
            : m_socket(0)
            , m_port(port)
            , m_buffer_size(buffer_size)
            , m_initialized(false)
            , m_need_cleanup(false)
        { }

        ~UDPServer();

        /// Initializes the server.
        /// \return False if the initialization failed.
        bool initialize();

        /// Receives UDP packets and calls message_received.
        void pump();

        virtual void message_received(const char* buffer) { }

    protected:
        SOCKET m_socket;
        const unsigned int m_port;
        const unsigned int m_buffer_size;

    private:
        bool m_initialized;
        bool m_need_cleanup;

    };

} // der

#endif // H_DER_UDPSERVER_H
