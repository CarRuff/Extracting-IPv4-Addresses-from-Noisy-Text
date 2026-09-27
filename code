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
    // Default values for failure.
    outAddress = 0;
    outPort = -1;

    bool found = false;
    unsigned long foundAddress = 0;
    int foundPort = -1;

    // Look at every position in the string as a possible
    // beginning of an IPv4 address.
    for (size_t i = 0; i < str.length(); i++)
    {
        // An address must begin with a digit.
        if (!std::isdigit(static_cast<unsigned char>(str[i])))
            continue;

        // Do not start an address immediately after a period
        // or colon. This prevents accepting part of a bad token.
        if (i > 0 &&
            (std::isdigit(static_cast<unsigned char>(str[i - 1])) ||
             str[i - 1] == '.' ||
             str[i - 1] == ':'))
        {
            continue;
        }

        size_t pos = i;
        int octets[4] = {0, 0, 0, 0};
        bool valid = true;

        // ----------------------------------------------------
        // Parse exactly four octets.
        // ----------------------------------------------------
        for (int octet = 0; octet < 4 && valid; octet++)
        {
            // Each octet must start with a digit.
            if (pos >= str.length() ||
                !std::isdigit(static_cast<unsigned char>(str[pos])))
            {
                valid = false;
                break;
            }

            size_t octetStart = pos;
            int value = 0;
            int digitCount = 0;

            // Read all consecutive digits.
            while (pos < str.length() &&
                   std::isdigit(static_cast<unsigned char>(str[pos])))
            {
                // More than 3 digits cannot be a valid octet.
                if (digitCount == 3)
                {
                    valid = false;
                    break;
                }

                // Convert the digit manually.
                int digit = str[pos] - '0';

                value = value * 10 + digit;

                digitCount++;
                pos++;
            }

            if (!valid)
                break;

            // No leading zero unless the octet is exactly "0".
            if (digitCount > 1 && str[octetStart] == '0')
            {
                valid = false;
                break;
            }

            // Octet must be between 0 and 255.
            if (value > 255)
            {
                valid = false;
                break;
            }

            octets[octet] = value;

            // The first three octets must be followed by periods.
            if (octet < 3)
            {
                if (pos >= str.length() || str[pos] != '.')
                {
                    valid = false;
                    break;
                }

                // Move past the period.
                pos++;
            }
        }

        // If the IPv4 portion failed, try the next possible
        // starting position in the input line.
        if (!valid)
            continue;

        // ----------------------------------------------------
        // Parse optional port.
        // ----------------------------------------------------
        int candidatePort = -1;

        if (pos < str.length() && str[pos] == ':')
        {
            // Move past the colon.
            pos++;

            // A colon MUST be followed by a port.
            if (pos >= str.length() ||
                !std::isdigit(static_cast<unsigned char>(str[pos])))
            {
                valid = false;
            }
            else
            {
                size_t portStart = pos;
                int portValue = 0;
                int digitCount = 0;

                // Read all port digits.
                while (pos < str.length() &&
                       std::isdigit(static_cast<unsigned char>(str[pos])))
                {
                    // Port can have at most 5 digits.
                    if (digitCount == 5)
                    {
                        valid = false;
                        break;
                    }

                    int digit = str[pos] - '0';

                    // Manual number accumulation.
                    portValue = portValue * 10 + digit;

                    digitCount++;
                    pos++;
                }

                if (valid)
                {
                    // No leading zero unless port is exactly "0".
                    if (digitCount > 1 && str[portStart] == '0')
                    {
                        valid = false;
                    }

                    // Valid port range is 0 through 65535.
                    if (portValue > 65535)
                    {
                        valid = false;
                    }

                    if (valid)
                    {
                        candidatePort = portValue;
                    }
                }
            }
        }

        if (!valid)
            continue;

        // ----------------------------------------------------
        // Check the right boundary.
        //
        // A period or colon directly after the candidate means
        // the token is malformed.
        // ----------------------------------------------------
        if (pos < str.length() &&
            (str[pos] == '.' || str[pos] == ':'))
        {
            continue;
        }

        // ----------------------------------------------------
        // Build the 32-bit address.
        //
        // A.B.C.D becomes:
        //
        // AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
        // ----------------------------------------------------
        unsigned long candidateAddress =
            (static_cast<unsigned long>(octets[0]) << 24) |
            (static_cast<unsigned long>(octets[1]) << 16) |
            (static_cast<unsigned long>(octets[2]) << 8) |
             static_cast<unsigned long>(octets[3]);

        // ----------------------------------------------------
        // Exactly one valid address is allowed.
        // ----------------------------------------------------
        if (found)
        {
            outAddress = 0;
            outPort = -1;
            return false;
        }

        // Save the valid candidate.
        found = true;
        foundAddress = candidateAddress;
        foundPort = candidatePort;

        // Move i to the end of this candidate so we do not
        // accidentally examine digits inside the same address
        // as new starting positions.
        if (pos > 0)
            i = pos - 1;
    }

    // --------------------------------------------------------
    // Return the result.
    // --------------------------------------------------------
    if (found)
    {
        outAddress = foundAddress;
        outPort = foundPort;
        return true;
    }

    // Failure values were already assigned at the beginning.
    return false;
}


int main()
{
    std::string input;

    while (true)
    {
        std::cout << "Enter text: ";
        std::getline(std::cin, input);

        // Stop when the user enters exactly END.
        if (input == "END")
        {
            std::cout << "Program terminated." << std::endl;
            break;
        }

        unsigned long address;
        int port;

        if (extractIPv4(input, address, port))
        {
            // Recover each octet from the 32-bit address.
            unsigned long a = (address >> 24) & 0xFF;
            unsigned long b = (address >> 16) & 0xFF;
            unsigned long c = (address >> 8) & 0xFF;
            unsigned long d = address & 0xFF;

            std::cout << "Extracted IPv4 address: "
                      << a << "."
                      << b << "."
                      << c << "."
                      << d
                      << " (decimal value: "
                      << address
                      << ", port: ";

            if (port == -1)
            {
                std::cout << "none";
            }
            else
            {
                std::cout << port;
            }

            std::cout << ")" << std::endl;
        }
        else
        {
            std::cout << "No valid IPv4 address found." << std::endl;
        }
    }

    return 0;
}
