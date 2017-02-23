//
//  ReadSpeech_objc.m
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/10/27.
//
//

#import "ReadSpeech_objc.h"

@implementation ReadSpeech_objc()
{
    AVSpeechSynthesizer* speechSynthesizer = nil;
    NSMutableArray* speechTextList = [@[] mutableCopy];
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
        speechSynthesizeer = [[AVSpeechSynthesizer alloc] init];
        speechSynthesizeer.delegate = self;
    }
    return self;
}






- (void)speech:(NSString *)text
{
    [self.speechTextList addObject:text];
    [self startSpeech];
    
    
    
    // AVSpeechUtteranceを読ませたい文字列で初期化する。
    NSString* speakingText = text;
    AVSpeechUtterance *utterance = [AVSpeechUtterance speechUtteranceWithString:speakingText];
    
    // AVSpeechSynthesizerにAVSpeechUtteranceを設定して読んでもらう
    [speechSynthesizer speakUtterance:utterance];
}


- (void)startSpeech
{
    if () return;
    
    
    
    
    
}


#pragma mark - AVSpeechSynthesizerDelegate

// 再生が始まった時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didStartSpeechUtterance:(AVSpeechUtterance *)utterance;
// 再生が終わった時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didFinishSpeechUtterance:(AVSpeechUtterance *)utterance
{
    if ([self.]
}



// 再生が一時停止した時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didPauseSpeechUtterance:(AVSpeechUtterance *)utterance;
// 再生が再開されたとき
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didContinueSpeechUtterance:(AVSpeechUtterance *)utterance;
// 読み上げが停止した時
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didCancelSpeechUtterance:(AVSpeechUtterance *)utterance;
// 再生中のテキスト範囲
- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer willSpeakRangeOfSpeechString:(NSRange)characterRange utterance:(AVSpeechUtterance *)utterance;




@end
