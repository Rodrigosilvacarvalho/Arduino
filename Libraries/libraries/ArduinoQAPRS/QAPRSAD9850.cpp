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
 * @brief Implementacja klasy QAPRSAD9850
 */


#include "QAPRSAD9850.h"
/**
 *
 */
void QAPRSAD9850::initializeRadio() {
	this->initIO();
	this->disableTranssmision();
}

/**
 *
 */
void QAPRSAD9850::enableTransmission() {
	this->setFreq(this->outputFrequencyMark, this->DDS_POWER_UP);
	this->doTxDelay();
}

/**
 *
 */
void QAPRSAD9850::disableTranssmision() {
	this->setFreq(this->outputFrequencyMark, this->DDS_POWER_DOWN);
}

/**
 *
 */
void QAPRSAD9850::toggleTone() {
	QAPRSBase::toggleTone();
	if (this->currentTone == QAPRSMark){
		this->setFreq(this->outputFrequencyMark, this->DDS_POWER_UP);
	} else {
		this->setFreq(this->outputFrequencySpace, this->DDS_POWER_UP);
	}
}

/**
 *
 * @param variant
 */
void QAPRSAD9850::setVariant(QAPRSVariant variant) {
	switch(variant){
	case QAPRSHF:
	default:
		QAPRSBase::setVariant(QAPRSHF);
		this->_toneSendTime = this->toneSendTime300;
		this->outputFrequencyMark = this->getFreqParam(this->outputFrequency + 1600);
		this->outputFrequencySpace = this->getFreqParam(this->outputFrequency + 1800);
		break;
	}
}

__attribute__((always_inline)) void QAPRSAD9850::writeBit(uint8_t bit) {
	setPinStateData(bit);
	setPinStateClk(HIGH);
	setPinStateClk(LOW);
	setPinStateData(LOW);
}

inline uint32_t QAPRSAD9850::getFreqParam(uint32_t freq) {
	return freq * 4294967296LL / this->DDS_CLK;
}

void QAPRSAD9850::setFreq(uint32_t freq, uint8_t control) {
	noInterrupts();
	uint8_t i;
	uint8_t * tmp = (uint8_t *)&freq;
	for(i=0;i<8;i++){
			this->writeBit(*(tmp) & (1<<i));
	}
	for(i=0;i<8;i++){
			this->writeBit(*(tmp+1) & (1<<i));
	}
	for(i=0;i<8;i++){
			this->writeBit(*(tmp+2) & (1<<i));
	}
	for(i=0;i<8;i++){
			this->writeBit(*(tmp+3) & (1<<i));
	}

	for(i=0;i<8;i++){
			this->writeBit((control & (1<<i)) != 0);
	}
	setPinStateFq(HIGH);
	delayMicroseconds(2);
	setPinStateFq(LOW);
	interrupts();
}

void QAPRSAD9850::initializeTimer1() {
}

/**
 * Metoda wywoływana przez przerwania po przepełnieniu Timera1.
 * W tej implemetacji nieużywana
 *
 */
void QAPRSAD9850::timerInterruptHandler() {
}

void QAPRSAD9850::init(int8_t pinData, uint8_t pinClk, uint8_t pinFq, uint32_t outputFrequency,
		char* from_addr, uint8_t from_ssid, char* to_addr, uint8_t to_ssid,
		char* relays) {
	this->pinData = pinData;
	this->pinClk = pinClk;
	this->pinFq = pinFq;
	this->outputFrequency = outputFrequency;

	QAPRSBase::init(0,0, from_addr, from_ssid, to_addr, to_ssid, relays);
}

void QAPRSAD9850::initIO() {
	pinMode(this->pinClk, OUTPUT);
	pinMode(this->pinData, OUTPUT);
	pinMode(this->pinFq, OUTPUT);

	this->pinClk_port = portOutputRegister(digitalPinToPort(this->pinClk));
	this->pinClk_bit = digitalPinToBitMask(this->pinClk);
	this->pinData_port = portOutputRegister(digitalPinToPort(this->pinData));
	this->pinData_bit = digitalPinToBitMask(this->pinData);
	this->pinFq_port = portOutputRegister(digitalPinToPort(this->pinFq));
	this->pinFq_bit = digitalPinToBitMask(this->pinFq);
}


inline void QAPRSAD9850::setPinStateData(uint8_t val) {
	if (val == LOW) {
		*(this->pinData_port) &= ~(this->pinData_bit);
	} else {
		*(this->pinData_port) |= (this->pinData_bit);
	}
}

inline void QAPRSAD9850::setPinStateClk(uint8_t val) {
	if (val == LOW) {
		*(this->pinClk_port) &= ~(this->pinClk_bit);
	} else {
		*(this->pinClk_port) |= (this->pinClk_bit);
	}
}

void QAPRSAD9850::setPinStateFq(uint8_t val) {
	if (val == LOW) {
		*(this->pinFq_port) &= ~(this->pinFq_bit);
	} else {
		*(this->pinFq_port) |= (this->pinFq_bit);
	}
}

void QAPRSAD9850::init(int8_t unused1, int8_t unused2) {
	if (!this->pinData){
		this->pinData = this->pinDataDefault;
	}
	if (!this->pinClk){
		this->pinClk = this->pinClkDefault;
	}
	if (!this->pinFq){
		this->pinFq = this->pinFqDefault;
	}
	if (!this->outputFrequency){
		this->outputFrequency = this->outputFrequencyDefault;
	}

	QAPRSBase::init(0,0);
}
