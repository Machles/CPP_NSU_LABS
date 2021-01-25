#include <gtest/gtest.h>
#include "RNA.h"
#include "RNA.cpp"

class RNAFixture : public :: testing:: Test {

public:
    virtual ~RNAFixture() {
    };
protected:
    virtual void TearDown(){
        delete rna1;
        delete rna2;
        delete rna3;
    }

    virtual void SetUp(){
        rna1 = new RNA();
        rna2 = new RNA();
        rna3 = new RNA();
    }

    RNA * rna1;
    RNA * rna2;
    RNA * rna3;

};

TEST_F(RNAFixture, isEqual) {
    for(int i = 0; i < 1000; i++){
        rna1->addNucleotide(T);
    }
    for(int i = 0; i < 1000; i++){
        rna1->addNucleotide(G);
    }
    for(int i = 0; i < 1000; i++){
        rna2->addNucleotide(T);
    }
    for(int i = 0; i < 1000; i++){
        rna2->addNucleotide(G);
    }

    EXPECT_EQ(true, *rna1 == *rna2);

    rna1->addNucleotide(A);
    rna2->addNucleotide(G);

    EXPECT_EQ(false, *rna1 == *rna2);
}

TEST_F(RNAFixture, correctGettingNucleotideInChar) {
    for(int i = 0; i < 1000; i++){
        rna1->addNucleotide(T);
    }
    for(int i = 0; i < 1000; i++){
        rna1->addNucleotide(G);
    }
    EXPECT_EQ('T',rna1->getNucleotideInChar(0));
    EXPECT_EQ('T',rna1->getNucleotideInChar(999));
    EXPECT_EQ('G',rna1->getNucleotideInChar(1000));
    EXPECT_EQ('G',rna1->getNucleotideInChar(1999));
}

TEST_F(RNAFixture, isCorrectOperatorCompliment ) {
    for(int i = 0; i < 64; i++){rna2->addNucleotide(T);}
    for(int i = 0; i < 64; i++){rna2->addNucleotide(G);}
    for(int i = 0; i < 64; i++){rna2->addNucleotide(C);}
    for(int i = 0; i < 64; i++){rna2->addNucleotide(A);}

    for(int i = 0; i < 64; i++){rna1->addNucleotide(A);}
    for(int i = 0; i < 64; i++){rna1->addNucleotide(C);}
    for(int i = 0; i < 64; i++){rna1->addNucleotide(G);}
    for(int i = 0; i < 64; i++){rna1->addNucleotide(T);}

    EXPECT_EQ(1,*rna1 == !(*rna2) );
}

TEST_F(RNAFixture, isCorrectOperatorPlus) {
    for(int i = 0; i < 100; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 20; i++){rna1->addNucleotide(G);}

    for(int i = 0; i < 23; i++){rna2->addNucleotide(C);}
    for(int i = 0; i < 32; i++){rna2->addNucleotide(A);}

    for(int i = 0; i < 100; i++){rna3->addNucleotide(T);}
    for(int i = 0; i < 20; i++){rna3->addNucleotide(G);}
    for(int i = 0; i < 23; i++){rna3->addNucleotide(C);}
    for(int i = 0; i < 32; i++){rna3->addNucleotide(A);}

    EXPECT_EQ(1,*rna3 == *rna1 + *rna2 );
}


TEST_F(RNAFixture, isCorrectOperatorPreferences1) {
    for(int i = 0; i < 100; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 100; i++){rna1->addNucleotide(G);}
    for(int i = 0; i < 100; i++){rna1->addNucleotide(C);}
    for(int i = 0; i < 100; i++){rna1->addNucleotide(A);}

    (*rna1)[0] = A;
    (*rna1)[1] = G;
    (*rna1)[3] = C;
    (*rna1)[100] = T;
    (*rna1)[199] = A;

    EXPECT_EQ(A,rna1->getNucleotideInNucleotide(0) );
    EXPECT_EQ(G,rna1->getNucleotideInNucleotide(1) );
    EXPECT_EQ(C,rna1->getNucleotideInNucleotide(3) );
    EXPECT_EQ(T,rna1->getNucleotideInNucleotide(100) );
    EXPECT_EQ(A,rna1->getNucleotideInNucleotide(199) );
}

TEST_F(RNAFixture, isCorrectOperatorPreferences2){
    for(int i = 0; i < 25; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 25; i++){rna1->addNucleotide(A);}

    for(int i = 0; i < 50; i++){rna2->addNucleotide(A);}

    for(int i = 0; i < 25; i++){(*rna1)[i] = (*rna1)[i+25];}

    EXPECT_EQ(1,(*rna1) == (*rna2) );
}

TEST_F(RNAFixture, isCorrectOperatorSet) {
    for(int i = 0; i < 100; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 20; i++){rna1->addNucleotide(G);}

    rna2 = rna1;

    EXPECT_EQ(1,(*rna1) == (*rna2) );
}

TEST_F(RNAFixture, isCorrectSplitAndTrimWith1MTest){
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(G);}
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(C);}
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(A);}

    for(int i = 0; i < 250000; i++){rna2->addNucleotide(A);}

    RNA rna4 = rna1->split(750000);

    EXPECT_EQ(1,(rna4) == (*rna2) );
}

TEST_F(RNAFixture, isCorrectTrimWith1MTest){
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(G);}
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(C);}
    for(int i = 0; i < 250000; i++){rna1->addNucleotide(A);}

    rna1->trim(0);

    EXPECT_EQ(0,rna1->getNucleotideAmount() );
}

TEST_F(RNAFixture, isCorrectWorkWithEmptyRNA1){
    for(int i = 0; i < 25; i++){rna1->addNucleotide(G);}
    for(int i = 0; i < 50; i++){rna1->addNucleotide(C);}
    (*rna2)[10] = (*rna1)[23];
    (*rna2)[9] = (*rna1)[49];
    (*rna2)[8] = T;
    EXPECT_EQ("AAAAAAAATCG", rna2->nucleotideChainToString());
}

TEST_F(RNAFixture, isCorrectWorkWithEmptyRNA2){
    EXPECT_THROW({
        try { cout << (*rna1)[0]; }
        catch( const exception& e )
        {
            EXPECT_STREQ( "Index is larger than Nucleotide Amount - 1", e.what() );
            throw;
        }
        }, exception );
}

TEST_F(RNAFixture, isCorrectWorkWithEmptyRNA3){
    EXPECT_THROW({
        try {
            for(int i = 0; i < 100; i++){rna1->addNucleotide(T);}
            cout << (*rna1)[100];
        }
        catch( const exception& e )
        {
            EXPECT_STREQ( "Index is larger than Nucleotide Amount - 1", e.what() );
            throw;
        }
        }, exception );
}

TEST_F(RNAFixture, isCorrectWorkComplimentary){
    for(int i = 0; i < 100; i++){rna1->addNucleotide(T);}
    for(int i = 0; i < 100; i++){rna1->addNucleotide(G);}
    for(int i = 0; i < 100; i++){rna1->addNucleotide(C);}
    for(int i = 0; i < 100; i++){rna1->addNucleotide(A);}

    for(int i = 0; i < 100; i++){rna2->addNucleotide(A);}
    for(int i = 0; i < 100; i++){rna2->addNucleotide(C);}
    for(int i = 0; i < 100; i++){rna2->addNucleotide(G);}
    for(int i = 0; i < 100; i++){rna2->addNucleotide(T);}

    EXPECT_EQ(1, rna1->isComplimentary(*rna2));
}

TEST_F(RNAFixture, isCorrectWorkWithEmptyRNA4){
    EXPECT_THROW({
        try{
         rna1->trim(0); }
        catch( const exception& e )
         {
             EXPECT_STREQ( "Index is larger than Nucleotide Amount - 1", e.what() );
             throw;
         }
     }, exception );
}
