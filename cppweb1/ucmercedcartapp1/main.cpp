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

#include <bcrypt/BCrypt.hpp>
#include <jwt-cpp/jwt.h>

using namespace std;
using namespace crow;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using mongocxx::cursor;


//Global hash of each instance of page
std::unordered_map<std::string, bool> h;

int main(int argc, const char* argv[])
{
    crow::SimpleApp app;

    mongocxx::instance inst{};
    const auto uri = mongocxx::uri{"mongodb+srv://dyaranon:Moonlight1201%21@neon.w6akxxq.mongodb.net/"};
    mongocxx::client client{uri};
    
     // Carts database with Cart collection
    mongocxx::database carts_db = client["Carts"];
    mongocxx::collection cart_collection = carts_db["Cart"];

    // Users database with User collection
    mongocxx::database users_db = client["Users"];
    mongocxx::collection user_collection = users_db["User"];

    CROW_ROUTE(app, "/")([](const request &req, response &res)
    {
        sendHTML(res, "index.html"); // Loads initial HTML page
    });

    CROW_ROUTE(app, "/index.html")([](const request &req, response &res)
    {
        sendHTML(res, "index.html"); // Loads initial HTML page
    });

    CROW_ROUTE(app, "/manifest.json")([](const request &req, response &res)
    {
        sendJSON(res, "manifest.json"); // Loads manifest file
    });

    CROW_ROUTE(app, "/favicon.ico")([](const request &req, response &res)
    {
        sendImage(res, "favicon.ico"); // Loads favicon
    });

    CROW_ROUTE(app, "/asset-manifest.json")([](const request &req, response &res)
    {
        sendJSON(res, "asset-manifest.json"); // Loads asset manifest
    });

    CROW_ROUTE(app, "/static/css/<string>")([](const request &req, response &res, string fileName)
    {
        sendStyle(res, fileName); // Deals with any CSS
    });

    CROW_ROUTE(app, "/static/js/<string>")([](const request &req, response &res, string fileName)
    {
        sendScript(res, fileName); // Loads javascript dependencies
    });

    CROW_ROUTE(app, "/static/media/<string>")([](const request &req, response &res, string fileName)
    {
        sendImage(res, fileName); // Deals with any images we might use
    });

    CROW_ROUTE(app, "/checkOut.html")([](const request &req, response &res)
    {
        sendHTML(res, "checkOut.html");
    });

    CROW_ROUTE(app, "/checkIn.html")([](const request &req, response &res)
    {
        sendHTML(res, "checkIn.html");
    });

    CROW_ROUTE(app, "/login/").methods("POST"_method)([&user_collection](const request &req)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        crow::json::wvalue x;

        // handles POST Request when submitting login form
        auto b = crow::json::load(req.body);
        if(!b) return crow::response(400);

        char* secret_key = std::getenv("SECRET_KEY");
        if (!secret_key) 
        {
            std::cerr << "The SECRET_KEY environment variable is not set" << std::endl;
            x["res"] = "Internal Server Error";
            return crow::response(500);
        } 
        if(b.has("email") && b.has("password"))
        {
            bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
                user_collection.find_one(bsoncxx::builder::stream::document{} << "email" << b["email"].s() << bsoncxx::builder::stream::finalize);
        
            if(maybe_result)
            {
                bsoncxx::document::view result_view = maybe_result->view();
                bsoncxx::document::element password_element = result_view["password"];
                std::string password_hash = password_element.get_utf8().value.to_string();

                if(BCrypt::validatePassword(b["password"].s(), password_hash))
                {
                    std::cout << "User logged in" << std::endl;
                    x["res"] = "Logged in";
                    x["login"] = true;

                    // Create JWT token
                    std::string secret_key_string(secret_key);
                    std::string email = b["email"].s();

                    auto token = jwt::create()
                        .set_issuer("cartapp")
                        .set_type("JWS")
                        .set_payload_claim("email", jwt::claim(email))
                        .set_issued_at(std::chrono::system_clock::now())
                        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{60*60*24*7})
                        .sign(jwt::algorithm::hs256{secret_key_string});

                    x["token"] = token;
                }
                else
                {
                    std::cout << "Incorrect password" << std::endl;
                    x["res"] = "Incorrect password";
                    x["login"] = false;
                }
            }
            else
            {
                std::cout << "User does not exist" << std::endl;
                x["res"] = "Email not found";
            }
        }
        else
        {
            std::cout << "Missing request params" << std::endl;
            x["res"] = "Unable to log into account, make sure all info is filled in";
        }

        chdir("/");
        return crow::response(x);
    });

    CROW_ROUTE(app, "/register/").methods("POST"_method)([&user_collection](const request &req)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        crow::json::wvalue x;

        // handles POST Request when submitting registration form
        auto b = crow::json::load(req.body);
        if(!b) return crow::response(400);

        if(b.has("email") && b.has("password"))
        {
            bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
                user_collection.find_one(bsoncxx::builder::stream::document{} << "email" << b["email"].s() << bsoncxx::builder::stream::finalize);
        
            if(maybe_result)
            {
                std::cout << "User already exists" << std::endl;
                x["res"] = "Email already taken/used";
            }
            else
            {
                char* secret_key = std::getenv("SECRET_KEY");
                if (!secret_key) 
                {
                    std::cerr << "The SECRET_KEY environment variable is not set" << std::endl;
                    x["res"] = "Internal Server Error";
                    return crow::response(500);
                } 
                else 
                {
                    std::string secret_key_string(secret_key);
                    std::string email = b["email"].s(), password = b["password"].s();
                    std::string hashed_password = BCrypt::generateHash(password);
                    bsoncxx::document::value doc_value = make_document(kvp("email", email), kvp("password", hashed_password));

                    user_collection.insert_one(std::move(doc_value));

                    auto token = jwt::create()
                        .set_issuer("cartapp")
                        .set_type("JWS")
                        .set_payload_claim("email", jwt::claim(email))
                        .set_issued_at(std::chrono::system_clock::now())
                        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{60*60*24*7})
                        .sign(jwt::algorithm::hs256{secret_key_string});

                    x["token"] = token;
                    x["res"] = "Account created successfully";
                } 
            }
        }
        else
        {
            std::cout << "Missing request params" << std::endl;
            x["res"] = "Unable to register account, make sure all info is filled in";
        }

        chdir("/");
        return crow::response(x);
    });

    CROW_ROUTE(app, "/check_out/").methods("POST"_method, "GET"_method)([&cart_collection](const request &req)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        crow::json::wvalue x;

        // handles init GET Request
        if(req.method == "GET"_method) 
        {
            std::cout << "GET request" << std::endl;
            x["method"] = "GET";

            // find how many carts are in collection
            int num_carts = cart_collection.count_documents({});

            // check which carts are available (have not been checked out)
            auto cursor = cart_collection.find(make_document(kvp("is_checked_out", false)));
            std::vector<int> carts;
            for(auto doc : cursor)
            {
                carts.push_back(doc["cart_id"].get_int32().value);
            }
            x["num_carts"] = num_carts;
            x["available_carts"] = carts;
        }

        // handles POST Request when submitting checkout form
        else if (req.method == "POST"_method)
        {
            std::cout << "POST request" << std::endl;
            x["method"] = "POST";

            auto b = crow::json::load(req.body);
            if (!b) return crow::response(400);

            if(b.has("name") && b.has("phoneNumber") && b.has("destination") && b.has("mileage") && b.has("report") && b.has("cart_id"))
            {
                std::string name = b["name"].s(), phoneNumber = b["phoneNumber"].s(), destination = b["destination"].s(), mileage = b["mileage"].s(), report = b["report"].s();
                int id = b["cart_id"].i();  

                x["url"] = "https://hooks.slack.com/services/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";

                // update the cart_collection to show that the cart has been checked out
                auto update_one_result = cart_collection.update_one(make_document(kvp("cart_id", id)), make_document(kvp("$set", make_document(kvp("is_checked_out", true)))));
                
                // check that the Cart collection was updated              
                if(!update_one_result || (update_one_result && update_one_result->modified_count() == 0))
                {
                    x["res"] = "Cart has already been checked out!";
                } 
                else
                {
                    x["res"] = "Cart successfully checked out!";
                }
            }
            else 
            {
                std::cout << "Missing request params" << std::endl;
                x["res"] = "Unable to check out cart, make sure all info is filled in";
            }
        }

        // return json response
        chdir("/");
        return crow::response(x);
    });

    CROW_ROUTE(app, "/check_in/").methods("POST"_method, "GET"_method)([&cart_collection](const request &req)
    {
        chdir("usr/src/cppweb1/ucmercedcartapp1");
        crow::json::wvalue x;

        // handles init GET Request
        if(req.method == "GET"_method) 
        {
            std::cout << "GET request" << std::endl;
            x["method"] = "GET";

            // find how many carts are in collection
            int num_carts = cart_collection.count_documents({});

            // check which carts have been checked out
            auto cursor = cart_collection.find(make_document(kvp("is_checked_out", true)));
            std::vector<int> carts;
            for(auto doc : cursor)
            {
                carts.push_back(doc["cart_id"].get_int32().value);
            }
            x["num_carts"] = num_carts;
            x["available_carts"] = carts;
        }

        // handles POST Request when submitting checkout form
        else if (req.method == "POST"_method)
        {
            std::cout << "POST request" << std::endl;
            x["method"] = "POST";

            auto b = crow::json::load(req.body);
            if (!b) return crow::response(400);

            if(b.has("cart_id"))
            {
                int id = b["cart_id"].i();  

                x["url"] = "https://hooks.slack.com/services/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";

                // update the cart_collection to show that the cart has been checked in
                auto update_one_result = cart_collection.update_one(make_document(kvp("cart_id", id)), make_document(kvp("$set", make_document(kvp("is_checked_out", false)))));
                if(!update_one_result || (update_one_result && update_one_result->modified_count() == 0))
                {
                    x["res"] = "Cart has already been checked in!";
                } 
                else
                {
                    x["res"] = "Cart successfully checked in!";
                }
            }
            else 
            {
                std::cout << "Missing request params" << std::endl;
                x["res"] = "Unable to check in cart, make sure all info is filled in";
            }
        }

        // return json response
        chdir("/");
        return crow::response(x);
    });

    // Necessary Crow stuff to run server 
    char* port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL? std::stoi(port): 18080);
    app.port(iPort).multithreaded().run();

    return 0;
}
