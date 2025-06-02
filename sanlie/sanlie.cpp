#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// 移除单词中的标点符号
string cleanWord(const string& word) {
    string cleaned;
    for (char c : word) {
        if (isalpha(c)) {
            cleaned += tolower(c);  // 转换为小写
        }
    }
    return cleaned;
}

// 统计词频
unordered_map<string, int> countWordFrequency(const string& filename) {
    unordered_map<string, int> wordCount;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return wordCount;
    }

    string word;
    while (file >> word) {
        string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            wordCount[cleaned]++;
        }
    }

    file.close();
    return wordCount;
}

// 查询单词词频
void queryWord(const unordered_map<string, int>& wordCount) {
    string word;
    cout << "请输入要查询的单词: ";
    cin >> word;

    string cleaned = cleanWord(word);
    auto it = wordCount.find(cleaned);

    if (it != wordCount.end()) {
        cout << "单词 '" << cleaned << "' 出现次数: " << it->second << endl;
    } else {
        cout << "单词 '" << cleaned << "' 未找到！" << endl;
    }
}

int main() {
    // 输入文件路径
    string inputFilename = "../Input/InFile.txt";
    // 输出文件路径
    string outputFilename = "../Output/OutFile4.txt";

    // 统计词频
    unordered_map<string, int> wordCount = countWordFrequency(inputFilename);

    // 将词频统计结果存储到向量中并排序
    vector<pair<string, int>> sortedWords(wordCount.begin(), wordCount.end());
    sort(sortedWords.begin(), sortedWords.end());

    // 打开输出文件
    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "无法打开输出文件: " << outputFilename << endl;
        return 1;
    }

    // 写入词频统计结果到文件
    outFile << "===== 词频统计 =====" << endl;
    for (const auto& entry : sortedWords) {
        outFile << entry.first << ": " << entry.second << endl;
    }

    // 关闭输出文件
    outFile.close();

    // 查询单词
    queryWord(wordCount);

    return 0;
}

