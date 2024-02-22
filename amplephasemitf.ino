int frot = 3;
int fgruen = 4;

int gelb = 6;
int rot = 5;
int gruen = 7;
int knopf = 2;
int statusknopf = 0;


void setup() {
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2, INPUT);
  }

void loop() {
  statusknopf = digitalRead(knopf);

  if (statusknopf == HIGH){
    digitalWrite(gruen, LOW);
    digitalWrite(gelb, HIGH);
    delay(1000);
    digitalWrite(gelb, LOW);
    digitalWrite(rot, HIGH);
    delay(500);

    digitalWrite(fgruen, HIGH);
    digitalWrite(frot, LOW);

    delay(3000);

    digitalWrite(fgruen, LOW);
    digitalWrite(frot, HIGH);
    delay(500);

    digitalWrite(gelb, HIGH);
    delay(1000);
    
    digitalWrite(rot, LOW);
    digitalWrite(gelb, LOW);

  }else{
    digitalWrite(fgruen, LOW);
    digitalWrite(frot, HIGH);

    digitalWrite(rot, LOW);
    digitalWrite(gruen, HIGH);
  }


  

}





/*digitalWrite(rot, HIGH);
  delay(2000);
  digitalWrite(gelb, HIGH);
  delay(1000);
  digitalWrite(rot, LOW);
  digitalWrite(gelb, LOW);
  digitalWrite(gruen, HIGH);
  delay(3000);
  
  digitalWrite(gruen, LOW);
  digitalWrite(gelb, HIGH);
  delay(1000);
  digitalWrite(rot, LOW);
  digitalWrite(gelb, LOW);
  */