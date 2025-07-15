/*******************************************************************************
 Simple SPI Transfer function

  File Name:
    drv_spi.c

  Summary:
    Initializes SPI 1. Transfers data over SPI.
    Uses SPI FIFO to speed up transfer.

  Description:
    .

  Remarks:

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 * Copyright (C) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END


#include "drv_spi.h"

#include <hardware/gpio.h>
#include <hardware/spi.h>

int8_t DRV_SPI_ChipSelectAssert(uint8_t spiSlaveDeviceIndex, bool assert) {
  int8_t error = 0;

  // Select Chip Select
  switch (spiSlaveDeviceIndex) {
  case DRV_CANFDSPI_INDEX_0:
    if (assert) {
      gpio_put(DRV_CANFDSPI_0_CS, 0);
    } else {
      gpio_put(DRV_CANFDSPI_0_CS, 1);
    }
    break;
  default:
    error = -1;
    break;
  }

  return error;
}

void DRV_SPI_Initialize() {
  /* Disable the SPI module to configure it*/
  spi_deinit(spi0);

  spi_init(spi0, DRV_CANFDSPI_SPEED);

  gpio_set_function(DRV_CANFDSPI_0_RX, GPIO_FUNC_SPI);
  gpio_set_function(DRV_CANFDSPI_0_TX, GPIO_FUNC_SPI);
  gpio_set_function(DRV_CANFDSPI_0_SCK, GPIO_FUNC_SPI);

  gpio_init(DRV_CANFDSPI_0_CS);
  gpio_set_dir(DRV_CANFDSPI_0_CS, GPIO_OUT);
  return;
}

int8_t DRV_SPI_TransferData(uint8_t spiSlaveDeviceIndex, uint8_t *SpiTxData,
                            uint8_t *SpiRxData, uint16_t spiTransferSize) {
  int8_t error = 0;

  // Assert CS
  error = DRV_SPI_ChipSelectAssert(spiSlaveDeviceIndex, true);
  if (error != 0)
    return error;

  switch (spiSlaveDeviceIndex) {
  case DRV_CANFDSPI_INDEX_0:
    spi_write_read_blocking(spi0, SpiTxData, SpiRxData, spiTransferSize);
    break;

  default:
    error = -1;
  }

  // De-assert CS
  error = DRV_SPI_ChipSelectAssert(spiSlaveDeviceIndex, false);

  return error;
}
