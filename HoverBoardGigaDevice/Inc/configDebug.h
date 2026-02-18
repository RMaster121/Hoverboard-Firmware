#ifndef CONFIGDEBUG_H
#define CONFIGDEBUG_H

	// =======================================================================================
	// WYBÓR MIKROKONTROLERA I UKŁADU PŁYTKI (TARGET & LAYOUT)
	// =======================================================================================
	// Wybierz swój mikrokontroler (Target) z listy rozwijanej w prawym górnym rogu IDE Keil
	// (np. GD32F130, GD32F103). Następnie ustaw odpowiedni układ (LAYOUT) poniżej.
	// LAYOUT definiuje mapowanie pinów dla konkretnej wersji płytki PCB.
	// Gen2-target-layout jest zdefiniowany w pliku defines.h.
	
	#ifdef GD32F130		// TARGET = 1
		#define LAYOUT 1
		#define LAYOUT_SUB 1	// Layout 2.1.7 exisits as 2.1.7.0 and 2.1.7.1
	#elif GD32F103		// TARGET = 2
		#define LAYOUT 1
		//#define STM32F103 	// uncomment if your board has stm32f103 instead of gd32f103. Will give the max 64 Mhz for STM32
			// Warning: will overclock gd32f103 mcu to 124 MHz and doubling current consumption. Keep GD32 to its 72 Mhz.
	#elif GD32E230		// TARGET = 3
		#define LAYOUT 1
	#elif MM32SPIN05	// TARGET = 4
		#define LAYOUT 1
	#endif

	// =======================================================================================
	// STEROWANIE SILNIKIEM (BLDC CONTROL)
	// =======================================================================================
	// Wybierz TYLKO JEDNĄ metodę komutacji.
	
	//#define BLDC_BC			// Komutacja blokowa (trapezoidalna). Prostsza, ale silniki pracują głośniej.
	#define BLDC_SINE			// Sterowanie sinusoidalne (FOC/Sinus). Silniki pracują cicho i płynnie. (Domyślne)
	//#define BLDC_SINE_BOOSTER		// Wariant sinusoidalny z "dopalaczem" (SVM - Space Vector Modulation).
								// Pozwala uzyskać ok. 15% wyższą prędkość maksymalną poprzez pełne wykorzystanie napięcia DC.

	// =======================================================================================
	// TRYB JAZDY (DRIVING MODE)
	// =======================================================================================

	// Określa, jak interpretowane są sygnały wejściowe (z manetki, UART itp.).
	// 0 = PWM (Voltage): Bezpośrednie sterowanie wypełnieniem napięcia. "Surowe" odczucie mocy.
	// 1 = Speed: Sterowanie prędkością (zamknięta pętla PID). Utrzymuje zadaną prędkość (obr/s * 1024).
	// 2 = Torque: Sterowanie momentem obrotowym (prądem). Daje odczucie "gazu" w samochodzie.
	// 3 = Odometer: Sterowanie pozycją (np. jedź o X kroków).
	// Określa, jak interpretowane są sygnały wejściowe (np. z manetki, UART).
	// 0 = PWM (Napięcie): Bezpośrednie sterowanie wypełnieniem PWM. Daje "surowe" odczucie mocy. Wartości od -1000 do 1000.
	// 1 = Speed (Prędkość): Sterowanie prędkością obrotową w pętli zamkniętej (PID). Utrzymuje zadaną prędkość niezależnie od obciążenia.
	//    Jednostka domyślna to "obr/s * 1024" (liczba stałoprzecinkowa).
	// 2 = Torque (Moment obrotowy): Sterowanie momentem obrotowym (prądem silnika). Daje odczucie "pedału gazu".
	//    Jednostka to "Nm * 1024".
	// 3 = Odometer (Pozycja): Sterowanie pozycją kątową. Zadajesz docelową pozycję w "krokach" czujników Halla.
	//    Jednostka: 1 krok = 1/90 obrotu koła (4 stopnie) dla typowego silnika z 15 parami biegunów.
	//    Przydatne w robotyce do precyzyjnego pozycjonowania (np. jedź o 1 metr = ok. 174 kroki dla kół 6.5").
	
	#define DRIVING_MODE 0

	//#define SPEED_AsRevsPerSec		// Jeśli zdefiniowane, prędkość liczona w obrotach/sekundę (uwaga na przepełnienie przy dużych prędkościach).
	//#define SPEED_AsRevsPerSec		// Zmienia jednostkę w trybie Speed (1). Domyślnie prędkość jest w formacie stałoprzecinkowym (obr/s * 1024),
									// co przy zmiennej int16_t ogranicza prędkość do ok. 1920 RPM. Ta opcja zmienia skalę,
									// pozwalając na pracę z silnikami o wyższych obrotach (do ok. 19620 RPM) bez przepełnienia zmiennej.

	// =======================================================================================
	// BATERIA I ZASILANIE
	// =======================================================================================
	#define BAT_CELLS         	10        // Liczba ogniw w szeregu (S). 10S = 36V nominalnie, 42V max.
	//#define BATTERY_LOW_SHUTOFF		// Jeśli odkomentowane: wyłącz deskę całkowicie przy krytycznie niskim napięciu (ochrona baterii).
	#define BATTERY_LOW_BEEP		// Jeśli odkomentowane: piszcz przy niskim poziomie baterii (ostrzeżenie).

	// =======================================================================================
	// DŹWIĘKI I SYGNALIZACJA
	// =======================================================================================
	#define BEEP_BACKWARDS 			// Piszczenie podczas jazdy do tyłu (jak w ciężarówkach).
	// #define DEBUG_LED			// Użyj diod LED do debugowania (sygnalizacja stanów wewnętrznych kodu).

	// =======================================================================================
	// ROLA PŁYTKI (BOARD ROLE)
	// =======================================================================================
	// Wybierz TYLKO JEDNĄ opcję. Możesz to zrobić w platformio.ini z flagą -D MASTER lub SLAVE
	// #define MASTER		// Płytka główna w układzie dwóch płytek (Master steruje Slavem).
	// #define SLAVE			// Płytka wykonawcza w układzie dwóch płytek (słucha Mastera).
	// #define SINGLE			// Pojedyncza płytka sterująca (np. rower, hulajnoga, gokart).

	#if defined(MASTER) || defined(SINGLE)
		// ===================================================================================
		// DODATKOWE OPCJE STEROWANIA
		// ===================================================================================
		//#define TEST_HALL2LED		// Wizualizacja stanów czujników Halla na diodach LED.
		//#define RTT_REMOTE		// Włącz logowanie przez RTT (wymaga debuggera).
		// uncoment for i2c_scanner and dump_i2c_registers to print to rtt :-)
			// needs either I2C_PB8PB9 or I2C_PB6PB7 and MPU_6050 or MPU_6050old or BMI_160 in your defines_2-x-y.h file


		#define PILOT_USER		// Włącz własny kod sterowania z pliku PilotUser.c.
		//TEMP
		#define TEST_HALL2LED		// Diagnostyka: diody LED migają w rytm sygnałów z czujników Halla silnika.
		#define RTT_REMOTE		// Logowanie danych i sterowanie przez debugger ST-Link (Real Time Transfer).
			#define WINDOWS_RN	// Formatowanie nowych linii dla Windows (\r\n).


		
		#define SPEED_COEFFICIENT   (-1)	// Skalowanie prędkości. -1 odwraca kierunek.
		#define STEER_COEFFICIENT   1	// Skalowanie skrętu.

		// ===================================================================================
		// KONFIGURACJA SPRZĘTOWA (HARDWARE CONFIG)
		// ===================================================================================
		//#define DISABLE_BUTTON	// Wyłącz obsługę przycisku Power (deska włącza się od razu po podaniu zasilania).
									// Przydatne przy zewnętrznym włączniku fizycznym.
		
		//#define DISABLE_CHARGESTATE	// Wyłącz wykrywanie ładowarki.
									// Włącz to, jeśli zasilasz deskę przez port ładowania (np. z zasilacza lab).
									// Normalnie deska wyłącza napęd po wykryciu ładowarki.

		#define MASTER_OR_SINGLE
	#endif
	
	// =======================================================================================
	// KONFIGURACJA PORTÓW KOMUNIKACYJNYCH (USART)
	// =======================================================================================
	// Wybór fizycznych pinów dla komunikacji.
	// 0: Zazwyczaj PB6/PB7 (złącze obok programowania).
	// 1: Zazwyczaj PA2/PA3 (oryginalne złącze Master-Slave).
	// 2: Zazwyczaj PB10/PB11 (tylko na STM32F103).

	#if defined(MASTER) || defined(SLAVE)
		#define MASTERSLAVE_USART		1 	// Port do komunikacji między dwiema płytkami (Master <-> Slave).
	#endif
	
	

#ifdef MASTER_OR_SINGLE
	// =======================================================================================
	// ZABEZPIECZENIA (SAFETY)
	// =======================================================================================
	#define INACTIVITY_TIMEOUT 	8        	// Czas bezczynności (w minutach), po którym deska się wyłączy.

	// Progi napięciowe dla ogniwa (Li-Ion):
	
	// Progi napięciowe na jedno ogniwo (w woltach). Całkowite napięcie jest mnożone przez BAT_CELLS.
	#define CELL_LOW_LVL1     3.5       // Poziom 1: Delikatne piszczenie, zielona dioda.
	#define CELL_LOW_LVL2     3.3       // Poziom 2: Bateria prawie pusta, pomarańczowa dioda. Naładuj!
	#define CELL_LOW_DEAD     3.0       // Poziom 3: Odcięcie zasilania (Undervoltage lockout), czerwona dioda.
	#define CELL_LOW_DEAD     3.0       // Poziom 3: Krytycznie niskie napięcie. Odcięcie zasilania (jeśli BATTERY_LOW_SHUTOFF jest włączone), czerwona dioda.

	// TODO: A to nie powinno być na slave?
#endif

#define DC_CUR_LIMIT     		15        // Limit prądu stałego silnika (w Amperach). Chroni silnik i elektronikę.

// #define DEBUG_HALL_LEVEL
// #define DEBUG_ADC

#define RTT_LOG_MPU
#define TEST_HALL2LED

#endif /* CONFIG_DEBUG_H_ */