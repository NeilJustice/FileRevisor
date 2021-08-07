#pragma once

class Regexer
{
public:
   virtual string Replace(string_view str, string_view matchPattern, string_view replacementPattern) const;
   virtual ~Regexer() = default;
};
