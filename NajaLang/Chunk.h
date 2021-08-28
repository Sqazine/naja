#pragma once 
#include <cstdint>
#include <vector>
#include <string>
#include "Object.h"
namespace NajaLang
{
    enum OpCode
    {
        FLOAT_NUM_OP,
        INT_NUM_OP,
        RETURN_OP,
        TRUE_OP,
        FALSE_OP,
        NULL_OP,
    };

    class Chunk
    {
        public:
            Chunk();
            ~Chunk();

            void AddOpCode(uint8_t code);
            uint64_t AddObject(Object* object);

            std::string Stringify();
        private:
            std::vector<uint8_t> m_Codes;
            std::vector<Object*> m_Objects;
    };

} // namespace NajaLang
