#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>

#include "util.hpp"
#include "queue.hpp"
#include "decoder.hpp"

namespace MILES {
    // ---------------------------------------------------------------------------------------------
    // Class: Code
    // ---------------------------------------------------------------------------------------------
    Code::Code(const Code &other) = default;

    Code::Code(std::vector<int> &&pattern, int id):
        m_id { id },
        m_pattern { std::move(pattern) },
        m_pattern_ittr { m_pattern.begin() },
        m_start { 0 },
        m_bad { false }
    {}

    void Code::reset(int pulse) {
        m_pattern_ittr = m_pattern.begin();
        m_start = pulse;
        m_bad = false;
    }

    bool Code::advance(int pulse) {
        if (m_bad)
            return false;
        int delta = pulse - m_start;
        if (delta == *m_pattern_ittr) {
            m_pattern_ittr += 1;
        } else if (delta > *m_pattern_ittr) {
            m_bad = true;
        }
        return m_pattern_ittr == m_pattern.end();
    }

    Code::operator std::string() const {
        std::ostringstream oss;
        oss << "id:" << m_id
            << " pattern:" << util::join(m_pattern)
            << " curr:" << *m_pattern_ittr
            << " start:" << m_start
            << " bad:" << std::boolalpha << m_bad;
        return oss.str();
    }

    // ---------------------------------------------------------------------------------------------
    // Class: Decoder
    // ---------------------------------------------------------------------------------------------
    Decoder::Decoder(std::vector<Code> &&codes):
        m_codes { std::move(codes) }
    {}

    void Decoder::reset_all(int pulse) {
        for (auto &code: m_codes)
            code.reset(pulse);
    }

    bool Decoder::all_bad() const {
        return std::all_of(m_codes.begin(), m_codes.end(),
                [](const Code &code) { return code.bad(); });
    }

    queue<int> Decoder::decode(queue<int> &&seq) {
        queue<int> found_codes;
        while (!seq.empty()) {
            // break if sequence smaller than all codes
            if (std::all_of(m_codes.cbegin(), m_codes.cend(),
                    [size = seq.size()](const Code &code) { return size < code.size(); }))
                break;
            // peek entire queue, reset codes to first value, and look for a code
            auto peeked = seq.peek();
            auto ittr = peeked.begin();
            reset_all(*ittr);
            for (size_t i = 0; ittr != peeked.end(); ++ittr, ++i) {
                auto found = std::find_if(m_codes.begin(), m_codes.end(),
                        [pulse = *ittr](Code &code) { return code.advance(pulse); });
                if (all_bad()) break;
                if (found != m_codes.end()) {
                    found_codes.enqueue(found->id());
                    seq.erase(i);
                    break;
                }
            }
            seq.erase(1);
        }
        return found_codes;
    }
}
