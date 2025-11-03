#include "InputSystem.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream> // only used if you call printConfig on desktop

static inline void trim(std::string& s) {
  auto ns = [](unsigned char c){ return !std::isspace(c); };
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), ns));
  s.erase(std::find_if(s.rbegin(), s.rend(), ns).base(), s.end());
}

InputSystem::ConfigEntry::ConfigEntry(const std::string& n, int v) : name(n), value(v) {}
InputSystem::InputSystem() : fsPath("/config.txt") {}

bool InputSystem::begin() {
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount FAILED");
    // don’t return yet — system can still run without FS-backed config if you want
    return false; // keep existing behavior if you prefer
  }

  File f = LittleFS.open(fsPath.c_str(), "r");
  if (!f) {
    Serial.printf("Failed to open %s\n", fsPath.c_str());
    return false; // keep as-is; or continue with defaults if you prefer
  }

  std::string content; content.reserve(f.size());
  while (f.available()) content.push_back(char(f.read()));
  f.close();

  std::istringstream iss(content);
  const auto cfg = parseConfig(iss);

  Serial.printf("[InputSystem] Loaded: %u buttons, %u dpad, %u pots, %u leds, %u gearbox lines\n",
    (unsigned)cfg.buttons.size(), (unsigned)cfg.dpadButtons.size(),
    (unsigned)cfg.potentiometers.size(), (unsigned)cfg.leds.size(),
    0u);

  initButtonsFromConfig(cfg);
  initPotentiometersFromConfig(cfg);
  initLedsFromConfig(cfg);

  gearbox_analog.init_midpoints_and_ranges_from_file("/config_analog_gear.txt");
  gearbox_analog.init(leds);
  return true;
}


void InputSystem::update()
{
  // Poll buttons and update any LEDs each cycle
  buttons_update();
  potentiometers_update();
  update_led();
  gearbox_analog.update();
}

void InputSystem::buttons_update() 
{
  for (auto& b : buttons)
  {
    b.update();
  }
}

void InputSystem::potentiometers_update()
{
  for (auto& p : potentiometers)
  {
    p.update();
  }
}

void InputSystem::update_led()
{
  for (auto& l : leds)
  {
    l.update();
  }
}

void InputSystem::printConfig(const ConfigData& cfg) 
{
  Serial.printf("Buttons (%u):\n", (unsigned)cfg.buttons.size());
  for (const auto& e : cfg.buttons) Serial.printf("  %s - %d\n", e.name.c_str(), e.value);

  Serial.printf("Potentiometers (%u):\n", (unsigned)cfg.potentiometers.size());
  for (const auto& e : cfg.potentiometers) Serial.printf("  %s - %d\n", e.name.c_str(), e.value);

  Serial.printf("DpadButtons (%u):\n", (unsigned)cfg.dpadButtons.size());
  for (const auto& e : cfg.dpadButtons) Serial.printf("  %s - %d\n", e.name.c_str(), e.value);

  Serial.printf("LEDs (%u):\n", (unsigned)cfg.leds.size());
  for (const auto& e : cfg.leds) Serial.printf("  %s - %d\n", e.name.c_str(), e.value);
}

InputSystem::ConfigData InputSystem::parseConfig(std::istream& in) {
  ConfigData config;
  std::string line, currentSection;

  while (std::getline(in, line)) {
    trim(line);
    if (line.empty()) continue;

    if      (line == "Buttons")          currentSection = "buttons";
    else if (line == "Potensiometers")   currentSection = "potentiometers"; // keep original spelling
    else if (line == "DpadButtons")      currentSection = "dpadButtons";
    else if (line == "led")              currentSection = "led";             // keep original case
    else if (line == "Geabox_analog" || 
             line == "Gearbox_analog")   { currentSection.clear(); continue; }
    else if (line.find("---") != std::string::npos) continue;
    else {
      const std::string sep = " - ";
      const auto dash = line.find(sep);
      if (dash == std::string::npos) continue;

      std::string name = line.substr(0, dash);
      std::string valueStr = line.substr(dash + sep.size());
      trim(name); trim(valueStr);
      if (!name.empty() && name.front()=='"' && name.back()=='"')
        name = name.substr(1, name.size()-2);

      try {
        int value = std::stoi(valueStr);
        if      (currentSection == "buttons")         config.buttons.emplace_back(name, value);
        else if (currentSection == "potentiometers")  config.potentiometers.emplace_back(name, value);
        else if (currentSection == "dpadButtons")     config.dpadButtons.emplace_back(name, value);
        else if (currentSection == "led")             config.leds.emplace_back(name, value);
        // analog gearbox overrides from config.txt are intentionally ignored
      } catch (...) {
        Serial.printf("Bad value: %s in line: %s\n", valueStr.c_str(), line.c_str());
      }
    }
  }
  return config;
}

void InputSystem::initButtonsFromConfig(const ConfigData& cfg) 
{
  buttons.clear();
  auto add = [this](const ConfigEntry& e) {
    buttons.emplace_back(uint8_t(e.value), e.name);
    buttons.back().init();
  };
  for (const auto& e : cfg.buttons)     add(e);
  for (const auto& e : cfg.dpadButtons) add(e);
}

void InputSystem::initLedsFromConfig(const ConfigData& cfg) {
  leds.clear();
  for (const auto& e : cfg.leds) {
    leds.emplace_back(uint8_t(e.value), e.name);
    leds.back().init();
  }
}

void InputSystem::initPotentiometersFromConfig(const ConfigData& cfg) {
  potentiometers.clear();
  for (const auto& e : cfg.potentiometers) {
    potentiometers.emplace_back(uint8_t(e.value), e.name);
    potentiometers.back().init();
  }
}

// initGearBoxAnalog removed: analog gearbox is now configured only via /config_analog_gear.txt
