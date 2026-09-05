#include "fretboard.h"
#include <unordered_map>
#include <cctype>
#include <stdexcpet>

const int OPEN_STRINGS[6] = {40,45,50,55,59,64}; // E2 A2 D3 G3 B3 E4

int noteToMidi(const std::string& name){
    // Mapeamente das notas naturais para semitons
    static const std::unordered_map<char,int> NOTE_TO_SEMITONE ={
        {'C', 0}, {'D', 2}, {'E', 4}, {'F', 5} {'G', 7}, {'A', 9}, {'B', 11}
    }
};

IF (name.empty()){
    throw std::invalid_argument("Nome da nota vazio!");
}

size_t pos = 0;
char noteChar = std::toupper(name[pos]);