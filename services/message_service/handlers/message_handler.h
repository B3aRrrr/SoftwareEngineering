#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <optional>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../../database/message/message.h"
#include "../../../utils/helper.h"
#include"../../../utils/hasSubstr.h"

std::optional<std::string> do_get(const std::string &url, const std::string &login, const std::string &password)
{
    std::string string_result;
    try
    {
        std::string token = login + ":" + password;
        std::ostringstream os;
        Poco::Base64Encoder b64in(os);
        b64in << token;
        b64in.close();
        std::string identity = "Basic " + os.str();

        Poco::URI uri(url);
        Poco::Net::HTTPClientSession s(uri.getHost(), uri.getPort());
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
        request.setVersion(Poco::Net::HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.set("Authorization", identity);
        request.set("Accept", "application/json");
        request.setKeepAlive(true);

        s.sendRequest(request);

        Poco::Net::HTTPResponse response;
        std::istream &rs = s.receiveResponse(response);

        while (rs)
        {
            char c{};
            rs.read(&c, 1);
            if (rs)
                string_result += c;
        }

        if (response.getStatus() != 200)
            return {};
    }
    catch (Poco::Exception &ex)
    {
        std::cout << "exception:" << ex.what() << std::endl;
        return std::optional<std::string>();
    }

    return string_result;
}

int TryAuth(HTTPServerRequest &request,  HTTPServerResponse &response)
{
    std::string scheme;
    std::string info;
    std::string login, password;

    try{
        request.getCredentials(scheme, info);
    }
    catch(...)
    {
        //Ignored
    }
    
    if (scheme == "Basic")
    {
        get_identity(info, login, password);
        std::cout << "login:" << login << std::endl;
        std::cout << "password:" << password << std::endl;
        std::string host = "localhost";
        std::string url;

        if(std::getenv("SERVICE_HOST")!=nullptr) host = std::getenv("SERVICE_HOST");
        
        url = "http://" + host+":8080/auth";

        try{
            std::optional<std::string> authStr = do_get(url, login, password);

            if (authStr.has_value())
            {
                Poco::JSON::Parser parser;
                Poco::Dynamic::Var result = parser.parse(authStr.value());
                Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

                return object->getValue<long>("id");
            }
        }
        catch(...)
        {
            //Ignored
        }
    }

    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    root->set("type", "/errors/unauthorized");
    root->set("title", "Unauthorized");
    root->set("status", "401");
    root->set("detail", "Invalid login or password");
    root->set("instance", "/User");
    std::ostream &ostr = response.send();
    Poco::JSON::Stringifier::stringify(root, ostr);

    return 0;
}


class MessageHandler : public HTTPRequestHandler
{
    public:
    MessageHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());

        long id_from = TryAuth(request, response);

        if(id_from == 0)
        {
            //No Auth
            return;
        }
        try
        {
            if (hasSubstr(request.getURI(), "/send_msg")&&
                    (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) &&
                    form.has("id_to") &&
                    form.has("message"))
            {
            
                    long id_to = atol(form.get("id_to").c_str());
                    std::string message = form.get("message").c_str();

                    database::Message::send(id_from,id_to,message);

                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    response.send();
                    return;
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
        {
            if (form.has("id_from")&&form.has("id_to")&&form.has("message"))
            {
                std::cout << "add message" << std::endl;
                database::Message message;
                message.id_from() = atol(form.get("id_from").c_str());
                message.id_to() = atol(form.get("id_to").c_str());
                message.message() = form.get("message");

                try
                {
                    message.save_to_mysql();
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    ostr << message.get_id();
                    return;
                }
                catch (...)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                    std::ostream &ostr = response.send();
                    ostr << "{ \"result\": false , \"reason\": \"exception\" }";
                    response.send();
                }
            }
        }
        }
        catch (...)
        {
        }
        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "Request not found");
        root->set("instance", "/p2p");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif