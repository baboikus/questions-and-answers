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
#include <vector>
#include <filesystem>

using namespace std;
// class for case insensitive ALMOST immutable string
class Word {
public:
    Word(){
    }

    // we don't want any spontaneous type conversion or additional memmory copy
    explicit Word(string &&s) :
        data_(s) {
        refresh();
    }

    const Word& operator = (string &&s) {
        data_ = s;
        refresh();
        return *this;
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

     static bool isLatinSymbol(const char c) {
         return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
     }

private:
    // let's forbid this constructor for now
    explicit Word(const string &s) = delete;

    void refresh() {
        for(size_t i = 0; i < data_.size(); ++i) {
            // it's always ASCII symbols, so tolower usage is safe
            data_[i] = tolower(data_[i]);
        }
        //we don't need to evaluate hash more than one time for immutable data
        hash_ = std::hash<string>{}(data_);
    }

    string data_;
    size_t hash_ = 0;
};

// we need to specify hash function for class Word
namespace std {
    template<> struct hash<Word> {
        size_t operator()(const Word &w) const {
            return w.hash();
        }
    };
}

// let's incapsulate skips of all non latin chars here
istream& operator>>(istream& is, Word& w){
    istream::sentry s(is);
    // we need at least one memmory allocation for each word here anyway
    // trimming next word from non latin characters from both sides
    if (s && is.good()) {
        char c;
        // trim before word start
        while(is.good()) {
            c = is.get();
            if(Word::isLatinSymbol(c)) {
                break;
            }
        }
        // this part is ugly, but it could reduce number of memmory allocations
        // while we filling string buffer
        const auto from = is.tellg();
        while(is.good()) {
            // we could optimize buffer filling later
            c = is.get();
            if(!Word::isLatinSymbol(c)) {
                break;
            }
        }
        string buffer;
        buffer.resize(is.tellg() - from);
        is.seekg(from);
        is.unget();
        for(size_t i = 0; i < buffer.size() && is.good(); ++i) {
            buffer[i] = is.get();
        }

        // trim after word end
        while(is.good()) {
            if(!Word::isLatinSymbol(is.peek())) {
                is.ignore();
            }
            else {
                break;
            }
        }

        // finally passing buffered data to word
        w = move(buffer);
    }
    return is;
}

constexpr int NOT_ENOUGHT_ARGS_ERROR_CODE = 1;
constexpr int TOO_MANY_ARGS_ERROR_CODE = 2;
constexpr int INPUT_FILE_NOT_EXISTS_ERROR_CODE = 3;
constexpr int OUTPUT_FILE_ALREADY_EXISTS_ERROR_CODE = 4;
constexpr int CAN_OPEN_FILE_ERROR_CODE = 5;

int main(int argc, char *argv[]) {
    // first we check argc value and existed/non existed files
    if(argc < 3) {
        cout << "not enough args" << endl;
        return NOT_ENOUGHT_ARGS_ERROR_CODE;
    }
    if(argc > 3) {
        cout << "too many args" << endl;
        return TOO_MANY_ARGS_ERROR_CODE;
    }
    if(!filesystem::exists(filesystem::path(argv[1]))) {
        cout << argv[1] << " non exists" << endl;
        return INPUT_FILE_NOT_EXISTS_ERROR_CODE;
    }
    if(filesystem::exists(filesystem::path(argv[2]))) {
        cout << argv[2] << " already exists" << endl;
        return OUTPUT_FILE_ALREADY_EXISTS_ERROR_CODE;
    }


    // we are using hash-table counter like (word->word_frequency)
    unordered_map<Word, uint> counter;

    // argv[1] should be input file
    ifstream inputFile(argv[1]);
    if(inputFile.fail() || inputFile.bad()) {
        cout << "can't open " << argv[1] << endl;
        return CAN_OPEN_FILE_ERROR_CODE;
    }
    while(inputFile.good()) {
        Word w;
        inputFile >> w;
        counter[w]++;
    }

    // for sorting we need some additional memmory. lets try to use as small as possible
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
    if(freqFile.fail() || freqFile.bad()) {
        cout << "can't open " << argv[2] << endl;
        return CAN_OPEN_FILE_ERROR_CODE;
    }
    for(auto i : buckets) {
        // i should be a pair like (word, word_frequency)
        freqFile << i->second << " " << i->first.data().c_str() << endl;
    }

    // always close your files!
    freqFile.close();

    return 0;
}
