#include <iostream>
#include <string>
#include <limits>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#define emptyText ""
#define listFile "infotext.dat"
using namespace std;
struct Node{
    char ch;
    int freq;
    Node *x, *y;
};
typedef struct{
    char copyFile[200];
    char saveFile[200];
} rec;

Node* pnode(char ch,int freq, Node* x, Node* y){
    Node* node = new Node();
    node->ch = ch;
    node->freq = freq;
    node->x = x;
    node->y = y;
    return node;
}
struct pass{
    bool operator()(const Node* a, const Node* b) const
    {
        return a->freq > b->freq;
    }
};

bool isLeaf(Node* root) {
    return root->x == nullptr && root->y == nullptr;
}

void Encode(Node* root, string str, unordered_map<char, string> &huffCode);
void Decode(Node* root, int &index, string str, string &decstr);
void Compress();
void Decompress();

int main(){
    int choice;
    int loop = 1;
    while(loop == 1){
    cout <<"HUFFMAN COMPRESSION AND DECOMPRESSION" << endl;
    cout << "1. COMPRESS FILE" << endl;
    cout << "2. DECOMPRESS FILE" << endl;
    cout << "3. EXIT " << endl;
    cout << "CHOOSE 1-3: ";
    cin >> choice;
    switch(choice){
        case 1:
            Compress();
            system("cls");
            break;
        case 2:
            Decompress();
            system("cls");
            break;
        case 3:
            loop = 0;
            break;
        default:
            cout << "Invalid, try again..." << endl;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            fflush(stdin);
            system("pause");
            break;
    }
    }
    return 0;
}

void Encode(Node* root, string str, unordered_map<char, string> &huffCode){
    if (root == nullptr) {
        return;
    }

    if (isLeaf(root)) {
        huffCode[root->ch] = (str != emptyText) ? str : "1";
    }
    Encode(root->x, str + "0", huffCode);
    Encode(root->y, str + "1", huffCode);
}
void Decode(Node* root, int &index, string str, string &decstr){                          if (root == nullptr) {
        return;
    }
    if (isLeaf(root))
    {
        decstr += root->ch;
        return;
    }

    index++;

    if (str[index] == '0') {
        Decode(root->x, index, str, decstr);
    }
    else {
        Decode(root->y, index, str, decstr);
    }
}

void Compress(){
    rec fSave = {};
    system("cls");
    fstream fp1, fp2, fp3;
    char oldfile[100], Newfile[100];
    int orgStringcount;
    string text, textInput, str;
cout << "--COMPRESSING FILE--" << endl;
cout << "Example:Data Algo.txt\n";
cout << "Enter filename to compress: ";
cin >> oldfile;
    fp1.open(oldfile, ios::in);
    strcpy(fSave.copyFile, oldfile);
    cout << "Press a new .txt to compressed file: ";
    cin >> Newfile;
    strcpy(fSave.saveFile, Newfile);

    fp2.open(Newfile, ios::out);
    fp3.open(listFile, ios::binary | ios::app);
    while(!fp1.eof()){
        getline(fp1, textInput);
        text += textInput + "\n";
    }
    if (text == emptyText) {
        return;
    }
    orgStringcount = text.size();
    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }
    priority_queue<Node*, vector<Node*>, pass> pq;
    for (auto pair: freq) {
        pq.push(pnode(pair.first, pair.second, nullptr, nullptr));
    }
    while (pq.size() != 1){
        Node* x = pq.top(); pq.pop();
        Node* y = pq.top(); pq.pop();
        int sum = x->freq + y->freq;
        pq.push(pnode('\0', sum, x, y));
    }
    Node* root = pq.top();
    unordered_map<char, string> huffCode;
    Encode(root, emptyText, huffCode);
     for (char ch: text) {
        str += huffCode[ch];
    }
    cout << "\t\nHUFFMAN COMPRESSION" << endl;
    cout << "CHARACTERS\tTHE HUFFMAN CODE\n" << endl;
    for(auto pair: huffCode)
    {cout <<  pair.first << "\t\t->" << pair.second << endl;}
    cout << "\nFILE SIZE IN BYTES: ";
    cout << orgStringcount * 8;
    cout << "\nCOMPRESS FILE IN BYTES: ";
    cout << str.size();

    fp2 << str;
    fp3.write((char*)&fSave, sizeof(fSave));
    cout << "\n\nCOMPRESS COMPLETE!\n" << endl;
    fp2.close();
    fp3.close();
    system("pause");
    fflush(stdin);
}

void Decompress(){
    rec efile = {};
    system("cls");
    fstream fp1, fp2, fp3;
    ifstream fp4;
    char fil1[100], fil2[100];
    string decFilename, encStr, text, textInput, decStr;


cout << "-DECOMPRESSING THE FILE-" << endl;
cout << "Example: Data Algo.txt" << endl;
cout << "Enter filename to decompressed: ";cin >> fil1;

    fp4.open(listFile,ios::binary);

    while (fp4.read((char*)&efile, sizeof(efile))){
        if(strcmp(efile.saveFile, fil1) == 0)
        {
            strcpy(fil2, efile.copyFile);
        }
    }

    fp1.open(fil1, ios::in);
    fp2.open(fil2, ios::in);

    while(!fp2.eof()){
        getline(fp2, textInput);
        text += textInput + "\n";
    }
    if (text == emptyText) {
        return;
    }

    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }
    priority_queue<Node*, vector<Node*>, pass> pq;
    for (auto pair: freq) {
        pq.push(pnode(pair.first, pair.second, nullptr, nullptr));
    }
    while (pq.size() != 1){
        Node* x = pq.top(); pq.pop();
        Node* y = pq.top();    pq.pop();
        int sum = x->freq + y->freq;
        pq.push(pnode('\0', sum, x, y));
    }
    Node* root = pq.top();
    unordered_map<char, string> huffCode;
    getline(fp1, encStr);
    if (isLeaf(root)){
        while (root->freq--) {
            decStr += root->ch;
            decStr += root->ch;
        }
    }
    else {
        int index = -1;
        while (index < (int)encStr.size() - 1) {
            Decode(root, index, encStr, decStr);
        }
    }
    cout << "Press a new .txt to decompressed file: ";
    cin >>decFilename;
    cout << "Converted Text:" << decStr;
    fp3.open(decFilename, ios::out);
    fp3 << decStr;
    fp3.close();
    cout << "\nDECOMPRESS COMPLETE!\n" << endl;
    system("pause");
    fflush(stdin);
}

