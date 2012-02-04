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

// parts of this credit to Jens Alfke
// https://bitbucket.org/snej/myutilities/src/tip/CollectionUtils.h
// /Volumes/EspionageMounts/gslepak/3117152101/Programming/Cocoa/ThirdPartyGoodies/Utility/myutilities.hg/CollectionUtils.h

#import <Foundation/Foundation.h>

// -- logging --

#if defined(DEBUG) || defined(DEBUG_RELEASE)
#	define log_debug(msg, ...) NSLog(@"DEBUG: " msg, ## __VA_ARGS__)
#	define DEBUG_STMT(...)     __VA_ARGS__;
#else
#	define log_debug(msg, ...) /* nothing */
#	define DEBUG_STMT(...)     /* nothing */
#endif

#ifdef DEBUG_SPECIAL
#	define log_special(msg, ...) NSLog(@"DEBUG: " msg, ## __VA_ARGS__)
#else
#	define log_special(msg, ...) /* nothing */
#endif

#define log_info(msg, ...) NSLog(@"INFO: " msg, ## __VA_ARGS__)
#define log_warn(msg, ...) NSLog(@"WARN (%s:%d): "  msg, __func__, __LINE__, ## __VA_ARGS__)
#define log_err(msg, ...)  NSLog(@"ERROR (%s:%d): " msg, __func__, __LINE__, ## __VA_ARGS__)
#define log_errp(msg, ...) NSLog(@"ERROR (%s:%d): " msg ": %s", __func__, __LINE__, ## __VA_ARGS__, strerror(errno))

// -- conveniences --

// Collection creation conveniences:

#define $A                  [NSArray array]
#define $a(OBJS...)         ({id objs[]={OBJS}; [NSArray arrayWithObjects:objs count:sizeof(objs)/sizeof(id)];})
#define $MA                 [NSMutableArray array]
#define $ma(OBJS...)        ({id objs[]={OBJS}; [NSMutableArray arrayWithObjects:objs count:sizeof(objs)/sizeof(id)];})

#define $D                  [NSDictionary dictionary]
#define $d(...)             _dictof(__VA_ARGS__, nil)
#define $MD                 [NSMutableDictionary dictionary]
#define $md(...)            ((NSMutableDictionary*)_dictof(__VA_ARGS__, nil))

#define $num(VAL)             ({__typeof(VAL) v=VAL; _box(&v,@encode(__typeof(v)));})
#define $color(r,g,b,a)     [NSColor colorWithCalibratedRed:r green:g blue:b alpha:a]

// other places to get info: NSProcessInfo, NSWorkspace, NSApplication, NSRunningApplication
#define $app                [NSRunningApplication currentApplication]

#define $yes                ((__bridge NSNumber*)kCFBooleanTrue)
#define $no                 ((__bridge NSNumber*)kCFBooleanFalse)

#define $sprintf(FMT, ...)  [NSString stringWithFormat:FMT, ## __VA_ARGS__]

#define $join(a, b)			a##b

#define INMAIN(...)         [[NSOperationQueue mainQueue] addOperationWithBlock:^{ __VA_ARGS__; }]

#define INMAINWAIT(...) do { \
    void(^aBlock)() = ^{ \
        __VA_ARGS__; \
    }; \
    if ( __unlikely([NSThread isMainThread]) ) \
        aBlock(); \
    else \
        [[NSOperationQueue mainQueue] addOperations: \
                                [NSArray arrayWithObject:[NSBlockOperation blockOperationWithBlock:aBlock]] waitUntilFinished:YES]; \
} while (0)


// NSError

enum {
    errTEUnknown = -1,
    errTEUserCanceled = 1000,
    errTEOperationFailed,
    errTEExceptionRaised,
    errTEInvalidParams,
    errTEInvalidPass,
    errTEPartialCopy,
    errTECopySuccessMoveFail
};

// IMPORTANT! Do NOT call this within an @autoreleasepool if you are setting an
// out NSError** variable. That will cause an extra release b/c of compiler inserted
// __autoreleasing type decorator. Using +alloc/-init will NOT compensate for this.
#define $error(CODE, MSG, ...)    [NSError errorWithDomain:$app.localizedName code:CODE userInfo:$d(NSLocalizedDescriptionKey, $sprintf(MSG, ## __VA_ARGS__))]

// Object conveniences:

BOOL $equal(id obj1, id obj2);      // Like -isEqual: but works even if either/both are nil

#define $observe(_who, _what, _where)	[[NSNotificationCenter defaultCenter] addObserver:_who selector:_where name:_what object:nil]
#define $note(NAME, OBJ)    [NSNotification notificationWithName:NAME object:OBJ]

// -- optimizations --

#ifndef __unlikely
#define __unlikely(cond) __builtin_expect(!!(cond), 0)
#endif
#ifndef __likely
#define __likely(cond) __builtin_expect(!!(cond), 1)
#endif

// -- exception hanlding --

#define CATCH_EXCEPTION @catch (NSException *e) { log_warn("*** Caught %@! *** %@", [e name], [e reason]); }
#define TRY(...) @try { __VA_ARGS__; } CATCH_EXCEPTION

// -- error handling --

#define DO_FAILABLE(_errVar, _func, args...) do { \
	if ( __unlikely((_errVar = _func(args)) != 0) ) { \
		log_err(#_func " returned: %d", (int)_errVar); \
		goto fail_label; \
	} \
} while (0)

#define DO_FAILABLE_SUB(_errVar, _subst, _func, args...) do { \
	if ( __unlikely((_errVar = _func(args)) != 0) ) { \
		_errVar = _subst; log_err(#_func " resulted in: %d", (int)_errVar); \
		goto fail_label; \
	} \
} while (0)

#define FAILABLE(_errVar, _func, args...) do { \
	if ( __unlikely((_errVar = _func(args)) != 0) ) { \
		log_err(#_func " returned: %d", (int)_errVar); \
	} \
} while (0)

#define FAIL_IF(_cond, args...) do { \
	if ( __unlikely((_cond) != 0) ) { log_err("failed because: " #_cond); args; goto fail_label; } \
} while (0)

#define FAIL_IFQ(_cond, args...) do { \
	if ( __unlikely((_cond) != 0) ) { args; goto fail_label; } \
} while (0)

// -- alerts --

#define TERunAlertPanel(TITLE, MSG, DEFAULT, ALT, OTHER, ARGS...) ({ \
    [NSApp activateIgnoringOtherApps:YES]; \
    NSLog(@"WARN (%s:%d): %@", __func__, __LINE__, $sprintf(MSG, ##ARGS)); \
    NSRunAlertPanel(TITLE, $sprintf(MSG, ##ARGS), DEFAULT, ALT, OTHER); \
})

#define TERunCriticalAlertPanel(TITLE, MSG, DEFAULT, ALT, OTHER, ARGS...) ({ \
    [NSApp activateIgnoringOtherApps:YES]; \
    NSLog(@"ERROR (%s:%d): %@",  __func__, __LINE__, $sprintf(MSG, ##ARGS)); \
    NSRunCriticalAlertPanel(TITLE, $sprintf(MSG, ##ARGS), DEFAULT, ALT, OTHER); \
})

// why these bizarre delays to display the errors? because the dragging system is fucked
// if we don't do it this way and the popoverWindow is up, then when the user hits "Enter"
// to dismiss the dialog, the keypress won't dismiss it, instead it will be sent post-facto
// to whatever app he dragged the file from.

#define TEDelayedAlertPanel(TITLE, MSG, DEFAULT, ALT, OTHER, ARGS...) do { \
    [NSApp activateIgnoringOtherApps:YES]; \
    NSLog(@"WARN (%s:%d): %@", __func__, __LINE__, $sprintf(MSG, ##ARGS)); \
    [[TEAppDelegate delegate] afterDelay:0.2 performBlockOnMainThread:^{ \
        NSRunAlertPanel(TITLE, $sprintf(MSG, ##ARGS), DEFAULT, ALT, OTHER); \
    }]; \
} while (0)

#define TEDelayedAlertCriticalPanel(TITLE, MSG, DEFAULT, ALT, OTHER, ARGS...) do { \
    [NSApp activateIgnoringOtherApps:YES]; \
    NSLog(@"ERROR (%s:%d): %@",  __func__, __LINE__, $sprintf(MSG, ##ARGS)); \
    [[TEAppDelegate delegate] afterDelay:0.2 performBlockOnMainThread:^{ \
        NSRunCriticalAlertPanel(TITLE, $sprintf(MSG, ##ARGS), DEFAULT, ALT, OTHER); \
    }]; \
} while (0)


// Internals (don't use directly)
NSDictionary* _dictof(id key, ...) NS_REQUIRES_NIL_TERMINATION;
NSNumber* _box(const void *value, const char *encoding);
