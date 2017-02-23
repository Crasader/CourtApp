//
//  ReadSpeech_objc.h
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/10/27.
//
//

#import <Foundation/Foundation.h>
#import "AVFoundation/AVFoundation.h"

@interface ReadSpeech_objc : NSObject <AVSpeechSynthesizerDelegate>

+ (ReadSpeech_objc *)getInstance;
- (void)speech:(NSString *)text;
- (void)stop;

@end
