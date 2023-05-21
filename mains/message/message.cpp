#include "../../services/message_service/http_message_server.h"

int main(int argc, char* argv[])
{
    HTTPMessageServer app;
    return app.run(argc,argv);
}