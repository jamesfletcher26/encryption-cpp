//
//  main.cpp
//  Project_2
//
//  Created by James Fletcher on 4/4/23.
//
// file path is: /Users/jamesfletcher/Desktop/CSE_232/Project_2/test.txt
//
// execute in terminal:
//
// g++ -std=c++17 -Wall main.cpp -o proj2
//
// echo /Users/jamesfletcher/Desktop/CSE_232/Project_2/test.txt aA eE iI | ./proj2
//
#include "header.hpp"
// function to open the file w/ error check
ifstream open_file(string& filename) {
    // open the file, check if it is open
    ifstream input(filename);
    if (!input.is_open()) {
        cerr << "ERROR: filename " << filename <<  " not found." << endl;
        exit(1);
    }
    return input;
}
// function to read encoding sequence into an unordered map w/ error check
unordered_map<char, char> read_pairs(string& encoding_sequence) {
    unordered_map<char, char> encoding_map;
    // build the map
    istringstream iss(encoding_sequence);
    string str_pair;
    while (iss >> str_pair) {
        // check for error
        if (encoding_map.count(str_pair.at(0))) {
            cerr << "ERROR: encoding contains key with multiple values." << endl;
            exit(1);
        }
        if (str_pair == "q" || str_pair.length() > 2) {
            cerr << "ERROR: encoding contains invalid pair." << endl;
            exit(1);
        }
        encoding_map.insert(make_pair(str_pair.at(0), str_pair.at(1)));
    }
    return encoding_map;
}
// function to read keys into a vector, preserving order
// no error checks yet
vector<char> read_keys(string& encoding_sequence) {
    // make a vector to store the keys in their original order
    // make map to store encoding pairs
    vector<char> keys;
    istringstream iss(encoding_sequence);
    string key;
    while (iss >> key) {
        keys.push_back(key.at(0));
    }
    return keys;
}
// helper to return encoded letter
char single_letter(char& c, unordered_map<char, char>& encoding_map) {
    if (encoding_map.contains(c)) {
        return encoding_map.at(c);
    } else {
        return c;
    }
}
string apply_encoding_to_word(string word, unordered_map<char, char>& encoding_map) {
    // insert the encoding value if the key exists in the word
    transform(word.begin(), word.end(), word.begin(), [&encoding_map](char c) {
        return single_letter(c, encoding_map);
    });
    return word;
}
// function to encode a line
string apply_encoding_to_line(string line, unordered_map<char, char>& encoding_map) {
    string transformed_line, word, encoded_word;
    istringstream iss(line);
    ostringstream oss;
    // grab a word
    while (iss >> word) {
        // encode one word at a time
        encoded_word = apply_encoding_to_word(word, encoding_map);
        oss << encoded_word << " ";
    }
    transformed_line = oss.str();
    return transformed_line;
}
//
// main
//
int main() {
    string filename;
    vector<string> line_list;
    string line, encoded_line, word, encoded_word;
    string pair;
    stringstream ss;
    // open the file
    cout << "give me a filename: " << endl;
    cin >> filename;
    ifstream file = open_file(filename);
    // read encoding sequence
    while (cin >> pair) {
        ss << pair << " ";
    }
    string encoding_sequence = ss.str();
    // use encoding sequence to build a map
    unordered_map<char, char> encoding_map = read_pairs(encoding_sequence);
    // read the file, line by line, apply encoding
    while (getline(file, line)) {
        encoded_line = apply_encoding_to_line(line, encoding_map);
        // check if there are duplicate lines
        if (find(line_list.begin(), line_list.end(), encoded_line) == line_list.end()) {
            line_list.push_back(encoded_line);
        }
    }
    // print out the encoded line
    for (auto i : line_list) {
        cout << i << endl;
    }
    return 0;
}
