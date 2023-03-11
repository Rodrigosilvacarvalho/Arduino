#include <SPI.h>
#include <String.h>
#include <Ethernet.h>
#include <SD.h>

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; //physical mac address
byte ip[] = { 192, 168, 1, 99 }; // ip in lan
byte gateway[] = { 192, 168, 1, 2 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

int led1 = 5;
int led2 = 6;
int led3 = 7;

String readString = String(30);

String statusLed;

File webFile;

void setup()
{
    Ethernet.begin(mac, ip, gateway, subnet);  // initialize Ethernet device
    server.begin();           // start to listen for clients
    Serial.begin(9600);       // for debugging
    
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
    
    // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
               if(readString.length() < 30){
                  readString += (c);
               }                
                if (c == '\n' && currentLineIsBlank) {
                  
                           // FAZ A VERIFICAÇÃO PRA ACENDER OU APAGAR O LED
         
                   if (readString.indexOf("1") >=0) {
                      digitalWrite(led1, !digitalRead(led1));
                  }
         
                   if (readString.indexOf("2")>=0) 
                   {
                      digitalWrite(led2, !digitalRead(led2));                
                      }
                   if (readString.indexOf("3") >=0)
                     {
                      digitalWrite(led3, !digitalRead(led3));
                    }
         
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    webFile = SD.open("index.htm");        // open web page file
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read()); // send web page to client
                        }
                        webFile.close();
   
                    }
                    break;
                                   }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        readString = "";
        client.stop(); // close the connection
        
    } 
}
