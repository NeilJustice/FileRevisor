#pragma once

class Pluralizer
{
public:
   virtual std::string PotentiallyPluralizeWord(size_t count, std::string_view singularWord, std::string_view pluralWord) const;
   virtual ~Pluralizer() = default;
};
