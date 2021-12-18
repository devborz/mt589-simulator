#ifndef LISTMODELS_H
#define LISTMODELS_H

#include <emulator.hpp>
#include <QListWidgetItem>


//// Jump list item
class JumpListItem {
public:
    JumpListItem(const std::string& name,
                 const std::vector<int> value) {
        this->name = name;
        this->value = value;
    }

    std::string name;
    std::vector<int> value;
};

//// Flag Controls list item
class FlagControlListItem: public QListWidgetItem {
public:
    FlagControlListItem(const std::string& name,
                        const std::vector<int>& value) {
        this->name = name;
        this->value = value;
    }

    std::string name;
    std::vector<int> value;
};

// CPE functions list item
class FuncListItem {
public:
    FuncListItem(const std::string& name,
                 std::vector<int> func,
                 std::vector<int> k) {
        this->name = name;
        this->func = func;
        this->k = k;
    }

    FuncListItem(const std::string& name,
                 std::string& func_str,
                 std::string& k_str) {
        this->name = name;

        std::vector<int> func;
        for (char ch: func_str) {
             func.push_back(ch - '0');
        }
        std::vector<int> k;
        for (char ch: k_str) {
             k.push_back(ch - '0');
        }
        this->func = func;
        this->k = k;
    }

    std::string name;

    std::vector<int> func;

    std::vector<int> k;

    static std::map<std::string, std::string> cpe_functions;

    static std::vector<FuncListItem*> items;

    static void prepareFunctions();

    static std::string get_mnemonic(const std::vector<int> func,
                                    const std::vector<int> k) {
        std::string key;

        for (int i: func) {
            key += static_cast<char>(i);
        }
        for (int i: k) {
            key += static_cast<char>(i);
        }
        return cpe_functions[key];
    }

    std::string get_func() {
        std::string result = "";
        for (auto n: func) {
            result += '0' + n;
        }
        return result;
    }

    std::string get_k() {
        std::string result = "";
        for (auto n: k) {
            result += '0' + n;
        }
        return result;
    }
};

// Programm commands list item
class CommandItem: public QListWidgetItem {
public:
    std::bitset<7> f;
    std::bitset<8> i;
    std::bitset<8> k;
    std::bitset<8> m;
    std::bitset<1> ci;
    std::bitset<1> ri;

    CommandItem(std::bitset<7> f, std::bitset<8> i, std::bitset<8> k,
                std::bitset<8> m, std::bitset<1> ci, std::bitset<1> ri) {
        this->f = f;
        this->i= i;
        this->k = k;
        this->ci = ci;
        this->ri = ri;
        this->m = m;
    }

    std::string prepareText()  {
        std::string result = "";
        result += "F=";
        result += f.to_string();
        result += " I=";
        result += i.to_string();
        result += " K=";
        result += k.to_string();
        result += " CI=";
        result += ci.to_string();
        result += " RI=";
        result += ri.to_string();
        result += " M=";
        result += m.to_string();
        return result;
    }
};

#endif // LISTMODELS_H
