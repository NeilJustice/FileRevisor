#include "pch.h"
#include "libFileRevisor/StaticUtilities/Map.h"
#include "libFileRevisor/Components/Utility/Docopt/DocoptParser.h"
#include "libFileRevisor/Components/Utility/Docopt/docopt.h"

DocoptParser::DocoptParser()
   // Function Pointers
   : _call_docopt_docopt(docopt::docopt)
{
}

map<string, docopt::Value> DocoptParser::ParseArgs(
   const string& usage, const vector<string>& argv) const
{
   if (argv.empty())
   {
      throw invalid_argument("argv cannot be empty");
   }
   const vector<string> argvWithoutFirstArgument(argv.data() + 1, argv.data() + argv.size());
   map<string, docopt::Value> argPairs =
      _call_docopt_docopt(usage, argvWithoutFirstArgument, true, "", false);
   return argPairs;
}

string DocoptParser::GetRequiredString(
   const map<string, docopt::Value>& docoptArgs, const string& argName) const
{
   const docopt::Value docoptValue = Map::At(docoptArgs, argName);
   const string& stringArg = docoptValue.AsString();
   return stringArg;
}

bool DocoptParser::GetRequiredBool(
   const map<string, docopt::Value>& docoptArgs, const string& argName) const
{
   const docopt::Value docoptValue = Map::At(docoptArgs, argName);
   const bool boolValue = docoptValue.AsBool();
   return boolValue;
}

string DocoptParser::GetProgramModeSpecificRequiredString(
   const map<string, docopt::Value>& docoptArgs, unsigned modeValue, unsigned fieldIsRequiredIfModeEqualsThisValue, const string& argName) const
{
   if (modeValue == fieldIsRequiredIfModeEqualsThisValue)
   {
      const docopt::Value docoptValue = Map::At(docoptArgs, argName);
      const string& stringValue = docoptValue.AsString();
      return stringValue;
   }
   return string();
}

bool DocoptParser::GetOptionalBool(
   const map<string, docopt::Value>& docoptArgs, const string& argName) const
{
   docopt::Value docoptValue;
   if (Map::TryGetValue(docoptArgs, argName, docoptValue))
   {
      const bool boolValue = docoptValue.AsBool();
      return boolValue;
   }
   return false;
}

string DocoptParser::GetOptionalString(
   const map<string, docopt::Value>& docoptArgs, const string& argName) const
{
   docopt::Value docoptValue;
   if (Map::TryGetValue(docoptArgs, argName, docoptValue))
   {
      const string& stringValue = docoptValue.AsString();
      return stringValue;
   }
   return string();
}

string DocoptParser::GetOptionalStringWithDefaultValue(
   const map<string, docopt::Value>& docoptArgs, string_view argName, string_view defaultValue) const
{
   docopt::Value docoptValue;
   if (Map::TryGetValue(docoptArgs, string(argName), docoptValue))
   {
      if (docoptValue.IsString())
      {
         const string& stringValue = docoptValue.AsString();
         return stringValue;
      }
   }
   return string(defaultValue);
}
