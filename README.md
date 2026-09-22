# 🔍 Plagiarism Detection System

A C++-based **Plagiarism Detection System** that analyzes and compares two text documents using multiple text-processing, similarity, hashing, and pattern-matching techniques.

The system evaluates similarity at the **word, phrase, and sentence levels** and generates an overall similarity score along with matching phrases, matching sentences, and document statistics.

> ⚠️ The final score is a similarity indicator based on multiple text-comparison algorithms. It should not be treated as definitive proof of plagiarism without human review.

---

## 📸 Project Overview

This project was developed as a practical application of **Data Structures and Algorithms in C++**.

Instead of relying on a single comparison technique, the system combines multiple approaches to analyze textual similarity:

- Jaccard Similarity
- Cosine Similarity
- 3-Word Shingling
- Rabin-Karp Pattern Matching
- Sentence-Level Similarity
- Hash Map-based Word Frequency Analysis
- Sorting of Matching Sentences

---

## ✨ Features

- 📄 Compare two text files
- ⌨️ Enter documents manually through the terminal
- 🔤 Clean and normalize text
- 🚫 Remove common stopwords
- 🔢 Calculate word frequencies
- 📊 Calculate Jaccard similarity
- 📐 Calculate Cosine similarity
- 🔗 Generate and compare 3-word shingles
- #️⃣ Detect matching phrases using Rabin-Karp hashing
- 📝 Compare sentences based on word overlap
- 📈 Generate an overall similarity score
- 🏷️ Classify the similarity level
- 📋 Display document statistics
- 🔍 Display matching phrases
- 🔎 Display highly similar sentences
- ℹ️ View information about the algorithms used

---

## 🔄 System Workflow

```text
                 ┌──────────────────────┐
                 │   Input Documents    │
                 │  File / Manual Text  │
                 └──────────┬───────────┘
                            │
                            ▼
                 ┌──────────────────────┐
                 │    Text Cleaning     │
                 │ Normalization        │
                 └──────────┬───────────┘
                            │
                            ▼
                 ┌──────────────────────┐
                 │     Tokenization     │
                 │  + Stopword Removal  │
                 └──────────┬───────────┘
                            │
              ┌─────────────┼─────────────┐
              ▼             ▼             ▼
       ┌────────────┐ ┌────────────┐ ┌─────────────┐
       │  Jaccard   │ │   Cosine   │ │  Shingling  │
       │ Similarity │ │ Similarity │ │  Similarity │
       └─────┬──────┘ └─────┬──────┘ └──────┬──────┘
             │              │               │
             └──────────────┼───────────────┘
                            │
                            ▼
                 ┌──────────────────────┐
                 │ Sentence Similarity  │
                 │ + Phrase Matching    │
                 │   (Rabin-Karp)       │
                 └──────────┬───────────┘
                            │
                            ▼
                 ┌──────────────────────┐
                 │ Final Similarity     │
                 │       Score          │
                 └──────────┬───────────┘
                            │
                            ▼
                 ┌──────────────────────┐
                 │ Classification +     │
                 │ Analysis Report      │
                 └──────────────────────┘

🛠️ Technologies Used
C++
Standard Template Library (STL)
vector
unordered_map
unordered_set
set
File handling
String processing
Hashing
Sorting
Algorithmic text comparison

🎯 Learning Outcomes
This project demonstrates practical implementation of:
Object/data organization using STL containers
Hash-based data structures
String processing
File handling
Set operations
Similarity measurement
Pattern matching
Hashing algorithms
Sorting algorithms
Modular C++ programming
Menu-driven application development

👩‍💻 Author
Piyusha Ghadigaonkar
Electronic and Computer Science Engineering Student
