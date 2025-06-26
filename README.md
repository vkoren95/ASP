# ASP Projekt – Analiza povijesnih podataka o dionicama (C++)

## Opis
Projekt implementira optimiziranu bazu podataka nad povijesnim financijskim podacima iz CSV datoteke. Pomoću klase `StockDatabase` korisnik može izvršiti različite upite nad podacima, uz fokus na vremensku efikasnost.

## Ključne strukture
- `std::vector<StockEntry>` – glavni spremnik svih unosa
- `tickerIndex` i `dateIndex` – hash mape za brzi dohvat po oznaci ili datumu
- `fastAccess[ticker][date]` – za O(1) pristup ticker+datum kombinacijama

## Implementirani upiti

| Br. | Upit | Metoda |
|----|------|--------|
| 1. | Dohvati sve podatke o dionicama za određeni datum | `getEntriesByDate(date)` |
| 2. | Prosječna završna cijena dionice | `getAverageCloseForTicker(ticker)` |
| 3. | Najviša cijena za dionicu u rasponu | `getMaxHighInRange(ticker, start, end)` |
| 4. | Sve jedinstvene oznake dionica | `getAllTickers()` |
| 5. | Provjera postojanja tickera | `tickerExists(ticker)` |
| 6. | Broj dana s Close > X | `countDaysWithCloseAbove(threshold)` |
| 7. | Sve cijene zatvaranja za ticker | `getDatesAndClosePrices(ticker)` |
| 8. | Ukupan volumen trgovanja za ticker | `getTotalVolume(ticker)` |
| 9. | Provjeri postoji li ticker+datum | `hasData(ticker, date)` |
|10. | Cijene otvaranja i zatvaranja (O(1)) | `getOpenCloseFast(ticker, date)` |
|11. | Dividenda za ticker+datum | `getDividendOnDate(ticker, date)` |
|12. | Top 10 tickera po volumenu na datum | `getTop10ByVolumeOnDate(date)` |
|13. | 5 dionica s najnižim zatvaranjima | `get5LowestCloseOverall()` |
|14. | Top 5 tickera po ukupnim dividendama | `getTop5DividendsOverall()` |

## Ručni unos
Korisnik može unijeti novi zapis putem izbornika (opcija 15), nakon čega se svi indeksi automatski ažuriraju.

## Mjerenje vremena
Svi upiti mjere vrijeme izvršavanja pomoću `chrono::high_resolution_clock`.

## Primjeri CSV formata
