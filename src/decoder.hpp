#pragma once

#include <vector>
#include <iostream>

#include "queue.hpp"
#include "util.hpp"

namespace MILES {
    //! MILES code abstraction
    // Encapsulates a MILES code. Keeps track of how much of the pattern has been previously matched
    // by maintaining an iterator over the pattern. Using advance attempts to advance this iterator,
    // and using reset resets the state of the object (to the beginning of the pattern).
    class Code {
    private:
        //! Code ID
        int m_id;

        //! Code pattern
        std::vector<int> m_pattern;

        //! Position in code pattern
        std::vector<int>::iterator m_pattern_ittr;

        //! Starting value
        int m_start;

        //! Bad flag
        bool m_bad;

    public:
        //! Default constructor (deleted)
        Code() = delete;

        //! Copy constructor
        explicit Code(const Code &other);

        //! Parameterized constructor
        explicit Code(std::vector<int> &&pattern, int id);

        //! ID
        int id() const
        { return m_id; }

        //! Pattern
        const std::vector<int>& pattern() const
        { return m_pattern; }

        //! Size
        size_t size() const
        { return m_pattern.size(); }

        //! Is bad
        // The bad flag is set when advance encounters a value who's difference from the starting
        // value is greater than the pattern value that's currently being looked for.
        bool bad() const
        { return m_bad; }

        //! Reset
        // Unset the bad flag and set the new starting value.
        // @param pulse New starting value
        void reset(int pulse);

        //! Advance
        // Advances the code object by a pulse value. If the difference between the value and the
        // starting value is less than the pattern value that is currently being looked for, nothing
        // happens. If equal, advances the pattern value being looked for to the next value in the
        // pattern. If greater, sets the bad flag.
        // @ return True if the last pattern value has been found, false if otherwise or if the bad
        // flag is set.
        bool advance(int pulse);

        //! String conversion operator
        explicit operator std::string() const;
    };

    //! MILES code sequence decoder abstraction
    // Encapsulates a list of MILES code pattern objects which are used in decoding MILES sequences.
    class Decoder {
    private:
        //! List of codes
        std::vector<Code> m_codes;

        //! Reset all codes
        void reset_all(int pulse);

        //! If all codes are bad
        bool all_bad() const;

    public:
        //! Default constructor (deleted)
        Decoder() = delete;

        //! Parameterized constructor (universal)
        explicit Decoder(std::vector<Code> &&codes);

        //! Decode sequence
        // Decodes the given sequence by checking for the codes the decoder was constructed with.
        // In decoding, the code objects are advanced per value of the sequence until either a
        // pattern is found and the found ID is added to a queue of found IDs (and the values of the
        // sequence in which the code pattern was found are removed), or until all codes become bad
        // in which case the process begins again by resetting all codes to the new first value of
        // the sequences after having dequeued a single value from the sequence (the old first
        // value). Once the sequence queue is empty (or the size is less than the length of all of
        // the code patterns), returns the queue of found IDs.
        // @param seq Input sequence
        // @return Queue of found code IDs
        queue<int> decode(queue<int> &&seq);
    };
}

//! ostream insertion overload
inline std::ostream& operator<<(std::ostream& os, const MILES::Code &code) {
    return os << std::string(code);
}
