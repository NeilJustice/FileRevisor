#pragma once

class EscapedRegexReplacer
{
public:
   virtual ~EscapedRegexReplacer() = default;
   virtual string ReplaceText(string_view str, string_view matchPattern, string_view replacementPattern) const;
private:
   static string EscapeRegex(string_view regexPattern);
};
