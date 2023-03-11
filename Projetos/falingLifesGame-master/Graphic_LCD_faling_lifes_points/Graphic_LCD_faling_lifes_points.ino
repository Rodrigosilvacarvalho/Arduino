
#include "U8glib.h"


U8GLIB_ST7920_128X64_1X u8g(13, 11, 12 ,2); //Enable, RW, RS, RESET  
 
const uint8_t hart_bitmap[] U8G_PROGMEM = {
  0x00,         // 00000000 
  0x66,         // 01100110
  0xff,         // 11111111
  0xff,         // 11111111
  0xff,         // 11111111 
  0x7e,         // 01111110
  0x3c,         // 00111100 
  0x18          // 00011000
};

const uint8_t super_bitmap[] U8G_PROGMEM = {
  0xbd,         // 00000000 
  0x66,         // 01100110
  0xcb,         // 11111111
  0xb9,         // 11111111
  0x9d,         // 11111111 
  0xd3,         // 01111110
  0x66,         // 00111100 
  0xbd          // 00011000
};

const uint8_t rook_bitmap[] U8G_PROGMEM = {
  0x18,         // 00011000 
  0x3c,         // 00111100
  0x7e,         // 01111110
  0xff,         // 11111111
  0xff,         // 11111111 
  0x7e,         // 01111110
  0x3c,         // 00111100 
  0x18          // 00011000
};

const uint8_t cursor_bitmap[] U8G_PROGMEM = {
  0x81,         // 10000001 
  0x81,         // 10000001
  0x81,         // 10000001
  0x81,         // 10000001
  0x81,         // 10000001 
  0xc3,         // 11000011
  0x66,         // 00111100 
  0x3C,         // 00011000
};

long previousMillis = 0;        // will store last time LED was updated
long interval = 3000;           // interval at which to blink (milliseconds)

int X = 0;
int Y = 0;
float Xanalog = 0;
int Pause = 100;
int PushButtonRipple = 0;

int ScreenWith = 128;
int ScreenHeight = 64;
uint8_t draw_state = 0;
int Loading = 0;

int Xhart = random(120) + 4;
int Yhart = 0;

int Scoore = 0;
int Hiscoore = 0;
int Life = 3;

int Icone = 2;
int Speed = 200;
int StartSpeed = 200;

void draw(void) {
  switch(draw_state) {
    case 0: drawWelcome(); break;
    case 1: drawGame(); break;
    case 2: drawGameOver(); break;
  }
}

void drawGameOver() {
  u8g.setFont(u8g_font_fub17);
    u8g.setPrintPos(3, 25); 
    u8g.print("Game over");
    
    u8g.setFont(u8g_font_gdr9);
    u8g.setPrintPos(20, 38); 
    u8g.print("Pontos: ");
    u8g.print(Scoore);
    
    u8g.setPrintPos(20, 50); 
    u8g.print("max pont.: ");
    u8g.print(Hiscoore);
    
    
    
}

void drawWelcome() {
  u8g.setFontPosTop();
  
  u8g.setFont(u8g_font_fub17);
  u8g.setPrintPos(10, 15); 
  u8g.print("Pega Pega");
  
  u8g.setFont(u8g_font_gdr9);
  u8g.setPrintPos(10, 42); 
  u8g.print("RSC Telecom");
  
  u8g.drawFrame(10,48,100,6);
  
  u8g.drawFrame(12,50,Loading,2);
  
  u8g.setFont(u8g_font_u8glib_4);
  u8g.setPrintPos(10, 63); 
  u8g.print("2018 (c)Rodrigo Silva carvalho");
  u8g.print(Hiscoore);
  
}

void drawGame(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFontPosTop();
  
  
    u8g.setFont(u8g_font_gdr9);
    u8g.setPrintPos(2, 50); 
    u8g.print("Pontos: ");
    u8g.print(Scoore); 
    
    u8g.setPrintPos(80, 50); 
    u8g.print("Vidas:");
    u8g.print(Life); 
    
    u8g.setPrintPos(0, 0); 
    //u8g.print("(");
    //u8g.print(Speed);
    //u8g.print(")"); 
    
    u8g.drawFrame(1,0,1,int(Speed/10));
  
    u8g.drawBitmapP( X, 44, 1, 8, cursor_bitmap); // player
    
    if(Icone==1) {
      u8g.drawBitmapP( Xhart, Yhart, 1, 8, hart_bitmap); //Faling hart
    }else if(Icone==2) {
      u8g.drawBitmapP( Xhart, Yhart, 1, 8, super_bitmap); //Faling hart
    }else {
      u8g.drawBitmapP( Xhart, Yhart, 1, 8, rook_bitmap); //Faling hart
    }
}

void setup(void) {
}

void loop(void) {
  // picture loop
  if(draw_state == 0 || draw_state == 2) {
    unsigned long currentMillis = millis();
    
    Loading = int((currentMillis - previousMillis)/30);
 
    if(draw_state == 0 && (currentMillis - previousMillis > interval)) {
      previousMillis = currentMillis;
      
      draw_state = 1;
      Scoore = 0;
      Life = 3;
    }
    
    if(draw_state == 2) {
      if(analogRead(A2)<10) {
        draw_state = 0;
        previousMillis = currentMillis;
        Loading = 0;
      }

    }
  }
  
  
  
  
  
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage());
  
  
  if(draw_state == 0) {
    //delay(5000); 
    //draw_state = 1;
  }
  
  if(draw_state == 2) {
    Yhart = 0;
    //draw_state = 0;
  }
  
  if(draw_state == 1) {
    if(analogRead(A2)<1 && PushButtonRipple==0) {
      Pause = Pause * -1;
      PushButtonRipple = 50;
    }
    PushButtonRipple--;
    if(PushButtonRipple<0) {
      PushButtonRipple = 0;
    }
    
    
    if(Pause>1) {
      updateData();
      checkHit();
    }else{
      
    }  
  }
  
  // rebuild the picture after some delay
  //delay(10);
}

void checkHit() {
  if(Xhart<X+8 && Xhart>X-8 && Yhart<49 && Yhart>41) {
   if(Icone==1) {
     Life++;
     //if(Life>5) {Life = 5;}
   }else if(Icone==2) {
     Scoore = Scoore + 3;
     Speed = Speed - 100;
     if(Speed<100) {Speed=100;}
     
   }else {
     Scoore++;
   }
   newIcone();
  }
  
}

void updateData() {

  X = analogRead(A1); 
  
  if(analogRead(A1)>812 || analogRead(A1)<212) {
    Speed = Speed + 1;
    if(Speed>StartSpeed+Scoore) {Speed = StartSpeed+Scoore;}
  }
  
  Xanalog = Xanalog + (X - 512) / Speed;
  X = round(Xanalog);
  if(Xanalog<1) {Xanalog=ScreenWith-8;}
  if(Xanalog>ScreenWith-8) {Xanalog = 0;}
  
  Y = analogRead(A0);   
  Y = map(Y, 0, 1023, 5, 60);
  
  Yhart++;
  if(Yhart>ScreenHeight) {   
    if(Icone==1) {
      
    }else if(Icone==2) {
      
    }else {
      Life--;
      if(Life<0) {
        if(Hiscoore<Scoore) {
          Hiscoore = Scoore;
        }
      }
    }
    newIcone();
    if(Life<0) {draw_state = 2;}
  }

}

void newIcone() {
  Icone = int(random(10));
  Xhart = random(120) + 4;
  Yhart = 0;
}
