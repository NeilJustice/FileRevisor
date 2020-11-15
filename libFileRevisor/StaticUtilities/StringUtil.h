#pragma once

class String
{
public:
   static bool ContainsSubstring(std::string_view stringView, std::string_view substring);
   static bool CaseInsensitiveContainsSubstring(std::string_view stringView, std::string_view substring);
   static std::string RegexReplace(std::string_view stringView, std::string_view matchingRegex, std::string_view replacingRegex);

   template<typename T, typename... Ts>
   static std::string Concat(const T& value, const Ts&... values)
   {
      std::ostringstream stringStream;
      stringStream << value;
      RecursivelyConcatenate(&stringStream, values...);
      const std::string stringStreamConcatedValues = stringStream.str();
      return stringStreamConcatedValues;
   }

   template<typename T, typename... Ts>
   static void RecursivelyConcatenate(std::ostringstream* outStringStream, const T& value, const Ts&... values)
   {
      (*outStringStream) << value;
      RecursivelyConcatenate(outStringStream, values...);
   }

   template<typename T, typename... Ts>
   static void RecursivelyConcatenate(std::ostringstream* outStringStream, const T& value)
   {
      (*outStringStream) << value;
   }

   template<typename... T>
   static void RecursivelyConcatenate(std::ostringstream*)
   {
   }

   String() = delete;
private:
   static std::string ToAllLowercase(std::string_view str);
};
