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
 * @brief Nagłówki dla klasy QAPRSR2R
 */

#ifndef QAPRSR2R_H_
#define QAPRSR2R_H_
#include "QAPRSBase.h"

/**
 * @brief R2R AFSK
 * @details Klasa generująca AFSK na drabince r2r na porcie B
 */
class QAPRSR2R: public QAPRSBase {
private:
	volatile uint16_t timer1StartValue;
	/**
	 * @brief Stała czasowa dla timera pomiedzy kolejnymi samplami sinusa dla Mark
	 */
	static const uint16_t toneMarkTimeVHF = 65535 - 640;

	/**
	 * @brief Stała czasowa dla timera pomiedzy kolejnymi samplami sinusa dla Space
	 */
	static const uint16_t toneSpaceTimeVHF =  65535 - 363;
	/**
	 * @brief Stała czasowa dla timera pomiedzy kolejnymi samplami sinusa dla Mark
	 */
	static const uint16_t toneMarkTimeHF = 65535 - (596);

	/**
	 * @brief Stała czasowa dla timera pomiedzy kolejnymi samplami sinusa dla Space
	 */
	static const uint16_t toneSpaceTimeHF =  65535 - (530);
	uint16_t toneMarkTime;
	uint16_t toneSpaceTime;
	virtual void enableTransmission();
	virtual void disableTranssmision();
	virtual uint8_t canTransmit();
protected:
	void toggleTone();
public:
	QAPRSR2R() {} ;
	virtual void timerInterruptHandler();
	virtual void init(int8_t sensePin, int8_t txEnablePin);
	using QAPRSBase::init;
	virtual void setVariant(QAPRSVariant variant = QAPRSVHF);
};


#endif /* QAPRSR2R_H_ */
