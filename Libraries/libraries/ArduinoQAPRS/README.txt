ArduinoQAPRS jest biblioteką dla środowska Arduino mającą na celu uprościć implementację APRS.
Do chwili obecnej testowana była na:
	- Arduino Mini Pro w wersji 16MHz. 

Podstawowe użycie sprowadza się do:
		- zainkludować #include "ArduinoQAPRS.h"
		- zainicjalizowac obiekt QAPRS.init(3,2):
			- pierwszy parametr to wejsciowy pin Arduino na którym stan niski zezwala na nadawanie
			- drugi parametr to wyjsciowy pin Arduino na którym logiczna 1 powinna uruchomić nadajnik
		- wywołać metodę send, np. QAPRS.send(from_addr, '0', dest_addr, '1', relays, packet_buffer) gdzie:
			- from_addr - string z adresem źródłowym pakietu, np. SQ5RWU
			- '0' - SSID nadawcy
			- dest_addr - string z adresem docelowym pakietu
			- '1' - SSID odbiorcy
			- relays - opcjonalny string z pośrednikami dla pakietu, np: "WIDE1 1" lub "WIDE2 2WIDE1 1"
			- packet_buffer - treść pakietu APRS do nadania
			
			Metoda zajmie się odpowiednim przygotowaniem pakietu ax.25 i wys�aniem go w momencie,
			kiedy pin wejściowy zezwoli na nadawanie: program sprawdza co 100ms obecność zezwolenia,
			max 20 prób. Jeśli nie uda się nadać pakietu metoda zwraca QAPRSReturnErrorTimeout (2). 
			Udane nadanie pakietu to QAPRSReturnOK (0)
		- istnieje także możliwość wywołania metody send do której przekazujemy 
			bufor z w pełni spreparowanym pakietem i jego długością.
			
Bardziej rozbudowana dokumentacja zostanie przedstawiona w póniejszym terminie.


Dokumentacja automatyczna z Doxygen: http://qyon.bitbucket.org/ArduinoQAPRS/doc/html/class_q_a_p_r_s_base.html

Przy opracowaniu projektu nieoceniona była pomoc projektów takich jak WhereAVR <http://garydion.com/projects/whereavr/> i Bertos APRS <http://www.bertos.org/use/examples-projects/arduino-aprs>