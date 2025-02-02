#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"
#include <map>
#include <string>
#include <functional>
#include <cstring> // For strdup, memcpy
#include <vector>
#include <sstream>

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

std::string getParsedValue(const char *str, int index) {
    std::string result;
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    tokens.reserve(5); // Example: Reserve space for 5 tokens (adjust as needed)

    while (std::getline(ss, token, '|')) {
        if (!token.empty()) { // Check for empty tokens
            tokens.push_back(token); 
        }
    }

    if (index >= 0 && index < tokens.size()) {
        result = tokens[index];
    }

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
      std::string filename_str = getParsedValue(arg, 1);
      strncpy(filename, filename_str.c_str(), sizeof(filename) - 1); 
    }},
    // Set Print TIme
    {"SPT|", [&](const char* arg){ 
        std::string print_time_str = getParsedValue(arg, 1);
        strncpy(print_time, print_time_str.c_str(), sizeof(print_time) - 1); 
    }},
    // Set Elapsed Time
    {"SET|", [&](const char* arg){ 
        std::string ptime_left_str = getParsedValue(arg, 1);
        strncpy(ptime_left, ptime_left_str.c_str(), sizeof(ptime_left) - 1); 
    }},
    // Set Time Left
    {"STL|", [&](const char* arg){ 
        std::string total_layers_str = getParsedValue(arg, 1);
        strncpy(total_layers, total_layers_str.c_str(), sizeof(total_layers) - 1); 
    }},
    // Set Current Layer
    {"SCL|", [&](const char* arg){ 
        std::string curr_layer_str = getParsedValue(arg, 1);
        strncpy(curr_layer, curr_layer_str.c_str(), sizeof(curr_layer) - 1); 
    }},
    // Set Progress
    {"SPP|", [&](const char* arg){ 
        std::string progress_str = getParsedValue(arg, 1);
        strncpy(progress, progress_str.c_str(), sizeof(progress) - 1); 
    }},
    // Update Progress
    {"UPP|", [&](const char* arg){ 
        std::string val_str = getParsedValue(arg, 1);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_Progress(val_str.c_str())); 
    }},
    // Update Current Layer
    {"UCL|", [&](const char* arg){ 
        std::string val_str = getParsedValue(arg, 1);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_CLayer(val_str.c_str())); 
    }},
    // Update Elapsed Time
    {"UET|", [&](const char* arg){ 
        std::string val_str = getParsedValue(arg, 1);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_ETA(val_str.c_str())); 
    }},
    {"UPT|", [&](const char* arg){ 
        std::string val_str_a = getParsedValue(arg, 1);
        std::string val_str_b = getParsedValue(arg, 2);
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_Progress(val_str_a.c_str())); 
        TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_ETA(val_str_b.c_str())); 
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
