#include "plagiarism.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <iomanip>

using namespace std;


// ============================================================
// TEXT CLEANING
// ============================================================

string cleanText(const string& text)
{
    string result;

    for (char ch : text)
    {
        if (isalnum(static_cast<unsigned char>(ch)))
        {
            result += static_cast<char>(
                tolower(static_cast<unsigned char>(ch))
            );
        }
        else
        {
            result += ' ';
        }
    }

    return result;
}


// ============================================================
// STOPWORDS
// ============================================================

unordered_set<string> createStopwords()
{
    unordered_set<string> stopwords =
    {
        "a", "an", "the",
        "is", "are", "was", "were",
        "am", "be", "been", "being",
        "to", "of", "in", "on", "at",
        "for", "from", "by",
        "with", "about",
        "and", "or", "but",
        "if", "then",
        "this", "that",
        "these", "those",
        "it", "its",
        "as",
        "into", "over", "under",
        "after", "before",
        "between",
        "through", "during",
        "very",
        "can", "could",
        "should", "would",
        "will",
        "do", "does", "did",
        "has", "have", "had",
        "not",
        "than",
        "so"
    };

    return stopwords;
}


// ============================================================
// TOKENIZATION
// ============================================================

vector<string> tokenize(
    const string& text,
    const unordered_set<string>& stopwords
)
{
    string cleaned = cleanText(text);

    stringstream ss(cleaned);

    vector<string> words;

    string word;

    while (ss >> word)
    {
        if (stopwords.find(word) == stopwords.end())
        {
            words.push_back(word);
        }
    }

    return words;
}


// ============================================================
// SENTENCE EXTRACTION
// ============================================================

vector<string> getSentences(const string& text)
{
    vector<string> sentences;

    string current;

    for (char ch : text)
    {
        current += ch;

        if (ch == '.' || ch == '?' || ch == '!')
        {
            string cleaned = cleanText(current);

            stringstream ss(cleaned);

            string word;
            string sentence;

            while (ss >> word)
            {
                if (!sentence.empty())
                {
                    sentence += " ";
                }

                sentence += word;
            }

            if (!sentence.empty())
            {
                sentences.push_back(sentence);
            }

            current.clear();
        }
    }

    // Handle text without final punctuation
    if (!current.empty())
    {
        string cleaned = cleanText(current);

        stringstream ss(cleaned);

        string word;
        string sentence;

        while (ss >> word)
        {
            if (!sentence.empty())
            {
                sentence += " ";
            }

            sentence += word;
        }

        if (!sentence.empty())
        {
            sentences.push_back(sentence);
        }
    }

    return sentences;
}


// ============================================================
// FILE READING
// ============================================================

string readFile(const string& filename)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "\nError: Unable to open file: "
             << filename << endl;

        return "";
    }

    stringstream buffer;

    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}


// ============================================================
// WORD FREQUENCY USING HASH MAP
// ============================================================

unordered_map<string, int> wordFrequency(
    const vector<string>& words
)
{
    unordered_map<string, int> frequency;

    for (const string& word : words)
    {
        frequency[word]++;
    }

    return frequency;
}


// ============================================================
// JACCARD SIMILARITY
// ============================================================

double jaccardSimilarity(
    const vector<string>& words1,
    const vector<string>& words2
)
{
    set<string> set1(
        words1.begin(),
        words1.end()
    );

    set<string> set2(
        words2.begin(),
        words2.end()
    );

    if (set1.empty() && set2.empty())
    {
        return 100.0;
    }

    set<string> intersection;

    set<string> unionSet;

    set_intersection(
        set1.begin(),
        set1.end(),
        set2.begin(),
        set2.end(),
        inserter(
            intersection,
            intersection.begin()
        )
    );

    set_union(
        set1.begin(),
        set1.end(),
        set2.begin(),
        set2.end(),
        inserter(
            unionSet,
            unionSet.begin()
        )
    );

    if (unionSet.empty())
    {
        return 0.0;
    }

    return (
        static_cast<double>(intersection.size())
        / unionSet.size()
    ) * 100.0;
}


// ============================================================
// COSINE SIMILARITY
// ============================================================

double cosineSimilarity(
    const unordered_map<string, int>& freq1,
    const unordered_map<string, int>& freq2
)
{
    if (freq1.empty() || freq2.empty())
    {
        return 0.0;
    }

    double dotProduct = 0.0;

    double magnitude1 = 0.0;

    double magnitude2 = 0.0;

    for (const auto& item : freq1)
    {
        const string& word = item.first;

        int count1 = item.second;

        auto it = freq2.find(word);

        if (it != freq2.end())
        {
            dotProduct +=
                static_cast<double>(count1)
                * it->second;
        }

        magnitude1 +=
            static_cast<double>(count1)
            * count1;
    }

    for (const auto& item : freq2)
    {
        int count = item.second;

        magnitude2 +=
            static_cast<double>(count)
            * count;
    }

    magnitude1 = sqrt(magnitude1);

    magnitude2 = sqrt(magnitude2);

    if (magnitude1 == 0.0 ||
        magnitude2 == 0.0)
    {
        return 0.0;
    }

    double similarity =
        dotProduct /
        (magnitude1 * magnitude2);

    return similarity * 100.0;
}


// ============================================================
// CREATE SHINGLES
// ============================================================

vector<string> createShingles(
    const vector<string>& words,
    int k
)
{
    vector<string> shingles;

    if (words.size() <
        static_cast<size_t>(k))
    {
        return shingles;
    }

    for (size_t i = 0;
         i + k <= words.size();
         i++)
    {
        string shingle;

        for (int j = 0; j < k; j++)
        {
            if (!shingle.empty())
            {
                shingle += " ";
            }

            shingle += words[i + j];
        }

        shingles.push_back(shingle);
    }

    return shingles;
}


// ============================================================
// SHINGLE SIMILARITY
// ============================================================

double shingleSimilarity(
    const vector<string>& shingles1,
    const vector<string>& shingles2
)
{
    set<string> set1(
        shingles1.begin(),
        shingles1.end()
    );

    set<string> set2(
        shingles2.begin(),
        shingles2.end()
    );

    if (set1.empty() && set2.empty())
    {
        return 0.0;
    }

    set<string> intersection;

    set<string> unionSet;

    set_intersection(
        set1.begin(),
        set1.end(),
        set2.begin(),
        set2.end(),
        inserter(
            intersection,
            intersection.begin()
        )
    );

    set_union(
        set1.begin(),
        set1.end(),
        set2.begin(),
        set2.end(),
        inserter(
            unionSet,
            unionSet.begin()
        )
    );

    if (unionSet.empty())
    {
        return 0.0;
    }

    return (
        static_cast<double>(intersection.size())
        / unionSet.size()
    ) * 100.0;
}


// ============================================================
// RABIN-KARP HASH
// ============================================================

long long calculateHash(
    const string& text,
    int start,
    int length
)
{
    const long long BASE = 257;

    const long long MOD = 1000000007;

    long long hashValue = 0;

    for (int i = start;
         i < start + length;
         i++)
    {
        hashValue =
            (hashValue * BASE + text[i])
            % MOD;
    }

    return hashValue;
}


// ============================================================
// RABIN-KARP STRING SEARCH
// ============================================================

bool rabinKarp(
    const string& text,
    const string& pattern
)
{
    if (pattern.empty())
    {
        return true;
    }

    if (text.length() < pattern.length())
    {
        return false;
    }

    const long long BASE = 257;

    const long long MOD = 1000000007;

    int m =
        static_cast<int>(pattern.length());

    int n =
        static_cast<int>(text.length());

    long long patternHash =
        calculateHash(
            pattern,
            0,
            m
        );

    long long textHash =
        calculateHash(
            text,
            0,
            m
        );

    long long highestPower = 1;

    for (int i = 1;
         i <= m - 1;
         i++)
    {
        highestPower =
            (highestPower * BASE)
            % MOD;
    }

    for (int i = 0;
         i <= n - m;
         i++)
    {
        if (patternHash == textHash)
        {
            bool match = true;

            for (int j = 0;
                 j < m;
                 j++)
            {
                if (text[i + j] != pattern[j])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                return true;
            }
        }

        if (i < n - m)
        {
            textHash =
                (
                    BASE *
                    (
                        textHash -
                        text[i] * highestPower
                    )
                    + text[i + m]
                ) % MOD;

            if (textHash < 0)
            {
                textHash += MOD;
            }
        }
    }

    return false;
}


// ============================================================
// FIND MATCHING PHRASES
// USING RABIN-KARP
// ============================================================

vector<string> findMatchingPhrases(
    const vector<string>& words1,
    const vector<string>& words2,
    int k
)
{
    vector<string> matches;

    if (words1.size() <
            static_cast<size_t>(k) ||
        words2.size() <
            static_cast<size_t>(k))
    {
        return matches;
    }

    vector<string> shingles1 =
        createShingles(
            words1,
            k
        );

    vector<string> shingles2 =
        createShingles(
            words2,
            k
        );

    // Combine Document 2 phrases
    // into one searchable string.
    string document2;

    for (const string& shingle : shingles2)
    {
        if (!document2.empty())
        {
            document2 += " | ";
        }

        document2 += shingle;
    }

    set<string> alreadyMatched;

    for (const string& phrase : shingles1)
    {
        // Rabin-Karp search
        bool foundByRabinKarp =
            rabinKarp(
                document2,
                phrase
            );

        if (!foundByRabinKarp)
        {
            continue;
        }

        // Exact phrase verification
        // to avoid substring false matches.
        bool exactMatch =
            find(
                shingles2.begin(),
                shingles2.end(),
                phrase
            ) != shingles2.end();

        if (exactMatch &&
            alreadyMatched.find(phrase)
                == alreadyMatched.end())
        {
            matches.push_back(phrase);

            alreadyMatched.insert(phrase);
        }
    }

    return matches;
}


// ============================================================
// SENTENCE SIMILARITY
// ============================================================

double sentenceSimilarity(
    const string& sentence1,
    const string& sentence2
)
{
    unordered_set<string> stopwords =
        createStopwords();

    vector<string> words1 =
        tokenize(
            sentence1,
            stopwords
        );

    vector<string> words2 =
        tokenize(
            sentence2,
            stopwords
        );

    set<string> set1(
        words1.begin(),
        words1.end()
    );

    set<string> set2(
        words2.begin(),
        words2.end()
    );

    if (set1.empty() ||
        set2.empty())
    {
        return 0.0;
    }

    set<string> intersection;

    set_intersection(
        set1.begin(),
        set1.end(),
        set2.begin(),
        set2.end(),
        inserter(
            intersection,
            intersection.begin()
        )
    );

    size_t smallerSize =
        min(
            set1.size(),
            set2.size()
        );

    if (smallerSize == 0)
    {
        return 0.0;
    }

    return (
        static_cast<double>(intersection.size())
        / smallerSize
    ) * 100.0;
}


// ============================================================
// FIND MATCHING SENTENCES
// ============================================================

vector<pair<string, string>> findMatchingSentences(
    const vector<string>& sentences1,
    const vector<string>& sentences2,
    double threshold
)
{
    vector<pair<string, string>> matches;

    for (const string& sentence1 : sentences1)
    {
        double bestScore = 0.0;

        string bestMatch;

        for (const string& sentence2 : sentences2)
        {
            double score =
                sentenceSimilarity(
                    sentence1,
                    sentence2
                );

            if (score > bestScore)
            {
                bestScore = score;

                bestMatch = sentence2;
            }
        }

        if (bestScore >= threshold)
        {
            string result =
                sentence1
                + " ["
                + to_string(
                    static_cast<int>(bestScore)
                )
                + "%]";

            string matchedResult =
                bestMatch
                + " ["
                + to_string(
                    static_cast<int>(bestScore)
                )
                + "%]";

            matches.push_back(
                {
                    result,
                    matchedResult
                }
            );
        }
    }

    return matches;
}


// ============================================================
// SORT MATCHING SENTENCES
// ============================================================

void sortMatchingSentences(
    vector<pair<string, string>>& matches
)
{
    sort(
        matches.begin(),
        matches.end(),
        [](const pair<string, string>& a,
           const pair<string, string>& b)
        {
            auto extractScore =
                [](const string& text)
                {
                    size_t start =
                        text.rfind('[');

                    size_t end =
                        text.rfind('%');

                    if (start == string::npos ||
                        end == string::npos ||
                        end <= start)
                    {
                        return 0;
                    }

                    string number =
                        text.substr(
                            start + 1,
                            end - start - 1
                        );

                    try
                    {
                        return stoi(number);
                    }
                    catch (...)
                    {
                        return 0;
                    }
                };

            return extractScore(a.first)
                   >
                   extractScore(b.first);
        }
    );
}


// ============================================================
// SIMILARITY CLASSIFICATION
// ============================================================

void displayClassification(double score)
{
    cout << "\nSimilarity Status : ";

    if (score < 20)
    {
        cout << "LOW SIMILARITY";
    }
    else if (score < 40)
    {
        cout << "SLIGHT SIMILARITY";
    }
    else if (score < 60)
    {
        cout << "MODERATE SIMILARITY";
    }
    else if (score < 80)
    {
        cout << "HIGH SIMILARITY";
    }
    else
    {
        cout << "VERY HIGH SIMILARITY";
    }

    cout << endl;
}


// ============================================================
// ALGORITHM INFORMATION
// ============================================================

void displayAlgorithmInformation()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "             ALGORITHM INFORMATION\n";
    cout << "====================================================\n";

    cout << "\n1. TOKENIZATION\n";
    cout << "   Converts text into individual words.\n";

    cout << "\n2. STOPWORD REMOVAL\n";
    cout << "   Removes common words such as 'the', 'is', 'and'.\n";

    cout << "\n3. HASH MAP\n";
    cout << "   unordered_map stores word frequencies.\n";

    cout << "\n4. JACCARD SIMILARITY\n";
    cout << "   Compares unique words in both documents.\n";

    cout << "\n5. COSINE SIMILARITY\n";
    cout << "   Compares documents using word-frequency vectors.\n";

    cout << "\n6. SHINGLING\n";
    cout << "   Creates consecutive groups of 3 words.\n";

    cout << "\n7. RABIN-KARP\n";
    cout << "   Uses hashing for efficient phrase searching.\n";

    cout << "\n8. SENTENCE COMPARISON\n";
    cout << "   Finds sentences with high word overlap.\n";

    cout << "\n9. SORTING\n";
    cout << "   Ranks matching sentences by similarity.\n";

    cout << "\n10. FINAL SCORE\n";
    cout << "    Combines multiple similarity techniques.\n";

    cout << "\n====================================================\n";
}


// ============================================================
// COMPLETE DOCUMENT ANALYSIS
// ============================================================

void analyzeDocuments(
    const string& text1,
    const string& text2
)
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "             ANALYZING DOCUMENTS...\n";
    cout << "====================================================\n";


    // --------------------------------------------------------
    // STEP 1: CREATE STOPWORD SET
    // --------------------------------------------------------

    unordered_set<string> stopwords =
        createStopwords();


    // --------------------------------------------------------
    // STEP 2: TOKENIZATION
    // --------------------------------------------------------

    vector<string> words1 =
        tokenize(
            text1,
            stopwords
        );

    vector<string> words2 =
        tokenize(
            text2,
            stopwords
        );


    // --------------------------------------------------------
    // STEP 3: SENTENCE EXTRACTION
    // --------------------------------------------------------

    vector<string> sentences1 =
        getSentences(text1);

    vector<string> sentences2 =
        getSentences(text2);


    // --------------------------------------------------------
    // STEP 4: WORD FREQUENCY
    // --------------------------------------------------------

    unordered_map<string, int> frequency1 =
        wordFrequency(words1);

    unordered_map<string, int> frequency2 =
        wordFrequency(words2);


    // --------------------------------------------------------
    // STEP 5: JACCARD SIMILARITY
    // --------------------------------------------------------

    double jaccard =
        jaccardSimilarity(
            words1,
            words2
        );


    // --------------------------------------------------------
    // STEP 6: COSINE SIMILARITY
    // --------------------------------------------------------

    double cosine =
        cosineSimilarity(
            frequency1,
            frequency2
        );


    // --------------------------------------------------------
    // STEP 7: SHINGLING
    // --------------------------------------------------------

    const int K = 3;

    vector<string> shingles1 =
        createShingles(
            words1,
            K
        );

    vector<string> shingles2 =
        createShingles(
            words2,
            K
        );

    double shingleScore =
        shingleSimilarity(
            shingles1,
            shingles2
        );


    // --------------------------------------------------------
    // STEP 8: SENTENCE SIMILARITY
    // --------------------------------------------------------

    vector<pair<string, string>> matchingSentences =
        findMatchingSentences(
            sentences1,
            sentences2,
            60.0
        );


    double sentenceScore = 0.0;

    if (!sentences1.empty())
    {
        double totalBestSimilarity = 0.0;

        for (const string& sentence1 : sentences1)
        {
            double best = 0.0;

            for (const string& sentence2 : sentences2)
            {
                double score =
                    sentenceSimilarity(
                        sentence1,
                        sentence2
                    );

                best = max(
                    best,
                    score
                );
            }

            totalBestSimilarity += best;
        }

        sentenceScore =
            totalBestSimilarity /
            sentences1.size();
    }


    // --------------------------------------------------------
    // STEP 9: RABIN-KARP PHRASE MATCHING
    // --------------------------------------------------------

    vector<string> matchingPhrases =
        findMatchingPhrases(
            words1,
            words2,
            K
        );


    // --------------------------------------------------------
    // STEP 10: SORT MATCHING SENTENCES
    // --------------------------------------------------------

    sortMatchingSentences(
        matchingSentences
    );


    // --------------------------------------------------------
    // STEP 11: FINAL SIMILARITY SCORE
    // --------------------------------------------------------

    double finalScore =
        (0.25 * jaccard)
        +
        (0.25 * cosine)
        +
        (0.25 * shingleScore)
        +
        (0.25 * sentenceScore);


    // --------------------------------------------------------
    // ANALYSIS REPORT
    // --------------------------------------------------------

    cout << fixed
         << setprecision(2);

    cout << "\n";

    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║              SIMILARITY ANALYSIS REPORT            ║\n";
    cout << "╠════════════════════════════════════════════════════╣\n";

    cout << "║ Word Similarity       : "
         << setw(6)
         << jaccard
         << "%                         ║\n";

    cout << "║ Cosine Similarity     : "
         << setw(6)
         << cosine
         << "%                         ║\n";

    cout << "║ Phrase Similarity     : "
         << setw(6)
         << shingleScore
         << "%                         ║\n";

    cout << "║ Sentence Similarity   : "
         << setw(6)
         << sentenceScore
         << "%                         ║\n";

    cout << "╠════════════════════════════════════════════════════╣\n";

    cout << "║ FINAL SIMILARITY SCORE : "
         << setw(6)
         << finalScore
         << "%                        ║\n";

    cout << "╚════════════════════════════════════════════════════╝\n";


    displayClassification(
        finalScore
    );


    // --------------------------------------------------------
    // DOCUMENT STATISTICS
    // --------------------------------------------------------

    cout << "\n";
    cout << "====================================================\n";
    cout << "               DOCUMENT STATISTICS\n";
    cout << "====================================================\n";

    cout << "\nDocument 1:";

    cout << "\nWords             : "
         << words1.size();

    cout << "\nUnique Words      : "
         << frequency1.size();

    cout << "\nSentences         : "
         << sentences1.size();

    cout << "\n3-Word Shingles   : "
         << shingles1.size();


    cout << "\n\nDocument 2:";

    cout << "\nWords             : "
         << words2.size();

    cout << "\nUnique Words      : "
         << frequency2.size();

    cout << "\nSentences         : "
         << sentences2.size();

    cout << "\n3-Word Shingles   : "
         << shingles2.size();


    // --------------------------------------------------------
    // MATCHING PHRASES
    // --------------------------------------------------------

    cout << "\n\n";
    cout << "====================================================\n";
    cout << "              MATCHING 3-WORD PHRASES\n";
    cout << "====================================================\n";

    if (matchingPhrases.empty())
    {
        cout << "\nNo matching phrases found.\n";
    }
    else
    {
        int count = 1;

        for (const string& phrase :
             matchingPhrases)
        {
            cout << count
                 << ". "
                 << phrase
                 << endl;

            count++;

            if (count > 15)
            {
                cout << "... and more.\n";
                break;
            }
        }
    }


    // --------------------------------------------------------
    // MATCHING SENTENCES
    // --------------------------------------------------------

    cout << "\n";
    cout << "====================================================\n";
    cout << "              MATCHING SENTENCES\n";
    cout << "====================================================\n";

    if (matchingSentences.empty())
    {
        cout << "\nNo highly similar sentences found.\n";
    }
    else
    {
        int count = 1;

        for (const auto& match :
             matchingSentences)
        {
            cout << "\nMatch "
                 << count
                 << ":\n";

            cout << "Document 1: "
                 << match.first
                 << endl;

            cout << "Document 2: "
                 << match.second
                 << endl;

            count++;

            if (count > 10)
            {
                cout << "\n... and more matching sentences.\n";
                break;
            }
        }
    }


    // --------------------------------------------------------
    // NOTE
    // --------------------------------------------------------

    cout << "\n";
    cout << "====================================================\n";
    cout << "NOTE: The final score is a similarity indicator\n";
    cout << "based on multiple text-comparison algorithms.\n";
    cout << "It should not be treated as definitive proof of\n";
    cout << "plagiarism without human review.\n";
    cout << "====================================================\n";
}