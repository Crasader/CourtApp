//
//  ReadSpeech_objc.m
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/10/27.
//
//

#import "ReadSpeech_objc.h"



@implementation ReadSpeech_objc
{
    AVSpeechSynthesizer* speechSynthesizer;
//    NSMutableArray* speechTextList; = [@[] mutableCopy];
}


static ReadSpeech_objc * _getInstance = nil;
+ (ReadSpeech_objc *)getInstance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _getInstance = [[self alloc] init];
    });
    return _getInstance;
}

- (id)init
{
    self = [super init];
    if (self) {
        // AVSpeechSynthesizerを初期化
        speechSynthesizer = [[AVSpeechSynthesizer alloc] init];
        speechSynthesizer.delegate = self;
    }
    return self;
}






- (void)speech:(NSString *)text
{
    [self stop];
    AVSpeechUtterance *utterance = [AVSpeechUtterance speechUtteranceWithString:text];
    [speechSynthesizer speakUtterance:utterance];
}

- (void)stop
{
    [speechSynthesizer stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];
}


#pragma mark - AVSpeechSynthesizerDelegate

// 再生が始まった時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didStartSpeechUtterance:(AVSpeechUtterance *)utterance
{
    NSLog(@"didStartSpeechUtterance");
}

// 再生が終わった時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didFinishSpeechUtterance:(AVSpeechUtterance *)utterance
{
    NSLog(@"didFinishSpeechUtterance");
}

// 再生が一時停止した時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didPauseSpeechUtterance:(AVSpeechUtterance *)utterance;
{
    NSLog(@"didPauseSpeechUtterance");
}

// 再生が再開されたとき
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didContinueSpeechUtterance:(AVSpeechUtterance *)utterance;
{
    NSLog(@"didContinueSpeechUtterance");
}

// 読み上げが停止した時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didCancelSpeechUtterance:(AVSpeechUtterance *)utterance;
{
    NSLog(@"didCancelSpeechUtterance");
}

// 再生中のテキスト範囲
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer willSpeakRangeOfSpeechString:(NSRange)characterRange utterance:(AVSpeechUtterance *)utterance;
{
    NSLog(@"willSpeakRangeOfSpeechString");
}


@end
