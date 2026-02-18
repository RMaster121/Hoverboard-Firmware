#include "commsMasterSlave.h"
#include "string.h"
#include "comms.h"


#ifdef MASTER
	PacketS2M receivedTelemetry;
#else
	PacketM2S receivedCommand;
#endif

volatile uint8_t newDataReceived = 0;
static uint32_t rx_read_pos = 0;

// Prywatny bufor parsera do składania ramki bajt po bajcie
static uint8_t aReceiveBuffer[sizeof(SerialReceive)];
static int16_t iReceivePos = -1; // -1 oznacza oczekiwanie na bajt startu

void Comms_Init(void)
{
	rx_read_pos = 0;
	newDataReceived = 0;
	#ifdef MASTER
        memset(&receivedTelemetry, 0, sizeof(receivedTelemetry));
    #else
        memset(&receivedCommand, 0, sizeof(receivedCommand));
    #endif

	USART1_Init(USART1_BAUD);
}

void ProcessReceived(const SerialReceive* pData)
{
	#ifdef MASTER
		memcpy(&receivedTelemetry, pData, sizeof(receivedTelemetry));
	#else
		memcpy(&receivedCommand, pData, sizeof(receivedCommand));
	#endif //TODO: może usunąć boilerplate - do funkcji

    newDataReceived = 1;
}

/**
 * @brief Przetwarza dane znajdujące się w buforze kołowym DMA.
 * * Funkcja ta jest "konsumentem" danych, które DMA (producent) wpisało do usart1_rx_buf.
 * Działa dopóki nie dogoni aktualnego wskaźnika zapisu sprzętowego.
 * * @param rx_write_pos Aktualna pozycja, na której DMA skończyło pisać (head).
 * Wartość ta jest obliczana w przerwaniu na podstawie licznika transferów DMA.
 */
void ProcessMasterSlaveRx(uint32_t rx_write_pos)
{
    // Pętla wykonuje się, dopóki nasz programowy wskaźnik odczytu (rx_read_pos)
    // nie zrówna się z pozycją, do której dane wpisał sprzęt (rx_write_pos).
    while (rx_read_pos != rx_write_pos)
    {
        // 1. Pobierz bajt z bufora kołowego DMA
        uint8_t cRead = usart1_rx_buf[rx_read_pos];

        // 2. Przesuń wskaźnik odczytu o jeden krok dalej
        rx_read_pos++;
        
        // Jeśli dojdziemy do końca fizycznego rozmiaru tablicy (128), 
        // musimy zawrócić na początek (indeks 0) - to realizuje ideę bufora kołowego.
        if (rx_read_pos >= 128)  //TODO: do stałej
        {
            rx_read_pos = 0;
        }

        // --- LOGIKA PARSERA (Maszyna Stanów) ---

        // KROK A: Szukanie początku ramki.
        // Jeśli bajt to 0xAA (lub twój '/'), uznajemy, że to start nowej wiadomości.
        if (cRead == COMM_START_BYTE) 
        {
            iReceivePos = 0; // Ustawiamy licznik na 0 - od teraz zbieramy bajty do aReceiveBuffer
        }

        // KROK B: Zbieranie bajtów ramki.
        // Jeśli iReceivePos >= 0, oznacza to, że jesteśmy w trakcie odbierania poprawnej ramki.
        if (iReceivePos >= 0)
        {
            // Przepisujemy bajt z bufora DMA do lokalnego bufora roboczego ramki
            aReceiveBuffer[iReceivePos++] = cRead;

            // KROK C: Sprawdzenie, czy mamy już kompletną strukturę.
            // Porównujemy aktualną pozycję z rozmiarem struktury (np. 7 bajtów dla M2S).
            if (iReceivePos == sizeof(SerialReceive))
            {
                // Mamy komplet bajtów! Natychmiast resetujemy parser (-1), 
                // aby ewentualne śmieci po ramce nie psuły logiki.
                iReceivePos = -1; 
                
                // Rzutujemy bufor bajtów na wskaźnik do naszej struktury (PacketM2S lub PacketS2M)
                SerialReceive* pData = (SerialReceive*)aReceiveBuffer;
                
                // KROK D: Weryfikacja integralności (CRC).
                // Obliczamy CRC dla wszystkich odebranych bajtów OPRÓCZ pola checksum (ostatnie 2 bajty).
                if (pData->checksum == CalcCRC(aReceiveBuffer, sizeof(SerialReceive) - 2))
                {
                    // Jeśli suma kontrolna się zgadza, przekazujemy dane do logiki biznesowej
                    // (np. ustawienie PWM silników, aktualizacja telemetrii).
                    ProcessReceived(pData);
                    // Błąd CRC - ramka zostaje odrzucona. System jest bezpieczny, 
                    // bo nie zareaguje na przekłamane dane.
                }
            }
        }
    }
}

#ifdef MASTER
	void SendToSlave(int16_t rpm, uint8_t flags) {
    SerialSend oData;
		oData.start = COMM_START_BYTE;
		oData.target_rpm = rpm;
		oData.flags = flags;
		oData.checksum = CalcCRC((uint8_t*)&oData, sizeof(oData) - 2);
		SendBuffer(USART_MASTERSLAVE, (uint8_t*)&oData, sizeof(oData));
	}
#else
	void SendToMaster(int16_t pitch, int16_t rpm, uint8_t err) {
		SerialSend oData;
		oData.start = COMM_START_BYTE;
		oData.pitch = pitch;
		oData.actual_rpm = rpm;
		oData.error_code = err;
		oData.checksum = CalcCRC((uint8_t*)&oData, sizeof(oData) - 2);
		SendBuffer(USART_MASTERSLAVE, (uint8_t*)&oData, sizeof(oData));
	}
#endif
