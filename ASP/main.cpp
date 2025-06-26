#include "StockDatabase.h"
#include <iostream>
#include <chrono>

using namespace std;

int main() {
    StockDatabase db;
    string filename = "all_stock_data.csv";
    db.loadFromCSV(filename);

    int opcija = -1;
    while (opcija != 0) {
        cout << "\nOdaberi opciju:\n";
        cout << "1 - Dohvati sve podatke za datum\n";
        cout << "2 - Prosjecna zatvaranja cijena za ticker\n";
        cout << "3 - Najvisa cijena u rasponu\n";
        cout << "4 - Svi tickeri\n";
        cout << "5 - Postoji li ticker\n";
        cout << "6 - Broj dana s close > X\n";
        cout << "7 - Datumi i close za ticker\n";
        cout << "8 - Ukupan volumen za ticker\n";
        cout << "9 - Postoji li zapis za ticker+datum\n";
        cout << "10 - Open/Close u O(1)\n";
        cout << "11 - Dividenda za ticker+datum\n";
        cout << "12 - Top 10 po volumenu na datum\n";
        cout << "13 - 5 najnizih close cijena\n";
        cout << "14 - Top 5 dividendi ukupno\n";
        cout << "15 - Rucni unos jednog zapisa\n";
        cout << "0 - Izlaz\n";

        cin >> opcija;
        auto start = chrono::high_resolution_clock::now();

        StockEntry e;
        string ticker, datum, startDate, endDate;
        double vrijednost;

        switch (opcija) {
        case 1:
            cout << "Unesi datum (YYYY-MM-DD): ";
            cin >> datum;
            for (auto& x : db.getEntriesByDate(datum)) {
                cout << x.ticker << " " << x.close << endl;
            }
            break;
        case 2:
            cout << "Unesi oznaku dionice: ";
            cin >> ticker;
            cout << "Prosjek: " << db.getAverageCloseForTicker(ticker) << endl;
            break;
        case 3:
            cout << "Unesi ticker, pocetni datum, krajnji datum: ";
            cin >> ticker >> startDate >> endDate;
            cout << "Max high: " << db.getMaxHighInRange(ticker, startDate, endDate) << endl;
            break;
        case 4:
            for (const auto& t : db.getAllTickers())
                cout << t << endl;
            break;
        case 5:
            cout << "Unesi ticker: ";
            cin >> ticker;
            cout << (db.tickerExists(ticker) ? "Postoji" : "Ne postoji") << endl;
            break;
        case 6:
            cout << "Unesi prag: ";
            cin >> vrijednost;
            cout << "Broj dana: " << db.countDaysWithCloseAbove(vrijednost) << endl;
            break;
        case 7:
            cout << "Unesi ticker: ";
            cin >> ticker;
            for (auto& p : db.getDatesAndClosePrices(ticker))
                cout << p.first << ": " << p.second << endl;
            break;
        case 8:
            cout << "Unesi ticker: ";
            cin >> ticker;
            cout << "Ukupan volumen: " << db.getTotalVolume(ticker) << endl;
            break;
        case 9:
            cout << "Unesi ticker i datum: ";
            cin >> ticker >> datum;
            cout << (db.hasData(ticker, datum) ? "Postoji" : "Ne postoji") << endl;
            break;
        case 10:
            cout << "Unesi ticker i datum: ";
            cin >> ticker >> datum;
            {
                auto [open, close] = db.getOpenCloseFast(ticker, datum);
                cout << "Open: " << open << ", Close: " << close << endl;
            }
            break;
        case 11:
            cout << "Unesi ticker i datum: ";
            cin >> ticker >> datum;
            cout << "Dividenda: " << db.getDividendOnDate(ticker, datum) << endl;
            break;
        case 12:
            cout << "Unesi datum: ";
            cin >> datum;
            for (auto& p : db.getTop10ByVolumeOnDate(datum))
                cout << p.first << ": " << p.second << endl;
            break;
        case 13:
            for (auto& p : db.get5LowestCloseOverall())
                cout << p.first << ": " << p.second << endl;
            break;
        case 14:
            for (auto& p : db.getTop5DividendsOverall())
                cout << p.first << ": " << p.second << endl;
            break;
        case 15:
            cout << "Unesi: datum ticker open high low close volume dividends splits\n";
            cin >> e.date >> e.ticker >> e.open >> e.high >> e.low >> e.close >> e.volume >> e.dividends >> e.stock_splits;
            db.manualInsert(e);
            cout << "Zapis unesen." << endl;
            break;
        default:
            if (opcija != 0)
                cout << "Nepoznata opcija.\n";
            break;
        }

        auto end = chrono::high_resolution_clock::now();
        cout << "Vrijeme izvrsavanja: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    }

    return 0;
}
