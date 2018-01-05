#ifndef AUTOMATANARAGUL_H
#define AUTOMATANARAGUL_H

#include "Automata.h"
namespace automata {
    namespace langs {
        namespace ko_kr {


/**
 * 나랏글 키패드 한글 오토마타 구현 클래스
 @file AutomataNaragul.h
 @author jeonguklee
 @date 2015-07-06
 */
class AutomataNaragul : public Automata {


public:
    /**
     * 사용자가 입력한 한글키의 유니코드를 구분하여 현재 상태를 나타냄.
     @author jeonguklee
     @date 2015-07-06
     @param unicode 입력한 키의 유니코드
     @return saveCharater()함수 호출반환 (0 반환)
     */
    virtual int appendChar(wchar_t unicode);
    /**
     * 사용자가 작성중인 텍스트를 오토마타에 의해 자소별로 지움.
     @author jeonguklee
     @date 2015-07-06
     */
    virtual void deleteChar();

private:
    /**
     * 모음 키버튼의 대표값 집합.
     * 'ㅏ', 'ㅗ', 'ㅡ', 'ㅣ'
     @author jeonguklee
     @date 2015-07-06
     */
    static const /*int*/wchar_t PREF_MO_HEAD[6];
    /**
     * 자음 키버튼의 대표값 집합.
     * 'ㄱ', L'ㄴ', L'ㄹ', L'ㅁ', L'ㅅ', L'ㅇ'
     @author jeonguklee
     @date 2015-07-06
     */
    static const /*int*/wchar_t PREF_JA_HEAD[6];

public:

    static const int ADD_LINE = 0x318d; ///< 획추가 버튼 정의 코드.
    static const int DOUBLE_CHAR = 0x318e; ///< 쌍자음 버튼 정의 코드.

private:

    static const wchar_t PREF_ADD_LINE[9][3]; ///<획추가 버튼을 눌렀을때 토글 처리하기 위한 자소 집합.
    static const wchar_t PREF_DOUBLE_CHAR[5][2]; ///<쌍자음 버튼을 눌렀을때 토글 처리하기 위한 자음 집합.
    static const wchar_t PREF_JA[6][5];


public:

private:
//    int mCurrentState;  ///< 현재 입력된 자소 상태
//    int mHangulCharBuffer[4]; ///<작성중인 한글 한 자를 처리하기 위한 입력 버퍼
//    int mWorkingChar; ///<현재 작성중인 한글
//    int mWorkingChar2; ///<현재 작성중인 한글의 예외적인 처리를 위한 변수
    bool mLastFunctionKey; ///<입력중에 기능키를 입력하였는지 체크.

public:
    AutomataNaragul();  ///<AutomataNaragul 생성자

    /**
     * 전달된 인자가 획추가 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 자소 코드.
     @return 획추가 자소의 행렬의 행 인덱스 반환.(같은 그룹)
     */
    static int getAddGroup(int v1);
    /**
     * 전달된 인자가 쌍자음 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 자소 코드.
     @return 쌍자음 자소의 행렬의 행 인덱스 반환.(같은 그룹)
     */
    static int getDoubleGroup(int v1);
    /**
     * 전달된 두 인자중 하나가 획추가나 쌍자음 그룹에 속하는지 판별 후 복합종성이 가능한지 확인.
     @author jeonguklee
     @date 2015-07-06
     @param v1 현재 mHangulCharBuffer에 구성되어 있는 종성.
     @param v2 사용자가 입력한 자음.
     @return 복합종성인지 판별하여 복합종성이라면 1 반환
     */
    static int isJongseongPairAble(int v1, int v2);
    /**
     * 입력한 키버튼이 획추가나 쌍자음 기능키인지 판별.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 키코드.
     @return 입력한 키버튼이 기능키라면 true
     */
    static bool isFunctionKey(int primaryCode);

    /**
     * 현재 작성중인 글자와 사용자가 입력한 글자가 토글이 가능한지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param v1 현재 작성중인 글자 코드.
     @param v2 사용자가 입력한 글자 코드.
     @return 토글이 가능하다면 1 반환
     */
    static int isToggle(int v1, int v2);

    /**
     * 전달된 인자가 자음 키패드 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 자소 코드.
     @return 자음 키패드 그룹 행렬의 행 인덱스 반환.(같은 그룹)
     */
    static int getJaGroup(int v1);

private:
    /**
     * 입력한 자소에서 획추가나 쌍자음 기능키를 눌렀을때 처리되는 과정.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 키코드.
     @return 입력한 기능키에 따라 각각 addLine(), doubleChar()함수 호출.
     */
    int setCode(int primaryCode);
    /**
     * 획추가 버튼이 입력되었을때 처리되는 함수.
     @author jeonguklee
     @date 2015-07-06
     @return addLine(code)함수 호출 반환.
     */
    int addLine();
    /**
     *  쌍자음 버튼이 입력되었을때 처리되는 함수.
     @author jeonguklee
     @date 2015-07-06
     @return doubleChar(code)함수 호출 반환.
     */
    int doubleChar();
    /**
     * 획추가가 가능한 자소에 대해 획추가한 자소 값을 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 초성 중성 종성에 대한 자소 값.
     @return 획추가 되어진 자소 코드 반환.
     */
    int addLine(int Code);
    /**
     * 쌍자음이 가능한 자음에 대해 쌍자음 변환한 자음 값을 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 초성  종성에 대한 자음 값.
     @return 획추가 되어진 자음 코드 반환.
     */
    int dobuleChar(int Code);
    /**
     * 입력한 중성에 대해서 모음을 조합하여 복합중성으로 만듦.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 조합된 복합 종성 코드.
     */
    int addJung(int Code);
    /**
     * 입력한 중성이 토글이 가능한지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 종성 코드.
     @return 0:ㅏ ㅓ ㅑ ㅕ, 1: ㅗ ㅜ ㅛ ㅠ
     */
    int getJungToggleGroup(int Code);
    /**
     * 입력되어진 중성을 토글하여 반환한다.
     @author jeonguklee
     @date 2015-07-06
     @return 토글된 중성을 반환한다.
     */
    int setJungToggle();
    /**
     * 입력한 글자가 복합중성이 가능한지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 복합중성으로 조합이 가능하다면 true
     */
    bool isJungPair(int Code);
    
};

        }
    }
}


#endif /* AUTOMATANARAGUL_H */
