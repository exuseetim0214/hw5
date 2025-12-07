#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <array>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

static std::array<std::set<char>, 26> posletters;

static void wordle_helper(std::string& curr, 
                          std::size_t idx,  
                          std::array<int,26>& need, 
                          int need_left, 
                          int blanks_left, 
                          const std::set<std::string>& dict,
                          std::set<std::string>& output)
{
    if(need_left > blanks_left) return;

    if(idx == curr.size()){
        if(need_left ==0 && dict.find(curr) != dict.end()){
            output.insert(curr);
        }
        return;
    }

    if(curr[idx] != '-'){
        wordle_helper(curr, idx+1, need, need_left, blanks_left, dict, output);
        return;
    }

    if(blanks_left == need_left){
        for(int k =0; k <26; k++){
            if( need[k] == 0) continue;
            curr[idx] = static_cast<char>('a' + k);
            need[k]--;

            wordle_helper(curr, idx+1, need, need_left -1, blanks_left -1, dict, output);

            need[k]++;
        }
        curr[idx] = '-';
        return;
    }

    for(int k =0; k < 26; k++){
        if(need[k] > 0){
            curr[idx] = static_cast<char>('a' + k);
            need[k]--;
            wordle_helper(curr, idx+1, need, need_left -1, blanks_left -1, dict, output);
            need[k]++;
        }
    }
        for(char c : posletters[idx]){
            int k = c -'a';
            if(need[k] > 0){
            curr[idx] = c;
            need[k]--;
            wordle_helper(curr, idx+1, need, need_left -1, blanks_left -1, dict, output);
            need[k]++;
        } 
        
        else if(blanks_left > need_left){
                curr[idx] =  c;
                wordle_helper(curr, idx+1, need, need_left, blanks_left -1, dict, output);
            }
        }

    curr[idx] = '-';
    
}

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> output;

    int blanks = 0;

    for(std::size_t i =0; i < in.size(); i++){
        if(in[i] == '-'){
            blanks++;
        }
    }
    
    std::array<int, 26> need{};
    int need_left = 0;

    for(std::size_t i =0; i < floating.size(); i++){
        char c = floating[i];
        if(c >= 'a' && c <= 'z'){
            need[c - 'a']++;
            need_left++;
        }
    }
    for( size_t i =0; i < posletters.size(); i++){
        posletters[i].clear();
    }
    for(const std::string& w_orig : dict){
        if(w_orig.size() == in.size()){
            std::string w = w_orig;
            for(char &c : w) c = std::tolower(c);
            
            for(size_t i =0; i < w.size(); i++){
                posletters[i].insert(w[i]);
            }
        }
    }
    std::string curr = in;
    for(char &c : curr) c = std::tolower(c);
    wordle_helper(curr, 0, need, need_left, blanks, dict, output);

    return output;
}

// Define any helper functions here
