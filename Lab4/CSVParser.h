#ifndef LAB4_CSVPARSER_H
#define LAB4_CSVPARSER_H

#include <tuple>
#include <vector>
#include <istream>
#include <sstream>
#include <iostream>

#include "FieldState.h"

using namespace std;

template <class... Args>
class CSVParser {
private:

    const char _rowDelimeter;   // Row Delimeter
    const char _colDelimeter;   // Column Delimeter
    const char _shieldingSymbol;
    const char _quoteSymbol = '"';
    size_t _skip; // skip first lines count

    string _buffer; // Hold string

    tuple<Args...> _currentTuple;

    istream &_istr;

    bool qqFlag = 0;

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
            return _parser._currentTuple;
        }
        bool operator!=(const CSVIterator& box){
            return !_parser._istr.fail(); // Feature, it is equal to EOF
        }
        CSVIterator& operator ++(){
            try{
                auto vector = _parser.readString();
                _parser.clearTuple(make_index_sequence<sizeof...(Args)>());
                _parser.makeTuple(vector, make_index_sequence<sizeof...(Args)>());
                ++_counterStrings;
            } catch (exception& ex){
                cout << "There is a problem with string " << to_string(_counterStrings) << ex.what() << endl;
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
    void makeTuple(vector<string>& vectorStr,  index_sequence<J...>){
        initializer_list<bool>{ readTupleElement(vectorStr[J], get<J>(_currentTuple))... };
    }

    template <size_t... J>
    void clearTuple(index_sequence<J...>){
        vector<string> vectorClear = {"0", "0", "0", ""};
        initializer_list<bool>{ readTupleElement(vectorClear[J], get<J>(_currentTuple))...};
    }

    template <typename T>
    bool readTupleElement(string& str, T& value) {
        castString(str, value);
        return true;
    }

    bool checkCorrectionForSymbol(const size_t& idx, const char& symbol, FieldState currentState){
        switch (idx) {
            case 0: {
                if (symbol <= '9' && symbol >= '0') {
                    return true;
                } else {
                    return false;
                }
            }
            case 1: {
                if ((symbol <= '9' && symbol >= '0') || symbol == '.') {
                    return true;
                } else {
                    return false;
                }
            }
            case 2: {
                if ((symbol <= '9' && symbol >= '0') || symbol == '.') {
                    return true;
                } else {
                    return false;
                }
            }
            case 3: {
                if (currentState == FieldState::ShieldedField) {
                    if ( symbol == _colDelimeter || symbol == _rowDelimeter || symbol == _quoteSymbol || symbol == _shieldingSymbol ) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }

    vector<string> readString() {
        size_t idx = 0;
        vector<string> fields = {""};
        FieldState currentState = FieldState::NoQoutedField;
        getline(_istr, _buffer);

        for (size_t i = 0; i < _buffer.size(); ++i) {
            switch (currentState) {
                case FieldState::QuotedField:
                    if (_buffer[i] == _shieldingSymbol) {
                        currentState = FieldState::ShieldedField;
                    } else if(_buffer[i] == _quoteSymbol){
                        currentState = FieldState::NoQoutedField;
                    }else {
                        fields[idx].push_back(_buffer[i]);
                    }
                    break;
                case FieldState::NoQoutedField:
                    if (_buffer[i] == _colDelimeter) {
                        fields.emplace_back("");
                        ++idx;
                    } else if (_buffer[i] == _quoteSymbol) {
                        currentState = FieldState::QuotedField;
                    } else {
                        if( checkCorrectionForSymbol(idx, _buffer[i], currentState) ){
                            fields[idx].push_back(_buffer[i]);
                        } else {
                            throw invalid_argument(", exactly in column " + to_string(idx+1) + "." );
                        }
                    }
                    break;
                case FieldState::ShieldedField:
                    char symb = _buffer[i];
                    if (checkCorrectionForSymbol(idx,_buffer[i], currentState) && qqFlag == 0) {
                        if(_buffer[i] == _colDelimeter){
                            fields[idx].push_back(_colDelimeter);
                        } if(_buffer[i] == _rowDelimeter){
                            fields[idx].push_back(_rowDelimeter);
                        } if(_buffer[i] == _quoteSymbol){
                            fields[idx].push_back(_quoteSymbol);
                        } if(_buffer[i] == _shieldingSymbol){
                            fields[idx].push_back(_shieldingSymbol);
                        }
                        qqFlag = 1;
                    } else if (_buffer[i] == _shieldingSymbol) {
                        currentState = FieldState::QuotedField;
                        qqFlag = 0;
                    } else {
                        throw invalid_argument(", exactly in column " + to_string(idx+1) + "." );
                    }
                    break;
            }

            if (i == _buffer.size() - 1) { // Дошли до конца строки буффера
                if (currentState == FieldState::QuotedField) {
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
            throw invalid_argument("\nFile is empty!");
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
