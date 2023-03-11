/*
	Copyright (C) 2013 Lukasz Nidecki SQ5RWU
	
    This file is part of ArduinoQAPRS.

    ArduinoQAPRS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ArduinoQAPRS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduinoQAPRS; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Ten plik jest częścią ArduinoQAPRS.

    ArduinoQAPRS jest wolnym oprogramowaniem; możesz go rozprowadzać dalej
    i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU,
    wydanej przez Fundację Wolnego Oprogramowania - według wersji 2 tej
    Licencji lub (według twojego wyboru) którejś z późniejszych wersji.

    Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
    użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
    gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
    ZASTOSOWAŃ. W celu uzyskania bliższych informacji sięgnij do
    Powszechnej Licencji Publicznej GNU.

    Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
    Powszechnej Licencji Publicznej GNU (GNU General Public License);
    jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
    Place, Fifth Floor, Boston, MA  02110-1301  USA

 */
/**
 * @file
 * @brief Plik testowy Arduino
 */
 
 
#include <Arduino.h>
#include <SPI.h>
#include <ArduinoQAPRS.h>

char * packet_buffer  = "                                                                    \n ";
char from_addr[] = "EU1ADI";
char dest_addr[] = "CQ";
char relays[] = "WIDE2 1";

void setup(){
 	// inicjalizacja
	// pin 3 to sensePin [wejscie] - 1 oznacza brak mozliwosci nadawania
	// pin 2 to txPin [wyjscie] - stan wyski w momencie rozpoczecia nadawania
	QAPRS.init(3,2);
}

void loop() {
		// nadanie paketu typu komentarz
		packet_buffer = ":TEST TEST TEST de EU1ADI";
		QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
		// nadanie pakietu z pozycja i symbolem wahadlowca
		packet_buffer = "!5215.68N/02057.48ES#";
		QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
		// nadanie danych pogodowych bez pozycji
		//packet_buffer = "_07071805c025s009g008t030r000p000P000h00b10218";
		//QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
		delay(1500);
}
