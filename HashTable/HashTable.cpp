#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>

using namespace std;

const int HASH_TABLE_SIZE = 5003;
const int MAX_WORD_LEN = 50;
const int MAX_WORDS = 5500;

struct Word {
    char word[MAX_WORD_LEN];
    int frequency;
};

class SequentialList {
private:
    Word* words;
    int size;
    int capacity;
    
public:
    // 交换单词位置
    void swapWord(Word& a, Word& b) {
        Word temp = a;
        a = b;
        b = temp;
    }
    // 快速排序
    void quickSort(int low, int high) {
        if(low >= high) return;
        int l = low - 1, r = high + 1;
        Word pivotVal = words[(low + high) >> 1];
        while(l < r) {
            do l++; while(strcmp(words[l].word, pivotVal.word) < 0);
            do r--; while(strcmp(words[r].word, pivotVal.word) > 0);
            if(l < r) swapWord(words[l], words[r]);
        } 
        quickSort(low, r);
        quickSort(r + 1, high);
    }


    SequentialList(int cap = MAX_WORDS) : size(0), capacity(cap) {
        words = new Word[capacity];
    }

    ~SequentialList() {
        delete[] words;
    }

    const char* findWord(int idx) {
        if (idx < 0 || idx >= size) {
            return nullptr;
        }
        return words[idx].word;
    }

        // 添加单词到顺序表
    void addWord(const char* newWord) {
        if(size == capacity) {
            cout << "顺序表已满，无法添加！！" << endl;
            return;
        }
        for(int i = 0; i < size; i++) {
            if(strcmp(words[i].word, newWord) == 0) {
                words[i].frequency++;
                return;
            }
        }
        strcpy(words[size].word, newWord);
        words[size++].frequency = 1;
    }
    
    // 将单词按照字典序输出到文件中
    void writeToFile(const char* filename) {
        quickSort(0, size - 1);
        ofstream outfile(filename);
        if (!outfile) {
            cerr << "无法打开输出文件！" << endl;
            return;
        }
        outfile << "===== 词频统计 =====" << endl;
        for(int i = 0; i < size; i++) {
            outfile << words[i].word << ": " << words[i].frequency << endl;
        }
    }
    
};

struct HashNode{
    char word[MAX_WORD_LEN];
    int frequency;
    HashNode* next;
    
    HashNode(const char* w) : frequency(1), next(nullptr){
        strcpy(word, w);
    }
};

class HashTable{
private:
    HashNode* table[HASH_TABLE_SIZE];
    // 顺序表
    SequentialList wordList;
    
    // 自定义哈希函数
    unsigned int hashFunction(const char* w){
        unsigned int hash = 0;
        for (; *w; ++w){
            hash = (hash * 31) + tolower(*w);
        }
        return hash % HASH_TABLE_SIZE;
    }
    
    bool isWordchar(char w){
        return isalpha(w) || (w == '-') || (w == '\'');
    }
    
    void cleanWord(char* word){
        char *p = word;
        char *q = word;
        
        while (*p && !isalpha(*p)){
            p++;
        }

        while (*p){
            if (isWordchar(*p)){
                *q++ = tolower(*p);
            }
            p++;
        }
        *q = '\0';
        
    }

public:
    HashTable(){
        for (int i = 0; i < HASH_TABLE_SIZE; ++i){
            table[i] = nullptr;
        }
    }
    
    ~HashTable(){
        for (int i = 0; i < HASH_TABLE_SIZE; ++i){
            HashNode* node = table[i];
            while (node){
                HashNode* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }
    
    void insert(const char* word){
        char cleaned[MAX_WORD_LEN];
        strncpy(cleaned, word, MAX_WORD_LEN-1);
        cleaned[MAX_WORD_LEN-1] = '\0';
        cleanWord(cleaned);
        
        if (strlen(cleaned) == 0) return;
        // 添加到顺序表中
        wordList.addWord(cleaned);
        // 添加到哈希表中
        unsigned int hash = hashFunction(cleaned);
        HashNode* node = table[hash];
        HashNode* prev = nullptr;
        
        while (node){
            if (strcmp(node->word, cleaned) == 0){
                node->frequency++;
                return;
            }
            prev = node;
            node = node->next;
        }
        // 单词不存在，新建节点
        HashNode* newNode = new HashNode(cleaned);
        if (prev){
            prev->next = newNode;
        }else {
            table[hash] = newNode;
        }
    }
    
    int asl = 1;
    
    int search(const char* word){
        asl = 1;
        char cleaned[MAX_WORD_LEN];
        strncpy(cleaned, word, MAX_WORD_LEN-1);
        cleaned[MAX_WORD_LEN-1] = '\0';
        cleanWord(cleaned);
        
        unsigned int hash = hashFunction(cleaned);
        HashNode* node = table[hash];
        while (node){
            if (strcmp(cleaned, node->word) == 0){
                return node->frequency;
            }
            asl++;
            node = node->next;
        }
        return 0;
    }
    
    void writeToFile(const char* filename) {
        wordList.writeToFile(filename);
    }
    
};


void processFile(HashTable& hashTable, const char* filename){
	ifstream infile(filename);
	if (!infile.is_open()){
		cerr << "无法打开文件"  << filename << endl;
		return;
	}
	
	char word[MAX_WORD_LEN];
    while (infile >> word){
        hashTable.insert(word);
    }
    
    infile.close();
}


int main(){
    HashTable wordCounts;
    const char* inFile = "../Input/InFile.txt";
    const char* outFile = "../Output/OutFile4.txt";
    
    processFile(wordCounts, inFile);
    wordCounts.writeToFile(outFile);
    
	int totalSearches = 0;
	int totalAsl = 0;
    
    while (true){
        cout << "请输入你要查询的单词(输入0结束查询):" ;
        char searchTerm[MAX_WORD_LEN];
        cin >> searchTerm;
        
        if (strcmp(searchTerm, "0") == 0){
            break;
        }
        // 计时
        auto start = chrono::high_resolution_clock::now();
        
        int frequency = wordCounts.search(searchTerm);
        if (frequency){
            auto end = chrono::high_resolution_clock::now();
            totalAsl += wordCounts.asl;
            totalSearches++;
            cout << searchTerm << "出现的次数为:" << frequency << endl
                 << "查找次数: " << wordCounts.asl << endl
                 << "查找时间: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
        }else {
            cout << "未查询到" << searchTerm << endl;
        }
        cout << endl;
    }
    cout << "本次总查询的平均查找长度: "  << totalAsl * 1.0 / totalSearches << endl; 
    
    return 0;
}
