#ifndef TARGET_H
#define TARGET_H

// System libraries
#include "gd32f1x0.h"
#include "gd32f1x0_gpio.h"
#include "gd32f1x0_exti.h"
#include "gd32f1x0_rcu.h"

// setup.c::usart init
#define TARGET_usart_oversample_config(a,b)	usart_oversample_config(a,b)

	// it.c
	#define TARGET_DMA_Channel0_IRQHandler DMA_Channel0_IRQHandler //Probably for ADC
	#define TARGET_TIMER0_BRK_UP_TRG_COM_IRQHandler TIMER0_BRK_UP_TRG_COM_IRQHandler
	
	// setup.c
	
	
	#define TARGET_dma_interrupt_flag_get(a,b) dma_interrupt_flag_get(a,b)
	#define TARGET_dma_interrupt_flag_clear(a,b) dma_interrupt_flag_clear(a,b)
	#define TARGET_adc_software_trigger_enable(a) adc_software_trigger_enable(a)	
	#define TARGET_fwdgt_window_value_config(a) fwdgt_window_value_config(a)
	#define TARGET_nvic_irq_enable(a, b, c){nvic_irq_enable(a, b, c);}
	#define TARGET_nvic_priority_group_set(a){nvic_priority_group_set(a);}
	#define TARGET_adc_vbat_disable(){adc_vbat_disable();}
	#define TARGET_ADC_RDATA ADC_RDATA
	#define TARGET_adc_dma_mode_enable()	adc_dma_mode_enable()
	#define TARGET_dma_deinit(a) dma_deinit(a)
	#define TARGET_dma_init(a,b) dma_init(a,b)
	#define TARGET_dma_circulation_enable(a) dma_circulation_enable(a)
	#define TARGET_dma_memory_to_memory_disable(a) dma_memory_to_memory_disable(a)
	#define TARGET_dma_interrupt_enable(a,b) dma_interrupt_enable(a,b)
	#define TARGET_dma_transfer_number_config(a,b) dma_transfer_number_config(a,b)
	#define TARGET_dma_channel_enable(a) dma_channel_enable(a)
	#define TARGET_adc_channel_length_config(a,b) adc_channel_length_config(a,b)
	#define TARGET_adc_regular_channel_config(a,b,c)	adc_regular_channel_config(a,b,c)
	#define TARGET_adc_data_alignment_config(a)	adc_data_alignment_config(a)
	#define TARGET_adc_external_trigger_config(a,b)	adc_external_trigger_config(a,b)
	#define TARGET_adc_external_trigger_source_config(a,b)	adc_external_trigger_source_config(a,b)
	#define TARGET_adc_watchdog_disable() adc_watchdog_disable()
	#define TARGET_adc_enable()	adc_enable()
	#define TARGET_adc_calibration_enable()	adc_calibration_enable()
	#define TARGET_adc_special_function_config(a,b)	adc_special_function_config(a,b)
	#define TARGET_nvic_irq_enable(a, b, c){nvic_irq_enable(a, b, c);}
	
	// usart 0
	#define TARGET_DMA_Channel1_2_IRQn DMA_Channel1_2_IRQn
	#define TARGET_DMA_CH2 DMA_CH2
	#define TARGET_DMA_Channel1_2_IRQHandler DMA_Channel1_2_IRQHandler

	// usart1
	#define TARGET_DMA_Channel3_4_IRQn	DMA_Channel3_4_IRQn
	#define TARGET_DMA_CH4	DMA_CH4
	#define TARGET_DMA_Channel3_4_IRQHandler	DMA_Channel3_4_IRQHandler

	#define USART0_DATA_RX_ADDRESS (uint32_t)&USART_RDATA(USART0)
	#define USART1_DATA_RX_ADDRESS (uint32_t)&USART_RDATA(USART1)
	//#define USART0_DATA_RX_ADDRESS ((uint32_t)0x40013824)
	//#define USART1_DATA_RX_ADDRESS ((uint32_t)0x40004424)



#ifndef pinMode
	
	/* GD32F130 and GD32E230
		#define GPIO_PIN_15 	BIT(15)
	
		#define GPIOA         (GPIO_BASE + 0x00000000U)
		#define GPIOB					(GPIO_BASE + 0x00000400U)
		#define GPIOC         (GPIO_BASE + 0x00000800U)
		#define GPIOD         (GPIO_BASE + 0x00000C00U)
		#define GPIOF         (GPIO_BASE + 0x00001400U)
	
		#define GPIO_BASE     (AHB2_BUS_BASE + 0x00000000U)  // !< GPIO base address
		#define AHB2_BUS_BASE ((uint32_t)0x48000000U)        //!< ahb2 base address
	*/
	
	// GD32F130 has 10 channels PA0..PA7 = 0..7 and PB0,PB1 = 8,9 . Only 64 pin MCU has further adc on GPIOC
	#define PIN_TO_CHANNEL(pin) ((pin&0xffffff00U) ==  GPIOA ? (pin&0xfU) : ((pin&0xfU)+8) )

/* function in setup.c instead of define saves memory
	#define pinMode(pin,mode) \
	{\
		gpio_mode_set(pin&0xffffff00U, mode, GPIO_PUPD_NONE,BIT(pin&0xfU) );	\
		gpio_output_options_set(pin&0xffffff00U, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, BIT(pin&0xfU));\
	}
	#define pinModePull(pin,mode,pull) \
	{\
		gpio_mode_set(pin&0xffffff00U, mode, pull,BIT(pin&0xfU) );	\
		gpio_output_options_set(pin&0xffffff00U, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, BIT(pin&0xfU));\
	}
*/
	#define pinModeSpeed(pin,mode,speed) \
	{\
		gpio_mode_set(pin&0xffffff00U, mode, GPIO_PUPD_NONE,BIT(pin&0xfU) );	\
		gpio_output_options_set(pin&0xffffff00U, GPIO_OTYPE_PP, speed, BIT(pin&0xfU));\
	}
	
	// AF = AlternateFunction
	#define pinModeAF(pin, AF, pullUpDown,speed) \
	{\
		gpio_mode_set(pin&0xffffff00U , GPIO_MODE_AF, pullUpDown, BIT(pin&0xfU));	\
		gpio_output_options_set(pin&0xffffff00U, GPIO_OTYPE_PP, speed, BIT(pin&0xfU));	\
		gpio_af_set(pin&0xffffff00U, AF(pin), BIT(pin&0xfU));		\
	}
	//GD32F130xx Datasheet	2.6.7. GD32F130xx pin alternate function	
	#define AF_TIMER0_BLDC(pin)	GPIO_AF_2	// GD32F130: all TIMER0 AF are AF2
	#define AF_TIMER0_BRKIN(pin)	(pin==PA6 ? GPIO_AF_2 : GPIO_AF_2)	// GD32F130: AF2 = PA6 or PB12
	#define AF_USART0_TX(pin)	(pin==PB6 ? GPIO_AF_0 : GPIO_AF_1)			// GD32F130: AF0 = PB6 , AF1 = PA2 or PA9 or PA14
	#define AF_USART0_RX(pin)	(pin==PB7 ? GPIO_AF_0 : GPIO_AF_1)			// GD32F130: AF0 = PB7 , AF1 = PA3 or PA15 
	#define AF_USART1_TX(pin)	(pin==PA8 ? GPIO_AF_4 : GPIO_AF_1)		// GD32F130: AF4 = PA8 , AF1 = PA2 or PA14
	#define AF_USART1_RX(pin)	(pin==PB0 ? GPIO_AF_4 : GPIO_AF_1)		// GD32F130: AF4 = PB0 , AF1 = PA3 or PA15
	// usart1 and 1 same for GD32F230: GD32E230xxDatasheet_Rev2.0.pdf page 33ff

	#define digitalWrite(pin,set) gpio_bit_write(pin&0xffffff00U,  (BIT(pin&0xfU) ), set)
	#define digitalRead(pin) 			gpio_input_bit_get(pin&0xffffff00U, BIT(pin&0xfU))

	

#endif	
	
#define PB1i	17	// 0x10 + 1
#define PB2i	18	// 0x10 + 2
#define PB11i	27	// 0x10 + 11
#define PC14i	62	// 0x30 + 14
#define PF1i	81		// 0x50 + 1 TODO: czy to potrzebne
	
	#define PA15	( (uint32_t)GPIOA | 15 )
	#define PA14	( (uint32_t)GPIOA | 14 )
	#define PA13	( (uint32_t)GPIOA | 13 )
	#define PA12	( (uint32_t)GPIOA | 12 )
	#define PA11	( (uint32_t)GPIOA | 11 )
	#define PA10	( (uint32_t)GPIOA | 10 )
	#define PA9		( (uint32_t)GPIOA | 9 )
	#define PA8		( (uint32_t)GPIOA | 8 )
	#define PA7		( (uint32_t)GPIOA | 7 )
	#define PA6		( (uint32_t)GPIOA | 6 )
	#define PA5		( (uint32_t)GPIOA | 5 )
	#define PA4		( (uint32_t)GPIOA | 4 )
	#define PA3		( (uint32_t)GPIOA | 3 )
	#define PA2		( (uint32_t)GPIOA | 2 )
	#define PA1		( (uint32_t)GPIOA | 1 )
	#define PA0		( (uint32_t)GPIOA | 0 )

	#define PB15	( (uint32_t)GPIOB | 15 )
	#define PB14	( (uint32_t)GPIOB | 14 )
	#define PB13	( (uint32_t)GPIOB | 13 )
	#define PB12	( (uint32_t)GPIOB | 12 )
	#define PB11	( (uint32_t)GPIOB | 11 )
	#define PB10	( (uint32_t)GPIOB | 10 )
	#define PB9		( (uint32_t)GPIOB | 9 )
	#define PB8		( (uint32_t)GPIOB | 8 )
	#define PB7		( (uint32_t)GPIOB | 7 )
	#define PB6		( (uint32_t)GPIOB | 6 )
	#define PB5		( (uint32_t)GPIOB | 5 )
	#define PB4		( (uint32_t)GPIOB | 4 )
	#define PB3		( (uint32_t)GPIOB | 3 )
	#define PB2		( (uint32_t)GPIOB | 2 )
	#define PB1		( (uint32_t)GPIOB | 1 )
	#define PB0		( (uint32_t)GPIOB | 0 )

	#define PC15	( (uint32_t)GPIOC | 15 )
	#define PC14	( (uint32_t)GPIOC | 14 )
	#define PC13	( (uint32_t)GPIOC | 13 )
	#define PC12	( (uint32_t)GPIOC | 12 )
	#define PC11	( (uint32_t)GPIOC | 11 )
	#define PC10	( (uint32_t)GPIOC | 10 )
	#define PC9	( (uint32_t)GPIOC | 9 )
	#define PC8	( (uint32_t)GPIOC | 8 )
	#define PC7	( (uint32_t)GPIOC | 7 )
	#define PC6	( (uint32_t)GPIOC | 6 )
	#define PC5	( (uint32_t)GPIOC | 5 )
	#define PC4	( (uint32_t)GPIOC | 4 )
	#define PC3	( (uint32_t)GPIOC | 3 )
	#define PC2	( (uint32_t)GPIOC | 2 )
	#define PC1	( (uint32_t)GPIOC | 1 )
	#define PC0	( (uint32_t)GPIOC | 0 )

	#define PF7	( GPIOF | 7 )
	#define PF6	( GPIOF | 6 )
	#define PF4	( GPIOF | 4 )
	#define PF1	( GPIOF | 1 )
	#define PF0	( GPIOF | 0 )


/* GD32F130 48pin possible IO pins: 
	C13 C14 C15 F0 F1 A0 A1 A2 
	A3 A4 A5 A6 A7 B0 B1 B2 B10 B11
	B12 B13 B14 B15 A8 A9 A10 A11 A12 A13 F6 F7
	A14 A15 B3 B4 B5 B6 B7 B8 B9 	*/ 

#endif // TARGET_H
	