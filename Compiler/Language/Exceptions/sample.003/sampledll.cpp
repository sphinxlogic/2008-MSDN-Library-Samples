///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Microsoft Corp 2000.  All rights reserved.
//
//  Description: Shows how to throw and handle CLR Exceptions in C++
//
///////////////////////////////////////////////////////////////////////////////

#using <mscorlib.dll>

extern "C" __declspec(dllexport) void thrower()
{
    throw gcnew System::Exception;
}
