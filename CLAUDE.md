# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a Qt/C++ project built with qmake. Use Qt Creator or the command line:

```bash
# From the project root, generate Makefiles
qmake videoRecorder.pro

# Build debug
mingw32-make -f Makefile.Debug

# Build release
mingw32-make -f Makefile.Release
```

The project requires prebuilt FFmpeg 4.2.2 and OpenCV 4.5.4 (with Qt contrib) libraries. Their paths are configured in `videoRecorder.pro`.

There are no automated tests in this project.

## Architecture

The app is a Qt desktop application for live streaming: it captures camera + desktop video and microphone audio, encodes to H.264/AAC, saves locally as MP4, and communicates with a backend server for user login and live chat.

**Entry point:** `main.cpp` initializes FFmpeg networking (`avformat_network_init`) and creates a `Kernel` instance.

**Kernel** (`kernel.h/cpp`) is the central orchestrator. It owns all major components and wires their Qt signals/slots together. All inter-component communication goes through Kernel.

**Data pipeline:**
1. `PicInPic_Read` (OpenCV) captures camera and desktop frames, converts to YUV420P, emits `SIG_sendVideoFrameData`
2. `Audio_Read` (Qt QAudioInput) captures microphone at 44.1kHz stereo 16-bit PCM, emits `SIG_sendAudioFrameData`
3. `SaveVedioThread` (QThread) receives both streams, encodes with FFmpeg (H.264 + AAC), and muxes to file or live stream URL

**Network:** `Network` connects via TCP to the server (hardcoded `192.168.167.1:8888`). Protocol: 4-byte length prefix + JSON payload. `CJson.h` is a custom JSON parser used for this protocol.

**UI flow:** Login → (on success) RecorderDialog with live preview → MessageDialog (enter roomId/title/stream link/type) → streaming starts → Room dialog shows incoming live comments from server.

**Video preview:** `CameraWidget` receives `QImage` frames from `PicInPic_Read` via `SIG_sendVideoFrame` and paints them directly.

**Beauty filter:** A whitening effect (`whiteFace`) can be toggled; it is applied inside `SaveVedioThread` before encoding.

## Key Constants & Configuration

- Server address: hardcoded in `network.cpp` as `192.168.167.1:8888`
- Audio: 44.1kHz, 2ch, 16-bit PCM; frame size 4096 bytes, 20ms interval
- Video codec: H.264 (AV_CODEC_ID_H264), pixel format YUV420P
- Audio codec: AAC
- Server message type IDs: 10002 (login response), 10006 (comments), 10008 (registration response)
