#ifndef AUTOMATACHEONJIIN_H
#define AUTOMATACHEONJIIN_H

#include "Automata.h"
namespace automata {
    namespace langs {
        namespace ko_kr {
        /**
         * 천지인 키패드 한글 오토마타 구현 클래스
         @file AutomataCheonJiIn.h
         @author jeonguklee
         @date 2015-07-06
         */
class AutomataCheonJiIn : public Automata {

public:
    /**
     * 사용자가 입력한 한글키의 유니코드를 구분하여 현재 상태를 나타냄.
     @author jeonguklee
     @date 2015-07-06
     @param unicode 입력한 키의 유니코드
     @return saveCharater()함수 호출반환 (0 반환)
     */
    virtual int appendChar(const wchar_t unicode);

    /**
     * 사용자가 작성중인 텍스트를 오토마타에 의해 자소별로 지움.
     @author jeonguklee
     @date 2015-07-06
     */
    virtual void deleteChar();

    // 천지인의 모음을 결정하는 코드 ㅣ, . , ㅡ
private:
    static const wchar_t PREF_MO_HEAD[3]; ///< 0x2025
    static const wchar_t PREF_JA_HEAD[19];
    static const wchar_t PREF_JA[7][3];

public:


private:


public:

private:
//    int mCurrentState;  ///< 현재 입력된 자소 상태
//    int mHangulCharBuffer[4]; ///<작성중인 한글 한 자를 처리하기 위한 입력 버퍼
//    int mWorkingChar; ///<현재 작성중인 한글
//    int mWorkingChar2; ///<현재 작성중인 한글의 예외적인 처리를 위한 변수

public:
    AutomataCheonJiIn(); ///< AutomataCheonJiIn 생성자

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
     * 입력한 글자를 각각 해당하는 그룹에 맞게 토글.
     @author jeonguklee
     @date 2015-07-06
     @param 사용자가 입력한 글자 코드.
     @return 토글된 글자 코드 반환.
     */
    static int setToggle(int v1);
    /**
     * 입력한 글자가 조합이나 복합중성이 가능한지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param v1 현재 중성 코드.
     @param v2 사용자가 입력한 글자 코드.
     @return 복합중성으로 조합이 가능하다면 true
     */
    static bool isAddJung(int v1, int v2);
    /**
     * 입력된 코드에 따라 현재 중성을 복합중성이나 조합단중성 코드 반환.
     @author jeonguklee
     @date 2015-07-06
     @param v1 입력되어있는 중성코드.
     @param v2 사용자가 새로 입력한 한글 코드.
     @return 조합된 중성 코드 반환
     */
    static int setJung(int v1, int v2);
    /**
     * 전달된 인자가 자음 키패드 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 한글 자소 코드.
     @return 자음 키패드 그룹 행렬의 행 인덱스 반환.(같은 그룹)
     */
    static int getJaGroup(int v1);
    /**
     * 전달된 두 인자중 하나가 자음 그룹에 속하는지 판별 후 복합종성이 가능한지 확인.
     @author jeonguklee
     @date 2015-07-06
     @param v1 현재 mHangulCharBuffer에 구성되어 있는 종성.
     @param v2 사용자가 입력한 자음.
     @return 복합종성인지 판별하여 복합종성이라면 1 반환
     */
    static int isJongseongPairAble(int v1, int v2);

};

        }
    }
}
#endif /* AUTOMATACHEONJIIN_H */
