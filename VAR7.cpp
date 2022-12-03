
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <stdlib.h>

struct scan_info
{
    char model[20]; // наименование модели
    int price; // цена
        double x_size; // горизонтальный размер области сканирования
        double y_size; // вертикальный размер области сканирования
    int optr; // оптическое разрешение
        int grey; // число градаций серого
};

void WriteToEnd(scan_info& rec, std::fstream& data, const int bufSize, std::string name);
void Input(scan_info& rec);
bool IsValid(bool val);
bool operator == (const scan_info& a, const scan_info& b);

int main()
{
    std::string filename = "C:\\Users\\dns\\Desktop\\test.bin";
    scan_info record;
    std::fstream file(filename, std::ios_base::binary | std::ios_base::in);
    int addZero = 0;
    int bufint = 2;
    int fileSize = 0;
    bool cont = true;
    if (!file.is_open())
        std::cout << "File does not exist!\n";
    else {
        if (file.peek() == EOF)
        {
            file.close();
            file.open(filename, std::ios_base::binary | std::ios_base::app);
            file.write((char*)&addZero, sizeof(int));
        }
        while (cont)
        {
            file.seekp(0, std::ios::end);
            fileSize = (int) file.tellp();
            file.close();
            Input(record);
            WriteToEnd(record, file, fileSize, filename);
            std::cout << "Continue? (1,0): ";
            std::cin >> cont;
            system("cls");
        }
    }
    return 0;
}



void WriteToEnd(scan_info& rec, std::fstream& data, int bufSize, std::string name)
{
    scan_info temp;
    int count;
    bool recEq = false;
    data.open(name, std::ios_base::binary | std::ios_base::in);
    data.read((char*)&count, sizeof(int));
    while ((data.peek() != EOF) && !recEq) {
        data.read((char*)&temp.model, 20);
        data.read((char*)&temp.price, sizeof(int));
        data.read((char*)&temp.x_size, sizeof(double));
        data.read((char*)&temp.y_size, sizeof(double));
        data.read((char*)&temp.optr, sizeof(int));
        data.read((char*)&temp.grey, sizeof(int));
        recEq = (rec == temp);
    };
    if (!recEq) {
        data.seekg(4, std::ios::beg);
        count++;
        char* buffer = new char[bufSize - 3];
        data.read(buffer, bufSize - 4);
        buffer[bufSize - 4] = '\0';
        data.close();
        data.open(name, std::ios_base::binary | std::ios_base::out);
        data.write((char*)&count, sizeof(int));
        data.write(buffer, bufSize - 4);
        data.write((char*)&rec.model, 20);
        data.write((char*)&rec.price, sizeof(int));
        data.write((char*)&rec.x_size, sizeof(double));
        data.write((char*)&rec.y_size, sizeof(double));
        data.write((char*)&rec.optr, sizeof(int));
        data.write((char*)&rec.grey, sizeof(int));
        delete[] buffer;
    }
}




void Input(scan_info& rec)
{
    std::string inp;
    std::cout << "Model: ";
    std::cin >> inp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 0; i <= 18; i++)
        if (i > (inp.size() - 1))
            rec.model[i] = '\0';
        else rec.model[i] = inp[i];
    rec.model[19] = '\0';
    system("cls");
    do {
        std::cout << "Price: ";
        std::cin >> rec.price;
    } while (IsValid(std::cin.fail()));
    do {
        std::cout << "X-Size: ";
        std::cin >> rec.x_size;
    } while (IsValid(std::cin.fail()));
    do {
        std::cout << "Y-Size: ";
        std::cin >> rec.y_size;
    } while (IsValid(std::cin.fail()));
    do {
        std::cout << "Opt. Resolution: ";
        std::cin >> rec.optr;
    } while (IsValid(std::cin.fail()));
    do {
        std::cout << "Grey Scale: ";
        std::cin >> rec.grey;
    } while (IsValid(std::cin.fail()));
}




bool IsValid(bool val)
{
    system("cls");
    if (val) {
        std::cin.clear();
        std::cout << "Invalid Input!\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return val;
}




bool operator == (const scan_info& a, const scan_info& b)
{
    int i = 0;
    bool arrEq = true;
    while (arrEq && i <= 19) {
        if (a.model[i] != b.model[i])
            arrEq = false;
        i++;
    }
    return arrEq && (a.price == b.price) && (a.x_size == b.x_size) && (a.y_size == b.y_size) && (a.optr == b.optr) && (a.grey == b.grey);
}