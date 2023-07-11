// Phần khai báo thư viện và định nghĩa

#include <FirebaseESP8266.h> //khai báo thư viện kết nối ESP8266 và Firebase
#include <ESP8266WiFi.h> // khai báo thư viện ESP8266
#include <ArduinoJson.h> // Khai báo thư viện json arduino
#include "DHT.h" //Khai báo thư viện cảm biến nhiệt độ,độ ẩm
#define DHTTYPE DHT11 // Định nghĩa loại cảm biến sử dụng là DHT11
#define DHTPIN D5 // Định nghĩa chân kết nối cảm biến DHT11 là chân D5
#define gas A0 // Định nghĩa chân kết nối cảm biến MQ-02 là chân A0

// Phần khai báo biến sử dụng trong chương trình

FirebaseData dulieufirebase; //Khai báo đối tượng dữ liệu firebase trên phạm vi toàn cục
DHT dht(DHTPIN, DHTTYPE);
StaticJsonBuffer<1000>jsonBuffer; // Tạo Buffer json có dung lượng 1000
float t,h; // Khai báo biến t và h kiểu float
int g; // Khai báo biến g kiểu int
String DK,DB,DN,DVS,QB,QKHACH,QKHI,QN;

// Hàm thiết lập

void setup()
{
  Serial.begin(115200); //Khởi tạo Serial ở baudrate là 115200
  WiFi.begin("Co Hong ( p.lanh)","dmk21 982121"); // thiết lập kết nối với wifi bao gồm tên và mật khẩu của wifi
  // Trong khi trạng thái của Wifi chưa được kết nối sẽ in ra ... và  sau 500ms in ra một lần 
  while (!(WiFi.status() == WL_CONNECTED)) 
  {
    delay(500);
    Serial.print("...");
  }
  // Khi chế độ Wifi đã được kết nối sẽ in ra " kết nối thành công "
  Serial.print("");
  Serial.println("ket noi thanh cong");
  // Thiết lập bắt đầu kết nối với firebase (" địa chỉ liên kết","mã xác thực")
  Firebase.begin("duankythuat1-default-rtdb.firebaseio.com","q2uwGITKfJVUsi2yHHfeiKbWilfWQcBORLNPL8iC");  
  dht.begin(); // Thiết lập cảm biến nhiệt độ, độ ẩm bắt đầu hoạt động
}

// chương trình chính

void loop()
{
  read_sensor(); // hàm đọc giá trị của cảm biến
  read_data_from_f(); // hàm đọc dữ liệu từ firebase
  if(WiFi.isConnected()==false)
  {
    while (!(WiFi.status() == WL_CONNECTED)) 
    {
      delay(500);
      Serial.println(".");
    }
  }
  //nếu biến g có giá trị lớn hơn hoặc bằng 320 sẽ gán giá trị QKHI=”1” còn ngược lại sẽ bằng “0”
  if(g>=240)  
    QKHI="1";
  else
    QKHI="0";
  process(); // hàm xử lí 
  sent_data_to_f(); // hàm gửi dữ liệu lên firebase
}

// Các chương trình con

void read_sensor()
{
  t = dht.readTemperature(); // đọc giá trị nhiệt độ lưu vào biến t
  h = dht.readHumidity(); // đọc giá trị độ ẩm lưu vào biến h
  g = analogRead(gas); // đọc giá trị từ chân analog A0 và lưu vào biến g
}

void read_data_from_f()
{
  // Nếu tag DEN-BEP trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến DB
  if(Firebase.getString(dulieufirebase,"/DEN-BEP"))     
    DB = dulieufirebase.stringData();
  

  // Nếu tag DEN-KHACH trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến DK
  if(Firebase.getString(dulieufirebase, "/DEN-KHACH")) 
    DK = dulieufirebase.stringData();

  // Nếu tag DEN-NGU trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến DN
  if(Firebase.getString(dulieufirebase, "/DEN-NGU"))
    DN = dulieufirebase.stringData();

  // Nếu tag DEN-VSINH trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến DVS
  if(Firebase.getString(dulieufirebase, "/DEN-VSINH")) 
    DVS = dulieufirebase.stringData();
  
  // Nếu tag QUAT-BEP trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến QB
  if(Firebase.getString(dulieufirebase, "/QUAT-BEP")) 
    QB = dulieufirebase.stringData();

  // Nếu tag QUAT-KHACH trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến QKHACH
  if(Firebase.getString(dulieufirebase, "/QUAT-KHACH")) 
    QKHACH = dulieufirebase.stringData();

  // Nếu tag QUAT-NGU trên firebase có dữ liệu. Dữ liệu sẽ được lưu vào biến QN
  if(Firebase.getString(dulieufirebase, "/QUAT-NGU")) 
    QN = dulieufirebase.stringData();
}

void process()
{
  // Tạo một biến root1 mang kiểu json
  JsonObject& root1 = jsonBuffer.createObject();   

  root1["DEN_KHACH"] = DK; // gán tham số DEN_KHACH giá trị DK
  root1["DEN_BEP"] = DB; // gán tham số DEN_BEP giá trị DB
  root1["DEN_NGU"] = DN; // gán tham số DEN_NGU giá trị DN
  root1["DEN_VESINH"] = DVS; // gán tham số DEN_VESINH giá trị DVS
  
  root1["QUAT_BEP"] = QB; // gán tham số QUAT_BEP giá trị QB
  root1["QUAT_KHACH"] = QKHACH; // gán tham số QUAT_KHACH giá trị QKHACH
  root1["QUAT_NGU"] = QN; // gán tham số QUAT_NGU giá trị QN
  root1["QUAT_KHI"]=QKHI; // gán tham số QUAT_KHI giá trị QKHI
  
  root1["temp"] = t; // gán tham số temp giá trị t
  root1["hum"] = h; // gán tham số hum giá trị h
  root1["gas"] = g; // gán tham số gas giá trị g

  //nếu không có dữ liệu trở cổng com thì gửi chuỗi root1 ra cổng com
  if(!Serial.available()) 
  {
    root1.printTo(Serial);
    Serial.println(""); 
  }
  jsonBuffer.clear(); // xóa dữ liệu trong bộ nhớ jsonbuffer  
}

void sent_data_to_f()
{
  // đưa giá trị của biến t lên firebase với tag tên là Nhietdo kiểu dữ liệu là Float
  Firebase.setFloat(dulieufirebase,"/Nhietdo",t);   
  // đưa giá trị của biến h lên firebase với tag tên là Doam kiểu dữ liệu là Float
  Firebase.setFloat(dulieufirebase,"/Doam",h);  
  // đưa giá trị của biến h lên firebase với tag tên là gas kiểu dữ liệu là Int
  Firebase.setInt(dulieufirebase,"/gas",g); 
  // đưa giá trị của biến QKHI lên firebase với tag tên là QUAT-KHI kiểu dữ liệu là String
  Firebase.setString(dulieufirebase,"/QUAT-KHI",QKHI); 
}
