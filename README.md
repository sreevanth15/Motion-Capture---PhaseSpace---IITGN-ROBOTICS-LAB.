

# PhaseSpace Mocap Python Client
# PhaseSpace Mocap Setup and Operation Guide

## Windows setup scope (current)

This guide is currently for the **Windows setup only**.
Linux setup documentation will be added in a future update.

For a new computer, configure a **static IPv4 address** on the Ethernet adapter before trying to connect to the PhaseSpace server (same subnet as the server, example network `192.168.1.x`).

## Startup flow chart (do this first)

```mermaid
flowchart TD
    A[Switch on PhaseSpace server and camera system] --> B[Ensure server is connected to hub [ip:10.0.0.1]]
    B --> C[Check physical/network connections]
    C --> D[Open web Configuration Manager at server IP]
    D --> E[Login for web config: admin / phasespace]
    E --> F[Verify cameras and ports]
    F --> G[Server-level access if needed: root / phasespace]
    G --> H[Open Master Client app on host PC]
    H --> I[Connect to server IP and verify stream]
    I --> J[Run code: example.py or owl.py]
```

Short sequence:
1. Switch on hardware in order:
    - Turn on the PhaseSpace hub first.
    - Turn on the PhaseSpace server next.
    - Turn on cameras and confirm RF camera is active.
2. Confirm server is connected to a monitor and fully booted.
3. For reliable connection, perform one restart cycle after initial power up:
    - Restart hub/server once if cameras are not detected or link is unstable.
    - Wait 30-60 seconds after restart before scanning ports.
4. Check physical connections:
    - Ethernet from host PC to server Ethernet port (not hub camera chain ports).
    - Camera chain cabling is secure and in correct direction.
5. Check host network:
    - Ensure host PC has static IP configured for the PhaseSpace network.
    - Verify reachability to server IP (`192.168.1.230`) using ping.
6. Open web Configuration Manager and login with `admin / phasespace`.
7. Scan ports and verify all expected cameras are visible.
8. Open Master Client app and connect to the server IP.
9. If server terminal/admin access is needed, use `root / phasespace`.
10. After connection is healthy in Master Client, start Python code.

This repository contains a Python client for PhaseSpace motion capture and reference project files. This guide is written from scratch to cover:

- environment setup
- device login and network checks
- ordered documentation reading steps
- running the Python code
- collecting and saving mocap data
- troubleshooting common failures

## 1) Repository contents

- `example.py`: minimal Python streaming client.
- `owl.py`: libowl2 Python API implementation and command-line client.
- `PhaseSpace Documents/`: vendor manuals in PDF format.
- `phasespace-master/`: additional LabVIEW and C++ integration project.

Vendor PDFs included:

- `PhaseSpace Documents/X2E-Quick-Start-Guide.pdf`
- `PhaseSpace Documents/X2E-Master-Client-Guide.pdf`
- `PhaseSpace Documents/X2E-API Guide.pdf`

## 2) Device and login details

- Device IP address: `192.168.1.230`
- Web Configuration Manager login: `admin` / `phasespace`
- Server-level login: `root` / `phasespace`

Keep credentials private in production environments. Rotate passwords if this repository is shared outside a trusted network.

## 3) Hardware and system setup (from Quick Start Guide)

### 3.1 Initial camera setup

**Equipment needed:**
- Cameras (60° field of vision each)
- RF camera (required for system to operate)
- Server with hub
- Connecting cables
- Computer with browser and Master Client

**Steps:**

1. **Position cameras** around your capture space.
   - Arrange in a circular or distributed pattern.
   - Ensure overlap between adjacent camera views.
   - Keep cameras at least a few feet apart.
   - RF camera must be included in the configuration.

2. **Connect cameras in chains** (maximum 6 cameras per chain).
   - Turn off the hub with the white button before connecting/disconnecting.
   - Connect camera's red (Previous/HUB) port to a server hub port.
   - Connect blue (Next) port of one camera to red port of the next camera.
   - Continue building chains until all cameras connected.

3. **Connect client machine** to the server.
   - Use the Ethernet port on the back of the server (NOT hub ports).
   - Consult network administrator for proper routing.

4. **Verify connections** in Configuration Manager.
   - Turn on the server (blue power light will turn on).
   - Open web browser and enter the server IP address (e.g., `192.168.1.230`).
   - Login with username `admin` and password `phasespace`.
   - Click "Scan Ports" to detect connected cameras.
   - Confirm number of cameras in list matches your physical setup.

### 3.2 Device (LED driver) registration

**Equipment needed:**
- Driver or Micro Driver (charge battery first)
- Calibration wand (8 LEDs) or micro wand (4 LEDs)
- Configuration Manager access
- Registered driver

**Steps:**

1. **Power on the driver** by pressing the power button.
   - Indicator light will turn on when active.
   - To standby: press power button (light blinks slowly).
   - To fully off: press and hold until light blinks 7 times.

2. **Register drivers** in Configuration Manager.
   - Go to LED Devices tab.
   - Click "Scan & Monitor Devices" to open RFScan dialog.
   - Set Global Power Setting according to your space size:
     - 25'×25' space → power level 45–55
     - Darker room → lower setting
     - Outdoor/bright → higher setting
   - Click "Scan" to search for drivers.

3. **Pair new drivers**.
   - Press and hold the power button until indicator blinks 3 times.
   - Driver should appear in the Drivers list.
   - Battery indicator must be on (charge via USB if needed).

4. **Assign driver IDs**.
   - Set calibration object driver to ID = 1.
   - Click "edit IDs" in RFScan dialog.
   - Adjust ID column and click "save IDs".

### 3.3 Calibration procedure

**Equipment needed:**
- Paired driver connected to calibration wand
- Master Client computer connected to system
- Capture space prepared with positioned cameras

**Pre-calibration steps:**

1. Start **PhaseSpace Master Client**.
2. Enter the server IP in OWL Configuration panel or select from dropdown.
3. Navigate to **Tools → Calibration** to enter calibration mode.
4. When prompted, select tracker file:
   - `wand.json` for 8-LED calibration wand
   - `wand-micro.json` for 4-LED micro wand

**Camera adjustment:**

5. Place calibration wand in center of capture space (LEDs facing up).
6. In Master Client 2D viewer, look for green diamonds (camera centers).
7. Adjust each camera's angle/position so white dots (wand markers) are near center of view.
8. Adjust LED Power slider in OWL Configuration for optimal light levels:
   - Yellow lines = LED too strong
   - Brown lines = LED too weak
   - Red lines = cannot distinguish source (may need power adjustment)

**Data capture and calibration:**

9. Click **Capture** button to start data collection.
10. Move the calibration wand around the capture space:
    - Hold wand so LEDs face outward toward cameras.
    - Keep moving until all camera views are at least 50% filled with green.
    - Don't block camera line-of-sight.

11. Select calibration quality:
    - `fastest` = quick (less accurate)
    - `normal` = recommended
    - `exhaustive` = thorough (longer processing)

12. Click **Calibrate** button. After 4 passes, calibration completes.
13. If calibration fails: click **Reset**, recapture data, and retry.
14. On success, click **Save** to save calibration.

### 3.4 Alignment procedure

**Equipment needed:**
- Registered driver and calibration wand
- Calibrated system (from section 3.3)
- Master Client

**Steps:**

1. In calibration mode, click **Align** to enter alignment mode (or use **Tools → Alignment**).
2. Verify tracker file matches alignment object (`wand.json` or `wand-micro.json`).
3. Click **Auto Snapshot** to enable automatic point placement.
4. Stand in center of capture space facing the desired "forward" direction.
   - Keep wand moving to avoid accidental snapshots.

5. **Set origin** (center of space):
   - Place wand on ground upright and hold still.
   - Bottom of wand is the end farthest from the driver.
   - 3D viewer will flash green when captured.

6. **Set X axis**:
   - Take one step left.
   - Place wand on ground upright and hold still.
   - Screen will flash green when captured.

7. **Set Z axis**:
   - Step back to origin, then take one step forward.
   - Place wand on ground upright and hold still.
   - Screen will flash green when captured.

8. To restart: click **Reset** and repeat steps 5–7.
9. When satisfied, take wand out of space to prevent further snapshots.
10. Click **Save** to save alignment.
11. Click **Done** to close alignment dialog. System is now ready for streaming and recording.

## 4) Master Client streaming and recording

### 4.1 Streaming motion capture data

Once the system is calibrated and aligned, you can stream live marker and rigid-body data.

**Prerequisites:**
- System calibrated and aligned
- Master Client connected to server
- LED drivers powered and paired

**Steps:**

1. In OWL Configuration panel, set the following:
   - **Address**: IP of the Impulse X2 server
   - **Profile**: Select desired tracking mode (from Configuration Manager profiles)
   - **Frequency**: Set capture frame rate (Hz)
   - **LED Power**: Adjust brightness (0–100%), can be changed while streaming
   - **Postprocess**: Enable if marker data smoothing desired
   - **Interpolate**: Max frame gap to auto-interpolate (recommended > number of LED groups)

2. Click **Connect** button to start live stream.
   - Camera side lights will flash, then settle on green.
   - RF camera will show blinking yellow light on its face.

3. Monitor 3D and 2D viewer panels:
   - Markers appear as white dots.
   - Rigid bodies appear as colored shapes.
   - Camera view cones visible in 3D.

### 4.2 Recording data in Master Client

**Steps:**

1. Set working directory:
   - Click **File → Working Directory**.
   - Select folder where recordings will be saved.
   - Working directory path will display in top toolbar.

2. Configure server and connect (see section 4.1, step 1–2).

3. Set take name in the **Take Name** field in OWL Tools.

4. Click **Record** button to start recording.
   - Red timer will appear showing elapsed time.

5. Move markers and rigids during capture to collect full data.

6. Click **Record** button again to stop recording.
   - Data saved as `.RPD` (motion capture data format).

**Optional C3D export:**
- Go to **Settings → Configure**.
- Check **Record C3D** checkbox to also save `.C3D` files alongside `.RPD`.

### 4.3 RPD playback

**Steps:**

1. Set working directory to folder containing `.RPD` files.
2. In OWL Configuration panel, enter IP of a server **not currently streaming**.
3. **RPD Playlist** panel will auto-populate with available recordings.
4. Double-click an `.RPD` file to begin playback.
5. Use same controls as live streaming (zoom, pan, show IDs, etc.).
6. Click **Connect** button to stop playback.

### 4.4 Rigid body creation and editing

**Create from live stream:**
1. Select at least 3 markers in 3D viewer (click and drag).
2. Right-click in OWL Trackers panel.
3. Select **Add Rigidbody Tracker**.

**Create from tracker file:**
1. Right-click in OWL Trackers panel.
2. Select **Load Tracker File**.
3. Select `.json` file containing marker positions.

**Edit rigid body:**
1. Double-click rigid body in OWL Trackers panel to open Rigid Body Editor.
2. **Transform** tab:
   - Translate along X, Y, Z axes.
   - Rotate around X, Y, Z axes.
3. **Insert** tab:
   - Manually add/remove individual markers.
   - Specify marker ID and position.
4. Click **Apply** to save changes.

## 5) Host PC prerequisites

- Windows host with network route to `192.168.1.230`
- Python 3 installed and available in terminal (`python --version`)

No external Python packages are required for this repository.

## 6) Python API reference (owl.py)

The `owl.py` module provides a Python implementation of the libowl2 API for programmatic access to the mocap system.

### 6.1 Core class: Context

```python
import owl

# Create context
context = owl.Context()

# Connect to server
context.open("192.168.1.230", "timeout=10000000")

# Initialize session
context.initialize("streaming=1")

# Set frequency (Hz)
context.frequency(120)

# Enable streaming
context.streaming(1)

# Poll for events
while context.isOpen() and context.property("initialized"):
    event = context.nextEvent(1000000)  # timeout in microseconds
    if not event:
        continue
    
    # Process event
    if event.type_id == owl.Type.FRAME:
        print(event)
    elif event.type_id == owl.Type.ERROR:
        print(f"Error: {event.name} - {event.data}")
        if event.name == "fatal":
            break

# Clean up
context.done()
context.close()
```

### 6.2 Event types

Events returned by `context.nextEvent()` have:

- `type_id`: Event type identifier
- `id`: Session-dependent event ID
- `flags`: Event flags
- `time`: Timestamp (in frames)
- `type_name`: Human-readable type name
- `name`: Event name (for BYTE events)

**Common event types:**

| Type | Meaning | Contains |
|------|---------|----------|
| `FRAME` | Synchronized marker/rigid data | `markers`, `rigids`, `cameras` |
| `CAMERA` | Camera information | Camera pose and condition |
| `DEVICEINFO` | Device metadata | Device name, type, status |
| `ERROR` | System error | `name` (error type), `data` (details) |
| `BYTE` | String event | `name`, `data` (string payload) |
| `MARKER` | Individual marker | Position [x, y, z], condition number |
| `RIGID` | Rigid body | Pose [x, y, z, s, x, y, z], condition |

### 6.3 Frame event data structures

**Marker:**
- `id`: Marker identifier
- `x, y, z`: 3D position (millimeters, or configured scale)
- `cond`: Condition number (quality metric, high positive = good)
- `flags`: Prediction/quality flags

**Rigid:**
- `id`: Rigid body identifier
- `pose`: [x, y, z, s, qx, qy, qz] (position + quaternion rotation)
- `cond`: Condition number (constraint planes, high positive = good)
- `flags`: Kalman filter flags

**Camera:**
- `id`: Camera ID
- `pose`: [x, y, z, s, qx, qy, qz] (position + quaternion)
- `cond`: Condition number
- `flags`: Camera status flags

### 6.4 Context methods

| Method | Purpose | Returns |
|--------|---------|---------|
| `open(host, options)` | Connect to server | 1 (success), 0 (timeout), <0 (error) |
| `close()` | Disconnect and clean up | boolean |
| `initialize(options)` | Start session | 1 (success), 0 (timeout), <0 (error) |
| `done()` | End session | 1 (success), 0 (timeout), <0 (error) |
| `isOpen()` | Check connection status | boolean |
| `streaming(enable)` | Enable/disable data stream | boolean |
| `frequency(hz)` | Set capture rate | boolean |
| `property(name)` | Query system property | varies (see below) |
| `nextEvent(timeout_us)` | Get next event, block until timeout | Event or None |
| `peekEvent(timeout_us)` | Look at next event without removing | Event or None |
| `option(key, value)` | Set option | boolean |
| `createTracker(id, type, name, options)` | Create tracker | boolean |
| `assignMarkers(markers_list)` | Assign markers to tracker | boolean |

### 6.5 Open and initialize options

**open() options:**
```
timeout=<microseconds>  # Connection timeout (default 5000000)
```

**initialize() options:**
```
streaming=<0|1>         # Auto-enable streaming (default 0)
slave=<0|1>             # Slave mode (default 0)
profile=<name>          # Tracker profile name
frequency=<hz>          # Capture frame rate
timeout=<microseconds>  # Session timeout
```

### 6.6 Data collection patterns

**Save to CSV:**
```python
import csv

with open('markers.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['frame', 'marker_id', 'x', 'y', 'z', 'cond'])
    
    while context.isOpen():
        event = context.nextEvent(1000000)
        if not event or event.type_id != owl.Type.FRAME:
            continue
        
        if "markers" in event:
            for m in event.markers:
                writer.writerow([event.time, m.id, m.x, m.y, m.z, m.cond])
```

**Save to JSON:**
```python
import json

frames = []
while context.isOpen():
    event = context.nextEvent(1000000)
    if not event or event.type_id != owl.Type.FRAME:
        continue
    
    frame_data = {
        'time': event.time,
        'markers': [{'id': m.id, 'pos': [m.x, m.y, m.z], 'cond': m.cond} 
                    for m in event.markers] if "markers" in event else [],
        'rigids': [{'id': r.id, 'pose': list(r.pose), 'cond': r.cond}
                   for r in event.rigids] if "rigids" in event else []
    }
    frames.append(frame_data)

with open('mocap.json', 'w') as f:
    json.dump(frames, f, indent=2)
```

## 7) Installation and setup

1. Open a terminal in this repository root.
2. Verify Python:

```bash
python --version
```

3. Verify network reachability to the device:

```powershell
ping 192.168.1.230
```

4. Ensure PhaseSpace system is powered and initialized before running scripts.

## 8) Quick start: Running Python clients

### Option 1: Minimal example

```bash
python example.py 192.168.1.230
```

This script:
1. Creates `owl.Context()`.
2. Connects to the server.
3. Initializes streaming session.
4. Polls for events and prints markers, rigids, and errors.
5. Closes cleanly on completion or error.

### Option 2: Full CLI from owl.py

```bash
python owl.py --device 192.168.1.230 --timeout 5000000
```

Useful flags:
- `--scan`: Discover all OWL servers on local network
- `--freq <hz>`: Set capture frame rate
- `--slave`: Run as slave (for multi-connection scenarios)
- `--debug`: Enable debug output
- `--peaks`, `--planes`, `--inputs`, `--hub`, `--rx`: Enable advanced event types
- `--options "<key=value ...>"`: Pass custom initialization options

## 9) Data collection via Python scripts

### Save to file (text)

```bash
python example.py 192.168.1.230 > mocap_capture.txt 2>&1
```

### Save to file (CSV)

Modify `example.py` to write CSV format:

```python
import csv
with open('capture.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['frame', 'marker_id', 'x', 'y', 'z', 'cond'])
    # ... event loop with writer.writerow() calls
```

### Recommended capture procedure

1. Verify system is calibrated and aligned in Master Client.
2. Power on all LED drivers.
3. Start Python script: `python example.py 192.168.1.230`.
4. Confirm initial `FRAME` events are printing with marker data.
5. Begin collecting actor/marker motion.
6. Run for full trial duration.
7. Stop with `Ctrl+C`.
8. Archive output file with session date/time in filename.

## 10) phasespace-master reference (LabVIEW/C++)

Based on `phasespace-master/README.md`:

1. Turn on all computers and machines before starting.
2. Wait at least one minute for full initialization.
3. Do not change tracker profiles if ID mapping must remain fixed.
4. Start `PSP.exe` first (from `phasespace-master/x64/Release/` when using C++/LabVIEW integration).
5. Then run `socket_target.vi` from `phasespace-master/LabVIEW/` for LabVIEW connections.

Project structure in `phasespace-master/`:
- `PSP/`: C++ server implementation
- `LabVIEW/`: LabVIEW VIs for socket communication
- `socket_client/`: Legacy client code
- `x64/`: Build output and release artifacts

## 11) Troubleshooting

| Problem | Cause | Fix |
|---------|-------|-----|
| `IndexError: list index out of range` | Missing server argument | Pass device IP: `python example.py 192.168.1.230` |
| `socket select error` or `connection timed out` | Network/device unreachable | Verify IP, network cable, firewall, and that PhaseSpace service is running. Check `ping 192.168.1.230`. |
| `python` command not found | Python not installed or not on PATH | Install Python 3 from python.org. Reopen terminal. |
| No FRAME events in output | Streaming not enabled or no markers visible | Ensure drivers are powered and visible in Master Client. Call `context.streaming(1)` or add `streaming=1` to initialize options. |
| Marker data looks erratic | Calibration or alignment error | Re-run calibration and alignment procedures (sections 3.3 and 3.4). |
| Connection works but stops during capture | Session timeout | Increase timeout value: `python owl.py --device 192.168.1.230 --timeout 30000000`. |
| RPD files unreadable | Wrong server IP or format issue | Verify server IP in Master Client Address field. Ensure files are in `.RPD` format, not corrupted. |

## 12) Advanced Python patterns

### Multi-event processing

```python
import owl

context = owl.Context()
context.open("192.168.1.230", "timeout=5000000")
context.initialize("streaming=1")

marker_count = 0
rigid_count = 0
frame_count = 0

while context.isOpen() and context.property("initialized"):
    event = context.nextEvent(1000000)
    if not event:
        continue
    
    if event.type_id == owl.Type.FRAME:
        frame_count += 1
        if "markers" in event:
            marker_count += len(event.markers)
        if "rigids" in event:
            rigid_count += len(event.rigids)
    
    elif event.type_id == owl.Type.ERROR:
        print(f"ERROR: {event.name} - {event.data}")
        if event.name == "fatal":
            break
    
    elif event.type_id == owl.Type.DEVICEINFO:
        print(f"Device: {event.name} ({event.type}) - {event.status}")

print(f"Captured {frame_count} frames, {marker_count} markers, {rigid_count} rigids")

context.done()
context.close()
```

### Creating trackers programmatically

```python
import owl

context = owl.Context()
context.open("192.168.1.230")
context.initialize()

# Create a point tracker
context.createTracker(1, "point", "marker_1", "")

# Create a rigid body from marker positions
markers = [
    owl.MarkerInfo(1, 2, "shoulder", "pos=0,0,0"),
    owl.MarkerInfo(2, 2, "elbow", "pos=100,0,0"),
    owl.MarkerInfo(3, 2, "wrist", "pos=200,0,0"),
]
context.assignMarkers(markers)

context.streaming(1)

while context.isOpen() and context.property("initialized"):
    event = context.nextEvent(1000000)
    if not event:
        continue
    if event.type_id == owl.Type.FRAME and "rigids" in event:
        for r in event.rigids:
            print(f"Rigid {r.id}: pos=({r.pose[0]:.1f}, {r.pose[1]:.1f}, {r.pose[2]:.1f})")

context.done()
context.close()
```

### Filtering and smoothing

```python
import owl

context = owl.Context()
context.open("192.168.1.230")

# Apply interpolation and smoothing on server side
init_options = "streaming=1 interpolate=5 smooth=3"
context.initialize(init_options)

# Or create a filter
context.filter(2, "smooth_filter", "Type=linear")

context.streaming(1)

while context.isOpen() and context.property("initialized"):
    event = context.nextEvent(1000000)
    if not event:
        continue
    if event.type_id == owl.Type.FRAME and "markers" in event:
        for m in event.markers:
            if m.cond > 0:  # Only good markers
                print(f"Marker {m.id}: ({m.x:.1f}, {m.y:.1f}, {m.z:.1f}) cond={m.cond:.2f}")

context.done()
context.close()
```

## 13) End-to-end setup and capture checklist

System setup (once per deployment):
- [ ] Position and connect cameras in chains (section 3.1)
- [ ] Register and pair LED drivers (section 3.2)
- [ ] Run calibration procedure (section 3.3)
- [ ] Run alignment procedure (section 3.4)

Capture day workflow:
- [ ] Power on all mocap hardware and host PC
- [ ] Wait 1 minute for full initialization
- [ ] Open Master Client and verify system health
- [ ] Verify network: `ping 192.168.1.230` succeeds
- [ ] Power on all LED drivers
- [ ] Open capture script: `python example.py 192.168.1.230`
- [ ] Confirm FRAME events printing with marker data
- [ ] Perform trial/recording
- [ ] Stop script with `Ctrl+C`
- [ ] Check output file was created
- [ ] Archive file with session date/time
- [ ] Power down drivers and hardware

## 14) File and document references

| File | Purpose |
|------|---------|
| `example.py` | Minimal streaming example |
| `owl.py` | Full libowl2 Python implementation |
| `PhaseSpace Documents/X2E-Quick-Start-Guide.pdf` | Hardware setup, calibration, alignment |
| `PhaseSpace Documents/X2E-Master-Client-Guide.pdf` | Master Client UI, recording, playback |
| `PhaseSpace Documents/X2E-API Guide.pdf` | Low-level API, data structures, Context methods |
| `phasespace-master/README.md` | LabVIEW and C++ integration notes |

