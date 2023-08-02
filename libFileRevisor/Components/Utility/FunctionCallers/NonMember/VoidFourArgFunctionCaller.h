#pragma once

template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
class VoidFourArgFunctionCaller
{
public:
   using VoidFourArgFunctionType = void(*)(Arg1Type, Arg2Type, Arg3Type, Arg4Type);

   virtual void Call(
      VoidFourArgFunctionType fourArgFunction,
      Arg1Type arg1,
      Arg2Type arg2,
      Arg3Type arg3,
      Arg4Type arg4) const
   {
      fourArgFunction(arg1, arg2, arg3, arg4);
   }

   virtual ~VoidFourArgFunctionCaller() = default;
};
