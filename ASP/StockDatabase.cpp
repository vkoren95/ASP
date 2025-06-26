#include "StockDatabase.h"
#include "StockEntry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>

using namespace std;

void StockDatabase::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ne mogu otvoriti datoteku: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // zaglavlje

    int count = 0;
    while (getline(file, line) && count < 5000000) {
        stringstream ss(line);
        string date, ticker, val;
        StockEntry entry;

        getline(ss, date, ',');
        getline(ss, ticker, ',');
        entry.date = date;
        entry.ticker = ticker;

        try {
            getline(ss, val, ','); entry.open = stod(val);
            getline(ss, val, ','); entry.high = stod(val);
            getline(ss, val, ','); entry.low = stod(val);
            getline(ss, val, ','); entry.close = stod(val);
            getline(ss, val, ','); entry.volume = stod(val);
            getline(ss, val, ','); entry.dividends = stod(val);
            getline(ss, val, ','); entry.stock_splits = stod(val);
        }
        catch (...) {
            cerr << "Greska u parsiranju brojeva u redu: " << count << endl;
            continue;
        }

        entries.push_back(entry);
        tickerIndex[entry.ticker].push_back(entry);
        dateIndex[entry.date].push_back(entry);
        fastAccess[entry.ticker][entry.date] = entry;

        if (++count % 100000 == 0)
            cout << count << " redova ucitano..." << endl;
    }

    cout << "Ucitavanje gotovo. Ukupno: " << entries.size() << " redova." << endl;
}

void StockDatabase::manualInsert(const StockEntry& e) {
    entries.push_back(e);
    tickerIndex[e.ticker].push_back(e);
    dateIndex[e.date].push_back(e);
    fastAccess[e.ticker][e.date] = e;
}

vector<StockEntry> StockDatabase::getEntriesByDate(const string& date) {
    if (dateIndex.count(date)) return dateIndex[date];
    return {};
}

double StockDatabase::getAverageCloseForTicker(const string& ticker) {
    if (!tickerIndex.count(ticker)) return 0.0;
    double sum = 0.0;
    for (const auto& e : tickerIndex[ticker])
        sum += e.close;
    return tickerIndex[ticker].empty() ? 0.0 : sum / tickerIndex[ticker].size();
}

double StockDatabase::getMaxHighInRange(const string& ticker, const string& startDate, const string& endDate) {
    double maxHigh = -1.0;
    if (!tickerIndex.count(ticker)) return maxHigh;
    for (const auto& e : tickerIndex[ticker]) {
        if (e.date >= startDate && e.date <= endDate)
            maxHigh = max(maxHigh, e.high);
    }
    return maxHigh;
}

unordered_set<string> StockDatabase::getAllTickers() {
    unordered_set<string> result;
    for (const auto& pair : tickerIndex)
        result.insert(pair.first);
    return result;
}

bool StockDatabase::tickerExists(const string& ticker) {
    return tickerIndex.count(ticker);
}

int StockDatabase::countDaysWithCloseAbove(double threshold) {
    unordered_set<string> days;
    for (const auto& e : entries) {
        if (e.close > threshold)
            days.insert(e.date);
    }
    return static_cast<int>(days.size());
}

vector<pair<string, double>> StockDatabase::getDatesAndClosePrices(const string& ticker) {
    vector<pair<string, double>> result;
    if (!tickerIndex.count(ticker)) return result;
    for (const auto& e : tickerIndex[ticker])
        result.emplace_back(e.date, e.close);
    return result;
}

double StockDatabase::getTotalVolume(const string& ticker) {
    double total = 0.0;
    if (!tickerIndex.count(ticker)) return 0.0;
    for (const auto& e : tickerIndex[ticker])
        total += e.volume;
    return total;
}

bool StockDatabase::hasData(const string& ticker, const string& date) {
    return fastAccess.count(ticker) && fastAccess[ticker].count(date);
}

pair<double, double> StockDatabase::getOpenCloseFast(const string& ticker, const string& date) {
    if (hasData(ticker, date)) {
        const auto& e = fastAccess[ticker][date];
        return { e.open, e.close };
    }
    return { -1.0, -1.0 };
}

double StockDatabase::getDividendOnDate(const string& ticker, const string& date) {
    if (hasData(ticker, date)) return fastAccess[ticker][date].dividends;
    return 0.0;
}

vector<pair<string, double>> StockDatabase::getTop10ByVolumeOnDate(const string& date) {
    vector<pair<string, double>> result;
    if (!dateIndex.count(date)) return result;

    vector<pair<string, double>> volumes;
    for (const auto& e : dateIndex[date])
        volumes.emplace_back(e.ticker, e.volume);

    sort(volumes.begin(), volumes.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    for (size_t i = 0; i < min(size_t(10), volumes.size()); ++i)
        result.push_back(volumes[i]);

    return result;
}

vector<pair<string, double>> StockDatabase::get5LowestCloseOverall() {
    priority_queue<pair<double, string>> pq;
    for (const auto& e : entries) {
        pq.emplace(e.close, e.ticker);
        if (pq.size() > 5) pq.pop();
    }

    vector<pair<string, double>> result;
    while (!pq.empty()) {
        auto [close, tick] = pq.top(); pq.pop();
        result.emplace_back(tick, close);
    }
    reverse(result.begin(), result.end());
    return result;
}

vector<pair<string, double>> StockDatabase::getTop5DividendsOverall() {
    unordered_map<string, double> map;
    for (const auto& e : entries)
        map[e.ticker] += e.dividends;

    priority_queue<pair<double, string>> pq;
    for (const auto& [ticker, div] : map) {
        pq.emplace(div, ticker);
        if (pq.size() > 5) pq.pop();
    }

    vector<pair<string, double>> result;
    while (!pq.empty()) {
        auto [div, tick] = pq.top(); pq.pop();
        result.emplace_back(tick, div);
    }
    reverse(result.begin(), result.end());
    return result;
}
