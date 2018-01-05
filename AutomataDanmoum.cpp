#include "AutomataDanmoum.h"

#include "../../../common/Const.h"
#include "../../../utils/LogUtils.h"
#include "../../../utils/Utils.h"
#include "../../IAutomata.h"
#include "../../../Defines.h"

namespace automata {
namespace langs {
namespace ko_kr {

const /*int*/wchar_t AutomataDanmoum::PREF_MO_HEAD[14] = { L'ㅏ',L'ㅑ', L'ㅓ',L'ㅕ', L'ㅗ',L'ㅛ', L'ㅜ',L'ㅠ', L'ㅡ', L'ㅣ', L'ㅐ',L'ㅒ', L'ㅔ',L'ㅖ'};
const /*int*/wchar_t AutomataDanmoum::PREF_JA_HEAD[19] = { L'ㄱ', L'ㄴ', L'ㄷ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅅ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ', L'ㅃ',L'ㅉ',L'ㄸ',L'ㄲ',L'ㅆ'};

const /*int*/wchar_t AutomataDanmoum::PREF_JA[5][2] = { { L'ㄱ', L'ㄲ' }, { L'ㄷ', L'ㄸ' }, { L'ㅂ', L'ㅃ' }, { L'ㅅ', L'ㅆ' }, { L'ㅈ', L'ㅉ' }, };
const /*int*/wchar_t AutomataDanmoum::PREF_MO[6][2] = { { L'ㅏ', L'ㅑ' }, { L'ㅓ', L'ㅕ' }, { L'ㅗ', L'ㅛ' }, { L'ㅜ', L'ㅠ' }, { L'ㅐ', L'ㅒ' }, { L'ㅔ', L'ㅖ' }, };

AutomataDanmoum::AutomataDanmoum() {
    mCGIMo = std::wstring(AutomataDanmoum::PREF_MO_HEAD, 0, sizeof(AutomataDanmoum::PREF_MO_HEAD) / sizeof(AutomataDanmoum::PREF_MO_HEAD[0]));
    mCGIJa = std::wstring(AutomataDanmoum::PREF_JA_HEAD, 0, sizeof(AutomataDanmoum::PREF_JA_HEAD) / sizeof(AutomataDanmoum::PREF_JA_HEAD[0]));
    mChoseong = std::wstring(PREF_CHO, 0, sizeof(PREF_CHO) / sizeof(PREF_CHO[0]));
	mJungseong = std::wstring(PREF_JUNG, 0, sizeof(PREF_JUNG) / sizeof(PREF_JUNG[0]));
}

int AutomataDanmoum::appendChar(wchar_t unicode) {
    LOGDWC("AutomataDanmoum::appendChar() is called..., c = %s", unicode);

//    LOGV(">> JeongUk mCurrentState = %d", mCurrentState);
//    LOGV(">> JeongUk mWorkingChar = %d", mWorkingChar);
//    for(int i = 0 ;i < 4; i++){
//		LOGV(">> JeongUk mHangulCharBuffer[%d] = %d", i, mHangulCharBuffer[i]);
//    }


    /*int*/wchar_t codes[1] = { unicode };
    std::wstring strPrimaryCode = std::wstring(codes, 0, 1);
    std::wstring cPrimaryCode = strPrimaryCode.substr(0, strPrimaryCode.length());

//    LOGD("kwiae : %d %d", unicode, PREF_JA_HEAD[0]);
    // 자/모음 구별
    int kind = HANGUL_NONE;
    if (mCGIJa.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_JA;
    } else if (mCGIMo.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_MO;
    } else {
        saveUnknownCharacter(mCurrentState, unicode); //, composer);
        LOGV("AutomataDanmoum::appendChar::saveUnknownCharacter()");
        return IAutomata::RETURN_COMPOSE;
    }

    // 문자 처리
    switch (mCurrentState) {
    case HANGUL_NONE:
        if (kind == HANGUL_JA) {
            mCurrentState = HANGUL_CHO1; // 초성하나만 나온상태
        } else {
            mCurrentState = HANGUL_JUNG1; // 중성 하나만 나온상태
        }
        break;

    case HANGUL_CHO1:
        if (kind == HANGUL_JA) {
            // 이전 초성이 제한시간안에 눌려서 토글이 될 수 있으면
            if (isToggleJa(mHangulCharBuffer[0], unicode) > 0 && isTimeToggle) {
                unicode = static_cast<wchar_t>(setToggleJa(mHangulCharBuffer[0]));
            } else { // 그렇지 않으면 앞글자 종료
                mCurrentState = HANGUL_CHO1_FINISH;
            }
        } else {
            mCurrentState = HANGUL_JUNG1;
        }
        break;

    case HANGUL_JUNG1:
        if (kind == HANGUL_JA) {
            mCurrentState = HANGUL_JONG1; // 종성이 나온상태
        } else {
            if (isToggleMo(mHangulCharBuffer[1], unicode) && isTimeToggle) {
                unicode = static_cast<wchar_t>(setToggleMo(mHangulCharBuffer[1]));
                mCurrentState = HANGUL_JUNG1;
            } else if (isJungPair(unicode)) {
                unicode = static_cast<wchar_t>(addJung(unicode));
                mCurrentState = HANGUL_JUNG1;
            } else {
                mCurrentState = HANGUL_JUNG1_FINISH;
            }
        }
        break;

    case HANGUL_JONG1: // 종성이 나온상태
        if (kind == HANGUL_JA) {
            if (isToggleJa(mHangulCharBuffer[2], unicode) > 0 && isTimeToggle) {
                unicode = static_cast<wchar_t>(setToggleJa(mHangulCharBuffer[2]));
                mHangulCharBuffer[2] = -1;
                if (getJongseongIndex(unicode) >= 0) {
                    mCurrentState = HANGUL_JONG1;
                } else {
                    mCurrentState = HANGUL_JONG1_FINISH;
                }
            } else {
                if (getJongseongPair(mHangulCharBuffer[2], unicode) >= 0) { // 종성
                    mCurrentState = HANGUL_JONG2;
                } else {
                    mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
                }
            }
        } else {
            mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
        }
        break;

    case HANGUL_JONG2: // 복자음 받침상태
        if (kind == HANGUL_JA) { // 복자음의 경우 복자음의 분리된 뒤쪽 자음과 토글관계인지 검사
            int *ret = resolveJongseongPair(mHangulCharBuffer[2]);
            if (isToggleJa(ret[1], unicode) > 0 && isTimeToggle) {
                int temp = setToggleJa(ret[1]);
                unicode = static_cast<wchar_t>(temp);
                mHangulCharBuffer[2] = ret[0];
                mCurrentState = HANGUL_JONG2_FINISH;
            } else {
                mCurrentState = HANGUL_JONG2_FINISH;
            }
        } else {
            mCurrentState = HANGUL_JONG2_FINISH;
        }
        break;

    default:
        break;
    }
    isTimeToggle = true;
    return saveCharacter(mCurrentState, unicode, kind);
}

void AutomataDanmoum::deleteChar() {

    LOGDWS("AutomataCheonJiIn::deleteChar():%s", currentWord.c_str());

    if (mWorkingChar != HANGUL_NONE) {
        if (currentWord.length() > 0) {
            currentWordDelete();
        }
        if (!isHangul(mWorkingChar)) {
            mWorkingChar = HANGUL_NONE;
        }
    } else {
        if (currentWord.length() > 0) {
            currentWordDelete();
        }
    }
    mWorkingChar2 = HANGUL_NONE;
    int mPreviousState = HANGUL_NONE;
    int mPrevHangulCharBuffer[3] = { -1, };
    if (currentWord[currentWord.length() - 1] != 0 && currentWord.length() > 0) {
        setBuffer(currentWord[currentWord.length() - 1], mPreviousState, mPrevHangulCharBuffer);
    }

    switch (mCurrentState) {
    case HANGUL_CHO1:
        mCurrentState = HANGUL_NONE;
        mHangulCharBuffer[0] = HANGUL_NONE;
        mWorkingChar = HANGUL_NONE;
        break;

    case HANGUL_JUNG1: {
        if (isJungseongPair(mHangulCharBuffer[1])) {
            mHangulCharBuffer[1] = resolveJungseongPair(mHangulCharBuffer[1])[0];
            mHangulCharBuffer[0] == HANGUL_NONE ? mWorkingChar = mHangulCharBuffer[1] : mWorkingChar = makeHangul(mHangulCharBuffer);
        } else {
            int temp = mHangulCharBuffer[1];
            mHangulCharBuffer[1] = HANGUL_NONE;
            if (mHangulCharBuffer[0] != HANGUL_NONE) {
                mWorkingChar = mHangulCharBuffer[0];
                mCurrentState = HANGUL_CHO1;
            } else {
                mCurrentState = HANGUL_NONE;
                mHangulCharBuffer[0] = HANGUL_NONE;
                mWorkingChar = HANGUL_NONE;
            }
            if (mCurrentState == HANGUL_CHO1 && mPreviousState != HANGUL_CHO1 && mPrevHangulCharBuffer[0] != HANGUL_NONE) {
                //중성을 지울때 앞글자가 있다면 현재 초성이 앞글자의 받침으로 들어감.
                if (getJongseongIndex(mHangulCharBuffer[0]) >= 0) {
                    if (isJongseongPair(mPrevHangulCharBuffer[2])) {
                        //앞글자 받침이 복종성으로 있는 상태
                        mCurrentState = HANGUL_CHO1;
                        mWorkingChar = mHangulCharBuffer[0];
                    } else if (mPrevHangulCharBuffer[2] == HANGUL_NONE) {
                        //앞글자 받침이 없는 상태
                        mPrevHangulCharBuffer[2] = mHangulCharBuffer[0];
                        mWorkingChar = makeHangul(mPrevHangulCharBuffer);
                        mWorkingChar2 = -2;
                        mCurrentState = HANGUL_JONG1;
                    } else if (getJongseongPair(mPrevHangulCharBuffer[2], mHangulCharBuffer[0]) >= 0) {
                        //앞글자의 받침과 결합이 되어 복종성이 될때
                        mPrevHangulCharBuffer[2] = getJongseongPair(mPrevHangulCharBuffer[2], mHangulCharBuffer[0]);
                        mWorkingChar = makeHangul(mPrevHangulCharBuffer);
                        mWorkingChar2 = -2;
                        mCurrentState = HANGUL_JONG2;
                    }
                }
            }
        }
        break;
    }

    case HANGUL_JONG2:
        if (isJongseongPair(mHangulCharBuffer[2])) {
            mHangulCharBuffer[2] = resolveJongseongPair(mHangulCharBuffer[2])[0];
            mCurrentState = HANGUL_JONG1;
            mWorkingChar = makeHangul(mHangulCharBuffer);
        }
        break;

    case HANGUL_JONG1:
        mHangulCharBuffer[2] = -1;
        mWorkingChar = makeHangul(mHangulCharBuffer);
        mCurrentState = HANGUL_JUNG1;
        break;

    case HANGUL_NONE:
    default:
        break;
    }

    if (mWorkingChar != -1 && mCurrentState != HANGUL_NONE) {
        if (mWorkingChar2 == -2) {
            currentWordDelete();
            mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
            mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
            mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
            mHangulCharBuffer[3] = -1;
        }
        currentWordAppend(mWorkingChar);

    } else {
        resetState();
        if (getCurrentWord().length() > 0) {
            setBuffer(currentWord[currentWord.length() - 1], mCurrentState, mHangulCharBuffer);
            mWorkingChar = currentWord[currentWord.length() - 1];
        }
    }
}

int AutomataDanmoum::appendSeparator(wchar_t const unicode) { //, IComposer *const composer) {
	LOGW(">> AutomataDanmoum::appendSeparator()");
	if(unicode == KeyCode::KEY_TYPE_BASIC_SEPARATOR){
		isTimeToggle = false;
//		if(currentWord.length() > 0){
//			setBuffer(currentWord[currentWord.length()-1], mCurrentState, mHangulCharBuffer);
//			mWorkingChar = currentWord[currentWord.length()-1];
//
//		    LOGV(">> JeongUk mCurrentState = %d", mCurrentState);
//		    LOGV(">> JeongUk mWorkingChar = %d", mWorkingChar);
//		    for(int i = 0 ;i < 4; i++){
//				LOGV(">> JeongUk mHangulCharBuffer[%d] = %d", i, mHangulCharBuffer[i]);
//		    }
//		}
		return 0;
	} else {
		resetState();
		isTimeToggle = true;
		return BaseAutomata::appendSeparator(unicode);
	}
}



int AutomataDanmoum::getGroupNumJa(int v1) {

    int count;
    int count2;

    for (count = 0; count < sizeof(PREF_JA) / sizeof(PREF_JA[0]); count++) {

        if (v1 == PREF_JA[count][0]) {
            return count;
        }
    }
    return -1;
}

int AutomataDanmoum::getGroupNumMo(int v1) {
    int count;
    int count2;

    for (count = 0; count < sizeof(PREF_MO) / sizeof(PREF_MO[0]); count++) {
        for (count2 = 0; count2 < LEN(PREF_MO[count]); count2++) {
            if (v1 == PREF_MO[count][count2]) {
                return count;
            }
        }
    }

    return -1;
}

bool AutomataDanmoum::isToggleMo(int v1, int v2) {

//    if (v1 == 12641 || v1 == 12643 || v2 == 12641 || v2 == 12643) {
//        return false;
//    }

    int ret = getGroupNumMo(v1);
    if(ret == -1 ){
        return false;
    }

    // CGI 추가
    if (ret == getGroupNumMo(v2)) {
        return true;
    }
    return false;

}

int AutomataDanmoum::setToggleMo(int v1) {
    int count;
    int count2;

    for (count = 0; count < sizeof(PREF_MO) / sizeof(PREF_MO[0]); count++) {
        for (count2 = 0; count2 < LEN(PREF_MO[count]); count2++) {
            if (v1 == PREF_MO[count][count2]) {
                // toggle
                count2++;
                count2 %= LEN(PREF_MO[count]);
                return PREF_MO[count][count2];
            }
        }
    }

    return v1;
}

int AutomataDanmoum::isToggleJa(int v1, int v2) {
    // CGI 추가
	int ret = getGroupNumJa(v2);
    if (ret == -1) {
        return 0;
    }
    if (getGroupNumJa(v1) == ret) {
        return 1;
    }
    return 0;
}

int AutomataDanmoum::setToggleJa(int v1) {
    int count;
    int count2;

    for (count = 0; count < sizeof(PREF_JA) / sizeof(PREF_JA[0]); count++) {
        for (count2 = 0; count2 < LEN(PREF_JA[count]); count2++) {
            if (v1 == PREF_JA[count][count2]) {
                // toggle
                count2++;
                count2 %= LEN(PREF_JA[count]);
                if (PREF_JA[count][count2] == 0) {
                    return PREF_JA[count][0];
                } else {
                    return PREF_JA[count][count2];
                }
            }
        }
    }

    return v1;
}

bool AutomataDanmoum::isJungPair(int Code) {

    int PreCode = mHangulCharBuffer[1];

    if (Code == L'ㅣ') {
        switch (PreCode) {
        case L'ㅗ':
        case L'ㅜ':
        case L'ㅡ':
            return true;
        }
    } else if (Code == L'ㅏ') {
        switch (PreCode) {
        case L'ㅗ':
            return true;
        }
    } else if (Code == L'ㅓ') {
        switch (PreCode) {
        case L'ㅜ':
            return true;
        }
    } else if (Code == L'ㅐ') {
        switch (PreCode) {
        case L'ㅗ':
            return true;
        }
    } else if (Code == L'ㅔ') {
        switch (PreCode) {
        case L'ㅜ':
            return true;
        }
    }

    return false;
}

int AutomataDanmoum::addJung(int Code) {
    int ret = Code;
    int PreCode = mHangulCharBuffer[1];

    if (Code == L'ㅣ') {
        switch (PreCode) {
        case L'ㅗ':
        case L'ㅜ':
            ret = PreCode + 3;
            break;
        case L'ㅡ':
            ret = PreCode + 1;
            break;
        }
    } else if (Code == L'ㅏ') {
        switch (PreCode) {
        case L'ㅗ':
            ret = PreCode + 1;
            break;
        }
    } else if (Code == L'ㅓ') {
        switch (PreCode) {
        case L'ㅜ':
            ret = PreCode + 1;
            break;
        }
    } else if (Code == L'ㅐ') {
        switch (PreCode) {
        case L'ㅗ':
            ret = PreCode + 2;
            break;
        }
    } else if (Code == L'ㅔ') {
        switch (PreCode) {
        case L'ㅜ':
            ret = PreCode + 2;
            break;
        }
    }

    return ret;
}

}
}
}

