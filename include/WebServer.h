#ifndef WEBSERVER_H
#define WEBSERVER_H

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>SpacePot Mission Command</title>
    <style>
      input[type=button],
      input[type=submit] {
        background-color: #62529c;
        border: none;
        color: #fff;
        padding: 15px 30px;
        text-decoration: none;
        margin: 4px 2px;
        cursor: pointer;
        height:100px;
        width:400px;
      }
    </style>
</head>
<body>
<center>
<h1>SpacePot Mission Command</h1><br>
<h2>Current Mode:$MODE$</h2><br>
<h2>Set Mode</h2><br>
<form action="/action" method="get">
    <p>Select InstaShip Mode:</p>
    <input type="radio" id="off" name="mode" value="off">
    <label for="off">Off</label><br>
    <input type="radio" id="descend" name="mode" value="descend">
    <label for="descend">Descend</label><br>
    <input type="radio" id="ground" name="mode" value="ground">
    <label for="ground">Ground</label><br>  
    <input type="radio" id="ascend" name="mode" value="ascend">
    <label for="ascend">Ascend</label><br>
    <input type="radio" id="setadvanced" name="mode" value="setadvanced">
    <label for="setadvanced">SetAdvanced</label>
    <input type="text" id="advmodevalue" name="advmode" value="0">
    <br><br>
    <input type="radio" id="restart" name="mode" value="restart">
    <label for="restart">Restart</label><br><br>
    <input type="submit" value="Submit">
</form> 
<hr>
</center>

</body>
</html>
)=====";
//---------------------------------------------------------------

const bool c_ENABLE_AS_ACCESS_POINT = false;

// WiFi AP Config
IPAddress localIp(192,168,1,1);
IPAddress gatewayIp(192,168,1,1);
IPAddress subnet(255,255,255,0);

//SSID and Password of your WiFi router
const char* c_DNS_NAME = "spacepotweb";
const char* ssid = "<YOUR_SSID>";
const char* password = "<YOUR_PASSWORD>";

#endif //WEBSERVER_H