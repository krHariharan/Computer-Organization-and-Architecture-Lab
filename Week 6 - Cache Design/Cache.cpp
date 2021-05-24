/*
 * Team members:
 * A Samuel Moses cs19b003
 * K R Hariharan cs19b079
 * Shree Vishnu cs19b045
 */
#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <bitset>
#include <set>
#include <list>
#include <iterator>
#include <utility>


using namespace std;

//counters
long long int cacheRef, cacheMiss, readAcc, writeAcc, compMiss, capMiss, confMiss, readMiss, writeMiss, dirtyEvict;

struct CacheBlock {
    string tag;
    int valid;
    int dirty;

    CacheBlock();

    CacheBlock(string, int, int);
};

CacheBlock::CacheBlock() {
    tag = "";
    valid = 0;
    dirty = 0;
}

CacheBlock::CacheBlock(string t, int v, int d) {
    tag = t;
    valid = v;
    dirty = d;
}

class PLRUTree {

    long n;
    int k; //n = 2^k ways
    vector<bool> tree; //array for binary tree

    static long left(long i) {
        return 2 * i + 1;
    }

    static long right(long i) {
        return 2 * i + 2;
    }


public:
    PLRUTree(long assc, int k);

    long replace();

    void read(long ind);
};

PLRUTree::PLRUTree(long assc, int k) {
    n = assc - 1;
    this->k = k;
    tree.resize(n, 0);
}

long PLRUTree::replace() {
    long i = 0;
    while (i < n) {//if i is the the tree part
        bool bit = tree[i];
        tree[i] = !bit; //all the bits in path accessed are flipped
        if (bit) { //if bit is 1 move to the right
            i = right(i);
        } else {//else move left
            i = left(i);
        }
    }

    return i - n; //the cache is in the last part. we are renumbering the cache from 0 by subtracting n
}

void PLRUTree::read(long ind) {
    long i = 0;
    //we traverse by using the binary representation of the index
    //if MSB is 1 move right, else move left
    //then we do the same for the next bin digit
    //we also make the bits in tree opposite to the path travelled
    for (int j = k-1; j >= 0; --j) {
        bool bit = (ind & (1 << j)) > 0;
        tree[i] = !bit;
        if (bit)
            i = right(i);
        else
            i = left(i);
    }
}


class Cache {

    vector<vector<CacheBlock>> cache;
    vector<list<CacheBlock>> LRUList;
    vector<PLRUTree> cacheTrees;
    long numBlocks;
    long sets;
    long size;
    long ways;
    int replacementPolicy;
    long block;
    int blockOffset;
    int setOffset;
    set<pair<string, unsigned long>> seenAddresses;

protected:
    void LRURW(const string &tag, const unsigned long &set, bool &write);

    void PsuedoLRURW(const string &tag, const unsigned long &set, bool &write);

    void RandomRW(const string &tag, const unsigned long &set, bool &write);

    void DirectMappedRW(const string &tag, const unsigned long &set, bool &write);

    static string addressConversion(string address);

    int searchTagArray(unsigned int set, const string &tag);

public:
    Cache(long s, long b, long w, int r);

    void load(string &address, char rw);
};

Cache::Cache(long s, long b, long w, int r) {
    size = s; //init the size, block, ways and rep policy
    block = b;
    ways = w;
    replacementPolicy = r;
    numBlocks = size / block;
    if (ways == 0) {//fully associative cache
        //equivalent set and ways
        sets = 1;
        ways = numBlocks;
        //A different structure LRU list is used when policy is LRU
        if (ways >= 2 && replacementPolicy == 1) {
            LRUList.resize(1);
            LRUList[0].resize(numBlocks);
        } else {
            cache.resize(1);
            cache[0].resize(numBlocks);
        }

    } else {
        sets = numBlocks / ways;
        if (ways >= 2 && replacementPolicy == 1) {
            LRUList.resize(sets);
            for (int i = 0; i < sets; i++) {
                LRUList[i].resize(ways);
            }
        } else {
            cache.resize(sets);
            for (int i = 0; i < sets; i++) {
                cache[i].resize(ways);
            }
        }
    }

    blockOffset = 0; //calculating number of bits for block and set in the address
    for (int j = 2; j <= block; j *= 2)
        blockOffset++;
    setOffset = 0;
    for (int l = 2; l <= sets; l *= 2)
        setOffset++;

    if (ways >= 2 && replacementPolicy == 2) { //if replacement policy is Pseudo LRU
        int wayOffset = 0;
        for (int k = 2; k <= ways; k *= 2)
            ++wayOffset;//calculating the number of bits to represent associativity in binary
        cacheTrees.resize(sets, PLRUTree(ways, wayOffset)); //init the PseudoLRU trees for each set
    }
}

int Cache::searchTagArray(unsigned int set, const string &tag) {
    int res = -1; //if no tag is found, this is the value returned
    auto currSet = cache[set];

    for (int i = 0; i < ways; ++i) {
        if (currSet[i].valid == 1 && currSet[i].tag == tag) {//if the block is valid and the tag matches
            res = i;
            break;
        }
    }

    return res;
}

void Cache::LRURW(const string &tag, const unsigned long &set, bool &write) {
    list<CacheBlock>::iterator it;
    int v, d;
    for (it = LRUList[set].begin(); it != LRUList[set].end(); it++) {
        if (it->valid == 1 && it->tag == tag) {            // Cache hit
            v = it->valid;
            d = (write || it->dirty);
            LRUList[set].erase(it);
            break;
        }
    }
    if (it == LRUList[set].end()) {            // Element not in cache
        cacheMiss++;
        write ? writeMiss++ : readMiss++;
        if (seenAddresses.find({tag, set}) != seenAddresses.end()) {
            if (sets == 1)
                ++capMiss;
            else
                confMiss++;
        } else
            ++compMiss;
        it = LRUList[set].begin();
        while (it->valid == 1 && it != LRUList[set].end()) it++;
        if (it != LRUList[set].end() && it->valid == 0) {        // Found unassigned cache block
            LRUList[set].erase(it);
            v = 1;
            d = write;
        } else {                    // Cache is full
            CacheBlock temp = LRUList[set].back();
            if (temp.dirty) {            // Dirty block eviction
                dirtyEvict++;
            }
            LRUList[set].pop_back();        // Remove last element
            v = 1;
            d = write;
        }
    }
    LRUList[set].emplace_front(tag,v,d);
}

void Cache::PsuedoLRURW(const string &tag, const unsigned long &set, bool &write) {
    long hitInd = searchTagArray(set, tag), replaceInd;
    if (hitInd != -1) {//cache hit
        cacheTrees[set].read(hitInd);
        if (write)
            cache[set][hitInd].dirty = 1;//set bit to dirty
    } else {//miss
        ++cacheMiss;//it is a cache miss

        CacheBlock b; //replacing block
        b.tag = tag;
        b.valid = 1; //the replacing block is valid
        replaceInd = cacheTrees[set].replace();

        if (seenAddresses.find({tag, set}) != seenAddresses.end()) {
            //if the block was already on the cache and was evicted it is conflict miss
            //or numBlocks miss in fully associative
            if (sets == 1)
                ++capMiss;
            else
                confMiss++;
        } else
            ++compMiss;
        //else it is a compulsory miss

        if (cache[set][replaceInd].dirty == 1) //if the victim is a dirty block
            ++dirtyEvict;


        if (write) {
            b.dirty = 1; //if write, the block will be dirty after it is brought into cache and data written
            cache[set][replaceInd] = b;
            ++writeMiss;
        } else {
            b.dirty = 0;
            cache[set][replaceInd] = b;
            ++readMiss;
        }
    }


}

void Cache::RandomRW(const string &tag, const unsigned long &set, bool &write) {
    int present = searchTagArray(set, tag);
    if (present >= 0) { //  block found in cache
        if (write)
            cache[set][present].dirty = 1;

    } else {    // block not found in stack
        CacheBlock b;
        b.tag = tag;
        b.valid = 1;
        ++cacheMiss;
        if (write) {    //  checking if write miss or read miss
            b.dirty = 1;
            ++writeMiss;
        } else {
            b.dirty = 0;
            ++readMiss;
        }
        int vacant;
        for (vacant = 0; vacant < ways; vacant++)   //  checking for any vacant line before carrying out replacement
            if (cache[set][vacant].valid == 0)
                break;
        if (vacant < ways) {    //  if a vacant line is present, compulsory miss
            cache[set][vacant] = b;
            compMiss++;
        } else {    //  conflict/capacity miss
            vacant = rand() % ways; //  choosing block to evict randomly
            if (seenAddresses.find({tag, set}) != seenAddresses.end()) {
                //if the block was already on the cache and was evicted it is conflict miss
                //or numBlocks miss in fully associative
                if (sets == 1)
                    ++capMiss;
                else
                    confMiss++;
            } else
                ++compMiss;
            if (cache[set][vacant].dirty == 1)
                dirtyEvict++;
            cache[set][vacant] = b;
        }
    }
}

void Cache::DirectMappedRW(const string &tag, const unsigned long &set, bool &write) {
    CacheBlock b;
    b.tag = tag;
    b.valid = 1;
    int present = searchTagArray(set, tag); //  checking if block already present in respective set
    if (present == 0) { //  block present
        if (write)
            cache[set][0].dirty = 1;

    } else {    //  cache miss
        ++cacheMiss;
        if (seenAddresses.find({tag, set}) != seenAddresses.end()) {
            //if the block was already on the cache and was evicted it is conflict miss
            //or numBlocks miss in fully associative
            if (sets == 1)
                ++capMiss;
            else
                confMiss++;
        } else
            ++compMiss;

        if (cache[set][0].dirty == 1)
            ++dirtyEvict;

        if (write) {    //  check read or write miss
            b.dirty = 1;
            cache[set][0] = b;
            writeMiss++;
        } else {
            b.dirty = 0;
            cache[set][0] = b;
            readMiss++;
        }
    }
}

string Cache::addressConversion(string address) {
    string ba;
    int l = address.length();
    char ch;
    for (int i = 2; i < l; i++) {
        ch = address.at(i);
        switch (ch) {
            case '0':
                ba = ba + "0000";
                continue;
            case '1':
                ba = ba + "0001";
                continue;
            case '2':
                ba = ba + "0010";
                continue;
            case '3':
                ba = ba + "0011";
                continue;
            case '4':
                ba = ba + "0100";
                continue;
            case '5':
                ba = ba + "0101";
                continue;
            case '6':
                ba = ba + "0110";
                continue;
            case '7':
                ba = ba + "0111";
                continue;
            case '8':
                ba = ba + "1000";
                continue;
            case '9':
                ba = ba + "1001";
                continue;
            case 'a':
            case 'A':
                ba = ba + "1010";
                continue;
            case 'b':
            case 'B':
                ba = ba + "1011";
                continue;
            case 'c':
            case 'C':
                ba = ba + "1100";
                continue;
            case 'd':
            case 'D':
                ba = ba + "1101";
                continue;
            case 'e':
            case 'E':
                ba = ba + "1110";
                continue;
            case 'f':
            case 'F':
                ba = ba + "1111";
                continue;
        }
    }
    return ba;
}

void Cache::load(string &address, char rw) {
    string binaryAddress = addressConversion(address);
    string setStr = binaryAddress.substr(32 - blockOffset - setOffset, setOffset); //getting the set part
    bitset<32> setBitSet(setStr);

    unsigned long set = setBitSet.to_ulong();
    bool write = rw == 'w';
    string tag = binaryAddress.substr(0, 32 - blockOffset - setOffset); //getting the tag part


    //incrementing access related counters
    ++cacheRef;
    if (write)
        writeAcc++;
    else
        readAcc++;

    if (ways == 1) {//if direct mapped cache
        DirectMappedRW(tag, set, write);
    } else if (replacementPolicy == 0) { //if random policy
        RandomRW(tag, set, write);
    } else if (replacementPolicy == 1) { //if LRU policy
        LRURW(tag, set, write);
    } else {//if pseudo-LRU policy
        PsuedoLRURW(tag, set, write);
    }
    seenAddresses.emplace(tag, set); //adding to the set of seen blocks
}


int main() {
    cacheRef = cacheMiss = readAcc = writeAcc = compMiss = capMiss = confMiss = readMiss = writeMiss = dirtyEvict = 0;
    long cacheSize, cacheLine;
    int ways, repPolicy;
    string inp;
    cin >> cacheSize >> cacheLine >> ways >> repPolicy; //getting input for cache structure

    Cache c(cacheSize, cacheLine, ways, repPolicy);//init the cache object

    cin >> inp;
    ifstream addressFile(inp); //opening file with addresses
    char rw;
    addressFile >> inp;
    addressFile >> rw;
    while (!addressFile.eof()) {//reading the addresses from file
        c.load(inp, rw);
        addressFile >> inp;
        addressFile >> rw;
    }
    addressFile.close();

    cout << cacheSize << endl;
    cout << cacheLine << endl;
    switch (ways) {
        case 0:
            cout << "Fully-associative cache" << endl;
            break;
        case 1:
            cout << "Direct-mapped cache" << endl;
            break;
        default:
            cout << "Set-associative cache" << endl;
            break;
    }
    switch (repPolicy) {
        case 0:
            cout << "Random Replacement" << endl;
            break;
        case 1:
            cout << "LRU Replacement" << endl;
            break;
        case 2:
            cout << "Pseudo-LRU Replacement" << endl;
            break;
        default:
            break;
    }
    cout << "Cache Accesses :"<< cacheRef << endl;
    cout << "Read Accesses :"<< readAcc << endl;
    cout << "Write Accesses :"<< writeAcc << endl;
    cout << "Cache Misses :"<< cacheMiss << endl;
    cout << "Compulsory Misses :"<< compMiss << endl;
    cout << "Capacity Misses :"<< capMiss << endl;
    cout << "Conflict Misses :"<< confMiss << endl;
    cout << "Read Misses :"<< readMiss << endl;
    cout << "Write Misses :"<< writeMiss << endl;
    cout << "Dirty Blocks Evicted :"<< dirtyEvict << endl;


    return 0;
}
