/*
 * RFM9x.c
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "LORA/RFM9x.h"
#include <string.h>
#include "LORA/LORACom.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* User code -----------------------------------------------------------------*/


void RFM9x_Init( void )
{
	// Assert Reset low on the RFM9x
	HAL_GPIO_WritePin(SPI1_RST_GPIO_Port, SPI1_RST_Pin, GPIO_PIN_RESET);
	Delay_ms(10);
	HAL_GPIO_WritePin(SPI1_RST_GPIO_Port, SPI1_RST_Pin, GPIO_PIN_SET);
	Delay_ms(10);

	// Set sleep mode, so we can also set RFM9x mode:
    RFM9x_WriteReg(RFM9x_REG_01_OP_MODE, RFM9x_MODE_SLEEP | RFM9x_LONG_RANGE_MODE);

    // Wait for sleep mode to take over from say, CAD
    Delay_ms(10);

    // Check we are in sleep mode, with RFM9x set
    if (RFM9x_ReadReg(RFM9x_REG_01_OP_MODE) != (RFM9x_MODE_SLEEP | RFM9x_LONG_RANGE_MODE))
    {
    	//	TODO: Throw RFM9x init error
    }

    // Either Rx or Tx can use the entire 256 byte FIFO, but not at same time
    RFM9x_WriteReg(RFM9x_REG_0E_FIFO_TX_BASE_ADDR, 0x00);
    RFM9x_WriteReg(RFM9x_REG_0F_FIFO_RX_BASE_ADDR, 0x80);

    // BW = 7: 125 kHz
    // CodingRate = 1:  4/5 code rate
    // ImplicitHeaderModeOn = 1, Implicit Header mode
    RFM9x_WriteReg(RFM9x_REG_1D_MODEM_CONFIG1, 0x72);

    // SpreadingFactor = 7: 128 chips / symbol,
    // TxContinuousMode = 0 : Normal mode: a single packet is sent
    // RxPayloadCrcOn = 1 : CRC enabled
    // SymbTimeout[9:8] = 0
   // RFM9x_WriteReg(RFM9x_REG_1E_MODEM_CONFIG2, 0x74);

    // LowDataRateOptimize = 1 : Enabled; mandated for when the symbol length exceeds 16ms
    // AgcAutoOn = 0 : LNA gain set by register LnaGain
    RFM9x_WriteReg(RFM9x_REG_26_MODEM_CONFIG3, 0x04);

    // Preamble Length = 16;
    RFM9x_WriteReg(RFM9x_REG_20_PREAMBLE_MSB, 0x00);
    RFM9x_WriteReg(RFM9x_REG_21_PREAMBLE_LSB, 0x10);

    // Set Frequency = 433 MHz
    //   FRF[23:0] = Freq / Fstep
    //   Fstep = Fxosc / 2^^19
    // where:
    //   Freq = 433 MHz
    //   Fxosc = 32 MHz
    uint32_t frf = (uint32_t) ( 433000000.0 / (32000000.0 / 524288.0) ) ;
    RFM9x_WriteReg(RFM9x_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    RFM9x_WriteReg(RFM9x_REG_07_FRF_MID, (frf >> 8) & 0xff);
    RFM9x_WriteReg(RFM9x_REG_08_FRF_LSB, frf & 0xff);

    // PaDac = 4 : Disables the +20dBm option on PA_BOOST pin
    RFM9x_WriteReg(RFM9x_REG_4D_PA_DAC, 0x04);

    // PaSelect = 1 : PA_BOOST pin (instead of RFO pin).
    // MaxPower = 0 : Pmax=10.8+0.6*MaxPower [dBm]
    // Output Power = 8 : 10dBm from Pout=17-(15-OutputPower) if PaSelect = 1. RadioHead says this is 13 dBm, though
    //RFM9x_WriteReg(RFM9x_REG_09_PA_CONFIG, 0x88);
    RFM9x_WriteReg(RFM9x_REG_09_PA_CONFIG, 0xcf);

    return;
}


void RFM9x_Send(const uint8_t* data, uint8_t len)
{
    //TODO: Chec (len > RFM9x_MAX_MESSAGE_LEN)

    //waitPacketSent(); // Make sure we dont interrupt an outgoing message
    //setModeIdle();

    //if (!waitCAD())
	// return false;  // Check channel activity

    // Position at the beginning of the FIFO
    RFM9x_WriteReg(RFM9x_REG_0D_FIFO_ADDR_PTR, 0);

    // The payload data
    for(int i=0; i < len; i++)
    {
    	RFM9x_WriteReg(RFM9x_REG_00_FIFO, data[i]);
    }
    uint8_t read_FIFO=RFM9x_ReadReg(RFM9x_REG_00_FIFO);
    LORA_debug("RFM9x_REG_00_FIFO", read_FIFO);
    // The message length
    RFM9x_WriteReg(RFM9x_REG_22_PAYLOAD_LENGTH, len);

    // Start the transmitter
    RFM9x_WriteReg(RFM9x_REG_01_OP_MODE, 0x03);

    // Interrupt on DIO0 for TxDone
    RFM9x_WriteReg(RFM9x_REG_40_DIO_MAPPING1, 0x40);

    LORA_debug("RFM9x SEND", data);
    return;
}

void RFM9x_Receive(uint8_t* data, uint8_t maxlen)
{
	LORA_debug("RxCurAddr", RFM9x_ReadReg(RFM9x_REG_10_FIFO_RX_CURRENT_ADDR));

	RFM9x_WriteReg(RFM9x_REG_01_OP_MODE, RFM9x_MODE_RXCONTINUOUS);

	// Set Interrupt on DIO0 to RxDone
	RFM9x_WriteReg(RFM9x_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone

	LORA_debug("WFI...", NULL);
	// wait for interrupt
	/*uint32_t start_time_ms = HAL_GetTick();
	while (! HAL_GPIO_ReadPin(SPI2_INT_GPIO_Port, SPI1_INT_Pin))
	{
		//spin wait

		//turn off the LED after 900 msec (pulse off 100ms in 1 sec Tx cycle)
		if( (HAL_GetTick() - start_time_ms) > 900)
		{
		}
	}*/

	// Read the interrupt register
	uint8_t irq_flags = RFM9x_ReadReg(RFM9x_REG_12_IRQ_FLAGS);
	LORA_debug("IRQ Flags", irq_flags);

	// Number of bytes received
	uint8_t start = RFM9x_ReadReg(RFM9x_REG_10_FIFO_RX_CURRENT_ADDR);
	uint8_t len = RFM9x_ReadReg(RFM9x_REG_13_RX_NB_BYTES);
	LORA_debug("RxCurAddr", start);
	LORA_debug("RxNbrBytes", len);

	// get the read data
	if (len > (maxlen-1)) len = maxlen-1;
	RFM9x_WriteReg(RFM9x_REG_0D_FIFO_ADDR_PTR, start);
	for (int i = 0; i < len; i++)
	{
		data[i] = RFM9x_ReadReg(RFM9x_REG_00_FIFO);
	}
	data[len] = '\0';

	// clear all the IRQ flags
	RFM9x_WriteReg( RFM9x_REG_12_IRQ_FLAGS, 0xFF );

	// Report the SNR and RSSI
	LORA_debug("SNR", RFM9x_ReadReg(RFM9x_REG_19_PKT_SNR_VALUE));
	LORA_debug("RSSI", RFM9x_ReadReg(RFM9x_REG_1A_PKT_RSSI_VALUE));

	LORA_debug("*Final data*",data);
	// if good CRC
	/*if ( (irq_flags & RFM9x_PAYLOAD_CRC_ERROR) == 0)
	{
		// turn on LED
		HAL_GPIO_WritePin(GRN_LED_GPIO_Port, GRN_LED_Pin, GPIO_PIN_SET);
		// Good beep
		beep(80,0);
	}
	else
	{
		// bad BEEEEP
		beep(160,3);
	}*/

}

uint8_t RFM9x_GetMode( void )
{
	uint8_t mode = RFM9x_ReadReg( RFM9x_REG_01_OP_MODE );
	return mode & 0x07;
}

void RFM9x_ClearInt( void )
{
	// set to clear all the IRQ bits
	RFM9x_WriteReg( RFM9x_REG_12_IRQ_FLAGS, 0xFF );
}

uint8_t RFM9x_ReadReg( uint8_t reg )
{
	HAL_StatusTypeDef status;

	// clear reg msb for read
	reg &= 0x7f;

	// buffers to transmit/receive
	uint8_t txData[] = {reg, 0x00};
	uint8_t rxData[] = {0x00, 0x00};
	const uint16_t	size = sizeof(txData);

	// default data value if error
	uint8_t data = 0x00;

	// Set CS low (active)
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	// write 8 bit reg and read 8 bit data
	status = HAL_SPI_TransmitReceive(hLORACom.hspi, txData, rxData, size, HAL_MAX_DELAY);

	if (status == HAL_OK)
	{
		//second byte is the register value
		data = rxData[1];
	}
	else
	{
		LORA_debug("*HAL_ERROR*", NULL);
		//HAL_UART_Transmit(&huart2, (uint8_t *) &"*HAL_ERROR*\r\n", 13, HAL_MAX_DELAY);
		// handle errors here
	}

	//print2("RFM9x RD", reg, data );

	// Set CS high (inactive)
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

	return data;
}

void RFM9x_WriteReg( uint8_t reg, uint8_t data )
{
	HAL_StatusTypeDef status;

	LORA_debug_val("RFM9x WR -reg", reg );
	LORA_debug_val("RFM9x WR -value", data );


	//set the reg msb for write
	reg |= 0x80;

	// Transmit buffer
	uint8_t txData[2] = {reg, data};
	const uint16_t size = sizeof(txData);


	// Set CS low (active)
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	// write 8 bit reg and read 8 bit data
	status = HAL_SPI_Transmit(hLORACom.hspi, txData, size, HAL_MAX_DELAY);

	if (status != HAL_OK)
	{
		HAL_UART_Transmit(hLORACom.huartDebug, (uint8_t *) &"*HAL_ERROR*\r\n", 13, HAL_MAX_DELAY);
		// handle errors here
	}

	//HACK: Wait for SPI transfer to complete
	Delay_ms(1);
	// Set CS high (inactive)
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void Delay_ms( uint32_t delay_ms )
{
	/**
	 * This should correctly handle SysTic roll-overs.
	 * See:
	 *   https://stackoverflow.com/questions/61443/rollover-safe-timer-tick-comparisons
	 */
	uint32_t start_time_ms = HAL_GetTick();
	while ( (HAL_GetTick() - start_time_ms) < delay_ms)
	{
		// spin wait
	}

	return;
}


