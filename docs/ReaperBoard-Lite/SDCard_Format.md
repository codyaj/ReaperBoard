# SD Card Data Format - ReaperBoard-Lite

> ⚠️ **Note:** If `wipeOnTamper` is enabled, the SD card contents will be irreversibly erased upon tamper detection. You will not be able to retrieve any data afterward.

---

## Settings (`settings.json`)

All device configuration values are stored in `settings.json`.

| Key | Type | Description |
|-----|------|-------------|
|`enableLogs`|`bool`|Enables or disables logging|
|`wipeOnTamper`|`bool`|If true, all SD data is wiped on tamper detection. Does *not* affect manual wipes.
|`passcode`|`str`|A 6-digit numeric passcode (only 0-9) used to unlock the device.
|`screenTimeout`|`int`|Time in seconds before the screen turns off due to inactivity.

---

## Logs (`logs.txt`)

If logging is enabled, the file `logs.txt` will contain significant events only. If the file is empty, it means no log-worthy events have occurred.

Each log entry follows this format:

```
[SourceModule] Description of the event or error
```

**Example**

```
[SDManager] Failed to open settings for readings: settings.json
```

---

## Captured Data (`data/` folder)

Captured POST data is stored inside the `data/` directory. Each file has a randomly generated filename.
- **First Line:** Name/ID of the *Bad AP* the data was captured from.
- **Remaining Lines:** Each line corresponds to a POST parameter to a POST parameter in the format:
```
<key>: <value>
```

---

## Bad Access Points (`badaps/` folder)

Each fake AP configuration is stored as a unique file in the badaps/ directory. The filename is used as the display name and selector for the attack.

**BadAP File Format (TBD)**
> This section is currently a *placeholder*

---

## Custom MAC Addresses (`macs/` folder)

Custom MAC addresses for spoofing are stored in the `macs/` directory. Each file should contain a single MAC address in uppercase hexadecimal, without separators.

**Example:**
```
AA11BB22CC33
```


> Last Updated 15/7/25
