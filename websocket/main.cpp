#include "websocket_server.h"

int main() {
    WebSocketCpp::WebSocketServer server;
    server.init("10.25.170.33", 8080);
    server.run();
    //
    return 0;
}
