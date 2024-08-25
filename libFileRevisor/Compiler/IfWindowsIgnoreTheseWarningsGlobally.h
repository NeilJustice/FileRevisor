#pragma once

#ifdef _WIN32
#pragma warning(disable: 4061) // enumerator 'ProgramMode::Unset' in switch of enum 'ProgramMode' is not explicitly handled by a case label
#pragma warning(disable: 4514) // 'Function' unreferenced inline function has been removed
#pragma warning(disable: 4619) // #pragma warning: there is no warning number '5267'
#pragma warning(disable: 4623) // '__std_type_info_data': default constructor was implicitly defined as deleted
#pragma warning(disable: 4625) // copy constructor was implicitly defined as deleted
#pragma warning(disable: 4626) // assignment operator was implicitly defined as deleted
#pragma warning(disable: 4710) // function not inlined
#pragma warning(disable: 4711) // function selected for automatic inline expansion
#pragma warning(disable: 4820) // 'N' bytes padding added after data member 'dataMember'
#pragma warning(disable: 4866) // compiler may not enforce left-to-right evaluation order for call to 'std::operator<<<char,std::char_traits<char> >
#pragma warning(disable: 5026) // move constructor was implicitly defined as deleted
#pragma warning(disable: 5027) // move assignment operator was implicitly defined as deleted
#pragma warning(disable: 5039) // pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavimay occur if this function throws an exception.
#pragma warning(disable: 5045) // Compiler will insert Spectre mitigation for memory load if / Qspectre switch specified
#pragma warning(disable: 5246) // the initialization of a subobject should be wrapped in braces
#pragma warning(disable: 5262) // implicit fall-through occurs here
#pragma warning(disable: 5264) // variable is not used
#pragma warning(disable: 5267) // definition of implicit copy constructor is deprecated because it has a user-provided destructor
#endif
