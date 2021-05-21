#ifndef ERRORCONTROLLER_H
#define ERRORCONTROLLER_H
#include <string>

using namespace std;

class ErrorController{
private:
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

    void setCodText(){
        switch (cod) {
        case 200:
            codText = "JSON загружен.";
            break;
        case 400:
            codText = "Файл невозможно открыть";
            break;
        case 401:
            codText = "JSON содержит неверное количество фигурных или квадратных скобок.";
            break;
        case 402:
            codText = "JSON содержит запрещенные символы.";
            break;
        case 403:
            codText = "Внутри JSON открыта кавычка.";
            break;
        default:
            codText = "Неизвестная ошибка";
            break;
        }
    }

    void correctData(string text){
        /* Проверяет, что файл не сломан. Возвращает код ошибки. */
        bool isMark_1 = false, isMark_2 = false; // Одинарные и двойные кавычки
        cod = 200;
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
    }


public:
    ErrorController(){
        cod = 400;
        setCodText();
    }

    ErrorController(string text){
        jsonText = text;
        correctData(text);
        setCodText();
    }

    void setCod(int newCod){
        cod = newCod;
        setCodText();
    }

    string getCodText(){
        return codText;
    }

    int getCod(){
        return cod;
    }

    bool isOk(){
        return cod == 200;
    }
};

#endif // ERRORCONTROLLER_H
