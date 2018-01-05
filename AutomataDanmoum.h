/*
 * AutomataDanmoum.h
 *
 *  Created on: 2015. 11. 5.
 *      Author: kabaek
 */

#ifndef JNI_SRC_AUTOMATA_LANGS_KO_KR_AUTOMATADANMOUM_H_
#define JNI_SRC_AUTOMATA_LANGS_KO_KR_AUTOMATADANMOUM_H_

#include "Automata.h"
namespace automata {
    namespace langs {
        namespace ko_kr {
        /**
         * 한글단모음 키패드 한글 오토마타 구현 클래스
         @file AutomataDanmoum.h
         @author kwiae
         @date 2015-11-05
         */
class AutomataDanmoum : public Automata {

public:
    /**
     * 사용자가 입력한 한글키의 유니코드를 구분하여 현재 상태를 나타냄.
     @author kwiae
     @date 2015-11-06
     @param unicode 입력한 키의 유니코드
     @return saveCharater()함수 호출반환 (0 반환)
     */
    virtual int appendChar(const wchar_t unicode);

    /**
     * 사용자가 작성중인 텍스트를 오토마타에 의해 자소별로 지움.
     @author kwiae
     @date 2015-11-06
     */
    virtual void deleteChar();

    virtual int appendSeparator(wchar_t unicode);

private:
    static const wchar_t PREF_MO_HEAD[14];
    static const wchar_t PREF_JA_HEAD[19];
    static const wchar_t PREF_JA[5][2];
    static const wchar_t PREF_MO[6][2];

public:
    AutomataDanmoum(); ///< AutomataCheonJiIn 생성자

    /**
       * 전달된 인자가 자음 키패드 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
       @author kwiae
       @date 2015-11-06
       @param 한글 자소 코드.
       @return 자음 키패드 그룹 행렬의 행 인덱스 반환.(같은 그룹)
       */
    static int getGroupNumJa(int v1);

    /**
       * 전달된 인자가 모음 키패드 그룹에 속한다면 속해있는 그룹의 행 인덱스 반환.
       @author kwiae
       @date 2015-11-06
       @param 한글 자소 코드.
       @return 모음 키패드 그룹 행렬의 행 인덱스 반환.(같은 그룹)
       */
    static int getGroupNumMo(int v1);

    /**
     * 현재 작성중인 글자와 사용자가 입력한 자음이 토글이 가능한지 체크.
     @author kwiae
     @date 2015-11-06
     @param v1 현재 작성중인 글자 코드.
     @param v2 사용자가 입력한 글자 코드.
     @return 토글이 가능하다면 1 반환
     */
    static int isToggleJa(int v1, int v2);

    /**
     * 현재 작성중인 글자와 사용자가 입력한 모음이 토글이 가능한지 체크.
     @author kwiae
     @date 2015-11-06
     @param v1 현재 작성중인 글자 코드.
     @param v2 사용자가 입력한 글자 코드.
     @return 토글이 가능하다면 1 반환
     */
    static bool isToggleMo(int v1, int v2);

    /**
     * 입력한 모음을 각각 해당하는 자음 토글 그룹에 맞게 토글.
     @author kwiae
     @date 2015-11-06
     @param 사용자가 입력한 글자 코드.
     @return 토글된 자음 글자 코드 반환.
     */
    static int setToggleJa(int v1);

    /**
     * 입력한 모음을 각각 해당하는 모음 토글 그룹에 맞게 토글.
     @author kwiae
     @date 2015-11-06
     @param 사용자가 입력한 글자 코드.
     @return 토글된 모음 글자 코드 반환.
     */
    static int setToggleMo(int v1);

private :
    /**
     * 입력한 글자가 복합중성이 가능한지 체크.
     @author kwiae
     @date 2015-11-06
     @param 사용자가 입력한 글자 코드.
     @return 복합중성으로 조합이 가능하다면 true
     */
    bool isJungPair(int Code);

    /**
     * 입력한 중성에 대해서 모음을 조합하여 복합중성으로 만듦.
     @author kwiae
     @date 2015-11-06
     @param 사용자가 입력한 글자 코드.
     @return 조합된 복합 종성 코드.
     */
    int addJung(int Code);

};

        }
    }
}




#endif /* JNI_SRC_AUTOMATA_LANGS_KO_KR_AUTOMATADANMOUM_H_ */
