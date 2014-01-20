//
//  main.cpp
//  LeapCast
//
//  Created by Jia Chen on 1/18/14.
//  Copyright (c) 2014 Jia Chen. All rights reserved.
//

#include <iostream>

#include "httpRequestHandler.h"

#ifndef HTTP_PORT
#define HTTP_PORT "1234"
#endif
using namespace std;
using namespace Leap;


static httpRequestHandler* gRequestHandler;
// This function will be called by mongoose on every new request
static int requestHander(struct mg_connection *conn) {
    gRequestHandler->handleRequest(conn);
    return 0;
}


int main(int argc, const char * argv[])
{

    cout<<"starting server......"<<endl;
    
    struct mg_server *server;
    gRequestHandler= new httpRequestHandler();
    server = mg_create_server(NULL);
    mg_set_option(server, "listening_port", HTTP_PORT);
    
    mg_add_uri_handler(server, "/", requestHander);
     printf("Server listening on port %s\n", mg_get_option(server, "listening_port"));
    Controller controller;
    
    gRequestHandler->setController(&controller);

    for (;;) {
        mg_poll_server(server, 6);
    }
    mg_destroy_server(&server);
    delete gRequestHandler;

    cout<<"server ended;";
    return 0;
}

