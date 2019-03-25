/************************************************************
ESP8266_Shield_Demo.h
SparkFun ESP8266 AT library - Demo
Jim Lindblom @ SparkFun Electronics
Original Creation Date: July 16, 2015
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates the basics of the SparkFun ESP8266
AT library. It'll show you how to connect to a WiFi network,
get an IP address, connect over TCP to a server (as a client),
and set up a TCP server of our own.

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 WiFi Shield Version: 1.0

This code is released under the MIT license.

Distributed as-is; no warranty is given.
************************************************************/

//////////////////////
// Library Includes //
//////////////////////
// SoftwareSerial is required (even you don't intend on
// using it).
#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>
//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.

//////////////////////////////
// ESP8266Server definition //
//////////////////////////////
// server object used towards the end of the demo.
// (This is only global because it's called in both setup()
// and loop()).
ESP8266Server server = ESP8266Server(80);

//////////////////
// HTTP Strings //
//////////////////
const char destServer[] = "example.com";
const String htmlHeader = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Connection: close\r\n\r\n"
                          "<!DOCTYPE HTML>\r\n"
                          "<html><head><title>T.R.Y</title><script language='javascript'><window.setTimeout('window.location.reload()',420000);</script></head>\r\n";
                          
const String httpRequest = "GET / HTTP/1.1\n"
                           "Host: example.com\n"
                           "Connection: close\n\n";

void initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = esp8266.begin();
  if (test != true)
  {
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(test);
  }
  Serial.println(F("ESP8266 Shield Present"));
}

void connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_AP)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_AP);
    if (retVal < 0)
    {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
    }
  }
  
  //Serial.println(F("Mode set to station"));
  Serial.println(F("Mode set to AP"));
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);
  if (retVal > 0)
  {
    Serial.print(F("Connected to"));
    Serial.println(connectedSSID);
  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void clientDemo()
{
  // To use the ESP8266 as a TCP client, use the 
  // ESP8266Client class. First, create an object:
  ESP8266Client client;

  // ESP8266Client connect([server], [port]) is used to 
  // connect to a server (const char * or IPAddress) on
  // a specified port.
  // Returns: 1 on success, 2 on already connected,
  // negative on fail (-1=TIMEOUT, -3=FAIL).
  int retVal = client.connect(destServer, 80);
  if (retVal <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }

  // print and write can be used to send data to a connected
  // client connection.
  client.print(httpRequest);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char
  
  // connected() is a boolean return value - 1 if the 
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}

void serverSetup()
{
  // begin initializes a ESP8266Server object. It will
  // start a server on the port specified in the object's
  // constructor (in global area)
  server.begin();
  Serial.print(F("Server started! Go to "));
  Serial.println(esp8266.localIP());
  Serial.println();
}

void serverDemo()
{
  // available() is an ESP8266Server function which will
  // return an ESP8266Client object for printing and reading.
  // available() has one parameter -- a timeout value. This
  // is the number of milliseconds the function waits,
  // checking for a connection.
  ESP8266Client client = server.available(500);
  
  if (client) 
  {
    Serial.println(F("Client Connected!..........................................."));
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) 
        {
          Serial.println(F("Sending HTML page"));
          // send a standard http response header:
          client.print(htmlHeader);
          String htmlBody;////////////////////////////////////////////////////////////////////////////////////
          // output the value of each analog input pin
          htmlBody+="<h5>| SangMyung University Humanoid RobotClub | From: Choi Eun Kang |</h5><hr><h1>[ Check ]</h1>";
          htmlBody+="CO<sub>2</sub> :  ";
            //float CO2= CO2검출 센서 결과값;
            //htmlBody+= String(); //ZG01CV센서값
          htmlBody+="<br>" ; 
          
          htmlBody+="NH<sub>3</sub> :  ";
            //float NH3= NH3센서 결과값;
            //htmlBody+= String();//TGS2602센서값
          htmlBody+="<br>";  
          
          htmlBody+="Humidity :  ";
            //float HUMI = 습도 센서결과값
            //htmlBody+= String();//DHT22센서값(습도)
          htmlBody+="<br>";
             
          htmlBody+="Temperature :  "; 
            //float TEMP = 온도 센서 결과값
            //htmlBody+= String();//DHT22센서값(온도)
          htmlBody+="<hr>\n";
            
          /*
          for (int a = 0; a < 6; a++)
          {
            htmlBody += "A";
            htmlBody += String(a);
            htmlBody += ": ";
            htmlBody += String(analogRead(a));
            htmlBody += "<br>\n";
          }*/
          client.print(htmlBody); //check 결과 출력
          //////////////// 결과 출력 [result] ///////////////////
          client.print("<h1>[ Result ]</h1>\n");//
          client.print("</html>");
          
          /*
             if(CO2) 범위 
             => client.print("<span style="color:blue"><b>GOOD :)</b></span>");
             => client.print("<span style="color:orange"><b>Not Bad :|</b></span>");
             => client.print("<span style="color:red"><b>Bad :(</b></span>");
             if(NH3)범위
             if(HUMI)범위
             if(TEMP)범위
            */
          
          break;
        }
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }////////////리퀘스트 읽어서 => string을 보낸다~ 자료를 주고받는다//////////////
    // give the web browser time to receive the data
    delay(1);
   
    // close the connection:
    client.stop();
    Serial.println(F("Client disconnected"));
  }
  
}

// errorLoop prints an error code, then loops forever.
void errorLoop(int error)
{
  Serial.print(F("Error: ")); Serial.println(error);
  Serial.println(F("Looping forever."));
  for (;;)
    ;
}

// serialTrigger prints a message, then waits for something
// to come in from the serial port.
void serialTrigger(String message)
{
  Serial.println();
  Serial.println(message);
  Serial.println();
  while (!Serial.available())
    ;
  while (Serial.available())
    Serial.read();
}


// All functions called from setup() are defined below the
// loop() function. They modularized to make it easier to
// copy/paste into sketches of your own.
void setup() 
{
  // Serial Monitor is used to control the demo and view
  // debug information.
  Serial.begin(9600);
  serialTrigger(F("Press any key to begin."));

  // initializeESP8266() verifies communication with the WiFi
  // shield, and sets it up.
  initializeESP8266();

  // connectESP8266() connects to the defined WiFi network.
  connectESP8266(); 

  // displayConnectInfo prints the Shield's local IP
  // and the network it's connected to.
  displayConnectInfo();

  serialTrigger(F("Press any key to connect client."));
//  clientDemo();
  
  serialTrigger(F("Press any key to test server."));
  serverSetup();
}

void loop() 
{
  serverDemo();
}
