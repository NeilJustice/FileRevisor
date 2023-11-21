#pragma once

class TextReplacer
{
public:
   virtual ~TextReplacer() = default;
   virtual string ReplaceText(string_view str, string_view substring, string_view replacement) const;
};
