#include <Arduino.h>
extern "C" {
  #include "../component/my_logging.h"
}

void my_log(const char *msg) {
  Serial.println(msg);
}
