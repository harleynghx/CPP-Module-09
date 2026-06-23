#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>

class BitcoinExchange {
private:
    std::map<std::string, float> _database;
    bool isValidDate(const std::string& date) const;
    bool isValidValue(const std::string& value, float& valOut) const;

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void loadDatabase(const std::string& filename);
    void evaluateInput(const std::string& filename) const;
};

#endif