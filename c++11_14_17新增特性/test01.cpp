//
// Created by jxq on 19-12-27.
//

#include <iostream>
#include <string>
#include <initializer_list>
#include <vector>

using namespace std;

enum class jsonType
{
    jsonTypeNull,
    jsonTypeInt,
    jsonTypeLong,
    jsonTypeDouble,
    jsonTypeBool,
    jsonTypeString,
    jsonTypeArray,
    jsonTypeObject
};

struct jsonNode
{
    jsonNode(const char* key, const char* value) :
        m_type(jsonType::jsonTypeString),
        m_key(key),
        m_value(value)
    {
        cout << "jsonNode constructor1 called." << endl;
    }

    jsonNode(const char* key, double value) :
        m_type(jsonType::jsonTypeString),
        m_key(key),
        m_value(to_string(value))
    {
        cout << "jsonNode constructor2 called." << endl;
    }


    jsonType m_type;
    string m_key;
    string m_value;
};

class json
{
public:
    json()
    {

    }

    ~json()
    {

    }

    static json& array(initializer_list<jsonNode> nodes)
    {
        m_json.m_nodes.clear();
        m_json.m_nodes.insert(m_json.m_nodes.begin(), nodes.begin(), nodes.end());

        cout << "json::array() called." << endl;

        return m_json;
    }

    string toString()
    {
        size_t size = m_nodes.size();
        for (size_t i = 0; i < size; ++i)
        {
            switch (m_nodes[i].m_type)
            {
                case jsonType::jsonTypeString:
                    cout << "{ \"" << m_nodes[i].m_key << "\", \"" << m_nodes[i].m_value << "\" }" << endl;
                    break;
            }
        }
    }
private:
    vector<jsonNode> m_nodes;
    static json m_json;
};

json json::m_json;

int main()
{
    json array_not_object = json::array({{"currency", "USD"}, {"value", 42.99}});
    array_not_object.toString();
    return 0;
}