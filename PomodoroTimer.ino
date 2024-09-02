// 引腳定義
#define RED_LED_PIN 18
#define YELLOW_LED_PIN 19
#define GREEN_LED_PIN 21
#define BUTTON_PIN 22
#define BUZZER_PIN 12
#define TIMER_MINUTES 25  // 工作時間
#define REST_MINUTES 5    // 休息時間
#define IDLE_TIMEOUT 3    // 超時的分鐘數，超過這個時間後進入省電模式

unsigned long lastActionTime = 0;  // 上次用戶操作時間
unsigned long idleTimeoutMillis = IDLE_TIMEOUT * 60 * 1000;  // 超时时间轉換為毫秒
bool isPowerSaving = false;  // 省電模式標誌
bool wasJustWokenUp = false;  // 用於追蹤是否剛從省電模式喚醒



// 定義計時器狀態
enum TimerState {
    INIT_STATE,          // 初始狀態
    TIMING_STATE,        // 計時中
    REST_STATE,          // 休息狀態
    PAUSED_STATE,        // 暫停狀態
    FINISHED_STATE,      // 計時結束
    POWER_SAVING_STATE   // 省電模式
};

TimerState currentState = INIT_STATE;  // 初始化當前狀態為初始狀態

// 計時參數
unsigned long workTime = TIMER_MINUTES * 60 * 1000;  // 工作時間
unsigned long restTime = REST_MINUTES * 60 * 1000;   // 休息時間
unsigned long remainingTime = workTime;  // 剩餘時間
unsigned long lastUpdateTime = 0;  // 上次更新的時間
unsigned long lastFlashTime = 0;  // 上次閃爍時間
// 按鈕處理相關變數
unsigned long lastButtonPressTime = 0;  // 上次按鈕按下的時間
unsigned long buttonPressDuration = 0;  // 按鈕按下持續時間
bool isButtonPressed = false;  // 按鈕是否按下
bool isLongPressHandled = false;  // 是否已處理長按
bool yellowLedState = LOW;  // 黃燈的當前狀態
bool redLedState = LOW;  // 紅燈的當前狀態
bool greenLedState = LOW;  // 綠燈的當前狀態

// 初始化函數
void setup() {
    Serial.begin(115200);  // 初始化串口，波特率115200

    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);  // 初始状态，绿灯亮起

    currentState = INIT_STATE;  // 设置初始状态为 INIT_STATE
    lastActionTime = millis();  // 初始化时记录操作时间
    Serial.println("Setup complete");
}

// 蜂鳴器控制函數
void beep(int frequency, int duration) {
    tone(BUZZER_PIN, frequency);  // 開始發聲
    delay(duration);              // 持續時間
    noTone(BUZZER_PIN);           // 停止發聲
}

void shortBeep() {
    beep(1000, 100);  // 短促的蜂鳴聲
}

void doubleBeep() {
    for (int i = 0; i < 2; i++) {
        beep(1000, 100);  // 鳴響一次，頻率1000Hz，持續100毫秒
        delay(100);       // 每次鳴響間隔100毫秒
    }
}

void tripleQuickBeep() {
    for (int i = 0; i < 3; i++) {
        beep(1000, 100);  // 鳴響一次，頻率1000Hz，持續100毫秒
        delay(100);       // 每次鳴響間隔100毫秒
    }
}

void longBeep() {
    beep(1000, 1000);  // 鳴響一次，頻率1000Hz，持續1秒
}

// 輸出當前狀態和剩餘時間
void printStateAndTime() {
    Serial.print("Current State: ");
    switch (currentState) {
        case INIT_STATE:
            Serial.println("Initial");
            break;
        case TIMING_STATE:
            Serial.println("Timing");
            break;
        case REST_STATE:
            Serial.println("Resting");
            break;
        case PAUSED_STATE:
            Serial.println("Paused");
            break;
        case FINISHED_STATE:
            Serial.println("Finished");
            break;
        case POWER_SAVING_STATE:
            Serial.println("Power Saving");
            break;
    }

    if (currentState == TIMING_STATE || currentState == REST_STATE || currentState == PAUSED_STATE) {
        Serial.print("Remaining Time: ");
        Serial.print(remainingTime / 60000);  // 分鐘
        Serial.print("m ");
        Serial.print((remainingTime % 60000) / 1000);  // 秒
        Serial.println("s");
    }
}

TimerState previousState;  // 用于保存暂停前的状态

void handleButtonPress() {
    bool currentButtonState = digitalRead(BUTTON_PIN) == HIGH;  // 假设按钮未按下时为高电平，按下时为低电平
    if (currentButtonState && !isButtonPressed) {  // 按钮刚被按下
        isButtonPressed = true;
        lastButtonPressTime = millis();
        isLongPressHandled = false;
        Serial.println("Button pressed");

        if (isPowerSaving) {  // 如果在省电模式下被按下
            isPowerSaving = false;
            wasJustWokenUp = true;  // 标记为刚刚唤醒
            currentState = INIT_STATE;
            digitalWrite(GREEN_LED_PIN, HIGH);  // 恢复绿灯亮起
            lastActionTime = millis();  // 重置计时器
            Serial.println("Woke up from power saving mode");
            printStateAndTime();
            return;  // 结束当前函数，避免继续执行其他逻辑
        }
    } else if (!currentButtonState && isButtonPressed) {  // 按钮刚松开
        isButtonPressed = false;
        buttonPressDuration = millis() - lastButtonPressTime;
        Serial.print("Button released after ");
        Serial.print(buttonPressDuration);
        Serial.println("ms");

        // 唤醒后应只进入初始状态
        if (wasJustWokenUp) {
            wasJustWokenUp = false;  // 清除标志，回到 INIT_STATE
            Serial.println("Button release after waking up, returning to INIT_STATE");
            return;  // 确保设备保持在 INIT_STATE，等待用户再次按下按钮
        } else if (buttonPressDuration < 2000) {
            if (currentState == INIT_STATE) {
                currentState = TIMING_STATE;
                remainingTime = workTime;  // 重置倒计时
                shortBeep();  // 从绿灯切换到红灯，短鸣一声
                digitalWrite(GREEN_LED_PIN, LOW);
                digitalWrite(RED_LED_PIN, HIGH);
                printStateAndTime();  // 输出状态
            } else if (currentState == TIMING_STATE || currentState == REST_STATE) {  
                previousState = currentState;  // 保存当前状态
                currentState = PAUSED_STATE;
                doubleBeep();  // 双短鸣提示进入暂停状态
                digitalWrite(RED_LED_PIN, LOW);
                digitalWrite(YELLOW_LED_PIN, LOW);  // 暂停时关闭所有灯
                lastUpdateTime = millis();  // 重置时间标记
                printStateAndTime();  // 输出状态
            } else if (currentState == PAUSED_STATE) {
                // 恢复到之前的状态和灯号
                if (previousState == TIMING_STATE) {  
                    currentState = TIMING_STATE;
                    digitalWrite(YELLOW_LED_PIN, LOW);  // 确保黄灯关闭
                    digitalWrite(RED_LED_PIN, HIGH);  // 只亮红灯
                } else if (previousState == REST_STATE) {  
                    currentState = REST_STATE;
                    digitalWrite(RED_LED_PIN, LOW);  // 确保红灯关闭
                    digitalWrite(YELLOW_LED_PIN, HIGH);  // 只亮黄灯
                }
                shortBeep();  // 恢复时短鸣一声
                printStateAndTime();
            }
        }
    } else if (isButtonPressed && (millis() - lastButtonPressTime > 2000) && !isLongPressHandled) {  // 长按
        currentState = INIT_STATE;
        longBeep();  // 提示重置系统
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);  // 绿灯亮起
        lastActionTime = millis();  // 长按后也更新操作时间
        printStateAndTime();  // 输出状态
        isLongPressHandled = true;
    }
}


void checkAndEnterPowerSavingMode() {
    unsigned long currentMillis = millis();

    // 只有在 INIT_STATE 且超过设定的闲置时间，且没有其他状态变更时才进入省电模式
    if (currentState == INIT_STATE && (currentMillis - lastActionTime >= idleTimeoutMillis)) {
        Serial.println("Entering power saving mode...");
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        currentState = POWER_SAVING_STATE;  // 确保状态被设置为 POWER_SAVING_STATE
        isPowerSaving = true;
    }
}


void loop() {
    if (isPowerSaving && wasJustWokenUp) {
        wasJustWokenUp = false;  // 清除唤醒标志
        currentState = INIT_STATE;  // 确保设备进入初始状态
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        return;  // 停止后续操作
    }

    handleButtonPress();  // 处理按钮操作

    if (currentState != POWER_SAVING_STATE) {  // 检查是否应进入省电模式
        checkAndEnterPowerSavingMode();
    }

    unsigned long currentMillis = millis();

    if (currentState == TIMING_STATE && currentMillis - lastUpdateTime >= 1000) {
        lastUpdateTime = currentMillis;
        remainingTime -= 1000;
        printStateAndTime();

        if (remainingTime <= 0) {
            currentState = REST_STATE;
            remainingTime = restTime;
            doubleBeep();
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, HIGH);
            printStateAndTime();
        }
    } else if (currentState == REST_STATE && currentMillis - lastUpdateTime >= 1000) {
        lastUpdateTime = currentMillis;
        remainingTime -= 1000;
        printStateAndTime();

        if (remainingTime <= 0) {
            currentState = FINISHED_STATE;
            tripleQuickBeep();
            lastFlashTime = currentMillis;
        }
    }

    if (currentState == PAUSED_STATE && currentMillis - lastFlashTime >= 500) {
        lastFlashTime = currentMillis;
        redLedState = !redLedState;
        yellowLedState = !redLedState;
        digitalWrite(RED_LED_PIN, redLedState);
        digitalWrite(YELLOW_LED_PIN, yellowLedState);
    }

    if (currentState == FINISHED_STATE && currentMillis - lastFlashTime >= 500) {
        lastFlashTime = currentMillis;
        redLedState = !redLedState;
        digitalWrite(RED_LED_PIN, redLedState);
        digitalWrite(YELLOW_LED_PIN, LOW);
    }
}
