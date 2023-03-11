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

#ifndef QAPRSSI4463_H_
#define QAPRSSI4463_H_
#include "QAPRSBase.h"
#include <SPI.h>
#include "Si4463_radio_config.h"

/**
 * @todo Zaimplementować obsługę ustawiania wariantów dla tej wersji.
 */

class QAPRSSi4463: public QAPRSBase {
private:
	static const uint8_t nselPin = A0;
	static const uint8_t powerPin = A2;
	static const uint8_t tonePin = 7;

	static const uint8_t SI446X_CMD_ID_GET_INT_STATUS = 0x20;
	static const uint8_t SI446X_CMD_ARG_COUNT_GET_INT_STATUS = 4;
	static const uint8_t SI446X_CMD_REPLY_COUNT_GET_INT_STATUS = 8;

	static const uint8_t SI446X_CMD_ID_START_TX = 0x31;
	static const uint8_t SI446X_CMD_ARG_COUNT_START_TX = 6;

	static const uint8_t SI446X_CMD_ID_CHANGE_STATE = 0x34;
	static const uint8_t SI446X_CMD_ARG_COUNT_CHANGE_STATE = 2;
	static const uint8_t SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP = 1;
	static const uint8_t SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY = 3;

	static const uint8_t SI446X_CMD_ID_FRR_A_READ = 0x50;
	static const uint8_t SI446X_CMD_ARG_COUNT_FRR_A_READ = 1;
	static const uint8_t SI446X_CMD_REPLY_COUNT_FRR_A_READ = 3;

	static const uint8_t SI446X_CMD_ID_START_RX = 0x32;
	static const uint8_t SI446X_CMD_ARG_COUNT_START_RX = 8;

	static const uint8_t SI446X_CMD_ID_GET_MODEM_STATUS = 0x22;
	static const uint8_t SI446X_CMD_ARG_COUNT_GET_MODEM_STATUS = 1;
	static const uint8_t SI446X_CMD_REPLY_COUNT_GET_MODEM_STATUS = 8;

	/**
	 * @brief Stała czasowa dla timera dla Mark
	 */
	static const uint16_t toneMarkTime =  3690;

	/**
	 * @brief Stała czasowa dla timera dla Space
	 */
	static const uint16_t toneSpaceTime =  6429;

	volatile uint16_t timer1StartValue;
	uint8_t Pro2Cmd[16];
	uint8_t ctsWentHigh;

	uint8_t toneChange;

	void Si4463_resetRadio();
	void Si4463_shutdownRadio();
	void Si4463_enableRadio();
	void initSPI();
	void initControlPins();
	uint8_t si446x_configuration_init(const uint8_t* pSetPropCmd);
	void si446x_get_int_status(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND);
	void si446x_start_tx();
	void si446x_start_rx();
	void si446x_change_state(uint8_t NEXT_STATE1);
	void si446x_frr_a_read(uint8_t respByteCount, uint8_t* pData);
	void si446x_get_modem_status();

	uint8_t radio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData, uint8_t respByteCount, uint8_t* pRespData);
	void radio_comm_SendCmd(uint8_t byteCount, uint8_t* pData);
	uint8_t radio_comm_PollCTS(void);
	uint8_t radio_comm_GetResp(uint8_t byteCount, uint8_t* pData);
	void radio_comm_ReadData(uint8_t cmd, uint8_t byteCount, uint8_t* pData);

	void radio_hal_ClearNsel(void);
	void radio_hal_SetNsel(void);
	void radio_hal_SpiWriteByte(uint8_t byteToWrite);
	uint8_t radio_hal_SpiReadByte(void);
	void radio_hal_SpiReadData(uint8_t byteCount, uint8_t* pData);
	void radio_hal_SpiWriteData(uint8_t byteCount, uint8_t* pData);

	void togglePin();
	virtual uint8_t canTransmit();
protected:
	/**
	 * @brief Czas wysyłania podedynczego tonu. W ms.
	 * @details Czas calkowity powinien wynosic 833ms. Wartosć podana tutaj uwzględnia zwłokę związaną z wywoływaniem
	 * funkcji itp.
	 */
	static const uint16_t toneSendTime = 819;
	virtual void initializeRadio();
	virtual void enableTransmission();
	virtual void disableTranssmision();

	virtual void toggleTone();
	virtual void setToneSendTime();
public:
	uint8_t rssi;
	void init(int8_t unused1, int8_t unused2);
	using QAPRSBase::init;
	virtual void timerInterruptHandler();
	void setVariant(QAPRSVariant variant);
};

#endif /* QAPRSSI4463_H_ */
