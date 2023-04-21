#ifndef PAGEHANDLER_H
#define PAGEHANDLER_H

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

#include "../../../database/page/page.h"
#include "../../../utils/helper.h"
#include"../../../utils/hasSubstr.h"

class PageHandler : public HTTPRequestHandler
{
    private:
        bool check_name(const std::string &name, std::string &reason)
        {
            if (name.length() < 3)
            {
                reason = "Name must be at leas 3 signs";
                return false;
            }

            if (name.find(' ') != std::string::npos)
            {
                reason = "Name can't contain spaces";
                return false;
            }

            if (name.find('\t') != std::string::npos)
            {
                reason = "Name can't contain spaces";
                return false;
            }

            return true;
        };
        bool check_last_name(const std::string &last_name, std::string &reason)
        {
            if (last_name.length() < 3)
            {
                reason = "Last name must be at leas 3 signs";
                return false;
            }

            if (last_name.find(' ') != std::string::npos)
            {
                reason = "Last name can't contain spaces";
                return false;
            }

            if (last_name.find('\t') != std::string::npos)
            {
                reason = "Last name can't contain spaces";
                return false;
            }

            return true;
        };  
    public:
        PageHandler(const std::string &format) : _format(format)
        {
        }
        void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
        {
            HTMLForm form(request,request.stream());
            try
            {
                
                if (hasSubstr(request.getURI(), "/load_info"))
                {

                    std::string fn = form.get("first_name");
                    std::string ln = form.get("last_name");
                    auto results = database::Page::load_info(fn, ln);
                    Poco::JSON::Array arr;
                    for (auto s : results)
                        arr.add(s.toJSON());
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(arr, ostr);

                    return;
                }

                else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    if (form.has("first_name") 
                    && form.has("last_name") 
                    && form.has("email")
                    && form.has("lovely_books") 
                    && form.has("lovely_musics") 
                    && form.has("lovely_movies") 
                    && form.has("religion"))
                    {
                        database::Page page;
                        page.first_name() = form.get("first_name");
                        page.last_name() = form.get("last_name");
                        page.email() = form.get("email");
                        page.lovely_books() = form.get("lovely_books");
                        page.lovely_musics() = form.get("lovely_musics");
                        page.lovely_movies() = form.get("lovely_movies");
                        page.religion() = form.get("religion");

                        bool check_result = true;
                        std::string message;
                        std::string reason;

                        if (!check_name(page.get_first_name(), reason))
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }

                        if (!check_last_name(page.get_last_name(), reason))
                        {
                            check_result = false;
                            message += reason;
                            message += "<br>";
                        }

                        if (check_result)
                        {
                            page.save_to_mysql();
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                            response.setChunkedTransferEncoding(true);
                            response.setContentType("application/json");
                            std::ostream &ostr = response.send();
                            ostr << page.get_id();
                            return;
                        }
                        else
                        {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                            std::ostream &ostr = response.send();
                            ostr << message;
                            response.send();
                            return;
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
            root->set("detail", "request not found");
            root->set("instance", "/page");
            std::ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(root, ostr);

        }
private:
    std::string _format;
};
#endif