#include "AutomataCheonJiIn.h"

#include "../../../common/Const.h"
#include "../../../utils/LogUtils.h"
#include "../../../utils/Utils.h"
#include "../../IAutomata.h"
#include "../../../Defines.h"




namespace automata {
    namespace langs {
        namespace ko_kr {

const /*int*/wchar_t AutomataCheonJiIn::PREF_MO_HEAD[3] = {
    0x3163, 0x318d, 0x3161  //ㅣ ㆍ ㅡ
};
const /*int*/wchar_t AutomataCheonJiIn::PREF_JA_HEAD[19] = { L'ㄱ', L'ㄴ', L'ㄷ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅅ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ', L'ㅃ',L'ㅉ',L'ㄸ',L'ㄲ',L'ㅆ'};

const /*int*/wchar_t AutomataCheonJiIn::PREF_JA[7][3] = {
    {0x3131, 0x314B, 0x3132},       //ㄱ ㅋ ㄲ
    {0x3134, 0x3139, },             //ㄴ ㄹ
    {0x3137, 0x314C, 0x3138},       //ㄷ ㅌ ㄸ
    {0x3142, 0x314D, 0x3143},       //ㅂ ㅍ ㅃ
    {0x3145, 0x314E, 0x3146},       //ㅅ ㅎ ㅆ
    {0x3148, 0x314A, 0x3149},       //ㅈ ㅊ ㅉ
    {0x3147, 0x3141, }              //ㅇ ㅁ
};


AutomataCheonJiIn::AutomataCheonJiIn() {
	mCGIMo = std::wstring(AutomataCheonJiIn::PREF_MO_HEAD, 0, sizeof(AutomataCheonJiIn::PREF_MO_HEAD) / sizeof(AutomataCheonJiIn::PREF_MO_HEAD[0]));
	mCGIJa = std::wstring(AutomataCheonJiIn::PREF_JA_HEAD, 0, sizeof(AutomataCheonJiIn::PREF_JA_HEAD) / sizeof(AutomataCheonJiIn::PREF_JA_HEAD[0]));
	mChoseong = std::wstring(PREF_CHO, 0, sizeof(PREF_CHO) / sizeof(PREF_CHO[0]));
	mJungseong = std::wstring(PREF_JUNG, 0, sizeof(PREF_JUNG) / sizeof(PREF_JUNG[0]));
}

int AutomataCheonJiIn::appendChar(wchar_t unicode) {
	LOGDWC("AutomataCheonJiIn::appendChar() is called..., c = %s", unicode);
    /*int*/wchar_t codes[1] = { unicode };
    std::wstring strPrimaryCode = std::wstring(codes, 0, 1);
    std::wstring cPrimaryCode = strPrimaryCode.substr(0, strPrimaryCode.length());

    // 자/모음 구별
    int kind = HANGUL_NONE;
    if(mCGIJa.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_JA;
    } else if(mCGIMo.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_MO;
    } else {
        saveUnknownCharacter(mCurrentState, unicode); //, composer);
        LOGV("AutomataCheonJiIn::appendChar::saveUnknownCharacter()");
        return IAutomata::RETURN_COMPOSE;
    }

    // 문자 처리
    switch(mCurrentState) {
        case HANGUL_NONE:
            if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_CHO1; // 초성하나만 나온상태
            } else {
                mCurrentState = HANGUL_JUNG1; // 중성 하나만 나온상태
            }
            break;

        case HANGUL_CHO1:
            if(kind == HANGUL_JA) {
                // 이전 초성이 제한시간안에 눌려서 토글이 될 수 있으면
                if(isToggle(mHangulCharBuffer[0], unicode) > 0 && isTimeToggle) {
                    unicode = static_cast<wchar_t>(setToggle(mHangulCharBuffer[0]));
                } else { // 그렇지 않으면 앞글자 종료
                    mCurrentState = HANGUL_CHO1_FINISH;
                }
            } else {
                mCurrentState = HANGUL_JUNG1;
            }
            break;

        case HANGUL_JUNG1:
            if(kind == HANGUL_JA) {
                if(isDot(mHangulCharBuffer[1])){
                	mCurrentState = HANGUL_JUNG1_FINISH;
                } else {
                	mCurrentState = HANGUL_JONG1; // 종성이 나온상태
                }
            } else {
                // 합쳐질 가능성이 있으면
                if(isAddJung(mHangulCharBuffer[1], unicode) ) {
                    unicode = static_cast<wchar_t>(setJung(mHangulCharBuffer[1], unicode));
                    if(isDot(mHangulCharBuffer[1])) {
                        mCurrentState = HANGUL_JUNG_DOT;
                    } else {
                        mCurrentState = HANGUL_JUNG1;
                    }
                } else {
                    mCurrentState = HANGUL_JUNG1_FINISH;
                }
            }
            break;

        case HANGUL_JONG1: // 종성이 나온상태
            if(kind == HANGUL_JA) {
                if(isToggle(mHangulCharBuffer[2], unicode) > 0 && isTimeToggle) {
                    unicode = static_cast<wchar_t>(setToggle(mHangulCharBuffer[2]));
                } else {
                    if(getJongseongPair(mHangulCharBuffer[2], unicode) >= 0)  { // 종성
                        mCurrentState = HANGUL_JONG2;
                    }
                    else if(isJongseongPairAble(mHangulCharBuffer[2], unicode) >= 0) { // 종성
                        mCurrentState = HANGUL_JONG2_ABLE;
                    } else {
                        mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
                    }
                }
            } else {
                if(isDot(unicode)) {
                    mCurrentState = HANGUL_JUNG_DOT;
                } else {
                    mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
                }
            }
            break;

        case HANGUL_JONG1_ABLE:
            if(kind == HANGUL_JA) {
                if(isToggle(mHangulCharBuffer[3], unicode) > 0 && isTimeToggle) {
                    mCurrentState = HANGUL_JONG1_ABLE;
                } else {
                	mCurrentState = HANGUL_JONG1_ABLE_FINISH;
                }
            } else {
                mCurrentState = HANGUL_JONG1_ABLE_FINISH;
            }
            break;

        case HANGUL_JONG2: // 복자음 받침상태
            if(kind == HANGUL_JA) { // 복자음의 경우 복자음의 분리된 뒤쪽 자음과 토글관계인지 검사
                int *ret = resolveJongseongPair(mHangulCharBuffer[2]);
                if(isToggle(ret[1], unicode) > 0 && isTimeToggle) {
                    int temp = setToggle(ret[1]);
                    unicode = static_cast<wchar_t>(temp);
                } else {
                    mCurrentState = HANGUL_JONG2_FINISH;
                }
            } else {
                if(isDot(unicode)) {
                    mCurrentState = HANGUL_JUNG_DOT;
                } else {
                    mCurrentState = HANGUL_JONG2_FINISH;
                }
            }
            break;

        case HANGUL_JONG2_ABLE:
            if(kind == HANGUL_JA) {
                if(isToggle(mHangulCharBuffer[3], unicode) > 0 && isTimeToggle) {
                    unicode = static_cast<wchar_t>(setToggle(mHangulCharBuffer[3]));
                    if(getJongseongPair(mHangulCharBuffer[2], unicode) >= 0) { // 종성
                        mCurrentState = HANGUL_JONG2; // 삶
                    } else if(isJongseongPairAble(mHangulCharBuffer[2], unicode) >= 0) { // 종성
                        mCurrentState = HANGUL_JONG2_ABLE;
                    } else {
                        mCurrentState = HANGUL_JONG2_ABLE_FINISH; // 한글자 완료.
                    }
                } else {
                    mCurrentState = HANGUL_JONG2_ABLE_FINISH; // 한글자 완료.
                }
            } else {
                mCurrentState = HANGUL_JONG2_ABLE_FINISH; // 한글자 완료.
            }
            break;

        case HANGUL_JUNG_DOT:
            if(kind == HANGUL_JA) {

            } else {
                if(isAddJung(mHangulCharBuffer[3], unicode)) {
                    if(isDot(mHangulCharBuffer[3])) {
                        mCurrentState = HANGUL_JUNG_DOT;
                    }

                    unicode = static_cast<wchar_t>(setJung(mHangulCharBuffer[3], unicode));
                }
            }
            break;
//        case HANGUL_TIME_COMPOSE:
//        	switch(mBeforeState){
//				case HANGUL_CHO1:
//					break;
//				case HANGUL_JUNG1:
//					break;
//				case
//        	}
//        	break;
        default:
            break;
    }
    isTimeToggle = true;
    return saveCharacter(mCurrentState, unicode, kind);
}

void AutomataCheonJiIn::deleteChar() {

	LOGDWS("AutomataCheonJiIn::deleteChar():%s", currentWord.c_str());

    if(mWorkingChar != HANGUL_NONE) {
        if(currentWord.length() > 0) {
            currentWordDelete();
        }
        if(!isHangul(mWorkingChar)) {
            mWorkingChar = HANGUL_NONE;
        }
    } else {
        if(currentWord.length() > 0) {
            currentWordDelete();
        }
    }
    mWorkingChar2 = HANGUL_NONE;
    int mPreviousState = HANGUL_NONE;
	int mPrevHangulCharBuffer[3] = {-1,};
    if(currentWord[currentWord.length()-1] != 0 && currentWord.length() > 0){
		setBuffer(currentWord[currentWord.length()-1], mPreviousState, mPrevHangulCharBuffer);
    }

    switch(mCurrentState) {
        case HANGUL_CHO1:
            mCurrentState = HANGUL_NONE;
            mHangulCharBuffer[0] = HANGUL_NONE;
            mWorkingChar = HANGUL_NONE;
            break;

        case HANGUL_JUNG1: {
            if(isJungseongPair(mHangulCharBuffer[1])){
                mHangulCharBuffer[1] = resolveJungseongPair(mHangulCharBuffer[1])[0];
                mHangulCharBuffer[0] == HANGUL_NONE ? mWorkingChar = mHangulCharBuffer[1] : mWorkingChar = makeHangul(mHangulCharBuffer);
            } else {
                int temp = mHangulCharBuffer[1];
                mHangulCharBuffer[1] = HANGUL_NONE;
                if(mHangulCharBuffer[0] != HANGUL_NONE) {
                    // 지워진 모음이 Dot인 경우 특수처리
                    if(temp == HANGUL_DOT) {
                        mWorkingChar = HANGUL_NONE;
                        mCurrentState = HANGUL_CHO1;
                    } else if(temp == HANGUL_DOUBLE_DOT) {
                        mWorkingChar = HANGUL_DOT;
                        mHangulCharBuffer[1] = HANGUL_DOT;
                    } else {
                        mWorkingChar = mHangulCharBuffer[0];
                        mCurrentState = HANGUL_CHO1;
                    }
                } else {
                	if(temp == HANGUL_DOT){
                		mWorkingChar = HANGUL_NONE;
                		mCurrentState = HANGUL_NONE;
                	} else if(temp == HANGUL_DOUBLE_DOT){
                		mWorkingChar = HANGUL_DOT;
                		mHangulCharBuffer[1] = HANGUL_DOT;
                		mCurrentState = HANGUL_JUNG1;
                	} else {
						mCurrentState = HANGUL_NONE;
						mHangulCharBuffer[0] = HANGUL_NONE;
						mWorkingChar = HANGUL_NONE;
                	}
                }
                if(mCurrentState == HANGUL_CHO1 && mPreviousState != HANGUL_CHO1 && !isDot(mPrevHangulCharBuffer[1]) && mPrevHangulCharBuffer[0] != HANGUL_NONE){
                    //중성을 지울때 앞글자가 있다면 현재 초성이 앞글자의 받침으로 들어감.
                	if(getJongseongIndex(mHangulCharBuffer[0]) >= 0){
						if(isJongseongPair(mPrevHangulCharBuffer[2])){
							//앞글자 받침이 복종성으로 있는 상태
							mCurrentState = HANGUL_CHO1;
							mWorkingChar = mHangulCharBuffer[0];
						} else if(mPrevHangulCharBuffer[2] == HANGUL_NONE){
							//앞글자 받침이 없는 상태
							mPrevHangulCharBuffer[2] = mHangulCharBuffer[0];
							mWorkingChar = makeHangul(mPrevHangulCharBuffer);
							mWorkingChar2 = -2;
							mCurrentState = HANGUL_JONG1;
						} else if(getJongseongPair(mPrevHangulCharBuffer[2], mHangulCharBuffer[0]) >= 0){
							//앞글자의 받침과 결합이 되어 복종성이 될때
							mPrevHangulCharBuffer[2] = getJongseongPair(mPrevHangulCharBuffer[2], mHangulCharBuffer[0]);
							mWorkingChar = makeHangul(mPrevHangulCharBuffer);
							mWorkingChar2 = -2;
							mCurrentState = HANGUL_JONG2;
						} else if(isJongseongPairAble(mPrevHangulCharBuffer[2],mHangulCharBuffer[0])){
							mWorkingChar = mHangulCharBuffer[0];
							mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
							mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
							mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
							mHangulCharBuffer[3] = mWorkingChar;
							mCurrentState = HANGUL_JONG2_ABLE;
						}
                	} else if(mPrevHangulCharBuffer[2] != HANGUL_NONE && isJongseongPairAble(mPrevHangulCharBuffer[2],mHangulCharBuffer[0])){
                		mWorkingChar = mHangulCharBuffer[0];
						mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
						mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
						mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
						mHangulCharBuffer[3] = mWorkingChar;
                		mCurrentState = HANGUL_JONG2_ABLE;
                	} else {
                		mCurrentState = HANGUL_JONG1_ABLE;
                		mWorkingChar = mHangulCharBuffer[0];
						mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
						mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
						mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
						mHangulCharBuffer[3] = mWorkingChar;
                	}
                }
            }
            break;
        }
        case HANGUL_JONG1_ABLE:
            mHangulCharBuffer[3] = -1;
            mWorkingChar = -1;
            break;

        case HANGUL_JONG2_ABLE:
            mHangulCharBuffer[3] = -1;
            mWorkingChar = -1;
            break;

        case HANGUL_JONG2:
            if(isJongseongPair(mHangulCharBuffer[2])){
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

        case HANGUL_JUNG_DOT:
            mCurrentState = HANGUL_NONE;
            mHangulCharBuffer[3] = -1;
            if(mHangulCharBuffer[2] != -1) {
                mCurrentState = HANGUL_JONG1;
            }
            mWorkingChar = -1;
            break;

        case HANGUL_NONE:
        default:
            break;

    }

    if(mWorkingChar != -1 && mCurrentState != HANGUL_NONE) {
        if(mWorkingChar2 == -2){
            currentWordDelete();
            mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
            mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
            mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
            mHangulCharBuffer[3] = -1;
        }
        currentWordAppend(mWorkingChar);

    } else {
        resetState();
        if(getCurrentWord().length() > 0) {
            setBuffer(currentWord[currentWord.length()-1], mCurrentState, mHangulCharBuffer);
            mWorkingChar = currentWord[currentWord.length()-1];
        }
    }
}

int AutomataCheonJiIn::isToggle(int v1, int v2) {
    // CGI 추가
    if(getJaGroup(v1) == getJaGroup(v2)) {
        return 1;
    }
    return 0;

}

int AutomataCheonJiIn::setToggle(int v1) {
    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_JA) / sizeof(PREF_JA[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_JA[count]); count2++) {
            if(v1 == PREF_JA[count][count2]) {
                // toggle
                count2++;
                count2 %= LEN(PREF_JA[count]);
                if(PREF_JA[count][count2] == 0){
                    return PREF_JA[count][0];
                } else{
                    return PREF_JA[count][count2];
                }
            }
        }
    }

    return v1;
}

bool AutomataCheonJiIn::isAddJung(int v1, int v2) {

    switch(v2) {
        case HANGUL_DOT:

            switch(v1) {
                case 0x3163:    //ㅣ
                case 12623:     //ㅏ
                case 12625:     //ㅑ
                case 0x3161:    //ㅡ
                case 12636:     //ㅜ
                case 12640:     //ㅠ
                case 12634:     //ㅚ
                case HANGUL_DOT:
                case HANGUL_DOUBLE_DOT:
                    return true;
                    break;
            }
            break;

        case 0x3163: // 'ㅣ'
            switch(v1) {
                case 12623:     //ㅏ
                case 12625:     //ㅑ
                case 12627:     //ㅓ
                case 12629:     //ㅕ
                case 12631:     //ㅗ
                case 12632:     //ㅘ
                case 12636:     //ㅜ
                case 12637:     //ㅝ
                case 12641:     //ㅡ
                case 12640:     //ㅠ
                case HANGUL_DOT:
                case HANGUL_DOUBLE_DOT:
                	return true;
                    break;
            }

            break;
        case 0x3161: // 'ㅡ'
            switch(v1) {
                case HANGUL_DOT:
                case HANGUL_DOUBLE_DOT:
                	return true;
                    break;
            }
            break;

    }

    return false;

}

int AutomataCheonJiIn::setJung(int v1, int v2) {
    int ret = v2;
    switch(v2) {
        case HANGUL_DOT:
            switch(v1) {
                case 0x3163://ㅣ
                    ret = 12623; //ㅏ
                    break;
                case 12623: //ㅏ
                    ret = 12625; //ㅑ
                    break;
                case 12625: //ㅑ
                    ret = 12623; //ㅏ
                    break;
                case 0x3161://ㅡ
                    ret = 12636; //ㅜ
                    break;
                case 12636: //ㅜ
                    ret = 12640; //ㅠ
                    break;
                case 12640: //ㅠ
                    ret = 12636; //ㅜ
                    break;
                case 12634: //ㅚ
                    ret = 12632; //ㅘ
                    break;
                case HANGUL_DOT:
                    ret = HANGUL_DOUBLE_DOT;
                    break;
                case HANGUL_DOUBLE_DOT:
                    ret = HANGUL_DOT;
                    break;
            }

            break;
        case 12643: // 'ㅣ'
            switch(v1) {
                case 12623: // ㅏ
                case 12625: // ㅑ
                case 12627: // ㅓ
                case 12629: // ㅕ
                case 12637: // ㅝ
                    ret = v1 + 1; //ㅞ
                    break;
                case 12631: // ㅗ
                case 12636: // ㅜ
                    ret = v1 + 3; //ㅟ
                    break;
                case 12632: // ㅘ
                    ret = 12633; // ㅙ
                    break;
                case 12640: // ㅠ
                    ret = 12637; // ㅝ
                    break;
                case 12641: // ㅡ
                    ret = 12642; // ㅢ
                    break;
                case HANGUL_DOT:
                    ret = 12627; // ㅓ
                    break;
                case HANGUL_DOUBLE_DOT:
                    ret = 12629; // ㅕ
                    break;
            }

            break;
        case 0x3161: // 'ㅡ'
            switch(v1) {
                case 0x3163: //ㅣ
                    ret = 12642; //ㅢ
                    break;
                case HANGUL_DOT:
                    ret = 12631; //ㅗ
                    break;
                case HANGUL_DOUBLE_DOT:
                    ret = 12635; //ㅛ
                    break;
            }
            break;

    }

    return ret;
}

int AutomataCheonJiIn::getJaGroup(int v1) {

    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_JA) / sizeof(PREF_JA[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_JA[count]); count2++) {
            if(v1 == PREF_JA[count][count2]) {
                return count;
            }
        }
    }
    return -1;
}

int AutomataCheonJiIn::isJongseongPairAble(int v1, int v2) {

    int group = getJaGroup(v2);
    if(group == -1) {
        return -1;
    }

    int count;

    for(count = 0; count < LEN(PREF_JA[group]); count++) {
        if(getJongseongPair(v1, PREF_JA[group][count]) >= 0) // 종성
        {
            return 1;
        }
    }
    return -1;
}
        }
    }
}
