#include "MCP2515.h"
#include "main.h"
/* Pin ・､・菩乱 ・樓ｲ・・們倣符・・ Modify below items for your SPI configurations */
extern SPI_HandleTypeDef        hspi2;
#define SPI_CAN                 &hspi2
#define SPI_TIMEOUT             10
#define MCP2515_CS_HIGH()   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET)
#define MCP2515_CS_LOW()    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET)

/* Prototypes */
static void SPI_Tx(uint8_t data);
static void SPI_TxBuffer(uint8_t *buffer, uint8_t length);
static uint8_t SPI_Rx(void);
static void SPI_RxBuffer(uint8_t *buffer, uint8_t length);

/* MCP2515 ・一ｸｰ嶹・*/
bool MCP2515_Initialize(void)
{
  MCP2515_CS_HIGH();

  uint8_t loop = 10;

  do {
    /* SPI Ready 嶹菩攤 */
    if(HAL_SPI_GetState(SPI_CAN) == HAL_SPI_STATE_READY)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* MCP2515 ・ｼ ・､・簿ｪｨ・罹｡・・・劍 */
bool MCP2515_SetConfigMode(void)
{
  /* CANCTRL Register Configuration ・ｨ・・・､・・*/
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x80);

  uint8_t loop = 10;

  do {
    /* ・ｨ・懍・劍 嶹菩攤 */
    if((MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0) == 0x80)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* MCP2515 ・ｼ Normal・ｨ・罹｡・・・劍 */
bool MCP2515_SetNormalMode(void)
{
  /* CANCTRL Register Normal ・ｨ・・・､・・*/
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x00);

  uint8_t loop = 10;

  do {
    /* ・ｨ・懍・劍 嶹菩攤 */
    if((MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0) == 0x00)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* MCP2515 ・ｼ Sleep ・ｨ・罹｡・・・劍 */
bool MCP2515_SetSleepMode(void)
{
  /* CANCTRL Register Sleep ・ｨ・・・､・・*/
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x20);

  uint8_t loop = 10;

  do {
    /* ・ｨ・懍・劍 嶹菩攤 */
    if((MCP2515_ReadByte(MCP2515_CANSTAT) & 0xE0) == 0x20)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* MCP2515 SPI-Reset */
void MCP2515_Reset(void)
{
  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_RESET);

  MCP2515_CS_HIGH();
}

/* 1・肥擽孖ｸ ・ｽ・ｰ */
uint8_t MCP2515_ReadByte (uint8_t address)
{
  uint8_t retVal;

  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_READ);
  SPI_Tx(address);
  retVal = SPI_Rx();

  MCP2515_CS_HIGH();

  return retVal;
}

/* Sequential Bytes ・ｽ・ｰ */
void MCP2515_ReadRxSequence(uint8_t instruction, uint8_t *data, uint8_t length)
{
  MCP2515_CS_LOW();

  SPI_Tx(instruction);
  SPI_RxBuffer(data, length);

  MCP2515_CS_HIGH();
}

/* 1・肥擽孖ｸ ・ｰ・ｰ */
void MCP2515_WriteByte(uint8_t address, uint8_t data)
{
  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_WRITE);
  SPI_Tx(address);
  SPI_Tx(data);

  MCP2515_CS_HIGH();
}

/* Sequential Bytes ・ｰ・ｰ */
void MCP2515_WriteByteSequence(uint8_t startAddress, uint8_t endAddress, uint8_t *data)
{
  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_WRITE);
  SPI_Tx(startAddress);
  SPI_TxBuffer(data, (endAddress - startAddress + 1));

  MCP2515_CS_HIGH();
}

/* TxBuffer・・Sequential Bytes ・ｰ・ｰ */
void MCP2515_LoadTxSequence(uint8_t instruction, uint8_t *idReg, uint8_t dlc, uint8_t *data)
{
  MCP2515_CS_LOW();

  SPI_Tx(instruction);
  SPI_TxBuffer(idReg, 4);
  SPI_Tx(dlc);
  SPI_TxBuffer(data, dlc);

  MCP2515_CS_HIGH();
}

/* TxBuffer・・1 Bytes ・ｰ・ｰ */
void MCP2515_LoadTxBuffer(uint8_t instruction, uint8_t data)
{
  MCP2515_CS_LOW();

  SPI_Tx(instruction);
  SPI_Tx(data);

  MCP2515_CS_HIGH();
}

/* RTS ・・ｹ・・奝ｵ﨑ｴ・・TxBuffer ・・・ */
void MCP2515_RequestToSend(uint8_t instruction)
{
  MCP2515_CS_LOW();

  SPI_Tx(instruction);

  MCP2515_CS_HIGH();
}

/* MCP2515 Status 嶹菩攤 */
uint8_t MCP2515_ReadStatus(void)
{
  uint8_t retVal;

  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_READ_STATUS);
  retVal = SPI_Rx();

  MCP2515_CS_HIGH();

  return retVal;
}

/* MCP2515 RxStatus ・溢ｧ・､奓ｰ 嶹菩攤 */
uint8_t MCP2515_GetRxStatus(void)
{
  uint8_t retVal;

  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_RX_STATUS);
  retVal = SPI_Rx();

  MCP2515_CS_HIGH();

  return retVal;
}

/* ・溢ｧ・､奓ｰ ・・・・ｽ */
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data)
{
  MCP2515_CS_LOW();

  SPI_Tx(MCP2515_BIT_MOD);
  SPI_Tx(address);
  SPI_Tx(mask);
  SPI_Tx(data);

  MCP2515_CS_HIGH();
}

/* SPI Tx Wrapper 﨑ｨ・・*/
static void SPI_Tx(uint8_t data)
{
  HAL_SPI_Transmit(SPI_CAN, &data, 1, SPI_TIMEOUT);
}

/* SPI Tx Wrapper 﨑ｨ・・*/
static void SPI_TxBuffer(uint8_t *buffer, uint8_t length)
{
  HAL_SPI_Transmit(SPI_CAN, buffer, length, SPI_TIMEOUT);
}

/* SPI Rx Wrapper 﨑ｨ・・*/
static uint8_t SPI_Rx(void)
{
  uint8_t retVal;
  HAL_SPI_Receive(SPI_CAN, &retVal, 1, SPI_TIMEOUT);
  return retVal;
}

/* SPI Rx Wrapper 﨑ｨ・・*/
static void SPI_RxBuffer(uint8_t *buffer, uint8_t length)
{
  HAL_SPI_Receive(SPI_CAN, buffer, length, SPI_TIMEOUT);
}
