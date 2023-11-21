#pragma once

class RegexReplacer
{
public:
   virtual ~RegexReplacer() = default;
   virtual string RegexReplace(string_view str, string_view matchPattern, string_view replacementPattern) const;
};
