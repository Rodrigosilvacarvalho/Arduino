/*! @file Si4463_radio_config.h
 * @brief This file contains the automatically generated
 * configurations.
 *
 * @n WDS GUI Version: 3.2.2.0
 * @n Device: Si4463 Rev.: B1                                 
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2013 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_CONFIG_H_
#define RADIO_CONFIG_H_

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
//%%	Crys_freq(Hz)	Crys_tol(ppm)	IF_mode	High_perf_Ch_Fil	OSRtune	Ch_Fil_Bw_AFC	ANT_DIV	PM_pattern
// 	30000000	20	2	1	0	0	0	0
//%%	MOD_type	Rsymb(sps)	Fdev(Hz)	RXBW(Hz)	Mancheste	AFC_en	Rsymb_error	Chip-Version
// 	2	120000	6000	150000	1	0	0.0	2
//%%	RF Freq.(MHz)	API_TC	fhst	inputBW	BERT	RAW_dout	D_source	Hi_pfm_div
// 	144.8	29	1000	0	0	0	1	1
//
// # WB filter 8 (BW = 145.37 kHz);  NB-filter 8 (BW = 145.37 kHz) 

// 
// Modulation index: 0
*/


// CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ                     {30000000L}
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER                    {0x00}
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH               {0x00}
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP        {0x03}
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       {0xF000}


// CONFIGURATION COMMANDS

/*
// Command:                  RF_POWER_UP
// Description:              Power-up the device with the specified function. Power-up is complete when the CTS bit is set. This command may take longer to set the CTS bit than other commands.
*/
#define RF_POWER_UP 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80

/*
// Command:                  RF_GPIO_PIN_CFG
// Description:              Configures the gpio pins.
*/
#define RF_GPIO_PIN_CFG 0x13, 0x44, 0x26, 0x00, 0x27, 0x00, 0x00

/*
// Set properties:           RF_GLOBAL_XO_TUNE_1
// Number of properties:     1
// Group ID:                 00
// Start ID:                 00
// Default values:           0x40, 
// Descriptions:
//   GLOBAL_XO_TUNE - Crystal oscillator frequency tuning value. 0x00 is maximum frequency value and 0x7F is lowest frequency value. Each LSB code corresponds to a 70 fF capacitance change. The total adjustment range assuming a 30 MHz XTAL is +/-100 ppm.
*/
#define RF_GLOBAL_XO_TUNE_1 0x11, 0x00, 0x01, 0x00, 0x52

/*
// Set properties:           RF_GLOBAL_CONFIG_1
// Number of properties:     1
// Group ID:                 00
// Start ID:                 03
// Default values:           0x20, 
// Descriptions:
//   GLOBAL_CONFIG - Various settings that affect entire chip. If PROTOCOL is specified, the chip is placed into protocol aware state.
*/
#define RF_GLOBAL_CONFIG_1 0x11, 0x00, 0x01, 0x03, 0x40

/*
// Set properties:           RF_INT_CTL_ENABLE_1
// Number of properties:     1
// Group ID:                 01
// Start ID:                 00
// Default values:           0x04, 
// Descriptions:
//   INT_CTL_ENABLE - Enables top-level interrupt sources to generate HW interrupts at the NIRQ pin. The three interrupt groups are Chip, Modem and Packet Handler. Each of them contains multiple possible interrupt sources that must be individually enabled via the INT_CTL_PH_ENABLE, INT_CTL_MODEM_ENABLE, INT_CTL_CHIP_ENABLE properties. Note that this property only provides for global enabling/disabling of the HW interrupt indication on the NIRQ output pin. An internal interrupt event may still be generated even if the indication on the NIRQ output pin is disabled. The interrupt event may always be monitored by polling a GPIO pin, or via GET_INT_STATUS, GET_CHIP_STATUS, GET_PH_STATUS, or GET_MODEM_STATUS commands.
*/
#define RF_INT_CTL_ENABLE_1 0x11, 0x01, 0x01, 0x00, 0x00

/*
// Set properties:           RF_FRR_CTL_A_MODE_4
// Number of properties:     4
// Group ID:                 02
// Start ID:                 00
// Default values:           0x01, 0x02, 0x09, 0x00, 
// Descriptions:
//   FRR_CTL_A_MODE - Set the data that is present in fast response register A.
//   FRR_CTL_B_MODE - Set the data that is present in fast response register B.
//   FRR_CTL_C_MODE - Set the data that is present in fast response register C.
//   FRR_CTL_D_MODE - Set the data that is present in fast response register D.
*/
//#define RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
#define RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00

/*
// Set properties:           RF_MODEM_MOD_TYPE_12
// Number of properties:     12
// Group ID:                 20
// Start ID:                 00
// Default values:           0x02, 0x80, 0x07, 0x0F, 0x42, 0x40, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x06, 
// Descriptions:
//   MODEM_MOD_TYPE - This property configures the Modem in the chip for transmission/reception of the following modulation types: OOK, 2(G)FSK, and 4(G)FSK (if supported) modulation. In TX mode, this property is additionally used to select the source of the TX data stream, and whether that TX data stream is from a synchronous or asynchronous source (in Direct mode only). The Modem must be configured for transmission/reception of only one type of modulation throughout the entire packet; it is not possible (for example) to use OOK modulation over certain fields and (G)FSK modulation over other fields. If 4(G)FSK is supported, it is possible to switch between 2(G)FSK and 4(G)FSK on a field-specific basis; however, this is accomplished by configuring the Modem to 4(G)FSK mode for the entire packet and then performing special data processing in the Packet Handler.
//   MODEM_MAP_CONTROL - Modem Mapping Control.
//   MODEM_DSM_CTRL - Delta Sigma Modulator control.
//   MODEM_DATA_RATE_2 - Data rate, unsigned 24-bit, 100 kbps by default.
//   MODEM_DATA_RATE_1 - Data rate, unsigned 24-bit, 100 kbps by default.
//   MODEM_DATA_RATE_0 - Data rate, unsigned 24-bit, 100 kbps by default.
//   MODEM_TX_NCO_MODE_3 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
//   MODEM_TX_NCO_MODE_2 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
//   MODEM_TX_NCO_MODE_1 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
//   MODEM_TX_NCO_MODE_0 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
//   MODEM_FREQ_DEV_2 - The TX frequency deviation is specified by an unsigned 17-bit value. This property defines Byte 2 of the frequency deviation value. The units of the MODEM_FREQ_DEV_X property are in increments of the LSB tuning resolution of the PLL Synthesizer, and are therefore a function of the crystal frequency and output frequency band. The formula for calculating the value of the MODEM_FREQ_DEV property is given by the following equation: For 2(G)FSK mode, the specified value is the peak deviation. For 4(G)FSK mode (if supported), the specified value is the inner deviation (i.e., between channel center frequency and the nearest symbol deviation level). The MODEM_FREQ_DEV_X properties are only used in TX mode.
//   MODEM_FREQ_DEV_1 - The TX frequency deviation is specified by an unsigned 17-bit value. This property defines Byte 1 of the frequency deviation value. Please refer to the text description for MODEM_FREQ_DEV_2 for more details regarding TX frequency deviation.
*/
#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x0A, 0x80, 0x07, 0x01, 0xD4, 0xC0, 0x00, 0x2D, 0xC6, 0xC0, 0x00, 0x04

/*
// Set properties:           RF_MODEM_FREQ_DEV_0_1
// Number of properties:     1
// Group ID:                 20
// Start ID:                 0C
// Default values:           0xD3, 
// Descriptions:
//   MODEM_FREQ_DEV_0 - The TX frequency deviation is specified by an unsigned 17-bit value. This property defines Byte 0 of the frequency deviation value. Please refer to the text description for MODEM_FREQ_DEV_2 for more details regarding TX frequency deviation.
*/
#define RF_MODEM_FREQ_DEV_0_1 0x11, 0x20, 0x01, 0x0C, 0xEA

/*
// Set properties:           RF_MODEM_TX_RAMP_DELAY_8
// Number of properties:     8
// Group ID:                 20
// Start ID:                 18
// Default values:           0x01, 0x00, 0x08, 0x03, 0xC0, 0x00, 0x10, 0x20, 
// Descriptions:
//   MODEM_TX_RAMP_DELAY - Unit in TX bit cycle, it is calculated according to modulation type.
//   MODEM_MDM_CTRL - Detector selection and PM pattern configuration.
//   MODEM_IF_CONTROL - Specifies fixed-IF(468.75 kHz), zero-IF, or scalable IF.
//   MODEM_IF_FREQ_2 - Bits 16 an 17 of the 18-bit IF frequency. Specify fixed-IF(468.75kHz), zero-IF, or scalable IF.
//   MODEM_IF_FREQ_1 - IF frequency is 18-bit.
//   MODEM_IF_FREQ_0 - IF frequency is 18-bit.
//   MODEM_DECIMATION_CFG1 - Specifies the exponent of decimation ratios of the three CIC decimators in RX filter chain.
//   MODEM_DECIMATION_CFG0 - This property specifies the selection of the two polyphase filters preceding CIC filters, the droop compensation filter, the RX decimate by 8 2x filter and the channel filter power mode.
*/
#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08, 0x18, 0x01, 0x00, 0x08, 0x02, 0x80, 0x00, 0x00, 0x10

/*
// Set properties:           RF_MODEM_BCR_OSR_1_9
// Number of properties:     9
// Group ID:                 20
// Start ID:                 22
// Default values:           0x00, 0x4B, 0x06, 0xD3, 0xA0, 0x06, 0xD3, 0x02, 0xC0, 
// Descriptions:
//   MODEM_BCR_OSR_1 - High byte of RX symbol oversampling rate at BCR/Slicer (12-bit unsigned number).
//   MODEM_BCR_OSR_0 - Low byte of RX symbol oversampling rate at BCR/Slicer, total 12 bits.
//   MODEM_BCR_NCO_OFFSET_2 - Specifies the 6 MSBs of the BCR NCO offset.
//   MODEM_BCR_NCO_OFFSET_1 - Byte 1 of RX BCR NCO offset (an unsigned 22-bit number).
//   MODEM_BCR_NCO_OFFSET_0 - Low byte of RX NCO offset.
//   MODEM_BCR_GAIN_1 - High byte of clock recovery timing loop gain value.
//   MODEM_BCR_GAIN_0 - Low byte of clock recovery timing loop gain value.
//   MODEM_BCR_GEAR - RX BCR loop gear control. A reduced value of bit clock recovery gain is often desired after gear shifting (to reduce clock jitter). The BCR loop gain in both gear modes is obtained from a baseline clock recovery gain value (crgain in the MODEM_BCR_GAIN_x properties), scaled by the crfast and crslow values specified in this property. NOTE: larger values of crfast or crslow result in lower values of bit clock recovery gain.
//   MODEM_BCR_MISC1 - This property configures miscellaneous options within the RX BCR loop.
*/
#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x00, 0x53, 0x06, 0x24, 0xDD, 0x07, 0xFF, 0x02, 0x00

/*
// Set properties:           RF_MODEM_AFC_GEAR_7
// Number of properties:     7
// Group ID:                 20
// Start ID:                 2C
// Default values:           0x00, 0x23, 0x83, 0x69, 0x00, 0x40, 0xA0, 
// Descriptions:
//   MODEM_AFC_GEAR - Selection of gear switching source for both RX AFC and BCR. Control of gain for RX AFC in both FAST and SLOW modes (i.e., before and after gear switching)
//   MODEM_AFC_WAIT - Short and long wait periods after AFC correction.
//   MODEM_AFC_GAIN_1 - High byte of 13-bit AFC loop gain value. Enabling of AFC frequency error estimation. Enabling of dynamic IF bandwidth switching during the packet.
//   MODEM_AFC_GAIN_0 - Low byte of 13-bit AFC loop gain value
//   MODEM_AFC_LIMITER_1 - High byte of 15-bit AFC limiter value.
//   MODEM_AFC_LIMITER_0 - Low byte of 15-bit AFC limiter value.
//   MODEM_AFC_MISC - Specifies misc AFC control bits.
*/
#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x00, 0x23, 0x8F, 0xFF, 0x00, 0xB5, 0xA0

/*
// Set properties:           RF_MODEM_AGC_CONTROL_1
// Number of properties:     1
// Group ID:                 20
// Start ID:                 35
// Default values:           0xE0, 
// Descriptions:
//   MODEM_AGC_CONTROL - AGC control.
*/
#define RF_MODEM_AGC_CONTROL_1 0x11, 0x20, 0x01, 0x35, 0xE2

/*
// Set properties:           RF_MODEM_AGC_WINDOW_SIZE_9
// Number of properties:     9
// Group ID:                 20
// Start ID:                 38
// Default values:           0x11, 0x10, 0x10, 0x0B, 0x1C, 0x40, 0x00, 0x00, 0x2B, 
// Descriptions:
//   MODEM_AGC_WINDOW_SIZE - Sets PGA and LNA settling time window and measurement time window.
//   MODEM_AGC_RFPD_DECAY - Sets RF detector decay time.
//   MODEM_AGC_IFPD_DECAY - Sets IF detector decay time.
//   MODEM_FSK4_GAIN1 - Controls 4(G)FSK suppression gain.
//   MODEM_FSK4_GAIN0 - Controls 4(G)FSK suppression gain.
//   MODEM_FSK4_TH1 - High byte of 4(G)FSK slicer threshold.
//   MODEM_FSK4_TH0 - Low byte of 4(G)FSK slicer threshold.
//   MODEM_FSK4_MAP - 4(G)FSK symbol mapping code.
//   MODEM_OOK_PDTC - OOK peak detector decay and attack time.
*/
#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09, 0x38, 0x11, 0x12, 0x12, 0x00, 0x02, 0x06, 0x66, 0x00, 0x28

/*
// Set properties:           RF_MODEM_OOK_CNT1_8
// Number of properties:     8
// Group ID:                 20
// Start ID:                 42
// Default values:           0xA4, 0x03, 0x56, 0x02, 0x00, 0xA3, 0x02, 0x80, 
// Descriptions:
//   MODEM_OOK_CNT1 - OOK control.
//   MODEM_OOK_MISC - Control OOK Peak Detector.
//   MODEM_RAW_SEARCH - Searching period for high and low gear.
//   MODEM_RAW_CONTROL - Defines gain and enable controls for raw / nonstandard mode.
//   MODEM_RAW_EYE_1 - Eye-open detector threshold.
//   MODEM_RAW_EYE_0 - Eye-open detector threshold. The reset of bits are set by property RAW_EYE_1.
//   MODEM_ANT_DIV_MODE - Antenna diversity mode settings.
//   MODEM_ANT_DIV_CONTROL - Specifies pm detection threshold and GPIO configuration in antenna diversity mode.
//   MODEM_RSSI_THRESH - Selects the threshold for Clear Channel Assessment (CCA) and generation of the RSSI interrupt. If the Current RSSI value is above this threshold, a GPIO pin configured to output the CCA signal will be high. If the Current RSSI value crosses above this threshold, the RSSI interrupt will be generated. NOTE: the Latched RSSI value exceeding this threshold will not generate an RSSI interrupt. If enabled in the MODEM_RSSI_CONTROL property, the Latched RSSI value may also be compared against this threshold. The purpose of the comparison is to determine if the Latched RSSI value is below (not above) the threshold; if so, the chip proceeds to the specified START_RX:RXTIMEOUT_STATE and generates a PREAMBLE_INVALID interrupt.
//   MODEM_RSSI_JUMP_THRESH - RSSI jumping detection threshold, step in 1 dB.
//   MODEM_RSSI_CONTROL - Selects if the RSSI value is latched, and at what point in the packet it is latched. The Latched RSSI value may be read by a Fast Response Register, or returned by the GET_MODEM_STATUS command. Selects whether the RSSI value is updated every 1*Tb bit period, or whether the RSSI value is averaged over the previous 4*Tb bit periods. Selects if the Latched RSSI value is compared against the MODEM_RSSI_THRESH value, for the purpose of exiting to the RXTIMEOUT_STATE if below threshold.
*/
#define RF_MODEM_OOK_CNT1_8  0x11, 0x20, 0x08, 0x42, 0x84, 0x03, 0xD6, 0x03, 0x00, 0x26, 0x01, 0x80

/*
// Set properties:           RF_MODEM_RSSI_COMP_1
// Number of properties:     1
// Group ID:                 20
// Start ID:                 4E
// Default values:           0x32, 
// Descriptions:
//   MODEM_RSSI_COMP - Offsets RSSI curve in 1 dB steps. 32 is no offset, lower will adjust RSSI down, and higher will adjust RSSI up.
*/
#define RF_MODEM_RSSI_COMP_1 0x11, 0x20, 0x01, 0x4E, 0x40

/*
// Set properties:           RF_MODEM_CLKGEN_BAND_1
// Number of properties:     1
// Group ID:                 20
// Start ID:                 51
// Default values:           0x08, 
// Descriptions:
//   MODEM_CLKGEN_BAND - Selects the divide ratio of the configurable divider at the output of the PLL Synthesizer as a function of the desired operating frequency band. Configures the PLL Synthesizer for High Performance or Low Power operating mode, allowing a tradeoff between tuning resolution and current consumption.
*/
#define RF_MODEM_CLKGEN_BAND_1 0x11, 0x20, 0x01, 0x51, 0x0D

/*
// Set properties:           RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12
// Number of properties:     12
// Group ID:                 21
// Start ID:                 00
// Default values:           0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01, 
// Descriptions:
//   MODEM_CHFLT_RX1_CHFLT_COE13_7_0 - The chip implements the RX channel selection bandpass filtering in the digital domain as an FIR filter. The MODEM_CHFLT_RX_CHFLT_COEXX properties define the values for the filter tap coefficients. The chip provides for two different sets of RX filter coefficients (MODEM_CHFLT_RX1 and MODEM_CHFLT_RX2). These properties define the values for the first set of filter coefficients; see also the text description for the MODEM_CHFLT_RX2_CHFLT_COEXX properties for a discussion of the second set of filter coefficients and use of adaptive RX filter bandwidth across the packet. By default, the digital filter is 27 taps in length. However, it is possible to configure the RX Modem for a channel selection filter with only 15 taps; the advantage of the 15-tap filter is a reduction in filter processing delay at the expense of reduced filtering performance (e.g., adjacent channel selectivity). Please contact Silicon Labs for assistance with configuring the reduced performance filter. The values of the tap coefficients are symmetrical; that is, the value of COEFF26=COEFF0, COEFF25=COEFF1, and so on. Thus it is only necessary to store 14 filter coefficients; the internal circuitry obtains the remaining coefficients through symmetry. Each filter tap coefficient is a 10-bit signed value. The lower 8-bits of each coefficient are held in the MODEM_CHFLT_RX1_CHFLT_COEXX properties; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEMXX properties. Silicon Labs has pre-calculated 15 different sets of filter tap coefficients. The WDS Calculator will recommend one of these filter sets, based upon the RX filter bandwidth required to receive the desired signal. The filter bandwidth is a function of both the selected filter set, as well as the filter clock decimation ratio (see the MODEM_DECIMATION_CFG1/0 properties). This property sets the lower 8-bits of the 13th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property.
//   MODEM_CHFLT_RX1_CHFLT_COE12_7_0 - This property sets the lower 8-bits of the 12th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE11_7_0 - This property sets the lower 8-bits of the 11th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE10_7_0 - This property sets the lower 8-bits of the 10th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE9_7_0 - This property sets the lower 8-bits of the 9th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE8_7_0 - This property sets the lower 8-bits of the 8th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE7_7_0 - This property sets the lower 8-bits of the 7th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE6_7_0 - This property sets the lower 8-bits of the 6th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE5_7_0 - This property sets the lower 8-bits of the 5th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE4_7_0 - This property sets the lower 8-bits of the 4th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE3_7_0 - This property sets the lower 8-bits of the 3rd filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE2_7_0 - This property sets the lower 8-bits of the 2nd filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
*/
#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0x23, 0x17, 0xF4, 0xC2, 0x88, 0x50, 0x21, 0xFF, 0xEC, 0xE6, 0xE8, 0xEE

/*
// Set properties:           RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12
// Number of properties:     12
// Group ID:                 21
// Start ID:                 0C
// Default values:           0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 
// Descriptions:
//   MODEM_CHFLT_RX1_CHFLT_COE1_7_0 - This property sets the lower 8-bits of the 1st filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE0_7_0 - This property sets the lower 8-bits of the 0th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM0 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 13th through 10th filter coefficients for the first set of filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM1 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 9th through 6th filter coefficients for the first set of filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM2 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 5th through 2nd filter coefficients for the first set of filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM3 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 1st through 0th filter coefficients for the first set of filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE13_7_0 - The chip provides for two different sets of RX filter coefficients (MODEM_CHFLT_RX1 and MODEM_CHFLT_RX2). These properties define the values for the second set of filter coefficients; see also the text description for the MODEM_CHFLT_RX1_CHFLT_COEXX properties for a general discussion of the configuration of filter tap coefficients. Two different sets of filter tap coefficients (i.e., filter bandwidths) are provided to allow narrowing the RX bandwidth after the PLL-based AFC algorithm has acquired the signal and centered it in the filter passband. With no residual frequency error, the filter bandwidth may be reduced to pass only the modulation bandwidth of the signal, thus improving sensitivity over the remainder of the packet. This adaptive RX bandwidth feature is only applicable when PLL AFC is enabled (by setting MODEM_AFC_MISC:ENFBPLL), and the adaptive RX bandwidth function is enabled (by setting MODEM_AFC_GAIN_1:AFCBD). When the adaptive RX bandwidth feature is enabled, the first set of filter coefficients (defined in the MODEM_CHFLT_RX1 properties) is effective prior to gear-switching (e.g., detection of PREAMBLE_VALID) while the second set of filter coefficients (defined in the MODEM_CHFLT_RX2 properties) is effective after gear-switching. When the adaptive RX bandwidth feature is disabled, the first set of filter coefficients is used for the entire packet. This property sets the lower 8-bits of the 13th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property.
//   MODEM_CHFLT_RX2_CHFLT_COE12_7_0 - This property sets the lower 8-bits of the 12th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE11_7_0 - This property sets the lower 8-bits of the 11th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE10_7_0 - This property sets the lower 8-bits of the 10th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE9_7_0 - This property sets the lower 8-bits of the 9th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_1 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE8_7_0 - This property sets the lower 8-bits of the 8th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
*/
#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0xF6, 0xFB, 0x05, 0xC0, 0xFF, 0x0F, 0x23, 0x17, 0xF4, 0xC2, 0x88, 0x50

/*
// Set properties:           RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12
// Number of properties:     12
// Group ID:                 21
// Start ID:                 18
// Default values:           0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 
// Descriptions:
//   MODEM_CHFLT_RX2_CHFLT_COE7_7_0 - This property sets the lower 8-bits of the 7th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_1 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE6_7_0 - This property sets the lower 8-bits of the 6th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_1 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE5_7_0 - This property sets the lower 8-bits of the 5th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE4_7_0 - This property sets the lower 8-bits of the 4th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE3_7_0 - This property sets the lower 8-bits of the 3rd filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE2_7_0 - This property sets the lower 8-bits of the 2nd filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE1_7_0 - This property sets the lower 8-bits of the 1st filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE0_7_0 - This property sets the lower 8-bits of the 0th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM0 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 13th through 10th filter coefficients for the second set of filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM1 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 9th through 6th filter coefficients for the second set of filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM2 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 5th through 2nd filter coefficients for the second set of filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM3 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 1st through 0th filter coefficients for the second set of filter coefficients.
*/
#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0x21, 0xFF, 0xEC, 0xE6, 0xE8, 0xEE, 0xF6, 0xFB, 0x05, 0xC0, 0xFF, 0x0F

/*
// Set properties:           RF_PA_MODE_4
// Number of properties:     4
// Group ID:                 22
// Start ID:                 00
// Default values:           0x08, 0x7F, 0x00, 0x5D, 
// Descriptions:
//   PA_MODE - Specify PA mode and HPA/MPA groups.
//   PA_PWR_LVL - Set the PA output power level with higher value as larger output power.
//   PA_BIAS_CLKDUTY - 
//   PA_TC - Configuration control for PA power ramping in order to minimize switching spectrum noise. In (G)FSK mode, the values of TC and FSK_MOD_DLY should be programmed together so data modulation only occurs after the PA power ramping has been completed.
*/
#define RF_PA_MODE_4 0x11, 0x22, 0x04, 0x00, 0x08, 0x7F, 0x00, 0x3D

/*
// Set properties:           RF_SYNTH_PFDCP_CPFF_7
// Number of properties:     7
// Group ID:                 23
// Start ID:                 00
// Default values:           0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03, 
// Descriptions:
//   SYNTH_PFDCP_CPFF - The charge pump and loop filter in the PLL Synthesizer have two paths: an integral path and a feed-forward path. This property adjusts the charge pump gain (i.e., current) for the feed-forward path. The coding of the MSB (bit 5) of the CP_FF_CUR field is inverted. As a result, the minimum charge pump gain setting is obtained for a value of CP_FF_CUR=0x20, while CP_FF_CUR=0x00 corresponds to a mid-range charge pump gain setting and 0x1F corresponds to the maximum charge pump gain setting. The step size or resolution of the charge pump feed-forward current is in increments of 5 uA. Thus the range of values provided by the CP_FF_CUR field is from 0 uA to 315 uA. CP_FF_CUR_TEST is a test bit and is not recommended for general customer applications. The CP_FF_CUR_TEST bit is not binary-weighted with the CP_FF_CUR field but instead provides an additional 160 uA, when set. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
//   SYNTH_PFDCP_CPINT - The charge pump and loop filter in the PLL Synthesizer have two paths: an integral path and a feed-forward path. This property adjusts the charge pump gain (i.e., current) for the integral path. The coding of the MSB (bit 3) of the CP_INT_CUR field is inverted. As a result, the minimum charge pump gain setting is obtained for a value of CP_INT_CUR=0x08, while CP_INT_CUR=0x00 corresponds to a mid-range charge pump gain setting and 0x07 corresponds to the maximum charge pump gain setting. The step size or resolution of the charge pump feed-forward current is in increments of 5 uA. Thus the range of values provided by the CP_INT_CUR field is from 0 uA to 75 uA. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
//   SYNTH_VCO_KV - The charge pump, loop filter, and VCO tuning varactors in the PLL Synthesizer have two paths: an integral path and a feed-forward path. This property adjusts the gain scaling factors (Kv) for the tuning varactors in both the integral path and the feed-forward path. The adjustment of the gain factors is accomplished internally by switching in different number of varactor devices. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
//   SYNTH_LPFILT3 - The loop filter in the feed-forward path contains three lowpass filter poles: R1-C1, R2-C2, and R3-C3. The LPF_FF_R2 field adjusts the value of resistor R2, and thus adjusts the cutoff frequency of the R2-C2 lowpass filter pole. Additionally, the resistor R2 affects the DC gain of the transistor Gm stage in the feed-forward path. Increasing the value of R2 has the effect of increasing the feed-forward path gain. The value of R2 is in increments of 18 kOhm, ranging from a minimum value of 18 kOhm to a maximum of 144 kOhm. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
//   SYNTH_LPFILT2 - The loop filter in the feed-forward path contains three lowpass filter poles: R1-C1, R2-C2, and R3-C3. The LPF_FF_C2 field adjusts the value of capacitor C2, and thus adjusts the cutoff frequency of the R2-C2 lowpass filter pole. The value of C2 is in increments of 335 fF, ranging from a minimum value of 877 fF to a maximum of 11.25 pF. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
//   SYNTH_LPFILT1 - The loop filter in the feed-forward path contains three lowpass filter poles: R1-C1, R2-C2, and R3-C3. The LPF_FF_C3 field adjusts the value of capacitor C3, and thus adjusts the cutoff frequency of the R3-C3 lowpass filter pole. The value of resistor R3 is not adjustable and is fixed at R3=3.94 kOhm. The value of C3 is in increments of 1 pF, ranging from a minimum value of 9 pF to a maximum of 12 pF. The total value of capacitor C1 is determined by a 3-bit binary-weighted field LPF_FF_C1 in increments of 380 fF, and a 2-bit offset field LPF_FF_C1_CODE in increments of 1 pF, plus an additional fixed capacitance of 4.55 pF. Thus the total value of capacitor C1 may be described by the following equation: C1 = 4.55pF + 380fF*LPF_FF_C1 + 1pF*LPF_FF_C1_CODE The value of resistor R1 is not adjustable and is fixed at R1=9.86 kOhm. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
//   SYNTH_LPFILT0 - Bias current of the loop filter, 25 uA, 34 uA, 50 uA or 100 uA.
*/
#define RF_SYNTH_PFDCP_CPFF_7 0x11, 0x23, 0x07, 0x00, 0x34, 0x04, 0x0B, 0x04, 0x07, 0x70, 0x03

/*
// Set properties:           RF_FREQ_CONTROL_INTE_8
// Number of properties:     8
// Group ID:                 40
// Start ID:                 00
// Default values:           0x3C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF, 
// Descriptions:
//   FREQ_CONTROL_INTE - The total divide ratio for the Fractional-N PLL Synthesizer consists of an integer portion and a fractional portion. This property defines the integer divide number; the fractional divide value is specified in properties FREQ_CONTROL_FRAC_2, FREQ_CONTROL_FRAC_1, and FREQ_CONTROL_FRAC_0. The formula for calculating RF channel frequency as a function of integer and fractional divide values is as follows: The output divider value OUTDIV is configured as a function of the desired frequency band, and is specified in property MODEM_CLKGEN_BAND:BAND. The entire FC_FRAC word is 20-bits in length, but the MSB should always be set to 1, and thus the term FC_FRAC/2^19 will always be between 1 and 2 in value. As a result, the integer term FC_INTE should be reduced by 1. Example: a total desired divide ratio of N = 60.135 should be implemented as FC_INTE = 59, FC_FRAC/2^19 = 1.135. Modifying this property will have no effect until the chip exits from TX or RX state, and then re-enters the state.
//   FREQ_CONTROL_FRAC_2 - Please refer to the text description for FREQ_CONTROL_INTE for more details regarding frequency calculation.
//   FREQ_CONTROL_FRAC_1 - Please refer to the text description for FREQ_CONTROL_INTE for more details regarding frequency calculation.
//   FREQ_CONTROL_FRAC_0 - Please refer to the text description for FREQ_CONTROL_INTE for more details regarding frequency calculation.
//   FREQ_CONTROL_CHANNEL_STEP_SIZE_1 - The EZ Frequency Programming method allows the user to control the frequency by selecting a desired channel number, given a base frequency and a channel step size. This property defines Byte 1 of the unsigned 16-bit channel step size value. The units of the FREQ_CONTROL_CHANNEL_STEP_SIZE_1/0 property are in increments of the LSB tuning resolution of the PLL Synthesizer, and are therefore a function of the crystal reference frequency and output frequency band. The formula for calculating the value of the FREQ_CONTROL_CHANNEL_STEP_SIZE property is given by the following equation: The base frequency is specified by the FREQ_CONTROL_INTE and FREQ_CONTROL_FRAC_2/1/0 properties. The channel number is specified by the CHANNEL parameter byte passed to the START_TX command. The commanded channel frequency is given by the formula:
//   FREQ_CONTROL_CHANNEL_STEP_SIZE_0 - The EZ Frequency Programming method allows the user to control the frequency by selecting a desired channel number, given a base frequency and a channel step size. This property defines Byte 0 of the unsigned 16-bit channel step size value. Please refer to the text description for FREQ_CONTROL_CHANNEL_STEP_SIZE_1 for more details regarding the channel step size function.
//   FREQ_CONTROL_W_SIZE - The chip performs a calibration of the VCO at each new commanded frequency. This is accomplished by counting cycles of the VCO frequency and comparing to an expected target count value. The length of the window during which cycles of the VCO frequency are counted is specified by this property. The units are in cycles of the crystal reference frequency (e.g., 30 MHz clock periods). This property does not need to change as a function of crystal reference frequency; the chip automatically calculates the target VCO count value as a function of crystal reference frequency and thus this property may remain constant. Silicon Labs recommends setting this property always to 0x20.
//   FREQ_CONTROL_VCOCNT_RX_ADJ - The chip performs a calibration of the VCO at each new commanded frequency. This is accomplished by counting cycles of the VCO frequency and comparing to an expected target count value. As the chip's default mode of operation uses a low-IF architecture with low-side Mixer injection, the RX LO must shift downwards in frequency during RX mode. The expected target count value changes as a result of this shift in frequency. This field is a signed value that represents the amount by which the target count value must be adjusted in RX mode.
*/
#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x38, 0x0F, 0x5C, 0x28, 0x00, 0xD2, 0x20, 0xFA


// AUTOMATICALLY GENERATED CODE! 
// DO NOT EDIT/MODIFY BELOW THIS LINE!
// --------------------------------------------

#ifndef FIRMWARE_LOAD_COMPILE
#define RADIO_CONFIGURATION_DATA_ARRAY { \
        0x07, RF_POWER_UP, \
        0x07, RF_GPIO_PIN_CFG, \
        0x05, RF_GLOBAL_XO_TUNE_1, \
        0x05, RF_GLOBAL_CONFIG_1, \
        0x05, RF_INT_CTL_ENABLE_1, \
        0x08, RF_FRR_CTL_A_MODE_4, \
        0x10, RF_MODEM_MOD_TYPE_12, \
        0x05, RF_MODEM_FREQ_DEV_0_1, \
        0x0C, RF_MODEM_TX_RAMP_DELAY_8, \
        0x0D, RF_MODEM_BCR_OSR_1_9, \
        0x0B, RF_MODEM_AFC_GEAR_7, \
        0x05, RF_MODEM_AGC_CONTROL_1, \
        0x0D, RF_MODEM_AGC_WINDOW_SIZE_9, \
        0x0C, RF_MODEM_OOK_CNT1_8, \
        0x05, RF_MODEM_RSSI_COMP_1, \
        0x05, RF_MODEM_CLKGEN_BAND_1, \
        0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \
        0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \
        0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \
        0x08, RF_PA_MODE_4, \
        0x0B, RF_SYNTH_PFDCP_CPFF_7, \
        0x0C, RF_FREQ_CONTROL_INTE_8, \
        0x00 \
 }
#else
#define RADIO_CONFIGURATION_DATA_ARRAY { 0 }
#endif

// DEFAULT VALUES FOR CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT                     30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT                    0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT               0x10
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT        0x01
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT       0x1000

#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_INCLUDED                      0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_SIZE                          0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH                               {  }

#ifndef RADIO_CONFIGURATION_DATA_ARRAY
#error "This property must be defined!"
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ         { RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER        { RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH   { RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP  { RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET { RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT }
#endif

#define RADIO_CONFIGURATION_DATA { \
                            Radio_Configuration_Data_Array,                            \
                            RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,                   \
                            RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH,              \
                            RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP,       \
                            RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       \
                            }

#endif /* RADIO_CONFIG_H_ */
