int cnt_btn = 3; //INT.0
int rst_btn = 2; //INT.1

int cfg_btn = 8;
int add_btn = 7;
int sub_btn = 5;

int relay = 4;
int modeLed = 6;

int DS_pin = 11; //SDI
int SHCP_pin = 10; //SCLK
int STCP_pin = 9; //LOAD


int dec_digits  [10] {3,159,37,13,153,73,65,31,1,9}; //common annode

bool add = 0;
bool sub = 0;
bool cnt = 0;
bool rst = 0;
bool cfg = 0;

bool Mode = 0; //1 config, 0 run

//Variables contador
int preset = 10;
int count = preset;

int x = preset;
/*
# ABCD EFGP
0 0000 0011 3
1 1001 1111 159
2 0010 0101 37
3 0000 1101 13
4 1001 1001 153
5 0100 1001 73
6 0000 0001 65
7 0001 1111 31
8 0000 0001 1
9 0000 1001 9
*/

volatile bool countFlag = 0;
volatile bool resetFlag = 0;

void setup() {
  pinMode(DS_pin,  OUTPUT);
  pinMode(STCP_pin, OUTPUT);
  pinMode(SHCP_pin, OUTPUT);

  pinMode(add_btn, INPUT);
  pinMode(sub_btn, INPUT);
  pinMode(cnt_btn, INPUT);
  pinMode(rst_btn, INPUT);
  pinMode(cfg_btn, INPUT);

  pinMode(relay, OUTPUT);
  pinMode(modeLed, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(cnt_btn), ISR_count, RISING);
  attachInterrupt(digitalPinToInterrupt(rst_btn), ISR_reset, RISING);

  Serial.begin(9600);
}


void  loop() {
  cfg = digitalRead(cfg_btn);
  if (cfg){
    Mode = !Mode;
    count = preset;
  }
  if(Mode){
    digitalWrite(modeLed, HIGH);
    Config();
  }else{
    digitalWrite(modeLed, LOW);
    Run();
  }

  displayValue(x);
  Serial.println(x);
} 

void ISR_count(){
  countFlag = 1;
}

void ISR_reset(){
  resetFlag = 1;
}

void displayValue(int val){
  int j = val/10;
  int i = val%10;

  digitalWrite(STCP_pin,LOW);
  shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_digits[i]);
  shiftOut(DS_pin, SHCP_pin, LSBFIRST,dec_digits[j]);
  digitalWrite(STCP_pin,  HIGH);
  delay(300);
}

void Config(){
  add = digitalRead(add_btn);
  sub = digitalRead(sub_btn);

  if(add){
    preset++;
  }
  if(sub){
    preset--;
  }
  x=preset;
}

void Run(){
  cnt = digitalRead(cnt_btn);
  rst = digitalRead(rst_btn);

  if(countFlag){
    //delay(10);
    //if(!cnt){
      count--;
      countFlag = 0;
    //}
    if(count<0){
      count = 0;
    }

  }
  if(resetFlag){
    count=preset;
    resetFlag = 0;
  }

  if(count <= 0){
    digitalWrite(relay,LOW); //LOGICA INVERSA
  }else{
    digitalWrite(relay,HIGH); //LOGICA INVERSA
  }

  x=count;
}