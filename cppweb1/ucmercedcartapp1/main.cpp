#include "crow_all.h"

#include <boost/filesystem.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/types.hpp>

#include "json.h"
#include "LoadStaticContent.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <mutex>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include <unordered_set>

using namespace std;
using namespace crow;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using mongocxx::cursor;


//Global hash of each instance of page
std::unordered_map<std::string, bool> h;

int main(int argc, const char* argv[])
{
    crow::SimpleApp app;

    mongocxx::instance inst{};
    const auto uri = mongocxx::uri{"mongodb+srv://dyaranon:Moonlight1201!@cluster0.jdlbz.mongodb.net/sample_restaurants?retryWrites=true&w=majority"};
    mongocxx::client client{uri};
    mongocxx::database db = client["cart_app_data"];
    mongocxx::collection cart_info = db["cart_info"];
    mongocxx::collection slack_channel = db["slack_channel"];

    std::string url = get_index(slack_channel, "url"), token = get_index(slack_channel, "token"), auth = get_index(slack_channel, "auth");
    
    CROW_ROUTE(app, "/")([](const request &req, response &res)
    {
        sendHTML(res, "index.html"); //Loads initial HTML page
    });

    CROW_ROUTE(app, "/index.html")([](const request &req, response &res)
    {
        sendHTML(res, "index.html"); //Loads initial HTML page
    });

    CROW_ROUTE(app, "/checkIn.html")([&slack_channel](const request &req, response &res)
    {
        sendHTML(res, "checkIn.html");
    });

    CROW_ROUTE(app, "/checkOut.html")([&slack_channel](const request &req, response &res)
    {
        sendHTML(res, "checkOut.html");
    });

    CROW_ROUTE(app, "/styles/<string>")([](const request &req, response &res, string fileName)
    {
        sendStyle(res, fileName); //Deals with any CSS
    });

    CROW_ROUTE(app, "/scripts/<string>")([](const request &req, response &res, string fileName)
    {
        sendScript(res, fileName); //Loads javascript dependencies
    });

    CROW_ROUTE(app, "/images/<string>")([](const request &req, response &res, string fileName)
    {
        sendImage(res, fileName); //Deals with any images we might use
    });

    CROW_ROUTE(app, "/check_out/<string>")([&slack_channel, &url, &token, &auth](const request &req, response &res, std::string ID)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        nlohmann::json x;
        if(req.url_params.get("auth") != nullptr)
        {
            std::ostringstream os;
            mongocxx::cursor cursor = slack_channel.find({});
            for(auto doc : cursor)
            {
                os << (bsoncxx::to_json(doc)) << '\n';
            }
            x["url"] = url;
            x["token"] = token;
            x["authorization"] = auth;
            x["is_checked_out"] = os.str();
            if(req.url_params.get("auth") != x["authorization"]) // password not accepted
            {
                x["auth"] = false;
                chdir("/");
                res.sendJSON(x);
                return;
            }
            x["auth"] = true;
            slack_channel.update_one(document{} << "is_checked_out" << false << finalize, document{} << "$set" << open_document << "is_checked_out" << true << close_document << finalize);
        }
        chdir("/");
        res.sendJSON(x);
    });

    CROW_ROUTE(app, "/check_in/<string>")([&slack_channel, &auth, &url, &token](const request &req, response &res, std::string ID)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        nlohmann::json x;
        if(req.url_params.get("auth") != nullptr)
        {
            std::ostringstream os;
            mongocxx::cursor cursor = slack_channel.find({});
            for(auto doc : cursor)
            {
                os << (bsoncxx::to_json(doc)) << '\n';
            }
            x["url"] = url;
            x["token"] = token;
            x["authorization"] = auth;
            x["is_checked_out"] = os.str();
            if(req.url_params.get("auth") != x["authorization"]) // password not accepted
            {
                x["auth"] = false;
                chdir("/");
                res.sendJSON(x);
                return;
            }
            x["auth"] = true;
            slack_channel.update_one(document{} << "is_checked_out" << true << finalize, document{} << "$set" << open_document << "is_checked_out" << false << close_document << finalize);
        }
        chdir("/");
        res.sendJSON(x);
    });

    CROW_ROUTE(app, "/slack_channel")([&slack_channel]()
    {
        std::ostringstream os;
        mongocxx::cursor cursor = slack_channel.find({});
        for(auto doc : cursor)
        {
            os << bsoncxx::to_json(doc) << '\n';
        }
        return crow::response(os.str());
    });

    CROW_ROUTE(app, "/cart_info")([&cart_info]()
    {
        std::ostringstream os;
        mongocxx::cursor cursor = cart_info.find({});
        for(auto doc : cursor)
        {
            os << bsoncxx::to_json(doc) << '\n' << '\n' << '\n';
        }
        return crow::response(os.str());
    });

    CROW_ROUTE(app, "/reverse_check_in/<string>")([&slack_channel](const request &req, response &res, std::string ID)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        slack_channel.update_one(document{} << "is_checked_out" << false << finalize, document{} << "$set" << open_document << "is_checked_out" << true << close_document << finalize);
        chdir("/");
    });

    CROW_ROUTE(app, "/reverse_check_out/<string>")([&slack_channel](const request &req, response &res, std::string ID)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        slack_channel.update_one(document{} << "is_checked_out" << true << finalize, document{} << "$set" << open_document << "is_checked_out" << false << close_document << finalize);
        chdir("/");
    });

    //Necessary Crow stuff to run server 
    char* port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL? std::stoi(port): 18080);
    //cout << "PORT = " << iPort << '\n';
    app.port(iPort).multithreaded().run();

    return 0;
}