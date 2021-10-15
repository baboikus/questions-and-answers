/*
** Словом считается набор латинских букв, a-zA-Z. Любой (любой) другой символ считается пробелом.
** Регистр нужно привести.
** Сортировать нужно сначала по частоте, потом по алфавиту.
** Выводить тоже сначала частоту, потом слово.
*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <locale>

using namespace std;

// class for case insensitive immutable string
class Word {
public:
    // we don't want any spontaneous type conversion or additional memmory copy
    explicit Word(string &&s) :
        data_(s)
    {
        for(size_t i = 0; i < data_.size(); ++i) {
            // it's always ASCII symbols, so tolower usage is safe
            data_[i] = tolower(data_[i]);
        }
        // we don't need to evaluate hash more than one time for immutable data
        hash_ = std::hash<string>{}(data_);
    }

    const string& data() const {
        return data_;
    }

    size_t hash() const {
        return hash_;
    }

    bool operator == (const Word &w) const {
        // speed up the comparison!
        if(hash_ != w.hash_) return false;
        return data_ == w.data_;
    }

    bool operator > (const Word &w) const {
        return data_.compare(w.data_) < 0;
    }

private:
    // let's forbid this constructor for now
    Word(const string &s) = delete;

    string data_;
    size_t hash_;
};

// we need to specify hash function for class Word
namespace std {
    template<> struct hash<Word> {
        size_t operator()(const Word &w) const {
            return w.hash();
        }
    };
}

/// TODO args count checks
/// TODO ignoring non alphabet symbols
/// TODO need additional test examples
/// TODO run valgrind
int main(int argc, char *argv[]) {
    // we are using hash-table counter like (word->word_frequency)
    unordered_map<Word, uint> counter;

    // argv[1] should be input file
    ifstream inputFile(argv[1]);
    while(inputFile.good()) {
        string s;
        inputFile >> s;
        counter[Word(move(s))]++;
    }

    vector<decltype(counter.cbegin())> buckets;
    buckets.reserve(counter.size());
    for(auto i = counter.cbegin(); i != counter.cend(); ++i) {
       buckets.push_back(move(i));
    }

    // sorting time. we are using custom reverse-sort function here
    sort(buckets.begin(),
         buckets.end(),
         [](const auto &l, const auto &r) {
             if(l->second > r->second) {
                 return true;
             }
             else {
                 return l->first > r->first;
             }
         });

    // argv[2] should be output file
    ofstream freqFile(argv[2]);
    for(auto i : buckets) {
        // i should be a pair like (word, word_frequency)
        freqFile << i->second << " " << i->first.data() << endl;
    }

    // always close your files!
    freqFile.close();

    return 0;
}
