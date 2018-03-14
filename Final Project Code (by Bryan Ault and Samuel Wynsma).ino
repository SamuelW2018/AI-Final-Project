#include <Servo.h>

bool roomMap[15][6] = {
{ false, false, false, false, false, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, true, true, true, true, false},
{ false, false, false, false, false, false}};

float dotMap[15][6];

//0 = south, 1 = west, 2 = North, 3 = east
int moveDir;
bool first_move = true;

bool prevReading[4];

//0 = front, 1 = left, 2 = right, 3 = back
bool curReading[4];

void initializeUniformly()
{
  Serial.println("Begin Uniform Initialization");
  float count = 0.0;
  for(int x = 0; x < 15; x++)
  {
    for(int y =0; y < 6; y++)
    {
      if(roomMap[x][y] == true)
      {
        count++;
      }
    }
  }

  Serial.print("Number of probable locations: ");
  Serial.println(count);
  
  for(int x = 0; x < 15; x++)
  {
    for(int y =0; y < 6; y++)
    {
      if(roomMap[x][y] == true)
      {
        dotMap[x][y] = (float)1/count;
      }
      else
      {
        dotMap[x][y] = 0.0;
      }
    }
  }
}

void redistribute()
{
  float count = 0.0;
  float sum = 0.0;

  //South
  if(moveDir == 0)
  {
    Serial.println("South");
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        sum += dotMap[x][y];
        if(dotMap[x][y] != 0)
        {
          //0 = front, 1 = left, 2 = right, 3 = back

          //Can we have come from this square
          if((roomMap[x-1][y]) == prevReading[2] && (roomMap[x+1][y] == prevReading[1]) && (roomMap[x][y+1] == prevReading[3]) && (roomMap[x][y-1] == prevReading[0]))
          {
            //Can we have ended up in this square
            if(roomMap[x-1][y-1] == curReading[2] && roomMap[x+1][y-1] == curReading[1] && roomMap[x][y-2] == curReading[0] && roomMap[x][y] == curReading[3])
            {
              //somehow save the coordinate this happens at
              count++;
            }
          }
        }
      }
    }

    //Update probability table given new data
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        if(dotMap[x][y] != 0)
        {
          //if there is a possibility we are in this square we should check
          //0 = front, 1 = left, 2 = right, 3 = back
  
          if((roomMap[x-1][y]) == prevReading[2] && (roomMap[x+1][y] == prevReading[1]) && (roomMap[x][y+1] == prevReading[3]) && (roomMap[x][y-1] == prevReading[0]))
          {
            //Can we have ended up in this square
            if(roomMap[x-1][y-1] == curReading[2] && roomMap[x+1][y-1] == curReading[1] && roomMap[x][y-2] == curReading[0] && roomMap[x][y] == curReading[3])
            {
              //somehow save the coordinate this happens at
              dotMap[x][y - 1] = 1 / count;
              dotMap[x][y] = 0.0;
            }
            else
            {
              dotMap[x][y] = 0.0;
            }
          }
          else
          {
            dotMap[x][y] = 0.0;
          }
        }
      }
    }
  }
  
  //West
  else if (moveDir == 1)
  {
    Serial.println("West");
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        sum += dotMap[x][y];
        //if there is a possibility we are in this square we should check
        //0 = front, 1 = left, 2 = right, 3 = back
        if(dotMap[x][y] != 0)
        {
          //ASSUMMING FACING NORTH: prev left && prev right && prev front && prev back
          //check previous square
          
          //W, E, S, N
          if(roomMap[x - 1][y] == prevReading[0] && roomMap[x+1][y] == prevReading[3] && roomMap[x][y+1] == prevReading[2] && roomMap[x][y-1] == prevReading[1])
          {
            if(roomMap[x - 2][y] == curReading[0] && roomMap[x][y] == curReading[3] && roomMap[x - 1][y + 1] == curReading[2] && roomMap[x][y - 1] == curReading[1])
            {
              count++;
            }
          }
        }
      }
    }
    /*now that we have found all the possible coordinate points update the probability distribution in the dotMap
     * dotMap[possible x][possible y] = 1 / count
     * else that x y = 0
     */
     Serial.print("Count: ");
     Serial.println(count);
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        
        //if there is a possibility we are in this square we should check
        //0 = front, 1 = left, 2 = right, 3 = back
        if(dotMap[x][y] != 0)
        {
          //W, E, S, N
          if(roomMap[x - 1][y] == prevReading[0] && roomMap[x+1][y] == prevReading[3] && roomMap[x][y+1] == prevReading[2] && roomMap[x][y-1] == prevReading[1])
          {
            if(roomMap[x - 2][y] == curReading[0] && roomMap[x][y] == curReading[3] && roomMap[x - 1][y + 1] == curReading[2] && roomMap[x][y - 1] == curReading[1])
            {
              dotMap[x - 1][y] = 1 / count;
              dotMap[x][y] = 0;
            }
            else
            {
              dotMap[x][y] = 0;
            }
          }
          else
          {
            dotMap[x][y] = 0;
          }
        }
      }
    }
  }
  //North
  else if(moveDir == 2)
  {
    Serial.println("North");
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        sum += dotMap[x][y];
        if(dotMap[x][y] != 0.0)
        {
          /*0 = front, 1 = left, 2 = right, 3 = back
           *ASSUMING FACING NORTH prev left && prev right && prev front && prev back */
           
          //Can we have moved from the current square
          if((roomMap[x-1][y]) == prevReading[1] && (roomMap[x+1][y] == prevReading[2]) && (roomMap[x][y+1] == prevReading[0]) && (roomMap[x][y-1] == prevReading[3]))
          {
            //Can we have moved to this square
            if((roomMap[x-1][y+1] == curReading[1]) && (roomMap[x+1][y+1] == curReading[2]) && (roomMap[x][y+2] == curReading[0]) && (roomMap[x][y] == curReading[3]))
            {
              count++;
            }
          }
        }
      }
    }
    for(int x = 0; x < 15; x++)
    {
      for(int y = 5; y >=0; y--)
      {
        if(dotMap[x][y] != 0)
        {
          /*0 = front, 1 = left, 2 = right, 3 = back
           * ASSUMING FACING NORTH prev left && prev right && prev front && prev back
           *if(roomMap[x-1][y-1] == prevReading[1] && roomMap[x+1][y-1] == prevReading[2] && roomMap[x][y] == prevReading[0] && roomMap[x][y-2] == prevReading[3])
           */
          if((roomMap[x-1][y]) == prevReading[1] && (roomMap[x+1][y] == prevReading[2]) && (roomMap[x][y+1] == prevReading[0]) && (roomMap[x][y-1] == prevReading[3]))
          {
            if((roomMap[x-1][y+1] == curReading[1]) && (roomMap[x+1][y+1] == curReading[2]) && (roomMap[x][y+2] == curReading[0]) && (roomMap[x][y] == curReading[3]))
            {
              //somehow save the coordinate this happens at
              dotMap[x][y + 1] = 1 / count;
              dotMap[x][y] = 0;
            }
            else
            {
              dotMap[x][y] = 0.0;
            }
          }
          else
          {
            dotMap[x][y] = 0.0;
          }
        }
      }
    }
  }
  
  //East
  else if(moveDir == 3)
  {
    
    Serial.println("East");
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        sum += dotMap[x][y];
        if(dotMap[x][y] != 0)
        {
          //0 = front, 1 = left, 2 = right, 3 = back
          //W, E, S, N
          if(roomMap[x - 1][y] == prevReading[3] && roomMap[x+1][y] == prevReading[0] && roomMap[x][y+1] == prevReading[1] && roomMap[x][y - 1] == prevReading[2])
          {
            if(roomMap[x][y] == curReading[3] && roomMap[x+2][y] == curReading[0] && roomMap[x + 1][y + 1] == curReading[1] && roomMap[x + 1][y - 1] == curReading[2])
            {
              //somehow save the coordinate this happens at
              count++;
            }
          }
        }
      }
    }

    //Update probability distribution based of new knowledge
    for(int x = 14; x >= 0; x--)
    {
      for(int y = 0; y < 6; y++)
      {
        if(dotMap[x][y] != 0)
        {

          //0 = front, 1 = left, 2 = right, 3 = back
          //W, E, S, N
          if(roomMap[x - 1][y] == prevReading[3] && roomMap[x+1][y] == prevReading[0] && roomMap[x][y+1] == prevReading[1] && roomMap[x][y - 1] == prevReading[2])
          {
            if(roomMap[x][y] == curReading[3] && roomMap[x+2][y] == curReading[0] && roomMap[x + 1][y + 1] == curReading[1] && roomMap[x + 1][y - 1] == curReading[2])
            {
              dotMap[x + 1][y] = 1/count;
              dotMap[x][y] = 0;
            }
            else
            {
              dotMap[x][y] = 0;
            }
          }
          else
          {
            dotMap[x][y] = 0;
          }
        }
      }
    }  
  }

  if(sum == 0.0)
  {
    Serial.println("Impossible reading - begin redistribution");
    initializeUniformly();
  }
  else if (sum == 1.0);
  {
    
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        if(dotMap[x][y] == 1.0)
        {
          Serial.print("The coordinates it is at are: (");
          Serial.print(x); Serial.print(","); Serial.print(y); Serial.print(")");
        }
      }
    }
    //GAME OVER
    delay(5000000);
  }
}

class Robot
{
  private:
    //0 = south, 1 = west, 2 = North, 3 = east
    int facing;
    Servo steer;
    Servo drive;

  public:
    Robot(){}
    
    Robot(int dir)
    {
      steer.attach(5);
      steer.write(90);
      delay(1500);

      drive.attach(4);
      drive.write(90);

      facing = dir;
    }

    void startupProcess()
    {
      steer.attach(5);
      steer.write(90);
      delay(1500);

      drive.attach(4);
      drive.write(90);
    }

    void move_forward()
    {
      Serial.println("Begin move forward");

      steer.write(90);
      delay(1500);
      drive.write(101);
      delay(1880);
      drive.write(90);

      moveDir = facing;
    }
    void move_back()
    {
      Serial.println("Begin move backward");
      steer.write(90);
      delay(2000);
      drive.write(79);
      delay(2650);
      drive.write(90);
    }

    void move_left()
    {
      Serial.println("Begin left turn.");
      
      //update direction facing
      //0 = south, 1 = west, 2 = North, 3 = east
      if(facing == 0)
      { 
        facing = 3;

        //0 = front, 1 = left, 2 = right, 3 = back
        bool f = curReading[0];
        bool l = curReading[1];
        bool r = curReading[2];
        bool b = curReading[3];

        curReading[0] = l;
        curReading[1] = b;
        curReading[2] = f;
        curReading[3] = r;
      }
      else
      { 
        facing--;

        //0 = front, 1 = left, 2 = right, 3 = back
        bool f = curReading[0];
        bool l = curReading[1];
        bool r = curReading[2];
        bool b = curReading[3];

        curReading[0] = l;
        curReading[1] = b;
        curReading[2] = f;
        curReading[3] = r;
      }

      //turn vehicle
      move_back();
      steer.write(45);
      delay(2000);
      drive.write(110);
      delay(1520);
      drive.write(90);

      moveDir = facing;
    }

    void move_right()
    {
      Serial.println("Begin right turn");

      //update direction facing
      //0 = south, 1 = west, 2 = North, 3 = east
      if(facing == 3)
      { 
        facing = 0;

        //0 = front, 1 = left, 2 = right, 3 = back
        bool f = curReading[0];
        bool l = curReading[1];
        bool r = curReading[2];
        bool b = curReading[3];

        curReading[0] = l;
        curReading[1] = b;
        curReading[2] = f;
        curReading[3] = r;
        
      }
      else
      { 
        facing++;

        //0 = front, 1 = left, 2 = right, 3 = back
        bool f = curReading[0];
        bool l = curReading[1];
        bool r = curReading[2];
        bool b = curReading[3];

        curReading[0] = l;
        curReading[1] = b;
        curReading[2] = f;
        curReading[3] = r;
        
      }

      //Turn
      move_back();
      steer.write(135);
      delay(2000);
      drive.write(110);
      delay(1520);
      drive.write(90);

      moveDir = facing;
    }
    
    //facing: 0 = south, 1 = west, 2 = North, 3 = east
    //Reading: 0 = N, 1 = W, 2 = E, 3 = S
    //Pins: rear = 8, front = 10, left = 9, right = 11
    void sensorReading()
    {
      //update prev readings
      
      for(int i = 0; i < 4; i++)
      {
        prevReading[i] = curReading[i]; 
      }

      //south
      if(facing == 0)
      {
        Serial.println("Running South facing ping");
        curReading[0] = ping(10);
        curReading[1] = ping(9);
        curReading[2] = ping(11);
        curReading[3] = ping(8);
      }
      //West
      else if(facing == 1)
      {
        Serial.println("Running West facing ping");
        curReading[0] = ping(10);
        curReading[1] = ping(9);
        curReading[2] = ping(11);
        curReading[3] = ping(8);
      }
      //North
      else if(facing == 2)
      {
        Serial.println("Running North facing ping");
        curReading[0] = ping(10);
        curReading[1] = ping(9);
        curReading[2] = ping(11);
        curReading[3] = ping(8);
      }
      //East
      else
      {
        Serial.println("Running East facing ping");
        curReading[0] = ping(10);
        curReading[1] = ping(9);
        curReading[2] = ping(11);
        curReading[3] = ping(8);
      }

      Serial.println("Sensor reading after ping");
        for(int i = 0; i < 4; i++)
        {
          Serial.println(curReading[i]);
        }
    }

    void setDir(int dir)
    {
      facing = dir;
    }

    void chooseMove()
    {
      //0 = front, 1 = left, 2 = right, 3 = back
      if(curReading[0] == true)
      {
        move_forward();
      }
      else if(curReading[1] == true)
      {
        move_left();
      }
      else if(curReading[2] == true)
      {
        move_right();
      }
    }
};


Robot bot;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("~~~~~~~~Begin startup process~~~~~~~~~");
  
  //0 = south, 1 = west, 2 = North, 3 = east
  moveDir = 2;
  bot.setDir(moveDir);
  Serial.println("Startup");
  bot.startupProcess();
  delay(3000);
  initializeUniformly();
  delay(3000);
  Serial.println("~~~~~~~~~~~End startup process~~~~~~~~~~~");
}

void loop() {
  Serial.println("------NEW LOOP CYCLE------");
  delay(3000);
  bot.sensorReading();
  
  if(first_move == false)
  {
    Serial.println("Redistributed probability");
    delay(1000);
    redistribute();
    for(int x = 0; x < 15; x++)
    {
      for(int y = 0; y < 6; y++)
      {
        Serial.print(dotMap[x][y]);
        Serial.print(" ");
      }
      Serial.println("");
    }
  }
  else
  {
    Serial.println("First move, no probability update");
    first_move = false;
  }
  
  delay(2000);
  //let bot move
  bot.chooseMove();
}


bool ping(int pingPin)
{
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  long duration = pulseIn(pingPin, HIGH);

  long inches = microsecondsToInches(duration);
  if(inches < 8)
  {
    return false; 
  }
  else
  { return true; }
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
