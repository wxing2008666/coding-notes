/*
 * rfc6455 : http://tools.ietf.org/rfc/rfc6455.txt
 */

/*
        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-------+-+-------------+-------------------------------+
        |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
        |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
        |N|V|V|V|       |S|             |   (if payload len==126/127)   |
        | |1|2|3|       |K|             |                               |
        +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
        |     Extended payload length continued, if payload len == 127  |
        + - - - - - - - - - - - - - - - +-------------------------------+
        |                               |Masking-key, if MASK set to 1  |
        +-------------------------------+-------------------------------+
        | Masking-key (continued)       |          Payload Data         |
        +-------------------------------- - - - - - - - - - - - - - - - +
        :                     Payload Data continued ...                :
        + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
        |                     Payload Data continued ...                |
        +---------------------------------------------------------------+
        opcode:
                *  %x0 denotes a continuation frame
                *  %x1 denotes a text frame
                *  %x2 denotes a binary frame
                *  %x3-7 are reserved for further non-control frames
                *  %x8 denotes a connection close
                *  %x9 denotes a ping
                *  %xA denotes a pong
                *  %xB-F are reserved for further control frames
        Payload length:  7 bits, 7+16 bits, or 7+64 bits
        Masking-key:  0 or 4 bytes
 */

/////////////////////////////////////////////////////////////////
#ifndef __WEBSOCKET_SERVER_H__
#define __WEBSOCKET_SERVER_H__

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "sha1.h"
#include "base64.h"

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#define ops_bswap_64(val) (((val) >> 56) |\
                    (((val) & 0x00ff000000000000ll) >> 40) |\
                    (((val) & 0x0000ff0000000000ll) >> 24) |\
                    (((val) & 0x000000ff00000000ll) >> 8)  |\
                    (((val) & 0x00000000ff000000ll) << 8)  |\
                    (((val) & 0x0000000000ff0000ll) << 24) |\
                    (((val) & 0x000000000000ff00ll) << 40) |\
                    (((val) << 56)))

#if __BYTE_ORDER == __LITTLE_ENDIAN
    #define ws_hton64(ll) (ops_bswap_64(ll))
    #define ws_ntoh64(ll) (ops_bswap_64(ll))
#else
    #define ws_hton64(ll) (ll)
    #define ws_ntoh64(ll) (ll)
#endif

namespace WebSocketCpp {
    // WebSocketServer
    class WebSocketServer {
    public:
        WebSocketServer() {}
        ~WebSocketServer() {
            for (auto &th : heartbeat_threads_) {
                th.join();
            }
        }

        enum WEBSOCKET_STATUS {
            WEBSOCKET_CONNECTED    = 0,
            WEBSOCKET_DISCONNECTED = 1,
        };
    
    public:
        void init(const std::string ip, unsigned short port) {
            //
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(sockaddr_in));
            server_addr.sin_family      = AF_INET;
            server_addr.sin_port        = htons(port);
            //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            inet_pton(AF_INET, ip.c_str(), &(server_addr.sin_addr));
            //
            listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
            if (listen_fd_ < 0) {
                std::cout << "server create socket error" << std::endl;
                return;
            }
            if (bind(listen_fd_, (struct sockaddr*)(&server_addr), sizeof(server_addr)) < 0) {
                std::cout << "server bind addr failed" << std::endl;
                return;
            }
            if (listen(listen_fd_, 5) < 0) {
                std::cout << "server listen failed" << std::endl;
                return;
            }
            //
            set_nonblock(listen_fd_);
            //
            epoll_fd_ = epoll_create(1024);
            epoll_event_ctl(listen_fd_, true);
            std::cout << "websocket server started!!!" << std::endl << std::endl;
        }

        void run() {
            while (true) {
                //
                struct epoll_event events[1024];
                int fds = epoll_wait(epoll_fd_, events, 1024, 10);
                for (int i = 0; i < fds; i++) {
                    if (events[i].data.fd == listen_fd_) {
                        std::cout << "new connection" << std::endl << std::endl;
                        // new connection
                        struct sockaddr_in client_addr;
                        socklen_t clilen = sizeof(client_addr);
                        //
                        while (1) {
                            int client = accept(listen_fd_, (struct sockaddr*)(&client_addr), &clilen);
                            if (client == -1) {
                                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                                    continue;
                                } else {
                                    std::cout << "accept client error:" << errno << std::endl;
                                    break;
                                }
                            }
                            set_nonblock(client);
                            epoll_event_ctl(client, true);
                            conn_status_[client] = WEBSOCKET_DISCONNECTED;
                            break;
                        }
                        //
                        //int client = accept(listen_fd_, (struct sockaddr*)(&client_addr), &clilen);
                        //
                        //set_nonblock(client);
                        //epoll_event_ctl(client, true);
                        //conn_status_[client] = WEBSOCKET_DISCONNECTED;
                    } else if (events[i].events & EPOLLIN) {
                        auto cli = conn_status_.find(events[i].data.fd);
                        if (cli != conn_status_.end() && WEBSOCKET_DISCONNECTED == cli->second) {
                            // handshark
                            char buf[1024];
                            if (read(events[i].data.fd, buf, sizeof(buf)) > 0) {
                                std::cout << "get request:" << std::endl << std::endl;
                                std::cout << buf << std::endl;
                                //
                                handshark(events[i].data.fd, buf);
                            }
                        } else {
                            // echo
                            char rev_data[655360];
                            memset(rev_data, 0, sizeof(rev_data));
                            std::string receive_data;
                            //
                            do {
                                int nread = read(events[i].data.fd, rev_data, sizeof(receive_data));
                                if (nread < 0) {
                                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                        break;
                                    } else if (errno == EINTR) {
                                        continue;
                                    } else {
                                        close(events[i].data.fd);
                                        epoll_event_ctl(events[i].data.fd, false);
                                        break;
                                    }
                                } else if (0 == nread) {
                                    close(events[i].data.fd);
                                    epoll_event_ctl(events[i].data.fd, false);
                                    break;
                                } else {
                                    receive_data.append(rev_data, nread);
                                    memset(rev_data, 0, sizeof(rev_data));
                                }
                            } while (1);
                            //
                            /**
                            while ((n = read(events[i].data.fd, rev_data, sizeof(rev_data))) > 0) {
                                receive_data.append(rev_data, n);
                                memset(&rev_data, 0, sizeof(rev_data));
                            }*/
                            std::cout << "receive raw data len:" << receive_data.length() << std::endl;
                            //
                            auto iter = map_cli_msg_.find(events[i].data.fd);
                            if (iter == map_cli_msg_.end()) {
                                map_cli_msg_[events[i].data.fd] = receive_data;
                            } else {
                                std::string &cli_msg = map_cli_msg_[events[i].data.fd];
                                cli_msg += receive_data;
                            }
                            std::string cli_msg;
                            msg_unpack(events[i].data.fd, cli_msg);
                            //
                            if (!cli_msg.empty()) {
                                std::cout << "echo msg:" << cli_msg << std::endl << std::endl;
                                std::string send_msg = msg_pack(cli_msg);
                                int n = 0, sent = 0;
                                while (n = write(events[i].data.fd, send_msg.c_str(), send_msg.size())) {
                                    sent += n;
                                    if (sent < send_msg.size()) {
                                        std::string left_msg;
                                        left_msg.assign(send_msg.c_str() + n);
                                        send_msg = left_msg;
                                    } else {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    private:
        int set_nonblock(int fd) {
            int flags;
            if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
                flags = 0;
            return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }

        void epoll_event_ctl(int fd, bool flag) {
            struct epoll_event ev;
            ev.data.fd = fd;
            ev.events  = flag ? EPOLLIN : 0;
            epoll_ctl(epoll_fd_, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
        }

        int handshark(int fd, char buf[]) {
            std::istringstream req;
            req.str(buf);
            std::map<std::string, std::string> req_headers;
            for (std::string line; std::getline(req, line);) {
                size_t pos = line.find(": ", 0);
                if (pos != std::string::npos) {
                    if (line[line.size() - 1] == '\r') {
                        line.erase(line.end() - 1);
                    }
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 2);
                    req_headers[key] = value;
                }
            }
            // handshark response
            char response[1024] = {0};
            strcat(response, "HTTP/1.1 101 Switching Protocols\r\n");
            strcat(response, "Connection: upgrade\r\n");
            strcat(response, "Sec-WebSocket-Accept: ");

            std::string server_key = req_headers["Sec-WebSocket-Key"];
            server_key += MAGIC_KEY;

            SHA1 sha;
            unsigned int message_digest[5];
            sha.Reset();
            sha << server_key.c_str();
            sha.Result(message_digest);
            for (int i = 0; i < 5; i++) {
                message_digest[i] = htonl(message_digest[i]);
            }
            server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
            server_key += "\r\n";
            //
            strcat(response, server_key.c_str());
            strcat(response, "Upgrade: websocket\r\n\r\n");
            if (write(fd, response, strlen(response)) < 0) {
                std::cout << "handshark failed" << std::endl;
            } else {
                std::cout << "handshark response:" << std::endl << std::endl << response << std::endl;
                std::cout << "handshark successed" << std::endl << std::endl;
                conn_status_[fd] = WEBSOCKET_CONNECTED;
                //
                //heartbeat_threads_.emplace_back(std::bind(&WebSocketServer::heartbeat, this, std::ref(fd)));
            }
            return 0;
        }

        void heartbeat(int fd) {
            //
            std::string msg = "hello";
            std::string ping_msg = msg_pack(msg, 0x9);
            if (write(fd, ping_msg.c_str(), ping_msg.size()) < 0) {
                std::cout << "ping failed" << std::endl;
            } else {
                std::cout << "ping -> client" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::string msg_pack(std::string &msg_body, int opcode = 0x01) {
            //
            std::string packed_msg = "  ";
            packed_msg[0] = 0;
            packed_msg[1] = 0;
            packed_msg[0] |= 0x80;
            packed_msg[0] |= (char)opcode;
            //
            int body_len_byte = 0;
            if (msg_body.size() >= 65536) {
                // 2 ^ 16 (2 Bytes)
                body_len_byte = 8;
            } else if (msg_body.size() >= 126) {
                body_len_byte = 2;
            }
            //
            if (0 == body_len_byte) {
                packed_msg[1] = (char)msg_body.size();
                packed_msg += msg_body;
            } else if (2 == body_len_byte) {
                packed_msg[1] = 126;
                uint16_t body_len  = (uint16_t)msg_body.size();
                int16_t n_body_len = htons((int16_t)body_len);
                packed_msg.append((const char*)(&n_body_len), sizeof(n_body_len));
                packed_msg += msg_body;
            } else {
                packed_msg[1] = 127;
                size_t body_len = msg_body.size();
                int64_t n_body_len = ws_hton64((int64_t)body_len);
                packed_msg.append((const char*)(&n_body_len), sizeof(n_body_len));
                packed_msg += msg_body;
            }
            return packed_msg;
        }

        void msg_unpack(int fd, std::string &client_msg) {
            //
            std::string recv_msg = map_cli_msg_[fd];
            do {
                //
                int fin = recv_msg[0] & 0x80;
                fin = fin >> 7;
                //
                int opcode = recv_msg[0] & 0x0F;
                //
                int msg_mask = recv_msg[1] & 0x80;
                msg_mask = msg_mask >> 7;
                //
                int payload_len = recv_msg[1] & 0x7F;
                int n_payload_len = payload_len;
                //
                int mask_key_byte_num = 4;
                int payload_length_byte_num = 1;
                //
                if (126 == payload_len) {
                    payload_length_byte_num = 3;
                    //
                    uint16_t tmp_len = 0;
                    memcpy(&tmp_len, recv_msg.c_str() + 2, 2);
                    payload_len = ntohs((int16_t)tmp_len);
                } else if (127 == payload_len) {
                    payload_length_byte_num = 9;
                    //
                    int64_t tmp_len = 0;
                    memcpy(&tmp_len, recv_msg.c_str() + 2, 8);
                    payload_len = (int)ws_ntoh64(tmp_len);
                }
                int msg_len = payload_len;
                //
                //std::string msg_body;
                //msg_body.assign(recv_msg.c_str() + 1 + payload_length_byte_num + mask_key_byte_num);
                if (recv_msg.size() < 1 + payload_length_byte_num + mask_key_byte_num + msg_len) {
                    //std::cout << "the req msg not receive all, req msg len:" << msg_len << ", body len:" << msg_body.length() << std::endl;
                    break;
                }
                //
                std::cout << "fin:" << fin << ", opcode:" << opcode << ", mask:" << msg_mask 
                << ", payload len:" << n_payload_len << ", real msg len:" << msg_len << std::endl;
                //
                std::string mask_key;
                mask_key.assign(recv_msg.c_str() + 1 + payload_length_byte_num, mask_key_byte_num);
                //
                std::string payload_data;
                payload_data.assign(recv_msg.c_str() + 1 + payload_length_byte_num + mask_key_byte_num, msg_len);
                for (int i = 0; i < msg_len; i++) {
                    payload_data[i] = (char)(payload_data[i] ^ mask_key[i % mask_key_byte_num]);
                }
                std::cout << "payload data:" << payload_data << std::endl;
                client_msg = payload_data;
                //
                std::string left_recv_msg;
                left_recv_msg.assign(recv_msg.c_str() + 1 + payload_length_byte_num + mask_key_byte_num + msg_len);
                map_cli_msg_[fd] = left_recv_msg;
            } while (0);
        }

    private:
        int listen_fd_;
        int epoll_fd_;
        std::vector<std::thread> heartbeat_threads_;
        std::map<int, WEBSOCKET_STATUS> conn_status_;
        std::map<int, std::string> map_cli_msg_;
    };
    
    // WebSocketClient
    class WebSocketClient {
    public:
        WebSocketClient() {}
        ~WebSocketClient() {}

        //
    private:
        int connect_fd_;
    };

} // namespace WebSocketCpp

#endif /* __WEBSOCKET_SERVER_H__ */