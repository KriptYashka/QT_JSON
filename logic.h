#ifndef LOGIC_H
#define LOGIC_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
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

    void addChild(GroupItem newItem){ // Функция удочерения
        children.push_back(newItem);
        childrenCount++;
    }

    void addChild(vector<GroupItem> newVectorChilds){ // Функция удочерения близняшек ( перегрузка )
        for (GroupItem child : newVectorChilds){
            children.push_back(child);
            childrenCount++;
        }
    }

    void setName(string title){
        name = title;
    }

    string getName(){
        return name;
    }

    bool hasChildren(){
        return childrenCount != 0;
    }

    int getChildrenCount(){
        return childrenCount;
    }

    GroupItem getChild(int index){
        return children.at(index);
    }

    void clear(){
        name = "";
        children.clear();
        childrenCount = 0;
    }

    bool isEmpty(){
        if (name == "" && childrenCount == 0){
            return true;
        }
            return false;
    }
};

class JsonData{
private:
    /* Поля */
    bool hasError;
    /* Методы */
    vector<GroupItem> rootGroupItems;
    vector<GroupItem> getChildren(string text){
        /* Сначала получаем нужный текст */
        string reqText = "";
        int count = 1, index = 1;
        while(count > 0){
            char symbol = text[index++];
            if (symbol == '{'){
                count++;
            } else if (symbol == '}'){
                count--;
                if (count == 0){
                    continue;
                }
            }
            if (symbol == ' ' || symbol == '\0' || symbol == '\n'){
                continue;
            }
            reqText += symbol;
        }
        /* Затем заполняем вектор */
        vector<GroupItem> groupRoot;
        string word = "";
        int i = 0; // Чтобы не трогать первую скобку
        GroupItem item;
        while (i < (int)reqText.length()){
            char symbol = reqText[i++]; // Внимание! Сразу увеличиваем индекс.
            if (symbol == ':'){
                item = GroupItem(word);
                word = "";
                continue;
            }
            if (symbol == ','){ // должны добавить в корневой вектор, если это обычный элемент
                if (item.isEmpty())
                    continue;
                GroupItem child = GroupItem(word);
                item.addChild(child);
                groupRoot.push_back(item);
                item.clear();
                word = "";
                continue;
            }
            if (symbol == '{'){
                int end_i = reqText.find('}');
                string childText = reqText.substr(i - 1, end_i - i + 2);
                vector<GroupItem> children = getChildren(childText);

                item.addChild(children);
                groupRoot.push_back(item);
                item.clear();
                i = end_i; // Переносимся под конец пройденного текста
                continue;
            }
            if (symbol == '}'){
                continue;
            }
            word += symbol;
        }
        if (word != ""){
            item.addChild(word);
            groupRoot.push_back(item);
        }
        return groupRoot;
    }

public:
    JsonData(string path){
        /* Считывает данные из файла и записывает */
        hasError = true;
        ifstream myFile(path);
        if (!myFile.is_open()){
            return;
        }
        hasError = false;

        string line = "";
        string all_text = "";
        while (getline(myFile, line)){
            all_text += line;
        }
        rootGroupItems = getChildren(all_text);
    }

    vector<GroupItem> getData(){
        vector<GroupItem> data;
        if (hasError)
            return data;
        return rootGroupItems;
    }

    int rootSize(){
        return rootGroupItems.size();
    }

    GroupItem at(int row){
        GroupItem data;
        if (hasError || (row < 0 || row >= (int)rootGroupItems.size()))
            return data;
        return rootGroupItems.at(row);
    }

    bool isError(){
        return hasError;
    }
};

#endif // LOGIC_H
