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

#include "QAPRSSi4463.h"

/**
 * Inicjalizacja radia
 */
void QAPRSSi4463::initializeRadio() {
	this->initSPI();
	this->initControlPins();
	//this->Si4463_enableRadio();

	this->disableTranssmision();
	this->initializeTimer1();
}
/**
 * Załączenie nadawania modułu
 */
void QAPRSSi4463::enableTransmission() {
	this->Si4463_enableRadio();
	si446x_start_tx();
	this->doTxDelay();

	TCCR1B=(1<<CS10);
}

/**
 * Wyłączenie nadawania w module
 */
void QAPRSSi4463::disableTranssmision() {
	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP);
	TCCR1B&= ~ ((1<<CS12)|(1<<CS11)|(1<<CS10));
	this->Si4463_shutdownRadio();
}

/**
 * Przełącz generowany ton.
 */
void QAPRSSi4463::toggleTone() {
	QAPRSBase::toggleTone();
	//this->toneChange = 1;
	//timer stop
	TCCR1B&= ~ ((1<<CS12)|(1<<CS11)|(1<<CS10));
	//ile ticków
	int16_t ticks_used = TCNT1 - this->timer1StartValue;
	if (this->currentTone == QAPRSMark){
		 this->timer1StartValue = 65535 - this->toneMarkTime;
		 if (ticks_used >= this->toneMarkTime){
			 this->timerInterruptHandler();
		 } else {
			 TCNT1 = this->timer1StartValue + ticks_used;
		 }
	} else {
		 this->timer1StartValue = 65535 - this->toneSpaceTime;
		 if (ticks_used >= this->toneSpaceTime){
			 this->timerInterruptHandler();
		 } else {
			 TCNT1 = this->timer1StartValue + ticks_used;
		 }
	}

	//timer start
	TCCR1B=(1<<CS10);

}

/**
 * Hardware reset modułu
 */
void QAPRSSi4463::Si4463_resetRadio() {
	//reset radio
	digitalWrite(this->powerPin, HIGH);
	delay(5);
	digitalWrite(this->powerPin, LOW);
	delay(4);
}

void QAPRSSi4463::Si4463_shutdownRadio(){
}

void QAPRSSi4463::Si4463_enableRadio(){
	this->Si4463_resetRadio();

	this->ctsWentHigh = 0;

	uint8_t radio_config[] = RADIO_CONFIGURATION_DATA_ARRAY;
	while(si446x_configuration_init(radio_config)){
		this->Si4463_resetRadio();
	}

	si446x_get_int_status(0u, 0u, 0u);
}
/**
 * Inicjalizacja SPI na potrzeby transmisji z modułem
 */
void QAPRSSi4463::initSPI() {
	// init SPI
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setBitOrder(MSBFIRST);
}

/**
 * Inicjalizacja dodatkowych pinów do sterowania modułem
 */
void QAPRSSi4463::initControlPins() {
	// init Control pins
	pinMode(this->nselPin, OUTPUT);
	digitalWrite(this->nselPin, HIGH);
	pinMode(this->powerPin, OUTPUT);
	digitalWrite(this->powerPin, LOW);
	pinMode(this->tonePin,OUTPUT);
	digitalWrite(this->tonePin, LOW);
}

/**
 * Przesłanie komend inicjalizujących moduł
 * @param pSetPropCmd Tablica z komendami
 * @todo W późniejszej wersji należy przepisać kod tak, aby trzymał dane konfiguracyjne w pamięci programu a nie w RAMie
 * @return
 */
uint8_t QAPRSSi4463::si446x_configuration_init(const uint8_t* pSetPropCmd) {
	uint8_t col;
	uint8_t numOfBytes;

	/* While cycle as far as the pointer points to a command */
	while (*pSetPropCmd != 0x00)
	{
		/* Commands structure in the array:
		 * --------------------------------
		 * LEN | <LEN length of data>
		 */

		numOfBytes = *pSetPropCmd++;


		for (col = 0u; col < numOfBytes; col++)
		{
			Pro2Cmd[col] = *pSetPropCmd;
			pSetPropCmd++;
		}

		if (radio_comm_SendCmdGetResp(numOfBytes, Pro2Cmd, 0, 0) != 0xFF)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * Wysyła komendę do modułu i pobiera dane zwrotne
 * @param cmdByteCount Ile danych komendy
 * @param pCmdData Dane komendy
 * @param respByteCount Ile danych odpowiedzi
 * @param pRespData Dane odpowiedzi
 * @return
 */
uint8_t QAPRSSi4463::radio_comm_SendCmdGetResp(uint8_t cmdByteCount,
		uint8_t* pCmdData, uint8_t respByteCount, uint8_t* pRespData) {
	radio_comm_SendCmd(cmdByteCount, pCmdData);
	return radio_comm_GetResp(respByteCount, pRespData);
}

void QAPRSSi4463::radio_comm_SendCmd(uint8_t byteCount, uint8_t* pData) {
	/* There was a bug in A1 hardware that will not handle 1 byte commands.
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
	if (byteCount == 1)
		byteCount++;

	while (!ctsWentHigh)
	{
		radio_comm_PollCTS();
	}
	radio_hal_ClearNsel();
	radio_hal_SpiWriteData(byteCount, pData);
	radio_hal_SetNsel();
	ctsWentHigh = 0;
}

void QAPRSSi4463::radio_comm_ReadData(uint8_t cmd, uint8_t byteCount, uint8_t* pData)
{
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiReadData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

void QAPRSSi4463::si446x_frr_a_read(uint8_t respByteCount, uint8_t* pData)
{
    radio_comm_ReadData(SI446X_CMD_ID_FRR_A_READ,
                        respByteCount,
                        pData);
}

uint8_t QAPRSSi4463::radio_comm_PollCTS(void) {
	return radio_comm_GetResp(0, 0);
}

uint8_t QAPRSSi4463::radio_comm_GetResp(uint8_t byteCount, uint8_t* pData) {
	uint8_t ctsVal = 0;
	uint16_t errCnt = 5000;

	while (errCnt != 0)      //wait until radio IC is ready with the data
	{
		radio_hal_ClearNsel();
		radio_hal_SpiWriteByte(0x44);    //read CMD buffer
		ctsVal = radio_hal_SpiReadByte();
		if (ctsVal == 0xFF)
		{
			if (byteCount)
			{
				radio_hal_SpiReadData(byteCount, pData);
			}
			radio_hal_SetNsel();
			break;
		}
		radio_hal_SetNsel();
		errCnt--;
	}

	if( errCnt == 0 )
	{
		pinMode(2,OUTPUT);
		digitalWrite(2, 1);
		while(1)
		{
			/* ERROR!!!!  CTS should never take this long. */
		}
	}

	if (ctsVal == 0xFF)
	{
		ctsWentHigh = 1;
	}

	return ctsVal;
}

void QAPRSSi4463::radio_hal_ClearNsel(void) {
	digitalWrite(this->nselPin, 0);
}

void QAPRSSi4463::radio_hal_SetNsel(void) {
	digitalWrite(this->nselPin, 1);
}

void QAPRSSi4463::radio_hal_SpiWriteByte(uint8_t byteToWrite) {
	SPI.transfer(byteToWrite);
}

uint8_t QAPRSSi4463::radio_hal_SpiReadByte(void) {
	return SPI.transfer(0xff);
}

void QAPRSSi4463::radio_hal_SpiReadData(uint8_t byteCount, uint8_t* pData) {
	uint8_t bCnt;
	// send command and get response from the radio IC
	for (bCnt=0; bCnt<byteCount; bCnt++)
	{
		pData[bCnt] = SPI.transfer(0xFF);           // Store data byte that came from the radio IC
	}
}

void QAPRSSi4463::radio_hal_SpiWriteData(uint8_t byteCount, uint8_t* pData) {
	uint8_t bCnt;

	for (bCnt=0; bCnt<byteCount; bCnt++)    // Send input data array via SPI
	{
		SPI.transfer(pData[bCnt]);
	}
}

void QAPRSSi4463::si446x_get_int_status(uint8_t PH_CLR_PEND,
		uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND) {
	Pro2Cmd[0] = SI446X_CMD_ID_GET_INT_STATUS;
	Pro2Cmd[1] = PH_CLR_PEND;
	Pro2Cmd[2] = MODEM_CLR_PEND;
	Pro2Cmd[3] = CHIP_CLR_PEND;

	radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_INT_STATUS,
			Pro2Cmd,
			SI446X_CMD_REPLY_COUNT_GET_INT_STATUS,
			Pro2Cmd
	);
}

void QAPRSSi4463::si446x_start_tx() {
	Pro2Cmd[0] = SI446X_CMD_ID_START_TX;
	Pro2Cmd[1] = 0;
	Pro2Cmd[2] = 0x18;
	Pro2Cmd[3] = 0;
	Pro2Cmd[4] = 0;

	radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_TX, Pro2Cmd );
}

void QAPRSSi4463::si446x_start_rx()
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_RX;
    Pro2Cmd[1] = 0;
    Pro2Cmd[2] = 0;
    Pro2Cmd[3] = 0;
    Pro2Cmd[4] = 0;
    Pro2Cmd[5] = 8;
    Pro2Cmd[6] = 8;
    Pro2Cmd[7] = 8;

    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_RX, Pro2Cmd );
}

void QAPRSSi4463::si446x_get_modem_status()
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_MODEM_STATUS;
    Pro2Cmd[1] = 0;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_MODEM_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_MODEM_STATUS,
                              Pro2Cmd );
    this->rssi = Pro2Cmd[2];
}


void QAPRSSi4463::si446x_change_state(uint8_t NEXT_STATE1) {
	Pro2Cmd[0] = SI446X_CMD_ID_CHANGE_STATE;
	Pro2Cmd[1] = NEXT_STATE1;

	radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_CHANGE_STATE, Pro2Cmd );
}


void QAPRSSi4463::init(int8_t unused1, int8_t unused2) {
	QAPRSBase::init(0,0);
	this->setTxDelay(50);
}

void QAPRSSi4463::setToneSendTime() {
	this->_toneSendTime = this->toneSendTime;
}

void QAPRSSi4463::togglePin() {
	if (PORTD & _BV(PIND7)){
		PORTD &= ~_BV(PIND7);
	} else {
		PORTD |= _BV(PIND7);
	}
}

uint8_t QAPRSSi4463::canTransmit() {
	uint8_t is_free = 0;
	while(!is_free){
		this->si446x_start_rx();
		delay(100);
		si446x_get_modem_status();
		if (rssi < 0x20){
			is_free = 1;
		}
	}
	return 1;
}

void QAPRSSi4463::setVariant(QAPRSVariant variant){
	QAPRSBase::setVariant(QAPRSVHF);
	this->_toneSendTime = this->toneSendTime;
}

/**
 * Ustaw wartosc pinu kluczującego przesunięcie częstotliwosci modułu
 * @warning ze względów czasowych ustawiam pin bezposrednio a nie digitalRead/Write PIND7 == this->tonePin
 */
void QAPRSSi4463::timerInterruptHandler() {
	// PIND7 == this->tonePin

	/*
	 * Ustaw czas następnego przepełnienia
	 */
/*	TCNT1 = this->timer1StartValue;*/
	this->togglePin();
	TCNT1 = this->timer1StartValue;
}
