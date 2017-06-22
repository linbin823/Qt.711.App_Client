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
// File:         M25P16_commands.h
// Date:         November 10, 2005
// Created by:   Bryan Fletcher
// Description:  Definitions for M25P16_commands.c
//               
// Notes:        Tested using the Xilinx Spartan-3E Starter Board
//***************************************************************************//

// PROTOTYPES
void spi_transfer (unsigned char *send, unsigned char *recv, unsigned char num_bytes);
void poll_until_complete (Xuint32 BaseAddress);
Xuint8 M25P16_read_status_register (Xuint32 BaseAddress);
void M25P16_write_enable (Xuint32 BaseAddress);
void M25P16_bulk_erase (Xuint32 BaseAddress);
void M25P16_sector_erase (Xuint32 BaseAddress, Xuint8 sector_address);
void M25P16_start_page_program (Xuint32 BaseAddress, Xuint8 sector_address, Xuint8 page_address, \
                                Xuint8 page_offset);
void M25P16_end_page_program (Xuint32 BaseAddress);
void M25P16_start_read (Xuint32 BaseAddress, Xuint8 sector_address, Xuint8 page_address, \
                        Xuint8 page_offset, Xuint8 speed_setting);
void M25P16_end_read (Xuint32 BaseAddress);


// DEFINITIONS
#define  M25P16_SECTORS           32
#define  M25P16_PAGES_PER_SECTOR  256
#define  M25P16_BYTES_PER_PAGE    256
#define  M25P16_BYTES             2097152

// Slave Select Masks
#define  SPI_NONE_SELECT  0xF
#define  SPI_FLASH_SELECT 0xE
#define  SPI_DAC_SELECT   0xD
#define  SPI_AMP_SELECT   0xB

// M25P16 One-byte Op-Codes
#define  WREN    0x06  // Write Enable
#define  WRDI    0x04  // Write Disable
#define  RDID    0x9F  // Read Identification
#define  RDSR    0x05  // Read Status Register
#define  WRSR    0x01  // Write Status Register
#define  READ    0x03  // Read Data Bytes
#define  FAST    0x0B  // Read Data Bytes at Higher Speed
#define  PP      0x02  // Page Program
#define  SE      0xD8  // Sector Erase
#define  BE      0xC7  // Bulk Erase
#define  DP      0xB9  // Deep Power-down
#define  RES     0xAB  // Read 8-bit Electronic Signature and/or Release from Deep power-down

// M25P16 Status Register masks
#define WRITE_IN_PROGRESS_MASK  0x01

// M25P16 Initial Control Reg
#define  INIT_CREG  (XSP_CR_TRANS_INHIBIT_MASK | XSP_CR_MANUAL_SS_MASK | XSP_CR_MASTER_MODE_MASK)

#define  DUMMY_BYTE 0xAA


//==============================================================================
// Function : Initialize_Spi_Controller(BaseAddress)
//
// Description : Initialize the SPI Controller
//
// Inputs:       BaseAddress
//
//==============================================================================

#define Initialize_Spi_Controller(BaseAddress) \
{ \
  XSpi_mSetControlReg(BaseAddress, INIT_CREG); \
  XSpi_mSetSlaveSelectReg(BaseAddress, SPI_NONE_SELECT); \
}


//==============================================================================
// Function : XSpi_Set_Inhibit(BaseAddress)
//
// Description : Set the transmit inhibit bit
//
// Inputs:       BaseAddress
//
//==============================================================================

#define XSpi_Set_Inhibit(BaseAddress) \
{ \
    Xuint16 Control; \
    Control = XSpi_mGetControlReg((BaseAddress)); \
    Control |= XSP_CR_TRANS_INHIBIT_MASK; \
    XSpi_mSetControlReg((BaseAddress), Control); \
}

//==============================================================================
// Function : XSpi_Clear_Inhibit(BaseAddress)
//
// Description : Set the transmit inhibit bit
//
// Inputs:       BaseAddress
//
//==============================================================================

#define XSpi_Clear_Inhibit(BaseAddress) \
{ \
    Xuint16 Control; \
    Control = XSpi_mGetControlReg((BaseAddress)); \
    Control &= ~XSP_CR_TRANS_INHIBIT_MASK; \
    XSpi_mSetControlReg((BaseAddress), Control); \
}



