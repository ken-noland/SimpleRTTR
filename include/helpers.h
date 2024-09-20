#pragma once

namespace SimpleRTTR
{
    //--
    //String manipulation helpers
    inline std::string& trimr(std::string& str)
    {
        if (!str.size()) return str;
        // find the last whitespace chr, then delete from there to end.
        std::string::iterator iter = --str.end();
        while (isspace(*iter)) --iter;
        str.erase(++iter, str.end());
        return str;
    }

    inline std::string& triml(std::string& str)
    {
        if (!str.size()) return str;
        // find first non-whitespace, then delete from begin to there.
        std::string::iterator iter = str.begin();
        while (iter != str.end() && isspace(*iter)) ++iter;
        str.erase(str.begin(), iter);
        return str;
    }

    inline std::string& trim(std::string& str)
    {
        return trimr(triml(str));
    }
}
