#include "Wire.h"
#include "I2Cdev.h" 
#include "MPU6050.h"
#include <SoftwareSerial.h>

MPU6050 mpu; //ประกาศตัวแปร mpu เป็นตัวแปรจาก Libraly
int16_t ax, ay, az;
int16_t gx, gy, gz;
float Gyro_x, Gyro_y, Gyro_z;
float Accl_x, Accl_y, Accl_z;

char inbyte = 0;
int led = 4;

void setup() 
{
Wire.begin();
Serial.begin(38400);

pinMode(led, OUTPUT);
digitalWrite(led, LOW);

//Serial.println("Initialize MPU");
mpu.initialize();
//Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
} 

void loop() //รับค่าและแสดงผลแกนต่างๆไปเรื่อยๆ
{
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

/* ----------- แปลงค่า raw value มาเป็นค่า องศา    ----------*/
double xAngle = atan( ax / (sqrt(square(ay) + square(az))));
double yAngle = atan( ay / (sqrt(square(ax) + square(az))));
double zAngle = atan( sqrt(square(ax) + square(ay)) / az);
xAngle *= 180.00;   yAngle *= 180.00;   zAngle *= 180.00;
xAngle /= 3.141592; yAngle /= 3.141592; zAngle /= 3.141592;
/* ----------- แปลงค่า raw value มาเป็นค่า องศา    ----------*/

/* ----------- แปลงค่า raw value มาเป็นค่า Acceleration [g]  ----------*/
Accl_x = (float) ax * 4.0 / 65536.0;
Accl_y = (float) ay * 4.0 / 65536.0;
Accl_z = (float) az * 4.0 / 65536.0;
// แปลง จาก [g] เป็น หน่วยความเร่งที่ เมตรต่อวินาที ยกกำลังสอง (m/s^2)
Accl_x *= 9.81;
Accl_y *= 9.81;
Accl_z *= 9.81;

float sumAcc = sqrt(pow(Accl_x,2) + pow(Accl_x,2) + (Accl_x,2));
/* ----------- แปลงค่า raw value มาเป็นค่า Acceleration [g]  ----------*/

// ลดค่า rang ลงมาให้เหลือ 0 - 10
//gx = gz / 3200; gy = gy / 3200; gz = gz / 3200;
Gyro_x = (float) gx * 500.0 / 65536.0;
Gyro_y = (float) gy * 500.0 / 65536.0;
Gyro_z = (float) gz * 500.0 / 65536.0;

float sumGyro = sqrt(pow(Gyro_x,2) + pow(Gyro_y,2) + (Gyro_z,2));

/*---------------- ส่วนส่งข้อมูลให้แอพ smart helmet   ----------------*/
// เพื่อแจ้งแอพให้รู้ว่า ให้เริ่มอ่านข้อมูลจากตรงนี้ได้แล้วนะ
Serial.print('#');
Serial.print("Degree : "); 
Serial.print(xAngle); Serial.print(','); 
Serial.print(yAngle); Serial.print(','); 
Serial.print(zAngle); Serial.print(',');  

//Serial.print(gx); Serial.print(',');
//Serial.print(gy); Serial.print(',');
//Serial.print(gz); 
//Serial.print("\t Gyro : ");
//Serial.print(Gyro_x); Serial.print(',');
//Serial.print(Gyro_y); Serial.print(',');
//Serial.print(Gyro_z); 
//Serial.print("\t Acc : ");
//Serial.print(Accl_x); Serial.print(',');
//Serial.print(Accl_y); Serial.print(',');
//Serial.print(Accl_z);
 
Serial.print("\t Acc : ");
Serial.print(sumAcc); 
Serial.print("\t Gyro : ");
Serial.print(sumAcc); Serial.print(',');

Serial.print('~'); // เพื่อบอกแอพว่าจบการส่งค่าในส่วนนี้แล้วนะ
Serial.println(); 
delay(50);        
/*---------------- ส่วนส่งข้อมูลให้แอพ smart helmet   ----------------*/
}

