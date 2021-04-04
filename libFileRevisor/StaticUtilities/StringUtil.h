#pragma once

class String
{
public:
   static bool ContainsSubstring(string_view stringView, string_view substring);
   static bool CaseInsensitiveContainsSubstring(string_view stringView, string_view substring);
   static string RegexReplace(string_view stringView, string_view matchingRegex, string_view replacingRegex);

   template<typename T, typename... Ts>
   static string Concat(const T& value, const Ts&... values)
   {
      ostringstream stringStream;
      stringStream << value;
      RecursivelyConcatenate(&stringStream, values...);
      const string stringStreamConcatedValues = stringStream.str();
      return stringStreamConcatedValues;
   }

   template<typename T, typename... Ts>
   static void RecursivelyConcatenate(ostringstream* outStringStream, const T& value, const Ts&... values)
   {
      (*outStringStream) << value;
      RecursivelyConcatenate(outStringStream, values...);
   }

   template<typename T, typename... Ts>
   static void RecursivelyConcatenate(ostringstream* outStringStream, const T& value)
   {
      (*outStringStream) << value;
   }

   template<typename... T>
   static void RecursivelyConcatenate(ostringstream*)
   {
   }

   String() = delete;
private:
   static string ToAllLowercase(string_view str);
};
