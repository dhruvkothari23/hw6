#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Array to store the 5 converted values
        unsigned long long w[5] = {0};
        
        // Process the string in groups of 6 characters, starting from the end
        int length = k.length();
        int group = 4; // Start with the last group (index 4)
        
        for (int i = length - 1; i >= 0; i -= 6) {
            // Calculate the start position for this group (ensuring we don't go below 0)
            int start = std::max(0, i - 5);
            // Calculate how many characters are in this group
            int chars_in_group = i - start + 1;
            
            // Convert this group of characters to a base-36 value
            unsigned long long value = 0;
            for (int j = 0; j < chars_in_group; j++) {
                char c = k[start + j];
                HASH_INDEX_T digit = letterDigitToNumber(c);
                // Use base conversion: value = value * 36 + digit
                value = value * 36 + digit;
            }
            
            // Store the value in the w array
            w[group] = value;
            
            // Move to the previous group
            group--;
            
            // If we've processed all characters, break
            if (start == 0) break;
        }
        
        // Print the w values as required
        for (int i = 0; i < 5; i++) {
            std::cout << "w[" << i << "] = " << w[i] << std::endl;
        }
        
        // Compute the hash value using the formula: h(k) = r[0]w[0] + r[1]w[1] + r[2]w[2] + r[3]w[3] + r[4]w[4]
        unsigned long long result = 0;
        for (int i = 0; i < 5; i++) {
            result += (unsigned long long)rValues[i] * w[i];
        }
        
        return result;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Convert to lowercase if it's an uppercase letter
        if (letter >= 'A' && letter <= 'Z') {
            letter = letter - 'A' + 'a';
        }
        
        // For lowercase letters: a=0, b=1, ..., z=25
        if (letter >= 'a' && letter <= 'z') {
            return letter - 'a';
        }
        // For digits: '0'=26, '1'=27, ..., '9'=35
        else if (letter >= '0' && letter <= '9') {
            return letter - '0' + 26;
        }
        
        // Default case (shouldn't happen with valid input)
        return 0;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif