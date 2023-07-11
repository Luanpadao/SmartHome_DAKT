// Phần khai báo thư viện và định nghĩa

#include <ArduinoJson.h>  
#include <LiquidCrystal_I2C.h>

// Phần khai báo  biến sử dụng trong chương trình
StaticJsonBuffer<1000>jsonBuffer; // Khai báo dung lượng sử dụng cho thư viện json
LiquidCrystal_I2C lcd(0x27,16,2); // Khai báo biến Lcd (địa chỉ, số cột, số dòng)
int dk=6;      // Đèn khách kết nối vào chân 6  
int db=7;      // Đèn bếp kết nối vào chân 7  
int dn=8;      // Đèn ngủ kết nối vào chân 8  
int dvs=9;     // Đèn vệ sinh kết nối vào chân 9  
int qn=10;     // Quạt ngủ kết nối vào chân 10  
int qb=11;     // Quạt bếp kết nối vào chân 11  
int qk=12;     // Quạt khách kết nối vào chân 12 
int qkhi=13;   // Quạt khí kết nối vào chân 13 
int buz=5;     // Loa kết nối vào chân 5

// Hàm thiết lập
void setup()
{
  Serial.begin(115200);

  // Khai báo chế độ chân là đầu ra
  pinMode(dk, OUTPUT);      
  pinMode(db, OUTPUT);
  pinMode(dn, OUTPUT);
  pinMode(dvs, OUTPUT);
  pinMode(qk, OUTPUT);
  pinMode(qb, OUTPUT);
  pinMode(qn, OUTPUT);
  pinMode(qkhi,OUTPUT);
  pinMode(buz,OUTPUT);

  // Tắt đèn và quạt
  digitalWrite(dk,0);
  digitalWrite(dn,0);
  digitalWrite(db,0);
  digitalWrite(dvs,0);
  // ( Lưu ý: Do module relay tích cực mức thấp nên đặt mức cao để tắt nó )
  digitalWrite(qk,1);
  digitalWrite(qn,1);
  digitalWrite(qb,1);
  digitalWrite(qkhi,1);
  // Loa 
  digitalWrite(buz,0);
  
  // Khởi tạo Lcd
  lcd.init(); // Khởi động LCD
  lcd.clear(); // Xóa màn hình 
  lcd.backlight(); // Mở đèn
}

// Hàm chương trình chính
void loop()
{
  recieve_data(); // gọi chương trình con recieve_data
}
// Hàm recieve_data()
void recieve_data()
{
  JsonObject& root = jsonBuffer.parseObject(Serial); // Nhận dữ liệu (parse) từ biến root kiểu dữ liệu json object
  if (root == JsonObject::invalid()) // Nếu root không có giá trị thì sẽ thoát ra khỏi chương trình con
  return;
  root.prettyPrintTo(Serial); // Cho phép nhận dữ liệu json từ cổng com
  String data1 = root["DEN_KHACH"];// Lưu giá trị của tham số DEN_KHACH vào biến data1 kiểu string   
  String data2 = root["DEN_BEP"]; // Lưu giá trị của tham số DEN_BEP vào biến data2 kiểu string   
  String data3 = root["DEN_NGU"]; // Lưu giá trị của tham số DEN_NGU vào biến data3 kiểu string   
  String data4 = root["DEN_VESINH"]; // Lưu giá trị của tham số DEN_VESINH vào biến data4 kiểu string   
  String data5 = root["QUAT_BEP"]; // Lưu giá trị của tham số QUAT_BEP vào biến data5 kiểu string   
  String data6 = root["QUAT_KHACH"]; // Lưu giá trị của tham số QUAT_KHACH vào biến data6 kiểu string   
  String data7 = root["QUAT_NGU"]; // Lưu giá trị của tham số QUAT_NGU vào biến data7 kiểu string   
  String data8 = root["QUAT_KHI"]; // Lưu giá trị của tham số QUAT_KHI vào biến data8 kiểu string   
  float  data9 = root["temp"]; // Lưu giá trị của tham số temp vào biến data9 kiểu float ( số thực )
  float  data10= root["hum"]; // Lưu giá trị của tham số hum vào biến data10 kiểu float ( số thực )
  int    data11= root["gas"]; // Lưu giá trị của tham số gas vào biến data11 kiểu int ( số nguyên )
  jsonBuffer.clear(); // Sau khi nhận xong thì xóa các giá trị trong các tham số
  
  if(data1 == "1") // Nếu data1 bằng 1 
    digitalWrite(dk,1); // cho ĐÈN KHÁCH sáng
  else
    digitalWrite(dk,0); // cho ĐÈN KHÁCH tắt
    
  if(data2 == "1") // Nếu data2 bằng 1 
    digitalWrite(db,1); // Cho ĐÈN BẾP sáng
  else
    digitalWrite(db,0); // Cho ĐÈN BẾP tắt
    
  if(data3 == "1") // Nếu data3 bằng 1 
    digitalWrite(dn,1); // Cho ĐÈN NGỦ sáng
  else
    digitalWrite(dn,0); // Cho  ĐÈN NGỦ tắt
  
  if(data4 == "1") // Nếu data4 bằng 1 
    digitalWrite(dvs,1); // Cho ĐÈN VỆ SINH sáng
  else
    digitalWrite(dvs,0); // Cho ĐÈN VỆ SINH  tắt
    
  if(data5 == "1") // Nếu data5 bằng 1
    digitalWrite(qk,0); // Cho QUẠT KHÁCH bật
  else
    digitalWrite(qk,1); // Cho QUẠT KHÁCH tắt
  
  if(data6 == "1") // Nếu data6 bằng 1
    digitalWrite(qb,0); // Cho QUẠT BẾP bật
  else
    digitalWrite(qb,1); // Cho QUẠT BẾP tắt
    
  if(data7 == "1") // Nếu data7 bằng 1
    digitalWrite(qn,0); // Cho QỤẠT NGỦ bật
  else
    digitalWrite(qn,1); // Cho QUẠT NGỦ tắt
  if(data8 == "1") // Nếu data8 bằng 1
  {
    digitalWrite(qkhi,0); // Cho QUẠT KHÍ bật 
    digitalWrite(buz,1); // Cho LOA bật
  }
  else
  {
    digitalWrite(qkhi,1); // Cho QUẠT KHÍ tắt
    digitalWrite(buz,0); // Cho LOA tắt
  }
  
  // In nhiệt độ ra màn hình LCD
  lcd.setCursor(0,0); // Con trỏ ở vị trí cột 0, hàng 0
  lcd.print(data9); // In giá trị nhiệt độ
  lcd.setCursor(5,0); // Con trỏ ở vị trí cột 5, hàng 0
  lcd.print(" (C) "); // In chuỗi " (C) "

  // In độ ẩm ra màn hình LCD
  lcd.setCursor(10,0); // Con trỏ ở vị trí cột 10, hàng 0
  lcd.print(data10); // In giá trị độ ẩm 
  lcd.setCursor(15,0); // Con trỏ ở vị trí cột 15, hàng 0
  lcd.print("%"); // In chuỗi "%"
  
  // In khí gas ra màn hình LCD
  lcd.setCursor(0,1); // Con trỏ ở vị trí cột 0, hàng 1
  lcd.print("Gas: "); // In ra chuỗi "Gas: "
  lcd.setCursor(6,1); // Con trỏ ở vị trí cột 6, hàng 1
  lcd.print(data11); // In giá trị gas
  lcd.setCursor(10,1); // Con trỏ ở vị trí cột 10, hàng 1
  lcd.print("PPM"); // In chuỗi "PPM" 
}
