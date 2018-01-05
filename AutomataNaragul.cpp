#include "AutomataNaragul.h"

#include "../../../common/Const.h"
#include "../../../utils/LogUtils.h"
#include "../../../utils/Utils.h"
#include "../../IAutomata.h"
#include "../../../Defines.h"

namespace automata {
    namespace langs {
        namespace ko_kr {

const /*int*/wchar_t AutomataNaragul::PREF_MO_HEAD[6] = {L'ㅏ',L'ㅓ', L'ㅗ',L'ㅜ', L'ㅣ', L'ㅡ'};
const /*int*/wchar_t AutomataNaragul::PREF_JA_HEAD[6] = {L'ㄱ', L'ㄴ', L'ㄹ', L'ㅁ', L'ㅅ', L'ㅇ'};

const /*int*/wchar_t AutomataNaragul::PREF_JA[6][5] = {
    {0x3131, 0x314B, 0x3132},                                //ㄱ ㅋ ㄲ
    {0x3134, 0x3137, 0x314C, 0x3138},                     //ㄴ ㄷ ㅌ ㄸ
    {0x3139},                                                    //ㄹ
    {0x3141, 0x3142, 0x314D, 0x3143},                     //ㅁ ㅂ ㅍ ㅃ
    {0x3145, 0x3146, 0x3148, 0x314A, 0x3149},          //ㅅ ㅆ ㅈ ㅊ ㅉ
    {0x3147, 0x314E}                                          //ㅇ ㅎ
};

const /*int*/wchar_t AutomataNaragul::PREF_ADD_LINE[9][3] = {
    {L'ㄱ', L'ㅋ'},
    {L'ㄴ', L'ㄷ', L'ㅌ'},
    {L'ㅁ', L'ㅂ', L'ㅍ'},
    {L'ㅅ', L'ㅈ', L'ㅊ'},
    {L'ㅇ', L'ㅎ'},
    {L'ㅏ', L'ㅑ'},
    {L'ㅓ', L'ㅕ'},
    {L'ㅗ', L'ㅛ'},
    {L'ㅜ', L'ㅠ'}
};
const /*int*/wchar_t AutomataNaragul::PREF_DOUBLE_CHAR[5][2] = {
    {L'ㄱ', L'ㄲ'},
    {L'ㄷ', L'ㄸ'},
    {L'ㅂ', L'ㅃ'},
    {L'ㅅ', L'ㅆ'},
    {L'ㅈ', L'ㅉ'}
};


AutomataNaragul::AutomataNaragul() {
	mLastFunctionKey = false;

	mCGIMo = std::wstring(AutomataNaragul::PREF_MO_HEAD, 0, sizeof(AutomataNaragul::PREF_MO_HEAD) / sizeof(AutomataNaragul::PREF_MO_HEAD[0]));
	mCGIJa = std::wstring(AutomataNaragul::PREF_JA_HEAD, 0, sizeof(AutomataNaragul::PREF_JA_HEAD) / sizeof(AutomataNaragul::PREF_JA_HEAD[0]));
	mChoseong = std::wstring(PREF_CHO, 0, sizeof(PREF_CHO) / sizeof(PREF_CHO[0]));
	mJungseong = std::wstring(PREF_JUNG, 0, sizeof(PREF_JUNG) / sizeof(PREF_JUNG[0]));
}


int AutomataNaragul::getAddGroup(int v1) {

    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_ADD_LINE) / sizeof(PREF_ADD_LINE[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_ADD_LINE[count]); count2++) {
            if(v1 == PREF_ADD_LINE[count][count2]) {
                return count;
            }
        }
    }
    return -1;
}

int AutomataNaragul::getDoubleGroup(int v1) {

    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_DOUBLE_CHAR) / sizeof(PREF_DOUBLE_CHAR[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_DOUBLE_CHAR[count]); count2++) {
            if(v1 == PREF_DOUBLE_CHAR[count][count2]) {
                return count;
            }
        }
    }
    return -1;
}

int AutomataNaragul::isJongseongPairAble(int v1, int v2) {

    int group = getAddGroup(v2);
    int count;
    int ret1 = -1;
    int ret2 = -1;
    if(group != -1) {
        for(count = 0; count < LEN(PREF_ADD_LINE[group]); count++) {
            if(getJongseongPair(v1, PREF_ADD_LINE[group][count]) >= 0) { // 종성
                ret1 = 1;
            }
        }
    }

    int group2 = getDoubleGroup(v2);
    if(group2 != -1) {
        for(count = 0; count < LEN(PREF_DOUBLE_CHAR[group2]); count++) {
            if(getJongseongPair(v1, PREF_DOUBLE_CHAR[group2][count]) >= 0) { // 종성
                ret2 = 1;
            }
        }
    }

    if(ret1 == 1 || ret2 == 1) {
        return 1;
    }

    return -1;
}

int AutomataNaragul::isToggle(int v1, int v2) {
    // CGI 추가
    if(getJaGroup(v1) == getJaGroup(v2)) {
        return 1;
    }
    return 0;

}

int AutomataNaragul::getJaGroup(int v1) {

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

bool AutomataNaragul::isFunctionKey(int primaryCode) {
    if(primaryCode == ADD_LINE || primaryCode == DOUBLE_CHAR) {
        return true;
    }
        return false;
}

int AutomataNaragul::setCode(int primaryCode) {

    switch(primaryCode) {
        case ADD_LINE:
            return addLine();
        case DOUBLE_CHAR:
            return doubleChar();
    }

    return -1;

}

int AutomataNaragul::addLine() {

    int code = -1;
    switch(mCurrentState) {
        case HANGUL_CHO1:
            code = mHangulCharBuffer[0];
            break;

        case HANGUL_JUNG1:
            code = mHangulCharBuffer[1];
            break;

        case HANGUL_JONG1:
            code = mHangulCharBuffer[2];
            break;

        case HANGUL_JONG2: {
            //array를 pointer로 변경
            int *jong = resolveJongseongPair(mHangulCharBuffer[2]);
            code = jong[1];
            break;

        }
        case HANGUL_JONG2_ABLE:
            code = mHangulCharBuffer[3];
            break;

        default:
            break;

    }

    if(code != -1) {
        return addLine(code);
    }

    return -1;
}

int AutomataNaragul::doubleChar() {

    int code = -1;
    switch(mCurrentState) {

        case HANGUL_CHO1:
            code = mHangulCharBuffer[0];
            break;
        case HANGUL_JONG1: // 종성이 나온상태
            code = mHangulCharBuffer[2];
            break;
        case HANGUL_JONG2: { // 종성이 나온상태
            //array를 pointer로 변경
            int *jong = resolveJongseongPair(mHangulCharBuffer[2]);
            code = jong[1];
            break;
        }
        case HANGUL_JONG1_ABLE:
        	code = mHangulCharBuffer[3];
        	break;
        case HANGUL_JONG2_ABLE: // 종성이 나온상태
            code = mHangulCharBuffer[3];
            break;
        default:
            break;

    }

    if(code != -1) {
        return dobuleChar(code);
    }

    return -1;
}

int AutomataNaragul::addLine(int Code) {
    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_ADD_LINE) / sizeof(PREF_ADD_LINE[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_ADD_LINE[count]); count2++) {
            // v2 의 그룹을 찾고
            if(Code == PREF_ADD_LINE[count][count2]) {
                // 그 그룹의 자음과 v1의 결합이 복모음에 존재하는지 검사
                count2++;
                count2 %= LEN(PREF_ADD_LINE[count]);
                if(PREF_ADD_LINE[count][count2] == 0){
                    return PREF_ADD_LINE[count][0];
                } else{
                    return PREF_ADD_LINE[count][count2];
                }
            }
        }
    }
    //Code == -1 ? ret = -1 : ret = Code;
    return -1;
}

int AutomataNaragul::dobuleChar(int Code) {
    int count;
    int count2;

    for(count = 0; count < sizeof(PREF_DOUBLE_CHAR) / sizeof(PREF_DOUBLE_CHAR[0]); count++) {
        for(count2 = 0; count2 < LEN(PREF_DOUBLE_CHAR[count]); count2++) {
            // v2 의 그룹을 찾고
            if(Code == PREF_DOUBLE_CHAR[count][count2]) {
                // 그 그룹의 자음과 v1의 결합이 복모음에 존재하는지 검사
                return PREF_DOUBLE_CHAR[count][(count2 + 1) % LEN(PREF_DOUBLE_CHAR[count])];
            }
        }
    }
    //Code == -1 ? ret = -1 : ret = Code;
    return -1;
}

int AutomataNaragul::addJung(int Code) {
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
            case L'ㅜ':
                ret = PreCode + 1;
                break;
        }
    }
    return ret;
}

int AutomataNaragul::getJungToggleGroup(int Code) {
    if(Code == L'ㅏ' || Code == L'ㅓ' || Code == L'ㅑ' || Code == L'ㅕ') {
        return 0;
    } else if(Code == L'ㅗ' || Code == L'ㅜ' || Code == L'ㅛ' || Code == L'ㅠ') {
        return 1;
    }

    return -1;
}

int AutomataNaragul::setJungToggle() {
    int preCode = mHangulCharBuffer[1];
    int ret = preCode;

    switch(preCode) {
        case L'ㅏ':
        case L'ㅑ':
            ret = preCode + 4;
            break;
        case L'ㅓ':
        case L'ㅕ':
            ret = preCode - 4;
            break;
        case L'ㅗ':
        case L'ㅛ':
            ret = preCode + 5;
            break;
        case L'ㅜ':
        case L'ㅠ':
            ret = preCode - 5;
            break;
        default:
            break;
    }
    return ret;
}

bool AutomataNaragul::isJungPair(int Code) {

    int PreCode = mHangulCharBuffer[1];
    bool ret = false;
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
                ret = true;
                break;
        }
    } else if(Code == L'ㅏ') {
        switch(PreCode) {
            case L'ㅗ':
            case L'ㅜ':
                ret = true;
                break;
        }
    }

    return ret;
}

int AutomataNaragul::appendChar(wchar_t unicode) {

    /*int*/wchar_t codes[1] = { unicode };
    std::wstring strPrimaryCode = std::wstring(codes, 0, 1);
    std::wstring cPrimaryCode = strPrimaryCode.substr(0, strPrimaryCode.length());

    // 자/모음 구별
    int kind = HANGUL_NONE;
    if(isFunctionKey(unicode)) {
        kind = HANGUL_FN; //2
    } else if(mCGIJa.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_JA; //0
    } else if(mCGIMo.find(cPrimaryCode) != string::npos) {
        kind = HANGUL_MO; //1
    } else {
        saveUnknownCharacter(mCurrentState, unicode);
        return 0;
    }

    // 문자 처리
    switch(mCurrentState) {
        case HANGUL_NONE:
            if(kind == HANGUL_FN) {
                return IAutomata::RETURN_COMMIT;
            } else if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_CHO1; // 초성하나만 나온상태
            } else {
                mCurrentState = HANGUL_JUNG1; // 중성 하나만 나온상태
            }
            break;

        case HANGUL_CHO1:
            if(kind == HANGUL_FN) {
                unicode = static_cast<wchar_t>(setCode(unicode));
                if(unicode != -1){
                	kind = mCurrentState;
                } else {
                	return IAutomata::RETURN_COMMIT;
                }
            } else if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_CHO1_FINISH;
                mLastFunctionKey = false;
            } else {
                mCurrentState = HANGUL_JUNG1;
                mLastFunctionKey = false;
            }
            break;

        case HANGUL_JUNG1:
            if(kind == HANGUL_FN && (unicode == ADD_LINE || unicode == DOUBLE_CHAR)) {
                mLastFunctionKey = true;
                int temp = static_cast<wchar_t>(setCode(unicode));
                if(temp != -1)
                    unicode = temp;
                else
                    return IAutomata::RETURN_COMMIT;;
            } else if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_JONG1;
                mLastFunctionKey = false;
            } else {
                if((getJungToggleGroup(unicode) != -1) && (getJungToggleGroup(unicode) == getJungToggleGroup(mHangulCharBuffer[1]))) {
                    if(mLastFunctionKey == false) {
                        unicode = static_cast<wchar_t>(setJungToggle());
                    } else {
                        mCurrentState = HANGUL_JUNG1_FINISH;
                    }
                } else {
                    if(isJungPair(unicode)) {
                        unicode = static_cast<wchar_t>(addJung(unicode));
                    } else {
                        mLastFunctionKey = false;
                        mCurrentState = HANGUL_JUNG1_FINISH;
                    }
                }
                mLastFunctionKey = false;
            }
            break;

        case HANGUL_JONG1: // 종성이 나온상태
        	if(kind == HANGUL_FN) {
        		unicode = static_cast<wchar_t>(setCode(unicode));
        		if(unicode != -1){
                    if(isToggle(mHangulCharBuffer[2], unicode) >0) {
						mCurrentState = HANGUL_JONG1;
					} else {
						mCurrentState = HANGUL_JONG1_ABLE;
					}
        		} else {
        			return IAutomata::RETURN_COMMIT;
        		}
        	} else if(kind == HANGUL_JA) {
        		if(getJongseongPair(mHangulCharBuffer[2], unicode) >= 0) { // 종성
        			mCurrentState = HANGUL_JONG2;
        		} else if(isJongseongPairAble(mHangulCharBuffer[2], unicode) >= 0) { // 종성
        			mCurrentState = HANGUL_JONG2_ABLE;
        		} else {
        			mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
				}
        	} else {
        		mCurrentState = HANGUL_JONG1_FINISH; // 한글자 완료.
			}
        	mLastFunctionKey = false;
        	break;

        case HANGUL_JONG1_ABLE:
        	if(kind == HANGUL_FN){
        		unicode = static_cast<wchar_t>(setCode(unicode));
        		if(unicode != -1){
                    if(isToggle(mHangulCharBuffer[2], unicode) >0) {
                        mCurrentState = HANGUL_JONG1_ABLE;
					} else {
		                mCurrentState = HANGUL_JONG1_ABLE_FINISH;
					}
        		} else {
        			return IAutomata::RETURN_COMMIT;
        		}
        	} else {
        		mCurrentState = HANGUL_JONG1_ABLE_FINISH;
        	}
        	mLastFunctionKey = false;
        	break;

        case HANGUL_JONG2: // 복자음 받침상태
        	if(kind == HANGUL_FN) {
        		unicode = static_cast<wchar_t>(setCode(unicode));
        		if(unicode != -1){
					int *jong = resolveJongseongPair(mHangulCharBuffer[2]);
                    if (isToggle(jong[0], unicode) >= 0) {
                        mCurrentState = HANGUL_JONG2;
					} else {
						mCurrentState = HANGUL_JONG2_FINISH;
					}
        		} else {
        			return IAutomata::RETURN_COMMIT;
        		}
        	} else {
        		mCurrentState = HANGUL_JONG2_FINISH;
			}
        	mLastFunctionKey = false;
        	break;

        case HANGUL_JONG2_ABLE:
			if(kind == HANGUL_FN) {
				unicode = static_cast<wchar_t>(setCode(unicode));
				if(unicode != -1){
					if(getJongseongPair(mHangulCharBuffer[2], unicode) >= 0) { // 종성
						mCurrentState = HANGUL_JONG2;
					} else if(isJongseongPairAble(mHangulCharBuffer[2], unicode) >= 0) { // 종성
						mCurrentState = HANGUL_JONG2_ABLE;
					} else {
						mCurrentState = HANGUL_JONG2_ABLE_FINISH; // 한글자 완료.
					}
				} else {
					return IAutomata::RETURN_COMMIT;
				}
			} else {
                mCurrentState = HANGUL_JONG2_ABLE_FINISH;
            }
            mLastFunctionKey = false;
            break;

        default:
        break;
    }

    return saveCharacter(mCurrentState, unicode, kind);
}

void AutomataNaragul::deleteChar() {

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
            mHangulCharBuffer[0] = -1;
            mWorkingChar = -1;
            break;

        case HANGUL_JUNG1: {
            if(isJungseongPair(mHangulCharBuffer[1])){
                mHangulCharBuffer[1] = resolveJungseongPair(mHangulCharBuffer[1])[0];
                mHangulCharBuffer[0] == -1 ? mWorkingChar = mHangulCharBuffer[1] : mWorkingChar = makeHangul(mHangulCharBuffer);
            }else{
                mHangulCharBuffer[1] = -1;
                if(-1 != mHangulCharBuffer[0]) {
                    mCurrentState = HANGUL_CHO1;
                    mWorkingChar = mHangulCharBuffer[0];
                } else {
                    mCurrentState = HANGUL_NONE;
                    mHangulCharBuffer[0] = -1;
                    mWorkingChar = -1;
                }

                if(currentWord[currentWord.length()-1] != 0&& mHangulCharBuffer[0] != HANGUL_NONE && mPrevHangulCharBuffer[1] != HANGUL_NONE && mPrevHangulCharBuffer[0] != HANGUL_NONE){
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
        case HANGUL_JONG2:
            if(isJongseongPair(mHangulCharBuffer[2])){
                mHangulCharBuffer[2] = resolveJongseongPair(mHangulCharBuffer[2])[0];
                mCurrentState = HANGUL_JONG1;
                mWorkingChar = makeHangul(mHangulCharBuffer);
            }
            break;
        case HANGUL_JONG1:
            mCurrentState = HANGUL_JUNG1;
            mHangulCharBuffer[2] = -1;
            mWorkingChar = makeHangul(mHangulCharBuffer);
            break;

        case HANGUL_JONG1_ABLE:
        	if(mHangulCharBuffer[3] != -1){
        		mHangulCharBuffer[3] = -1;
        		mWorkingChar = -1;
        	}
        	break;

        case HANGUL_JONG2_ABLE:
            if(mHangulCharBuffer[3] != -1) {
                mHangulCharBuffer[3] = -1;
                mWorkingChar = -1;
            } else {
                mHangulCharBuffer[1] = -1;
                mWorkingChar = makeHangul(mHangulCharBuffer);

            }
            break;

        default:
        case HANGUL_NONE:
            break;

    }

    mLastFunctionKey = false;

    if(mWorkingChar != -1 && mCurrentState != HANGUL_NONE) {
        if(mWorkingChar2 == -2){
            currentWordDelete();
            mHangulCharBuffer[3] = mHangulCharBuffer[0];
            mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
            mHangulCharBuffer[1] = mPrevHangulCharBuffer[1];
            mHangulCharBuffer[2] = mPrevHangulCharBuffer[2];

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


        }
    }
}
