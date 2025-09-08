using NWH.Common.Vehicles;
using NWH.VehiclePhysics2;
using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public class ArduinoInputReader : MonoBehaviour
{
    SerialPort serialPort;
    public string portName = "COM12";
    public int baudRate = 115200;

    public CarNetworkFunctions m_car_net_func;
    [SerializeField] VehicleController m_vehicle_data;

    private readonly StringBuilder buffer = new StringBuilder();
    private const char Separator = (char)0x1F; // ASCII Unit Separator

    private Dictionary<string, Action<char>> handlers;

    private struct Packet
    {
        public string name; // empty for gear packets
        public char value;
    }

    private bool ReadPacket(SerialPort port, out Packet pkt)
    {
        pkt = default;
        while (port.BytesToRead > 0)
        {
            int b = port.ReadByte();
            if (b < 0)
            {
                break;
            }

            char c = (char)b;
            if (c == '\n')
            {
                var buf = buffer.ToString();
                buffer.Clear();
                int sep = buf.IndexOf(Separator);
                if (sep > 0 && sep + 1 < buf.Length)
                {
                    pkt.name = buf.Substring(0, sep);
                    pkt.value = buf[sep + 1];
                    return true;
                }
                else if (buf.Length == 1)
                {
                    pkt.name = string.Empty;
                    pkt.value = buf[0];
                    return true;
                }
            }
            else if (c != '\r')
            {
                buffer.Append(c);
            }
        }
        return false;
    }

    void Start()
    {
        if (ApexManager.Instance.programType == ApexManager.ProgramType.TRAINER)
        {
            return;
        }

        handlers = new Dictionary<string, Action<char>>
        {
            {"Alarm", Alarm},
            {"ABS", ABS},
            {"TCS", TCS},
            {"ESC", ESC},
            {"StartStop", StartStop}
        };

        try
        {
            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 100;
            serialPort.Open();
            Debug.Log("Serial Port Opened: " + portName);
        }
        catch (Exception e)
        {
            Debug.LogError("Error opening serial port: " + e.Message);
        }
    }

    void Update()
    {
        if (ApexManager.Instance.programType == ApexManager.ProgramType.TRAINER)
        {
            return;
        }

        if (serialPort != null && serialPort.IsOpen)
        {
            try
            {
                Packet pkt;
                while (ReadPacket(serialPort, out pkt))
                {
                    if (string.IsNullOrEmpty(pkt.name))
                    {
                        HandleGearPacket(pkt.value);
                    }
                    else if (handlers.TryGetValue(pkt.name, out var handler))
                    {
                        handler(pkt.value);
                    }
                    else
                    {
                        Debug.Log("Unknown packet received: " + pkt.name);
                    }
                }
            }
            catch (TimeoutException)
            {
                // ignore
            }
            catch (Exception e)
            {
                Debug.LogError("An error occurred: " + e.Message);
            }
        }
    }

    void HandleGearPacket(char gear)
    {
        switch (gear)
        {
            case 'P':
                HandBreakPuled();
                break;
            case 'N':
                NeutralGearPressed();
                break;
            case 'D':
                EnabledAutomaticGearBox();
                break;
            case 'R':
                ReversedGearSet();
                break;
            case 'M':
                ManualGearWithSequential();
                break;
            default:
                Debug.Log("Unknown gear packet received: " + gear);
                break;
        }
    }

    void HandBreakPuled()
    {
        m_car_net_func.HandBreakPulled();
    }

    void NeutralGearPressed()
    {
        m_car_net_func.NeutralGearSet();
    }

    void EnabledAutomaticGearBox()
    {
        m_car_net_func.UpdateTransTypeRealTime(true, false);
    }

    void ReversedGearSet()
    {
        m_car_net_func.ReverceGearSet();
    }

    void ManualGearWithSequential()
    {
        m_car_net_func.UpdateTransTypeRealTime(false, true);
    }

    void Alarm(char state)
    {
        if (state == 'U') m_car_net_func.AlarmTriggered();
    }

    void ABS(char state)
    {
        m_car_net_func.ToggleAbs(state == 'U');
    }

    void TCS(char state)
    {
        m_car_net_func.ToggleTcs(state == 'U');
    }

    void ESC(char state)
    {
        m_car_net_func.ToggleEsc(state == 'U');
    }

    void StartStop(char state)
    {
        if (state == 'U') m_car_net_func.IgnitionTriggered();
    }

    private void OnDestroy()
    {
        if (ApexManager.Instance.programType == ApexManager.ProgramType.TRAINER)
        {
            return;
        }
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
            Debug.Log("Serial Port Closed.");
        }
    }

    void OnApplicationQuit()
    {
        if (ApexManager.Instance.programType == ApexManager.ProgramType.TRAINER)
        {
            return;
        }
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
            Debug.Log("Serial Port Closed.");
        }
    }
}
