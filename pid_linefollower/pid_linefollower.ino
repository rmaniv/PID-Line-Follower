/*  
 *   PID Line Follower
 *   Developer: Vinamr L. Sachdeva 
*/

long sensor[] = {0, 1, 2, 3, 4}; //leftmost - 0, rightmost - 4

int rmf = 9;
int rmb = 6;
int lmf = 10; 
int lmb = 11;

//speeds
int rspeed;
int lspeed;
const int base_speed = 255;

int pos;
long sensor_average;
int sensor_sum;

int button = 3; //to be pressed to find set point

float p;
float i;
float d;
float lp;
float error;
float correction;
float sp;

float Kp = 5; // dummy
float Ki = 0; //dummy
float Kd = 40; //(Kp-1)*10

void pid_calc();
void calc_turn();
void motor_drive(int , int );

void setup()
{
  //sensors
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  //motors
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);

  Serial.begin(9600);
  
  //finding set point

  while(button)
  {
    sensor_average = 0;
    sensor_sum = 0;
  
    for (int i = -2; i <= 2; i++)
    {
      sensor[i] = analogRead(i);
      sensor_average += sensor[i] * i * 1000;   //weighted mean   
      sensor_sum += int(sensor[i]);
    }
  
    pos = int(sensor_average / sensor_sum);
  
    Serial.print(sensor_average);
    Serial.print(' ');
    Serial.print(sensor_sum);
    Serial.print(' ');
    Serial.print(pos);
    Serial.println();
    delay(2000);
  }

  sp = pos;
}

void loop()
{
  pid_calc();
  calc_turn();
}

void pid_calc()
{
  sensor_average = 0;
  sensor_sum = 0;
  i = 0;

  for(int i = -2; i <= 2; i++)
  {
    sensor[i]=analogRead(i);
    sensor_average = sensor[i]*i*1000; //weighted mean
    sensor_sum += sensor[i];
  }

  pos = int(sensor_average / sensor_sum);

  error = pos-sp;

  p = error;
  i += p;
  d = p - lp;

  lp = p;

  correction = int(Kp*p + Ki*i + Kd*d);
}

void calc_turn()
{
  rspeed = base_speed + correction;
  lspeed = base_speed - correction;

  //restricting speeds of motors between 255 and -255
  
  if (rspeed > 255) 
    rspeed = 255;
    
  if (lspeed > 255) 
    lspeed = 255;
    
  if (rspeed < -255) 
    rspeed = -255;
    
  if (lspeed < -255) 
    lspeed = -255;
 
 motor_drive(rspeed,lspeed);  
}

void motor_drive(int right, int left){
  
  if(right>0)
  {
    analogWrite(rmf, right);   
    analogWrite(rmb, 0);
  }
  else 
  {
    analogWrite(rmf, 0); 
    analogWrite(rmb, abs(right));
  }
  
 
  if(left>0)
  {
    analogWrite(lmf, left);
    analogWrite(lmb, 0);
  }
  else 
  {
    analogWrite(lmf, 0);
    analogWrite(lmb, abs(left));
  }
  
}
