#include "RNA.h"

using namespace std;

RNA::RNA(){
    this->arrayRNA = nullptr;
    this->nucleotideAmount = 0;
    this->arrayLength = 0;
}

RNA::RNA(size_t nucleotideAmount, const Nucleotide & n){
    this->nucleotideAmount = 0;
    this->arrayLength = 0;
    this->arrayRNA = nullptr;
    for(size_t i = 0; i < nucleotideAmount; i++){
        addNucleotide(n);
    }
}

RNA::RNA(const RNA & anotherRNA){
    this->nucleotideAmount = anotherRNA.nucleotideAmount;
    this->arrayLength = anotherRNA.arrayLength;
    this->arrayRNA = anotherRNA.arrayRNA;
}

RNA::RNA(const Nucleotide & anotherN ){
    this->nucleotideAmount = 0;
    this->arrayLength = 0;
    this->arrayRNA = nullptr;

    addNucleotide(anotherN);
}

RNA::~RNA() {
    this->arrayLength = 0;
    this->nucleotideAmount = 0;
};

void RNA::trim(size_t lastIndex){
    try{
        int box = this->nucleotideAmount;
        box -= 1;
        if( (int) lastIndex > box)
            throw out_of_range("Index is larger than Nucleotide Amount - 1");
    } catch (exception& ex) {
        cout << ex.what() << endl;
        throw;
    }

    size_t indexInArrayLength = lastIndex / NUCL_AMOUNT_IN_1_SIZE_T;

    size_t * box = new size_t[indexInArrayLength+1];
    memcpy(box, this->arrayRNA, (indexInArrayLength)*sizeof(size_t) );

    delete this->arrayRNA;
    this->arrayRNA = box;
    this->arrayLength = indexInArrayLength+1;
    this->nucleotideAmount = lastIndex;
}

RNA RNA::split(size_t index){
    try{
        if(index > this->nucleotideAmount - 1)
            throw out_of_range("Index is larger than Nucleotide Amount - 1");
    } catch (exception& ex) {
        cout << ex.what() << endl;
    }

    RNA rnaRight;
    rnaRight.nucleotideAmount = 0;
    rnaRight.arrayLength = 0;

    for(size_t i = index; i < this->nucleotideAmount; i++){
        Nucleotide n = (Nucleotide) (*this)[i];
        rnaRight.addNucleotide(n);
    }

    this->trim(index);

    return rnaRight;
}

string RNA::nucleotideChainToString(){
    string rnaChain = "";
    for(int i = 0; i < this->getNucleotideAmount(); i++) {
        rnaChain += this->getNucleotideInChar(i);
    }

    return rnaChain;
}

bool RNA::isComplimentary(RNA & rna2){
    return (*this) == !rna2;
}

int RNA::calculateShift(int arrayLength, int nucleotideAmount){ // Check with addNucleotide
    return (int) (NUCL_AMOUNT_IN_1_SIZE_T * (arrayLength) - nucleotideAmount) * 2 - 2;
}

void RNA::addNucleotide(Nucleotide n) {
    if (arrayRNA == nullptr) { // RNA is empty
        arrayRNA = new size_t[1];
        arrayRNA[0] = (size_t) n << calculateShift(1, 0);;

        nucleotideAmount += 1;
        arrayLength += 1;
    } else if (nucleotideAmount / arrayLength >=
               NUCL_AMOUNT_IN_1_SIZE_T) { // При доступном месте для нуклеотидов - nucleotideAmount/arrayLength < 4;
        size_t *t_box = new size_t[arrayLength + 1]; // Создаем массив по-больше
        for (size_t i = 0; i < arrayLength; i++) {
            t_box[i] = (size_t) arrayRNA[i];
        }
        t_box[arrayLength] = (size_t) 0;
        size_t shift = calculateShift(arrayLength+1, nucleotideAmount);
        t_box[arrayLength] = t_box[arrayLength] | ( (size_t) n << shift );

        delete[] arrayRNA;
        arrayRNA = t_box;

        nucleotideAmount += 1;
        arrayLength += 1;
    } else {
        arrayRNA[arrayLength-1] =
                arrayRNA[arrayLength-1] | ((size_t) n << calculateShift(arrayLength, nucleotideAmount));
        nucleotideAmount += 1;
    }
}

bool RNA::operator == (RNA const & rna2) const{
    int rnaNuclAmount = rna2.getNucleotideAmount();

    size_t * rnaArray2 = rna2.getArrayRNA();
    if( this->getNucleotideAmount() != rnaNuclAmount ){
        return 0;
    } else {
        for(int i = 0; i < this->getArrayLength(); i++){
            if( this->getArrayRNA()[i] != rnaArray2[i] ){
                return 0;
            }
        }
    }

    return 1;
}

bool RNA::operator != (RNA const & rna2) const{
    return !( *this == rna2 );
}

RNA & RNA::operator = (const RNA &rna2) {
    this->arrayLength = rna2.arrayLength;
    this->nucleotideAmount = rna2.nucleotideAmount;

    size_t * box = new size_t[this->arrayLength+1];
    memcpy(box, rna2.arrayRNA, (this->arrayLength)*sizeof(size_t) );

    delete[] this->arrayRNA;
    this->arrayRNA = box;

    return *this;
}

RNA RNA::operator ! () {
    for(int i = 0; i < this->getNucleotideAmount(); i++){
        switch ( (Nucleotide) (*this)[i] ) {
            case T: (*this)[i] = A; break;
            case A: (*this)[i] = T; break;
            case G: (*this)[i] = C; break;
            case C: (*this)[i] = G; break;
        }
    }
    return (*this);
}

RNA::nucleotideReference RNA::operator[](const size_t & index){
    try{
        if(index > this->nucleotideAmount-1)
            throw out_of_range("Index is larger than Nucleotide Amount - 1");
        return RNA::nucleotideReference(index, *this);
    } catch (exception& ex) {
        this->arrayRNA = nullptr;
        cout << ex.what() << endl;
        return RNA::nucleotideReference(0, *this);
    }
}

RNA::constNucleotideReference RNA::operator[](const size_t & index) const{
    try{
        if(index > this->nucleotideAmount-1)
            throw out_of_range("Index is larger than Nucleotide Amount - 1");
        return RNA::constNucleotideReference(index, *this);
    } catch (exception& ex) {
        cout << ex.what() << endl;
        return RNA::constNucleotideReference(0, *this);
    }
}

RNA RNA::operator + (RNA const & rna2) {
    size_t rnaLength2 = rna2.getNucleotideAmount();

    for(int i = 0; i < rnaLength2; i++){
        RNA::addNucleotide( (Nucleotide) rna2[i]);
    }

    return *this;
}

ostream& operator << (ostream& os, RNA& rna)
{
    for(int i = 0; i < rna.getNucleotideAmount(); i++){
        printf( "%c", rna.getNucleotideInChar(i) );
        if( (i % 32) == 31 && i!=0){
            printf( "%c", '!' );
        }
    }
}

size_t RNA::getNucleotideInNumber(const size_t &index) const {
    size_t currentArrayPos = index / (NUCL_AMOUNT_IN_1_SIZE_T);
    size_t currentNuclInSizeTPos = index % (NUCL_AMOUNT_IN_1_SIZE_T*2) ;

    size_t fixPos = 2*NUCL_AMOUNT_IN_1_SIZE_T - 1 - (currentNuclInSizeTPos*2);

    size_t box = this->arrayRNA[currentArrayPos];
    box >>= (fixPos-1);
    box &= (size_t) 3;

    return box;
}

char RNA::getNucleotideInChar(const size_t & index) const {
    switch (getNucleotideInNumber(index)) {
        case 0: return 'A';
        case 1: return 'T';
        case 2: return 'C';
        case 3: return 'G';
    }
    return 0;
}

Nucleotide RNA::getNucleotideInNucleotide(const size_t &index) const {
    switch(getNucleotideInNumber(index)){
        case 0: return A;
        case 1: return T;
        case 2: return C;
        case 3: return G;
    }
    return A;
}