#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
//int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
//int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}



long int x_data[3], y_data[3], z_data[3];
long int changein_x, changein_y, changein_z, x_tot = 0, y_tot = 0, z_tot = 0, x_avg = 0, y_avg = 0, z_avg = 0;
int p = 0, j, l = 0;
bool filled = false;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);


}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  //  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  //  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  //  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  //  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  //  print out data


  Serial.println("xdata ydata zdata");
  Serial.print(accelerometer_x);
  Serial.print("   ");
  Serial.print(accelerometer_y);
  Serial.print("   ");
  Serial.println(accelerometer_z);



  x_data[p] = abs(accelerometer_x);
  y_data[p] = abs(accelerometer_y);
  z_data[p] = abs(accelerometer_z);
  p++;



  if (p == 3) {
    if (filled == false) {
      Serial.println("-------------------------------------- ");
    }
    filled = true;
    p = 0;
  }



  if (filled == true) {

    for (j = 0; j < 3; j++) {
      x_tot = x_tot + x_data[j];
      Serial.println("  ");
      Serial.print(x_data[j] );
      Serial.print("  ");
      y_tot = y_tot + y_data[j];
      Serial.print("  ");
      Serial.print(y_data[j]);
      Serial.print("  ");
      z_tot = z_tot + z_data[j];
      Serial.print("  ");
      Serial.print(+ z_data[j]);
      Serial.print("  ");

    }
  }



  x_avg = x_tot / 3;
  y_avg = y_tot / 3;
  z_avg = z_tot / 3;

  changein_x = x_avg - accelerometer_x;
  changein_y = y_avg - accelerometer_y;
  changein_z = z_avg - accelerometer_z;
  x_tot = 0; y_tot = 0; z_tot = 0;
  x_avg = 0; y_avg = 0; z_avg = 0;

  //  Serial.print(".\t");
  //  Serial.print(l);
  //  Serial.print(".\t");
  //  l++;
  //

  //  // Serial.print("aX = ");
  //  Serial.print("\t");
  //  Serial.print(changein_x);
  //  //Serial.print(" | aY = ");
  //  Serial.print("\t");
  //  Serial.print(changein_y);
  //  // Serial.print(" | aZ = ");
  //  Serial.print("\t");
  //  Serial.print(changein_z);




  if (abs(changein_x) > 10000 || abs(changein_y) > 10000 || abs(changein_z) > 10000)
    //Serial.println("\n BAM IMPACT DETECTED ");


    Serial.println();


  delay(500);
}
