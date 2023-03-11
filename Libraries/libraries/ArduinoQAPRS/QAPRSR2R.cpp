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
 * @brief Implementacja klasy QAPRSR2R
 */

#include "QAPRSR2R.h"

/**
 * Przełącz generowany ton. W tej implementacji (R2R) zmieniany jest czas co jaki ma się przepełnić Timer1.
 */
void QAPRSR2R::toggleTone() {
	QAPRSBase::toggleTone();
	if (this->currentTone == QAPRSMark){
		 this->timer1StartValue = this->toneMarkTime;
		 //PORTC &= ~_BV(PINC2);
	} else {
		 this->timer1StartValue = this->toneSpaceTime;
		 //PORTC |= _BV(PINC2);
	}
	TCNT1 = this->timer1StartValue;
}

/**
 * Włącz nadawania. W tej implementacji uruchom także Timer1
 */
void QAPRSR2R::enableTransmission() {
	QAPRSBase::enableTransmission();
	TCCR1B=(1<<CS10);
}

/**
 * Wyłącz nadawanie. W tej implementacji zatrzymaj także Timer1
 */
void QAPRSR2R::disableTranssmision() {
	QAPRSBase::disableTranssmision();
	TCCR1B&= ~ ((1<<CS12)|(1<<CS11)|(1<<CS10));
}

/**
 * Metoda wywoływana przez przerwania po przepełnieniu Timera1.
 * W tej implemetacji następuje wystawienie na portach PB0 do PB3 4bitowej wartosci próbki funkcji sinus.
 * Testowano z drabinką rezystorową jak na obrazku <http://upload.wikimedia.org/wikipedia/commons/thumb/4/41/R2r-ladder.png/420px-R2r-ladder.png>
 * Wartosci rezystorów w modelu to 1k i 2k2
 *
 */
void QAPRSR2R::timerInterruptHandler() {
	/*
	 * Wartosci przybliżajace probki sinusa na 4 bitach.
	 */
	static const uint8_t sineSamples[] = {7, 10, 13, 14, 15, 14, 13, 10, 8, 5, 2, 1, 0, 1, 2, 5	};
	/*
	 * Wskaźnika na bieżącą próbkę
	 */
	static uint8_t sineSamplesPointer;

	/*
	 * Przypisz do dolnych (młodszych) 4 bitów zawartosć bieżącej próbki
	 */
	PORTB = ( PORTB & 0b11110000 ) | (sineSamples[sineSamplesPointer] & 0b00001111);
	sineSamplesPointer++;
	sineSamplesPointer &= 0b00001111;
	/*
	 * Ustaw czas następnego przepełnienia
	 */
	TCNT1 = this->timer1StartValue;
}

/**
 * Czy można wysyłać dane?
 * @return Jesli pin sense jest ustawiony i panuje na nim stan wysoki (lub niski jesli numer pinu jest ujemny) to nie.
 */
uint8_t QAPRSR2R::canTransmit() {
	if (this->sensePin){
		if (this->sensePin > 0){
			return !digitalRead(this->sensePin);
		} else {
			return digitalRead(abs(this->sensePin));
		}
	} else {
		return 1;
	}

}

void QAPRSR2R::setVariant(QAPRSVariant variant) {
	QAPRSBase::setVariant(variant);
	switch(variant){
	case QAPRSVHF:
		this->toneMarkTime = this->toneMarkTimeVHF;
		this->toneSpaceTime = this->toneSpaceTimeVHF;
		break;
	case QAPRSHF:
		this->toneMarkTime = this->toneMarkTimeHF;
		this->toneSpaceTime = this->toneSpaceTimeHF;
		break;
	}
}

/**
 * Inicjalizacja biblioteki. W tej implementacji dodatkowo następuje ostawienie od PB0 do PB3 jako wyjsć.
 * @param sensePin Pin [we] Arduino na którym 0 oznacza zgodę na nadwanie a 1 jej brak
 * 		Podanie 0 jako numeru pinu powoduje WYŁACZENIE wykrywania nadawania i zmusza programistę do samodzielnej jego obsługi!
 * @param txEnablePin Pin [wy] Arduino na którym 1 oznacza nadawanie
 */
void QAPRSR2R::init(int8_t sensePin, int8_t txEnablePin) {
	QAPRSBase::init(sensePin, txEnablePin);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
}
