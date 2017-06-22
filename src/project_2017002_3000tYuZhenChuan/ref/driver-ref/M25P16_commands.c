//-----------------------------------------------------------------------------
//
//     AVNET IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
//     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
//     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
//     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
//     OR STANDARD, AVNET IS MAKING NO REPRESENTATION THAT THIS
//     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
//     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
//     FOR YOUR IMPLEMENTATION.  AVNET EXPRESSLY DISCLAIMS ANY
//     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
//     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
//     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
//     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//     FOR A PARTICULAR PURPOSE.
//     
//     (c) Copyright 2005 AVNET, Inc.
//     All rights reserved.
//
//-----------------------------------------------------------------------------

//***************************************************************************//
//
// File:         M25P16_commands.c
// Date:         November 10, 2005
// Created by:   Bryan Fletcher
// Description:  Routines for accessing STMicro M25P16 SPI Flash
//               
// Notes:        Tested using the Xilinx Spartan-3E Starter Board
//***************************************************************************//
                   
/************* INCLUDES *******************************************************/
#include "xparameters.h"
#include "xbasic_types.h"
#include "xspi_l.h"
#include "M25P16_commands.h"
/******************************************************************************/

//==============================================================================
// Function : spi_transfer (*send, *recv, num_bytes)
//
// Description : This function provides the hook into the SPI DRIVER transfer function.  
// Inputs:
//          *send     - pointer to 8-bit values to send
//          *recv     - pointer to 8-bit values to be received
//          num_bytes - the number of bytes to be transferred
//
//==============================================================================
void spi_transfer (unsigned char *send, unsigned char *recv, unsigned char num_bytes)
{
    unsigned char NumBytesRcvd = 0;
  unsigned char NumBytesSent = 0;   

    //  Fill up the transmitter with data, assuming the receiver can hold
    //  the same amount of data.
    //  while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_TX_FULL_MASK) == 0)
    while(num_bytes--)
  {
        XSpi_mSendByte(XPAR_SPI_FLASH_BASEADDR, *send++);
        NumBytesSent++;
    }

    // Enable the device
    XSpi_mEnable(XPAR_SPI_FLASH_BASEADDR);

    // Wait for the transmit FIFO to transition to empty before checking
    // the receive FIFO, this prevents a fast processor from seeing the
    // receive FIFO as empty
    while (!(XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_TX_EMPTY_MASK));

  // Receive the data from the RX fifo
    while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK) == 0)
    {
        *recv++ = XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
    NumBytesRcvd++;
    }

    /*
     * If no data was sent or the data that was sent was not received,
     * then return an error
     */
    if ((NumBytesSent != NumBytesRcvd) || (NumBytesSent == 0))
    {
      print("SPI XFER ERROR");
    }
}

//==============================================================================
// Function : poll_until_complete (Xuint32 BaseAddress)
//
// Description : This function polls the status register until the WRITE_IN_PROGRESS
//               bit is false.  
//
// Inputs:       32-bit base address of SPI controller
//
//==============================================================================

void poll_until_complete (Xuint32 BaseAddress)
{
  Xuint8 status_reg, op_code = RDSR;

  // Poll the M25P16 status register and check the WIP bit
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  spi_transfer(&op_code, &status_reg, 1);
  do
  {
    spi_transfer(0x00, &status_reg, 1);
    // xil_printf("Status Register   = : %x \n\r", status_reg);
  }
  while((status_reg & WRITE_IN_PROGRESS_MASK) == WRITE_IN_PROGRESS_MASK);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);

}

//==============================================================================
// Function : M25P16_read_status_register (Xuint32 BaseAddress)
//
// Description : This function reads and returns the M25P16 status register
//               
//
// Inputs:       32-bit base address of SPI Controller
//
// Returns:      8-bit contents of M25P16 status register
//
//==============================================================================

Xuint8 M25P16_read_status_register (Xuint32 BaseAddress)
{
  Xuint8 status_reg, op_code = RDSR;
  // Read the Status Register
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  spi_transfer(&op_code, &status_reg, 1);
  XSpi_mSetSlaveSelectReg(XPAR_SPI_FLASH_BASEADDR, SPI_NONE_SELECT);
  return status_reg;
}

//==============================================================================
// Function : M25P16_write_enable (Xuint32 BaseAddress)
//
// Description : This function turns on the write enable bit
//
// Inputs:       32-bit base address of SPI Controller
//
//==============================================================================

void M25P16_write_enable (Xuint32 BaseAddress)
{
  Xuint8 op_code = WREN, bogus_data;
  // Send the Write Enable op-code -- ignoring recv_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  spi_transfer(&op_code, &bogus_data, 1);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);
}

//==============================================================================
// Function : M25P16_bulk_erase (Xuint32 BaseAddress)
//
// Description : This function erases the entire M25P16 chip
//
// Inputs:       32-bit base address of SPI Controller
//
//==============================================================================

void M25P16_bulk_erase (Xuint32 BaseAddress)
{
  Xuint8 op_code = WREN, bogus_data;

  // Send the Write Enable op-code -- ignoring recv_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  spi_transfer(&op_code, &bogus_data, 1);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);

  // Send the Bulk Erase op-code -- ignoring recv_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  op_code = BE;
  spi_transfer(&op_code, &bogus_data, 1);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);

  poll_until_complete(BaseAddress);
}


//==============================================================================
// Function : M25P16_sector_erase (Xuint32 BaseAddress, Xuint8 sector_address)
//
// Description : This function erases the entire M25P16 chip
//
// Inputs:       32-bit base address of SPI Controller
//               8-bit sector address (0 to 31)
//
//==============================================================================

void M25P16_sector_erase (Xuint32 BaseAddress, Xuint8 sector_address)
{
  Xuint8 op_code[4], bogus_data[4];

  // Send the Write Enable op-code -- ignoring bogus_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  op_code[0] = WREN;
  spi_transfer(op_code, bogus_data, 1);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);

  // Send the Page Program op-code -- ignoring recv_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  op_code[0] = SE;
  op_code[1] = sector_address;
  op_code[2] = 0x00;
  op_code[3] = 0x00;
  spi_transfer(op_code, bogus_data, 4);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);

  poll_until_complete(BaseAddress);
}


//==============================================================================
// Function : M25P16_start_page_program (Xuint32 BaseAddress, Xuint8 sector_address, Xuint8 page_address, \
//                                       Xuint8 page_offset)
//
// Description : This function begins a page program (256 bytes max per page)
//
// Inputs:       32-bit base address of SPI Controller
//               8-bit sector address (0 to 31)
//               8-bit page address inside the sector (0 to 255)
//               8-bit offset within the page (0 to 255)
//
//==============================================================================

void M25P16_start_page_program (Xuint32 BaseAddress, Xuint8 sector_address, Xuint8 page_address, \
                                Xuint8 page_offset)
{
  Xuint8 op_code[4], bogus_data[4];

  // Send the Write Enable op-code -- ignoring recv_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  op_code[0] = WREN;
  spi_transfer(op_code, bogus_data, 1);
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);

  // Send the Page Program op-code -- ignoring recv_data
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  op_code[0] = PP;
  op_code[1] = sector_address;
  op_code[2] = page_address;
  op_code[3] = page_offset;
  spi_transfer(op_code, bogus_data, 4);
}


//==============================================================================
// Function : M25P16_end_page_program (Xuint32 BaseAddress)
//
// Description : This function ends the page program
//
// Inputs:       32-bit base address of SPI Controller
//
//==============================================================================

void M25P16_end_page_program (Xuint32 BaseAddress)
{
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);
  poll_until_complete(BaseAddress);
}


//==============================================================================
// Function : M25P16_start_read (Xuint32 BaseAddress, Xuint8 sector_address, Xuint8 page_address, \
//                               Xuint8 page_offset, Xuint8 speed_setting)
//
// Description : This function begins a read (unlimited bytes -- rolls over at M25P16_BYTES)
//
// Inputs:       32-bit base address of SPI Controller
//               8-bit sector address (0 to 31)
//               8-bit page address inside the sector (0 to 255)
//               8-bit offset within the page (0 to 255)
//               8-bit speed_setting: 1 if SCK > 20MHz, 0 if SCK <= 20MHz
//
//==============================================================================

void M25P16_start_read (Xuint32 BaseAddress, Xuint8 sector_address, Xuint8 page_address, \
                        Xuint8 page_offset, Xuint8 speed_setting)
{
  Xuint8 op_code[5], bogus_data[5];

  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_FLASH_SELECT);
  if(speed_setting)
    op_code[0] = FAST;
  else
    op_code[0] = READ;
  op_code[1] = sector_address;
  op_code[2] = page_address;
  op_code[3] = page_offset;
  op_code[4] = DUMMY_BYTE;
  if(speed_setting)
    spi_transfer(op_code, bogus_data, 5);
  else
    spi_transfer(op_code, bogus_data, 4);

}

//==============================================================================
// Function : M25P16_end_read (Xuint32 BaseAddress)
//
// Description : This function ends the read
//
// Inputs:       32-bit base address of SPI Controller
//
//==============================================================================

void M25P16_end_read (Xuint32 BaseAddress)
{
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT);
}

