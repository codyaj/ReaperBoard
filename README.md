# ReaperBoard

ReaperBoard is a modular, multi-platform wireless security toolkit designed for ethical penetration testing and hardware hacking. It currently includes two main versions:

- **ReaperBoard-Lite:** A lightweight, ESP8266-12E based device featuring Wi-Fi scanning, Bad AP emulation, deauthentication attacks, MAC spoofing, and RFID/NFC cloning.
- **ReaperBoard-Full:** A more powerful version (in development) built around the Raspberry Pi Compute Module 4 (CM4), adding advanced capabilities like packet sniffing, signal jamming, Wi-Fi brute forcing, and more.

---

## Project Overview

ReaperBoard aims to provide an extensible, open-source platform for wireless security researchers, pentesters, and hardware enthusiasts. By combining Wi-Fi, RFID, and network attack features into a single device (or set of devices), it enables flexible testing and exploration of wireless security vulnerabilities.

---

## Features

### ReaperBoard-Lite (ESP8266-12E)

- Wi-Fi network scanning and signal analysis  
- Bad Access Point (AP) emulation  
- Deauthentication attack module  
- MAC address spoofing  
- RFID/NFC reading and cloning  
- Customizable profiles and onboard storage  
- Self-destruct mechanism on tamper detection  
- PIN entry security  

### ReaperBoard-Full (Raspberry Pi CM4) *(coming soon)*

- Everything from Lite version  
- Packet sniffing and logging  
- Signal jamming  
- Wi-Fi brute force attacks  
- Extended storage and advanced UI  

---

## Getting Started

### Cloning the repo

```bash
git clone https://github.com/codyaj/ReaperBoard.git
cd ReaperBoard/ReaperBoard-Lite
```

### Building and Flashing (Lite)

- Install ESP8266 toolchain (Arduino IDE or PlatformIO recommended).
- Configure hardware pins and settings.
- Build and flash firmware to your ESP8266-12E board.

### Using ReaperBoard

- todo

---

## License

This project is licensed under the GNU General Public License v3 (GPLv3). See the [LICENSE](LICENSE) file for details

---

## Disclaimer

ReaperBoard is designed for ethical use only. Unauthorized access to networks or devices is illegal and unethical. Use responsibly and with permission.

---
