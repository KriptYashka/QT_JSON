#include "logic.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class JsonData{
private:
    /*  */
public:
    vector<vector<string>> read_json(string path){
        /* Считывает данные из файла и записывает
        Возвращает код ошибки*/

        vector<vector<string>> result;
        string line;
        ifstream myFile(path);
        if (!myFile.is_open()) throw runtime_error("Could not open file");

        //getline(myFile, line);

        while(getline(myFile, line)){
            vector<string> line_model = split_line(line);
            result.push_back(line_model);
        }
        return result;
    }

};
