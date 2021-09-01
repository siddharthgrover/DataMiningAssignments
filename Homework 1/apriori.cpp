/*
    It is not
    allowed to parse the complete input data and save it into an array or
   similar data structure. However, the frequent patterns and candidate sets can
   be stored in memory.
*/

/*
    References :-
    1. https://root-forum.cern.ch/t/reading-data-file-in-c-code/17126/2
*/

// TODO :- Sort the output lexographically before output.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

#define sz(a) int((a).size())
#define all(a) x.begin(), x.end()
#define pb push_back
#define endl '\n'
#define watch(x) cout << #x << " : " << x << endl;

map<int, int> mp;
// vector<int> cnts;
map<int, int> cnts;
vector<vector<int>> freq_sets, candidates;
int support;
const double threshold = 25;

void init_pass() {

  int dataset_size = 0;

  freq_sets.clear();

  ifstream inputFile(
      "/home/saksham/Desktop/DataMiningAssignments/Homework 1/webdocs.dat");
  string transaction;
  int item;
  while (getline(inputFile, transaction)) {
    if (!transaction.length())
      continue;
    dataset_size++;
    istringstream iss(transaction);
    while (iss >> item) {
      mp[item]++;
    }
  }

  support = ceil(threshold * 0.01 * dataset_size);

  for (auto it : mp) {
    if (it.second >= support) {
      vector<int> temp{it.first};
      freq_sets.pb(temp);
    }
  }
}

bool can_merge(int i, int j) {
  int num_items = sz(freq_sets[i]), cnt = 0;
  bool ok = true;
  auto it1 = freq_sets[i].begin();
  auto it2 = freq_sets[j].begin();
  while (ok && cnt < num_items - 1) {
    if (*it1 != *it2) {
      ok = false;
      break;
    }
    it1++;
    it2++;
    cnt++;
  }
  if (!ok)
    return false;
  if (*it1 >= *it2)
    return false;
  return true;
}

bool subset(vector<int> &merged, int to_remove) {
  vector<int> to_check;
  copy_if(merged.begin(), merged.end(), back_inserter(to_check),
          [to_remove](const int &t) { return t != to_remove; });
  bool ok = false;
  for (auto freq : freq_sets) {
    if (freq == to_check) {
      ok = true;
      break;
    }
  }
  return ok;
}

// Generate the candidate C(k) from F(k-1)
void candidate_gen() {
  candidates.clear();
  if (sz(freq_sets) == 0)
    return;
  int n = sz(freq_sets);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      bool merge = can_merge(i, j);
      if (merge) {
        vector<int> merged = freq_sets[i];
        merged.pb(freq_sets[j][sz(freq_sets[j]) - 1]);
        // Do a subset check now.
        bool ok = true;
        for (auto it : merged) {
          ok = ok & subset(merged, it);
        }
        if (ok) {
          // for (auto x : merged) cout<<x<<" ";cout<<endl;
          candidates.push_back(merged);
        }
      }
    }
  }
}

void generate_frequent_itemsets() {
  freq_sets.clear();
  cnts.clear();
  // cnts.resize((candidates.size(), 0));

  ifstream inputFile(
      "/home/saksham/Desktop/DataMiningAssignments/Homework 1/webdocs.dat");
  string transaction;
  int item;
  vector<int> trans;
  while (getline(inputFile, transaction)) {
    if (!transaction.length())
      continue;
    istringstream iss(transaction);
    while (iss >> item) {
      trans.push_back(item);
    }
    for (int i = 0; i < candidates.size(); i++) {
      bool ok = includes(trans.begin(), trans.end(), candidates[i].begin(),
                         candidates[i].end());
      if (ok)
        cnts[i]++;
    }
    trans.clear();
  }
  for (int i = 0; i < candidates.size(); i++) {
    if (cnts[i] >= support) {
      freq_sets.push_back(candidates[i]);
    }
  }
}

int main() {

  init_pass();

  for (auto v : freq_sets) {
    for (auto x : v)
      cout << x << " ";
    cout << endl;
  }

  while (true) {
    candidate_gen();
    if (candidates.size() == 0)
      break;
    generate_frequent_itemsets();
    for (auto v : freq_sets) {
      for (auto x : v)
        cout << x << " ";
      cout << endl;
    }
  }
}