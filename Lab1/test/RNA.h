#ifndef TASK1_RNA_H
#define TASK1_RNA_H

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cstring>
#include <exception>

#define NUCL_AMOUNT_IN_1_SIZE_T ( (sizeof(size_t)*8)/2 ) // кол-во нуклеотидов в одном size_t (байты в биты, потом делим на 2) (sizeof(size_t) = 8 байт)

using namespace std;

enum Nucleotide{
    A, T, C, G
};

class RNA{
private:
    size_t * arrayRNA = nullptr;
    int nucleotideAmount = 0;
    size_t arrayLength = 0;

public:
    RNA();                                                  /// Correct
    RNA(const RNA & anotherRNA);                            /// Correct
    RNA(const Nucleotide & anotherN);                       /// Correct
    RNA(size_t nucleotideAmount, const Nucleotide & n);     /// Correct
    virtual ~RNA();                                         /// Correct

    void trim(size_t lastIndex);                            /// Correct, without memory leaks
    RNA split(size_t index);                                /// Correct, without memory leaks
    size_t length(){
        return this->getNucleotideAmount();
    }                                    /// Correct
    string nucleotideChainToString();                       /// Correct
    bool isComplimentary(RNA & rna2);

    char getNucleotideInChar(const size_t & index) const;                   /// Correct
    size_t getNucleotideInNumber(const size_t & index) const;               /// Correct
    Nucleotide getNucleotideInNucleotide(const size_t & index) const;       /// Correct

    class constNucleotideReference{
    private:
        const size_t & index;
        const RNA & rna;

    public:
        constNucleotideReference(const size_t & number, const RNA & r1) : index(number), rna(r1) {};
        const size_t & getIndex() const { return this->index; };
        const RNA & getRNA() const { return this->rna; };
        operator Nucleotide() const {
            try{
                if(this->rna.nucleotideAmount <= this->index || this->rna.arrayRNA == nullptr)
                    throw out_of_range("Index is larger than Nucleotide Amount - 1");
                return this->rna.getNucleotideInNucleotide(this->index);
            } catch (exception& ex) {
                cout << ex.what() << endl;
            }
        }
    };

    class nucleotideReference{
    private:
        const size_t & index;
        RNA & rna;
    public:
        nucleotideReference(const size_t & number, RNA & r1) : index(number), rna(r1) {};       /// Correct
        nucleotideReference & operator = (Nucleotide nucleotide){
            if(this->rna.nucleotideAmount < this->index + 1){
                for(int i = this->rna.nucleotideAmount; i < this->index + 1; i++){
                    this->rna.addNucleotide(A); // Allocate memory
                }
            }
            size_t currentArrayPos = this->index / (NUCL_AMOUNT_IN_1_SIZE_T); // size_t position, which contain nucleotide, in size_t array
            size_t currentNuclInSizeTPos = this->index % (NUCL_AMOUNT_IN_1_SIZE_T); // Nucleotide position in size_t

            size_t fixPos = NUCL_AMOUNT_IN_1_SIZE_T*2 - 1 - (currentNuclInSizeTPos*2); // Left bite pos of nucl window
            size_t mask1 = SIZE_MAX - (size_t) ( pow(2.0, (float) fixPos) + pow(2.0, (float) fixPos-1) );
            size_t mask2 = ( (size_t) nucleotide ) << fixPos-1;

            this->rna.arrayRNA[currentArrayPos] = this->rna.arrayRNA[currentArrayPos] & mask1 | mask2;

            return *this;
        }                           /// Correct
        nucleotideReference & operator = (nucleotideReference & anotherNuclRef){
            RNA rna = this->rna;
            RNA rnaSource = anotherNuclRef.rna;

            size_t currentArrayPos = this->index / (NUCL_AMOUNT_IN_1_SIZE_T);
            size_t currentNuclInSizeTPos = this->index % (NUCL_AMOUNT_IN_1_SIZE_T);

            size_t currentArrayPosSource = anotherNuclRef.index / (NUCL_AMOUNT_IN_1_SIZE_T);
            size_t currentNuclInSizeTPosSource = anotherNuclRef.index % (NUCL_AMOUNT_IN_1_SIZE_T);

            size_t fixPos = (NUCL_AMOUNT_IN_1_SIZE_T*2) - 1 - (currentNuclInSizeTPos*2); // Left bite pos of nucl window
            size_t fixPosSource = (NUCL_AMOUNT_IN_1_SIZE_T*2) - 1 - (currentNuclInSizeTPosSource*2); // Left bite pos of nucl window

            size_t mask1 = SIZE_MAX - (size_t) ( pow(2, fixPos) + pow(2, fixPos-1) );
            size_t mask2 = rnaSource.arrayRNA[currentArrayPosSource] >> (fixPosSource-1);
            mask2 &= (size_t) 3;
            mask2 <<= fixPos-1;

            rna.arrayRNA[currentArrayPos] = rna.arrayRNA[currentArrayPos] & mask1 | mask2;

            return *this;
        }
        nucleotideReference & operator = (const constNucleotideReference & anotherNuclRef){
            Nucleotide box_n = anotherNuclRef.getRNA().getNucleotideInNucleotide(anotherNuclRef.getIndex());

            size_t currentArrayPos = this->index / (NUCL_AMOUNT_IN_1_SIZE_T); // size_t position, which contain nucleotide, in size_t array
            size_t currentNuclInSizeTPos = this->index % (NUCL_AMOUNT_IN_1_SIZE_T); // Nucleotide position in size_t

            size_t fixPos = NUCL_AMOUNT_IN_1_SIZE_T*2 - 1 - (currentNuclInSizeTPos*2); // Left bite pos of nucl window
            size_t mask1 = SIZE_MAX - (size_t) ( pow(2.0, (float) fixPos) + pow(2.0, (float) fixPos-1) );
            size_t mask2 = ( (size_t) box_n ) << fixPos-1;

            this->rna.arrayRNA[currentArrayPos] = this->rna.arrayRNA[currentArrayPos] & mask1 | mask2;

            return *this;
        }
        operator Nucleotide() {
            try{
                if(this->rna.nucleotideAmount <= this->index || this->rna.arrayRNA == nullptr)
                    throw out_of_range("Index is larger than Nucleotide Amount - 1");
                return this->rna.getNucleotideInNucleotide(this->index);
            } catch (exception& ex) {
                cout << ex.what() << endl;
                exit(0);
            }
        }                                                             /// Correct
    };

    int calculateShift(int arrayLength, int nucleotideAmount); /// Subfunction for addNucleotide     /// Correct
    void addNucleotide(Nucleotide n); /// Adding nucleotide into RNA                                 /// Correct

    size_t getArrayLength() const { return this->arrayLength; };            /// Correct
    size_t getNucleotideAmount() const { return this->nucleotideAmount; };  /// Correct
    size_t * getArrayRNA() const { return this->arrayRNA; };                /// Correct

    RNA operator ! ();                                                                           /// Correct
    RNA operator + (RNA const & rna2);                                                           /// Correct
    bool operator == (RNA const & rna2) const; /// Test passed                                   /// Correct
    bool operator != (RNA const & rna2) const; /// Also correct, as == correct (test passed)     /// Correct
    RNA & operator = (const RNA & rna2);                                                         /// Correct
    friend ostream & operator << (ostream& os, RNA& rna);                                        /// Correct

    nucleotideReference operator[](const size_t & index);                   /// Correct
    constNucleotideReference operator[](const size_t & index) const;        /// Correct
};

#endif //TASK1_RNA_H
