# UC-Merced-Cart-Checkout-App

Useful tool to checkout carts on campus, still configuring MongoDB CXX database to support multiple carts and user data

Steps to run on localhost:8080

- Make sure Docker is installed!<br />
- go into main.cpp and adjust the mongocxx uri (message me for database details)
- cd ucmercedcartapp1/cppweb1/cppbox1<br />
- docker build -t cppbox1 .<br />
- cd .. (go back into cppweb1)<br />
- docker run -v / <local/cloned/folder> /ucmercedcartapp1/cppweb1:/usr/src/cppweb1 -ti cppbox1:latest bash<br />
- cd usr/src/cppweb1/ucmercedcartapp1/build<br />
- cmake ..<br />
- make<br />
- exit<br />
- docker run -v / <local/cloned/folder > /ucmercedcartapp1/cppweb1:/usr/src/cppweb1 -p 8080:8080 -e PORT=8080 cppbox1:latest /usr/src/cppweb1/ucmercedcartapp1/build/ucmercedcartapp1<br />
- open localhost:8080 in browser<br />


![ae](https://user-images.githubusercontent.com/74125645/143675864-042189f9-d756-4e2c-88d3-697158b7f86a.PNG)
![ph](https://user-images.githubusercontent.com/74125645/143675866-b0ce7200-606d-4a99-8d1f-61beff5d42be.PNG)
![mongo](https://user-images.githubusercontent.com/74125645/143675912-3ef94c03-3ef7-4521-a291-7c6ceab8e64e.PNG)

/*
LIST OF THINGS STILL NEEDING TO BE DONE
- ENSURE USER WHO IS CHECKING IN/OUT IS THE SAME (MAYBE?!)
- ADD DUO SIGN IN (MAYBE), IF NOT INTRODUCE PASSWORD PROTECTION, ENCRYPT + SALT/PEPPER ON BACKEND
- ENSURE MILEAGE IS NOT LESS THAN INTENDED OR MORE THAN INTENDED
- WITH CORRECT CREDENTIALS, BE ABLE TO UPDATE SERVER ACCORDINGLY
- ADD IN SLACK VIEW TO BOTTOM OF SITE
- DO SOMETHING WITH PHONE NUMBER, PERHAPS SEND USER DATA TO DATABASE, MIGHT CLOG IT UP SO MAYBE MOST RECENT USER?
*/
