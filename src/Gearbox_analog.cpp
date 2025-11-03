#include "Gearbox_analog.h"

// ---- FS includes (LittleFS on ESP32/ESP8266 to match project config) ----
#if defined(ESP32) || defined(ESP8266)
  #include <FS.h>
  #include <LittleFS.h>
  #define FS_IMPL LittleFS
#endif

Gearbox_analog::Gearbox_analog()
    : analog_pin(A0),
      range(30),
      park_led(nullptr),
      drive_led(nullptr),
      neutral_led(nullptr),
      rear_led(nullptr),
      manual_led(nullptr) {}

void Gearbox_analog::init(std::vector<Led>& leds)
{
    // Bind LEDs by their configured names in data/config.txt
    park_led = drive_led = neutral_led = rear_led = manual_led = nullptr;
    for (auto& l : leds) {
        const std::string& name = l.get_name();
        if      (name == "Park")    { park_led    = &l; }
        else if (name == "Reverse") { rear_led    = &l; }
        else if (name == "Neutral") { neutral_led = &l; }
        else if (name == "Drive")   { drive_led   = &l; }
        else if (name == "Manual")  { manual_led  = &l; }
    }

    setGear('P'); // Default to 'P' on startup
}

void Gearbox_analog::init_midpoints_and_ranges(std::vector<int> parsed_points, int parsed_ranges)
{
    mid_points.clear();
    range = parsed_ranges;

    static const char ORDER[] = { 'P','R','N','D','M' };
    const int n = (int)parsed_points.size();
    for (int i = 0; i < n && i < (int)(sizeof(ORDER)); ++i) {
        MidPoint mp;
        mp.value = parsed_points[i];
        mp.gear  = ORDER[i];
        mp.name  = String(ORDER[i]);
        mid_points.push_back(mp);
    }
}

static bool parseNameValue(const String& line, String& nameOut, int& valOut)
{
    int comma = line.indexOf(',');
    if (comma < 0) return false;
    String left  = line.substring(0, comma);
    String right = line.substring(comma + 1);

    left.trim();
    right.trim();
    if (left.length() == 0 || right.length() == 0) return false;

    nameOut = left;
    valOut  = right.toInt();
    return true;
}

String Gearbox_analog::trimCopy(const String& s)
{
    String t = s;
    t.trim();
    return t;
}

char Gearbox_analog::inferGearFromName(const String& name)
{
    if (name.length() == 0) return '\0';
    char c = toupper(name[0]);
    if (c == 'P') return 'P';
    if (c == 'R') return 'R';
    if (c == 'N') return 'N';
    if (c == 'D') return 'D';
    if (c == 'M') return 'M';
    return '\0';
}

void Gearbox_analog::init_midpoints_and_ranges_from_file(const char* path)
{
#if defined(FS_IMPL)
    if (!FS_IMPL.begin(true)) {
        Serial.println(F("[Gearbox_analog] FS mount failed. Formatting and continuing."));
    }

    if (!FS_IMPL.exists(path)) {
        Serial.print(F("[Gearbox_analog] Config file not found: "));
        Serial.println(path);
        return;
    }

    File f = FS_IMPL.open(path, "r");
    if (!f) {
        Serial.print(F("[Gearbox_analog] Failed to open config: "));
        Serial.println(path);
        return;
    }

    mid_points.clear();

    // 1) pin_number
    String line = f.readStringUntil('\n');
    analog_pin = trimCopy(line).toInt();

    // 2) range
    line = f.readStringUntil('\n');
    range = trimCopy(line).toInt();

    // 3..n) name, value
    while (f.available()) {
        line = f.readStringUntil('\n');
        line = trimCopy(line);
        if (line.length() == 0) continue;     // blanks
        if (line[0] == '#') continue;          // comments

        String name;
        int val = 0;
        if (!parseNameValue(line, name, val)) {
            Serial.print(F("[Gearbox_analog] Skipping bad line: "));
            Serial.println(line);
            continue;
        }

        MidPoint mp;
        mp.name  = name;
        mp.value = val;
        mp.gear  = inferGearFromName(name);

        if (mp.gear == '\0') {
            Serial.print(F("[Gearbox_analog] Unknown gear name: "));
            Serial.println(name);
            continue;
        }

        mid_points.push_back(mp);
    }
    f.close();

    Serial.print(F("[Gearbox_analog] Loaded pin="));
    Serial.print(analog_pin);
    Serial.print(F(" range="));
    Serial.print(range);
    Serial.print(F(" midpoints="));
    Serial.println((int)mid_points.size());
#else
    Serial.println(F("[Gearbox_analog] No FS available on this platform. Use init_midpoints_and_ranges()."));
    (void)path;
#endif
}

void Gearbox_analog::update()
{
    int raw = analogRead(this->analog_pin);
    Serial.print(raw);
    Serial.print("\n");
    delay(500);

    for (size_t i = 0; i < this->mid_points.size(); i++)
    {
        const MidPoint& mp = this->mid_points[i];
        if (abs(raw - mp.value) <= this->range)
        {
            char g = mp.gear;

            if (g == '\0') {
                switch (i)
                {
                    case 0: g = 'P'; break;
                    case 1: g = 'R'; break;
                    case 2: g = 'N'; break;
                    case 3: g = 'D'; break;
                    case 4: g = 'M'; break;
                    default: break;
                }
            }

            if (g != '\0') {
                this->setGear(g);
            }
            return; // first match wins
        }
    }
    // No match => keep lastGear
}

void Gearbox_analog::setGear(char gear)
{
    if (gear != lastGear) {
        delay(100); // Debounce
        setLeds(gear);
        sendPacket(gear);
        lastGear = gear;
    }
}

void Gearbox_analog::setLeds(char gear)
{
    if (park_led)   { park_led->set_state(gear == 'P' ? HIGH : LOW); park_led->update(); }
    if (rear_led)   { rear_led->set_state(gear == 'R' ? HIGH : LOW); rear_led->update(); }
    if (neutral_led){ neutral_led->set_state(gear == 'N' ? HIGH : LOW); neutral_led->update(); }
    if (drive_led)  { drive_led->set_state(gear == 'D' ? HIGH : LOW); drive_led->update(); }
    if (manual_led) { manual_led->set_state(gear == 'M' ? HIGH : LOW); manual_led->update(); }
}

void Gearbox_analog::sendPacket(char gear)
{
    byte packet[2] = { (byte)gear, '\n' };
    Serial.write(packet, 2);
}
