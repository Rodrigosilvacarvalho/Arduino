/*
  ----      SEMÁFORO DE CARROS E PEDESTRES      ---
  =================================================
  Autor: Rodrigo Silva de Carvalho
  E-mail: rodrigo.silva.carvalho@hotmail.com
  Revisao: 01/11/21
  =================================================
*/

//int grupo1Botao= A1;
int grupo1Vermelho= 2;
int grupo1Amarelo= 3;
int grupo1Verde= 4;

//int grupo2Botao= A2;
int grupo2Vermelho= 5;
int grupo2Amarelo= 6;
int grupo2Verde= 7;

//int grupo3Botao= A3;
int grupo3Vermelho= 8;
int grupo3Amarelo= 9;
int grupo3Verde= 10;

//int grupo4Botao= A4;
int grupo4Vermelho= 11;
int grupo4Amarelo= 12;
int grupo4Verde= 13;

int ciclo= 16000; 
int intervalo= 4000;

void setup() {
 
//pinMode(grupo1Botao, INPUT);
pinMode(grupo1Vermelho, OUTPUT);
pinMode(grupo1Amarelo, OUTPUT);
pinMode(grupo1Verde, OUTPUT);

//pinMode(grupo2Botao, INPUT);
pinMode(grupo2Vermelho, OUTPUT);
pinMode(grupo2Amarelo, OUTPUT);
pinMode(grupo2Verde, OUTPUT);

//pinMode(grupo3Botao, INPUT);
pinMode(grupo3Vermelho, OUTPUT);
pinMode(grupo3Amarelo, OUTPUT);
pinMode(grupo3Verde, OUTPUT);

//pinMode(grupo4Botao, INPUT);
pinMode(grupo4Vermelho, OUTPUT);
pinMode(grupo4Amarelo, OUTPUT);
pinMode(grupo4Verde, OUTPUT);

digitalWrite(grupo1Vermelho, LOW);
digitalWrite(grupo1Amarelo, LOW);
digitalWrite(grupo1Verde, LOW);

digitalWrite(grupo2Vermelho, LOW);
digitalWrite(grupo2Amarelo, LOW);
digitalWrite(grupo2Verde, LOW);

digitalWrite(grupo3Vermelho, LOW);
digitalWrite(grupo3Amarelo, LOW);
digitalWrite(grupo3Verde, LOW);

digitalWrite(grupo4Vermelho, LOW);
digitalWrite(grupo4Amarelo, LOW);
digitalWrite(grupo4Verde, LOW);
}

void loop() {
 
    fase1();
    delay(ciclo);
    fase2();
    delay(intervalo);
    fase3();
    delay(ciclo);
    fase4();
    delay(intervalo);
    fase5();
    delay(ciclo);
    fase6();
    delay(intervalo);
    fase7();
    delay(ciclo);
    fase8();
    delay(intervalo);
  
    
}

void fase1(){
 
 
  digitalWrite(grupo1Vermelho, LOW);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, HIGH);
  

  digitalWrite(grupo2Vermelho,HIGH);
  digitalWrite(grupo2Amarelo,LOW);
  digitalWrite(grupo2Verde,LOW);

  
  digitalWrite(grupo3Vermelho, HIGH);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho,HIGH);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, LOW);

}

void fase2(){
 
 
  digitalWrite(grupo1Vermelho, LOW);
 for(int x = 0; x<5; x++)
  {
    digitalWrite(grupo1Amarelo, HIGH);
    delay(250);
    digitalWrite(grupo1Amarelo, LOW);
    delay(250);
  }
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho,HIGH);
  digitalWrite(grupo2Amarelo,LOW);
  digitalWrite(grupo2Verde,LOW);

  
  digitalWrite(grupo3Vermelho, HIGH);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho,HIGH);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, LOW);

}

void fase3(){
 
 
  digitalWrite(grupo1Vermelho, HIGH);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho,LOW);
  digitalWrite(grupo2Amarelo,LOW);
  digitalWrite(grupo2Verde,HIGH);

  
  digitalWrite(grupo3Vermelho, HIGH);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho,HIGH);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, LOW);

}

void fase4(){
 
 
  digitalWrite(grupo1Vermelho, HIGH);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho,HIGH);
  for(int x = 0; x<5; x++)
  {
    digitalWrite(grupo2Amarelo, HIGH);
    delay(250);
    digitalWrite(grupo2Amarelo, LOW);
    delay(250);
  }
  digitalWrite(grupo2Verde,LOW);

  
  digitalWrite(grupo3Vermelho, HIGH);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho,HIGH);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, LOW);

}

void fase5(){
 
 
  digitalWrite(grupo1Vermelho, HIGH);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho,HIGH);
  digitalWrite(grupo2Amarelo,LOW);
  digitalWrite(grupo2Verde,LOW);

  
  digitalWrite(grupo3Vermelho, LOW);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, HIGH);

  
  digitalWrite(grupo4Vermelho,HIGH);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, LOW);

}

void fase6(){
 
 
  digitalWrite(grupo1Vermelho, HIGH);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho,HIGH);
  digitalWrite(grupo2Amarelo,LOW);
  digitalWrite(grupo2Verde,LOW);

  
  digitalWrite(grupo3Vermelho, LOW);
   for(int x = 0; x<5; x++)
  {
    digitalWrite(grupo3Amarelo, HIGH);
    delay(250);
    digitalWrite(grupo3Amarelo, LOW);
    delay(250);
  }
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho,HIGH);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, LOW);

}

void fase7(){
 
 
  digitalWrite(grupo1Vermelho, HIGH);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho, HIGH);
  digitalWrite(grupo2Amarelo, LOW);
  digitalWrite(grupo2Verde, LOW);

  
  digitalWrite(grupo3Vermelho, HIGH);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho, LOW);
  digitalWrite(grupo4Amarelo, LOW);
  digitalWrite(grupo4Verde, HIGH);

}

void fase8(){
 
 
  digitalWrite(grupo1Vermelho, HIGH);
  digitalWrite(grupo1Amarelo, LOW);
  digitalWrite(grupo1Verde, LOW);
  

  digitalWrite(grupo2Vermelho, HIGH);
  digitalWrite(grupo2Amarelo, LOW);
  digitalWrite(grupo2Verde, LOW);

  
  digitalWrite(grupo3Vermelho, HIGH);
  digitalWrite(grupo3Amarelo, LOW);
  digitalWrite(grupo3Verde, LOW);

  
  digitalWrite(grupo4Vermelho,LOW);
    for(int x = 0; x<5; x++)
  {
    digitalWrite(grupo4Amarelo, HIGH);
    delay(250);
    digitalWrite(grupo4Amarelo, LOW);
    delay(250);
  }
  digitalWrite(grupo4Verde, LOW);

}