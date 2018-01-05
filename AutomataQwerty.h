#ifndef AUTOMATAQWERTY_H
#define AUTOMATAQWERTY_H

#include "Automata.h"
using namespace std;

namespace automata {
    namespace langs {
        namespace ko_kr {
        /**
         * 쿼티 키패드 한글 오토마타 구현 클래스
         @file AutomataQwerty.h
         @author jeonguklee
         @date 2015-07-06
         */
class AutomataQwerty : public BaseAutomata {

public:
	/**
	 * 전달된 인자가 한글인지 검사.
	 @author jeonguklee
	 @date 2015-07-06
	 @param 사용자가 입력한 문자 코드.
	 @return 한글이라면 true 반환.
	 */
	bool isHangul(wchar_t const primaryCode);

    /**
     @author jhkim
     @date 2015-07-06
     @brief 키보드 앱에서 터치된 Suggestion/Prediction 된 단어를 선택 했을 때, 호출되는 함수
     @param *predictionWord 선택한 단어의 문자열
     */
    virtual void selectPrediction(wstring* predictionWord);

    /**
     * 사용자가 커서를 옮겼을때 커서를 옮긴 지점의 앞뒤 글자와 상태를 가져옴.
     @author jeonguklee
     @date 2015-07-06
     @param beforeString 커서 뒤의 문자열을 가져옴.
     @param afterString  커서 앞의 문자열을 가져옴.
     */
    virtual void cursorChange(wstring beforeString, wstring afterString);
    /**
     * 사용자가 입력한 한글키의 유니코드를 구분하여 현재 상태를 나타냄.
     @author jeonguklee
     @date 2015-07-06
     @param unicode 입력한 키의 유니코드
     @return saveCharater()함수 호출반환 (0 반환)
     */
    virtual int appendChar(const wchar_t wc);
    /**
     * 사용자가 글자를 작성한 후 입력하는 구두점 및 띄어쓰기 처리.
     @author jeonguklee
     @date 2015-07-06
     @param unicode Separator처리를 위한 사용자가 입력한 유니코드 '.',   ',',   ' ',   '\n'
     @return Automata::appendSeparator()함수 호출 (N-gram과 composingWord, composingWord 처리 후 0 반환)
     */
    virtual int appendSeparator(const wchar_t wc);
    /**
     * 문자열을 입력하게 될 때 composingWord와 ComposingWord에 붙여넣음.
     @author jeonguklee
     @date 2015-07-06
     @param unicode 입력한 키의 유니코드
     @return 입력된 문자열의 길이가 1이라면 appendChar()호출 반환
     */
    virtual int appendText(const wstring &s);
    /**
     * 사용자가 작성중인 텍스트를 오토마타에 의해 자소별로 지움.
     @author jeonguklee
     @date 2015-07-06
     */
    virtual void deleteChar();
    virtual void deleteCharComposing();

    /**
     * 현재 오토마타 상태와 입력된 글자들을 초기화.
     @author jeonguklee
     @date 2015-07-06
     */
    virtual void reset();
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
//    /**
//     * 사용자가 Prediction 글자를 선택하였을 때 선택한 글자를 N-gram에 추가.
//     @author jeonguklee
//     @date 2015-07-06
//     @param 사용자가 선택한 prediction 글자.
//     */
//    virtual void selectPrediction(wstring* predictionWord);

    /**
	 * 문자열과 키보드앱에 그려질 문자열을 모두 삭제 및 상태 초기화
	 @author jeonguklee
	 @date 2015-07-20
	 */
    virtual void deleteAllChar();

    // 상태
private:
    int mSeparatedChar[3]; ///<작성중인 한글 한 자를 처리하기 위한 입력 버퍼
    int mCurrentState; ///< 현재 입력된 자소 상태
    int mWorkingChar; ///<현재 작성중인 한글

    // 상수
protected:
    static const wchar_t ARRAY_CHOSEONGS[];
    static const std::wstring STRING_CHOSEONGS;
    static const wchar_t ja_g  ;
    static const wchar_t ja_gg ;
    static const wchar_t ja_n  ;
    static const wchar_t ja_d  ;
    static const wchar_t ja_dd ;
    static const wchar_t ja_l  ;
    static const wchar_t ja_m  ;
    static const wchar_t ja_b  ;
    static const wchar_t ja_bb ;
    static const wchar_t ja_s  ;
    static const wchar_t ja_ss ;
    static const wchar_t ja_o  ;
    static const wchar_t ja_j  ;
    static const wchar_t ja_jj ;
    static const wchar_t ja_ch ;
    static const wchar_t ja_k  ;
    static const wchar_t ja_t  ;
    static const wchar_t ja_p  ;
    static const wchar_t ja_h  ;

    static const wchar_t ARRAY_JUNGSEONGS[];
    static const std::wstring STRING_JUNGSEONGS;
    static const wchar_t mo_a   ;
    static const wchar_t mo_ae  ;
    static const wchar_t mo_ya  ;
    static const wchar_t mo_yae ;
    static const wchar_t mo_eo  ;
    static const wchar_t mo_e   ;
    static const wchar_t mo_yeo ;
    static const wchar_t mo_ye  ;
    static const wchar_t mo_o   ;
    static const wchar_t mo_wa  ;
    static const wchar_t mo_wae ;
    static const wchar_t mo_woi ;
    static const wchar_t mo_yo  ;
    static const wchar_t mo_woo ;
    static const wchar_t mo_weo ;
    static const wchar_t mo_we  ;
    static const wchar_t mo_wi  ;
    static const wchar_t mo_yu  ;
    static const wchar_t mo_eu  ;
    static const wchar_t mo_eui ;
    static const wchar_t mo_i   ;

    static const wchar_t ARRAY_JONGSEONGS[];
    static const std::wstring STRING_JONGSEONGS;
    static const wchar_t ja_gs;
    static const wchar_t ja_nj;
    static const wchar_t ja_nh;
    static const wchar_t ja_lg;
    static const wchar_t ja_lm;
    static const wchar_t ja_lb;
    static const wchar_t ja_ls;
    static const wchar_t ja_lt;
    static const wchar_t ja_lp;
    static const wchar_t ja_lh;
    static const wchar_t ja_bs;

    static const int KIND_NONE                  ; ///<  ///< 입력 키의 종류 구분. 한글x
    static const int KIND_JA                    ; ///< 입력 키의 종류 구분. 자음 = 0
    static const int KIND_MO                    ; ///< 입력 키의 종류 구분. 모음 = 1

    static const int STATE_EMPTY                ; ///< 입력 상태가 비어있음.
    static const int STATE_1_CHO                ; ///< 초성     ㄱ
    static const int STATE_1_JUNG_SINGLE        ; ///< 단일중성 ㅓ
    static const int STATE_1_JUNG_PAIR          ; ///< 복합중성 ㅘ
    static const int STATE_2_JUNG_SINGLE        ; ///< 단일중성 가
    static const int STATE_2_JUNG_PAIR          ; ///< 복합중성 과
    static const int STATE_3_JONG_SINGLE        ; ///< 단일종성 곽
    static const int STATE_3_JONG_PAIR          ; ///< 복합종성 괅
    static const int STATE_FIN_AND_1_CHO        ; ///< 초성 상태 종료
    static const int STATE_FIN_AND_1_JUNG_SINGLE; ///< 중성 상태 종료
    static const int STATE_FIN_AND_2_JUNG_SINGLE; ///< 중성 상태 종료

    // ------------------------------------------------------------
public:
    AutomataQwerty(); ///< AutomataQwerty 생성자

public:
    /**
	 * 글자를 지울때 앞글자를 초중종성으로 분해하여 정보를 가져오기 위한 함수.
	 @author jeonguklee
	 @date 2015-07-20
	 @param1 primaryCode 분해 할 한글 코드
	 @param2 state 		 한글을 분해한 글자 상태
	 @param3 buffer		 분해 한 한글 코드를 저장할 한글입력버퍼
	 */
    virtual void setBuffer(wchar_t const primaryCode, int &state, int *buffer);

private:
    /**
     * appendChar에서 처리된 현재 입력한 글자 상태에 따라 mHanguleCharBuffer 배열을 조합하여 완성형 한글 코드를 생성.
     @author jeonguklee
     @date 2015-07-06
     @param state       현재 입력된 글자 상태 (초성, 중성, 종성)
     @param primaryCode 현재 입력된 한글의 자소 코드 (조합형 한글 코드)
     @param kind        현재 입력된 글자의 자소 종류(자음, 모음)
     @return 0반환(IAutomata::RETURN_DEFAULT_ACTION)
     */
    virtual int saveCharacter(int const state, wchar_t const primaryCode);
    /**
     * 한글이 아닌 글자를 현재 작업중인 문자열에 저장.
     @author jeonguklee
     @date 2015-07-06
     @param state       현재 입력된 글자 상태 (초성, 중성, 종성)
     @param primaryCode 현재 입력된 한글의 자소 코드 (조합형 한글 코드)
     */
    void saveUnknownCharacter(int const state, wchar_t const primaryCode);
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

public:
private:
    /**
     * 전달된 인자가 조합이 가능한 중성인지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param j1 현재 작성중인 중성 코드.
     @param j2 사용자가 입력한 한글 코드.
     @return 분해가 가능하다면 true.
     */
    bool isCombinedJungseong(wchar_t const j1, wchar_t const j2);
    /**
     * 전달된 인자가 조합이 가능한 종성인지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param j1 현재 작성중인 종성 코드.
     @param j2 사용자가 입력한 한글 코드.
     @return 분해가 가능하다면 true.
     */
    bool isCombinedJongseong(wchar_t const j1, wchar_t const j2);
    /**
     * 전달된 인자가 분해가 가능한 복합중성인지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param j1 현재 작성중인 중성 코드.
     @return 분해가 가능하다면 true.
     */
    bool isCombinedJungseong(wchar_t const j);
    /**
     * 전달된 인자가 분해가 가능한 복합종성인지 체크.
     @author jeonguklee
     @date 2015-07-06
     @param j1 현재 작성중인 종성 코드.
     @return 분해가 가능하다면 true.
     */
    bool isCombinedJongseong(wchar_t const j);

    /**
	 @author jhkim
	 @date 2015-07-06
	 @brief 키보드 앱 TextBox에 그려질 문자열을 반환
	 @return ComposingWord 리턴
	 */
	wstring getComposingWord();

private:
    /**
     * 전달된 인자를 복합중성으로 결합.
     @author jeonguklee
     @date 2015-07-06
     @param j1 현재 작성중인 중성 코드.
     @param j2 사용자가 입력한 한글 코드
     @return 조합한 복합중성 코드 반환.
     */
    int makeCombinedJungseong(wchar_t const j1, wchar_t const j2);
    /**
     * 전달된 인자를 복합종성으로 결합.
     @author jeonguklee
     @date 2015-07-06
     @param j1 현재 작성중인 종성 코드.
     @param j2 사용자가 입력한 한글 코드
     @return 조합한 복합종성 코드 반환.
     */
    int makeCombinedJongseong(wchar_t const j1, wchar_t const j2);
    /**
     * 입력한 자소가 초성 인덱스에서 몇번째인지 위치값 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 입력된 조합형 한글 코드.
     @return 매개변수의 초성 인덱스값 반환
     */
    int getChoseongIndex(wchar_t const primaryCode);
    /**
     * 입력한 자소가 중성 인덱스에서 몇번째인지 위치값 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 입력된 조합형 한글 코드.
     @return 매개변수의 중성 인덱스값 반환
     */
    int getJungseongIndex(wchar_t const primaryCode);
    /**
     * 입력한 자소가 종성 인덱스에서 몇번째인지 위치값 반환.
     @author jeonguklee
     @date 2015-07-06
     @param 입력된 조합형 한글 코드.
     @return 매개변수의 종성 인덱스값 반환
     */
    int getJongseongIndex(wchar_t const primaryCode);

    /**
     * 사용자가 입력한 글자 하나를 조합하여 완성형 유니코드로 값 반환.
     @author jeonguklee
     @date 2015-07-06
     @param mHangulCharBuffer가 인자로 전달되어 한글 조합이 이루어짐.
     @return 완성형 한글 유니코드 반환.
     */
    int makeHangul(int slicedPieces[]);


    /**
     * 결합된 복합중성을 분해.
     @author jeonguklee
     @date 2015-07-06
     @param j   분해할 중성 코드.
     @param ret 복합중성을 저장하여 반환할 매개변수
     @return 복합중성을 분해하여 각각의 중성 코드로 반환.
     */
    int *separateCombinedJungseong(int const j, int *ret);
    /**
      * 작업중인 한글 버퍼를 비움.
      @author jeonguklee
      @date 2015-07-06
      */
    void clearSeparatedChar();

protected:
    /**
     * 결합된 복합종성을 분해.
     @author jeonguklee
     @date 2015-07-06
     @param j   분해할 종성 코드.
     @param ret 복합종성을 저장하여 반환할 매개변수
     @return 복합종성을 분해하여 각각의 종성 코드로 반환.
     */
    int *separateCombinedJongseong(int const j, int *ret);



};

        }
    }
}

#endif /* AUTOMATAQWERTY_H */
