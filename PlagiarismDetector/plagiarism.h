#ifndef PLAGIARISM_H
#define PLAGIARISM_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using namespace std;

// ---------------- TEXT PROCESSING ----------------

string cleanText(const string& text);

unordered_set<string> createStopwords();

vector<string> tokenize(
    const string& text,
    const unordered_set<string>& stopwords
);

vector<string> getSentences(const string& text);


// ---------------- FILE HANDLING ----------------

string readFile(const string& filename);


// ---------------- FREQUENCY ANALYSIS ----------------

unordered_map<string, int> wordFrequency(
    const vector<string>& words
);


// ---------------- SIMILARITY ALGORITHMS ----------------

double jaccardSimilarity(
    const vector<string>& words1,
    const vector<string>& words2
);

double cosineSimilarity(
    const unordered_map<string, int>& freq1,
    const unordered_map<string, int>& freq2
);


// ---------------- SHINGLING ----------------

vector<string> createShingles(
    const vector<string>& words,
    int k
);

double shingleSimilarity(
    const vector<string>& shingles1,
    const vector<string>& shingles2
);


// ---------------- RABIN-KARP ----------------

long long calculateHash(
    const string& text,
    int start,
    int length
);

bool rabinKarp(
    const string& text,
    const string& pattern
);

vector<string> findMatchingPhrases(
    const vector<string>& words1,
    const vector<string>& words2,
    int k
);


// ---------------- SENTENCE COMPARISON ----------------

double sentenceSimilarity(
    const string& sentence1,
    const string& sentence2
);

vector<pair<string, string>> findMatchingSentences(
    const vector<string>& sentences1,
    const vector<string>& sentences2,
    double threshold
);


// ---------------- SORTING ----------------

void sortMatchingSentences(
    vector<pair<string, string>>& matches
);


// ---------------- DISPLAY ----------------

void displayClassification(double score);

void displayAlgorithmInformation();


// ---------------- MAIN ANALYSIS ----------------

void analyzeDocuments(
    const string& text1,
    const string& text2
);

#endif