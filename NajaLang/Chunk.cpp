#include "Chunk.h"
#include <string_view>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace NajaLang
{

    Chunk::Chunk()
    {
    }
    Chunk::~Chunk()
    {
    }

    void Chunk::AddOpCode(uint8_t code)
    {
        m_Codes.emplace_back(code);
    }

    uint64_t Chunk::AddObject(Object *object)
    {
        m_Objects.emplace_back(object);
        return m_Objects.size() - 1;
    }

    std::string Chunk::Stringify()
    {
        std::stringstream result;
        for (size_t i = 0; i < m_Codes.size(); ++i)
        {
            switch (m_Codes[i])
            {
            case RETURN_OP:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "RETURN_OP\n";
                break;
            case FLOAT_NUM_OP:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "FLOAT_NUM_OP"
                       << "     " << std::to_string(m_Codes[++i]) << "    " << m_Objects[m_Codes[i]]->Stringify() << "\n";

                break;
            case INT_NUM_OP:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "INT_NUM_OP"
                       << "     " << std::to_string(m_Codes[++i]) << "     " << m_Objects[m_Codes[i]]->Stringify() << "\n";
                break;
            case TRUE_OP:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "TRUE_OP"
                       << "     " << std::to_string(m_Codes[++i]) << "     " << m_Objects[m_Codes[i]]->Stringify() << "\n";
                break;
            case FALSE_OP:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "FALSE_OP"
                       << "     " << std::to_string(m_Codes[++i]) << "     " << m_Objects[m_Codes[i]]->Stringify() << "\n";
                break;
            case NULL_OP:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "NULL_OP"
                       << "     " << std::to_string(m_Codes[++i]) << "     " << m_Objects[m_Codes[i]]->Stringify() << "\n";
                break;
            default:
                result << std::setfill('0') << std::setw(8) << i << "     "
                       << "Unknown Op\n";
                break;
            }
        }
        return result.str();
    }
}