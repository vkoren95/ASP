#pragma once
#include <string>

struct StockEntry {
    std::string date;
    std::string ticker;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    double volume = 0.0;
    double dividends = 0.0;
    double stock_splits = 0.0;
};
