#pragma once

#include "StockEntry.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class StockDatabase {
private:
    std::vector<StockEntry> entries;
    std::unordered_map<std::string, std::vector<StockEntry>> tickerIndex;
    std::unordered_map<std::string, std::vector<StockEntry>> dateIndex;
    std::unordered_map<std::string, std::unordered_map<std::string, StockEntry>> fastAccess;

public:
    void loadFromCSV(const std::string& filename);
    void manualInsert(const StockEntry& e);

    std::vector<StockEntry> getEntriesByDate(const std::string& date);
    double getAverageCloseForTicker(const std::string& ticker);
    double getMaxHighInRange(const std::string& ticker, const std::string& startDate, const std::string& endDate);
    std::unordered_set<std::string> getAllTickers();
    bool tickerExists(const std::string& ticker);
    int countDaysWithCloseAbove(double threshold);
    std::vector<std::pair<std::string, double>> getDatesAndClosePrices(const std::string& ticker);
    double getTotalVolume(const std::string& ticker);
    bool hasData(const std::string& ticker, const std::string& date);
    std::pair<double, double> getOpenCloseFast(const std::string& ticker, const std::string& date);
    double getDividendOnDate(const std::string& ticker, const std::string& date);
    std::vector<std::pair<std::string, double>> getTop10ByVolumeOnDate(const std::string& date);
    std::vector<std::pair<std::string, double>> get5LowestCloseOverall();
    std::vector<std::pair<std::string, double>> getTop5DividendsOverall();
};
