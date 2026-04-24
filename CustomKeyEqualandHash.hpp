#pragma once

#include <iostream>
#include <string>

class CustomKeyEqual {
    public:
        bool operator()(const std::string& lhs, const std::string& rhs) const {
            return lhs == rhs;
        }
};

class CustomHash {
    public:
        size_t operator()(const std::string& s) const {
            size_t sum = 0;
            for (int i{}; i < s.size(); ++i) {
                sum += s[i] * (i + 1);
            }
            return sum * 3 + 2; 
        }  
};
