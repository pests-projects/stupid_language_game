#include "Word.h"

#include <cmath>
#include <cctype>

Word::Word(const std::string& str) {
    text = str;

    for (unsigned char ch : str) {
        char c = static_cast<char>(std::toupper(ch));

        if (c >= 'A' && c <= 'Z') {
            dict[c - 'A'] += 1;
        }
    }

    float norm2 = 0.0f;

    for (int i = 0; i < 26; i++) {
        norm2 += dict[i] * dict[i];
    }

    if (norm2 > 0) {
        inv_norm = 1.0f / std::sqrt(norm2);
    }
}

float Word::cosine(const Word& other) const {
    int dot = 0;

    for (int i = 0; i < 26; i++) {
        dot += dict[i] * other.dict[i];
    }

    return dot * inv_norm * other.inv_norm;
}

const std::string& Word::getText() const {
    return text;
}
