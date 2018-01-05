#include "Automata.h"

#include "../../BaseAutomata.h"
#include "../../../utils/LogUtils.h"
#include "../../../utils/Utils.h"
#include "../../IAutomata.h"

//ㄱ ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
const wchar_t Automata::PREF_CHO[19] = {
		12593, 12594, 12596, 12599, 12600, 12601, 12609, 12610, 12611, 12613, 12614, 12615, 12616, 12617, 12618, 12619, 12620, 12621, 12622
	};
	//ㅏ ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ
const wchar_t Automata::PREF_JUNG[21] = {
		12623, 12624, 12625, 12626, 12627, 12628, 12629, 12630, 12631, 12632, 12633, 12634, 12635, 12636, 12637, 12638, 12639, 12640, 12641, 12642, 12643
	};
	//ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
const wchar_t Automata::PREF_JONG[27] = {
		12593, 12594, 12595, 12596, 12597, 12598, 12599, 12601, 12602, 12603, 12604, 12605, 12606, 12607, 12608, 12609, 12610, 12612, 12613, 12614, 12615, 12616, 12618, 12619, 12620, 12621, 12622
	};

Automata::Automata() {
	reset();
	isHangle = true;
	isTimeToggle = true;
}

void Automata::reset() {
	LOGD("Automata::reset()");
	BaseAutomata::reset();
    resetState();
}

void Automata::resetState(){
	LOGD("Automata::resetState()");

    for(int i = 0; i < LEN(mHangulCharBuffer); i++) {
        mHangulCharBuffer[i] = HANGUL_NONE;
    }

    mCurrentState  = HANGUL_NONE;
    mWorkingChar   = HANGUL_NONE;
    mWorkingChar2  = HANGUL_NONE;
}

void Automata::resetState(int &state, int *buffer){
	for(int i = 0; i < LEN(buffer); i++){
		buffer[i] = HANGUL_NONE;
	}
	state = HANGUL_NONE;
}

int Automata::getChoseongIndex(int primaryCode) {
    for(int i = 0; i < sizeof(PREF_CHO) / sizeof(PREF_CHO[0]); i++) {
        if(primaryCode == PREF_CHO[i]) {
            return i;
        }
    }

    return -1;
}

int Automata::getJungseongIndex(int primaryCode) {
    for(int i = 0; i < sizeof(PREF_JUNG) / sizeof(PREF_JUNG[0]); i++) {
        if(primaryCode == PREF_JUNG[i]) {
            return i;
        }
    }

    return -1;
}

int Automata::getJongseongIndex(int primaryCode) {
    for(int i = 0; i < sizeof(PREF_JONG) / sizeof(PREF_JONG[0]); i++) {
        if(primaryCode == PREF_JONG[i]) {
            return i;
        }
    }

    return -1;
}

void Automata::setBuffer(int primaryCode, int &state, int *buffer) {
    resetState(state, buffer);
    mWorkingChar = HANGUL_NONE;
    mWorkingChar2 = HANGUL_NONE;

    if(!isHangul(primaryCode)) {
        mWorkingChar = primaryCode;
        return;
    }

    if((primaryCode >= L'ㄱ' && primaryCode <= L'ㅣ') || primaryCode == HANGUL_DOT || primaryCode == HANGUL_DOUBLE_DOT) {
        /*int*/wchar_t codes[1] = { (wchar_t)primaryCode };
        std::wstring strPrimaryCode = std::wstring(codes, 0, 1);
        std::wstring cPrimaryCode = strPrimaryCode.substr(0, strPrimaryCode.length());

        if(mChoseong.find(cPrimaryCode) != string::npos) {
            state = HANGUL_CHO1;
            buffer[0] = primaryCode;
        } else if(mJungseong.find(cPrimaryCode) != string::npos || primaryCode == HANGUL_DOT || primaryCode == HANGUL_DOUBLE_DOT) {
        	state = HANGUL_JUNG1;
        	buffer[1] = primaryCode;
        } else {
        	buffer[2] = primaryCode;
        }
    } else if(primaryCode >= L'가' && primaryCode <= L'힣') {
        int purePrimaryCode = primaryCode - L'가';
		int jongIdx = purePrimaryCode % 28;
        int divJong = purePrimaryCode / 28;

        if(jongIdx > 0) {
        	buffer[2] = PREF_JONG[jongIdx - 1];
        } else {
        	buffer[2] = HANGUL_NONE;
        }

        int divJung = divJong / 21;
        jongIdx = divJong % 21;
        buffer[1] = PREF_JUNG[jongIdx];

        jongIdx = divJung % 19;
        buffer[0] = PREF_CHO[jongIdx];

        if(buffer[2] != HANGUL_NONE) {
            if(isJongseongPair(buffer[2])) {
            	state = HANGUL_JONG2;
            } else {
            	state = HANGUL_JONG1;
            }
        } else if(buffer[1] != HANGUL_NONE) {
        	state = HANGUL_JUNG1;
        }
    }
}

int Automata::makeHangul(int buffer[]) {
    if(buffer[1] == HANGUL_DOT || buffer[1] == HANGUL_DOUBLE_DOT) {
        return -1;
    }
    return 0xAC00 + getChoseongIndex(buffer[0]) * 588 + getJungseongIndex(buffer[1]) * 28 + ((-1 != buffer[2]) ? (getJongseongIndex(buffer[2]) + 1) : 0);
}

void Automata::currentWordAppend(wchar_t wc) {
    currentWord += wc;
    LOGDWS("[apd]currentWord = %s", currentWord.c_str());
//    composingWord += wc;
//    LOGDWS("[apd]composingWord = %s", composingWord.c_str());
}

void Automata::currentWordDelete() {
    int len = currentWord.length();
    if(len > 0) {
        currentWord = currentWord.substr(0, len - 1);
    }
//    len = composingWord.length();
//    if(len > 0) {
//        composingWord = composingWord.substr(0, len - 1);
//    }
    LOGDWS("[del]currentWord = %s", currentWord.c_str());
//    LOGDWS("[del]composingWord = %s", composingWord.c_str());
}

wstring Automata::getComposingWord(){
	LOGDWS("Automata::getComposingWord():%s", currentWord.c_str());
	if(isHangle)
		return currentWord;

	return L"";
}

wstring Automata::getCurrentWord(){
	LOGDWS("Automata::getCurrentWord():%s", currentWord.c_str());
	return currentWord;
}

void Automata::deleteAllChar(){
	resetState();
	BaseAutomata::deleteAllChar();
}

int Automata::appendChar(const wchar_t c) {
    LOGDWC("Automata::appendChar() is called..., c = %s", c);
    return BaseAutomata::appendChar(c);
}


bool Automata::isHangul(wchar_t const primaryCode) {
    if ((primaryCode >= UnicodeHanGulStart && primaryCode <= UnicodeHanGulLast) || (primaryCode >= UnicodeHanGulJamoStart && primaryCode <= UnicodeHanGulJamoLast) || primaryCode == HANGUL_DOT || primaryCode == HANGUL_DOUBLE_DOT) {
    	isHangle = true;
        return true;
    }
    isHangle = false;
    return false;
}

void Automata::saveUnknownCharacter(int const state, int const primaryCode) {
    currentWordAppend(primaryCode);
    resetState();
    mWorkingChar = primaryCode;
}

bool Automata::isJungseongPair(int v) {
    switch(v) {
        case 12632: //ㅘ
        case 12633: //ㅙ
        case 12634: //ㅚ
        case 12637: //ㅝ
        case 12638: //ㅞ
        case 12639: //ㅟ
        case 12642: //ㅢ
            return true;
        default:
            return false;
    }
    return false;
}

bool Automata::isJongseongPair(int v) {
    switch(v) {
        case 12595: //ㄳ
        case 12597: //ㄵ
        case 12598: //ㄶ
        case 12602: //ㄺ
        case 12603: //ㄻ
        case 12604: //ㄼ
        case 12605: //ㄽ
        case 12606: //ㄾ
        case 12607: //ㄿ
        case 12608: //ㅀ
        case 12612: //ㅄ
            return true;
        default:
            return false;
    }
	return false;
}

static int r[2] = {0,};
int *Automata::resolveJungseongPair(int v){
    r[0] = HANGUL_NONE;
    r[1] = HANGUL_NONE;
    switch(v){
        case 12632: //ㅘ
            r[0] = 12631;   //ㅗ
            r[1] = 12623;   //ㅏ
            break;
        case 12633: //ㅙ
            r[0] = 12631;   //ㅗ
            r[1] = 12624;   //ㅐ
            break;
        case 12634: //ㅚ
            r[0] = 12631;   //ㅗ
            r[1] = 12643;   //ㅣ
            break;
        case 12637: //ㅝ
            r[0] = 12636;   //ㅜ
            r[1] = 12627;   //ㅓ
            break;
        case 12638: //ㅞ
            r[0] = 12636;   //ㅜ
            r[1] = 12628;   //ㅔ
            break;
        case 12639: //ㅟ
            r[0] = 12636;   //ㅜ
            r[1] = 12643;   //ㅣ
            break;
        case 12642: //ㅢ
            r[0] = 12641;   //ㅡ
            r[1] = 12643;   //ㅣ
            break;
        default:
            r[0] = -1;
            r[1] = -1;
            break;
    }
    return r;
}

int *Automata::resolveJongseongPair(int v) {
    r[0] = HANGUL_NONE;
    r[1] = HANGUL_NONE;
    //int r[2];
    switch(v) {
        case 12595: //ㄳ
            r[0] = 12593;   //ㄱ
            r[1] = 12613;   //ㅅ
            break;
        case 12597: //ㄵ
            r[0] = 12596;   //ㄴ
            r[1] = 12616;   //ㅈ
            break;
        case 12598: //ㄶ
            r[0] = 12596;   //ㄴ
            r[1] = 12622;   //ㅎ
            break;
        case 12602: //ㄺ
            r[0] = 12601;   //ㄹ
            r[1] = 12593;   //ㄱ
            break;
        case 12603: //ㄻ
            r[0] = 12601;   //ㄹ
            r[1] = 12609;   //ㅁ
            break;
        case 12604: //ㄼ
            r[0] = 12601;   //ㄹ
            r[1] = 12610;   //ㅂ
            break;
        case 12605: //ㄽ
            r[0] = 12601;   //ㄹ
            r[1] = 12613;   //ㅅ
            break;
        case 12606: //ㄾ
            r[0] = 12601;   //ㄹ
            r[1] = 12620;   //ㅌ
            break;
        case 12607: //ㄿ
            r[0] = 12601;   //ㄹ
            r[1] = 12621;   //ㅍ
            break;
        case 12608: //ㅀ
            r[0] = 12601;   //ㄹ
            r[1] = 12622;   //ㅎ
            break;
        case 12612: //ㅄ
            r[0] = 12610;   //ㅂ
            r[1] = 12613;   //ㅅ
            break;
        default:
            r[0] = -1;
            r[1] = -1;
            break;
    }

    return r;
}

int Automata::getJongseongPair(int v1, int v2) {
    switch(v1) {
        case 12593: //ㄱ
            switch(v2) {
                case 12613: //ㅅ
                    return 12595;   //ㄳ
                default:
                    break;
            }
            break;
        case 12596: //ㄴ
            switch(v2) {
                case 12616: //ㅈ
                    return 12597;   //ㄵ
                case 12622: //ㅎ
                    return 12598;   //ㄶ
                default:
                    break;
            }
            break;
        case 12601: //ㄹ
            switch(v2) {
                case 12593: //ㄱ
                    return 12602;   //ㄺ
                case 12609: //ㅁ
                    return 12603;   //ㄻ
                case 12610: //ㅂ
                    return 12604;   //ㄼ
                case 12613: //ㅅ
                    return 12605;   //ㄽ
                case 12620: //ㅌ
                    return 12606;   //ㄾ
                case 12621: //ㅍ
                    return 12607;   //ㄿ
                case 12622: //ㅎ
                    return 12608;   //ㅀ
                default:
                    break;
            }
            break;
        case 12610: //ㅂ
            switch(v2) {
                case 12613: //ㅅ
                    return 12612;   //ㅄ
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return -1;
}

void Automata::cursorChange(wstring beforeString, wstring afterString) {
	LOGW(">> Automata::cursorChange()");
    resetState();

    BaseAutomata::cursorChange(beforeString, afterString);
}

int Automata::appendSeparator(wchar_t const unicode) { //, IComposer *const composer) {
	LOGW(">> Automata::appendSeparator()");
	if(unicode == KeyCode::KEY_TYPE_BASIC_SEPARATOR){
		isTimeToggle = false;
		if(currentWord.length() > 0){
			currentWord.erase(currentWord.begin(), currentWord.end()-1);
			setBuffer(currentWord[currentWord.length()-1], mCurrentState, mHangulCharBuffer);
			mWorkingChar = currentWord[currentWord.length()-1];
		}
		return 0;
	} else {
		resetState();
		isTimeToggle = true;
		return BaseAutomata::appendSeparator(unicode);
	}
}

void Automata::selectPrediction(wstring* predictionWord) {
	LOGW(">> Automata::selectPrediction()");
	BaseAutomata::selectPrediction(predictionWord);
    resetState();
}

int Automata::saveCharacter(int state, int primaryCode, int kind) {
    if (mWorkingChar != HANGUL_NONE) {

        if (isHangul(mWorkingChar) || isDot(mWorkingChar)) {
            if (getCurrentWord().length() > 0) {
                currentWordDelete();
            }
        }
    }

    int completedChar = HANGUL_NONE;
    mWorkingChar2 = HANGUL_NONE;
    int *jong;
    int temp;

    switch(state) {
        case HANGUL_CHO1:
            mHangulCharBuffer[0] = primaryCode;
            mWorkingChar = mHangulCharBuffer[0];
            break;

        case HANGUL_CHO1_FINISH:
            completedChar = mHangulCharBuffer[0];
            mHangulCharBuffer[0] = primaryCode;
            mWorkingChar = mHangulCharBuffer[0];
            mCurrentState = HANGUL_CHO1;
            break;

        case HANGUL_JUNG_DOT:{ // CheonJiIn, CheonJiInPlus
            int mPrevHangulCharBuffer[3] = {-1,};
            int mPreviousState = HANGUL_NONE;
            setBuffer(currentWord[currentWord.length()-1], mPreviousState, mPrevHangulCharBuffer);
            if(currentWord.length() > 0 && kind == HANGUL_MO && mPrevHangulCharBuffer[0] != HANGUL_NONE && !isDot(primaryCode)){
                if(mPreviousState == HANGUL_CHO1){
                    mHangulCharBuffer[0] = mPrevHangulCharBuffer[0];
                    mHangulCharBuffer[1] = primaryCode;
                    mWorkingChar = makeHangul(mHangulCharBuffer);
                    completedChar = -2;
                    mCurrentState = HANGUL_JUNG1;
                } else if(mPreviousState == HANGUL_JUNG1){
                    mHangulCharBuffer[0] = HANGUL_NONE;
                    mHangulCharBuffer[1] = primaryCode;
                    mWorkingChar = mHangulCharBuffer[1];
                    mHangulCharBuffer[2] = HANGUL_NONE;
                    mHangulCharBuffer[3] = HANGUL_NONE;
                } else if(mPreviousState == HANGUL_JONG1){
                    completedChar = -2;
                    mHangulCharBuffer[0] = mPrevHangulCharBuffer[2];
                    mPrevHangulCharBuffer[2] = HANGUL_NONE;
                    mWorkingChar = makeHangul(mPrevHangulCharBuffer);
                    mHangulCharBuffer[1] = primaryCode;
                    mHangulCharBuffer[2] = HANGUL_NONE;
                    mHangulCharBuffer[3] = HANGUL_NONE;
                    mWorkingChar2 = makeHangul(mHangulCharBuffer);
                    mCurrentState = HANGUL_JUNG1;
                } else if(mPreviousState == HANGUL_JONG2){
                    jong = resolveJongseongPair(mPrevHangulCharBuffer[2]);
                    completedChar = -2;
                    mPrevHangulCharBuffer[2] = jong[0];
                    mWorkingChar = makeHangul(mPrevHangulCharBuffer);
                    mHangulCharBuffer[0] = jong[1];
                    mHangulCharBuffer[1] = primaryCode;
                    mHangulCharBuffer[2] = HANGUL_NONE;
                    mHangulCharBuffer[3] = HANGUL_NONE;
                    mWorkingChar2 = makeHangul(mHangulCharBuffer);
                    mCurrentState = HANGUL_JUNG1;
                } else {
                    mHangulCharBuffer[1] = primaryCode;
                    mWorkingChar = mHangulCharBuffer[1];
                    mCurrentState = HANGUL_JUNG1;
                }
            } else {
                if(isDot(primaryCode)) {
                    if(mHangulCharBuffer[2] != HANGUL_NONE) {
                        if(isDot(mHangulCharBuffer[3])) {
                            mHangulCharBuffer[3] = primaryCode;
                            mWorkingChar = mHangulCharBuffer[3];
                        } else {
                            mHangulCharBuffer[3] = primaryCode;
                            mWorkingChar = makeHangul(mHangulCharBuffer);
                            mWorkingChar2 = mHangulCharBuffer[3];
                        }
                    } else {
                        mHangulCharBuffer[1] = primaryCode;
                        if(-1 != mHangulCharBuffer[0]) {

                            mWorkingChar = makeHangul(mHangulCharBuffer);
                            if(mWorkingChar == HANGUL_NONE) {
                                mWorkingChar = mHangulCharBuffer[1];
                            } else {
                                completedChar = -2;
                            }
                        } else {
                            mWorkingChar = mHangulCharBuffer[1];
                        }
                        mCurrentState = HANGUL_JUNG1;
                    }
                } else {
                    if(mHangulCharBuffer[2] != -1) {
                        // 종음이 복종음일때
                        if(isJongseongPair(mHangulCharBuffer[2])) {
                            if(isDot(mHangulCharBuffer[3])){
                                mWorkingChar = mHangulCharBuffer[3];
                                mHangulCharBuffer[0] = primaryCode;
                                mHangulCharBuffer[1] = HANGUL_NONE;
                                mHangulCharBuffer[2] = HANGUL_NONE;
                                mHangulCharBuffer[3] = HANGUL_NONE;
                                mWorkingChar2 = mHangulCharBuffer[0];
                                mCurrentState = HANGUL_CHO1;
                            } else {
                                jong = resolveJongseongPair(mHangulCharBuffer[2]);
                                mHangulCharBuffer[2] = jong[0];
                                completedChar = -2;
                                mWorkingChar = makeHangul(mHangulCharBuffer);
                                mHangulCharBuffer[0] = jong[1];
                                mHangulCharBuffer[1] = primaryCode;
                                mHangulCharBuffer[2] = HANGUL_NONE;
                                mHangulCharBuffer[3] = HANGUL_NONE;
                                mWorkingChar2 = makeHangul(mHangulCharBuffer);

                                mCurrentState = HANGUL_JUNG1;
                            }
                        } else {
                            if(isDot(mHangulCharBuffer[3])){
                                mWorkingChar = mHangulCharBuffer[3];
                                mHangulCharBuffer[0] = primaryCode;
                                mHangulCharBuffer[1] = HANGUL_NONE;
                                mHangulCharBuffer[2] = HANGUL_NONE;
                                mHangulCharBuffer[3] = HANGUL_NONE;
                                mWorkingChar2 = mHangulCharBuffer[0];
                                mCurrentState = HANGUL_CHO1;
                            } else {
                                temp = mHangulCharBuffer[2];
                                // 단종음 일때
                                mHangulCharBuffer[2] = HANGUL_NONE;
                                completedChar = -2;
                                mWorkingChar = makeHangul(mHangulCharBuffer);
                                mHangulCharBuffer[0] = temp;
                                mHangulCharBuffer[1] = primaryCode;
                                mHangulCharBuffer[2] = HANGUL_NONE;
                                mHangulCharBuffer[3] = HANGUL_NONE;
                                mWorkingChar2 = makeHangul(mHangulCharBuffer);

                                mCurrentState = HANGUL_JUNG1;
                            }
                        }
                    } else {
                        mHangulCharBuffer[1] = primaryCode;
                        if(-1 != mHangulCharBuffer[0]) {
                            mWorkingChar = makeHangul(mHangulCharBuffer);
                            if(mWorkingChar == HANGUL_NONE) {
                                mWorkingChar = mHangulCharBuffer[1];
                            } else {
                                completedChar = -2;
                            }
                        } else {
                            mWorkingChar = mHangulCharBuffer[1];
                        }
                        mCurrentState = HANGUL_JUNG1;
                    }
                }
            }

            break;
        }

        case HANGUL_JUNG1:
            mHangulCharBuffer[1] = primaryCode;
            if(mHangulCharBuffer[0] != HANGUL_NONE) { // 초성이 있을 때
                mWorkingChar = makeHangul(mHangulCharBuffer);
                if(mWorkingChar == HANGUL_NONE) { // DOT 특수처리 'ㄱㆍ' 상태
                    completedChar = mHangulCharBuffer[0];
                    mWorkingChar = mHangulCharBuffer[1];
                }
            } else { // 초성이 없을 때
                mWorkingChar = mHangulCharBuffer[1];
            }
            break;

        case HANGUL_JUNG1_FINISH:
            if(mHangulCharBuffer[0] == HANGUL_NONE) {
                if(kind == HANGUL_JA){ // CheonJiIn, CheonJiInPlus 첫 입력 시 ㆍ 입력 후 모음 입력 할 때
                    completedChar = mHangulCharBuffer[1];
                    mHangulCharBuffer[1] = HANGUL_NONE;
                    mHangulCharBuffer[0] = primaryCode;
                    mWorkingChar = mHangulCharBuffer[0];
                    mCurrentState = HANGUL_CHO1;
                } else {
                    completedChar = mHangulCharBuffer[1];
                    mHangulCharBuffer[1] = primaryCode;
                    mWorkingChar = mHangulCharBuffer[1];
                    mCurrentState = HANGUL_JUNG1;
                }
            } else {
                if(mHangulCharBuffer[1] == HANGUL_DOT || mHangulCharBuffer[1] == HANGUL_DOUBLE_DOT){ // CheonJiIn, CheonJiInPlus  입력 중 ㆍ뒤에 모음이 입력되는 경우
                    completedChar = mHangulCharBuffer[1];
                    mHangulCharBuffer[0] = primaryCode;
                    mHangulCharBuffer[1] = HANGUL_NONE;
                    mWorkingChar = mHangulCharBuffer[0];
                    mCurrentState = HANGUL_CHO1;
                } else {
                    completedChar = makeHangul(mHangulCharBuffer);
                    mHangulCharBuffer[0] = HANGUL_NONE;
                    mHangulCharBuffer[1] = primaryCode;
                    mWorkingChar = mHangulCharBuffer[1];
                    mCurrentState = HANGUL_JUNG1;
                }
            }
            break;

        case HANGUL_JONG1:
            mHangulCharBuffer[2] = primaryCode;
            if(getJongseongIndex(primaryCode) >= 0) {
                if(mHangulCharBuffer[0] != HANGUL_NONE && mHangulCharBuffer[1] != HANGUL_NONE) {
                    mWorkingChar = makeHangul(mHangulCharBuffer);
                    if(mWorkingChar == HANGUL_NONE){ // dot, double_dot
                        mHangulCharBuffer[0] = HANGUL_NONE;
                        completedChar = mHangulCharBuffer[1];
                        mWorkingChar = primaryCode;
                    }
                } else {
                    mCurrentState = HANGUL_CHO1;
                    completedChar = mHangulCharBuffer[1];
                    mHangulCharBuffer[0] = mHangulCharBuffer[2];
                    mHangulCharBuffer[1] = HANGUL_NONE;
                    mHangulCharBuffer[2] = HANGUL_NONE;
                    mHangulCharBuffer[3] = HANGUL_NONE;
                    mWorkingChar = mHangulCharBuffer[0];
                }
            } else {
                mCurrentState = HANGUL_JONG1_ABLE;
                mHangulCharBuffer[3] = primaryCode;
                mWorkingChar = makeHangul(mHangulCharBuffer);
                mWorkingChar2 = mHangulCharBuffer[3];
            }
            break;

        case HANGUL_JONG1_FINISH:
            if(kind == HANGUL_JA) {
                mCurrentState = HANGUL_CHO1;
                if(mHangulCharBuffer[0] != HANGUL_NONE && mHangulCharBuffer[1] != HANGUL_NONE) {
                    completedChar = makeHangul(mHangulCharBuffer);
                }
                mHangulCharBuffer[0] = primaryCode;
                mHangulCharBuffer[1] = HANGUL_NONE;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = mHangulCharBuffer[0];
            } else {
                mCurrentState = HANGUL_JUNG1;
                temp = mHangulCharBuffer[2];
                mHangulCharBuffer[2] = HANGUL_NONE;
                if(mHangulCharBuffer[0] != HANGUL_NONE && mHangulCharBuffer[1] != HANGUL_NONE) {
                    completedChar = makeHangul(mHangulCharBuffer);
                }
                mHangulCharBuffer[0] = temp;
                mHangulCharBuffer[1] = primaryCode;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = makeHangul(mHangulCharBuffer);
            }
            break;

        case HANGUL_JONG1_ABLE:
            mHangulCharBuffer[2] = primaryCode;
            mHangulCharBuffer[3] = HANGUL_NONE;
            mWorkingChar = makeHangul(mHangulCharBuffer);
            completedChar = -2;
            mCurrentState = HANGUL_JONG1;
            break;

        case HANGUL_JONG1_ABLE_FINISH:
            if(kind == HANGUL_JA) {
                completedChar = mHangulCharBuffer[3];
                mHangulCharBuffer[0] = primaryCode;
                mHangulCharBuffer[1] = HANGUL_NONE;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = mHangulCharBuffer[0];
                mCurrentState = HANGUL_CHO1;
            } else {
                mCurrentState = HANGUL_JUNG1;
                completedChar = HANGUL_NONE;
                mHangulCharBuffer[0] = mHangulCharBuffer[3];
                mHangulCharBuffer[1] = primaryCode;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = makeHangul(mHangulCharBuffer);
                if(mWorkingChar == HANGUL_NONE){
                    mWorkingChar = mHangulCharBuffer[0];
                    mWorkingChar2 = mHangulCharBuffer[1];
                    mHangulCharBuffer[2] = HANGUL_NONE;
                    mHangulCharBuffer[3] = HANGUL_NONE;
                }
            }
            break;

        case HANGUL_JONG2:
            if(isJongseongPair(mHangulCharBuffer[2])) { //복합중성이면
                jong = resolveJongseongPair(mHangulCharBuffer[2]);
                temp = getJongseongPair(jong[0], primaryCode);
                if(temp >= 0) {
                    mHangulCharBuffer[2] = temp;
                    if(HANGUL_NONE != mHangulCharBuffer[0] && HANGUL_NONE != mHangulCharBuffer[1]) {
                        mWorkingChar = makeHangul(mHangulCharBuffer);
                    }
                } else {
                    mHangulCharBuffer[2] = jong[0];
                    if(HANGUL_NONE != mHangulCharBuffer[0] && HANGUL_NONE != mHangulCharBuffer[1]) {
                        mWorkingChar = makeHangul(mHangulCharBuffer);
                        mHangulCharBuffer[3] = primaryCode;
                        mWorkingChar2 = mHangulCharBuffer[3];
                        mCurrentState = HANGUL_JONG2_ABLE;
                    }
                }
            } else {
                mHangulCharBuffer[2] = getJongseongPair(mHangulCharBuffer[2], primaryCode);
                if(HANGUL_NONE != mHangulCharBuffer[0] && HANGUL_NONE != mHangulCharBuffer[1]) {
                    if(mHangulCharBuffer[3] != HANGUL_NONE) {
                        completedChar = -2; // -> 을+ㅅ error로 리마크
                    }
                    mWorkingChar = makeHangul(mHangulCharBuffer);
                    mHangulCharBuffer[3] = HANGUL_NONE;
                } else {
                    // mCurrentState = HANGUL_CHO2;
                    completedChar = mHangulCharBuffer[1];
                    mHangulCharBuffer[0] = mHangulCharBuffer[2];
                    mHangulCharBuffer[1] = HANGUL_NONE;
                    mHangulCharBuffer[2] = HANGUL_NONE;
                    mWorkingChar = mHangulCharBuffer[0];
                }
            }

            break;

        case HANGUL_JONG2_FINISH:
            if(kind == HANGUL_JA) {
                completedChar = makeHangul(mHangulCharBuffer);
                mHangulCharBuffer[0] = primaryCode;
                mHangulCharBuffer[1] = HANGUL_NONE;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = mHangulCharBuffer[0];
                mCurrentState = HANGUL_CHO1;
            } else {
                jong = resolveJongseongPair(mHangulCharBuffer[2]);
                mHangulCharBuffer[2] = jong[0];
                completedChar = makeHangul(mHangulCharBuffer);
                mHangulCharBuffer[0] = jong[1];
                mHangulCharBuffer[1] = primaryCode;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = makeHangul(mHangulCharBuffer);
                mCurrentState = HANGUL_JUNG1;
            }
            break;

        case HANGUL_JONG2_ABLE:
            if(kind == HANGUL_FN) { // Naragul
                int *jong = resolveJongseongPair(mHangulCharBuffer[2]);
                if (jong[0] != -1) {
                    completedChar = makeHangul(mHangulCharBuffer);
                }
                mHangulCharBuffer[3] = primaryCode;
                mWorkingChar = mHangulCharBuffer[3];
            } else {
                if(mHangulCharBuffer[3] == -1) {
                    completedChar = makeHangul(mHangulCharBuffer);
                }
                mHangulCharBuffer[3] = primaryCode;
                mWorkingChar = mHangulCharBuffer[3];
            }

            break;

        case HANGUL_JONG2_ABLE_FINISH:
            if(kind == HANGUL_FN) {
                mCurrentState = HANGUL_CHO1;
                mHangulCharBuffer[0] = primaryCode;
                mHangulCharBuffer[1] = HANGUL_NONE;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = mHangulCharBuffer[0];

            } else
            if(kind == HANGUL_JA) {
                completedChar = mHangulCharBuffer[3];
                mCurrentState = HANGUL_CHO1;
                mHangulCharBuffer[0] = primaryCode;
                mHangulCharBuffer[1] = HANGUL_NONE;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = mHangulCharBuffer[0];
            } else {
                mCurrentState = HANGUL_JUNG1;
                mHangulCharBuffer[0] = mHangulCharBuffer[3];
                mHangulCharBuffer[1] = primaryCode;
                mHangulCharBuffer[2] = HANGUL_NONE;
                mHangulCharBuffer[3] = HANGUL_NONE;
                mWorkingChar = makeHangul(mHangulCharBuffer);
                if(mWorkingChar == HANGUL_NONE) {
                    mWorkingChar = mHangulCharBuffer[0];
                    mWorkingChar2 = primaryCode;
                }
            }
            break;

        default:
            break;
    }

    if(completedChar != HANGUL_NONE) {
        if(completedChar == -2) {
            if(getCurrentWord().length() > 0) {
                currentWordDelete();
            }
        } else {
            currentWordAppend(completedChar);
        }
    }

    if(mWorkingChar != HANGUL_NONE) {
        currentWordAppend(mWorkingChar);
    }

    if(mWorkingChar2 != HANGUL_NONE) {
        currentWordAppend(mWorkingChar2);
    }

    return IAutomata::RETURN_DEFAULT_ACTION;
}

bool Automata::isDot(int v1) { // CheonJiIn, CheonJiInPlus
    if(v1 == HANGUL_DOT || v1 == HANGUL_DOUBLE_DOT) {
        return true;
    }

    return false;
}

