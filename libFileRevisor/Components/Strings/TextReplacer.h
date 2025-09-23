#pragma once

class TextReplacer
{
public:
   virtual ~TextReplacer() = default;

   virtual string ReplaceText(
      string_view str,
      string_view substring,
      const string& replacement) const;
private:
   static regex EscapeRegexPattern(string_view regexPattern);
};
