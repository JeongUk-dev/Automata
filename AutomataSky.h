#ifndef AUTOMATASKY_H
#define AUTOMATASKY_H

#include "Automata.h"
namespace automata {
    namespace langs {
        namespace ko_kr {
        /**
         * 스카이 키패드 한글 오토마타 구현 클래스
         @file AutomataSky.h
         @author jeonguklee
         @date 2015-07-06
         */
class AutomataSky : public Automata {

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



    ///< 자음 그룹
private:
    static const wchar_t PREF_JA[7][3]; ///< ㄱ,ㅋ,ㄲ
    static const wchar_t PREF_MO[5][3]; ///< ㅏㅑ

    ///< 상수
    ///< ㄱ, ㄴ, ㄷ, ㅁ, ㅂ, ㅇ, ㅌ
    static const wchar_t PREF_JA_HEAD[19]; ///< ㄱ
    ///< ㅏ ㅓ ㅗ ㅜ ㅡ
    static const wchar_t PREF_MO_HEAD[11]; ///< ㅏ

    static const wchar_t Double_Jeongseong_Single[]; //조합중성에 분해를 위한 배열


public:
    AutomataSky(); ///< AutomataSky 생성자

private:

    /**
     * 전달된 인자가 분해가 가능한 단중성인지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param 입력된 조합형 한글 코드.
     @return 분해가 가능하다면 true.
     */
    static bool isCombinedJungseongSingle(int v);

public:

     /**
      * 현재 작성중인 자음과 사용자가 입력한 글자가 토글이 가능한지 체크.
      @author jeonguklee
      @date 2015-07-06
      @param v1 현재 작성중인 글자 코드.
      @param v2 사용자가 입력한 글자 코드.
      @return 토글이 가능하다면 1 반환
      */
    static bool isToggleJa(int v1, int v2);
    /**
     * 전달된 인자가 자음 키패드 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 자소 코드.
     @return 자음 키패드 그룹 행렬의 행 인덱스 반환.(같은 그룹)
     */
    static int getGroupNumJa(int v1);
    /**
     * 입력한 자음을 각각 해당하는 자음 토글 그룹에 맞게 토글.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 토글된 자음 글자 코드 반환.
     */
    static int setToggleJa(int v1);
    /**
     * 현재 작성중인 모음과 사용자가 입력한 글자가 토글이 가능한지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param v1 현재 작성중인 글자 코드.
     @param v2 사용자가 입력한 글자 코드.
     @return 토글이 가능하다면 1 반환
     */
    static bool isToggleMo(int v1, int v2);
    /**
     * 전달된 인자가 모음 키패드 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 자소 코드.
     @return 자음 키패드 그룹 행렬의 행 인덱스 반환.(같은 그룹)
     */
    static int getGroupNumMo(int v1);
    /**
     * 입력한 모음을 각각 해당하는 모음 토글 그룹에 맞게 토글.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 토글된 모음 글자 코드 반환.
     */
    static int setToggleMo(int v1);


private:
    /**
     * 입력한 글자가 복합중성이 가능한지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 복합중성으로 조합이 가능하다면 true
     */
    bool isJungPair(int Code);
    /**
     * 입력한 중성에 대해서 모음을 조합하여 복합중성으로 만듦.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 조합된 복합 종성 코드.
     */
    int addJung(int Code);

public:
    /**
     * 전달된 두 인자중 하나가 획추가나 쌍자음 그룹에 속하는지 판별 후 복합종성이 가능한지 확인.
     @author jeonguklee
     @date 2015-07-06
     @param v1 현재 mHangulCharBuffer에 구성되어 있는 종성.
     @param v2 사용자가 입력한 자음.
     @return 복합종성인지 판별하여 복합종성이라면 1 반환
     */
    static bool isJongseongPairAble(int v1, int v2);
};

        }
    }
}
#endif /* AUTOMATASKY_H */
