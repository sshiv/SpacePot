#ifndef WEBSERVER_H
#define WEBSERVER_H

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
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

//SSID and Password of your WiFi router
const char* ssid = "Aaham";
const char* password = "kaniporisaavi";

#endif //WEBSERVER_H