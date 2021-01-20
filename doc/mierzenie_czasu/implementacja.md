# Implementacja odmierzania czasu

Mierzenie czasu partii to główne zadanie zegara szachowego. Aby wspierać najpopularniejsze tryby potrzebujemy czterech parametrów:
- czasu początkowego
- bonusu na każdy ruch
- po ilu posunięciach następuje kontrola czasu
- bonus dodany po kontroli czasu

Te wartości są przechowywane w strukturze time_config. W module game są one obsługiwane tak, aby poprawnie przechowywać pozostały czas dla obu graczy, ilość ruchów do kontroli i bonusy. Moduł game zapamiętuje również kto aktualnie jest na posunięciu, komu jako pierwszemu skończył się czas oraz potrafi ustawiać początkowe wartości, pauzować i wznawiać grę.

Moduł game obsługuję również interwencje sędziego, która polega na wprowadzeniu nowych parametrów pozostałego czasu i numeru posunięcia. Wartości do konfiguracji czasu oraz do interwencji sędziowskiej są uzyskiwane z innych modułów programu.



