#include <IRrecv.h>
#define RECV_PIN     0        // Infrared receiving pin
IRrecv irrecv(RECV_PIN);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object
unsigned long previousInput,currentInput;

void IRSetup()
{      
  irrecv.enableIRIn();       
}


int switch_irr(int irr_data)
{
  switch(irr_data)
  {
    case 16750695: return 0;
    case 16753245: return 1;
    case 16736925: return 2;
    case 16769565: return 3;
    case 16720605: return 4;
    case 16712445: return 5;
    case 16761405: return 6;
    case 16769055: return 7;
    case 16754775: return 8;
    case 16748655: return 9;
    case 16738455: return 10; // *
    case 16756815: return 11; // #
    case 16718055: return 12; // up
    case 16730805: return 13; // down
    case 16716015: return 14; // left
    case 16734885: return 15; // right
    case 16726215: return 16; // ok
    default: return -1;
  }
}

bool IRUpdate()
{
    if (irrecv.decode(&results)) {          // Waiting for decoding
        unsigned long value = results.value;
        value = switch_irr(value);
        if(value==4294967295){ //value on long press.
            currentInput=previousInput;  // Change the value received this time to the value received last time
            irrecv.resume();
            return false;
        }else{ // Update if not long press.
            previousInput=currentInput;
            currentInput=value;
        }
        irrecv.resume();  
        return true;
    }

    return false;
}

int AwaitForIRInput() {
    while (!IRUpdate()){
        delay(50);
    };

    return currentInput;
}