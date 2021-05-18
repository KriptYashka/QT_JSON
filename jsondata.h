#ifndef JSONDATA_H
#define JSONDATA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

#include "groupitem.h"
using namespace std;

class JsonData{
private:
    /* Поля */
    int cod;
    /*
    Коды программы:
    200 - все отлично
    400 - файл не открывается или не json формат
    401 - неверное количество фигурных или квадратных скобок
    402 - json-файл содержит запрещенный символ
    403 - не закрыты кавычки
    */
    string codText;
    string jsonText;
    vector<GroupItem> rootGroupItems;


    /* Методы */
    string getInnerText(string text){
        /* Передается текст, внутри {}. Обрабатывается от лишних знаков. */
        string reqText = "";
        int count = 1, index = 1;
        while (count > 0){
            char symbol = text[index++];
            if (symbol == ' ' || symbol == '\0' || symbol == '\n'){
                continue;
            }
            if (symbol == '{'){
                count++;
            } else if (symbol == '}'){
                count--;
                if (count == 0){
                    continue;
                }
            }
            reqText += symbol;
        }
        return reqText;
    }

    int bracketPosition(string reqText, int index){
        int count = 1;
        while (count > 0){
            char c = reqText[index++];
            if (c == '{'){
                count++;
            } else if (c == '}'){
                count--;
            }
        }
        return index;
    }

    vector<GroupItem> getChildren(string text){
        /* Сначала получаем нужный текст */
        string reqText = getInnerText(text);
        /* Затем заполняем вектор */
        vector<GroupItem> groupRoot;

        GroupItem item; // временный элемент

        string word = "";
        bool flag_arr = false;
        int i = 0;
        while (i < (int)reqText.length()){
            char symbol = reqText[i++]; // Внимание! Сразу увеличиваем индекс.
            if (symbol == ':'){
                item = GroupItem(word);
                word = "";
                continue;
            }
            if (symbol == ',' || symbol == ']'){ // должны добавить в корневой вектор, если это обычный элемент
                if (item.isEmpty())
                    continue;
                GroupItem child = GroupItem(word);
                item.addChild(child);
                word = "";
                if (symbol == ']'){
                    flag_arr = false;
                }
                if (!flag_arr){
                    groupRoot.push_back(item);
                    item.clear();
                }
                continue;
            }
            if (symbol == '{'){
                int end_i = bracketPosition(reqText, i);
                string childText = reqText.substr(i - 1, end_i - i + 2);
                vector<GroupItem> children = getChildren(childText); // рекурсия

                item.addChild(children);
                if (!flag_arr){
                    groupRoot.push_back(item);
                    item.clear();
                }
                i = end_i; // Переносимся под конец пройденного текста
                continue;
            }
            if (symbol == '}'){
                continue;
            }
            if (symbol == '['){
                flag_arr = true;
                continue;
            }
            word += symbol;
        }
        /* В случае, если нет запятой в конце текста */
        if (word != ""){
            item.addChild(word);
            groupRoot.push_back(item);
        }
        return groupRoot;
    }

    bool isJSONFile(string path){
        bool flag = true;
        if ((int)path.find(".json") == -1)
            flag = false;
        return flag;
    }

    int correctData(string text){
        /* Проверяет, что файл не сломан. Возвращает код ошибки. */
        bool isMark_1 = false, isMark_2 = false; // Одинарные и двойные кавычки
        int cod = 200;
        int count_figure = 0, count_square = 0;
        string forbidden_symbols = "+-*/|_\()<>;%^&!~";
        for (char symbol : text){
            if (count_figure < 0 || count_square < 0){
                cod = 401;
                break;
            }
            if (!isMark_1 && !isMark_2 && (int)forbidden_symbols.find(symbol) != -1){
                cod = 402;
                break;
            }
            switch (symbol) {
            case '{':
                if (!isMark_2 || !isMark_1)
                    count_figure++;
                break;
            case '}':
                if (!isMark_2 || !isMark_1)
                    count_figure--;
                break;
            case '[':
                if (!isMark_2 || !isMark_1)
                    count_square++;
                break;
            case ']':
                if (!isMark_2 || !isMark_1)
                    count_square--;
                break;
            case '"':
                if (!isMark_1)
                    isMark_2 = !isMark_2;
                break;
            case '\'':
                if (!isMark_2)
                    isMark_1 = !isMark_1;
                break;
            }
        }
        if ((count_figure > 0 || count_square > 0) && cod == 200)
            cod = 401;
        if ((isMark_1 || isMark_2) && cod == 200)
            cod = 403;
        return cod;
    }

public:
    JsonData(string path){
        /* Конструктор. Считывает данные из файла и записывает */
        cod = 400;
        ifstream myFile(path);
        if (!myFile.is_open() && isJSONFile(path)){
            return;
        }
        string line = "";
        jsonText = "";
        while (getline(myFile, line)){
            jsonText += line + '\n';
        }
        cod = correctData(jsonText);
        if (cod == 200)
            rootGroupItems = getChildren(jsonText);
    }

    vector<GroupItem> getData(){
        vector<GroupItem> empty_data;
        if (cod != 200)
            return empty_data;
        return rootGroupItems;
    }

    string getJsonText(){
        return jsonText;
    }

    int rootSize(){
        return rootGroupItems.size();
    }

    GroupItem at(int row){
        GroupItem empty_data;
        if (cod != 200 || (row < 0 || row >= (int)rootGroupItems.size()))
            return empty_data;
        return rootGroupItems.at(row);
    }

    bool isError(){
        return cod != 200;
    }

    int getCod(){
        return cod;
    }

    string getCodeText(){
        string text;
        switch (cod) {
        case 200:
            text = "JSON загружен.";
            break;
        case 400:
            text = "Файл невозможно открыть";
            break;
        case 401:
            text = "JSON содержит неверное количество фигурных или квадратных скобок.";
            break;
        case 402:
            text = "JSON содержит запрещенные символы.";
            break;
        case 403:
            text = "Внутри JSON открыта кавычка.";
            break;
        default:
            text = "Неизвестная ошибка";
            break;
        }
        return text;
    }
};

#endif // JSONDATA_H
