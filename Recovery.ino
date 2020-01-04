/**
  Using this script to orchestrate.
 */

void init_recovery(){
  sensor_read_data();
  trigger_altitude = (float)(altitude + DIFF_TRIGGER);
  audible_start();
}

void check_rising(){
  triggered = (trigger_altitude < altitude);
}

void check_falling(){
  dropping = altitude < last_altitude;
  if(dropping){
    trigger_altitude = (float)(altitude - (DIFF_TRIGGER/2));  // Trying to recover faster
  }else{
    last_altitude = altitude;
  }
}

void check_deploy(){
  if(trigger_altitude > altitude){
    trigger_recovery();
    deployed = true;
  }
}

void reset_recovery(){
    //triggerState = 0;
    digitalWrite(ledPin, LOW); 
    myservo.write(30);
    delay(100);
    return;
}

void trigger_recovery() {
  
  digitalWrite(ledPin, HIGH); 
  //triggerState = 1;
  myservo.write(150);
  delay(100);
//  return;


  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void audible_start(){
  
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(8, melody_init[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void where_are_we()
{
  if(!dropping && !triggered){
    check_rising();
  }else if(triggered && !dropping){
    check_falling();
  }else if(dropping && !deployed){
    check_deploy();
  }
}
