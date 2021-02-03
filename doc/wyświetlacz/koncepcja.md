# Cel

Moduł wyświetlacza otrzymuje dane dotyczące gry i konfiguracji aby wyświetlić je na docelowym HW.

# Wymagania

- Wyświetlacz pokazuje pozostały czas graczy
- Pokazuje również stan (start/stop/pauza) i kto przegrał na czas
- Czas otrzymujemy z modułu game w sekundach
- Informacje o stanie i kto przegrał również
- Wyświetlacz bieże również udział w konfiguracji i interwencji sędziego
- Pozwala ustawić czas dla gracza w aktualnie wybranej opcji

# HW

Wyświetlacz nie jest do końca sprecyzowany:
- Może być jeden duży wyświetlacz
- Mogą być dwa mniejsze
- Wyświetlacz może być oparty o HD44780 (wyświetlacz alfanumeryczny)
- Wyświetlacz może być graficzny (Soft kontroluje każdy piksel)

Rozszerzenie - wysyłanie czasu do zewnętrznego urządzenia np. po UART albo WiFi.
- Wyświetlamy sam czas bez konfiguracji

# Co robi ten moduł?

Moduł konwertuje dane z modułu gry na format rozumiany przez HW.
Pozostały do końca czas graczy przechowujemy jako string.
Pozostałe wyświetlane elementy są włączane/wyłączane oddzielnymi funkcjami.

Kwestie konfiguracji początkowej, interwencji sędziego i komunikacji z zewnętrznymi urządzeniami zostaną dodane w póżniejszych wersjach.

