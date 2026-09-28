/*
  SEMESTER:        EECS 581 Fall 2026
  ASSIGNMENT:      Extracting IPv4 Addresses from Noisy Text
  FILE:            code.c

  DESCRIPTION:     Searches for a valid IPv4 Address in a string of text

  AUTHOR:          Carter Ruff
  SOURCES:         ChatGPT (GPT-5.6 Sol)
  CREATION DATE:   09/27/2026
  
  Note: Any comments containing "Generated:" were added by ChatGPT
*/

// Libraries
#include <iostream>
#include <string>
#include <cctype>

// Returns true if a valid address was found, false otherwise.
// On success: outAddress holds the 32-bit value,
// and outPort holds the port number, or -1 if no port was present.
// On failure: outAddress is set to 0 and outPort is set to -1.
bool extractIPv4(const std::string& str,
                 unsigned long& outAddress,
                 int& outPort)
{
    // Generated: Default values for failure.
    outAddress = 0;
    outPort = -1;

    // Variables to store our results
    bool found = false;
    unsigned long foundAddress = 0;
    int foundPort = -1;

    // Generated: Look at every position in the string as a possible
    // beginning of an IPv4 address.
    for (size_t i = 0; i < str.length(); i++)
    {
        // Generated: An address must begin with a digit.
        if (!std::isdigit(static_cast<unsigned char>(str[i])))
            continue;

        // The mistake ChatGPT made was this line
        // Originally:
        // if (i > 0 && (str[i - 1] == '.' || str[i - 1] == ':'))
        //        continue;
        // This caused it to accept 192.168.1.1.1 despite being invalid
        // It Accepted because it saw 68.1.1.1 as valid
        // I made it reject a starting position if it was in the middle of digits
        // std::isdigit(static_cast<unsigned char>(str[i - 1])) was added to stop it from starting in the middle.

        // Make sure to not start an address in the middle of a number or after a period or colon
        if (i > 0 && (std::isdigit(static_cast<unsigned char>(str[i - 1])) || str[i - 1] == '.' || str[i - 1] == ':'))
        {
            continue;
        }

        // Separate position for address
        size_t pos = i;

        // Store the found octets
        int octets[4] = {0, 0, 0, 0};
      
        // We start assuming its valid
        bool valid = true;

        // ----------------------------------------------------
        // Generated: Parse exactly four octets.
        // ----------------------------------------------------
        for (int octet = 0; octet < 4 && valid; octet++)
        {
            // Generated: Each octet must start with a digit.
            if (pos >= str.length() ||
                !std::isdigit(static_cast<unsigned char>(str[pos])))
            {
                // If the octect doesn't start with a digit, its not valid
                valid = false;
                break;
            }

            // Starting Position remembered
            size_t octetStart = pos;

            // Stores octet's value and how many digits
            int value = 0;
            int digitCount = 0;

            // Generated: Read all consecutive digits.
            while (pos < str.length() && std::isdigit(static_cast<unsigned char>(str[pos])))
            {
                // Generated: More than 3 digits cannot be a valid octet.
                if (digitCount == 3)
                {
                    // If the octet has more than 3 digits, its invalid
                    valid = false;
                    break;
                }

                // Generated: Convert the digit manually.
                int digit = str[pos] - '0';

                // Makes the octet value one value at a time
                value = value * 10 + digit;

                // Move to the next digit
                digitCount++;
                pos++;
            }

            // Checkpoint to make sure its still valid
            if (!valid)
                break;

            // Generated: No leading zero unless the octet is exactly "0".
            if (digitCount > 1 && str[octetStart] == '0')
            { 
                // If the octect has a leading 0 and isn't 0 then its not valid
                valid = false;
                break;
            }

            // Generated: Octet must be between 0 and 255.
            if (value > 255)
            {
                // If the octect's value is above 255 or less than 0 its invalid
                valid = false;
                break;
            }

            // Store the octect
            octets[octet] = value;

            // Generated: The first three octets must be followed by periods.
            if (octet < 3)
            {
                // Checks for a period in between the octect
                if (pos >= str.length() || str[pos] != '.')
                {
                    // If it doesn't have a period its invalid
                    valid = false;
                    break;
                }

                // Move past the period.
                pos++;
            }
        }

        // Generated: If the IPv4 portion failed, try the next possible
        // starting position in the input line.
        if (!valid)
            continue;

        // ----------------------------------------------------
        // Generated: Parse optional port.
        // ----------------------------------------------------
        int candidatePort = -1;

        // looks for a colon after the octects
        if (pos < str.length() && str[pos] == ':')
        {
            // Generated: Move past the colon.
            pos++;

            // Generated: A colon MUST be followed by a port.
            if (pos >= str.length() ||
                !std::isdigit(static_cast<unsigned char>(str[pos])))
            {
                // if its just a colon then its invalid
                valid = false;
            }
            else
            {
                // Variables for finding the port
                size_t portStart = pos;
                int portValue = 0;
                int digitCount = 0;

                // Generated: Read all port digits.
                while (pos < str.length() &&
                       std::isdigit(static_cast<unsigned char>(str[pos])))
                {
                    // If we are already at 5, then we are reading a sixth digit
                    if (digitCount == 5)
                    {
                        // if the port has more than 5 digits its invalid
                        valid = false;
                        break;
                    }

                    // Prepares the digit to be added manual
                    int digit = str[pos] - '0';

                    // Generated: Manual number accumulation.
                    portValue = portValue * 10 + digit;

                    // Move to the next digit of the port
                    digitCount++;
                    pos++;
                }

                // If we are still valid at this point
                if (valid)
                {
                    // Generated: No leading zero unless port is exactly "0".
                    if (digitCount > 1 && str[portStart] == '0')
                    {  
                        // If the port has a leading 0 its invalid 
                        valid = false;
                    }

                    // Generated: Valid port range is 0 through 65535.
                    if (portValue > 65535)
                    {
                        // if the port value isn't with in 0 through 65535, then its invalid
                        valid = false;
                    }

                    // if we are still valid
                    if (valid)
                    {    
                        // Save the port number
                        candidatePort = portValue;
                    }
                }
            }
        }

        // Checkpoint to make sure we are still valid
        if (!valid)
            continue;

        // ----------------------------------------------------
        // Generated: Check the right boundary.
        //
        // A period or colon directly after the candidate means
        // the token is malformed.
        // ----------------------------------------------------

        // Check for a period or colon after the port or end of address
        if (pos < str.length() &&
            (str[pos] == '.' || str[pos] == ':'))
        {
            continue;
        }

        // ----------------------------------------------------
        // Generated: Build the 32-bit address.
        //
        // A.B.C.D becomes:
        //
        // AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
        // ----------------------------------------------------

        // Build the Address using the octets 
        unsigned long candidateAddress =
            (static_cast<unsigned long>(octets[0]) << 24) |
            (static_cast<unsigned long>(octets[1]) << 16) |
            (static_cast<unsigned long>(octets[2]) << 8) |
             static_cast<unsigned long>(octets[3]);

        // ----------------------------------------------------
        // Generated: Exactly one valid address is allowed.
        // ----------------------------------------------------

        // Check to make sure we haven't already found a valid address 
        if (found)
        {
            outAddress = 0;
            outPort = -1;
            return false;
        }

        // Generated: Save the valid candidate.
        found = true;
        foundAddress = candidateAddress;
        foundPort = candidatePort;

        // Generated: Move i to the end of this candidate so we do not
        // accidentally examine digits inside the same address
        // as new starting positions.
        if (pos > 0)
            i = pos - 1;
    }

    // --------------------------------------------------------
    // Generated: Return the result.
    // --------------------------------------------------------
    if (found)
    {
        outAddress = foundAddress;
        outPort = foundPort;
        return true;
    }

    // Generated: Failure values were already assigned at the beginning.
    return false;
}


int main()
{
    // Input Variable
    std::string input;

    // Main loop
    while (true)
    { 
        // Promt the user
        std::cout << "Enter text: ";
        std::getline(std::cin, input);
    
        // Generated: Stop when the user enters exactly END.
        if (input == "END")
        {
            // End the program when END is typed
            std::cout << "Program terminated." << std::endl;
            break;
        }

        // Address and port variables
        unsigned long address;
        int port;

        // If their is a found address
        if (extractIPv4(input, address, port))
        {
            // Generated: Recover each octet from the 32-bit address.
            unsigned long a = (address >> 24) & 0xFF;
            unsigned long b = (address >> 16) & 0xFF;
            unsigned long c = (address >> 8) & 0xFF;
            unsigned long d = address & 0xFF;

            // Print the Address
            std::cout << "Extracted IPv4 address: "
                      << a << "."
                      << b << "."
                      << c << "."
                      << d
                      << " (decimal value: "
                      << address
                      << ", port: ";

            // If there isn't a port
            if (port == -1)
            {
                // print none for port
                std::cout << "none";
            }
            // if there is a port
            else
            {
                // print the port
                std::cout << port;
            }

            std::cout << ")" << std::endl;
        }
        // If no valid address is found
        else
        {
            // Print none found
            std::cout << "No valid IPv4 address found." << std::endl;
        }
    }

    return 0;
}
