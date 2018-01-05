#pragma once

#include <string>

#include "../../BaseAutomata.h"
#include "../../../keypad/KeyCode.h"


class Automata : public BaseAutomata {


public:
    virtual int appendChar(const wchar_t c);

public:
	Automata();

	/**
	 * 현재 오토마타 상태와 입력된 글자들을 초기화.
	 @author jeonguklee
	 @date 2015-07-06
	 */
	void reset();
	/**
	 * 현재 오토마타 상태 초기화
	 @author jeonguklee
	 @date 2015-07-06
	 */
	virtual void resetState();
	/**
	 * 전달 받은 매개변수들의 상태 초기화
	 @author jeonguklee
	 @date 2015-07-17
	 @param1 state 글자 상태
	 @param2 buffer 한글 입력 버퍼
	 */
	void resetState(int &state, int *buffer);

	/**
	 @author jhkim
	 @date 2015-07-06
	 @brief 키보드 앱 TextBox에서 Composing된 문자열이 없을때, Cursor가 이동되면 호출되는 함수
	 @param beforeString Cursor기준 좌측 문자열
	 @param afterString Cursor기준 우측 문자열
	 */
	void cursorChange(wstring beforeString, wstring afterString);

	/**
	 @author jhkim
	 @date 2015-07-06
	 @brief 키보드 앱에서 터치된 Suggestion/Prediction 된 단어를 선택 했을 때, 호출되는 함수
	 @param *predictionWord 선택한 단어의 문자열
	 */
	void selectPrediction(wstring* predictionWord);

	/**
	 * 글자를 지울때 앞글자를 초중종성으로 분해하여 정보를 가져오기 위한 함수.
	 @author jeonguklee
	 @date 2015-07-20
	 @param1 primaryCode 분해 할 한글 코드
	 @param2 state 		 한글을 분해한 글자 상태
	 @param3 buffer		 분해 한 한글 코드를 저장할 한글입력버퍼
	 */
	void setBuffer(int primaryCode, int &state, int *buffer);
	/**
	 * 사용자가 글자를 작성한 후 입력하는 구두점 및 띄어쓰기 처리.
	 @author jeonguklee
	 @date 2015-07-06
	 @param unicode Separator처리를 위한 사용자가 입력한 유니코드 '.',   ',',   ' ',   '\n'
	 @return Automata::appendSeparator()함수 호출 (N-gram과 currentWord, composingWord 처리 후 0 반환)
	 */
	int appendSeparator(wchar_t unicode);

	/**
	 * 현재 작성중인 문자열에 글자 추가.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 완성형 한글 코드 등이 전달됨.
	 */
	void currentWordAppend(wchar_t wc);
	/**
	 * 현재 작성중인 문자열에 글자 하나를 삭제.
	 @author jeonguklee
	 @date 2015-07-06
	 */
	void currentWordDelete();
	/**
	 * 문자열과 키보드앱에 그려질 문자열을 모두 삭제 및 상태 초기화
	 @author jeonguklee
	 @date 2015-07-17
	 */
	virtual void deleteAllChar();
	/**
	 @author jhkim
	 @date 2015-07-06
	 @brief 키보드 앱 TextBox에 그려질 문자열을 반환
	 @return ComposingWord 리턴
	 */
	wstring getComposingWord();



	wstring getCurrentWord();

	 /**
	 * 입력한 자소가 초성 인덱스에서 몇번째인지 위치값 반환.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 매개변수의 초성 인덱스값 반환
	 */
	int getChoseongIndex(int primaryCode);
	/**
	 * 입력한 자소가 중성 인덱스에서 몇번째인지 위치값 반환.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 매개변수의 중성 인덱스값 반환
	 */
	int getJungseongIndex(int primaryCode);
	/**
	 * 입력한 자소가 종성 인덱스에서 몇번째인지 위치값 반환.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 매개변수의 종성 인덱스값 반환
	 */
	int getJongseongIndex(int primaryCode);
	/**
	 * 사용자가 입력한 글자 하나를 조합하여 완성형 유니코드로 값 반환.
	 @author jeonguklee
	 @date 2015-07-06
	 @param mHangulCharBuffer가 인자로 전달되어 한글 조합이 이루어짐.
	 @return 완성형 한글 유니코드 반환.
	 */
	int makeHangul(int buffer[]);

    /**
     * 전달된 인자가 한글인지 검사.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 문자 코드.
     @return 한글이라면 true 반환.
     */
	bool isHangul(wchar_t const primaryCode);
	/**
	 * 한글이 아닌 글자를 현재 작업중인 문자열에 저장.
	 @author jeonguklee
	 @date 2015-07-06
	 @param state       현재 입력된 글자 상태 (초성, 중성, 종성)
	 @param primaryCode 현재 입력된 한글의 자소 코드 (조합형 한글 코드)
	 */
	void saveUnknownCharacter(int const state, int const primaryCode);
	/**
	 * 인자로 들어온 한글 코드가 현재 작성중인 한 글자의 종성과 복합종성으로 조합이 가능한지 체크.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 복합종성이 가능한지 체크하여 결과값 반환.
	 */
	static bool isJongseongPair(int v);
	/**
	 * 인자로 들어온 한글 코드가 현재 작성중인 한 글자의 중성과 복합중성으로 조합이 가능한지 체크.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 복합중성이 가능한지 체크하여 결과값 반환.
	 */
	static bool isJungseongPair(int v);
	/**
	 * 인자로 받은 한글 코드가 복합중성이라면 복합중성을 분해하여 각각의 한글 코드를 저장한 배열로 반환.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 분리한 중성 코드를 r배열로 반환.
	 */
	static int *resolveJungseongPair(int v);
	/**
	 * 인자로 받은 한글 코드가 복합종성이라면 복합종성을 분해하여 각각의 한글 코드를 저장한 배열로 반환..
	 @author jeonguklee
	 @date 2015-07-06
	 @param 입력된 조합형 한글 코드.
	 @return 분리한 종성 코드를 r배열로 반환.
	 */
	static int *resolveJongseongPair(int v);
	/**
	 * 두개의 인자값을 비교하여 복합종성으로 조합이 가능한지 체크하여 조합된 복합종성 코드값 반환.
	 @author jeonguklee
	 @date 2015-07-06
	 @param v1 복합종성의 첫 자음.
	 @param v2 복합종성의 두번째 자음.
	 @return 조합된 복합종성의 유니 코드.
	 */
	static int getJongseongPair(int v1, int v2);

    /**
     * appendChar에서 처리된 현재 입력한 글자 상태에 따라 mHanguleCharBuffer 배열을 조합하여 완성형 한글 코드를 생성.
     @author jeonguklee
     @date 2015-07-06
     @param state       현재 입력된 글자 상태 (초성, 중성, 종성)
     @param primaryCode 현재 입력된 한글의 자소 코드 (조합형 한글 코드)
     @param kind        현재 입력된 글자의 자소 종류(자음, 모음)
     @return 0반환(IAutomata::RETURN_DEFAULT_ACTION)
     */
    int saveCharacter(int state, int primaryCode, int kind);

    /**
     * 현재 중성 상태가 점인지 쌍점인지 판별.
     @author jeonguklee
     @date 2015-07-06
     @param 현재 중성 코드.
     @return 점, 쌍점이라면 true
     */
    static bool isDot(int v1);


	static const int MAX_COMPOSING_LENGTH = 60; // 단종음 상태 종료

	static const unsigned short UnicodeHanGulStart = 0xAC00;
	static const unsigned short UnicodeHanGulLast = 0xD7A3;  //0xD79F;
	static const unsigned short UnicodeHanGulJamoStart = 0x3131;
	static const unsigned short UnicodeHanGulJamoLast = 0x318E;

//	static const int HANGUL_DOT = 0x318d; ///< 천지인의 천(점)
//	static const int HANGUL_DOUBLE_DOT = 0x11A2; ///< 쌍점
	static const int HANGUL_DOT = L'ㆍ'; ///< 천지인의 천(점)
	static const int HANGUL_DOUBLE_DOT = L'ᆢ'; ///< 쌍점

	static const int HANGUL_JA = 0;
	static const int HANGUL_MO = 1;
	static const int HANGUL_FN = 2;

	static const int HANGUL_NONE = -1;
	static const int HANGUL_CHO1 = 0; // 단초음
	static const int HANGUL_JUNG1 = 1; // 단모음
	static const int HANGUL_JONG1 = 2; // 단종음
	static const int HANGUL_JONG2 = 3; // 복종음
	static const int HANGUL_JONG1_ABLE = 4; // 앞글자가 모음으로 끝나고, 뒤에오는 자음이 토글로 인해 앞자의 받침이 될 수 있는 상태
	static const int HANGUL_JONG2_ABLE = 5; // 앞글자가 종음으로 끝나고, 뒤에오는 자음이 토글로 인해 앞자의 복종음이 될 수 있는 상태
	static const int HANGUL_CHO1_FINISH = 6; // 단초음 상태 종료
	static const int HANGUL_JUNG1_FINISH = 7; // 단모음 상태 종료
	static const int HANGUL_JONG1_FINISH = 8; // 단종음 상태 종료
	static const int HANGUL_JONG2_FINISH = 9; // 복종음 상태 종료
	static const int HANGUL_JONG1_ABLE_FINISH = 10; // 단종음 가능상태 종료
	static const int HANGUL_JONG2_ABLE_FINISH = 11; // 복종음 가능상태 종료
//	static const int HANGUL_NOT_WORK = 12; ///< 복합종성 가능상태 종료
	static const int HANGUL_JUNG_DOT = 12; ///< .
	static const int HANGUL_TIME_COMPOSE = 13;

	static const wchar_t PREF_CHO[19]; ///< ㄱ ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
	static const wchar_t PREF_JUNG[21]; ///< ㅏ ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ
	static const wchar_t PREF_JONG[27]; ///< ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ

protected:

	wstring mCGIMo; ///< 입력한 키코드를 모음으로 분별하기 위한 모음 전체 집합.
	wstring mCGIJa; ///< 입력한 키코드를 자음으로 분별하기 위한 자음 전체 집합.
	wstring mChoseong; ///< makeHangul시 필요한 초성 비교를 위한 자음 집합
	wstring mJungseong; ///< makeHangul시 필요한 중성 비교를 위한 모음 집합

	bool isHangle;
	bool isTimeToggle;

	int mCurrentState;  ///< 현재 입력된 자소 상태
	int mHangulCharBuffer[4]; ///<작성중인 한글 한 자를 처리하기 위한 입력 버퍼
	int mWorkingChar; ///<현재 작성중인 한글
	int mWorkingChar2; ///<현재 작성중인 한글의 예외적인 처리를 위한 변수

};
