# UC-Merced-Cart-Checkout-App

Useful tool to checkout carts on campus, still configuring MongoDB CXX database to support multiple carts and user data

Steps to run on localhost:8080

Make sure Docker is installed!
cd ucmercedcartapp1/cppweb1/cppbox1
docker build -t cppbox1 .
cd .. (go back into cppweb1)
docker run -v / <local/cloned/folder> /ucmercedcartapp1/cppweb1:/usr/src/cppweb1 -ti cppbox1:latest bash
cd usr/src/cppweb1/ucmercedcartapp1/build
cmake ..
make
exit
docker run -v / <local/cloned/folder > /ucmercedcartapp1/cppweb1:/usr/src/cppweb1 -p 8080:8080 -e PORT=8080 cppbox1:latest /usr/src/cppweb1/ucmercedcartapp1/build/ucmercedcartapp1
open localhost:8080 in browser
