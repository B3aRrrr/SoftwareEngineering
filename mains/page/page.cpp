#include "../../services/page_service/http_page_server.h"


int main(int argc, char*argv[]) 
{
    HTTPPageServer app;
    return app.run(argc, argv);
}