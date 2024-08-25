#pragma once
namespace docopt
{
   struct Value;
}
class DocoptParserTests;

class DocoptParser
{
   friend class DocoptParserTests;
private:
   // Function Pointers
   function<map<string, docopt::Value>(string, const vector<string>&, bool, string, bool)> _call_docopt_docopt;
public:
   DocoptParser();
   virtual ~DocoptParser() = default;
   virtual map<string, docopt::Value> ParseArgs(const string& usage, const vector<string>& argv) const;
   virtual string GetRequiredString(const map<string, docopt::Value>& docoptArgs, const string& argName) const;
   virtual bool GetRequiredBool(const map<string, docopt::Value>& docoptArgs, const string& argName) const;
   virtual string GetProgramModeSpecificRequiredString(
      const map<string, docopt::Value>& docoptArgs, unsigned modeValue, unsigned fieldIsRequiredIfModeEqualsThisValue, const string& argName) const;
   virtual bool GetOptionalBool(const map<string, docopt::Value>& docoptArgs, const string& argName) const;
   virtual string GetOptionalString(const map<string, docopt::Value>& docoptArgs, const string& argName) const;
   virtual string GetOptionalStringWithDefaultValue(const map<string, docopt::Value>& docoptArgs, string_view argName, string_view defaultValue) const;
};
