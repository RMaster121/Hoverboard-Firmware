#ifndef CONFIG_H
#define CONFIG_H

//#define REMOTE_AUTODETECT
				// ONLY test with 1-2A constant current power supply !!!! The charger with 1.5A might also do :-)
				// will drive the motor without hall input to detect the hall pins..

#ifdef REMOTE_AUTODETECT
	#define WINDOWS_RN		// adds a \r before every \n
	
	//#define RTT_REMOTE

	#define REMOTE_USART				0 	// 	1 is usually PA2/PA3 and the original master-slave 4pin header
																	//	0 is usually PB6/PB7 and the empty header close to the flash-header
																	//	2 is usually PB10/PB11 on stm32f103 boards
																	
#else

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
	//#define DEBUG_LED			// Użyj diod LED do debugowania (sygnalizacja stanów wewnętrznych kodu).

	// =======================================================================================
	// ROLA PŁYTKI (BOARD ROLE)
	// =======================================================================================
	// Wybierz TYLKO JEDNĄ opcję. Możesz to zrobić w platformio.ini z flagą -D MASTER lub SLAVE
	// #define MASTER		// Płytka główna w układzie dwóch płytek (Master steruje Slavem).
	// #define SLAVE			// Płytka wykonawcza w układzie dwóch płytek (słucha Mastera).
	// #define SINGLE			// Pojedyncza płytka sterująca (np. rower, hulajnoga, gokart).

	#if defined(MASTER) || defined(SINGLE)
		
		// ===================================================================================
		// METODA STEROWANIA (REMOTE CONTROL)
		// ===================================================================================
		// Wybierz TYLKO JEDNO źródło sterowania.
		
		#define REMOTE_DUMMY		// Tryb testowy. Silnik wykonuje automatyczne ruchy (zygzak).
			#ifdef REMOTE_DUMMY
				#define REMOTE_PERIOD 6 	// Okres w sekundach dla automatycznego ruchu testowego.
				#define TEST_HALL2LED		// Diagnostyka: diody LED migają w rytm sygnałów z czujników Halla silnika.
				#define RTT_REMOTE		// Logowanie danych i sterowanie przez debugger ST-Link (Real Time Transfer).
					#define WINDOWS_RN	// Formatowanie nowych linii dla Windows (\r\n).
			#endif
		//#define REMOTE_UART		// Sterowanie przez UART (np. Arduino, ESP32, Bluetooth). Prosty protokół.
		 // send the IMU data with RemoteUart or RemoteUartBus. Tested for 2.1.20 !

		//#define REMOTE_UARTBUS	// UART z adresowaniem (wiele desek na jednej magistrali).
				#ifdef REMOTE_UARTBUS
					#define SLAVE_ID	0	// Unikalny adres ID dla tej płytki na magistrali.
				#endif
		//#define REMOTE_CRSF		// Protokół CRSF (Crossfire/ELRS) dla aparatur RC. // https://github.com/RoboDurden/Hoverboard-Firmware-Hack-Gen2.x/issues/26
		//#define REMOTE_ROS2		// Integracja z ROS2 (Robot Operating System) dla robotów autonomicznych. // https://github.com/RoboDurden/Hoverboard-Firmware-Hack-Gen2.x/issues/122
		//#define REMOTE_ADC		// Sterowanie analogowe (potencjometry, manetka gazu, joystick).
									// Wymaga kalibracji przy starcie (trzymaj przycisk Power).
									// Podłączenie: Speed=PA2, Steer=PA3 (złącze Master-Slave). Zasilanie 3.3V.
									
		//#define REMOTE_OPTIMIZEPID	// Tryb do automatycznego dostrajania regulatora PID (zygzakowanie i analiza błędu).

		//#define REMOTE_OPTIMIZEPID	// Specjalny tryb do automatycznego dostrajania parametrów regulatora PID.
									// UŻYCIE:
									// 1. Wybierz ten tryb (zakomentuj inne REMOTE_*).
									// 2. Ustaw DRIVING_MODE na 1 (Speed) lub 3 (Odometer).
									// 3. Podłącz ST-Link i uruchom RTT Viewer (np. J-Link RTT Viewer).
									// 4. Wgraj soft. Silnik zacznie wykonywać ruchy "zygzak" (oscylacje).
									// 5. Obserwuj logi w RTT Viewer. Algorytm będzie zmieniał kp, ki, kd szukając najmniejszego błędu.
									// 6. Gdy wartości się ustabilizują, skopiuj je do defines.h (struktura PIDINIT_a3o).
								// will zigzag motor and optimize pid parameters for 1=speed or 3=iOdometer.
								// monitor with StmStudio/McuViewer or via ST-Link usb dongle RTT and openocd_rtt_32f1x.bat or PlatformIO RTT_Task

				#ifdef REMOTE_OPTIMIZEPID
					#define WINDOWS_RN		
				#endif


		// ===================================================================================
		// DODATKOWE OPCJE STEROWANIA
		// ===================================================================================
		//#define TEST_HALL2LED		// Wizualizacja stanów czujników Halla na diodach LED.
		//#define RTT_REMOTE		// Włącz logowanie przez RTT (wymaga debuggera).
		// uncoment for i2c_scanner and dump_i2c_registers to print to rtt :-)
			// needs either I2C_PB8PB9 or I2C_PB6PB7 and MPU_6050 or MPU_6050old or BMI_160 in your defines_2-x-y.h file

		//#define SEND_IMU_DATA 	// Wysyłaj dane z akcelerometru/żyroskopu przez UART (dla REMOTE_UART/UARTBUS).

		//#define PILOT_USER		// Włącz własny kod sterowania z pliku PilotUser.c.
		//#define PILOT_HOVERBIKE	// Eksperymentalne wykrywanie pedałowania (dla roweru elektrycznego).
		
		#define SPEED_COEFFICIENT   -1	// Skalowanie prędkości. -1 odwraca kierunek.
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
	
	#if defined(REMOTE_UART) || defined(REMOTE_UARTBUS) || defined(REMOTE_CRSF) || defined(REMOTE_ROS2)
		#define REMOTE_USART			0 	// Port do podłączenia zewnętrznego sterowania.
	#endif
	
	
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

#endif		// CONFIG_H