#ifndef TOML_UTF_CODEPOINT
#define TOML_UTF_CODEPOINT
#include <string>
#include <iostream>
#include <sstream>

namespace toml
{

template<typename charT>
std::basic_string<charT>
utf8_to_char(const std::basic_string<charT>& str)
{
    unsigned int codepoint;
    std::basic_istringstream<charT> iss(str);
    iss >> std::hex >> codepoint;

    std::basic_string<charT> charactor;
    if(codepoint < 0x80)
    {
        charactor += static_cast<unsigned char>(codepoint);
    }
    else if(codepoint < 0x800)
    {
        charactor += static_cast<unsigned char>(0xC0 | codepoint >> 6);
        charactor += static_cast<unsigned char>(0x80 | (codepoint & 0x3F));
    }
    else if(codepoint < 0x10000)
    {
        charactor += static_cast<unsigned char>(0xE0 | codepoint >> 12);
        charactor += static_cast<unsigned char>(0x80 |(codepoint >> 6 & 0x3F));
        charactor += static_cast<unsigned char>(0x80 |(codepoint & 0x3F));
    }
    else
    {
        charactor += static_cast<unsigned char>(0xF0 | codepoint >> 18);
        charactor += static_cast<unsigned char>(0x80 |(codepoint >> 12 & 0x3F));
        charactor += static_cast<unsigned char>(0x80 |(codepoint >> 6  & 0x3F));
        charactor += static_cast<unsigned char>(0x80 |(codepoint & 0x3F));
    }
    return charactor;
}

/* @brief unescape toml::String. *
 * @param String inside of ""    *
 * @return unescaped string.     */
template<typename charT>
std::basic_string<charT>
unescape(const std::basic_string<charT>& str)
{
    std::basic_string<charT> retval;
    for(typename std::basic_string<charT>::const_iterator
            iter = str.begin(); iter != str.end(); ++iter)
    {
        if(*iter != '\\')
        {
            retval += *iter;
        }
        else // *iter == '\'
        {
            ++iter;
                 if(*iter == 'b') retval += '\b';
            else if(*iter == 't') retval += '\t';
            else if(*iter == 'n') retval += '\n';
            else if(*iter == 'f') retval += '\f';
            else if(*iter == 'r') retval += '\r';
            else if(*iter == '\"') retval += '\"';
            else if(*iter == '\\') retval += '\\';
            else if(*iter == 'u')
            {
                std::string utf8code("0000");
                for(std::size_t i=0; i<4; ++i)
                {
                    ++iter;
                    utf8code += *iter;
                }
                retval += utf8_to_char(utf8code);
            }
            else if(*iter == 'U')
            {
                std::string utf8code("");
                for(std::size_t i=0; i<8; ++i)
                {
                    ++iter;
                    utf8code += *iter;
                }
                retval += utf8_to_char(utf8code);
            }
        }
    }
    return retval;
}

}//toml

#endif /*TOML_UTF_CODEPOINT*/
