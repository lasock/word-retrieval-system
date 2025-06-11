#include<iostream>
#include<cstring>
#include<cctype>
#include<fstream>
#include<chrono>

using namespace std;

#define WORD_MAX_SIZE 20
#define MAX_WORDS 5500

void removePunctuation(char* word);

struct Word {
    char word[WORD_MAX_SIZE];
    int frequency;
};

class SequentialList {
private:
    Word* words;
    int size;
    int capacity;

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

public:
    SequentialList(int cap = MAX_WORDS) : size(0), capacity(cap) {
        words = new Word[capacity];
    }

    ~SequentialList() {
        delete[] words;
    }

    // 交换单词位置
    void swapWord(Word& a, Word& b) {
        Word temp = a;
        a = b;
        b = temp;
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

    // 从文件中读取单词并按照字典序排序
    void processFile(const char* filename) {
        ifstream infile(filename);
        if (!infile) {
            cerr << "无法打开文件！" << endl;
            return;
        }
        char newWord[WORD_MAX_SIZE];
        while(infile >> newWord) {
            removePunctuation(newWord);
            if(strlen(newWord) == 0) continue; // 跳过空单词
            addWord(newWord);
        }
        infile.close();
        quickSort(0, size - 1);
    }

    // 顺序查找单词
    void searchWord() {
        int totalSearches = 0;
        int totalAsl = 0;
        char word[WORD_MAX_SIZE];
        while (true){
            cout << "请输入你要查询的单词(输入0结束查询):" ;
            cin >> word;
            cout << endl;
            if (!strcmp(word, "0")) break;
            for (int i = 0; word[i]; i++){
                word[i] =  tolower(word[i]);
            }
            int asl = 0;
            auto start = chrono::high_resolution_clock::now();
            for(int i = 0; i < size; i++) {
                asl++;
                if(strcmp(words[i].word, word) == 0) {
                    auto end = chrono::high_resolution_clock::now();
                    totalAsl += asl;
                    totalSearches++;
                    cout << words[i].word << " 出现的次数为: " << words[i].frequency << endl
                         << "查找次数: " << asl << endl
                         << "查找时间: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << "μs" << endl;
                    break;
                } else if(strcmp(words[i].word, word) > 0) {
                    cout << "未查询到 " << word << endl;
                    break;
                }
            }
        }
        cout << "本次总查询的平均查找长度: "  << totalAsl * 1.0 / totalSearches << endl; 
    }
};

// 判断是否为单词字符串
bool isWordchar(char c){
	if (isalpha(c) || c=='-'){
		return true;
	}
	return false;
}

// 去除单词中的标点符号并全部转换为小写
void removePunctuation(char* word) {
    if (!isalpha(word[0])){
		int startPos = 0;
		while (word[startPos] != '\0' && !isalpha(word[startPos])){
			startPos++;
		}
		int k = 0;
		while (word[startPos] != '\0'){
			word[k++] = word[startPos++];
		}
		word[k] = '\0';
	}
    int i = 0, l = 0;
    while(word[l] != '\0') {
        if(isWordchar(word[l])) {
            word[i++] = tolower(word[l]);
        } 
        l++;
    }
    word[i] = '\0';
}

int main()
{
    SequentialList sl;
    const char* filename = "../Input/InFile.txt";
    sl.processFile(filename);
    const char* outFilename = "../Output/OutFile1.txt";
    sl.writeToFile(outFilename);
    sl.searchWord();
    return 0;
}