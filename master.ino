#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
// #include <FlexiTimer2.h>

SoftwareSerial SerialToSlave(2,3);
SoftwareSerial SerialToPC(4,5);
//
//union StateData
//{
//  char ssid[32];
//  char password[32];
////  char linkmode[8];
//  char targetip[32];
////  char port[8];
////  char stateA[8];
////  char stateB[8];
////  char stateC[8];
//  byte hex[96];
//};

/*************************************************
 本程序是丹丹项目的主芯片管理通讯控制程序
 大概要求有：
 1.通过串口获取从机的各端口基本情况
 2.负责连接wifi
 3.构造通讯协议
 4.按通讯协议完成相关功能
 5.辅助串口要完成wifi配置，和状态查看等通讯内容
 */

//StateData alldata;

String ssid = "\"Smartisan personal hotspot\"";
String password = "\"wudan2888\"";
String linkmode = "\"UDP\"";
String Targetip = "\"192.168.43.186\"";
String port = "6666";
String ssid1="";
String password1="";
String Targetip1="";

bool connect_state = false;
bool one_init_flag = true;
bool Qt_comm_flag = true; //test: cancle flexitimer
int init_count = 0;

bool stringComplete = false;
String QT_string="";

bool master_slave_comm = false;
String from_slave_string="";

bool wifi_master_comm = false;
String wifi_comm_string="";

void handle_wifi_to_master_comm(){

  if(wifi_master_comm){   //rev_string.length()>0
    if(wifi_comm_string.substring(0, 4) == "$sta"){
      Serial.println("state is fine");
      //sdf
    }else if(wifi_comm_string.substring(0, 4) == "$chk"){
      SerialToSlave.println("$ask\0");
      //sdf
    }else{
      Serial.println(wifi_comm_string);
    }
//    if(from_slave_string.substring(0, 4) == "uin1"){
//      Serial.println("state is fine");
//      //sdf
//    }else if(from_slave_string.substring(0, 4) == "$cmd"){
//      Serial.println("cmd is kill");
//      //sdf
//    }else{
      //Serial.println(from_slave_string);
//    }
  }
  wifi_comm_string="";
  wifi_master_comm = false;
}

void WifiToSerial_Int() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the rev_string:
    wifi_comm_string += inChar;
    delay(2);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      wifi_master_comm = true;
    }
  }
}


void handle_master_slave_comm(){
    if(master_slave_comm){   
    Serial.println(from_slave_string);
    //mcuA = from_slave_string;
  }
    from_slave_string="";
    master_slave_comm = false;
}

void SerialToSlave_Int() {
  //SerialToSlave.listen();
  while (SerialToSlave.available()) {
    // get the new byte:
    char inChar = (char)SerialToSlave.read();
    // add it to the rev_string:
    from_slave_string += inChar;
    delay(2);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '>') {
      master_slave_comm = true;
    }
  }
}

//1.字符初始化
//2.等待qt修改alldata
//3.应用alldata数据连接
void clear_eeprom(void){
  int i;
  for(i=0;i<512;i++)
    EEPROM.write(i, 0x3F);//save is ?
  delay(200);
}

void Qt_Comm_Change_MS_Comm(){
  one_init_flag = false;
  Qt_comm_flag = false;
  SerialToPC.end();
  SerialToSlave.begin(9600);
  // FlexiTimer2::stop();
}

void one_init(void){
  init_count++;
    if(init_count == 10){
        init_count = 0;
        Qt_Comm_Change_MS_Comm();
      }
}



void eeprom_To_String(void){
  int addr_ssid = 0;
  int addr_pw = 64;
  int addr_tarip = 128;
  cli();
  EEPROM.get(addr_ssid,ssid1);
  EEPROM.get(addr_pw,password1);
  EEPROM.get(addr_tarip,Targetip1);
  sei();
//  for(i=0;i<96;i++){
//    alldata.hex[i]=EEPROM.read(i);
//  }
//  for(i=0;i<32;i++){
//    if(alldata.ssid[i] == "\n"){
//      break;
//    }
//    ssid += alldata.ssid[i]; 
//  }
//  for(i=0;i<32;i++){
//    if(alldata.password[i] == "\n"){
//      break;
//    }
//    password += alldata.password[i]; 
//  }
//  for(i=0;i<32;i++){
//    if(alldata.targetip[i] == "\n"){
//      break;
//    }
//    Targetip += alldata.targetip[i];
//  }
//  
//  for(i=0;i<8;i++){
//      linkmode += alldata.linkmode[i];
//  }
//  for(i=0;i<8;i++){
//      port += alldata.port[i];
//  }
//  for(i=0;i<32;i++){
//    if(alldata.ssid[i] !='\n'){
//      ssid += alldata.ssid[i];
//    }
//  }
//  for(i=0;i<32;i++){
//    if(alldata.password[i] !='\n'){
//      password += alldata.password[i];
//    }
//  }
//  for(i=0;i<8;i++){
//    if(alldata.linkmode[i] !='\n'){
//      linkmode += alldata.linkmode[i];
//    }
//  }
//  for(i=0;i<32;i++){
//    if(alldata.targetip[i] !='\n'){
//      Targetip += alldata.targetip[i];
//    }
//  }
//  for(i=0;i<8;i++){
//    if(alldata.port[i] !='\n'){
//      port += alldata.port[i];
//    }
//  }
//  for(i=0;i<8;i++){
//    if(alldata.stateA[i] !='\n'){
//      mcuA += alldata.stateA[i];
//    }
//  }
//  for(i=0;i<8;i++){
//    if(alldata.stateB[i] !='\n'){
//      mcuB += alldata.stateB[i];
//    }
//  }
//  for(i=0;i<8;i++){
//    if(alldata.stateC[i] !='\n'){
//      mcuC += alldata.stateC[i];
//    }
//  }
}

void String_to_eeprom(void){
  int addr_ssid = 0;
  int addr_pw = 64;
  int addr_tarip = 128;
  cli();
  EEPROM.put(addr_ssid,ssid);
  EEPROM.put(addr_pw,password);
  EEPROM.put(addr_tarip,Targetip);
  sei();
//  char i,charssid[32],charpassword[32],charTargetip[32];//,charlinkmode[8],charport[8],chartempA[8],chartempB[8],chartempC[8];
//  int count;
//  ssid.toCharArray(charssid,ssid.length());
//  password.toCharArray(charpassword,password.length());
//  Targetip.toCharArray(charTargetip,Targetip.length());
//  linkmode.toCharArray(charlinkmode,8);
//  port.toCharArray(charport,8);
//  mcuA.toCharArray(chartempA,8);
//  mcuB.toCharArray(chartempB,8);
//  mcuC.toCharArray(chartempC,8);
//  for(i=0;i<32;i++){
//    if(i >= ssid.length()+1){
//      alldata.ssid[i] = 0;
//    }else{
//      alldata.ssid[i]=charssid[i];
//    }
//    if(i >= password.length()+1){
//      alldata.password[i]=0;
//    }else{
//      alldata.password[i]=charpassword[i];
//    }
//    if(i >= Targetip.length()+1){
//      alldata.targetip[i]=0;
//    }else{
//      alldata.targetip[i]=charTargetip[i];
//    }
//  }
//  for(i=0;i<8;i++){
//    alldata.linkmode[i]=charlinkmode[i];
//  }
//  for(i=0;i<8;i++){
//    alldata.port[i]=charport[i];
//  }
//  for(i=0;i<5;i++){
//    if(chartempA[i] != alldata.stateA[i]){
//      for(j=0;j<7;j++){
//        alldata.stateA[j] = chartempA[j];
//      }
//      break;
//    }
//  }
//  for(i=0;i<5;i++){
//    if(chartempB[i] != alldata.stateB[i]){
//      for(j=0;j<7;j++){
//        alldata.stateB[j] = chartempB[j];
//      }
//      break;
//    }
//  }
//  for(i=0;i<5;i++){
//    if(chartempC[i] != alldata.stateC[i]){
//      for(j=0;j<7;j++){
//        alldata.stateC[j] = chartempC[j];
//      }
//      break;
//    }
//  }
//  for(count=0;count<96;count++){
//    EEPROM.write(count,alldata.hex[count]);
//  }
}

void handle_Qt_comm(void){

  if(stringComplete){
    if(QT_string.substring(0, 5) == "$read"){
      connect_state = true;
      // FlexiTimer2::stop();
      SerialToPC.println("si,"+ssid);
      delay(64);
      SerialToPC.println("pw,"+password);
      delay(64);
      SerialToPC.println("tp,"+Targetip);
      //SerialToPC.print("read ok\n");
      //sdf
    }
    if(QT_string.substring(0, 6) == "$clear"){
      //sdf
      clear_eeprom();
      SerialToPC.print("clear ok \n");
    }
    if(QT_string.substring(0, 6) == "$write"){
        //qt告诉avr写完毕
        String_to_eeprom();
        SerialToPC.print("wrtok,\n");
        Qt_Comm_Change_MS_Comm();
    }
    if(QT_string.substring(0, 6) == "$testt"){
        //测试联调
        SerialToPC.print("testtok,\n");
        Qt_Comm_Change_MS_Comm();
    }
    //接收qt发回的数据
    if(QT_string.substring(0,4) == "Ssid"){
      ssid = "";
      ssid = QT_string.substring(5,QT_string.length()-1);
      SerialToPC.print("ssid ok\n");
    }
    if(QT_string.substring(0,8) == "Password"){
      password = "";
      password = QT_string.substring(9,QT_string.length()-1);
      SerialToPC.print("password ok\n");
    }
    if(QT_string.substring(0,8) == "TargetIP"){
      Targetip = "";
      Targetip = QT_string.substring(9,QT_string.length()-1);
      SerialToPC.print("TargetIP ok\n");
    }
//    if(rev_string.substring(0,8) == "LinkMode"){
//      linkmode = rev_string.substring(9,rev_string.length()-1);
//    }
//    if(rev_string.substring(0,4) == "Port"){
//      port = rev_string.substring(9,rev_string.length()-1);
//    }
  }
  QT_string="";
  stringComplete = false;
}

void serialToPC_Int() {
  //SerialToPC.listen();
  while (SerialToPC.available()) {
    // get the new byte:
    char a= (char)SerialToPC.read();
    // add it to the rev_string:
    QT_string +=a;
    delay(2);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (a == '\n') {
      stringComplete = true;
    }
  }
}



//void handle_Qt_comm(void){
//
//  if(stringComplete){
//    if(rev_string.substring(0, 5) == "$read"){
//      connect_state = true;
//        SerialToPC.println(ssid+","+password+","+ linkmode+","+Targetip+","+port+","+mcuA+","+mcuB+","+mcuC+","+"ok,");
//        //sdf
//    }else if (rev_string.substring(0, 6) == "$write"){
//        //qt告诉avr写完毕
//        SerialToPC.println("wrt ok,,,");
//        SerialToPC.end();
//        FlexiTimer2::stop();
//        String_to_alldata();
//        one_init_flag = false;
//        
//    }else{
//      //接收qt发回的数据
//      if(rev_string.substring(0,4) == "Ssid"){
//        ssid = rev_string.substring(5,rev_string.length()-6);
//      }
//      if(rev_string.substring(0,8) == "Password"){
//        password = rev_string.substring(9,rev_string.length()-10);
//      }
//      if(rev_string.substring(0,8) == "LinkMode"){
//        linkmode = rev_string.substring(9,rev_string.length()-10);
//      }
//      if(rev_string.substring(0,8) == "TargetIP"){
//        Targetip = rev_string.substring(9,rev_string.length()-10);
//      }
//      if(rev_string.substring(0,4) == "Port"){
//        port = rev_string.substring(9,rev_string.length()-6);
//      }
//    }
//    rev_string="";
//    stringComplete = false;
//  }
//}

void wifilinkprocess(void){
  delay(4000);
  Serial.println("AT+CWJAP="+ssid1+","+password1);
  delay(4000);
  Serial.println("AT+CIPSTART="+linkmode+","+Targetip1+","+port);
  delay(4000);
  Serial.println("AT+CIPMODE=1");
  delay(2000);
  Serial.println("AT+CIPSEND");
  //test
  delay(200);
  Serial.println("fuck force");
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  // FlexiTimer2::set(1000,one_init);
  // FlexiTimer2::start();
//  rev_string.reserve(200);
//  from_slave_string.reserve(200);
  
  SerialToPC.begin(9600);
  SerialToPC.listen();
  if(SerialToPC.isListening()){
    digitalWrite(LED_BUILTIN,LOW);
    while(Qt_comm_flag){
      serialToPC_Int();
      handle_Qt_comm();
    }
  }else
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }
//SerialToSlave.begin(9600);
  SerialToSlave.listen();
  digitalWrite(LED_BUILTIN,HIGH);
  eeprom_To_String();
  if (SerialToSlave.isListening())
  {
    wifilinkprocess();
  }
  digitalWrite(LED_BUILTIN,LOW);  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  SerialToSlave_Int();
  WifiToSerial_Int();
  handle_master_slave_comm();
  handle_wifi_to_master_comm();
}
