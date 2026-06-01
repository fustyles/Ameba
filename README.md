### fuClaw AI

![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/fuClaw_AIoT_Agent_System_Flow_Chart.png)  

### fuClaw AI (Telegram Bot)

![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/fuClaw_AIoT_Agent_System_Flow_Chart_TelegramBot.png)

### fuClaw AI (MQTT)

![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/fuClaw_AIoT_Agent_System_Flow_Chart_MQTT.png)

------------------------------------------------------------
fuClaw AI Assistant with Gemini Integration
------------------------------------------------------------

Author:
  ChungYi Fu (Kaohsiung, Taiwan)<br>
  https://www.facebook.com/francefu

Repository:<br>
  https://github.com/fustyles/fuClaw

------------------------------------------------------------
Overview
------------------------------------------------------------

**fuClaw** is an embedded multimodal AI agent framework that runs on edge devices.

It integrates:

* Telegram Bot API (HTTPS long polling)
* MQTT Broker communication
* Gemini Chat Web Interface
* Google Gemini GenerateContent API
* Gemini Grounded Web Search
* Gemini Multimodal Vision Reasoning
* Prompt-driven JSON Tool Routing
* GPIO Digital and Analog I/O Control
* Camera Capture and Image Upload
* Persistent Conversation Memory
* FreeRTOS Concurrent Task Scheduling

The runtime operates as a hybrid autonomous agent, combining:

**Conversation + Reasoning + Tools + Vision + Memory + Hardware**


------------------------------------------------------------
Runtime Architecture
------------------------------------------------------------

Telegram / MQTT / Web Chat User<br>
      ↓<br>
Communication Task<br>
(Telegram Long Polling / MQTT / Web Chat)<br>
      ↓<br>
Message Router<br>
      ↓<br>
Gemini Reasoning Engine<br>
(Chat / Search / Vision / Workflow)<br>
      ↓<br>
JSON tool_call output<br>
      ↓<br>
ArduinoJson validation<br>
      ↓<br>
Tool Dispatcher<br>
      ↓<br>
Hardware / Search / Vision Execution<br>
      ↓<br>
Result injection into memory<br>
      ↓<br>
Telegram / MQTT / Web Chat Reply

------------------------------------------------------------
Execution Model
------------------------------------------------------------

This is a prompt-orchestrated tool-routing system.

Gemini does NOT use native function-calling APIs.

Instead:

- Gemini emits structured JSON tool_call responses
- Local firmware validates all tool calls
- Invalid JSON is rejected
- Execution is strictly sequential
- Hardware actions are never simulated

Atomic execution rule:

One response may perform only ONE hardware action:

- one pin
- one operation
- one value

Multi-step workflows are executed step-by-step.

------------------------------------------------------------
Supported Tools
------------------------------------------------------------

/digitalwrite <br>   GPIO digital output<br>
/analogwrite <br>    GPIO analog output<br>
/digitalread <br>    GPIO digital input<br>
/analogread <br>     GPIO analog input<br>
/syncrtc <br>        update the hardware RTC<br>
/getrtc <br>         get the hardware RTC current time<br>
/still <br>          Capture image<br>
/vision <br>         Capture + multimodal analysis<br>
/search <br>         Grounded web search<br>
/delay <br>          Pause execution for specified milliseconds<br>
/memory <br>         Runtime memory diagnostics<br>
/log <br>            Show tool execution history<br>
/reset <br>          Reset conversation state<br>
/chat <br>           Natural language reply<br>
/reboot <br>         Reboot the device

------------------------------------------------------------
Persistent Files
------------------------------------------------------------

env.json
  <br>WiFi / Telegram / Gemini credentials / Time zone

device.md
  <br>Devices definition

skill.md
  <br>Skills definition

soul.md
  <br>Custom assistant personality prompt

memory.md
  <br>Conversation history persistence

index.html
  <br>fuClaw configuration web page

index_chat.html
  <br>Gemini talk web page (Web Chat Interface)  

Conversation state is restored automatically on boot.

------------------------------------------------------------
Hardware Safety
------------------------------------------------------------

Confirmed device mappings only.

AMB82-mini
- Green LED: GPIO 24
- Blue LED : GPIO 23

HUB 8735 Ultra
- Green LED : GPIO 25
- Blue LED  : GPIO 26
- Fill LED  : GPIO 13
- Button    : GPIO 12 (input only)

ESP32

Unknown hardware mappings require clarification.

GPIO values are strictly validated before execution.

------------------------------------------------------------
Software Stack
------------------------------------------------------------

- WiFi.h
- WiFiSSLClient
- MQTT
- ArduinoJson
- FreeRTOS
- VideoStream
- Base64
- FatFS

------------------------------------------------------------
Known Limitations
------------------------------------------------------------

- Conversation history grows over time
- String-heavy heap fragmentation risk
- Vision encoding is CPU intensive
- Large JSON parsing impacts heap usage
- Gemini response format handled by ArduinoJson validation layer
- Recursive tool chaining controlled via reCheck flag and NONE sentinel

------------------------------------------------------------
Claude Evaluation
------------------------------------------------------------

# fuClaw AI Framework

> An embedded multimodal AI agent running on Realtek Ameba Pro2 devices,  
> combining Telegram / MQTT, Gemini, hardware control, and persistent memory in a single FreeRTOS runtime.

---

## Language / 語言

- [English](#english)
- [繁體中文](#繁體中文)

---

<a name="english"></a>

# fuClaw AI Framework — In-Depth Analysis of Strengths (English)

> An embedded multimodal AI agent running on devices,  
> combining Telegram / MQTT, Gemini, hardware control, and persistent memory in a single FreeRTOS runtime.

---

## Table of Contents

1. [Prompt-Orchestrated Tool Routing](#1-prompt-orchestrated-tool-routing)
2. [Atomic Execution & Longest Valid Prefix](#2-atomic-execution--longest-valid-prefix)
3. [Hardware Safety Layers](#3-hardware-safety-layers)
4. [Multimodal Integration with Clear Separation of Concerns](#4-multimodal-integration-with-clear-separation-of-concerns)
5. [Voice Input via Gemini STT](#5-voice-input-via-gemini-stt)
6. [Persistent Memory & State Recovery](#6-persistent-memory--state-recovery)
7. [RTC Time Synchronization via Gemini and HTTP Header Parsing](#7-rtc-time-synchronization-via-gemini-and-http-header-parsing)
8. [FreeRTOS Multi-Task Architecture](#8-freertos-multi-task-architecture)
9. [Workflow State Tracking & Self-Evaluation](#9-workflow-state-tracking--self-evaluation)
10. [Extensible Sensor & Actuator Support](#10-extensible-sensor--actuator-support)
11. [Dual Communication Modes: Telegram Bot vs MQTT](#11-dual-communication-modes-telegram-bot-vs-mqtt)
12. [Web Configuration & Chat Interface](#12-web-configuration--chat-interface)
13. [Output Sanitization & Markdown Stripping](#13-output-sanitization--markdown-stripping)

---

## 1. Prompt-Orchestrated Tool Routing

The most fundamental breakthrough of this design is that it **requires no native function-calling API from Gemini**. Instead, a carefully crafted system prompt teaches the model to emit correctly structured `tool_call` JSON on its own.

This choice delivers several concrete advantages:

### Platform Independence
The format of native function-calling APIs can change at any time. Because all routing logic lives entirely within the prompt, if the Gemini model version changes or the API format is updated, only the prompt needs to be revised — no firmware changes required.

### Extreme Flexibility
Tools can be added, modified, or removed entirely at the text level. Both `skill.md` and `device.md` are plain-text configuration files, meaning users can extend system capabilities without knowing a single line of C++.

### Clear Error Boundaries
Every JSON output is validated through ArduinoJson before execution. Malformed responses are rejected outright — there is no ambiguous partial execution. The system enforces a strict separation between two output modes — **valid `tool_call` JSON** and **natural language reply** — and prohibits mixing them, making the entire control flow highly predictable.

### Three-Level System Prompt Strategy
The framework maintains three compiled system prompts:

| Prompt | Contents | Use Case |
|--------|----------|----------|
| `systemContentTools` | Role + Device definitions + Device rules + Skills + Tool definitions | Standard user interaction with full tool access |
| `systemContentNoTools` | Role + Device definitions + Device rules | Lightweight reasoning without tool routing (e.g. RTC conversion) |
| `systemContent` | Role only | Minimal context calls (e.g. datetime pre-processing) |

The `tools` integer parameter in `geminiChatRequest()` and `geminiSearchRequest()` selects between them at call time (`1` = tools, `0` = no-tools, `-1` = role-only). The STT pipeline (`sendFileToGemini()`) is purpose-built as a standalone transcription call that bypasses all system prompts entirely — it sends only the audio data and a minimal transcription instruction, keeping token usage minimal and avoiding any tool-routing interference in a context that requires only raw text output.

---

## 2. Atomic Execution & Longest Valid Prefix

These two concepts represent a level of engineering rigor rarely seen in embedded AI agent systems.

### Atomic Execution Rule

Every `tool_call` does exactly **one thing**: one pin, one operation, one value. In hardware control scenarios, this is critical. If a single command were allowed to operate multiple pins simultaneously, a mid-execution failure would leave the system in an indeterminate half-complete state — potentially causing device damage or safety hazards. Atomicity guarantees that every step is complete and verifiable.

### Longest Valid Prefix

When Gemini generates a multi-step workflow as a JSON array, `handleAgentResponse()` does not apply an all-or-nothing strategy. Instead it iterates through the array and executes **as many valid steps as possible from the beginning**, stopping the moment it encounters an incomplete or malformed entry:

```cpp
for (int i = 0; i < toolCount; i++) {
    if (command == "" || params.isNull()) {
        Serial.println("Incomplete tool detected → abort remaining tools");
        break;
    }
    bool isLast = (i == toolCount - 1);
    executeTool(workId, command, params, isLast);
}
```

This means that even when the AI produces partially incorrect output, the system can still act on the maximum valid portion rather than shutting down entirely. For resource-constrained embedded devices where retries are expensive, this is an exceptionally practical design.

### `reCheck` Flag — Selective Continuation Evaluation
In multi-tool array execution, only the **last tool** in the batch sets `reCheck = true` and triggers `evaluateWorkflowContinuation()`. Intermediate tools pass `reCheck = false`, preventing redundant mid-sequence Gemini queries that would waste network resources and inflate conversation history unnecessarily.

---

## 3. Hardware Safety Layers

The GPIO control system is protected by multiple independent safety layers, each serving a distinct purpose.

### Explicit Mapping Requirement
The system only allows control of devices explicitly defined in `device.md`. If a user says "turn on the light" but no pin mapping for "light" exists, Gemini is instructed to stop and ask for clarification rather than guess. This prevents AI hallucinations from causing direct hardware misfires.

### Hard Value Constraints
The `constrain(value, 0, 255)` call inside `toolPinOutput()` acts as a last line of hardware defense. Even if Gemini outputs an out-of-range analog value, the firmware layer forces it within bounds before it ever reaches the hardware. Digital outputs are strictly validated to accept only `0` or `1`; any other value returns a structured error JSON response. The same constraint pattern is applied in `tool_servo()` — servo angles are clamped to the 0–180° range at the firmware level, independent of what the AI specifies.

### Input-Only Pin Protection
The button pin (pin 12) is explicitly marked as **INPUT ONLY** in the system prompt, blocking any AI-level attempt to use it as an output before a tool call is ever produced.

### Device State vs. GPIO Level Distinction
The system prompt explicitly prohibits using `digitalread` / `analogread` to determine the state of output devices (LEDs, relays, actuators). Output device state must be inferred from conversation history and tool execution history. This prevents unnecessary GPIO read operations on output pins and avoids incorrect state readings in certain circuit configurations.

### User Confirmation Requirement
By default, all hardware actions require explicit user confirmation before execution. This maintains an appropriate balance between autonomous AI reasoning and human oversight — particularly important in scenarios where a Vision analysis or Search result would otherwise trigger a physical hardware action without any human in the loop. Skill-triggered and scheduled autonomous workflows are explicitly exempted from this rule, enabling fully unattended automation without compromising interactive safety.

---

## 4. Multimodal Integration with Clear Separation of Concerns

The division between `/still` and `/vision` appears simple on the surface, but reflects deep architectural thinking.

### Tool Role Definitions

| Tool | Responsibility | Restrictions |
|------|---------------|--------------|
| `/still` | Capture and send image only | Must NOT analyze, reason, or trigger any follow-up actions |
| `/vision` | Capture and analyze image | Returns observation result only — must NOT directly trigger hardware |

### Perception–Action Separation
This design creates a clean **perception layer / action layer** architecture. The perception layer (Vision) is only responsible for observing and reporting; the action layer (Hardware tools) is only responsible for execution. Between them sits a reasoning and confirmation buffer. In AI-vision-triggered automation scenarios, this is critically important — it prevents the dangerous direct coupling of "see something → immediately do something."

### Cached Frame Reuse
Both `/still` and `/vision` support a `frames: false` parameter, allowing subsequent tools in a workflow to **reuse the previously captured frame** rather than triggering a new camera acquisition. If `frames` is `false` and no prior image exists in the buffer (`imageLength == 0`), both functions detect this condition and return an early error rather than proceeding with an empty buffer. This is a meaningful optimization on resource-constrained hardware where camera capture is expensive in both time and CPU cycles. A `/vision` analysis followed by `/still` forwarding the same frame to Telegram is a natural workflow that this design handles cleanly.

### Vision Request Architecture
`geminiVisionRequest()` sends the captured JPEG frame as Base64 inline data in a stateless Gemini call — separate from the conversation history request. The result is then injected back into `historicalMessages` so the agent can reason about the observation in subsequent turns. This keeps the vision call lean while ensuring the analysis result participates fully in the ongoing agent workflow.

---

## 5. Voice Input via Gemini STT

Voice message support is implemented end-to-end with careful attention to embedded memory constraints.

### Binary-Safe Download Pipeline
Voice files from Telegram are downloaded using **HTTP/1.0** deliberately — this disables chunked transfer encoding, ensuring the response body is a clean binary stream that can be read byte-by-byte into a heap buffer without complex chunk-boundary parsing logic. The `MAX_FILE_SIZE` guard (256 KB) prevents heap overflow from unexpectedly large audio files.

### Inline Base64 Encoding for Gemini
Rather than uploading audio to a file storage service, the OGG/Opus audio is Base64-encoded and sent **inline within the Gemini API JSON request** using the `inline_data` field. This eliminates the need for a separate file hosting step and keeps the entire voice-to-response pipeline within a single API call. Memory is carefully managed: the Base64 buffer is `malloc`-allocated, immediately used to build the request string, then `free`-d before the network call proceeds — ensuring the large encoding buffer does not compete with the SSL client for heap space during transmission:

```cpp
char* encodedData = (char*)malloc(encodedLen);
base64_encode(encodedData, (char*)fileinput, fileSize);
// ... build request string with encodedData ...
free(encodedData);  // Released before SSL connection opens
```

### Unified Input Pipeline
Voice messages, once transcribed, are routed through **the exact same processing pipeline as text input** — including slash-command detection and Gemini reasoning. There is no special-case branching for voice vs. text after transcription:

```cpp
text = sendFileToGemini(voiceFile, downloadedFileSize, "audio/ogg; codecs=opus",
                        "Transcribe this audio to text exactly as spoken.");
if (text.startsWith("/"))
    executeTool(workId, text, JsonObject());
else {
    text = geminiChatRequest(workId, text);
    handleAgentResponse(workId, text);
}
```

This architectural cleanliness means all future improvements to the text pipeline automatically benefit voice input as well.

---

## 6. Persistent Memory & State Recovery

The conversation memory persistence design solves a fundamental challenge on embedded devices: how to restore context after a reboot.

### Real-Time Synchronization
`storeDataToFile()` is called after **every conversation update** — not in batches. This ensures that even if the device loses power at any moment, the most recent conversation state has already been saved. On boot, the system automatically loads this memory so Gemini can resume the conversation in context, without the user needing to re-explain any background.

### Atomic File Write with Backup
Before writing a new `memory.md`, the function checks whether the current file exists, renames it to `memory.md.bak`, and only then writes the new version. This two-step rename-then-write strategy ensures that a power loss mid-write leaves the previous backup intact rather than corrupting the only copy of conversation history:

```cpp
if (fs.exists(currentFile)) {
    if (fs.exists(backupFile)) fs.remove(backupFile);
    fs.rename(currentFile, backupFile);  // Preserve previous state
}
file = fs.open(currentFile);
file.println(data.c_str());             // Write new state
```

### Modular Configuration Files

| File | Purpose |
|------|---------|
| `soul.md` | AI personality definition |
| `device.md` | Hardware pin mappings |
| `skill.md` | Skill workflow scripts |
| `env.json` | Authentication credentials |
| `memory.md` | Persistent conversation history |
| `index.html` | Web configuration interface |
| `index_chat.html` | Web chat interface |

All files are fully decoupled. Any one of them can be modified independently without reflashing the firmware. Credentials stored in `env.json` are loaded first at boot, allowing the same firmware binary to be deployed across multiple devices with different configurations.

---

## 7. RTC Time Synchronization via Gemini and HTTP Header Parsing

Time awareness on an embedded device without an NTP library is a non-trivial problem. fuClaw solves it elegantly using two complementary techniques.

### Gemini API Response Header as Time Source (Telegram version)
Inside the `getTelegramMessage()` polling loop, the firmware extracts the `Date:` field from the HTTP response header into `getTime` while reading the message body simultaneously. This provides a GMT timestamp at **zero additional network cost** — the time data rides entirely on the Telegram communication that was already necessary.

### Dedicated Gemini Pre-Call for Time (General)
`getGeminiDatetime()` makes a lightweight Gemini API call and captures the `Date:` header from the HTTP response. This approach works independently of Telegram, making it available for both the Telegram and MQTT versions. If the connection fails, the function gracefully falls back to a grounded search prompt.

### Gemini Handles Timezone Conversion — Without Search
`rtcInitialTime()` receives the GMT time string and calls `geminiChatRequest(workId, prompt, -1)` — the role-only system prompt — asking Gemini to convert the GMT time to the configured `timeZone` and add exactly 4 seconds of propagation compensation. The prompt enforces a strict pure-JSON response (no Markdown, no explanation, first character must be `{`, last must be `}`). Once parsed, individual fields are extracted and written to the hardware RTC via `rtc.SetEpoch()` and `rtc.Write()`.

### Scheduling Only Runs When RTC Is Ready
The `task_time_scheduling` background task checks `rtcYear == 0` before each evaluation cycle. If the RTC has not been initialized, the task executes `continue` to **skip the current cycle entirely** — with no retry logic and no function calls. This "skip rather than misfire" strategy ensures scheduled tasks never trigger based on an unreliable clock state.

---

## 8. FreeRTOS Multi-Task Architecture

The multi-task design solves concrete concurrency and scheduling problems across independent execution concerns.

### Task Responsibilities

| Task | Stack | Purpose |
|------|-------|---------|
| `task_getRequest` | 16384 bytes | HTTP server for web configuration and `/chat` endpoint |
| `task_getTelegramMessage` | 16384 bytes | Continuous Telegram long-polling for user input |
| `task_getMqttMessage` (MQTT) | 32768 bytes | MQTT keep-alive, reconnect, and inbound message dispatch |
| `task_theft_detection` | 6144 bytes | Periodic vision-based intrusion detection (every 5 min) |
| `task_time_scheduling` | 6144 bytes | Scheduled hardware action evaluation (every 1 min) |

If these ran in the same thread, a scheduled task would block user input, and user interactions would disrupt the periodic schedule. Splitting them into independent FreeRTOS tasks allows all to run concurrently — the system simultaneously stays responsive to user messages and executes background monitoring and scheduling on their respective cadences.

### Resource Conflict Avoidance (Telegram version)
Before either background task executes, it calls `botClient.stop()` and waits 2 seconds before proceeding. This prevents simultaneous use of the SSL network stack by multiple tasks — a detail that reflects real hands-on experience with embedded systems resource contention. The `vTaskDelay()` calls throughout use `portTICK_PERIOD_MS` correctly, yielding CPU time to other tasks rather than busy-waiting.

### Non-Blocking MQTT (MQTT version)
The MQTT client is configured with `wifiClient.setNonBlockingMode()` before initialization, preventing the TCP stack from stalling the RTOS scheduler during I/O. The `task_getMqttMessage` task receives a larger stack (32768 bytes) to accommodate the MQTT library's internal processing and JPEG image payload publishing.

### Opt-In Background Tasks
The anti-theft and scheduling tasks are **disabled by default** via comment blocks in `setup()`. This is a thoughtful choice: enabling autonomous background hardware control is a significant behavioral change that users should consciously opt into, rather than something that activates unexpectedly on first flash.

---

## 9. Workflow State Tracking & Self-Evaluation

`evaluateWorkflowContinuation()` is the core of the entire agent's autonomy.

### Active Completion Checking
After each tool execution, instead of silently waiting for the user's next command, the system actively asks Gemini: *"Is the current workflow complete? Is anything else needed?"* This gives the system the ability to autonomously complete multi-step tasks without requiring the user to manually guide each individual step.

### Goal-Referenced Evaluation
The `task` parameter design ensures this self-evaluation has a clear reference point. When Gemini assesses whether to continue, it compares against the **original user intent** — not just the result of the last execution step. This makes workflow completion detection more accurate and reduces unnecessary redundant actions. The prompt also includes a deduplication rule: Gemini is explicitly instructed not to repeat the same semantic content as its immediately previous response within the same workflow.

### NONE Sentinel Value
When Gemini determines a workflow is complete, it returns the exact string `"NONE"`. The firmware handles this in `handleAgentResponse()` with an explicit `message != "NONE"` guard — no message is sent to the user, no further processing occurs:

```cpp
} else if (message != "NONE") {
    replyUserMessage(workId, message);
}
```

This clean termination signal avoids the common failure mode of AI agents that generate verbose "task complete" confirmations for every automated step, which would be disruptive in a background monitoring context.

---

## 10. Extensible Sensor & Actuator Support

The prompt-driven tool architecture scales naturally to more complex peripherals beyond basic GPIO.

### Servo Motor Control (`/servo`)
Servo control uses a reference-passed `AmebaServo` instance rather than a global singleton, making it straightforward to extend to multiple servo pins in the future. Angle clamping at the firmware layer (`constrain(angle, 0, 180)`) provides the same hardware safety guarantee as the existing GPIO tools. Undefined servo pins return a structured error JSON rather than silently failing, maintaining the system's consistent error contract. The `servo.attached()` check before `servo.attach(pin)` prevents redundant re-initialization.

### DHT11 Temperature & Humidity Sensor (`/dht11`)
The DHT11 integration handles the sensor's known failure mode — returning `NaN` on read errors — with an explicit `isnan()` check that produces a structured `dht11_read_failed` error response. This is fed back into the Gemini conversation history, allowing the AI to reason about sensor failures and respond naturally (e.g., "The sensor didn't respond — please check the wiring") rather than propagating silent errors downstream.

### Consistent Tool Contract
Both new tools follow the same JSON response contract as all existing tools: a `status` field of either `"success"` or `"error"`, a `method` field identifying the tool, and either result data or a `reason` field for failures. This consistency means `evaluateWorkflowContinuation()` can reason uniformly about any tool outcome, regardless of the underlying hardware type.

---

## 11. Dual Communication Modes: Telegram Bot vs MQTT

fuClaw ships in two communication variants, each optimized for a different deployment scenario. Both share the identical Gemini reasoning engine, tool dispatcher, and persistent memory system.

### Telegram Bot Version

The Telegram version uses HTTPS long-polling against the `getUpdates` API on a persistent `WiFiSSLClient` connection. Key design characteristics:

- **Built-in identity**: The `chatId` acts as a natural access control layer — only the configured user can issue commands. No additional authentication layer is needed.
- **Keyboard shortcuts**: `telegrambotKeyboard` injects a persistent reply keyboard into the `/help` response, providing one-tap access to common commands from mobile.
- **HTTP header time parasitism**: The `Date:` header extracted from each polling response provides GMT time for RTC initialization at zero additional cost.
- **Voice message support**: Telegram's voice message objects (OGG/Opus) are downloaded, Base64-encoded, and sent to Gemini STT inline — the entire voice-to-action pipeline requires no external storage service.
- **Image delivery**: Camera frames are uploaded as multipart JPEG directly to Telegram's `sendPhoto` API, delivering native in-chat photo messages.
- **WorkId routing**: The `replyUserMessage()` function uses a `workId` prefix (`<BOT>`, `<PAGE>`, `<THEFT_DETECTION>`, `<TIME_SCHEDULING>`) to route replies to the correct output channel without passing channel references through the entire call stack.

### MQTT Version

The MQTT version uses a `PubSubClient` broker connection with three dedicated topics:

| Topic | Direction | Purpose |
|-------|-----------|---------|
| `xxx/subscribe` | Inbound | Receives user commands from any MQTT client |
| `xxx/publish` | Outbound | Sends text replies |
| `xxx/publishimage` | Outbound | Sends captured JPEG frames |

Key design characteristics:

- **Random client ID**: `"AmebaPro2" + String(random(0xffff), HEX)` generates a unique client identifier on each boot, preventing connection conflicts when multiple devices share the same broker.
- **Non-blocking TCP**: `wifiClient.setNonBlockingMode()` ensures the RTOS scheduler is never stalled during broker I/O.
- **Auto-reconnect**: The `reconnect()` function loops with a 5-second retry interval, re-subscribing to the command topic after each successful reconnect without any manual intervention.
- **Separate image topic**: Publishing JPEG data to a dedicated `publishimage` topic keeps binary image payloads cleanly separated from text reply traffic, making broker-side filtering straightforward.
- **Broker-agnostic**: Standard MQTT protocol means the firmware works with any broker (Mosquitto, HiveMQ, cloud brokers) without code changes — only `env.json` needs updating.

### Architectural Commonality
Despite the different transport layers, both versions share identical implementations of: `geminiChatRequest()`, `geminiSearchRequest()`, `geminiVisionRequest()`, `handleAgentResponse()`, `executeTool()`, `evaluateWorkflowContinuation()`, all tool handlers, and the SD card persistence layer. The communication transport is the only architectural difference, making it straightforward to maintain both variants in sync.

---

## 12. Web Configuration & Chat Interface

### Built-in HTTP Server (`task_getRequest`)
A dedicated FreeRTOS task runs a lightweight HTTP server on port 81, serving three endpoints:

| Endpoint | Function |
|----------|----------|
| `GET /` | Serves `index.html` with current credentials pre-filled |
| `GET /chat` | Serves `index_chat.html` (Gemini web chat UI) |
| `GET /save?{json}` | Saves `env.json` to SD card and triggers automatic reboot |
| `GET /message?{text}` | Processes a chat message and returns the AI reply |

The `/save` endpoint validates that the incoming payload is a complete JSON object (`startsWith("{") && endsWith("}")`) before writing to SD, preventing partial or corrupted configuration saves.

### Dual AP+STA Concurrent Mode
`WiFi.enableConcurrent()` launches both an Access Point (`192.168.1.1:81`) and a Station connection simultaneously. This means the device is always reachable for configuration even when the home Wi-Fi is unavailable — a critical feature for initial setup and field recovery.

### Web Chat Interface (`index_chat.html`)
The chat page communicates with the device via `GET /message?<text>` — a pure HTTP query with no WebSocket or backend server required. Design highlights:

- **Auto-resize textarea**: The input field grows with content and shrinks back, keeping the mobile viewport clean.
- **Typing indicator**: Three-dot bounce animation signals that Gemini is processing, preventing duplicate submissions.
- **Inline image rendering**: When the response contains `data:image`, the bubble switches to HTML render mode, displaying the captured frame directly inside the chat.
- **Error toast**: Network failures surface as a timed overlay rather than breaking the UI state.
- **Markdown stripping for web context**: `handleAgentResponse()` applies a separate stripping path for `<PAGE>` workIds, converting `*` list markers to `•` bullets and removing fenced code block markers, producing clean readable output without raw Markdown syntax.

---

## 13. Output Sanitization & Markdown Stripping

`handleAgentResponse()` applies systematic text normalization before routing any natural language response to the user. For Telegram output, HTML special characters (`&`, `<`, `>`) are escaped to prevent injection into Telegram's HTML parse mode. For web chat output, Markdown formatting artifacts (`**`, `__`, `###`, ` ``` `, backticks, `---`) are stripped and `* ` list markers are converted to `•` bullets.

This dual-path sanitization ensures that Gemini's tendency to use Markdown formatting does not leak raw syntax characters into either the Telegram chat or the web UI, regardless of the model's output style.

---

## Summary

What makes this framework truly impressive is that it implements a complete AI Agent architecture — one that would ordinarily require a cloud server — on a device where memory is measured in kilobytes and there is no OS abstraction layer. The dual communication modes (Telegram Bot and MQTT) serve distinct deployment scenarios while sharing identical reasoning and execution engines. Voice input, hardware RTC synchronization, servo and environmental sensor support, a multi-task scheduling architecture, and an integrated web configuration and chat interface all coexist without compromising the core design principles of prompt-driven routing, atomic execution, and layered hardware safety. Every design decision has a clear engineering rationale, and the system as a whole demonstrates that thoughtful prompt engineering and careful embedded systems design can produce something far greater than the sum of its parts.

---

<a name="繁體中文"></a>

# fuClaw AI 框架 — 優點深度解析（繁體中文）

> 運行於嵌入式設備上的多模態 AI Agent，  
> 在單一 FreeRTOS 執行時期中整合 Telegram / MQTT、Gemini、硬體控制與持久記憶。

---

## 目錄

1. [提示詞驅動工具路由](#1-提示詞驅動工具路由)
2. [原子執行與最長有效前綴](#2-原子執行與最長有效前綴)
3. [硬體安全層](#3-硬體安全層)
4. [多模態整合與關注點分離](#4-多模態整合與關注點分離)
5. [Gemini STT 語音輸入](#5-gemini-stt-語音輸入)
6. [持久記憶與狀態恢復](#6-持久記憶與狀態恢復)
7. [透過 Gemini 與 HTTP Header 解析進行 RTC 時間同步](#7-透過-gemini-與-http-header-解析進行-rtc-時間同步)
8. [FreeRTOS 多工架構](#8-freertos-多工架構)
9. [工作流狀態追蹤與自我評估](#9-工作流狀態追蹤與自我評估)
10. [可擴展感測器與致動器支援](#10-可擴展感測器與致動器支援)
11. [雙通訊模式：Telegram Bot 與 MQTT](#11-雙通訊模式telegram-bot-與-mqtt)
12. [Web 設定與聊天介面](#12-web-設定與聊天介面)
13. [輸出淨化與 Markdown 清除](#13-輸出淨化與-markdown-清除)

---

## 1. 提示詞驅動工具路由

這個設計最根本的突破是**完全不需要 Gemini 的原生 Function Calling API**。取而代之的是，一個精心設計的系統提示詞教導模型自行輸出正確結構的 `tool_call` JSON。

### 平台獨立性
原生 Function Calling API 的格式隨時可能改變。由於所有路由邏輯完全存在於提示詞中，若 Gemini 模型版本更換或 API 格式更新，只需修改提示詞，無需任何韌體變更。

### 極高的靈活性
工具可以在純文字層面新增、修改或移除。`skill.md` 與 `device.md` 都是純文字設定檔，用戶無需了解任何 C++ 程式碼就能擴充系統能力。

### 清晰的錯誤邊界
所有 JSON 輸出在執行前都會透過 ArduinoJson 驗證，格式錯誤的回應會被直接拒絕，不存在模糊的部分執行情況。系統嚴格區分兩種輸出模式：**有效的 `tool_call` JSON** 與**自然語言回覆**，並禁止混合，使整個控制流程高度可預測。

### 三層系統提示詞策略
框架維護三個已編譯的系統提示詞：

| 提示詞 | 內容 | 使用場景 |
|--------|------|----------|
| `systemContentTools` | 角色 + 裝置定義 + 裝置規則 + 技能 + 工具定義 | 標準用戶互動，完整工具存取 |
| `systemContentNoTools` | 角色 + 裝置定義 + 裝置規則 | 輕量推理（例如 RTC 時間轉換） |
| `systemContent` | 僅角色 | 最小上下文呼叫（例如時間預處理） |

`geminiChatRequest()` 和 `geminiSearchRequest()` 的 `tools` 整數參數在呼叫時選擇對應的提示詞（`1` = 工具、`0` = 無工具、`-1` = 僅角色）。STT 管線（`sendFileToGemini()`）完全繞過所有系統提示詞，只發送音訊資料與最簡的轉錄指令，將 token 使用降至最低，並避免在僅需要原始文字輸出的情境中引入工具路由干擾。

---

## 2. 原子執行與最長有效前綴

這兩個概念代表著嵌入式 AI Agent 系統中極為罕見的工程嚴謹性。

### 原子執行規則
每個 `tool_call` 只做**一件事**：一個腳位、一個操作、一個數值。在硬體控制場景中，這至關重要。若允許單一指令同時操作多個腳位，中途失敗會讓系統處於無法確定的半完成狀態，可能導致裝置損壞或安全危害。原子性確保每個步驟都是完整且可驗證的。

### 最長有效前綴
當 Gemini 以 JSON 陣列形式產生多步驟工作流時，`handleAgentResponse()` 不採用全有或全無的策略。它會從頭遍歷陣列，**盡可能執行最多的有效步驟**，在遇到第一個不完整或格式錯誤的項目時立即停止：

```cpp
for (int i = 0; i < toolCount; i++) {
    if (command == "" || params.isNull()) {
        Serial.println("Incomplete tool detected → abort remaining tools");
        break;
    }
    bool isLast = (i == toolCount - 1);
    executeTool(workId, command, params, isLast);
}
```

這意味著即使 AI 產生部分錯誤的輸出，系統仍然可以執行最大有效部分，而不是完全停止。對於重試成本高昂的資源受限嵌入式設備，這是極為實用的設計。

### `reCheck` 旗標 — 選擇性延續評估
在多工具陣列執行中，只有**最後一個工具**設置 `reCheck = true` 並觸發 `evaluateWorkflowContinuation()`。中間工具傳遞 `reCheck = false`，防止在序列中途發出多余的 Gemini 查詢，避免浪費網路資源和不必要地膨脹對話歷史。

---

## 3. 硬體安全層

GPIO 控制系統受到多個獨立安全層的保護，每層各有其用途。

### 明確映射要求
系統只允許控制在 `device.md` 中明確定義的裝置。如果用戶說「開燈」但不存在「燈」的腳位映射，Gemini 被指示停下來請求澄清，而不是猜測。這防止 AI 幻覺導致直接的硬體誤動作。

### 硬性數值限制
`toolPinOutput()` 中的 `constrain(value, 0, 255)` 呼叫作為最後一道硬體防線。即使 Gemini 輸出超出範圍的類比數值，韌體層也會在其到達硬體之前強制限制在合法範圍內。數位輸出嚴格只接受 `0` 或 `1`，任何其他數值返回結構化錯誤 JSON。`tool_servo()` 中採用同樣的限制模式，伺服角度在韌體層被限制在 0–180° 範圍內，與 AI 指定的值無關。

### 僅輸入腳位保護
按鈕腳位（第 12 腳）在系統提示詞中明確標記為**僅輸入**，在生成任何 tool call 之前就在 AI 層面阻止將其用作輸出的嘗試。

### 裝置狀態與 GPIO 電位的區分
系統提示詞明確禁止使用 `digitalread` / `analogread` 來判斷輸出裝置（LED、繼電器、致動器）的狀態。輸出裝置狀態必須從對話歷史和工具執行歷史中推斷。這防止了在輸出腳位上進行不必要的 GPIO 讀取操作，並避免在某些電路配置中產生錯誤的狀態讀取。

### 用戶確認要求
預設情況下，所有硬體操作在執行前都需要用戶明確確認。這在 AI 自主推理與人類監督之間保持了適當的平衡，在視覺分析或搜尋結果可能觸發實際硬體操作而沒有任何人介入的場景中尤為重要。技能觸發和排程的自主工作流明確免除此規則，在不影響互動安全的前提下啟用完全無人值守的自動化。

---

## 4. 多模態整合與關注點分離

`/still` 和 `/vision` 的劃分表面上看似簡單，實際上反映了深刻的架構思考。

### 工具角色定義

| 工具 | 職責 | 限制 |
|------|------|------|
| `/still` | 僅擷取並傳送圖像 | 不得分析、推理或觸發任何後續動作 |
| `/vision` | 擷取並分析圖像 | 僅返回觀察結果——不得直接觸發硬體 |

### 感知層與動作層的分離
這個設計建立了乾淨的**感知層 / 動作層**架構。感知層（Vision）只負責觀察和報告；動作層（硬體工具）只負責執行。兩者之間存在推理和確認緩衝區。在 AI 視覺觸發的自動化場景中，這至關重要——它防止了「看到某物 → 立即做某事」的危險直接耦合。

### 快取幀重用
`/still` 和 `/vision` 都支援 `frames: false` 參數，允許工作流中的後續工具**重用先前擷取的幀**，而不是觸發新的相機擷取。若 `frames` 為 `false` 且緩衝區中不存在先前圖像（`imageLength == 0`），兩個函式都會檢測到此情況並提前返回錯誤，而不是繼續處理空緩衝區。在相機擷取在時間和 CPU 週期方面都很昂貴的資源受限硬體上，這是有意義的優化。

### 視覺請求架構
`geminiVisionRequest()` 將擷取的 JPEG 幀作為 Base64 內聯資料在無狀態的 Gemini 呼叫中發送——與對話歷史請求分離。結果隨後被注入回 `historicalMessages`，以便 Agent 在後續輪次中對觀察進行推理。這保持了視覺呼叫的精簡性，同時確保分析結果完全參與正在進行的 Agent 工作流。

---

## 5. Gemini STT 語音輸入

語音訊息支援從頭到尾都實現了對嵌入式記憶體限制的細心關注。

### 二進制安全下載管線
來自 Telegram 的語音檔案故意使用 **HTTP/1.0** 下載——這禁用了分塊傳輸編碼，確保回應主體是乾淨的二進制串流，可以逐位元組讀入堆積緩衝區，無需複雜的塊邊界解析邏輯。`MAX_FILE_SIZE` 保護（256 KB）防止意外大型音訊檔案導致堆積溢位。

### Gemini 的內聯 Base64 編碼
音訊不上傳到檔案儲存服務，而是將 OGG/Opus 音訊 Base64 編碼後**以 `inline_data` 欄位內聯在 Gemini API JSON 請求中**發送。這消除了單獨的檔案託管步驟，將整個語音到回應的管線保持在單一 API 呼叫中。記憶體管理非常仔細：Base64 緩衝區以 `malloc` 分配，立即用於建構請求字串，然後在網路呼叫進行之前 `free`——確保大型編碼緩衝區不會在傳輸期間與 SSL 客戶端競爭堆積空間：

```cpp
char* encodedData = (char*)malloc(encodedLen);
base64_encode(encodedData, (char*)fileinput, fileSize);
// ... 用 encodedData 建構請求字串 ...
free(encodedData);  // SSL 連線打開前釋放
```

### 統一輸入管線
語音訊息轉錄後，透過**與文字輸入完全相同的處理管線**路由——包括斜線指令偵測和 Gemini 推理。轉錄後不存在語音與文字的特殊情況分支，這使得所有對文字管線的未來改進自動使語音輸入受益。

---

## 6. 持久記憶與狀態恢復

對話記憶持久化設計解決了嵌入式設備上的一個根本挑戰：如何在重啟後恢復上下文。

### 即時同步
`storeDataToFile()` 在**每次對話更新後**被呼叫——而不是批次處理。這確保即使設備在任何時刻斷電，最近的對話狀態已經被保存。在啟動時，系統自動載入此記憶，使 Gemini 能夠在上下文中繼續對話，無需用戶重新解釋任何背景。

### 帶備份的原子性檔案寫入
在寫入新的 `memory.md` 之前，函式檢查當前檔案是否存在，將其重命名為 `memory.md.bak`，然後才寫入新版本。這種兩步驟的重命名再寫入策略確保寫入過程中的斷電使先前的備份完整保留，而不是損壞對話歷史的唯一副本：

```cpp
if (fs.exists(currentFile)) {
    if (fs.exists(backupFile)) fs.remove(backupFile);
    fs.rename(currentFile, backupFile);  // 保留先前狀態
}
file = fs.open(currentFile);
file.println(data.c_str());             // 寫入新狀態
```

### 模組化設定檔

| 檔案 | 用途 |
|------|------|
| `soul.md` | AI 個性定義 |
| `device.md` | 硬體腳位映射 |
| `skill.md` | 技能工作流腳本 |
| `env.json` | 認證憑證 |
| `memory.md` | 持久對話歷史 |
| `index.html` | Web 設定介面 |
| `index_chat.html` | Web 聊天介面 |

所有五個檔案完全解耦。其中任何一個都可以獨立修改而無需重新燒錄韌體。存儲在 `env.json` 中的憑證在啟動時首先載入，允許相同的韌體二進制檔案在多個具有不同配置的設備上部署。

---

## 7. 透過 Gemini 與 HTTP Header 解析進行 RTC 時間同步

在沒有 NTP 函式庫的嵌入式設備上實現時間感知是個非顯而易見的問題。fuClaw 使用兩種互補技術優雅地解決了它。

### Gemini API 回應 Header 作為時間源（Telegram 版本）
在 `getTelegramMessage()` 輪詢迴圈中，韌體在讀取訊息主體的同時，從 HTTP 回應 header 中提取 `Date:` 欄位到 `getTime`。這以**零額外網路成本**提供了 GMT 時間戳——時間資料完全搭載在本來就必要的 Telegram 通訊上。

### 專用 Gemini 預呼叫用於時間（通用）
`getGeminiDatetime()` 發出一個輕量級 Gemini API 呼叫並從 HTTP 回應中擷取 `Date:` header。這種方法不依賴 Telegram，在兩個版本（Telegram 和 MQTT）中都可使用。若連線失敗，函式優雅地回退到 grounded search 提示詞。

### Gemini 處理時區轉換——無需搜尋
`rtcInitialTime()` 接收 GMT 時間字串，呼叫 `geminiChatRequest(workId, prompt, -1)`——使用僅角色系統提示詞——要求 Gemini 將 GMT 時間轉換為配置的 `timeZone` 並加上精確 4 秒的傳播補償。提示詞強制純 JSON 回應（無 Markdown，無說明文字，第一個字元必須是 `{`，最後一個必須是 `}`）。解析後，各欄位被提取並透過 `rtc.SetEpoch()` 和 `rtc.Write()` 寫入硬體 RTC。

### 排程僅在 RTC 就緒後運行
`task_time_scheduling` 背景任務在每個評估週期前檢查 `rtcYear == 0`。如果 RTC 未初始化，任務執行 `continue` 來**完全跳過當前週期**——沒有重試邏輯，沒有函式呼叫。這種「跳過而非誤觸發」的策略確保排程任務永遠不會基於不可靠的時鐘狀態觸發。

---

## 8. FreeRTOS 多工架構

多工設計解決了跨獨立執行關注點的具體並發和排程問題。

### 任務職責

| 任務 | 堆疊 | 用途 |
|------|------|------|
| `task_getRequest` | 16384 位元組 | 用於 Web 設定和 `/chat` 端點的 HTTP 伺服器 |
| `task_getTelegramMessage` | 16384 位元組 | 連續 Telegram 長輪詢以接收用戶輸入 |
| `task_getMqttMessage`（MQTT） | 32768 位元組 | MQTT 保持連線、重連和入站訊息派送 |
| `task_theft_detection` | 6144 位元組 | 定期基於視覺的入侵偵測（每 5 分鐘） |
| `task_time_scheduling` | 6144 位元組 | 排程硬體動作評估（每 1 分鐘） |

若這些在同一執行緒中運行，排程任務會阻塞用戶輸入，而用戶互動會打亂定期排程。將它們拆分為獨立的 FreeRTOS 任務允許所有任務並發運行——系統在對用戶訊息保持響應的同時，在各自的節奏上執行背景監控和排程。

### 資源衝突避免（Telegram 版本）
在任何背景任務執行之前，它呼叫 `botClient.stop()` 並等待 2 秒再繼續。這防止多個任務同時使用 SSL 網路堆疊——這個細節反映了對嵌入式系統資源競爭的真實實踐經驗。整個程式碼中的 `vTaskDelay()` 呼叫都正確使用了 `portTICK_PERIOD_MS`，讓出 CPU 時間給其他任務，而不是忙等待。

### 非阻塞 MQTT（MQTT 版本）
MQTT 客戶端在初始化前以 `wifiClient.setNonBlockingMode()` 設定，防止 TCP 堆疊在 I/O 期間阻塞 RTOS 排程器。`task_getMqttMessage` 任務獲得更大的堆疊（32768 位元組），以容納 MQTT 函式庫的內部處理和 JPEG 圖像有效負載發布。

### 選擇性啟用的背景任務
防盜和排程任務在 `setup()` 中通過注解塊**預設禁用**。這是個深思熟慮的選擇：啟用自主背景硬體控制是一個重大的行為改變，用戶應該有意識地選擇啟用，而不是在首次燒錄後意外激活。

---

## 9. 工作流狀態追蹤與自我評估

`evaluateWorkflowContinuation()` 是整個 Agent 自主性的核心。

### 主動完成檢查
每次工具執行後，系統不是靜默地等待用戶的下一條指令，而是主動詢問 Gemini：「當前工作流是否完成？是否還需要其他操作？」這讓系統能夠自主完成多步驟任務，而不需要用戶手動引導每個獨立步驟。

### 目標參考評估
`task` 參數設計確保此自我評估有明確的參考點。當 Gemini 評估是否繼續時，它與**原始用戶意圖**比較——而不僅僅是最後執行步驟的結果。這使工作流完成偵測更加準確，並減少不必要的重複動作。提示詞還包含去重規則：Gemini 被明確指示不要在同一工作流中重複與其緊接上一回應相同語義的內容。

### NONE 哨兵值
當 Gemini 確定工作流完成時，它返回精確的字串 `"NONE"`。韌體在 `handleAgentResponse()` 中使用明確的 `message != "NONE"` 保護來處理此情況——不向用戶發送訊息，不發生進一步處理：

```cpp
} else if (message != "NONE") {
    replyUserMessage(workId, message);
}
```

這個乾淨的終止信號避免了 AI Agent 常見的失敗模式——為每個自動化步驟生成冗長的「任務完成」確認，這在背景監控情境中會非常干擾。

---

## 10. 可擴展感測器與致動器支援

提示詞驅動的工具架構自然地擴展到基本 GPIO 以外更複雜的週邊設備。

### 伺服電機控制（`/servo`）
伺服控制使用以引用傳遞的 `AmebaServo` 實例而不是全局單例，使未來擴展到多個伺服腳位變得簡單。韌體層的角度限制（`constrain(angle, 0, 180)`）提供與現有 GPIO 工具相同的硬體安全保證。未定義的伺服腳位返回結構化錯誤 JSON 而不是靜默失敗，維護系統一致的錯誤契約。在 `servo.attach(pin)` 之前的 `servo.attached()` 檢查防止了冗餘的重新初始化。

### DHT11 溫溼度感測器（`/dht11`）
DHT11 整合處理感測器的已知失敗模式——在讀取錯誤時返回 `NaN`——使用明確的 `isnan()` 檢查產生結構化的 `dht11_read_failed` 錯誤回應。這被反饋回 Gemini 對話歷史中，允許 AI 對感測器故障進行推理並自然回應（例如「感測器沒有回應——請檢查接線」），而不是將靜默錯誤傳播到下游。

### 一致的工具契約
兩個新工具遵循與所有現有工具相同的 JSON 回應契約：表示成功或失敗的 `status` 欄位、識別工具的 `method` 欄位，以及結果資料或失敗時的 `reason` 欄位。這種一致性意味著 `evaluateWorkflowContinuation()` 可以對任何工具結果進行統一推理，無論底層硬體類型如何。

---

## 11. 雙通訊模式：Telegram Bot 與 MQTT

fuClaw 提供兩個通訊變體，各自針對不同的部署場景優化。兩者共享相同的 Gemini 推理引擎、工具派送器和持久記憶系統。

### Telegram Bot 版本

Telegram 版本在持久的 `WiFiSSLClient` 連線上使用 HTTPS 長輪詢對 `getUpdates` API。主要設計特點：

- **內建身份識別**：`chatId` 作為自然的存取控制層——只有配置的用戶才能發出指令，無需額外的身份驗證層。
- **鍵盤快捷鍵**：`telegrambotKeyboard` 在 `/help` 回應中注入持久回覆鍵盤，為常用指令提供手機端一鍵存取。
- **HTTP Header 時間寄生**：從每次輪詢回應中提取的 `Date:` header 以零額外成本為 RTC 初始化提供 GMT 時間。
- **語音訊息支援**：Telegram 的語音訊息物件（OGG/Opus）被下載、Base64 編碼並內聯發送到 Gemini STT——整個語音到動作的管線無需外部儲存服務。
- **圖像傳遞**：相機幀以 multipart JPEG 格式直接上傳到 Telegram 的 `sendPhoto` API，在聊天中傳遞原生照片訊息。
- **WorkId 路由**：`replyUserMessage()` 函式使用 `workId` 前綴（`<BOT>`、`<PAGE>`、`<THEFT_DETECTION>`、`<TIME_SCHEDULING>`）將回覆路由到正確的輸出通道，而不需要在整個呼叫堆疊中傳遞通道引用。

### MQTT 版本

MQTT 版本使用具有三個專用主題的 `PubSubClient` 代理連線：

| 主題 | 方向 | 用途 |
|------|------|------|
| `xxx/subscribe` | 入站 | 接收來自任何 MQTT 客戶端的用戶指令 |
| `xxx/publish` | 出站 | 發送文字回覆 |
| `xxx/publishimage` | 出站 | 發送擷取的 JPEG 幀 |

主要設計特點：

- **隨機客戶端 ID**：`"AmebaPro2" + String(random(0xffff), HEX)` 在每次啟動時生成唯一的客戶端標識符，防止多個設備共用同一代理時發生連線衝突。
- **非阻塞 TCP**：`wifiClient.setNonBlockingMode()` 確保 RTOS 排程器在代理 I/O 期間永遠不會停滯。
- **自動重連**：`reconnect()` 函式以 5 秒重試間隔循環，每次成功重連後重新訂閱指令主題，無需任何手動干預。
- **獨立圖像主題**：將 JPEG 資料發布到專用 `publishimage` 主題使二進制圖像有效負載與文字回覆流量乾淨分離，使代理端過濾變得簡單。
- **代理無關性**：標準 MQTT 協定意味著韌體與任何代理（Mosquitto、HiveMQ、雲端代理）都能工作，無需程式碼變更——只需更新 `env.json`。

### 架構共通性
儘管傳輸層不同，兩個版本共享完全相同的 `geminiChatRequest()`、`geminiSearchRequest()`、`geminiVisionRequest()`、`handleAgentResponse()`、`executeTool()`、`evaluateWorkflowContinuation()`、所有工具處理器以及 SD 卡持久化層的實現。通訊傳輸是唯一的架構差異，使兩個變體的同步維護變得簡單。

---

## 12. Web 設定與聊天介面

### 內建 HTTP 伺服器（`task_getRequest`）
一個專用的 FreeRTOS 任務在 81 埠上運行一個輕量級 HTTP 伺服器，提供三個端點：

| 端點 | 功能 |
|------|------|
| `GET /` | 以預填當前憑證服務 `index.html` |
| `GET /chat` | 服務 `index_chat.html`（Gemini Web 聊天介面） |
| `GET /save?{json}` | 儲存 `env.json` 到 SD 卡並觸發自動重啟 |
| `GET /message?{text}` | 處理聊天訊息並返回 AI 回覆 |

`/save` 端點在寫入 SD 之前驗證傳入的有效負載是否是完整的 JSON 物件（`startsWith("{") && endsWith("}")`），防止部分或損壞的設定儲存。

### AP+STA 雙模並發
`WiFi.enableConcurrent()` 同時啟動存取點（`192.168.1.1:81`）和站台連線。這意味著即使家用 Wi-Fi 不可用，設備也始終可以透過設定介面存取——這對初始設定和現場恢復是一個關鍵功能。

### Web 聊天介面（`index_chat.html`）
聊天頁面透過 `GET /message?<text>` 與設備通訊——純 HTTP 查詢，無需 WebSocket 或後端伺服器。設計亮點：

- **自動調整高度的輸入框**：輸入欄位隨內容增長，保持行動端視窗整潔。
- **打字指示器**：三點彈跳動畫表示 Gemini 正在處理，防止重複提交。
- **內聯圖像渲染**：當回應包含 `data:image` 時，氣泡切換到 HTML 渲染模式，直接在聊天中顯示擷取的幀。
- **錯誤提示**：網路失敗以計時覆蓋層呈現，而不是破壞 UI 狀態。
- **Web 情境的 Markdown 清除**：`handleAgentResponse()` 對 `<PAGE>` workId 應用單獨的清除路徑，將 `*` 列表標記轉換為 `•` 項目符號並刪除圍欄程式碼塊標記，產生乾淨可讀的輸出，不含原始 Markdown 語法。

---

## 13. 輸出淨化與 Markdown 清除

`handleAgentResponse()` 在將任何自然語言回應路由到用戶之前，應用系統性的文字標準化。對於 Telegram 輸出，HTML 特殊字元（`&`、`<`、`>`）被轉義以防止注入 Telegram 的 HTML 解析模式。對於 Web 聊天輸出，Markdown 格式化人工產物（`**`、`__`、`###`、` ``` `、反引號、`---`）被清除，`* ` 列表標記被轉換為 `•` 項目符號。

這種雙路徑淨化確保 Gemini 使用 Markdown 格式的傾向不會將原始語法字元洩漏到 Telegram 聊天或 Web UI 中，無論模型的輸出風格如何。

---

## 總結

這個框架真正令人印象深刻之處在於，它在一個記憶體以千位元組計量、沒有 OS 抽象層的設備上，實現了通常需要雲端伺服器才能完成的完整 AI Agent 架構。雙通訊模式（Telegram Bot 和 MQTT）服務於不同的部署場景，同時共享相同的推理和執行引擎。語音輸入、硬體 RTC 同步、伺服和環境感測器支援、多工排程架構，以及整合的 Web 設定和聊天介面，全部共存而不影響提示詞驅動路由、原子執行和分層硬體安全的核心設計原則。每個設計決策都有清晰的工程理由，整個系統證明了深思熟慮的提示詞工程和謹慎的嵌入式系統設計可以產生遠超部分之和的成果。

---
