//
//  NSFileManager+TEAdditions.h
//
//  Created by Greg Slepak on 6/2/08.
//
// Copyright 2008 Tao Effect LLC. All rights reserved.
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

#import <Cocoa/Cocoa.h>
#import <sys/types.h>
#import <sys/acl.h>

@interface NSFileManager (TEAdditions)

// these return the new path on success or nil on failure
- (NSString*)moveFileUpADirectory:(NSString *)itemPath;
- (NSString*)moveFile:(NSString *)itemPath toDirectory:(NSString *)dir;
- (NSString*)renameItemAtPath:(NSString *)path to:(NSString *)newName;
- (NSString*)forceRenameItemAtPath:(NSString *)path to:(NSString *)newName; // will append date if necessary
- (BOOL)deleteIfEmpty:(NSString*)path;

- (int64_t)approxSizeOfFolderAtPath:(NSString *)path; // quickly calculates a folder's size in MB (-1 on error)
- (NSUInteger)fastFolderSizeAtFSRef:(FSRef *)theFileRef cancelBlock:(BOOL(^)(NSUInteger curSize))cancelBlock;

// a bad-ass copy. synchronous.
- (BOOL)copyContentsFrom:(NSURL*)fromURL
          intoLocationAt:(NSURL*)toURL
                 options:(OptionBits)flags
             cancelBlock:(BOOL(^)(OSStatus err, NSDictionary *dict))cancelBlock
                   error:(NSError**)error;

- (BOOL)copyContentsFrom:(NSURL*)fromURL
          intoLocationAt:(NSURL*)toURL
         destinationName:(NSString*)destinationName
                 options:(OptionBits)flags
              upOneLevel:(BOOL)upOneLevel
             cancelBlock:(BOOL(^)(OSStatus err, NSDictionary *dict))cancelBlock
                   error:(NSError**)error;

- (BOOL)moveContentsFrom:(NSURL*)fromURL
          intoLocationAt:(NSURL*)toURL
         destinationName:(NSString*)destinationName
                 options:(OptionBits)flags
             cancelBlock:(BOOL(^)(OSStatus err, NSDictionary *dict))cancelBlock
                   error:(NSError**)error;


- (BOOL)folderAtPath:(NSString *)path isAtLeast:(int32_t)megs; // quickly answers this question
- (uid_t)ownerForPath:(NSString*)path;
- (BOOL)modifyCatologInfoAtPath:(NSString *)filePath isVisible:(BOOL)val;
- (BOOL)modifyCatologInfoAtPath:(NSString *)filePath hasCustomIcon:(BOOL)val;
- (BOOL)modifyCatologInfoAtURL:(NSURL *)fileURL hasCustomIcon:(BOOL)val; // NSUIntegerMax = ERROR
- (BOOL)modifyCatalogFlag:(NSInteger)flag forPath:(NSString*)filePath enable:(BOOL)enable;
- (BOOL)modifyCatalogFlag:(NSInteger)flag forURL:(NSURL*)fileURL enable:(BOOL)enable;
- (BOOL)folderHasCustomIcon:(NSString *)folderPath;
- (BOOL)isSymbolicLinkAtPath:(NSString *)path;
- (BOOL)isAliasFolderAtPath:(NSString *)path;
- (BOOL)isDirectoryAtPath:(NSString *)path;
- (BOOL)isVolumeAtPath:(NSString *)path;
- (BOOL)isVolumeAtURL:(NSURL*)url error:(NSError**)error;
- (BOOL)isReadOnlyFSAtPath:(NSString *)path;
- (OSStatus)ejectVolumeAtPath:(NSString *)path pid:(pid_t*)dissenter;

- (NSString *)fileSystemDescriptionAtMount:(NSString *)mountPoint;
- (NSString *)volnameAtURL:(NSURL*)url;

// ACLs
- (OSStatus)removeACLsAtPath:(NSString *)path matchingPerm:(acl_perm_t)perm andTag:(acl_tag_t)tag;
- (OSStatus)removeACL:(const char *)aclText fromFile:(NSString *)filePath;
- (OSStatus)addACL:(const char *)aclText toFile:(NSString *)filePath;
- (BOOL)fileHasACLs:(NSString *)filePath;
- (BOOL)fileHasACLs:(NSString *)filePath matchingPerm:(acl_perm_t)perm andTag:(acl_tag_t)tag;

// NOTE: you must call acl_free((void*)acl); on the returned acl_t when you're done with it
- (acl_t)aclsForFileAtPath:(NSString *)filePath;
// pass NULL for acl if you want to remove them all
- (OSStatus)setACL:(acl_t)acl forFileAtPath:(NSString *)filePath;

- (void)notePathChanged:(NSString *)path;

// extended attributes
- (NSArray*)xattrNamesAtPath:(NSString*)path;
- (NSData*)xattrDataForName:(NSString*)name atPath:(NSString*)path;
- (OSStatus)setData:(NSData*)data forXattr:(NSString*)name atPath:(NSString*)path;

@end
