#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <queue>
#include <cctype>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

class Word {
public:
    string text;
    array<unsigned short, 26> dict = {};
    float inv_norm = 0.0f;

    Word(const string& str) {
        text = str;

        for (unsigned char ch : str) {
            char c = static_cast<char>(toupper(ch));

            if (c >= 'A' && c <= 'Z') {
                dict[c - 'A'] += 1;
            }
        }

        float norm2 = 0.0f;

        for (int i = 0; i < 26; i++) {
            norm2 += dict[i] * dict[i];
        }

        if (norm2 > 0) {
            inv_norm = 1.0f / sqrt(norm2);
        }
    }

    float cosine(const Word& other) const {
        int dot = 0;

        for (int i = 0; i < 26; i++) {
            dot += dict[i] * other.dict[i];
        }

        return dot * inv_norm * other.inv_norm;
    }
};

struct WordPair {
    string first;
    string second;
    float similarity;
};

struct ComparePair {
    bool operator()(const WordPair& a, const WordPair& b) const {
        return a.similarity > b.similarity;
    }
};

using TopQueue = priority_queue<WordPair, vector<WordPair>, ComparePair>;

void add_candidate(
    TopQueue& topPairs,
    const string& word1,
    const string& word2,
    float similarity,
    size_t topN
    ) {
    if (word1 == word2) {
        return;
    }

    WordPair pair{word1, word2, similarity};

    if (topPairs.size() < topN) {
        topPairs.push(pair);
    } else if (similarity > topPairs.top().similarity) {
        topPairs.pop();
        topPairs.push(pair);
    }
}

void print_top(TopQueue topPairs, unsigned long long checkedPairs) {
    vector<WordPair> result;

    while (!topPairs.empty()) {
        result.push_back(topPairs.top());
        topPairs.pop();
    }

    sort(result.begin(), result.end(), [](const WordPair& a, const WordPair& b) {
        return a.similarity > b.similarity;
    });

    cout << "\n==============================\n";
    cout << "Checked pairs: " << checkedPairs << "\n";
    cout << "Current top pairs:\n\n";

    cout << fixed << setprecision(6);

    for (const WordPair& p : result) {
        cout << p.first << " - "
             << p.second << " : "
             << p.similarity << "\n";
    }

    cout << "==============================\n";
}

int main()
{
    ifstream file("words_alpha.txt");

    if (!file.is_open()) {
        cerr << "Cannot open file\n";
        return 1;
    }

    vector<Word> words;
    words.reserve(400000);

    string token;

    while (file >> token) {
        if (token.size() > 6) {
            words.push_back(Word(token));
        }
    }

    cout << "Loaded words longer than 6 letters: " << words.size() << "\n";

    const size_t TOP_N = 20;
    TopQueue topPairs;

    using Clock = chrono::steady_clock;
    auto lastPrint = Clock::now();

    unsigned long long checkedPairs = 0;
    unsigned long long sinceTimerCheck = 0;

    for (size_t i = 0; i < words.size(); i++) {
        for (size_t j = i + 1; j < words.size(); j++) {
            float sim = words[i].cosine(words[j]);

            add_candidate(
                topPairs,
                words[i].text,
                words[j].text,
                sim,
                TOP_N
                );

            checkedPairs++;
            sinceTimerCheck++;

            if (sinceTimerCheck >= 1000000) {
                sinceTimerCheck = 0;

                auto now = Clock::now();

                if (chrono::duration_cast<chrono::seconds>(now - lastPrint).count() >= 2) {
                    print_top(topPairs, checkedPairs);
                    lastPrint = now;
                }
            }
        }
    }

    cout << "\nFINAL RESULT\n";
    print_top(topPairs, checkedPairs);

    return 0;
}
