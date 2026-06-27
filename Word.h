#ifndef WORD_H
#define WORD_H


#include <array>
#include <string>

class Word {
public:
    Word(const std::string& str);

    float cosine(const Word& other) const;

    const std::string& getText() const;

private:
    std::string text;
    std::array<unsigned short, 26> dict = {};
    float inv_norm = 0.0f;
};


#endif // WORD_H
