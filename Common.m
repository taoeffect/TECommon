//
//  Common.m
//  Created by Greg Slepak on 8/28/11.
//
// Copyright 2011 Tao Effect LLC. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY TAO EFFECT LLC ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of Tao Effect LLC.
//

#import "Common.h"

NSDictionary* _dictof(id key, ...)
{
	NSMutableDictionary *dict = [NSMutableDictionary dictionary];
	va_list args;
	va_start(args, key);
	id value = va_arg(args, id);
	while (YES)
	{
		[dict setObject:value forKey:key];
		key = va_arg(args, id);
		if ( !key ) break;
		value = va_arg(args, id);
	}
	va_end(args);
	return dict;
}

BOOL $equal(id obj1, id obj2)      // Like -isEqual: but works even if either/both are nil
{
    if( obj1 )
        return obj2 && [obj1 isEqual: obj2];
    else
        return obj2==nil;
}


NSNumber* _box(const void *value, const char *encoding)
{
    // file:///Developer/Documentation/DocSets/com.apple.ADC_Reference_Library.DeveloperTools.docset/Contents/Resources/Documents/documentation/DeveloperTools/gcc-4.0.1/gcc/Type-encoding.html
    char e = encoding[0];
    if( e=='r' )                // ignore 'const' modifier
        e = encoding[1];
    switch( e ) {
        case 'B':   return [NSNumber numberWithBool: *(BOOL*)value];
        case 'c':   return [NSNumber numberWithChar: *(char*)value];
        case 'C':   return [NSNumber numberWithUnsignedChar: *(char*)value];
        case 's':   return [NSNumber numberWithShort: *(short*)value];
        case 'S':   return [NSNumber numberWithUnsignedShort: *(unsigned short*)value];
        case 'i':   return [NSNumber numberWithInt: *(int*)value];
        case 'I':   return [NSNumber numberWithUnsignedInt: *(unsigned int*)value];
        case 'l':   return [NSNumber numberWithLong: *(long*)value];
        case 'L':   return [NSNumber numberWithUnsignedLong: *(unsigned long*)value];
        case 'q':   return [NSNumber numberWithLongLong: *(long long*)value];
        case 'Q':   return [NSNumber numberWithUnsignedLongLong: *(unsigned long long*)value];
        case 'f':   return [NSNumber numberWithFloat: *(float*)value];
        case 'd':   return [NSNumber numberWithDouble: *(double*)value];
//        case '*':   return [NSString stringWithUTF8String: *(char**)value];
            // TODO: figure out ARC implications
//        case '@':   return *(id*)value;
//        default:    return [NSValue value: value withObjCType: encoding];
        default:    return [NSNumber numberWithUnsignedLongLong: *(unsigned long long*)value];;
    }
}



/*
 Copyright (c) 2008, Jens Alfke <jens@mooseyard.com>. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted
 provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions
 and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 and the following disclaimer in the documentation and/or other materials provided with the
 distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRI-
 BUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
 THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */