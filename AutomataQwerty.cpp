#include "AutomataQwerty.h"

#include "../../../common/ErrorDefine.h"
#include "../../../utils/LogUtils.h"
#include "../../../utils/Utils.h"
#include "../../../common/Const.h"
#include "../../../Defines.h"



using namespace std;

namespace automata {
    namespace langs {
        namespace ko_kr {

const wchar_t AutomataQwerty::ARRAY_CHOSEONGS[] = {L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};
const std::wstring AutomataQwerty::STRING_CHOSEONGS = std::wstring(ARRAY_CHOSEONGS, sizeof(ARRAY_CHOSEONGS) / sizeof(ARRAY_CHOSEONGS[0]));
const wchar_t AutomataQwerty::ja_g =  L'ㄱ';
const wchar_t AutomataQwerty::ja_gg = L'ㄲ';
const wchar_t AutomataQwerty::ja_n =  L'ㄴ';
const wchar_t AutomataQwerty::ja_d =  L'ㄷ';
const wchar_t AutomataQwerty::ja_dd=  L'ㄸ';
const wchar_t AutomataQwerty::ja_l =  L'ㄹ';
const wchar_t AutomataQwerty::ja_m =  L'ㅁ';
const wchar_t AutomataQwerty::ja_b =  L'ㅂ';
const wchar_t AutomataQwerty::ja_bb = L'ㅃ';
const wchar_t AutomataQwerty::ja_s =  L'ㅅ';
const wchar_t AutomataQwerty::ja_ss = L'ㅆ';
const wchar_t AutomataQwerty::ja_o =  L'ㅇ';
const wchar_t AutomataQwerty::ja_j =  L'ㅈ';
const wchar_t AutomataQwerty::ja_jj = L'ㅉ';
const wchar_t AutomataQwerty::ja_ch = L'ㅊ';
const wchar_t AutomataQwerty::ja_k =  L'ㅋ';
const wchar_t AutomataQwerty::ja_t =  L'ㅌ';
const wchar_t AutomataQwerty::ja_p =  L'ㅍ';
const wchar_t AutomataQwerty::ja_h =  L'ㅎ';

const wchar_t AutomataQwerty::ARRAY_JUNGSEONGS[] = {L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ'};
const std::wstring AutomataQwerty::STRING_JUNGSEONGS = std::wstring(ARRAY_JUNGSEONGS, sizeof(ARRAY_JUNGSEONGS) / sizeof(ARRAY_JUNGSEONGS[0]));
const wchar_t AutomataQwerty::mo_a   = L'ㅏ';
const wchar_t AutomataQwerty::mo_ae  = L'ㅐ';
const wchar_t AutomataQwerty::mo_ya  = L'ㅑ';
const wchar_t AutomataQwerty::mo_yae = L'ㅒ';
const wchar_t AutomataQwerty::mo_eo  = L'ㅓ';
const wchar_t AutomataQwerty::mo_e   = L'ㅔ';
const wchar_t AutomataQwerty::mo_yeo = L'ㅕ';
const wchar_t AutomataQwerty::mo_ye  = L'ㅖ';
const wchar_t AutomataQwerty::mo_o   = L'ㅗ';
const wchar_t AutomataQwerty::mo_wa  = L'ㅘ';
const wchar_t AutomataQwerty::mo_wae = L'ㅙ';
const wchar_t AutomataQwerty::mo_woi = L'ㅚ';
const wchar_t AutomataQwerty::mo_yo  = L'ㅛ';
const wchar_t AutomataQwerty::mo_woo = L'ㅜ';
const wchar_t AutomataQwerty::mo_weo = L'ㅝ';
const wchar_t AutomataQwerty::mo_we  = L'ㅞ';
const wchar_t AutomataQwerty::mo_wi  = L'ㅟ';
const wchar_t AutomataQwerty::mo_yu  = L'ㅠ';
const wchar_t AutomataQwerty::mo_eu  = L'ㅡ';
const wchar_t AutomataQwerty::mo_eui = L'ㅢ';
const wchar_t AutomataQwerty::mo_i   = L'ㅣ';

const wchar_t AutomataQwerty::ARRAY_JONGSEONGS[] = {L' ', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};
const std::wstring AutomataQwerty::STRING_JONGSEONGS = std::wstring(ARRAY_JONGSEONGS, sizeof(ARRAY_JONGSEONGS) / sizeof(ARRAY_JONGSEONGS[0]));
const wchar_t AutomataQwerty::ja_gs = L'ㄳ';
const wchar_t AutomataQwerty::ja_nj = L'ㄵ';
const wchar_t AutomataQwerty::ja_nh = L'ㄶ';
const wchar_t AutomataQwerty::ja_lg = L'ㄺ';
const wchar_t AutomataQwerty::ja_lm = L'ㄻ';
const wchar_t AutomataQwerty::ja_lb = L'ㄼ';
const wchar_t AutomataQwerty::ja_ls = L'ㄽ';
const wchar_t AutomataQwerty::ja_lt = L'ㄾ';
const wchar_t AutomataQwerty::ja_lp = L'ㄿ';
const wchar_t AutomataQwerty::ja_lh = L'ㅀ';
const wchar_t AutomataQwerty::ja_bs = L'ㅄ';

const int AutomataQwerty::KIND_NONE                   = -1;
const int AutomataQwerty::KIND_JA                     = 0 ;
const int AutomataQwerty::KIND_MO                     = 1 ;

const int AutomataQwerty::STATE_EMPTY                 = -1;
const int AutomataQwerty::STATE_1_CHO                 = 0 ;
const int AutomataQwerty::STATE_1_JUNG_SINGLE         = 1 ;
const int AutomataQwerty::STATE_1_JUNG_PAIR           = 2 ;
const int AutomataQwerty::STATE_2_JUNG_SINGLE         = 3 ;
const int AutomataQwerty::STATE_2_JUNG_PAIR           = 4 ;
const int AutomataQwerty::STATE_3_JONG_SINGLE         = 5 ;
const int AutomataQwerty::STATE_3_JONG_PAIR           = 6 ;
const int AutomataQwerty::STATE_FIN_AND_1_CHO         = 7 ;
const int AutomataQwerty::STATE_FIN_AND_1_JUNG_SINGLE = 8 ;
const int AutomataQwerty::STATE_FIN_AND_2_JUNG_SINGLE = 9 ;

AutomataQwerty::AutomataQwerty() {
    mWorkingChar = KIND_NONE;
    mCurrentState = 0;

    clearSeparatedChar();
}

int AutomataQwerty::appendChar(const wchar_t unicode) {

    LOGDWC("AutomataQwerty::appendChar() is called..., unicode = %s", unicode);

    //suggest_check = false;


    // 자/모음 구별
    //CharSequence * const csUnicode = StringConverterHelper::toString(unicode);
    wchar_t const csUnicode = unicode;

    int kind = KIND_NONE;
    if(STRING_CHOSEONGS.find(csUnicode) != wstring::npos) {
        kind = KIND_JA;
    } else if(STRING_JUNGSEONGS.find(csUnicode) != wstring::npos) {
        kind = KIND_MO;
    } else {
        saveUnknownCharacter(mCurrentState, unicode);
        return 0;
    }

    // 상태 전이
    switch(mCurrentState) {
    case STATE_EMPTY:
        if(kind == KIND_JA) {
            // ㄱ
            // ㄲ
            mCurrentState = STATE_1_CHO;
        } else {
            // ㅏ
            mCurrentState = STATE_1_JUNG_SINGLE;
        }
        break;

    case STATE_1_CHO:
        // ㄱ
        // ㄲ

        if(kind == KIND_JA) {
            // ㄱ, ㄱ -> ㄱ, ㄱ
            // ㄲ, ㄱ -> ㄲ, ㄱ
            mCurrentState = STATE_FIN_AND_1_CHO;
        } else {
            // ㄱ, ㅏ -> 가
            // ㄲ, ㅏ -> 까
            mCurrentState = STATE_2_JUNG_SINGLE;
        }
        break;

    case STATE_1_JUNG_SINGLE:
        // ㅏ

        if(kind == KIND_JA) {
            // ㅏ, ㄱ -> ㅏ, ㄱ
            mCurrentState = STATE_FIN_AND_1_CHO;
        } else {
            if(isCombinedJungseong(mSeparatedChar[1], unicode)) {
                // ㅗ, ㅏ -> ㅘ
                mCurrentState = STATE_1_JUNG_PAIR;
            } else {
                // ㅏ, ㅏ -> ㅏ, ㅏ
                mCurrentState = STATE_FIN_AND_1_JUNG_SINGLE;
            }
        }
        break;

    case STATE_1_JUNG_PAIR:
        // ㅘ

        if(kind == KIND_JA) {
            // ㅘ, ㄱ -> ㅘ, ㄱ
            mCurrentState = STATE_FIN_AND_1_CHO;
        } else {
            // ㅘ, ㅏ -> ㅘ, ㅏ
            mCurrentState = STATE_FIN_AND_1_JUNG_SINGLE;
        }
        break;

    case STATE_2_JUNG_SINGLE:
        // 가
        // 까

        if(kind == KIND_JA) {
            if(STRING_JONGSEONGS.find(csUnicode) != wstring::npos) {
                // 가, ㄱ -> 각
                // 까, ㄱ -> 깍
                mCurrentState = STATE_3_JONG_SINGLE;
            } else {
                // 가, ㅃ -> 가, ㅃ
                // 까, ㅃ -> 까, ㅃ
                mCurrentState = STATE_FIN_AND_1_CHO;
            }
        } else {
            if(isCombinedJungseong(mSeparatedChar[1], unicode)) {
                // 고, ㅏ -> 과
                // 꼬, ㅏ -> 꽈
                mCurrentState = STATE_2_JUNG_PAIR;
            } else {
                // 가, ㅏ -> 가, ㅏ
                // 까, ㅏ -> 까, ㅏ
                mCurrentState = STATE_FIN_AND_1_JUNG_SINGLE;
            }
        }
        break;

    case STATE_2_JUNG_PAIR:
        // 과
        // 꽈

        if(kind == KIND_JA) {
            if(STRING_JONGSEONGS.find(csUnicode) != wstring::npos) {
                // 과, ㄱ -> 곽
                // 꽈, ㄱ -> 꽉
                mCurrentState = STATE_3_JONG_SINGLE;
            } else {
                // 과, ㅃ -> 과, ㅃ
                // 꽈, ㅃ -> 꽈, ㅃ
                mCurrentState = STATE_FIN_AND_1_CHO;
            }
        } else {
            // 과, ㅏ -> 과, ㅏ
            // 꽈, ㅏ -> 꽈, ㅏ
            mCurrentState = STATE_FIN_AND_1_JUNG_SINGLE;
        }
        break;

    case STATE_3_JONG_SINGLE:
        // 각
        // 깍
        // 곽
        // 꽉

        if(kind == KIND_JA) {
            if(isCombinedJongseong(mSeparatedChar[2], unicode)) {
                // 각, ㅅ -> 갃
                // 깍, ㅅ -> 깏
                // 곽, ㅅ -> 곿
                // 꽉, ㅅ -> 꽋
                mCurrentState = STATE_3_JONG_PAIR;
            } else {
                // 각, ㄴ -> 각, ㄴ
                // 깍, ㄴ -> 깍, ㄴ
                // 곽, ㄴ -> 곽, ㄴ
                // 꽉, ㄴ -> 꽉, ㄴ
                mCurrentState = STATE_FIN_AND_1_CHO;
            }
        } else {
            // 각, ㅏ -> 가, 가
            // 깍, ㅏ -> 까, 가
            // 곽, ㅏ -> 과, 가
            // 꽉, ㅏ -> 꽈, 가
            mCurrentState = STATE_FIN_AND_2_JUNG_SINGLE;
        }
        break;

    case STATE_3_JONG_PAIR:
        // 갃
        // 깏
        // 곿
        // 꽋

        if(kind == KIND_JA) {
            // 갃, ㄱ -> 갃, ㄱ
            // 깏, ㄱ -> 깏, ㄱ
            // 곿, ㄱ -> 곿, ㄱ
            // 꽋, ㄱ -> 꽋, ㄱ
            mCurrentState = STATE_FIN_AND_1_CHO;
        } else {
            // 갃, ㅏ -> 각, 사
            // 깏, ㅏ -> 깍, 사
            // 곿, ㅏ -> 곽, 사
            // 꽋, ㅏ -> 꽉, 사
            mCurrentState = STATE_FIN_AND_2_JUNG_SINGLE;
        }
        break;
    }

    return saveCharacter(mCurrentState, unicode);
}

void AutomataQwerty::clearSeparatedChar() {
    LOGD(">> clearSeparatedChar");
    mSeparatedChar[0] = KIND_NONE;
    mSeparatedChar[1] = KIND_NONE;
    mSeparatedChar[2] = KIND_NONE;
}

void AutomataQwerty::selectPrediction(wstring* predictionWord) {
    BaseAutomata::selectPrediction(predictionWord);
    clearSeparatedChar();
}

int AutomataQwerty::saveCharacter(int const state, wchar_t const primaryCode) {

    LOGD("state = %d, primaryCode = %d", state, (int)primaryCode);

    int separateBuf[2];

    if(mWorkingChar != KIND_NONE) {
        // 한글코드가 아닌 문자는 조합의 상태가 아니므로 지워지면 안된다. (예 : 숫자)
        if(isHangul(mWorkingChar)) {
            currentWordDelete();
        }
    }

    int completedChar = KIND_NONE;
//    completedChar = makeHangul(mSeparatedChar);
//    LOGDWC("saveCharacter() completedChar = %s", completedChar);

    switch(state) {
        case STATE_1_CHO:
            // ㄱ
            // ㄲ
            mSeparatedChar[0] = primaryCode;
            mWorkingChar = mSeparatedChar[0];
            break;

        case STATE_1_JUNG_SINGLE:
            // ㅏ
            mSeparatedChar[1] = primaryCode;
            mWorkingChar = mSeparatedChar[1];
            break;

        case STATE_1_JUNG_PAIR:
            // ㅘ
            mSeparatedChar[1] = makeCombinedJungseong(mSeparatedChar[1], primaryCode);
            mWorkingChar = mSeparatedChar[1];
            break;

        case STATE_2_JUNG_SINGLE:
            // 가
            // 까
            mSeparatedChar[1] = primaryCode;
            mWorkingChar = makeHangul(mSeparatedChar);
            break;

        case STATE_2_JUNG_PAIR:
            // 과
            // 꽈
            mSeparatedChar[1] = makeCombinedJungseong(mSeparatedChar[1], primaryCode);
            mWorkingChar = makeHangul(mSeparatedChar);
            break;

        case STATE_3_JONG_SINGLE:
            // 각
            // 깍
            // 곽
            // 꽉
            mSeparatedChar[2] = primaryCode;
            mWorkingChar = makeHangul(mSeparatedChar);
            break;

        case STATE_3_JONG_PAIR:
            // 갃
            // 곿
            // 깏
            // 꽋
            mSeparatedChar[2] = makeCombinedJongseong(mSeparatedChar[2], primaryCode);
            mWorkingChar = makeHangul(mSeparatedChar);
            break;

        case STATE_FIN_AND_1_CHO:
            // ㄱ, ㄱ
            // ㄲ, ㄱ
            // ㅏ, ㄱ
            // ㅘ, ㄱ
            // 각, ㄴ
            // 깍, ㄴ
            // 곽, ㄴ
            // 꽉, ㄴ
            // 갃, ㄱ
            // 깏, ㄱ
            // 곿, ㄱ
            // 꽋, ㄱ
            completedChar = makeHangul(mSeparatedChar);
            clearSeparatedChar();
            mSeparatedChar[0] = primaryCode;
            mWorkingChar = mSeparatedChar[0];
            mCurrentState = STATE_1_CHO;
            break;

        case STATE_FIN_AND_1_JUNG_SINGLE:
            // ㅏ, ㅏ
            // ㅘ, ㅏ
            // 가, ㅏ
            // 까, ㅏ
            // 과, ㅏ
            // 꽈, ㅏ
            completedChar = makeHangul(mSeparatedChar);
            clearSeparatedChar();
            mSeparatedChar[1] = primaryCode;
            mWorkingChar = mSeparatedChar[1];
            mCurrentState = STATE_1_JUNG_SINGLE;
            break;

        case STATE_FIN_AND_2_JUNG_SINGLE: {
            // 각, ㅏ -> 가, 가
            // 깍, ㅏ -> 까, 가
            // 곽, ㅏ -> 과, 가
            // 꽉, ㅏ -> 꽈, 가
            // 갃, ㅏ -> 각, 사
            // 깏, ㅏ -> 깍, 사
            // 곿, ㅏ -> 곽, 사
            // 꽋, ㅏ -> 꽉, 사
            const int beforeJong = mSeparatedChar[2];

            // 복합 종성
            if(isCombinedJongseong(mSeparatedChar[2])) {
                //array를 pointer로 변경
            	separateCombinedJongseong(beforeJong, separateBuf);
            	LOGDWS("isCombinedJongseong() separateBuf = %s", (wchar_t*)separateBuf);
                mSeparatedChar[2] = separateBuf[0];
                completedChar = makeHangul(mSeparatedChar);
                mSeparatedChar[0] = separateBuf[1];
                mSeparatedChar[1] = primaryCode;
                mSeparatedChar[2] = KIND_NONE;
            } else { // 단일 종성
                mSeparatedChar[2] = KIND_NONE;
                completedChar = makeHangul(mSeparatedChar);
                mSeparatedChar[0] = beforeJong;
                mSeparatedChar[1] = primaryCode;
                mSeparatedChar[2] = KIND_NONE;
            }
            mWorkingChar = makeHangul(mSeparatedChar);
            mCurrentState = STATE_2_JUNG_SINGLE;
            break;
        }
    }

    if(completedChar != -1) {
        LOGDWC("saveCharacter() completedChar = %s", completedChar);
        currentWordAppend(completedChar);
    }

    if(mWorkingChar != -1) {
        LOGDWC("saveCharacter() mWorkingChar = %s", mWorkingChar);
        currentWordAppend(mWorkingChar);
    }


    return 0;
}

void AutomataQwerty::saveUnknownCharacter(int const state, wchar_t const primaryCode) {
    currentWordAppend(primaryCode);

    resetState();

//    mWorkingChar = primaryCode;
}

wstring AutomataQwerty::getComposingWord(){
	LOGDWS("AutomataQwerty::getComposingWord():%s", composingWord.c_str());
	return composingWord;
}

void AutomataQwerty::deleteCharComposing() {
}

void AutomataQwerty::deleteChar() {

    LOGW("AutomataQwerty::deleteChar() is called...%d", mWorkingChar);
    int separateBuf[2];

    if(mWorkingChar != KIND_NONE) {
        currentWordDelete();

        if(!isHangul(mWorkingChar)) {
            mWorkingChar = KIND_NONE;

        }
    } else {
        if(currentWord.length() > 0) {
            currentWordDelete();
        }
    }
    int mPreviousState = KIND_NONE;
    int mPrevHangulCharBuffer[3];
	if(currentWord[currentWord.length()-1] != 0 && currentWord.length() > 0){
		setBuffer(currentWord[currentWord.length()-1], mPreviousState, mPrevHangulCharBuffer);
	}

    switch(mCurrentState) {
    case STATE_1_CHO:
        // ㄱ
        // ㄲ
        mSeparatedChar[0] = KIND_NONE;
        mWorkingChar = KIND_NONE;
        mCurrentState = STATE_EMPTY;
        break;

    case STATE_1_JUNG_SINGLE:
        // ㅏ
        mSeparatedChar[1] = KIND_NONE;
        mWorkingChar = KIND_NONE;
        mCurrentState = STATE_EMPTY;
        break;

    case STATE_1_JUNG_PAIR:
        // ㅘ
        mSeparatedChar[1] = separateCombinedJungseong(mSeparatedChar[1], separateBuf)[0];
        mWorkingChar = mSeparatedChar[1];
        mCurrentState = STATE_1_JUNG_SINGLE;
        break;

    case STATE_2_JUNG_SINGLE:
        // 가
        // 까
        mSeparatedChar[1] = KIND_NONE;
        mWorkingChar = mSeparatedChar[0];
        mCurrentState = STATE_1_CHO;

        if(getJongseongIndex(mSeparatedChar[0]) > 0 && currentWord[currentWord.length()-1] != 0 && mSeparatedChar[0] != KIND_NONE && mPrevHangulCharBuffer[1] != KIND_NONE){
            //중성을 지울때 앞글자가 있다면 현재 초성이 앞글자의 받침으로 들어감.
            if(isCombinedJongseong(mPrevHangulCharBuffer[2])){
                //앞글자 받침이 복종성으로 있는 상태
                mCurrentState = STATE_1_CHO;
                mWorkingChar = mSeparatedChar[0];
            } else if(mPrevHangulCharBuffer[2] == KIND_NONE){
                //앞글자 받침이 없는 상태
                mPrevHangulCharBuffer[2] = mSeparatedChar[0];
                mSeparatedChar[0] = -1;
                mWorkingChar = makeHangul(mPrevHangulCharBuffer);
                mCurrentState = STATE_3_JONG_SINGLE;
            } else if(isCombinedJongseong(mPrevHangulCharBuffer[2], mSeparatedChar[0])){
                //앞글자의 받침과 결합이 되어 복종성이 될때
                mPrevHangulCharBuffer[2] = makeCombinedJongseong(mPrevHangulCharBuffer[2], mSeparatedChar[0]);
                mSeparatedChar[0] = -1;
                mWorkingChar = makeHangul(mPrevHangulCharBuffer);
                mCurrentState = STATE_3_JONG_PAIR;
            }
        }


        break;

    case STATE_2_JUNG_PAIR:
        // 과
        // 꽈
        mSeparatedChar[1] = separateCombinedJungseong(mSeparatedChar[1], separateBuf)[0];
        mWorkingChar = makeHangul(mSeparatedChar);
        mCurrentState = STATE_2_JUNG_SINGLE;
        break;

    case STATE_3_JONG_SINGLE:
        // 각
        // 깍
        // 곽
        // 꽉
        mSeparatedChar[2] = KIND_NONE;
        mWorkingChar = makeHangul(mSeparatedChar);
        mCurrentState = isCombinedJungseong(mSeparatedChar[1]) ? STATE_2_JUNG_PAIR : STATE_2_JUNG_SINGLE;
        break;

    case STATE_3_JONG_PAIR:
        // 갃
        // 곿
        // 깏
        // 꽋
        mSeparatedChar[2] = separateCombinedJongseong(mSeparatedChar[2], separateBuf)[0];
        mWorkingChar = makeHangul(mSeparatedChar);
        mCurrentState = STATE_3_JONG_SINGLE;
        break;
    }

    if(mWorkingChar != -1 && mCurrentState != KIND_NONE) {
        if((mCurrentState == STATE_3_JONG_SINGLE || mCurrentState == STATE_3_JONG_PAIR) && mSeparatedChar[0] == KIND_NONE){
            currentWordDelete();
            mSeparatedChar[0] = mPrevHangulCharBuffer[0];
            mSeparatedChar[1] = mPrevHangulCharBuffer[1];
            mSeparatedChar[2] = mPrevHangulCharBuffer[2];
        }
        currentWordAppend(mWorkingChar);
    } else {
        resetState();
        if(currentWord.length() > 0) {
        	setBuffer(currentWord[currentWord.length()-1], mCurrentState, mSeparatedChar);
            mWorkingChar = currentWord[currentWord.length()-1];
        }

    }

    LOGDWS("deleteChar() currentWord = '%s'", currentWord.c_str());
}

bool AutomataQwerty::isCombinedJungseong(wchar_t const j1, wchar_t const j2) {
    switch(j1) {
    case mo_o:
        switch(j2) {
        case mo_a: // ㅘ
        case mo_ae: // ㅙ
        case mo_i: // ㅚ
            return true;
        }
        break;
    case mo_woo:
        switch(j2) {
        case mo_eo: // ㅝ
        case mo_e: // ㅞ
        case mo_i: // ㅟ
            return true;
        }
        break;
    case mo_eu:
        switch(j2) {
        case mo_i: // ㅢ
            return true;
        }
        break;
    }
    return false;
}

bool AutomataQwerty::isCombinedJongseong(wchar_t const j1, wchar_t const j2) {
// ※쿼티자판에선 ㅃ, ㅉ과 같은 쌍자음을 포함시키지 않는다.

    switch(j1) {
    case ja_g:
        switch(j2) {
        case ja_s: // ㄳ
            return true;
        }
        break;
    case ja_n:
        switch(j2) {
        case ja_j: // ㄵ
        case ja_h: // ㄶ
            return true;
        }
        break;
    case ja_l:
        switch(j2) {
        case ja_g: // ㄺ
        case ja_m: // ㄻ
        case ja_b: // ㄼ
        case ja_s: // ㄽ
        case ja_t: // ㄾ
        case ja_p: // ㄿ
        case ja_h: // ㅀ
            return true;
        }
        break;
    case ja_b:
        switch(j2) {
        case ja_s: // ㅄ
            return true;
        }
        break;
    }
    return false;
}

bool AutomataQwerty::isCombinedJungseong(wchar_t const j) {
    switch(j) {
    case mo_wa://ㅘ:
    case mo_wae://ㅙ:
    case mo_woi://ㅚ:
    case mo_weo://ㅝ:
    case mo_we://ㅞ:
    case mo_wi://ㅟ:
    case mo_eui://ㅢ:
        return true;
    }
    return false;
}

bool AutomataQwerty::isCombinedJongseong(wchar_t const j) {
    switch(j) {
    case ja_gs://ㄳ:
    case ja_nj://ㄵ:
    case ja_nh://ㄶ:
    case ja_lg://ㄺ:
    case ja_lm://ㄻ:
    case ja_lb://ㄼ:
    case ja_ls://ㄽ:
    case ja_lt://ㄾ:
    case ja_lp://ㄿ:
    case ja_lh://ㅀ:
    case ja_bs://ㅄ:
        return true;
    }

    return false;
}

int AutomataQwerty::makeCombinedJungseong(wchar_t const j1, wchar_t const j2) {
    switch(j1) {
    case mo_o:
        switch(j2) {
        case mo_a: // ㅘ
            return mo_wa;
        case mo_ae: // ㅙ
            return mo_wae;
        case mo_i: // ㅚ
            return mo_woi;
        }
        break;
    case mo_woo:
        switch(j2) {
        case mo_eo: // ㅝ
            return mo_weo;
        case mo_e: // ㅞ
            return mo_we;
        case mo_i: // ㅟ
            return mo_wi;
        }
        break;
    case mo_eu:
        switch(j2) {
        case mo_i: // ㅢ
            return mo_eui;
        }
        break;
    }
    return KIND_NONE;
}

int AutomataQwerty::makeCombinedJongseong(wchar_t const j1, wchar_t const j2) {
    switch(j1) {
    case ja_g:
        switch(j2) {
        case ja_s: // ㄳ
            return ja_gs;
        }
        break;
    case ja_n:
        switch(j2) {
        case ja_j: // ㄵ
            return ja_nj;
        case ja_h: // ㄶ
            return ja_nh;
        }
        break;
    case ja_l:
        switch(j2) {
        case ja_g: // ㄺ
            return ja_lg;
        case ja_m: // ㄻ
            return ja_lm;
        case ja_b: // ㄼ
            return ja_lb;
        case ja_s: // ㄽ
            return ja_ls;
        case ja_t: // ㄾ
            return ja_lt;
        case ja_p: // ㄿ
            return ja_lp;
        case ja_h: // ㅀ
            return ja_lh;
        }
        break;
    case ja_b:
        switch(j2) {
        case ja_s: // ㅄ
            return ja_bs;
        }
        break;
    }
    return KIND_NONE;
}

int AutomataQwerty::getChoseongIndex(wchar_t const c) {

    const int size = sizeof(ARRAY_CHOSEONGS) / sizeof(ARRAY_CHOSEONGS[0]);

    for(int i = 0; i < size; i++) {

        if(c == ARRAY_CHOSEONGS[i]) {
            return i;
        }
    }

    return 0;
}

int AutomataQwerty::getJungseongIndex(wchar_t const j) {

    const int size = sizeof(ARRAY_JUNGSEONGS) / sizeof(ARRAY_JUNGSEONGS[0]);

    for(int i = 0; i < size; i++) {

        if(j == ARRAY_JUNGSEONGS[i]) {
            return i;
        }
    }

    return 0;
}

int AutomataQwerty::getJongseongIndex(wchar_t const j) {
    const int size = sizeof(ARRAY_JONGSEONGS) / sizeof(ARRAY_JONGSEONGS[0]);

    for(int i = 0; i < size; i++) {

        if(j == ARRAY_JONGSEONGS[i]) {
            return i;
        }
    }

    return 0;
}

int AutomataQwerty::makeHangul(int slicedPieces[]) {

    // 비완성형 문자
    if(slicedPieces[0] == KIND_NONE) {
        return slicedPieces[1];
    } else if(slicedPieces[1] == KIND_NONE) {
        return slicedPieces[0];
    } else { // 완성형 문자
        return 0xAC00 + (getChoseongIndex(slicedPieces[0]) * 588) + (getJungseongIndex(slicedPieces[1]) * 28) + (slicedPieces[2] == KIND_NONE ? 0 : getJongseongIndex(slicedPieces[2]));
    }
}
void AutomataQwerty::setBuffer(wchar_t const primaryCode, int &state, int *buffer) {

    resetState(state, buffer);

// 한글이 아니면 패스
    if(!isHangul(primaryCode)) {
        mWorkingChar = primaryCode;
        return;
    }
    if(L'ㄱ' <= primaryCode && primaryCode <= L'ㅣ') { // 비완성형 문자
        // ㄱ
        // ㄲ
        // ㅏ
        // ㅘ

        if(STRING_CHOSEONGS.find(primaryCode) != wstring::npos) {
        	buffer[0] = primaryCode;
        	buffer[1] = KIND_NONE;
        	buffer[2] = KIND_NONE;
        	state = STATE_1_CHO;
        } else if(STRING_JUNGSEONGS.find(primaryCode) != wstring::npos) {
        	buffer[0] = KIND_NONE;
        	buffer[1] = primaryCode;
        	buffer[2] = KIND_NONE;
            if(isCombinedJungseong(primaryCode)) {
            	state = STATE_1_JUNG_PAIR;
            } else {
            	state = STATE_1_JUNG_SINGLE;
            }
        } else {
            return;
        }
    } else if(L'가' <= primaryCode && primaryCode <= L'힣') { // 완성형 문자
        // 가
        // 까
        // 과
        // 꽈
        // 각
        // 깍
        // 곽
        // 꽉
        // 갃
        // 깏
        // 곿
        // 꽋
        const wchar_t purePrimaryCode = primaryCode - L'가';
        buffer[0] = ARRAY_CHOSEONGS[purePrimaryCode / 588];
        buffer[1] = ARRAY_JUNGSEONGS[(purePrimaryCode % 588) / 28];
        const wchar_t jongIdx = purePrimaryCode % 28;
        buffer[2] = jongIdx == 0 ? KIND_NONE : ARRAY_JONGSEONGS[jongIdx];

        if(buffer[2] == KIND_NONE) {
            if(isCombinedJungseong(buffer[1])) {
                // 가
                // 까
            	state = STATE_2_JUNG_PAIR;
            } else {
                // 과
                // 꽈
            	state = STATE_2_JUNG_SINGLE;
            }
        } else {
            if(isCombinedJongseong(buffer[2])) {
                // 갃
                // 깏
                // 곿
                // 꽋
            	state = STATE_3_JONG_PAIR;
            } else {
                // 각
                // 깍
                // 곽
                // 꽉
            	state = STATE_3_JONG_SINGLE;
            }
        }
    }
}


int *AutomataQwerty::separateCombinedJungseong(int const j, int *ret) {
    ret[0] = KIND_NONE;
    ret[1] = KIND_NONE;
    switch(j) {
    case mo_wa:
        ret[0] = mo_o;
        ret[1] = mo_a;
        break;
    case mo_wae:
        ret[0] = mo_o;
        ret[1] = mo_ae;
        break;
    case mo_woi:
        ret[0] = mo_o;
        ret[1] = mo_i;
        break;
    case mo_weo:
        ret[0] = mo_woo;
        ret[1] = mo_eo;
        break;
    case mo_we:
        ret[0] = mo_woo;
        ret[1] = mo_e;
        break;
    case mo_wi:
        ret[0] = mo_woo;
        ret[1] = mo_i;
        break;
    case mo_eui:
        ret[0] = mo_eu;
        ret[1] = mo_i;
        break;
    }

    return ret;
}

int *AutomataQwerty::separateCombinedJongseong(int const j, int *ret) {
    ret[0] = KIND_NONE;
    ret[1] = KIND_NONE;
    switch(j) {
    case ja_gs: //ㄳ:
        ret[0] = ja_g;//ㄱ;
        ret[1] = ja_s;//ㅅ;
        break;
    case ja_nj: //ㄵ:
        ret[0] = ja_n;//ㄴ;
        ret[1] = ja_j;//ㅈ;
        break;
    case ja_nh: //ㄶ:
        ret[0] = ja_n;//ㄴ;
        ret[1] = ja_h;//ㅎ;
        break;
    case ja_lg: //ㄺ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_g;//ㄱ;
        break;
    case ja_lm: //ㄻ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_m;//ㅁ;
        break;
    case ja_lb: //ㄼ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_b;//ㅂ;
        break;
    case ja_ls: //ㄽ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_s;//ㅅ;
        break;
    case ja_lt: //ㄾ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_t;//ㅌ;
        break;
    case ja_lp: //ㄿ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_p;//ㅍ;
        break;
    case ja_lh: //ㅀ:
        ret[0] = ja_l;//ㄹ;
        ret[1] = ja_h;//ㅎ;
        break;
    case ja_bs: //ㅄ:
        ret[0] = ja_b;//ㅂ;
        ret[1] = ja_s;//ㅅ;
        break;
    }

    return ret;
}

int AutomataQwerty::appendText(const wstring &s) {

    LOGD(">> AutomataQwerty::appendText");

    int len = s.length();
    if(len > 1) {
        return BaseAutomata::appendText(s);
    } else if(len == 1) {
        return appendChar(s[0]);
    }
    return RESULT_ERROR;
}


int AutomataQwerty::appendSeparator(wchar_t c) {

    LOGD(">> AutomataQwerty::appendSeparator()");
    int ret = BaseAutomata::appendSeparator(c);
    resetState();
    return ret;
}

void AutomataQwerty::cursorChange(wstring beforeString, wstring afterString) {
    LOGD(">> AutomataQwerty::cursorChange");
    resetState();

    BaseAutomata::cursorChange(beforeString, afterString);
}



void AutomataQwerty::currentWordAppend(wchar_t wc) {
    currentWord += wc;
    LOGDWS("currentWordAppend() currentWord = %s", currentWord.c_str());
    composingWord += wc;
	LOGDWS("currentWordAppend() composingWord = %s", composingWord.c_str());
}

void AutomataQwerty::currentWordDelete() {
    int len = currentWord.length();
    if(len > 0) {
        currentWord = currentWord.substr(0, len - 1);
    }
    LOGDWS("[del]currentWord = %s", currentWord.c_str());

    len = composingWord.length();
	if(len > 0) {
		composingWord = composingWord.substr(0, len - 1);
	}
	LOGDWS("[del]composingWord = %s", composingWord.c_str());
}

bool AutomataQwerty::isHangul(wchar_t const primaryCode) {
	if ((primaryCode >= 0xAC00 && primaryCode <= 0xD7A3) || (primaryCode >= 0x3131 && primaryCode <= 0x318E) || primaryCode == 0x11A2) {
        return true;
    }
    return false;
}

void AutomataQwerty::reset() {
    BaseAutomata::reset();
    resetState();
}

void AutomataQwerty::resetState() {
    clearSeparatedChar();
    mCurrentState = STATE_EMPTY;
    mWorkingChar = KIND_NONE;
}

void AutomataQwerty::resetState(int &state, int *buffer){
	for(int i = 0; i < LEN(buffer); i++){
		buffer[i] = KIND_NONE;
	}
	state = KIND_NONE;
}

void AutomataQwerty::deleteAllChar(){
	resetState();
	BaseAutomata::deleteAllChar();
}


        }
    }
}
