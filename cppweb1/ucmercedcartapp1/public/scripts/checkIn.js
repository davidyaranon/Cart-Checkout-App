$(document).ready(function(){
    //https://gist.github.com/gordonbrander/2230317
    //Generates random session ID :)
    ID = '_' + Math.random().toString(36).substr(2, 9);
});

// Today's date
Date.prototype.today = function () { 
    return (((this.getMonth()+1) < 10)?"0":"") + (this.getMonth()+1) +"/"+ ((this.getDate() < 10)?"0":"") + this.getDate() +"/"+this.getFullYear();
}

// Current time
Date.prototype.timeNow = function () {
     return ((this.getHours() < 10)?"0":"") + this.getHours() +":"+ ((this.getMinutes() < 10)?"0":"") + this.getMinutes() +":"+ ((this.getSeconds() < 10)?"0":"") + this.getSeconds();
}

function checkIn(auth="temp", name=document.getElementById("name").value, phoneNumber="2095551234",
    mileage=document.getElementById("mileage").value, report=document.getElementById("report").value)
{
    if(isNaN(mileage))
    {
        alert("Enter correct value for mileage!");
    }
    // else if(mileage < getCurrMileage())
    // {
    //     alert("Miles do not correspond to what is displayed on cart... check again");
    // }
    // else if(mileage > getCurrMileage() + 10)
    // {
    //     // are you sure?
    // }
    else 
    {
        if(checkFields(auth, name, phoneNumber, mileage, report))
        {
            $.get("/check_in/" + ID, 
            {"auth":auth, "name":name, "phoneNumber":phoneNumber,
            "mileage":mileage,"report":report}, 
            function(response)
            {
                let json = JSON.parse(response);
                const d = new Date();
                let day = d.today();
                let time = d.timeNow();
                if(!determineIfCheckedOut(json))
                {
                    alert("Cannot check in without checking out cart!");
                }
                else if(json["auth"])
                {
                    let text = name + " has checked in cart. \r\nDate: " + day + " @" + time + "\r\n Starting Mileage: " + mileage + "\r\nMaintenence Report: " + report;
                    let token = json["token"]
                    let url = json["url"];
                    $.ajax
                    ({
                        data: 'payload=' + JSON.stringify
                        ({
                            "text": text,
                            "token" : token
                        }),
                        dataType: 'json',
                        processData: false,
                        type: 'POST',
                        url: url
                    }).fail(function()
                    { 
                        // $.get("/reverse_check_in/" + ID, {}, function(response)
                        // {

                        // }).fail(function()
                        // {
                        //     alert("could not reverse check in, uh oh :( ");
                        // });
                        //alert("Server failer while POST... try reloading the page :) ");
                    });
                    document.getElementById('check-in').innerHTML = '<button type = "button" id="checked-in" class="button button-primary"">Checked In!</button>';
                    document.getElementById('checked-in').style.opacity='0.5';
                    document.getElementById('checked-in').disabled = true;
                }
                else 
                {
                    alert("Unable to verify user!");
                }    
            }).fail(function()
            { 
                alert("Server failer while GET... try reloading the page :) ");
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
            return false;
        }
    }
    return true;
}

function determineIfCheckedOut(json)
{
    let long_string = json["is_checked_out"];
    let pos = long_string.search("is_checked_out");
    let sub = long_string.substring(pos + 18, pos + 19);
    if(sub[0] == 'f')
    {
        return false;
    }
    return true;
}