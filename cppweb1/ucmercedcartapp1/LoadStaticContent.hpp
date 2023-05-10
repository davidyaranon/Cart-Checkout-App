#ifndef LOAD_STATIC_CONTENT_HPP
#define LOAD_STATIC_CONTENT_HPP

/* This header files loads static content we need to display on the webpage
   (HTML, JS, CSS, images, etc.)
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "crow_all.h"

using namespace std;
using namespace crow;

void sendFile(response &res, std::string fileName, std::string contentType)
{
    auto ss = std::ostringstream{};
    std::ifstream file("/usr/src/cppweb1/ucmercedcartapp1/cart-app/build/" + fileName);
    if(file)
    {
        ss << file.rdbuf();
        res.set_header("Content-Type", contentType);
        res.write(ss.str());
        std::string fileContents = ss.str();
    }
    else
    {
        char f[256];
        res.code = 404;
        res.write("FILE NOT FOUND!!\nEmail dy45@humboldt.edu for updates on the site (he might not know how to fix it though)\n\n\n");
        res.write(getcwd(f, 256));
    }
    res.end();
}

void sendHTML(response &res, std::string fileName)
{
    sendFile(res, fileName, "text/html");
}

void sendJSON(response &res, std::string fileName)
{
    sendFile(res, fileName, "application/json");
}

void sendImage(response &res, std::string fileName)
{
    sendFile(res, "static/media/" + fileName, "image/png");
}

void sendScript(response &res, std::string fileName)
{
    sendFile(res, "static/js/" + fileName, "text/javascript");
}

void sendStyle(response &res, std::string fileName)
{
    sendFile(res, "static/css/" + fileName, "text/css");
}

std::string get_index(mongocxx::collection& coll, std::string key) 
{
    mongocxx::cursor cursor = coll.find({}); // Execute a query with an empty filter (i.e. get all documents).\

    for (const bsoncxx::document::view& doc : cursor) // Iterate the cursor into bsoncxx::document::view objects.
    {
        try
        {
            bsoncxx::document::element ele = doc[key];
            auto value = ele.get_utf8().value.to_string();
            return value;
        }
        catch(...)
        {
            continue;
        }
    }
    return "";
}

bool to_bool(std::string s)
{
    return s == "false";
}

#endif
