/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;


import org.cocos2dx.lib.Cocos2dxActivity;
import android.os.Bundle;
import android.util.Log;

// 読み上げ用
import android.speech.tts.TextToSpeech;
import java.util.Locale;


public class AppActivity extends Cocos2dxActivity implements TextToSpeech.OnInitListener
{
	// 読み上げよう
	private static TextToSpeech textToSpeech;
	
	
	protected void onCreate(Bundle savedInstanceState)
	{ 
		super.onCreate(savedInstanceState);
		
		textToSpeech = new TextToSpeech(this, this);
	}
	
	
	
	
	// *********************************************
	// **               音声読み上げ用               **
	// *********************************************
	// TextToSpeech.OnInitListener
	@Override
	public void onInit(int status)
	{
        if (status != TextToSpeech.SUCCESS)
        {
            Log.d("", "Error Init");
            return;
        }

//        Locale locale = Locale.ENGLISH;
        Locale locale = Locale.JAPANESE;
        if (textToSpeech.isLanguageAvailable(locale) >= TextToSpeech.LANG_AVAILABLE)
        {
        	textToSpeech.setLanguage(locale);
        } else {
            Log.d("", "Error SetLocale");
        }
	}

	public static void stopSpeech()
	{
        if (textToSpeech.isSpeaking())
        {
        	textToSpeech.stop();
        }
	}

	public static void readSpeech(final String text)
	{
		if (text.length() == 0) return;	
		stopSpeech();
		textToSpeech.speak(text, TextToSpeech.QUEUE_ADD, null);
	}	
	
	public static boolean enbaleJapanese()
	{
		return enableLanguage(Locale.JAPANESE);
	}
	
	public static boolean enableEnglish()
	{
		return enableLanguage(Locale.ENGLISH);
	}
	
	public static boolean enableLanguage(Locale locale)
	{
		return (textToSpeech.isLanguageAvailable(locale) >= TextToSpeech.LANG_AVAILABLE);
	}
}
