//#include <stdint>
#include <stm32l476xx.h>
#include "system_clock.h"
#include "SPI.h"

#include <stdint.h>
#define CONCAT(A,B) A ## B

//some of these confugurations use two bits to control the function
#define TWO_CONFIG_BITS(X) X*2

#define GPIO_PORT(PORT_LETTER) CONCAT(GPIO,PORT_LETTER) 

//note all theses pins are in the ARF low register 
//PA4
#define SPI1_NSS 4
#define SPI1_NSS_PORT A
#define START_AFR_SPI1_NSS_PA4 16

//PB3
#define SPI1_SCK 3
#define SPI1_SCK_PORT B
#define START_AFR_SPI1_SCK_PB3 12

//PA6
#define SPI1_MISO 6
#define SPI1_MISO_PORT A
#define START_AFR_SPI1_MISO_PA6 24

//PA7
#define SPI1_MOSI 7
#define SPI1_MOSI_PORT A
#define START_AFR_SPI1_MOSI_PA7 28


#define SPI1_ARF 5ul

static void config_SPI_clock(SPI_TypeDef * SPIx);

void general_config_SPI_master(SPI_TypeDef * SPIx)
{
    /*
    this configures the slave slect line to be done in software, so the slave slecet line can be any IO
    the slave slect line should not be the slave slect alternative functino for that spi
    PA4 AFR 5 makes this the NSS for SPI1, this is the wrong configuration to use software to select the slave
    PA4 as digital output is the correct way to use it as the slve select 
    */

    SPIx->CR1 &= ~SPI_CR1_SPE; //disable SPI
    SPIx->CR1 |= SPI_CR1_MSTR;//0= set as slave, 1=set as SPI master


    SPIx->CR1 &= ~SPI_CR1_BIDIMODE; //bidirectional data enabled//0=2 line unidirectional mode 1= 1 line bidrectional mode
                                    //we selected the SPI to use MISO and MOSI and each line only sends data one way
                                    //if we set this bit, we make the SPI use only one line, we select if it is sending or recieving with the BIDIOE bit
    SPIx->CR1 &= ~SPI_CR1_BIDIOE;   //output ebable in pidirecional mode// 0 = output disabled (rx only mode) 1 = output enabled (tx only mode)
                                    //if we set the BIDIMODE bit we would use this bit to configure if this SPI device was the RX to TX side 
    SPIx->CR1 &= ~SPI_CR1_RXONLY;//set duplex//0 = duplex(rx and tx), 1 = rx only
                                    //the data sheet says to keep this cleared if BIDIMODE bit is set (2 line unidirectional mode)
    //This bit enables simplex communication using a single unidirectional line to receive data exclusively
    //if we want to opperate in BIDIMODE 2 line unidirectinoal and only send data, we set this bit
    
    /*
    BIDIMODE    |   BIDIOE  |   RXONLY  |      function
    0           |   x       |   0       |       2 line unidirectional communication (M/S MISO connected, M/S MOSI connected, two connections)
    0           |   x       |   1       |       RX only but in unidirectional mode(M/S MISO connected, M/S MOSI connected, two connections)
    1           |   0       |   x       |       1 line half duplex send only (M MISO/MOSI connected, S MISO/MOSI connected, M/S combined MISO/MOSI connected, simgle connection) 
    1           |   1       |   x       |       1 line half duplex recieve only (M MISO/MOSI connected, S MISO/MOSI connected, M/S combined MISO/MOSI connected, simgle connection) 
    
    
    */
    SPIx->CR2 &= ~SPI_CR2_DS;
    SPIx->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2; //data format 0111:8 bit

    SPIx->CR1 &= ~SPI_CR1_LSBFIRST; //bit order o = MSB tx/rx fst, 1= LSB tx/rx fst
    SPIx->CR1 &= ~SPI_CR1_CPHA;//clock phase // 0 = 1st clock transmit is the 1st data capture edge
                                            //1 = second clock transition is the 1st data capture edge
    SPIx->CR1 &= ~SPI_CR1_CPOL; // clock polarity // 0= set ck to 0 when idol, 1= set ck to 1 when idol
    SPIx->CR1 |= 3UL<<4; //baud rate control scaler: 000=2, 001=3. 010=8, 011 = 16, 100=32,101=64,110=128,111=256

    SPIx->CRCPR=10; //CRC polynomial

    SPIx->CR1 &= ~SPI_CR1_CRCEN;//hardware CRC calculation disabled

    SPIx->CR2  &= ~SPI_CR2_FRF;//frame format 0 = SPI motorola mode, 1= SPI TI mode

    SPIx->CR1 |= SPI_CR1_SSM; //1= softare slave managment enabled, 0= hardware nss managment enabled
    //SPIx->CR1 &= ~SPI_CR1_SSOE;
    /*
    SSM |   SSOE    |   functino
    1   |   x       |   software selcts slave
    0   |   0       |   hardware controls NSS, when SPE is enabled this is pulled low, SPE disabled this is pulled high, single master system
    0   |   1       |   multi master system, allows for some form of arbitration, data sheet page 1478
    */


    SPIx->CR1 |= SPI_CR1_SSI;//manage NSS slave selecr in software
                                //This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the
                                //NSS pin and the I/O value of the NSS pin is ignored.
                                //I think I change this bit to control the NSS line, using it as an alternative fucntion 

    //SPIx->CR2 |= SPI_CR2_NSSP; //enable NSS pulse managment
    SPIx->CR2 |= SPI_CR2_FRXTH;

    SPIx->CR1 |= SPI_CR1_SPE;//enable SPI
}

void general_config_SPI_slave(SPI_TypeDef * SPIx)
{
    /*
    this configures the slave slect line to be done in software, so the slave slecet line can be any IO
    the slave slect line should not be the slave slect alternative functino for that spi
    PA4 AFR 5 makes this the NSS for SPI1, this is the wrong configuration to use software to select the slave
    PA4 as digital output is the correct way to use it as the slve select 
    */

    SPIx->CR1 &= ~SPI_CR1_SPE; //disable SPI
    SPIx->CR1 &= ~SPI_CR1_MSTR;//0= set as slave, 1=set as SPI master==================================slave===================


    SPIx->CR1 &= ~SPI_CR1_BIDIMODE; //bidirectional data enabled//0=2 line unidirectional mode 1= 1 line bidrectional mode
                                    //we selected the SPI to use MISO and MOSI and each line only sends data one way
                                    //if we set this bit, we make the SPI use only one line, we select if it is sending or recieving with the BIDIOE bit
    SPIx->CR1 &= ~SPI_CR1_BIDIOE;   //output ebable in pidirecional mode// 0 = output disabled (rx only mode) 1 = output enabled (tx only mode)
                                    //if we set the BIDIMODE bit we would use this bit to configure if this SPI device was the RX to TX side 
    SPIx->CR1 &= ~SPI_CR1_RXONLY;//set duplex//0 = duplex(rx and tx), 1 = rx only
                                    //the data sheet says to keep this cleared if BIDIMODE bit is set (2 line unidirectional mode)
    //This bit enables simplex communication using a single unidirectional line to receive data exclusively
    //if we want to opperate in BIDIMODE 2 line unidirectinoal and only send data, we set this bit
    
    /*
    BIDIMODE    |   BIDIOE  |   RXONLY  |      function
    0           |   x       |   0       |       2 line unidirectional communication (M/S MISO connected, M/S MOSI connected, two connections)
    0           |   x       |   1       |       RX only but in unidirectional mode(M/S MISO connected, M/S MOSI connected, two connections)
    1           |   0       |   x       |       1 line half duplex send only (M MISO/MOSI connected, S MISO/MOSI connected, M/S combined MISO/MOSI connected, simgle connection) 
    1           |   1       |   x       |       1 line half duplex recieve only (M MISO/MOSI connected, S MISO/MOSI connected, M/S combined MISO/MOSI connected, simgle connection) 
    
    
    */
    SPIx->CR2 &= ~SPI_CR2_DS;
    SPIx->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2; //data format 0111:8 bit

    SPIx->CR1 &= ~SPI_CR1_LSBFIRST; //bit order o = MSB tx/rx fst, 1= LSB tx/rx fst
    
    /*SPIx->CR1 &= ~SPI_CR1_CPHA;//clock phase // 0 = 1st clock transmit is the 1st data capture edge
                                            //1 = second clock transition is the 1st data capture edge
    SPIx->CR1 &= ~SPI_CR1_CPOL; // clock polarity // 0= set ck to 0 when idol, 1= set ck to 1 when idol
    SPIx->CR1 |= 3UL<<4; //baud rate control scaler: 000=2, 001=3. 010=8, 011 = 16, 100=32,101=64,110=128,111=256
*/
    SPIx->CRCPR=10; //CRC polynomial

    SPIx->CR1 &= ~SPI_CR1_CRCEN;//hardware CRC calculation disabled

    SPIx->CR2  &= ~SPI_CR2_FRF;//frame format 0 = SPI motorola mode, 1= SPI TI mode
/*
    SPIx->CR1 |= SPI_CR1_SSM; //1= softare slave managment enabled, 0= hardware nss managment enabled
    //SPIx->CR1 &= ~SPI_CR1_SSOE;
    //
    SSM |   SSOE    |   functino
    1   |   x       |   software selcts slave
    0   |   0       |   hardware controls NSS, when SPE is enabled this is pulled low, SPE disabled this is pulled high, single master system
    0   |   1       |   multi master system, allows for some form of arbitration, data sheet page 1478
  //  


    SPIx->CR1 |= SPI_CR1_SSI;//manage NSS slave selecr in software
                                //This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the
                                //NSS pin and the I/O value of the NSS pin is ignored.
                                //I think I change this bit to control the NSS line, using it as an alternative fucntion 

    //SPIx->CR2 |= SPI_CR2_NSSP; //enable NSS pulse managment
    */
    SPIx->CR2 |= SPI_CR2_FRXTH;

    SPIx->CR1 |= SPI_CR1_SPE;//enable SPI
}

void config_SPI1_master()
{
    enable_HSI16();
    //using PA4 (NSS), PB3 (SCK), PA6 (MISO), PA7 (MOSI)
    //config the pins
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;//enable clock to GPIO portA
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;//enable clock to GPIO portB

    GPIO_PORT(SPI1_NSS_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_NSS))); //set alt functinos
  //  GPIO_PORT(SPI1_NSS_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_NSS)));
    GPIO_PORT(SPI1_NSS_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(SPI1_NSS)));
    GPIO_PORT(SPI1_SCK_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_SCK_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_MISO_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MISO_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MOSI_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));
    GPIO_PORT(SPI1_MOSI_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));
    //GPIO_PORT(SPI1_NSS_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_NSS_PA4);
   // GPIO_PORT(SPI1_NSS_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_NSS_PA4);
    GPIO_PORT(SPI1_SCK_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_SCK_PB3);
    GPIO_PORT(SPI1_SCK_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_SCK_PB3);
    GPIO_PORT(SPI1_MISO_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_MISO_PA6);
    GPIO_PORT(SPI1_MISO_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_MISO_PA6);
    GPIO_PORT(SPI1_MOSI_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_MOSI_PA7);
    GPIO_PORT(SPI1_MOSI_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_MOSI_PA7);//finish setting alt functinos
        
    GPIO_PORT(SPI1_NSS_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_NSS)));//config as high speed
    GPIO_PORT(SPI1_SCK_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_MISO_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MOSI_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));

    GPIO_PORT(SPI1_NSS_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_NSS)));//select pull-up
    GPIO_PORT(SPI1_NSS_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_NSS)));
    GPIO_PORT(SPI1_SCK_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_SCK_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_MISO_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MISO_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MOSI_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));
    GPIO_PORT(SPI1_MOSI_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));

    GPIO_PORT(SPI1_NSS_PORT)->OTYPER &= ~(1ul<<SPI1_NSS);//push pull
    GPIO_PORT(SPI1_SCK_PORT)->OTYPER &= ~(1ul<<SPI1_SCK);
    GPIO_PORT(SPI1_MISO_PORT)->OTYPER &= ~(1ul<<SPI1_MISO);
    GPIO_PORT(SPI1_MOSI_PORT)->OTYPER &= ~(1ul<<SPI1_MOSI);

    config_SPI_clock(SPI1);

    //SPI1->CR2 |= SPI_CR2_TXEIE;
    SPI1->CR2 |= SPI_CR2_RXNEIE; // recieve buffer not empty interrupt
    NVIC_SetPriority(SPI1_IRQn, 0);
    NVIC_EnableIRQ(SPI1_IRQn);

    SPI1_deselect_slave();

    //enable the interrupt
    /*
    need to config the clocks before you can enable interruprs
    SPI1->CR2 |= SPI_CR2_TXEIE;
    SPI1->CR2 |= SPI_CR2_RXNEIE; // recieve buffer not empty interrupt
    NVIC_SetPriority(SPI1_IRQn, 0);
    NVIC_EnableIRQ(SPI1_IRQn);
    */
    general_config_SPI_master(SPI1);
}
void SPI1_select_slave()
{
    GPIO_PORT(SPI1_NSS_PORT)->ODR &= ~(1ul<<SPI1_NSS);
}
void SPI1_deselect_slave()
{
    GPIO_PORT(SPI1_NSS_PORT)->ODR |= (1ul<<SPI1_NSS);
}

void config_SPI1_slave()
{
    enable_HSI16();
    //using PA4 (NSS), PB3 (SCK), PA6 (MISO), PA7 (MOSI)
    //config the pins
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;//enable clock to GPIO portA
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;//enable clock to GPIO portB

    GPIO_PORT(SPI1_NSS_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_NSS))); //set alt functinos
    GPIO_PORT(SPI1_NSS_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_NSS)));//select NSS as alternative function
    //GPIO_PORT(SPI1_NSS_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(SPI1_NSS)));//select NSS as pin output to selct slabe
    GPIO_PORT(SPI1_SCK_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_SCK_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_MISO_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MISO_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MOSI_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));
    GPIO_PORT(SPI1_MOSI_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));
    GPIO_PORT(SPI1_NSS_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_NSS_PA4);
    GPIO_PORT(SPI1_NSS_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_NSS_PA4);
    GPIO_PORT(SPI1_SCK_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_SCK_PB3);
    GPIO_PORT(SPI1_SCK_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_SCK_PB3);
    GPIO_PORT(SPI1_MISO_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_MISO_PA6);
    GPIO_PORT(SPI1_MISO_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_MISO_PA6);
    GPIO_PORT(SPI1_MOSI_PORT)->AFR[0] &= ~(0xF<<START_AFR_SPI1_MOSI_PA7);
    GPIO_PORT(SPI1_MOSI_PORT)->AFR[0] |= (SPI1_ARF<<START_AFR_SPI1_MOSI_PA7);//finish setting alt functinos
        
    GPIO_PORT(SPI1_NSS_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_NSS)));//config as high speed
    GPIO_PORT(SPI1_SCK_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_MISO_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MOSI_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));

    GPIO_PORT(SPI1_NSS_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_NSS)));//select pull-up
    GPIO_PORT(SPI1_NSS_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_NSS)));
    GPIO_PORT(SPI1_SCK_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_SCK_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_SCK)));
    GPIO_PORT(SPI1_MISO_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MISO_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_MISO)));
    GPIO_PORT(SPI1_MOSI_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));
    GPIO_PORT(SPI1_MOSI_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(SPI1_MOSI)));

    GPIO_PORT(SPI1_NSS_PORT)->OTYPER &= ~(1ul<<SPI1_NSS);//push pull
    GPIO_PORT(SPI1_SCK_PORT)->OTYPER &= ~(1ul<<SPI1_SCK);
    GPIO_PORT(SPI1_MISO_PORT)->OTYPER &= ~(1ul<<SPI1_MISO);
    GPIO_PORT(SPI1_MOSI_PORT)->OTYPER &= ~(1ul<<SPI1_MOSI);

    config_SPI_clock(SPI1);

    //SPI1->CR2 |= SPI_CR2_TXEIE;
    SPI1->CR2 |= SPI_CR2_RXNEIE; // recieve buffer not empty interrupt
    NVIC_SetPriority(SPI1_IRQn, 0);
    NVIC_EnableIRQ(SPI1_IRQn);


    //enable the interrupt
    /*
    SPI1->CR2 |= SPI_CR2_TXEIE;
    SPI1->CR2 |= SPI_CR2_RXNEIE; // recieve buffer not empty interrupt
    NVIC_SetPriority(SPI1_IRQn, 0);
    NVIC_EnableIRQ(SPI1_IRQn);
    */
    general_config_SPI_slave(SPI1);
}

uint8_t SPI_read(SPI_TypeDef * SPIx)
{
        uint8_t bla;

while((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE);
SPIx->DR = 'c';
while((SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE);
while((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY);
bla = SPIx->DR;


//SPIx->SR|= SPI_SR_RXNE;


return bla;
}

uint8_t SPI_interrupt_read(SPI_TypeDef * SPIx)
{   
    uint8_t temp;
  //  uint8_t fake;

    temp = SPIx->DR;
   // fake = SPIx->DR;

   // SPIx->SR|= SPI_SR_RXNE;
    return temp;
    
}
void SPI_slave_update_DR(SPI_TypeDef * SPIx, uint8_t input)
{
    SPIx->DR = input;
}
void SPI_write(SPI_TypeDef * SPIx, uint8_t input)
{
    //wait for  TXE to be empty(transmisio buffer empty)
   /*if(SPIx->SR & SPI_SR_TXE)
   {
    SPIx->DR = input;
   }
  // (SPIx->SR &= ~SPI_SR_BSY);
*/  
   while((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE);
   //if((SPIx->SR & SPI_SR_RXNE) == 0)
  // {
    SPIx->DR = input;
  // }
    //wait for busy flag cleared
    while((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY);
    
}

static void config_SPI_clock(SPI_TypeDef * SPIx)
{
     //enable clock and then reset and clear 
    if(SPIx == SPI1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
    }
    else if (SPIx == SPI2)
    {
        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
        RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST;
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST;
    }
    else if (SPIx == SPI3)
    {
        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
        RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI3RST;
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI3RST;
    }
}