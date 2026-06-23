#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _database(other._database) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        _database = other._database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
    int year, month, day;
    std::stringstream ss(date.substr(0, 4) + " " + date.substr(5, 2) + " " + date.substr(8, 2));
    ss >> year >> month >> day;
    if (ss.fail() || !ss.eof()) return false;
    if (year < 2000 || month < 1 || month > 12 || day < 1 || day > 31) return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (isLeap ? 29 : 28)) return false;
    }
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& valOut) const {
    char* endptr;
    double val = std::strtod(valueStr.c_str(), &endptr);
    if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') return false;
    if (val < 0) {
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    if (val > 1000.0) {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    valOut = static_cast<float>(val);
    return true;
}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string date = line.substr(0, commaPos);
            float rate = std::strtof(line.substr(commaPos + 1).c_str(), NULL);
            _database[date] = rate;
        }
    }
}

void BitcoinExchange::evaluateInput(const std::string& filename) const {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        size_t delimPos = line.find(" | ");
        if (delimPos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        std::string date = line.substr(0, delimPos);
        std::string valueStr = line.substr(delimPos + 3);
        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }
        float value;
        if (!isValidValue(valueStr, value)) continue;

        std::map<std::string, float>::const_iterator it = _database.upper_bound(date);
        if (it == _database.begin()) {
            std::cerr << "Error: no older date found in database for => " << date << std::endl;
            continue;
        }
        --it;
        std::cout << date << " => " << value << " = " << (value * it->second) << std::endl;
    }
}