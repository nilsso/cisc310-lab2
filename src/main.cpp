// CISC-310: Lab 2 -- Nils Olsson
//
// Driver program in charge of:
// - Streaming data from a MILES CSV file and constructing a list of MILES code objects
// - Streaming data from a sequence CSV file into a list of sequences (queues)
// - Constructing a decoder from the list of MILES code objects
// - Using the decoder to decode the sequences and collecting found IDs
// - Outputting the found MILES code IDs in CSV format
#include <iostream>
#include <fstream>
#include <vector>

// Note for cxxopts (https://github.com/jarro2783/cxxopts):
// This is an open-source commandline option parser that facilitates
// building robust commandline behavior easily.
#include "cxxopts.hpp"

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

// Validate std::vector as code pattern
// @param pattern Vector to validate
bool validate_pattern(const std::vector<int> &pattern);

// Default file names (paths)
const std::string MILES_FILE_DEFAULT = "miles.csv";
const std::string SEQ_FILE_DEFAULT = "seq.csv";

// Entry point
int main(int argc, char *argv[]) {
    // Construct command-line arguments parser
    cxxopts::Options options = { argv[0], "CS 310: Lab 2" };
    options
        .add_options()
        ("h,help", "Print this help message")
        ("m,miles", "MILES pattern file",
            cxxopts::value<std::string>()->default_value(MILES_FILE_DEFAULT))
        ("s,sequence", "Value sequence file",
            cxxopts::value<std::string>()->default_value(SEQ_FILE_DEFAULT))
        ("n,normalize", "Normalize input sequences")
        ("o,output", "Output file for found MILES codes in CSV format",
            cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    // Get file paths
    std::string path_m = result["miles"].as<std::string>();
    std::string path_s = result["sequence"].as<std::string>();
    if (path_m == path_s) {
        std::cerr
            << "Error: MILES and sequence files cannot be the same."
            << std::endl;
        return EXIT_FAILURE;
    }

    // Open files
    std::ifstream ifs_m = std::ifstream(path_m, std::ios::in);
    std::ifstream ifs_s = std::ifstream(path_s, std::ios::in);
    if (!(ifs_m.is_open() && ifs_s.is_open())) {
        if (!ifs_m.is_open())
            std::cerr
                << "Error: failed to open MILES file \""
                << path_m << "\" to be read." << std::endl;
        if (!ifs_s.is_open())
            std::cerr
                << "Error: failed to open sequence file \""
                << path_s << "\" to be read." << std::endl;
        ifs_m.close();
        ifs_s.close();
        return EXIT_FAILURE;
    }

    // Variables for CSV reading
    int temp;
    std::string line;
    std::istringstream iss;
    iss.imbue(std::locale(iss.getloc(), new CSV_facet));

    // Collect codes
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
    std::vector<queue<int>> sequences;
    while (ifs_s >> line) {
        iss.clear();
        iss.str(std::move(line));
        queue<int> sequence;
        while (iss >> temp) {
            sequence.enqueue(temp);
        }
        if (result.count("normalize"))
            sequence.normalize();
        sequences.push_back(sequence);
    }
    ifs_s.close();

    // Set output stream (standard out, or to file)
    std::ostream *out;
    if (result.count("output")) {
        std::string out_path = result["output"].as<std::string>();
        auto file = new std::ofstream(out_path);
        if (!file->is_open()) {
            std::cerr
                << "Error: failed to open output file \""
                << out_path << "\"." << std::endl;
        }
        out = file;
    } else {
        out = &std::cout;
    }

    // Process sequences and output found code ID's
    MILES::Decoder decoder(std::move(codes));
    for (auto sequence: sequences) {
        auto found_ids = decoder.decode(std::move(sequence));
        std::cout << util::join(found_ids.peek(), ",", "", "") << std::endl;
    }

    return EXIT_SUCCESS;
}

bool validate_pattern(const std::vector<int> &pattern) {
    if (pattern.empty() || pattern[0] != 0 || pattern.size() != 7)
        return false;
    for (int i = 1; i < pattern.size() - 1; i++) {
        if (pattern[i-1] >= pattern[i]) {
            return false;
        }
    }
    return true;
}
