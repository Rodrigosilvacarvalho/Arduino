#include "U8glib.h" 
#include <SPI.h>
#include <SD.h>
#include <EtherCard.h>


U8GLIB_ST7920_128X64 u8g(13, 11, 12, U8G_PIN_NONE);

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,1,203 };

byte Ethernet::buffer[500];
BufferFiller bfill;


void setup() {  

   Serial.begin(9600);

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  ether.staticSetup(myip);
  
   u8g.setColorIndex(1);

 
   
delay(1000);
}

static word homePage() {
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='1'/>"
    "<title>RSC Telecom</title>"
    "<h1>$D$D:$D$D:$D$D</h1>"),
      h/10, h%10, m/10, m%10, s/10, s%10);
  return bfill.position();
}

void draw(void) {

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(5, 20); 
  u8g.print ("WEB SERVER");
  u8g.setPrintPos(5, 40); 
  //u8g.print ("192.168.1.203");
  u8g.setPrintPos(40, 25); 
  u8g.drawStr(50, 50, myip); 
  u8g.drawFrame(0,0,128,64);  
 
  
}

void loop() {  


  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data

 u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  
   

}


