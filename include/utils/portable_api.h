
#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifndef __WINDOWS__
        #define __WINDOWS__
    #endif
#elif defined(__linux__)
    #ifndef __LINUX__
        #define __LINUX__
    #endif
#elif defined(__APPLE__)
    
#endif

#if defined(__WINDOWS__)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #include <mswsock.h>
    #pragma comment(lib, "ws2_32.lib")

    using socklen_t = int;
    using sa_family_t = ADDRESS_FAMILY;
#else
    
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <poll.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <netdb.h>
    #if defined(__LINUX__)
        #include <ifaddrs.h>
        #include <net/if.h>
    #endif

    using SOCKET = int;
    static constexpr SOCKET INVALID_SOCKET = -1;
    static constexpr int SOCKET_ERROR = -1;
    inline int closesocket(SOCKET s) { return ::close(s); }
    inline int ioctlsocket(SOCKET s, long cmd, unsigned long* arg) {
        return ::ioctl(s, cmd, arg);
    }
    
    inline int WSAGetLastError() { return errno; }
    static constexpr int WSAEWOULDBLOCK = EWOULDBLOCK;
    static constexpr int WSAECONNRESET  = ECONNRESET;
    static constexpr int FIONREAD       = 0x541B; 
#endif

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cassert>
#include <cstring>

namespace utils {
    namespace Endian {
        template<typename T>
        inline T net_swap(T value) {
            if constexpr (sizeof(T) == 2) return static_cast<T>(ntohs(static_cast<uint16_t>(value)));
            if constexpr (sizeof(T) == 4) return static_cast<T>(ntohl(static_cast<uint32_t>(value)));
            return value;
        }
    }

    template<size_t N>
    constexpr size_t static_strlen(const char(&)[N]) { return N - 1; }
}

namespace Socket {
    inline void InitSocket() {
#if defined(__WINDOWS__)
        WSADATA wsa{};
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    }
    inline void CleanupSocket() {
#if defined(__WINDOWS__)
        WSACleanup();
#endif
    }

    enum class poll_flags : short {
        none   = 0,
        in     = POLLIN,
        out    = POLLOUT,
        hup    = POLLHUP,
        in_hup = POLLIN | POLLHUP,
    };
    inline poll_flags operator|(poll_flags a, poll_flags b) {
        return static_cast<poll_flags>(static_cast<short>(a) | static_cast<short>(b));
    }
    inline poll_flags operator&(poll_flags a, poll_flags b) {
        return static_cast<poll_flags>(static_cast<short>(a) & static_cast<short>(b));
    }
    inline bool operator!=(poll_flags a, poll_flags b) {
        return static_cast<short>(a) != static_cast<short>(b);
    }

    enum class level       { sol_socket = SOL_SOCKET };
    enum class option_name { so_broadcast = SO_BROADCAST };
    enum class cmd_name    { fionread = 0 }; 
}

// iface_ip_t and get_ifaces_ip moved to os_funcs.h/cpp

namespace PortableAPI {

class socket_exception : public std::runtime_error {
public:
    explicit socket_exception(const std::string& msg) : std::runtime_error(msg) {}
};
class is_connected    : public socket_exception { public: is_connected()    : socket_exception("already connected") {} };
class not_connected   : public socket_exception { public: not_connected()   : socket_exception("not connected") {} };
class would_block     : public socket_exception { public: would_block()     : socket_exception("would block") {} };

struct ipv4m_addr {
    sockaddr_in addr{};

    ipv4m_addr() { addr.sin_family = AF_INET; }
    ipv4m_addr(const std::string& ip, uint16_t port) {
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    }
    ipv4m_addr(uint32_t ip, uint16_t port) {
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        addr.sin_addr.s_addr = htonl(ip);
    }
    uint16_t port() const { return ntohs(addr.sin_port); }
    std::string ip() const {
        char buf[INET_ADDRSTRLEN]{};
        inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
        return buf;
    }
    bool operator==(const ipv4m_addr& o) const {
        return addr.sin_addr.s_addr == o.addr.sin_addr.s_addr
            && addr.sin_port        == o.addr.sin_port;
    }
    bool operator<(const ipv4m_addr& o) const {
        if (addr.sin_addr.s_addr != o.addr.sin_addr.s_addr)
            return addr.sin_addr.s_addr < o.addr.sin_addr.s_addr;
        return addr.sin_port < o.addr.sin_port;
    }
};

inline void set_nonblocking(SOCKET s) {
#if defined(__WINDOWS__)
    u_long mode = 1;
    ioctlsocket(s, FIONBIO, &mode);
#else
    int flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);
#endif
}

inline bool Poll(SOCKET s, Socket::poll_flags flags, int timeout_ms) {
    pollfd pfd{};
    pfd.fd      = s;
    pfd.events  = static_cast<short>(flags);
    pfd.revents = 0;
#if defined(__WINDOWS__)
    int r = WSAPoll(&pfd, 1, timeout_ms);
#else
    int r = ::poll(&pfd, 1, timeout_ms);
#endif
    return r > 0 && (pfd.revents & static_cast<short>(flags)) != 0;
}

class udp_socket {
    SOCKET m_sock{INVALID_SOCKET};
public:
    udp_socket() {}
    ~udp_socket() { close(); }
    udp_socket(const udp_socket&) = delete;
    udp_socket& operator=(const udp_socket&) = delete;
    udp_socket(udp_socket&& o) noexcept : m_sock(o.m_sock) { o.m_sock = INVALID_SOCKET; }
    udp_socket& operator=(udp_socket&& o) noexcept { close(); m_sock = o.m_sock; o.m_sock = INVALID_SOCKET; return *this; }

    bool is_valid() const { return m_sock != INVALID_SOCKET; }

    void open() {
        m_sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_sock == INVALID_SOCKET) throw socket_exception("Failed to create UDP socket");
        set_nonblocking(m_sock);
        int yes = 1;
        setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&yes), sizeof(yes));
        setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes));
    }

    void bind(const ipv4m_addr& addr) {
        if (::bind(m_sock, reinterpret_cast<const sockaddr*>(&addr.addr), sizeof(addr.addr)) == SOCKET_ERROR)
            throw socket_exception("UDP bind failed");
    }

    void bind(uint16_t port) {
        ipv4m_addr a("0.0.0.0", port);
        bind(a);
    }

    void close() {
        if (m_sock != INVALID_SOCKET) { closesocket(m_sock); m_sock = INVALID_SOCKET; }
    }

    int sendto(const void* data, size_t len, const ipv4m_addr& dest) {
        return static_cast<int>(::sendto(m_sock,
            reinterpret_cast<const char*>(data), static_cast<int>(len), 0,
            reinterpret_cast<const sockaddr*>(&dest.addr), sizeof(dest.addr)));
    }

    int recvfrom(void* buf, size_t len, ipv4m_addr& src) {
        socklen_t sl = sizeof(src.addr);
        return static_cast<int>(::recvfrom(m_sock,
            reinterpret_cast<char*>(buf), static_cast<int>(len), 0,
            reinterpret_cast<sockaddr*>(&src.addr), &sl));
    }

    bool has_data(int timeout_ms = 0) const {
        return Poll(m_sock, Socket::poll_flags::in, timeout_ms);
    }

    uint16_t local_port() const {
        sockaddr_in la{};
        socklen_t sl = sizeof(la);
        getsockname(m_sock, reinterpret_cast<sockaddr*>(&la), &sl);
        return ntohs(la.sin_port);
    }

    SOCKET native() const { return m_sock; }
};

class tcp_socket {
    SOCKET m_sock{INVALID_SOCKET};
    bool   m_connected{false};
public:
    tcp_socket() {}
    ~tcp_socket() { close(); }
    tcp_socket(const tcp_socket&) = delete;
    tcp_socket& operator=(const tcp_socket&) = delete;
    tcp_socket(tcp_socket&& o) noexcept : m_sock(o.m_sock), m_connected(o.m_connected) {
        o.m_sock = INVALID_SOCKET; o.m_connected = false;
    }
    tcp_socket& operator=(tcp_socket&& o) noexcept {
        close(); m_sock = o.m_sock; m_connected = o.m_connected;
        o.m_sock = INVALID_SOCKET; o.m_connected = false;
        return *this;
    }

    bool is_valid()    const { return m_sock != INVALID_SOCKET; }
    bool is_connected()const { return m_connected; }

    void open() {
        m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_sock == INVALID_SOCKET) throw socket_exception("Failed to create TCP socket");
        set_nonblocking(m_sock);
        int yes = 1;
        setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes));
    }

    void bind(uint16_t port) {
        ipv4m_addr a("0.0.0.0", port);
        if (::bind(m_sock, reinterpret_cast<const sockaddr*>(&a.addr), sizeof(a.addr)) == SOCKET_ERROR)
            throw socket_exception("TCP bind failed");
    }

    void listen(int backlog = 16) {
        if (::listen(m_sock, backlog) == SOCKET_ERROR) throw socket_exception("TCP listen failed");
    }

    tcp_socket accept() {
        tcp_socket client;
        sockaddr_in ca{};
        socklen_t sl = sizeof(ca);
        SOCKET cs = ::accept(m_sock, reinterpret_cast<sockaddr*>(&ca), &sl);
        if (cs != INVALID_SOCKET) {
            client.m_sock = cs;
            client.m_connected = true;
            set_nonblocking(cs);
        }
        return client;
    }

    bool connect(const ipv4m_addr& addr) {
        int r = ::connect(m_sock, reinterpret_cast<const sockaddr*>(&addr.addr), sizeof(addr.addr));
#if defined(__WINDOWS__)
        if (r == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
#else
        if (r == SOCKET_ERROR && (errno == EINPROGRESS || errno == EWOULDBLOCK)) {
#endif
            if (Poll(m_sock, Socket::poll_flags::out, 2000)) {
                int err = 0; socklen_t el = sizeof(err);
                getsockopt(m_sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &el);
                m_connected = (err == 0);
            }
        } else {
            m_connected = (r == 0);
        }
        return m_connected;
    }

    int send(const void* data, size_t len) {
        return static_cast<int>(::send(m_sock, reinterpret_cast<const char*>(data), static_cast<int>(len), 0));
    }

    int recv(void* buf, size_t len) {
        return static_cast<int>(::recv(m_sock, reinterpret_cast<char*>(buf), static_cast<int>(len), 0));
    }

    void close() {
        if (m_sock != INVALID_SOCKET) { closesocket(m_sock); m_sock = INVALID_SOCKET; m_connected = false; }
    }

    bool has_data(int timeout_ms = 0) const {
        return Poll(m_sock, Socket::poll_flags::in, timeout_ms);
    }

    bool can_write(int timeout_ms = 0) const {
        return Poll(m_sock, Socket::poll_flags::out, timeout_ms);
    }

    uint16_t local_port() const {
        sockaddr_in la{};
        socklen_t sl = sizeof(la);
        getsockname(m_sock, reinterpret_cast<sockaddr*>(&la), &sl);
        return ntohs(la.sin_port);
    }

    SOCKET native() const { return m_sock; }
};

} 
