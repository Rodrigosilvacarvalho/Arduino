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
 * @brief Plik testowy
 */

#ifndef ARDUINO

#include <Arduino.h>
#include <ArduinoQAPRS.h>
#include <stdio.h>
#define QAPRS_EXAMPLE_NEW

int main() {
	init();
	// inicjalizacja
	// pin 3 to sensePin [wejscie] - 1 oznacza brak mozliwosci nadawania
	// pin 2 to txPin [wyjscie] - wyan wysko w momencie rozpoczecia nadawania
#ifndef QAPRS_EXAMPLE_NEW
	QAPRS.init(3,2);
#else
	QAPRS.init(0,-2,"SQ5RWU", '0', "APZQAP", '0', "WIDE1-1");
	//QAPRS.init(2,3,4,10147600,"SQ5RWU", '0', "APZQAP", '0', "WIDE1-1");
	//QAPRS.setVariant(QAPRSHF);
#endif

	char packet_buffer[255];
#ifndef QAPRS_EXAMPLE_NEW
	char from_addr[] = "SQ5RWU";
	char dest_addr[] = "APZQAP";
	char relays[] = "WIDE1 1";
#endif
	uint16_t i = 0;
	while(1) {
/*		// nadanie paketu typu komentarz
		packet_buffer = ":TEST TEST TEST de SQ5RWU";
#ifndef QAPRS_EXAMPLE_NEW
		QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
#else
		QAPRS.setFromAddress("SQ5R", '1');
		QAPRS.sendData(packet_buffer);
#endif
		delay(100);*/
		// nadanie pakietu z pozycją i symbolem wahadłowca
		//packet_buffer = "!5215.68N/02057.48E-op. Lukasz testy QAPRS";
		sprintf(packet_buffer, "!5215.68N/02057.48E-op. Lukasz testy QAPRS + Baofeng UV3R+ id=%u", i);
		i++;
#ifndef QAPRS_EXAMPLE_NEW
		QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
#else
		QAPRS.setFromAddress("SQ5RWU", '2');
		QAPRS.setRelays("WIDE2-1");
		QAPRS.sendData(packet_buffer);
#endif
/*
		delay(100);
		// nadanie danych pogodowych bez pozycji
		packet_buffer = "_07071805c025s009g008t030r000p000P000h00b10218";
#ifndef QAPRS_EXAMPLE_NEW
		QAPRS.send(from_addr, '0', dest_addr, '0', relays, packet_buffer);
#else
		QAPRS.setRelays("WIDE1-1");
		QAPRS.sendData(packet_buffer);
#endif
*/
		for(uint8_t j=0;j<15;j++){
			delay(1000);
		}
	}
}
#else

#endif
