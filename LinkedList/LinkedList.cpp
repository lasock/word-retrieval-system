#include <iostream>
 #include <fstream>
#include <cstring>
// #include <cctype>
// #include <ctime>

#define MAX_WORD_LEN 50
using namespace std;

// 节点
typedef struct ListNode{
    char word[MAX_WORD_LEN];
    int frequency;
    ListNode* next;
}WordNode;

// 初始化
WordNode* initList(){
    WordNode* head = (WordNode*)malloc(sizeof(WordNode));
    head->word[0] = '\0';
    head->frequency = 0;
    head->next = nullptr;
    return head;
}

// 创建新节点
WordNode* createNode(const char* word){
    WordNode* newnode = (WordNode*)malloc(sizeof(WordNode));
    strcpy(newnode->word, word);
    newnode->frequency = 1;
    newnode->next = nullptr;
    return newnode;
}

// 在链表中插入或更新单词
void insertWord(WordNode* head, const char* word){
    if (head->next == nullptr){
        head->next = createNode(word);
        return;
    }
    
    WordNode* current = head->next;
    WordNode* prev = nullptr;
    while (current != nullptr){
        int cmp = strcmp(current->word, word);
        if (cmp == 0){
            current->frequency++;
            return;
        }
        else if (cmp > 0 ){
            // 在prev和curr前插入
            break;
        }
        prev = current;
        current = current->next;
    }

    // 新建节点
    WordNode* newnode = createNode(word);
    // 处理prev=null，插入第一位
    if (prev == nullptr){
        head->next = newnode;
        newnode->next = current;
    }
    else {
        prev->next = newnode;
        newnode->next = current;
    }

}

//判断是否为单词字符串
bool isWordchar(char c){
	if (isalpha(c) || c == '\'' || c=='-'){
		return true;
	}
	return false;
}

//提取函数，用于循环
void extractWord(const char* input, char* word){
	int i = 0, j = 0;
	while (input[i] != '\0'){
		if (isWordchar(input[i])){
			word[j++] = tolower(input[i]);
		}
		i++;
	}
	word[j] = '\0';
//	处理非字母开头的情况
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
}

//输入文件操作
void processFile(WordNode* list, const char* filename){
	ifstream infile(filename);
	if (!infile.is_open()){
		cerr << "无法打开文件"  << filename << endl;
		return;
	}
	
	char buffer[MAX_WORD_LEN];
	char word[MAX_WORD_LEN];
	
	while (infile >> buffer){
		extractWord(buffer, word);
		if (strlen(word)>0){
			insertWord(list, word);
		}
	}
	infile.close();
}

//输出到OutFile2里
void writeToFile(WordNode* list, const char* filename){
	ofstream outfile(filename);
	if (!outfile.is_open()){
		cerr << "无法打开文件" << filename << endl;
		return;
	}
	
	outfile << "===== 词频统计 =====" << endl;
	WordNode* node = list->next;
	while (node != nullptr){
		outfile << node->word << ": " << node->frequency << endl;
		node = node->next;
	}

	outfile.close();
}

//查询单词
void searchWord(WordNode* list){
	char word[MAX_WORD_LEN];
	while (true){
		cout << "请输入你要查询的单词(输入0结束查询):" ;
		cin >> word;
		cout << endl;
		if (!strcmp(word, "0")){
			return;
		}
//		转小写
		for (int i = 0; word[i]; i++){
			word[i] =  tolower(word[i]);
		}
		
		WordNode* curr = list->next;

		while (curr != nullptr){
			int cmp = strcmp(word, curr->word);
			if (cmp == 0){
				cout << curr->word << "出现的次数为: " << curr->frequency << endl;
				break;
			}else if(cmp < 0){
				cout << "未查询到" << word << endl;
				break;
			}
			curr = curr->next;
		}
	}
}

// 遍历
void coutList(WordNode* list){
    cout << "当前链表为：" << endl ;
    WordNode* node = list->next;
    while (node != nullptr){
        cout << node->word << ": " << node->frequency << endl;
        node = node->next;
    }
}

// 释放内存
void freeList(WordNode* list){
    WordNode* curr = list;
    while (curr != nullptr){
        WordNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

// 测试
void test(WordNode* list){
    coutList(list);
    char inputWord[MAX_WORD_LEN];
    while (true){
        cout << "请输入要插入的词(输入0结束)：";
        cin >> inputWord;
        if (strcmp(inputWord, "0") == 0){
            coutList(list);
            break;
        }
        insertWord(list, inputWord);
        coutList(list);
    }
}


int main(){
    WordNode* list = initList();
//    test(list);
	const char* ifname = "../Input/InFile.txt";
	const char* ofname = "../Output/OutFile2.txt";
	processFile(list, ifname);
	writeToFile(list, ofname);
	searchWord(list);
	
    freeList(list);
    return 0;
}
