#ifndef LAB4_CSVPARSER_H
#define LAB4_CSVPARSER_H

#include <tuple>
#include <vector>
#include <istream>
#include <sstream>
#include <iostream>

#include "QuotesStatement.h"

using namespace std;

template <class... Args>
class CSVParser {
private:

    const char _rowDelimeter;   // Row Delimeter
    const char _colDelimeter;   // Column Delimeter
    const char _shieldingSymbol;
    const char _quoteSymb = '"';
    size_t _skip; // skip first lines count

    string _buffer; // Hold string
    tuple<Args...> _currerntTuple;

    istream &_istr;

    class CSVIterator{
    private:
        CSVParser<Args...>& _parser;
        size_t _counterStrings = 1;
    public:
        explicit CSVIterator(CSVParser<Args...>& parser, bool end = false) : _parser(parser){
            if (!end)
                for(auto i = 0; i < parser._skip + 1; ++(*this), ++i);
        }

        tuple<Args...>& operator*(){
            return _parser._currerntTuple;
        }

        bool operator!=(const CSVIterator& box){
            return !_parser._istr.fail(); // Feature, it is equal to EOF
        }

        CSVIterator& operator ++(){
            try{
                auto vector = _parser.readString();
                _parser.makeTuple(vector, make_index_sequence<sizeof...(Args)>());
                ++_counterStrings;
            } catch (exception&){
                cout << "There is a problem with string " << to_string(_counterStrings) << endl;
                exit(1);
            }
            return *this;
        }
    };

    template<typename T, typename U>
    void castString(T& a, U& b){
        istringstream ss(a);
        ss >> b;
    }

    template<typename T>
    void castString(T& a, T& b){
        b = a;
    }

    template <size_t... J>
    tuple<Args...> makeTuple(vector<string>& strings,  index_sequence<J...>){
        initializer_list<bool>{ readTupleElement(strings[J], get<J>(_currerntTuple))... };
        return _currerntTuple;
    }

    template <typename T>
    bool readTupleElement(string& str, T& value) {
        castString(str, value);
        return true;
    }

    vector<string> readString() {
        size_t idx = 0;
        vector<string> fields = {""};
        QuotesStatement currentState = QuotesStatement::NoQField;
        getline(_istr, _buffer);

        for (size_t i = 0; i < _buffer.size(); ++i) {
            switch (currentState) {
                case QuotesStatement::QField:
                    if (_buffer[i] == _quoteSymb) {
                        currentState = QuotesStatement::QQField;
                    } else {
                        fields[idx].push_back(_buffer[i]);
                    }
                    break;
                case QuotesStatement::NoQField:
                    if (_buffer[i] == _colDelimeter) {
                        fields.emplace_back("");
                        ++idx;
                    } else if (_buffer[i] == _quoteSymb) {
                        currentState = QuotesStatement::QField;
                    } else {
                        fields[idx].push_back(_buffer[i]);
                    }
                    break;
                case QuotesStatement::QQField:
                    if (_buffer[i] == _colDelimeter) {
                        fields.template emplace_back("");
                        ++idx;
                        currentState = QuotesStatement::NoQField;
                    } else if (_buffer[i] == _quoteSymb) {
                        fields[idx].push_back(_buffer[i]);
                        currentState = QuotesStatement::QField;
                    } else {
                        currentState = QuotesStatement::NoQField;
                    }
                    break;
            }

            if (i == _buffer.size() - 1) { // Дошли до конца строки буффера
                if (currentState == QuotesStatement::QField) {
                    fields[idx].push_back(_rowDelimeter);
                    getline(_istr, _buffer);
                    i = -1;
                } else if (sizeof...(Args) == fields.size()){
                    return fields;
                } else {
                    throw invalid_argument("Wrong syntax");
                }
            }
        }

        if (fields.size() == 1){
            throw invalid_argument("File is empty!");
        }

        return fields;
    }

public:

    CSVParser(istream& file, size_t skip, const char columnDelimeter, const char rowDelimeter, const char shieldingSymbol) :
    _istr(file), _skip(skip), _colDelimeter(columnDelimeter), _rowDelimeter(rowDelimeter), _shieldingSymbol(shieldingSymbol) {}

    CSVIterator end(){
        return CSVIterator(*this,true);
    }
    CSVIterator begin(){
        return CSVIterator(*this);
    }

};

#endif //LAB4_CSVPARSER_H
