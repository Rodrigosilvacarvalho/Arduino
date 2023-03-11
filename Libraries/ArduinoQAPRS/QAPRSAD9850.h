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
 * @brief Nagłówki dla klasy QAPRSAD9850
 */

#ifndef QAPRSAD9850_H_
#define QAPRSAD9850_H_

#include "QAPRSBase.h"

/**
 * Klasa obsługująca AD9850
 */
class QAPRSAD9850: public QAPRSBase {
private:
	static const uint32_t DDS_CLK = 125000000;
	static const uint8_t DDS_POWER_DOWN = 0b00000100;
	static const uint8_t DDS_POWER_UP = 0b00000000;

	static const uint8_t pinDataDefault = 2;
	static const uint8_t pinClkDefault = 3;
	static const uint8_t pinFqDefault = 4;
	static const uint32_t outputFrequencyDefault = 10147600;

	/**
	 * @brief Czas wysyłania podedynczego tonu. W ms.
	 * @details Czas calkowity powinien wynosic 4*833ms. Wartosć podana tutaj uwzględnia zwłokę związaną z wywoływaniem
	 * funkcji itp.
	 */
	static const uint16_t toneSendTime300 = 700 + 3 * 833;

	uint8_t pinData;
	uint8_t pinClk;
	uint8_t pinFq;

	uint8_t pinData_bit;
	volatile uint8_t * pinData_port;
	uint8_t pinClk_bit;
	volatile uint8_t * pinClk_port;
	uint8_t pinFq_bit;
	volatile uint8_t * pinFq_port;


	uint32_t outputFrequency;
	uint32_t outputFrequencyMark;
	uint32_t outputFrequencySpace;



	inline void writeBit(uint8_t bit);
	uint32_t getFreqParam(uint32_t freq);
	void setFreq(uint32_t freq, uint8_t control);
	void initIO();
	void setPinStateData(uint8_t val);
	void setPinStateClk(uint8_t val);
	void setPinStateFq(uint8_t val);

protected:
	virtual void initializeRadio();
	virtual void enableTransmission();
	virtual void disableTranssmision();
	virtual void initializeTimer1();

	virtual void toggleTone();
public:
	virtual void setVariant(QAPRSVariant variant = QAPRSHF);
	virtual void timerInterruptHandler();
	virtual void init(int8_t pinData, uint8_t pinClk, uint8_t pinFq, uint32_t outputFrequency, char * from_addr, uint8_t from_ssid, char * to_addr, uint8_t to_ssid, char * relays);
	void init(int8_t unused1, int8_t unused2);
	using QAPRSBase::init;
};

#endif /* QAPRSAD9850_H_ */
