#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>
#include <unordered_map>
#include <omp.h>
#include <cmath>
#include <cstring>


#define NGRAM_LENGTH 2

#include "utilities.h"

int main(int argc, char* argv[]) {
    const char* INPUT_PATH = argv[1];

    std::ifstream input,size;
    std::ofstream outputWords,outputLetters;

    std::string ngram[NGRAM_LENGTH],tmpString = "",line,words_ngram;

    int words_index,letters_index;
    char nextChar,letters_ngram[NGRAM_LENGTH + 1] = "";
    char* buffer;

    std::unordered_map<std::string,int> lettersHashtable,wordsHashtable;


    //auto start = std::chrono::system_clock::now();
    double start = omp_get_wtime();

    size.open(INPUT_PATH, std::ios::ate);
    const long int file_size = size.tellg();
    size.close();

    //leggo tutto il file nel buffer
    buffer = new char[file_size];
    input.open(INPUT_PATH,std::ios::binary);
    input.read(buffer,file_size);
    input.close();


    for(int i = 0; i < file_size; i++){
        nextChar = buffer[i];
        //NGRAMMI DI PAROLE
        if (computeWords(nextChar, ngram, tmpString, words_index)) {
            words_ngram = ngram[0] + " " + ngram[1];
            wordsHashtable[words_ngram] += 1;
            if (isgroupterminator(nextChar))
                words_index = 0;
            else {
                words_index = NGRAM_LENGTH - 1;
                shiftArrayofStrings(ngram);
            }
        }

        //NGRAMMI DI LETTERE
        if(computeLetters(nextChar,letters_ngram,letters_index)){
            lettersHashtable[letters_ngram] += 1;
            letters_ngram[0] = letters_ngram[1];
            letters_index = 1;
        }
    }



    //auto end = std::chrono::system_clock::now();
    //std::chrono::duration<double> elapsedTime = end-start;
    double elapsedTime = omp_get_wtime() - start;




    /* PRINT RESULTS */

    outputWords.open("../files/outputs/sequential_output_words.txt", std::ios::binary);
    outputLetters.open("../files/outputs/sequential_output_letters.txt", std::ios::binary);


    for ( auto mapIterator = wordsHashtable.begin(); mapIterator != lettersHashtable.end(); ++mapIterator )
        outputWords << mapIterator->first << " : " << mapIterator->second<<std::endl;
    outputWords << wordsHashtable.size() << " elementi trovati." << std::endl;

    for ( auto mapIterator = lettersHashtable.begin(); mapIterator != lettersHashtable.end(); ++mapIterator )
        outputLetters << mapIterator->first << " : " << mapIterator->second<<std::endl;
    outputLetters << lettersHashtable.size() << " elementi trovati." << std::endl;

    outputWords.close();
    outputLetters.close();

    std::cout << elapsedTime << std::endl;

    return 0;
}