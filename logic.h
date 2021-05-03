#ifndef LOGIC_H
#define LOGIC_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class GroupItem{
private:
    string name;
    vector<GroupItem> children;
    int childrenCount;

public:
    GroupItem(){
        name = "";
        children.clear();
        childrenCount = 0;
    }

    GroupItem(string title){
        name = title;
        children.clear();
        childrenCount = 0;
    }

    void addItem(GroupItem newItem){
        children.push_back(newItem);
        childrenCount++;
    }

    void setName(string title){
        name = title;
    }

    int getChildrenCount(){
        return childrenCount;
    }
};

class JsonData{
private:
    vector<GroupItem> rootGroupItem;

public:
    JsonData(){};
    JsonData(string path){
        /* Считывает данные из файла и записывает */
        ifstream myFile(path);
        if (!myFile.is_open()) throw runtime_error("Could not open file");

        string word = "";
        string line = "";
        string all_text = "";
        bool isChild = false,  isRoot = true;
        GroupItem rootItem;

        while (getline(myFile, line)){
            all_text += line;
        }

        for (int i = 0; i < (int)all_text.length(); ++i){
            char symbol = all_text[i];
            if (symbol == ' ' || symbol == '{'){
                continue;
            }
            if (symbol == ',' || symbol == '}'){
                if (isChild){
                    rootItem.addItem(GroupItem(word));
                    isChild = false;
                }

                if (isRoot){
                    rootGroupItem.push_back(rootItem);
                }
                word = "";
                continue;
            }
            if (symbol == ':'){
                if (!isChild){
                    rootItem = GroupItem(word);
                }
                word = "";
                isChild = true;
                continue;
            }
            word += symbol;
        }
        line = "";
    }
};

#endif // LOGIC_H
