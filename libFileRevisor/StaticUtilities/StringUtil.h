#pragma once

class String
{
public:
   template<typename... Types>
   static string ConcatStrings(Types&&... values)
   {
      string concatenatedString;
      string_view stringViews[] = { values... };
      size_t lengthOfAllStrings = 0;
      for (string_view stringView : stringViews)
      {
         const size_t stringViewSize = stringView.size();
         lengthOfAllStrings += stringViewSize;
      }
      concatenatedString.reserve(lengthOfAllStrings);
      for (string_view stringView : stringViews)
      {
         concatenatedString.append(stringView);
      }
      return concatenatedString;
   }

   template<typename... Types>
   static string ConcatValues(const Types&... values)
   {
      ostringstream oss;
      (oss << ... << values);
      string ossConcatenatedValues = oss.str();
      return ossConcatenatedValues;
   }

   static bool ContainsSubstring(string_view stringView, string_view substring);
   static bool CaseInsensitiveContainsSubstring(string_view stringView, string_view substring);
   static string ReplaceText(string_view stringView, string_view matchingRegex, string_view replacingRegex);

   template<typename T, typename... Ts>
   static void RecursivelyConcatenate(const ostringstream* outStringStream, const T& value, const Ts&... values)
   {
      (*outStringStream) << value;
      RecursivelyConcatenate(outStringStream, values...);
   }

   template<typename T, typename... Ts>
   static void RecursivelyConcatenate(const ostringstream* outStringStream, const T& value)
   {
      (*outStringStream) << value;
   }

   template<typename... T>
   static void RecursivelyConcatenate(const ostringstream*)
   {
   }

   String() = delete;
private:
   static string ToAllLowercase(string_view str);
};
