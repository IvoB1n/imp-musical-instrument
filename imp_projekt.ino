/*
Musical instrument simulator
Ivan Bobrov (xbobro01)
*/

// buzzer pin
const int TONE_OUTPUT_PIN = 12;
// PWM channel 0
const int TONE_PWM_CHANNEL = 0; 
// error flag
int internal_error = 0;

// setting up program parameters
void setup() {
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
  Serial.begin(115200);
}

// main programm loop
void loop() {
  // clear error flag
  internal_error = 0;
  // if there is a data on input
  if(Serial.available()) {
    // read data
    String input_data = Serial.readStringUntil('\n');
    int data_lenght = input_data.length();

    // if string has 'x' - play demo melody
    if (data_lenght == 1 && input_data[0] == 'x') {
      play_melody("c8d8f8d8f4f4e2c8d8f8d8e4e4d2e8d4c8d8f8d8d2g4e4d2c4c2d2");
    } else if (data_lenght % 2 != 0) {
      // if string has an odd number of elements - error
      Serial.println("ERROR: wrong input format");
    } else {
      play_melody(input_data);
    }
  }
}


void play_melody(String input_data) {
  int data_lenght = input_data.length();
  Serial.println("input string is: ");
  Serial.println(input_data);
  Serial.println(data_lenght);
  Serial.println(data_lenght/2);
  Serial.println('\n');
  // arrays for notes and time signatures
  note_t note_array[data_lenght/2];
  int time_signature_array[data_lenght/2];
  
  for (int index = 0; index < data_lenght; index++) {
    if (index % 2 == 0) {
      // add note to an array
      note_t current_note;
      if (input_data[index] == 'a') {
        current_note = NOTE_A;  
      } else if (input_data[index] == 'b') {
        current_note = NOTE_B;
      } else if (input_data[index] == 'c') {
        current_note = NOTE_C;
      } else if (input_data[index] == 'd') {
        current_note = NOTE_D;
      } else if (input_data[index] == 'e') {
        current_note = NOTE_E;
      } else if (input_data[index] == 'f') {
        current_note = NOTE_F;
      } else if (input_data[index] == 'g') {
        current_note = NOTE_G;
      } else {
        internal_error = 1;
        break;
      }      
      note_array[index/2] = current_note;
    } else {
      // add time signature to an array
      int time_sig;
      if (input_data[index] == '1') {
        time_sig = 1000;
      } else if (input_data[index] == '2') {
        time_sig = 500;
      } else if (input_data[index] == '4') {
        time_sig = 250;
      } else if (input_data[index] == '8') {
        time_sig = 125;
      } else {
        internal_error = 2;
        break;
      }
      time_signature_array[index/2] = time_sig;
    }  
  }
   if (internal_error == 1) {
    // error - unknown note
    Serial.println("ERROR: wrong note format");
  } else if (internal_error == 2) {
    // error - unknown time signature
    Serial.println("ERROR: wrong time signature format");
  } else {
    for (int note = 0; note < (data_lenght/2); note++) {
      // attach pin
      ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
      // play note with delay
      ledcWriteNote(TONE_PWM_CHANNEL, note_array[note], 4);
      delay(time_signature_array[note]);
      // detach pin
      ledcDetachPin(TONE_OUTPUT_PIN);
      delay(30);
    }
    ledcDetachPin(TONE_OUTPUT_PIN);
  }
}
