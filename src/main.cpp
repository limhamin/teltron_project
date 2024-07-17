#include <Arduino.h>
#include <stdio.h>

//7세그먼트 PIN PORT
#define CLK_PIN GPIO_NUM_22  // 시계 신호를 보낼 핀
#define DIO_PIN GPIO_NUM_21  // 데이터를 보낼 핀
//RADAR SENSOR PIN PORT
#define RAD_OR GPIO_NUM_34 
#define RAD_DET GPIO_NUM_35
#define RAD_NRST GPIO_NUM_27
#define RAD_CS GPIO_NUM_25
#define RAD_CE GPIO_NUM_26
#define RAD_MOSI GPIO_NUM_23    
#define RAD_MISO GPIO_NUM_19
#define RAD_SCK GPIO_NUM_18

//LORA pin PORT
#define LOR_DIO2 GPIO_NUM_39
#define LOR_NCS GPIO_NUM_32
#define LOR_DIDO GPIO_NUM_15
#define LOR_NRST GPIO_NUM_5
#define LOR_NINT GPIO_NUM_33
//ETH PIN PORT
#define ETH_NINT GPIO_NUM_36
#define ETH_L_SCK GPIO_NUM_14
#define ETH_L_MISO GPIO_NUM_12
#define ETH_L_MOSI GPIO_NUM_13
#define ETH_NCS GPIO_NUM_4
#define ETH_NRST GPIO_NUM_15
// 숫자에 해당하는 7세그먼트

const uint8_t segmentMap[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

void StartSignal() {
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(DIO_PIN, HIGH);
    digitalWrite(DIO_PIN, LOW);
    digitalWrite(CLK_PIN, LOW);
}

void StopSignal() {
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIO_PIN, LOW);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(DIO_PIN, HIGH);
}

void sendData(uint8_t data) {
    for (int i = 0; i < 8; ++i) {
        digitalWrite(CLK_PIN, LOW);
        digitalWrite(DIO_PIN, data & 0x01 ? HIGH : LOW);
        data >>= 1;
        digitalWrite(CLK_PIN, HIGH);
    }
    digitalWrite(CLK_PIN, LOW);
    pinMode(DIO_PIN, INPUT);
    digitalWrite(CLK_PIN, HIGH);
    pinMode(DIO_PIN, OUTPUT);
}

void number(int number) {
    StartSignal();
    sendData(0x40);  // 데이터 명령 설정, 자동 주소 증가 모드
    StopSignal();
    
    StartSignal();
    sendData(0xC0);  

    for (int i = 0; i < 4; ++i) {
        sendData(segmentMap[number % 10]);
        number /= 10;
    }
    StopSignal();

    StartSignal();
    sendData(0x8F);  //
    StopSignal();
}

void setup() {
    pinMode(CLK_PIN, OUTPUT);
    pinMode(DIO_PIN, OUTPUT);
    number(1234); 
    delay(10); // 처음에 1234 표시
}

void loop() {
}