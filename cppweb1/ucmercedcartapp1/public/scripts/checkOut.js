let dest;

$(document).ready(function(){
    //https://gist.github.com/gordonbrander/2230317
    //Generates random session ID :)
    ID = '_' + Math.random().toString(36).substr(2, 9);
    dest = "Academic Office Building (AOB)"

    $.ajax({
        url: "/check_out/",
        method: "GET",
        contentType: "application/json",
        success: function(response) 
        {
            console.log(response);
            const cartSelect = document.getElementById("cart_id");
            for(let i = 0; i < response["num_carts"]; ++i)
            {
                const option = document.createElement("option");
                if(response["available_carts"].includes(i + 1))
                {
                    option.text = "Cart " + (i + 1);
                }
                else
                {
                    option.text = "Cart " + (i + 1) + " (Unavailable)";
                    option.disabled = true;
                }
                option.value = i + 1;
                cartSelect.add(option);
            }
        },
        error: function(xhr, status, error) 
        {
            alert("Unable load cart info, check your internet connection!");
        }
    });
});

function destinationSelection(selection)
{
    dest = (selection.options[selection.selectedIndex].text);
}

// Today's date
Date.prototype.today = function () { 
    return (((this.getMonth()+1) < 10)?"0":"") + (this.getMonth()+1) +"/"+ ((this.getDate() < 10)?"0":"") + this.getDate() +"/"+this.getFullYear();
}

// Current time
Date.prototype.timeNow = function () {
     return ((this.getHours() < 10)?"0":"") + this.getHours() +":"+ ((this.getMinutes() < 10)?"0":"") + this.getMinutes() +":"+ ((this.getSeconds() < 10)?"0":"") + this.getSeconds();
}

function checkOut(auth="temp", name=document.getElementById("name").value, phoneNumber="2095551234", 
    destination=dest, mileage=document.getElementById("mileage").value, report=document.getElementById("report").value, cart_id = document.getElementById("cart_id").value)
{
    if(isNaN(mileage))
    {
        alert("Enter correct value for mileage!");
    }
    else 
    {
        if(checkFields(auth, name, phoneNumber, destination, mileage, report, cart_id))
        {   
            $.ajax({
                url: "/check_out/",
                method: "POST",
                contentType: "application/json",
                data: JSON.stringify({
                    auth: auth,
                    name: name,
                    phoneNumber: phoneNumber,
                    destination: destination,
                    mileage: mileage,
                    report: report,
                    cart_id: cart_id
                }),
                success: function(response) 
                {
                    console.log(response);
                    let json = response;
                    if(!json["res"])
                    {
                        alert("Check yor internet connection!");
                        return;
                    }
                    const d = new Date();
                    let day = d.today();
                    let time = d.timeNow();

                    document.getElementById("check-out-res").innerHTML = json["res"];

                    if(json["url"])
                    {
                        let text = name + " has checked out cart " + cart_id + "\r\nDate: " + day + " @" + time + "\r\nDestination: " + destination + "\r\nEnding Mileage: " + mileage + "\r\nMaintenence Report: " + report;
                        let url = json["url"];

                        // sends message to slack channel
                        $.ajax
                        ({
                            data: 'payload=' + JSON.stringify
                            ({
                                "text": text,
                            }),
                            dataType: 'json',
                            processData: false,
                            type: 'POST',
                            url: url
                        }).done(function(response)
                        {
                            console.log(response);
                        }
                        ).fail(function(jqXHR) {
                            if(jqXHR.status !== 200) 
                            {
                                console.error("Failed to send message to slack channel! Status code: " + jqXHR.status);
                            }
                            else
                            {
                                document.getElementById("check-out-res").innerHTML += "<br>Message sent to slack channel!";
                            }
                        });
                    }
                    document.getElementById('check-out').innerHTML = '<button type = "button" id="checked-out" class="button button-primary"">Checked Out!</button>';
                    document.getElementById('checked-out').style.opacity='0.5';
                    document.getElementById('checked-out').disabled = true; 
                },
                error: function(xhr, status, error) 
                {
                    alert("Unable to checkout cart, try reloading the page or check your internet connection!");
                }
            });
        } 
        else 
        {
            alert("One or more text boxes empty!");
        }
    }
}

function checkFields()
{
    for(let i = 0; i < arguments.length; ++i)
    {
        if(arguments[i] === "")
        {
            console.log("empty field" + arguments[i]);
            return false;
        }
    }
    return true;
}


/*
LIST OF THINGS STILL NEEDING TO BE DONE
- ENSURE USER WHO IS CHECKING IN/OUT IS THE SAME (MAYBE?!)
- ADD DUO SIGN IN (MAYBE), IF NOT INTRODUCE PASSWORD PROTECTION, ENCRYPT + SALT/PEPPER ON BACKEND
- ENSURE MILEAGE IS NOT LESS THAN INTENDED OR MORE THAN INTENDED
- WITH CORRECT CREDENTIALS, BE ABLE TO UPDATE SERVER ACCORDINGLY
- ADD IN SLACK VIEW TO BOTTOM OF SITE
- DO SOMETHING WITH PHONE NUMBER, PERHAPS SEND USER DATA TO DATABASE, MIGHT CLOG IT UP SO MAYBE MOST RECENT USER?
*/
