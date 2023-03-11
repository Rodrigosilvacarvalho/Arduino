
#include <Adafruit_SSD1306.h> //Inclui a biblioteca para o Display OLED 128x64 0.96 Inch
#include <DHT.h> //Inclui a bilbioteca DHT                       

//==================== Mapeamento de Hadware =================//
#define DHTPIN 2                       //Define o pino do DHT no Arduino
#define DHTTYPE DHT22                   //Define modelo do DHT

//==================== Instânciando Objetos =================//
DHT dht (DHTPIN, DHTTYPE);// Cria o obejto dht para a Classe DHT

Adafruit_SSD1306 Display;


//===================== Variáveis Globais ==================//
const unsigned char imgMestresdoArduino [] PROGMEM = {  
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xc1, 0xe0, 0xe0, 0x1f, 0x07, 0x80, 0x30, 0x3f, 0x00, 0x78, 0x3f, 0xfc, 0x0f, 0xfc, 0x3f, 
  0xff, 0x81, 0xc0, 0x40, 0x0c, 0x03, 0x00, 0x00, 0x06, 0x00, 0x60, 0x0f, 0xf8, 0x01, 0xf0, 0x0f, 
  0xff, 0x81, 0xc0, 0xc0, 0x18, 0x01, 0x00, 0x00, 0x06, 0x00, 0x40, 0x0f, 0xf8, 0x00, 0xe0, 0x07, 
  0xff, 0x81, 0x80, 0x80, 0x10, 0x01, 0x00, 0x00, 0x02, 0x00, 0x40, 0x0f, 0xf8, 0x00, 0xc0, 0x07, 
  0xff, 0x81, 0x80, 0x83, 0xf0, 0xe1, 0xe1, 0xe1, 0x82, 0x1f, 0x83, 0x07, 0xf8, 0x20, 0xc3, 0x87, 
  0xff, 0x01, 0x80, 0x87, 0xf0, 0xe1, 0xe1, 0xe1, 0x80, 0x1f, 0x83, 0x0f, 0xf8, 0x70, 0x83, 0x87, 
  0xff, 0x00, 0x00, 0x80, 0x30, 0x03, 0xc1, 0xc1, 0x84, 0x01, 0x80, 0x0f, 0xf8, 0x70, 0x87, 0x87, 
  0xff, 0x00, 0x01, 0x80, 0x30, 0x07, 0xc1, 0xc0, 0x04, 0x01, 0x80, 0x3f, 0xf8, 0x70, 0x87, 0x87, 
  0xff, 0x00, 0x01, 0x80, 0x30, 0x03, 0xc3, 0xc0, 0x04, 0x01, 0xc0, 0x1f, 0xf0, 0x70, 0x87, 0x87, 
  0xff, 0x00, 0x01, 0x00, 0x7c, 0x03, 0xc3, 0xc0, 0x0c, 0x01, 0xe0, 0x0f, 0xf0, 0xf0, 0x87, 0x07, 
  0xff, 0x00, 0x01, 0x0f, 0xe1, 0x83, 0xc3, 0xc0, 0x18, 0x3f, 0x0e, 0x0f, 0xf0, 0xe0, 0x07, 0x0f, 
  0xfe, 0x00, 0x01, 0x0f, 0xe1, 0xc3, 0x83, 0x80, 0x18, 0x3f, 0x0e, 0x0f, 0xf0, 0xc1, 0x07, 0x0f, 
  0xfe, 0x10, 0x01, 0x00, 0x20, 0x03, 0x83, 0x82, 0x18, 0x01, 0x00, 0x1f, 0xf0, 0x01, 0x80, 0x0f, 
  0xfe, 0x10, 0x01, 0x00, 0x20, 0x07, 0x87, 0x82, 0x18, 0x01, 0x00, 0x1f, 0xe0, 0x03, 0x80, 0x1f, 
  0xfe, 0x10, 0x41, 0x00, 0x20, 0x07, 0x87, 0x86, 0x18, 0x01, 0x80, 0x3f, 0xe0, 0x07, 0x80, 0x3f, 
  0xfe, 0x18, 0x43, 0x00, 0x78, 0x1f, 0x87, 0x86, 0x18, 0x01, 0xc0, 0xff, 0xe0, 0x1f, 0xe0, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0x7c, 0x0f, 0x81, 0xfc, 0x78, 0xc3, 0x0f, 0x1f, 0x07, 0xff, 0x00, 0xff, 0xc0, 0x7f, 
  0xff, 0xe0, 0x78, 0x03, 0x80, 0x38, 0x78, 0x42, 0x0e, 0x1c, 0x03, 0xfe, 0x00, 0x3f, 0x00, 0x1f, 
  0xff, 0xe0, 0x78, 0x01, 0x80, 0x18, 0x70, 0xc2, 0x0e, 0x18, 0x01, 0xfc, 0x00, 0x1e, 0x00, 0x0f, 
  0xff, 0xc0, 0x78, 0x01, 0x00, 0x18, 0x70, 0x82, 0x06, 0x10, 0x01, 0xf8, 0x3c, 0x0c, 0x1f, 0x07, 
  0xff, 0xc0, 0x70, 0x61, 0x0c, 0x18, 0x70, 0x82, 0x04, 0x10, 0xe1, 0xf0, 0xff, 0x00, 0x3f, 0x87, 
  0xff, 0x80, 0x70, 0xe1, 0x0e, 0x18, 0x70, 0x86, 0x00, 0x10, 0xe1, 0xf1, 0xff, 0x80, 0x7b, 0xc3, 
  0xff, 0x80, 0x30, 0xc1, 0x0e, 0x10, 0xf0, 0x84, 0x00, 0x21, 0xe1, 0xf1, 0xff, 0xc0, 0xf1, 0xe3, 
  0xff, 0x80, 0x30, 0x01, 0x0e, 0x10, 0xf1, 0x84, 0x00, 0x21, 0xe1, 0xe1, 0x81, 0xc1, 0xe0, 0xe3, 
  0xff, 0x08, 0x30, 0x03, 0x0e, 0x10, 0xe1, 0x84, 0x00, 0x21, 0xe1, 0xe1, 0xc1, 0xc1, 0xe1, 0xe3, 
  0xff, 0x00, 0x20, 0x06, 0x1e, 0x10, 0xe1, 0x84, 0x00, 0x21, 0xc1, 0xf1, 0xff, 0xc0, 0xf3, 0xe3, 
  0xfe, 0x00, 0x20, 0x0e, 0x1c, 0x10, 0xe1, 0x8c, 0x20, 0x01, 0xc1, 0xf1, 0xff, 0x80, 0xff, 0xc3, 
  0xfe, 0x00, 0x21, 0x0e, 0x18, 0x10, 0xc1, 0x08, 0x20, 0x41, 0xc3, 0xf0, 0xff, 0x00, 0x7f, 0xc3, 
  0xfc, 0x00, 0x21, 0x06, 0x00, 0x30, 0x01, 0x08, 0x20, 0x60, 0x03, 0xf8, 0x7e, 0x0c, 0x1f, 0x07, 
  0xfc, 0x18, 0x21, 0x86, 0x00, 0x70, 0x03, 0x08, 0x70, 0x60, 0x07, 0xf8, 0x00, 0x1e, 0x00, 0x0f, 
  0xfc, 0x38, 0x01, 0x84, 0x00, 0xf0, 0x07, 0x08, 0x70, 0x70, 0x0f, 0xfc, 0x00, 0x3f, 0x00, 0x1f, 
  0xfc, 0x3c, 0x23, 0x84, 0x03, 0xf8, 0x0f, 0x18, 0x70, 0x78, 0x1f, 0xff, 0x00, 0x7f, 0x80, 0x3f, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe1, 0xdc, 0xee, 0x31, 0xf5, 0xef, 0x3c, 0xf7, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe1, 0x98, 0x4e, 0x31, 0xf1, 0xcf, 0x18, 0xe7, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xc1, 0x98, 0xdc, 0xb1, 0xf1, 0x8e, 0x18, 0xe7, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xc1, 0x18, 0xc4, 0x21, 0xe1, 0x8e, 0x38, 0xef, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xd3, 0x5e, 0xce, 0x73, 0xeb, 0xae, 0xb9, 0xef, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

//Vetor para armazenar os dias da semana
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

//==================== void setup =================//
void setup()
{
  pinMode(3, OUTPUT);
  
  Serial.begin(9600);

  dht.begin();
  //Inicializa o OLED 128X64 0.96 INCH com endereço I2C 0x3C
  Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


  Display.setTextColor(WHITE); 
  Display.setTextSize(1); 

  //Limpa o display
  Display.clearDisplay();
  //Atualiza o display
  Display.display();

  //Mostra a imagem no Display
  Display.drawBitmap(0, 0,  imgMestresdoArduino, 128, 64, WHITE);
  //Atualiza o display
  Display.display();

  delay(2000);

  Display.clearDisplay();
  Display.display();

  
  delay(100); //Tempo para salvar novo horário RTC

}//endSetup




//==================== void loop =================//
void loop() 
{
  verifyCommunication();
  screenOne();
  delay(1000);
}//endLoop




//==================== void verifyCommunication =================//
void verifyCommunication()
{

  //Checa a comunicação com o DTH22
  while (isnan( dht.readTemperature() ) || isnan( dht.readHumidity() ))
  {
    //Mensagens
    Serial.println("Falha: DHT22");
   
    Display.setTextSize(1);//Seleciona o tamanho do texto
    Display.setTextColor(WHITE);//Seleciona a cor do texto BLACK ou WHITE
    Display.clearDisplay();
    Display.setCursor(0, 0);
    Display.print("Falha: DHT22");
    Display.display();
  }//endWhile


}//endVerifyCommunication



//==================== void screenOne =================//
void screenOne()
{
  Display.clearDisplay();//Limpa o display



//====== Retângulos - void screenOne ===========//
  //Retângulos
  //          X  Y Comprimento Altura Cor
  Display.drawRect(0, 0, 128, 35, WHITE);

  //          X  Y Comprimento Altura Cor
  Display.drawRect(0, 37, 65, 27, WHITE);

  //          X  Y Comprimento Altura Cor
  Display.drawRect(68, 37, 60, 27, WHITE);

  //====== FILABOX ===========//
  //Printa a temperatura no Display
  Display.setTextSize(3);
  Display.setCursor(2 , 10);
  Display.print("FILABOX");

//====== Temperatura - void screenOne ===========//
  //Printa a temperatura no Display
  Display.setTextSize(3);

  Display.setCursor(7 , 41);
  Display.print(dht.readTemperature(), 0);

//======= Circulo e C - void screenOne =========//
  //Circulo
  //          X  Y Raio Cor
  Display.drawCircle(45, 41, 2, WHITE);

  Display.setCursor(48 , 41);
  Display.print("C");

//========= Umidade- void screenOne ===========//
  //Printa a umidade no Display
  Display.setCursor(72 , 41);
  Display.print(dht.readHumidity(), 0);
  
//========= Porcentagem- void screenOne ===========//
  Display.setCursor(110 , 41);
  Display.print("%");

  Display.display();

  
}//endScreenOne