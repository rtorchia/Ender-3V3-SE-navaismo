#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"
#include <map>
#include <string>
#include <functional>

/**
 * M5000: Set Printing Details JOB from OctoPrint in LCD
 *
 * Params:
 *    - Filename
 *    - Print Time
 *    - Print Time Left
 *    - Total Layers
 *    - Current Layer
 *    - B64 thumbnail
 *    - Progress
 */
bool O9000_collecting = false;
char filename[50] = {0};
char print_time[50] = {0};
char ptime_left[50] = {0};
char total_layers[50] = {0};
char curr_layer[50] = {0};
char thumbnail[50] = {0};
char progress[10] = {0};
char param_value[50] = {0};
// bool serial_connection_active = false; // defined in serial.cpp

const char *getParsedValue(char *str)
{
  const char delimiter[] = "|";
  char *token;

  // Split the string to get the first part (before '|')
  token = strtok(str, delimiter);

  // Split again to get the second part (after '|')
  token = strtok(NULL, delimiter); // NULL tells strtok to continue with the same string

  if (token != NULL)
  {
    // Copy the second part into the param_value variable
    strncpy(param_value, token, sizeof(param_value) - 1);
    param_value[sizeof(param_value) - 1] = '\0'; // Ensure null termination
  }

  return param_value;

}
const std::string getParsedValue(const char *str)
{
  const char delimiter[] = "|";
  char *token;
  std::string result;

  char *str_copy = strdup(str);
  if (str_copy == nullptr)
  {
    return ""; // Or throw an exception, log an error, etc.
  }

  // Split the string to get the first part (before '|')
  token = strtok(str_copy, delimiter);

  // Split again to get the second part (after '|')
  token = strtok(NULL, delimiter); // NULL tells strtok to continue with the same string

  if (token != NULL)
  {
    result = token;
  }
  
  free(str_copy);
  return result;
}

void GcodeSuite::O9000()
{
  if (!parser.string_arg || parser.string_arg[0] == '\0')
  {
    //SERIAL_ECHOLN("Empty Command Argument...");
    return;
  }

  char *my_string = parser.string_arg;
  std::map<std::string, std::function<void(const char*)>> commandHandlers =
  {
    {"SC|", [&](const char*){
      TERN_(DWIN_CREALITY_LCD, DWIN_OctoPrintJob(filename, print_time, ptime_left, total_layers, curr_layer, thumbnail, progress));
    }},
    // Set FileName
    {"SFN|", [&](const char* arg){
      std::string filename_str = getParsedValue(arg);
      strncpy(filename, filename_str.c_str(), sizeof(filename) - 1); 
    }},
    // Set Print TIme
    {"SPT|", [&](const char* arg){ 
        std::string print_time_str = getParsedValue(arg);
        strncpy(print_time, print_time_str.c_str(), sizeof(print_time) - 1); 
    }},
    // Set Elapsed Time
    {"SET|", [&](const char* arg){ 
        std::string ptime_left_str = getParsedValue(arg);
        strncpy(ptime_left, ptime_left_str.c_str(), sizeof(ptime_left) - 1); 
    }},
    // Set Time Left
    {"STL|", [&](const char* arg){ 
        std::string total_layers_str = getParsedValue(arg);
        strncpy(total_layers, total_layers_str.c_str(), sizeof(total_layers) - 1); 
    }},
    // Set Current Layer
    {"SCL|", [&](const char* arg){ 
        std::string curr_layer_str = getParsedValue(arg);
        strncpy(curr_layer, curr_layer_str.c_str(), sizeof(curr_layer) - 1); 
    }},
    // Set Progress
    {"SPP|", [&](const char* arg){ 
        std::string progress_str = getParsedValue(arg);
        strncpy(progress, progress_str.c_str(), sizeof(progress) - 1); 
    }},
    // Update Progress
    {"UPP|", [&](const char* arg){ 
        std::string val_str = getParsedValue(arg);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_Progress(val_str.c_str())); 
    }},
    // Update Current Layer
    {"UCL|", [&](const char* arg){ 
        std::string val_str = getParsedValue(arg);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_CLayer(val_str.c_str())); 
    }},
    // Update Elapsed Time
    {"UET|", [&](const char* arg){ 
        std::string val_str = getParsedValue(arg);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_ETA(val_str.c_str())); 
    }},
    {"PF|",  [&](const char*){ TERN_(DWIN_CREALITY_LCD, DWIN_OctoJobFinish()); }},                               // Print Finished
    {"BU1|", [&](const char*){ TERN_(DWIN_CREALITY_LCD, DWIN_OctoShowGCodeImage()); }},                          // Show GCode Image
    {"OCON|", [&](const char*){ serial_connection_active = true; TERN_(DWIN_CREALITY_LCD, Goto_MainMenu()); }},  // Octoprint Connection On
    {"OCOFF|", [&](const char*){ serial_connection_active = false; TERN_(DWIN_CREALITY_LCD, Goto_MainMenu()); }},// Octoprint Connection Off
    };

  // Find the command
  std::string command = my_string;
  size_t delimiterPos = command.find('|');
  if (delimiterPos != std::string::npos) {
    command = command.substr(0, delimiterPos + 1);
  }
  
  auto it = commandHandlers.find(command);

  if (it != commandHandlers.end()) {
    it->second(my_string);
  } else {
    SERIAL_ECHOLN("Invalid Command Argument...");
  }

  // TODO THUMBNAIL RECBUFFER
}
