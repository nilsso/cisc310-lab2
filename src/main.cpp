// CISC-310: Lab 2 -- Nils Olsson
//
// Driver program in charge of:
// - Streaming data from a MILES CSV file and constructing a list of MILES code objects
// - Streaming data from a sequence CSV file into a list of sequences (queues)
// - Constructing a decoder from the list of MILES code objects
// - Using the decoder to decode the sequences and collecting found IDs
// - Outputting the found MILES code IDs in CSV format
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

#include "util.hpp"
#include "queue.hpp"
#include "decoder.hpp"

// Facet extension for use in CSV file stream extraction
struct CSV_facet: std::ctype<char> {
    static const mask *make_table() {
        static std::vector<mask> v(
                classic_table(),
                classic_table() + table_size);
        v[','] |= space;
        return &v[0];
    }
    CSV_facet(std::size_t refs = 0):
        ctype{make_table(), false, refs}
    {}
};

//! Validate std::vector as code pattern
// @param pattern Vector to validate
bool validate_pattern(const std::vector<int> &pattern);

// Default file names (paths)
const std::string MILES_FILE_DEFAULT = "miles.csv";
const std::string SEQS_FILE_DEFAULT = "seqs.csv";

// Help message
const std::string HELP =
"CS 310: Lab 2\n"
"Usage:\n"
"  ./src/lab2 [-h] [miles] [seqs]\n"
"\n"
"Optional arguments:\n"
"    -h       Print this help message\n"
"    miles    MILES pattern file (default: miles.csv)\n"
"    seqs     Sequence file (default: seqs.csv)\n";

// Entry point
int main(int argc, char *argv[]) {
    if (argc > 1 and strcmp(argv[1], "-h") == 0) {
        std::cout << HELP << std::endl;
        return EXIT_SUCCESS;
    }

    // Get file paths
    std::string path_m = argc >= 2 ? argv[1] : MILES_FILE_DEFAULT;
    std::string path_s = argc >= 3 ? argv[2] : SEQS_FILE_DEFAULT;

    // Variables for CSV reading
    int temp;
    std::string line;
    std::istringstream iss;
    iss.imbue(std::locale(iss.getloc(), new CSV_facet));

    // Collect codes
    std::ifstream ifs_m(path_m);
    if (!ifs_m.is_open()) {
        std::cerr
            << "Error: failed to open MILES file \""
            << path_m << "\" to be read." << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<MILES::Code> codes;
    while (ifs_m >> line) {
        iss.clear();
        iss.str(std::move(line));
        std::vector<int> pattern;
        while (iss >> temp)
            pattern.push_back(temp);
        if (!validate_pattern(pattern)) {
            std::cout
                << "Error: Malformed MILES code \"" << util::join(pattern) << "\"." << std::endl
                << "  (MILES codes consist of 6 sequential numbers and an ID number.)" << std::endl;
            return EXIT_FAILURE;
        }
        int id = pattern.back();
        pattern.pop_back();
        codes.emplace_back(std::move(pattern), id);
    }
    ifs_m.close();

    // Collect sequences
    std::ifstream ifs_s(path_s);
    if (!ifs_s.is_open()) {
        std::cerr
            << "Error: failed to open sequence file \""
            << path_s << "\" to be read." << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<queue<int>> sequences;
    while (ifs_s >> line) {
        iss.clear();
        iss.str(std::move(line));
        queue<int> sequence;
        while (iss >> temp)
            sequence.enqueue(temp);
        sequences.push_back(sequence);
    }
    ifs_s.close();

    // Process sequences and output found code ID's
    MILES::Decoder decoder(std::move(codes));
    for (auto &sequence: sequences) {
        auto found_ids = decoder.decode(std::move(sequence));
        std::cout << util::join(found_ids.peek(), ",", "", "") << std::endl;
    }

    return EXIT_SUCCESS;
}

bool validate_pattern(const std::vector<int> &pattern) {
    if (pattern.empty() || pattern[0] != 0 || pattern.size() != 7)
        return false;
    for (unsigned long i = 1; i < pattern.size() - 1; i++) {
        if (pattern[i-1] >= pattern[i])
            return false;
    }
    return true;
}
