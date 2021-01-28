*******************************************************************************
*  				Lessons Learned        
*******************************************************************************

--------- Clock / registers ---------
* By default the system clock is the MSI at 2 MHz
* Some registers have a reset value - This is to ensure functionalities such as SWD/JTAG is always enabled.
* PA13 and PA14 is used for SWDIO SWDCLK, therefor specific reset values for PORTA in GPIOA->MODER and GPIOA->PUPDR and GPIA->SPEEDR etc.
* Some registers have a specific format they accept byte/word/half word/bit  - Check which can be used to access!

* BSRR - Bit Set Reset Register is used for Atomic write - This is interrupt safe, as it is done in one clock cycle and therefore cannot be preempted.
  These bits are write-only. A read to these bits returns the value 0x0000.


--------- CM0+ / STM32 --------- 
#define PERIPH_BASE            (0x40000000UL) /*!< Peripheral base address in the alias region */
#define IOPPERIPH_BASE        (PERIPH_BASE + 0x10000000UL)
#define GPIOA_BASE            (IOPPERIPH_BASE + 0x00000000UL)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)

GPIO base address 0X5000 0000 - 0X5000 03FF    1K    GPIOA

typedef struct
{
  __IO uint32_t MODER;        /*!< GPIO port mode register,                     Address offset: 0x00 */
  __IO uint32_t OTYPER;       /*!< GPIO port output type register,              Address offset: 0x04 */
  __IO uint32_t OSPEEDR;      /*!< GPIO port output speed register,             Address offset: 0x08 */
  __IO uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C */
  __IO uint32_t IDR;          /*!< GPIO port input data register,               Address offset: 0x10 */
  __IO uint32_t ODR;          /*!< GPIO port output data register,              Address offset: 0x14 */
  __IO uint32_t BSRR;         /*!< GPIO port bit set/reset registerBSRR,        Address offset: 0x18 */
  __IO uint32_t LCKR;         /*!< GPIO port configuration lock register,       Address offset: 0x1C */
  __IO uint32_t AFR[2];       /*!< GPIO alternate function register,            Address offset: 0x20-0x24 */
  __IO uint32_t BRR;          /*!< GPIO bit reset register,                     Address offset: 0x28 */
}GPIO_TypeDef;

From datasheet memory map Peripherals start address is 0x4000 0000 up to 0x5000 1FFFF
inside that range (Peripherals) we have 
APB1 - Advanced Peripheral Bus 1 start 0x4000 0000 up to 0x4000 8000
Reserved Area 0x4000 8000 up to 0x4001 000
APB2 - Advanced Peripheral Bus 2 start 0x4000 1000 up to 0x4001 8000
Reserved Area. ...
AHB - Advanced High Performance Bus 0x4002 0000 to 0x4002 63FF
and so on ... up to 0x5000 1FFFF

Datasheet says the total memory map is divded into 8 sections each of 512MB
Lets look at 0x4000 0000 up to 0x6000 0000 = 0x2000 0000 -> 536870912 -> 536.870.912
1 kilobyte = 1024 bytes -> 2^10
1 megabyte = 2^20 = 1048576 bytes  
1 MB = 1 Megabyte = 1024 * 1 KB = 1,048,576 bytes.
536.870.912 / 2^20 = 512 MB

So, 
Capacity = Last Address - First Address + 1.


--------- Protocol ---------


--------- Software / Basic operations ---------
Setting a bit
number |= 1UL << n;
Clearing a bit
number &= ~(1UL << n);
Toggling a bit
number ^= 1UL << n;


--------- IAR ---------




