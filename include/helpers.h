#pragma once

namespace SimpleRTTR
{
    //--
    //String manipulation helpers
    inline stdrttr::string& trimr(stdrttr::string& str)
    {
        if (!str.size()) return str;
        // find the last whitespace chr, then delete from there to end.
        stdrttr::string::iterator iter = --str.end();
        while (isspace(*iter)) --iter;
        str.erase(++iter, str.end());
        return str;
    }

    inline stdrttr::string& triml(stdrttr::string& str)
    {
        if (!str.size()) return str;
        // find first non-whitespace, then delete from begin to there.
        stdrttr::string::iterator iter = str.begin();
        while (iter != str.end() && isspace(*iter)) ++iter;
        str.erase(str.begin(), iter);
        return str;
    }

    inline stdrttr::string& trim(stdrttr::string& str)
    {
        return trimr(triml(str));
    }
}