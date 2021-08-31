/*
    It is not
    allowed to parse the complete input data and save it into an array or similar data
    structure. However, the frequent patterns and candidate sets can be stored in memory.
*/

/*
    References :-
    1. https://root-forum.cern.ch/t/reading-data-file-in-c-code/17126/2
    2. 
*/

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <list>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <map>
#include <set>

using namespace std;

// struct candidate {
//     set<int> itemset;
//     int count;
// };

map<int, int> mp;
// vector<candidate> freq_sets, freq_sets_prev;

vector<vector<int>> freq_sets, freq_sets_prev, candidates;
vector<int> cnt_sets, cnt_sets_prev;

/*
    1. Generate candidate itemsets in order of length.
    2. Tests each such candidate itemset for support threshold.
    3. Uses all frequent itemsets of a particular length to generate candidates having length one more.
*/

//! NOTE :- can try unordered_map instead of map.

// Generate the initial candidate itemset C1 and F1 from that C1.
/*
    * Checked Working Fine.
    @param support threshold.
*/
void init_pass (int support) {

    mp.clear();

    ifstream inputFile ("/home/saksham/Desktop/DataMiningAssignments/Homework 1/temp.dat");
    string transaction;
    int item;
    while (getline(inputFile, transaction)) {
        if (!transaction.length()) continue;
        istringstream iss(transaction);
        while (iss >> item) {
            mp[item]++;
        }
    }

    // for (auto it : mp) {
    //     cout<<it.first<<"   "<<it.second<<endl;
    // }

    for (auto it : mp) {
        if (it.second >= support) {
            vector<int> temp;
            temp.push_back(it.first);
            freq_sets.push_back(temp);
        }
    }

    // for (auto can : freq_sets) {
    //     for (auto x : can.itemset) cout<<x<<" ";
    //     cout<<can.count<<" ";
    //     cout<<endl;
    // }

}


bool can_merge (vector<int> f1, vector<int> f2) {
    int num_items = f1.size(), cnt = 0;
    bool ok = true;
    auto it1 = f1.begin();
    auto it2 = f2.begin();
    while (ok && cnt < num_items-1) {
        if (*it1 != *it2) {
            ok = false;
            break;
        }
        it1++;
        it2++;
        cnt++; 
    }
    if (!ok) return false;
    if (*it1 >= *it2) return false;
    return true;
}

bool subset (vector<int>& merged, int to_remove) {

    vector<int> to_check;
    copy_if(merged.begin(), merged.end(), back_inserter(to_check), [to_remove](const int& t) { return t != to_remove; });

    bool ok = false;
    for (auto freq : freq_sets_prev) {
        if (freq == to_check) {
            ok = true;
            break;
        }
    }

    return ok;
}

// Generate the candidate C(k) from F(k-1)
void candidate_gen () {
    candidates.clear();
    freq_sets_prev = freq_sets;
    if (freq_sets_prev.size() == 0) {
        return;
    }
    for (int i=0;i<freq_sets_prev.size();i++) {
        for (int j=0;j<freq_sets_prev.size();j++) {
            if (i == j) continue;
            vector<int> f1 = freq_sets_prev[i];
            vector<int> f2 = freq_sets_prev[j];
            // The conditions are satisfied for merging.
            bool merge = can_merge(f1, f2);
            if (merge) {

                vector<int> merged = f1;
                merged.push_back(f2[f2.size()-1]);

                // Do a subset check now.
                bool ok = true;
                for (auto it : merged) {
                    ok = ok & subset(merged, it);
                }

                if (ok) {
                    // for (auto x : merged) cout<<x<<" ";cout<<endl;
                    candidates.push_back(merged);
                }
                // cout<<*(f1.begin())<<" "<<*(f2.begin())<<endl;
            }
        }
    }
}

void generate_frequent_itemsets (int support) {

    freq_sets.clear();
    vector<int> cnts (candidates.size(), 0);

    // Now check for every candidate in the database.

    // Iterate through the transactions.
    ifstream inputFile ("/home/saksham/Desktop/DataMiningAssignments/Homework 1/temp.dat");
    string transaction;
    int item;
    vector<int> trans;
    while (getline(inputFile, transaction)) {
        if (!transaction.length()) continue;
        istringstream iss(transaction);
        while (iss >> item) {
            trans.push_back(item);
        }
        // for (auto x : trans) cout<<x<<" ";cout<<endl;
        for (int i=0;i<candidates.size();i++) {
            bool ok = includes(trans.begin(), trans.end(), candidates[i].begin(), candidates[i].end());
            if (ok) cnts[i]++;
        }
        trans.clear();
    }

    // cout<<"----------------------------------------"<<endl;

    for (int i=0;i<candidates.size();i++) {
        if (cnts[i] >= support) {
            freq_sets.push_back(candidates[i]);
        }
        // cout<<cnts[i]<<endl;
        // for (auto x : candidates[i]) cout<<x<<" ";cout<<endl;
    }   
}

int main (int argc, char *argv[]) {

    // ifstream inputFile ("/home/saksham/Desktop/DataMiningAssignments/Homework 1/webdocs.dat");
    // string line;
    // int num;
    // while (getline(inputFile, line)) {
    //     if (!line.length()) continue;
    //     istringstream iss(line);
    //     while (iss >> num) {
    //     }
    // }

    init_pass(10);   

    while (true) {
        candidate_gen();
        if (candidates.size() == 0) break;
        generate_frequent_itemsets(10);
        for (auto ff : freq_sets) {
            for (auto x : ff) cout<<x<<" ";cout<<endl;
        }
    }
 
    // for (auto ff : freq_sets) {
    //     for (auto x : ff) cout<<x<<" ";cout<<endl;
    // }

    // for (auto cc : candidates) {
    //     for (auto x : cc) cout<<x<<" ";cout<<endl;
    // }

    return 0;
}