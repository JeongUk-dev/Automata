#include "AutomataSky.h"


#include "../../../common/Const.h"
#include "../../../utils/LogUtils.h"
#include "../../../utils/Utils.h"
#include "../../IAutomata.h"
#include "../../../Defines.h"

namespace automata {
    namespace langs {
        namespace ko_kr {

const /*int*/wchar_t AutomataSky::PREF_JA[7][3] = {
    {0x3131, 0x314B, 0x3132},           //ㄱ ㅋ ㄲ
    {0x3134, 0x3139},                   //ㄴ ㄹ
    {0x3137, 0x314C, 0x3138},           //ㄷ ㅌ ㄸ
    {0x3141, 0x3145, 0x3146},           //ㅁ ㅅ ㅆ
    {0x3142, 0x314D, 0x3143},           //ㅂ ㅍ ㅃ
    {0x3148, 0x314A, 0x3149},           //ㅈ ㅊ ㅉ
    {0x3147, 0x314E}                    //ㅇ ㅎ
};
const /*int*/wchar_t AutomataSky::PREF_MO[5][3] = {
    {12623, 12625},                     //ㅏ ㅑ
    {12627, 12629},                     //ㅓ ㅕ
    {12631, 12635},                     //ㅗ ㅛ
    {12636, 12640},                     //ㅜ ㅠ
    {12641, 12643, 12642}                      // ㅡ ㅣㅢ
};

//ㄱ ㄴ ㄷ ㅁ ㅂ ㅇ ㅈ
const /*int*/wchar_t AutomataSky::PREF_JA_HEAD[19] = { L'ㄱ', L'ㄴ', L'ㄷ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅅ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ', L'ㅃ',L'ㅉ',L'ㄸ',L'ㄲ',L'ㅆ'};

//ㅏ ㅓ ㅗ ㅜ ㅡ
const /*int*/wchar_t AutomataSky::PREF_MO_HEAD[11] = { L'ㅏ', L'ㅑ', L'ㅓ',  L'ㅕ',L'ㅗ', L'ㅛ', L'ㅜ', L'ㅠ', L'ㅡ', L'ㅣ',L'ㅢ'};


const wchar_t AutomataSky::Double_Jeongseong_Single[] = {L'ㅐ', L'ㅒ', L'ㅔ', L'ㅖ'};


AutomataSky::AutomataSky() {
	mCGIMo = std::wstring(AutomataSky::PREF_MO_HEAD, 0, sizeof(AutomataSky::PREF_MO_HEAD) / sizeof(AutomataSky::PREF_MO_HEAD[0]));
	mCGIJa = std::wstring(AutomataSky::PREF_JA_HEAD, 0, sizeof(AutomataSky::PREF_JA_HEAD) / sizeof(AutomataSky::PREF_JA_HEAD[0]));
	mChoseong = std::wstring(PREF_CHO, 0, sizeof(PREF_CHO) / sizeof(PREF_CHO[0]));
	mJungseong = std::wstring(PREF_JUNG, 0, sizeof(PREF_JUNG) / sizeof(PREF_JUNG[0]));
}

int AutomataSky::appendChar(wchar_t unicode) {

    /*int*/wchar_t codes[1] = {unicode};
    std::wstring strPrimaryCode = std::wstring(codes, 0, 1);
    std::wstring cPrimaryCode = strPrimaryCode.substr(0, strPrimaryCode.length());

    int kind = HANGUL_NONE;
    if(mCGIJa.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_JA;
    } else if(mCGIMo.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_MO;
    } else {
        saveUnknownCharacter(mCurrentState, unicode);
        return IAutomata::RETURN_COMPOSE;
    }

    // 문자 처리
    switch(mCurrentState) {
        case HANGUL_NONE:
            if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_CHO1;
            } else {
                mCurrentState = HANGUL_JUNG1;
            }
            break;

        case HANGUL_CHO1:
            if(kind == HANGUL_JA) {
                // 토글 검사
                if(isToggleJa(mHangulCharBuffer[0], unicode) && isTimeToggle) {
                    unicode = static_cast<wchar_t>(setToggleJa(mHangulCharBuffer[0]));
                } else {
                    mCurrentState = HANGUL_CHO1_FINISH;
                }
            } else {
                mCurrentState = HANGUL_JUNG1;
            }
            break;

        case HANGUL_JUNG1:
            if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_JONG1;
            } else {
				if(isToggleMo(mHangulCharBuffer[1], unicode) && isTimeToggle) {
					unicode = static_cast<wchar_t>(setToggleMo(mHangulCharBuffer[1]));
					mCurrentState = HANGUL_JUNG1;
				} else if(isJungPair(unicode)) {
					unicode = static_cast<wchar_t>(addJung(unicode));
					mCurrentState = HANGUL_JUNG1;
				} else {
					mCurrentState = HANGUL_JUNG1_FINISH;
				}
            }
            break;

        case HANGUL_JONG1:
        	if(kind == HANGUL_JA) {
				if(isToggleJa(mHangulCharBuffer[2], unicode) > 0 && isTimeToggle) {
					unicode = static_cast<wchar_t>(setToggleJa(mHangulCharBuffer[2]));
				} else {
					if(getJongseongPair(mHangulCharBuffer[2], unicode) >= 0)  { // 종성
						mCurrentState = HANGUL_JONG2;
					}
					else if(isJongseongPairAble(mHangulCharBuffer[2], unicode)) { // 종성
						mCurrentState = HANGUL_JONG2_ABLE;
					} else {
						mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
					}
				}
			} else {
                mCurrentState = HANGUL_JONG1_FINISH;
            }
            break;

        case HANGUL_JONG1_ABLE:
            if(kind == HANGUL_JA) {
                if(isToggleJa(mHangulCharBuffer[3], unicode) && isTimeToggle) {
					mCurrentState = HANGUL_JONG1_ABLE;
                } else {
                	mCurrentState = HANGUL_JONG1_ABLE_FINISH;
                }
			} else {
				mCurrentState = HANGUL_JONG1_ABLE_FINISH;
			}
            break;

        case HANGUL_JONG2:
            if(kind == HANGUL_JA) {
                //array를 포인터로 변경
                int *ret = resolveJongseongPair(mHangulCharBuffer[2]);
                if(isToggleJa(ret[1], unicode) > 0 && isTimeToggle) {
                    int temp = setToggleJa(ret[1]);
                    unicode = static_cast<wchar_t>(temp);
				} else {
					mCurrentState = HANGUL_JONG2_FINISH;
				}
            } else {
                mCurrentState = HANGUL_JONG2_FINISH;
            }
            break;

        case HANGUL_JONG2_ABLE:
            if(kind == HANGUL_JA) {
                if(isToggleJa(mHangulCharBuffer[3], unicode) && isTimeToggle) {
                    unicode = static_cast<wchar_t>(setToggleJa(mHangulCharBuffer[3]));
                    if(getJongseongPair(mHangulCharBuffer[2], unicode) >= 0){
                        mCurrentState = HANGUL_JONG2;
                    } else if(isJongseongPairAble(mHangulCharBuffer[2],unicode)){
                        mCurrentState = HANGUL_JONG2_ABLE;
                    } else {
                    	mCurrentState = HANGUL_JONG2_ABLE_FINISH;
                    }
                } else {
                    mCurrentState = HANGUL_JONG2_ABLE_FINISH;
                }
            } else {
                mCurrentState = HANGUL_JONG2_ABLE_FINISH;
            }
            break;
        default:
            break;
    }
    isTimeToggle = true;
    return saveCharacter(mCurrentState, unicode, kind);
}

void AutomataSky::deleteChar() {

	if(mWorkingChar != HANGUL_NONE) {
		if(getCurrentWord().length() > 0) {
			currentWordDelete();
		}
		if(!isHangul(mWorkingChar)) {
			mWorkingChar = HANGUL_NONE;
		}
	} else {
		if(getCurrentWord().length() > 0) {
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
				mHangulCharBuffer[1] = HANGUL_NONE;
				if(mHangulCharBuffer[0] != HANGUL_NONE) {
					mCurrentState = HANGUL_CHO1;
					mWorkingChar = mHangulCharBuffer[0];
				} else {
					mCurrentState = HANGUL_NONE;
					mHangulCharBuffer[0] = HANGUL_NONE;
					mWorkingChar = HANGUL_NONE;
				}

				if(currentWord[currentWord.length()-1] != 0 && mHangulCharBuffer[0] != -1 && mPrevHangulCharBuffer[1] != -1 && mPrevHangulCharBuffer[0] != HANGUL_NONE){
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
							//앞글자의 받침과 현재 지우고 남은 초성이 토글되어 복종성이 가능할때
							mWorkingChar = mHangulCharBuffer[0];
							mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
							mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
							mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
							mHangulCharBuffer[3] = mWorkingChar;
							mCurrentState = HANGUL_JONG2_ABLE;
						}
					} else if(mPrevHangulCharBuffer[2] != -1 && isJongseongPairAble(mPrevHangulCharBuffer[2],mHangulCharBuffer[0])){
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
		case HANGUL_JONG1:
			mHangulCharBuffer[2] = HANGUL_NONE;
            mWorkingChar = makeHangul(mHangulCharBuffer);
            mCurrentState = HANGUL_JUNG1;
            break;
        case HANGUL_JONG2:
            if(isJongseongPair(mHangulCharBuffer[2])) {
            	mHangulCharBuffer[2] = resolveJongseongPair(mHangulCharBuffer[2])[0];
            	mCurrentState = HANGUL_JONG1;
            	mWorkingChar = makeHangul(mHangulCharBuffer);
            } else {

            }
            break;
        case HANGUL_JONG1_ABLE:
        case HANGUL_JONG2_ABLE:
            mHangulCharBuffer[3] = -1;
            //mCurrentState = HANGUL_JONG1;
            mWorkingChar = -1;
            break;
        case HANGUL_NONE:
        default:
            break;
    }
    if(mWorkingChar != -1 &&  mCurrentState != HANGUL_NONE) {
    	if(mWorkingChar2 == -2){
    		currentWordDelete();
    		mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
    		mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
    		mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];
    		mHangulCharBuffer[3] = HANGUL_NONE;
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

bool AutomataSky::isCombinedJungseongSingle(int v){

    for(int i = 0; i< LEN(Double_Jeongseong_Single); i++){
        if(Double_Jeongseong_Single[i] == v){
            return true;
        }
    }
    return false;
}

bool AutomataSky::isToggleJa(int v1, int v2) {

    // CGI 추가
    if(getGroupNumJa(v1) == getGroupNumJa(v2)) {
        return true;
    }
    return false;

}

int AutomataSky::getGroupNumJa(int v1) {
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

int AutomataSky::setToggleJa(int v1) {
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

bool AutomataSky::isToggleMo(int v1, int v2) {

    int ret = getGroupNumMo(v1);
       if(ret == -1 ){
           return false;
       }
    // CGI 추가
    if(getGroupNumMo(v1) == getGroupNumMo(v2)) {
        return true;
    }
    return false;

}

int AutomataSky::getGroupNumMo(int v1) {
    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_MO) / sizeof(PREF_MO[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_MO[count]); count2++) {
            if(v1 == PREF_MO[count][count2]) {
                return count;
            }
        }
    }

    return -1;
}

int AutomataSky::setToggleMo(int v1) {
    int count;
    int count2;

    for (count = 0; count < sizeof(PREF_MO) / sizeof(PREF_MO[0]); count++) {
        for (count2 = 0; count2 < LEN(PREF_MO[count]); count2++) {
            if (v1 == PREF_MO[count][count2]) {
                // toggle
                count2++;
                count2 %= LEN(PREF_MO[count]);
                if (PREF_MO[count][count2] == 0) {
                    return PREF_MO[count][0];
                } else {
                    return PREF_MO[count][count2];
                }
            }
        }
    }

    return v1;
}

bool AutomataSky::isJungPair(int Code) {

    int PreCode = mHangulCharBuffer[1];

    if(Code == L'ㅣ') {
        switch(PreCode) {
            case L'ㅏ':
            case L'ㅑ':
            case L'ㅓ':
            case L'ㅕ':
            case L'ㅗ':
            case L'ㅜ':
            case L'ㅡ':
            case L'ㅘ':
            case L'ㅝ':
                return true;
        }
    } else if(Code == L'ㅏ') {
        switch(PreCode) {
            case L'ㅗ':
//            case L'ㅜ':
                return true;
        }
    } else if(Code == L'ㅓ') {
        switch(PreCode) {
            case L'ㅜ':
                return true;
        }
    }

    return false;
}

int AutomataSky::addJung(int Code) {
    int ret = Code;
    int PreCode = mHangulCharBuffer[1];

    if(Code == L'ㅣ') {
        switch(PreCode) {
            case L'ㅏ':
            case L'ㅑ':
            case L'ㅓ':
            case L'ㅕ':
            case L'ㅡ':
            case L'ㅘ':
            case L'ㅝ':
                ret = PreCode + 1;
                break;
            case L'ㅗ':
            case L'ㅜ':
                ret = PreCode + 3;
                break;
        }
    } else if(Code == L'ㅏ') {
        switch(PreCode) {
            case L'ㅗ':
                ret = PreCode + 1;
                break;
        }
    } else if(Code == L'ㅓ') {
        switch(PreCode) {
            case L'ㅜ':
                ret = PreCode + 1;
                break;
        }
    }

    return ret;
}

bool AutomataSky::isJongseongPairAble(int v1, int v2) {

    int group = getGroupNumJa(v2);
    if(group == -1) {
        return false;
    }

    int count;

    for(count = 0; count < LEN(PREF_JA[group]); count++) {
        if(getJongseongPair(v1, PREF_JA[group][count]) >= 0) // 종성
        {
            return true;
        }
    }
    return false;
}



        }
    }
}
