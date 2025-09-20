# Threat Model - ReaperBoard Project

## Overview

ReaperBoard is a dual-platform hardware tool for wireless and NFC testing:

- **ReaperBoard-Lite** (ESP8266-based): Lightweight small version for stealthy operation.

This platform supports a range of wireless functions intended for **educational, auditing, and lawful penetration testing** purposes.

This document outlines the security posture of the device, potential risks, and mitigations for both the device and the environments it may operate in.

---

## Assets

| Asset                     | Description                                                 |
|---------------------------|-------------------------------------------------------------|
| User Data                 | Sensitive data entered by clients or targets, including Wi-Fi credentials, NFC card info, and system logs. |
| Device Config             | Stored settings such as MAC address presets, custom AP definitions, spoofing modes, and session history. |
| System Integrity          | Firmware stored in ESP8266 flash, configuration logic, tamper detection routines, and enforcement of secure states. |
| Operator Trust & Compliance | The expectation that the device is used ethically and legally, with proper disclosure and authorization. |

---

## Threats & Mitigations

| Threat | Vector | Impact | Mitigation |
|--------|--------|--------|------------|
| Unauthorized device access | Physical access | High | Limit access to physical only; Require a pin-code to enter the device |
| Firmware Tampering | Physical access | Medium | Firmware is stored in ESP8266 flash suggest future implementation of signed firmware and restrict physical debug access |
| Hardware Tampering | Physical access | Medium | Implement tamper detection using a magnetic sensor; design enclosure to obscure SD and I/O lines |
| Post-seizure data exposure | Physical access | High | Tamper sensor triggers SD wipe to erase MAC lists, saved APs, and logs |

---

## Tamper Detection

**Mechanism**:
A magnetic reed sensor is installed in the enclosure. If removed or opened improperly, the device:

- Detects the tamper state
- Wipes sensitive config and logs from the SD card

**Wiped Items**:
- Saved MAC addresses
- Custom AP lists
- Stored credentials (if any)
- Session logs

**Limitations**:
- Cannot prevent tamper if power is off
- Only as effective as the user's physical enclosure setup

---

## Assumptions

- Device is used in **authorized**, **legal**, and **controlled** environments
- Physical access is **restricted** by the user
- The device **does not store** sensitive credentials long-term unless explicitly configured to

---

## Known Limitations

- No secure boot or firmware verification (on current hardware).
- ESP8266 platform lacks hardware encryption or protected memory.
- Captive portal credentials are not encrypted at rest.
- Serial/debug ports, and logs may expose sensitive data if left enabled in production environments.

---

## Responsible Use Policy

ReaperBoard provides tools that can be used for good or misused. The developers:

- Do **not condone illegal activity**
- Encourage use only in **environments you own or are authorized to test**
- Strongly recommend familiarizing yourself with **local wireless laws**

Misuse (e.g. Wi-Fi deauth on public networks, phishing credentials from strangers) is **illegal in many countries**.

---

_Last updated: June 2025_
