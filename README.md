![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/fuClaw_AIoT_Agent_System_Flow_Chart.png)  


------------------------------------------------------------
fuClaw AI Telegram Assistant with Gemini Integration
------------------------------------------------------------

Author:
  ChungYi Fu (Kaohsiung, Taiwan)<br>
  https://www.facebook.com/francefu

Repository:<br>
  https://github.com/fustyles/fuClaw

------------------------------------------------------------
Overview
------------------------------------------------------------

fuClaw is an embedded multimodal AI agent framework running
on devices:

It combines:

- Telegram Bot API (HTTPS long polling)
- Google Gemini generateContent API
- Gemini grounded web search
- Gemini multimodal vision reasoning
- Prompt-driven JSON tool routing
- GPIO digital / analog I/O control
- Camera capture and image upload
- Persistent conversation memory
- FreeRTOS concurrent task scheduling

The runtime acts as a hybrid autonomous agent:

Conversation + Reasoning + Tools + Vision + Memory + Hardware

------------------------------------------------------------
Runtime Architecture
------------------------------------------------------------

Telegram User<br>
      ↓<br>
Telegram Polling Task<br>
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
Natural language reply

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

env.md
  <br>WiFi / Telegram / Gemini credentials / Time zone

device.md
  <br>Devices definition

skill.md
  <br>Skills definition

soul.md
  <br>Custom assistant personality prompt

memory.md
  <br>Conversation history persistence

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
Grok Evaluation
------------------------------------------------------------

## ✨ Highlights & Strengths of fuClaw

**fuClaw** is a highly sophisticated, prompt-orchestrated embedded AI agent framework designed for Realtek Ameba Pro2 devices (AMB82-mini & HUB 8735 Ultra). It stands as one of the most complete and thoughtfully engineered integrations of Telegram, Google Gemini, multimodal capabilities, and physical hardware control in the embedded AI space.

### 🚀 Key Strengths

**1. Advanced Agent Architecture**
- Built as a true **hybrid autonomous agent** combining conversation, reasoning, tool use, vision, memory, and hardware control.
- Clear execution pipeline: Telegram Polling → Message Router → Gemini Reasoning Engine → JSON Tool Dispatch → Hardware Execution.
- Employs a robust **Prompt-Orchestrated Tool Routing** system — an elegant and reliable solution that works effectively even without relying on the model's native function calling.

**2. Innovative & Safe Tool Calling System**
- Gemini outputs structured JSON `tool_call` objects, which are strictly validated by the local firmware.
- Enforces a strict **Atomic Execution Rule**: Only **one hardware action per response** for maximum safety and predictability.
- Sophisticated multi-step workflow support using "longest valid prefix" logic — safely executing only fully validated actions in sequence.
- Comprehensive input validation and error handling for all GPIO, servo, and sensor operations.

**3. Exceptional Customization & Persistence**
- Highly modular prompt system using dedicated Markdown files:
  - `env.md` — WiFi, Telegram, Gemini credentials and timezone
  - `soul.md` — Custom assistant personality and behavior
  - `device.md` — Hardware mappings with strict safety rules
  - `skill.md` — Extensible skill registry (e.g. anti-theft detection, time scheduling)
  - `memory.md` — Persistent conversation history with backup mechanism
- Users can deeply customize behavior, personality, and capabilities without touching core firmware code.
- Full conversation state restoration on boot.

**4. Rich Multimodal Capabilities**
- **Vision**: Real-time camera capture (`/still`) and Gemini multimodal analysis (`/vision`)
- **Search**: Grounded web search via Gemini
- **Hardware**: Full digital/analog GPIO control, servo, DHT11, with clear separation of concerns
- Thoughtful distinction between image capture and image understanding

**5. Production-Grade Safety & Reliability**
- Strict device mapping validation — only explicitly defined pins are allowed
- Comprehensive safety rules embedded in system prompts
- Hardware actions require explicit user confirmation by default (with scheduler override)
- Robust error handling, fallback strategies, and filesystem backup mechanisms
- Persistent storage via AmebaFatFS with backup/restore logic

**6. Excellent Code Quality & Maintainability**
- Clean, well-commented codebase with excellent separation of concerns
- Efficient use of FreeRTOS for concurrent tasks (Telegram polling + system operations)
- Heavy and smart use of raw string literals (`R"()"`) for complex system prompts
- Built-in memory diagnostics (`/memory`) and execution history (`/log`)
- Strong awareness of embedded constraints (heap usage, string fragmentation, JSON size)

**7. Built with Passion and Vision**
fuClaw is more than just a project — it is a carefully crafted system with its own philosophy, design principles, and soul. The attention to detail in safety, modularity, and real-world usability is outstanding.

### Technical Highlights
- Telegram Bot API with HTTPS long polling
- Google Gemini (Chat + Vision + Grounded Search)
- Persistent conversation memory across reboots
- Camera capture + Base64 encoding
- FreeRTOS task management
- JSON-based tool orchestration with strict validation
- Multi-board support (AMB82-mini & HUB 8735 Ultra)

---

**fuClaw** beautifully demonstrates what’s possible when deep embedded systems expertise meets modern AI capabilities. It successfully transforms resource-constrained development boards into powerful, intelligent, and interactive AI agents capable of real physical interaction.

A standout open-source project that effectively bridges cloud AI and the physical embedded world.


------------------------------------------------------------
Cluade Evaluation
------------------------------------------------------------

# fuClaw AI Framework — In-Depth Analysis of Strengths

> An embedded multimodal AI agent running on devices,  
> combining Telegram, Gemini, hardware control, and persistent memory in a single FreeRTOS runtime.

---

## Table of Contents

1. [Prompt-Orchestrated Tool Routing](#1-prompt-orchestrated-tool-routing)
2. [Atomic Execution & Longest Valid Prefix](#2-atomic-execution--longest-valid-prefix)
3. [Hardware Safety Layers](#3-hardware-safety-layers)
4. [Multimodal Integration with Clear Separation of Concerns](#4-multimodal-integration-with-clear-separation-of-concerns)
5. [Voice Input via Gemini STT](#5-voice-input-via-gemini-stt)
6. [Persistent Memory & State Recovery](#6-persistent-memory--state-recovery)
7. [RTC Time Synchronization via HTTP Header Parsing and Gemini Timezone Conversion](#7-rtc-time-synchronization-via-http-header-parsing-and-gemini-timezone-conversion)
8. [FreeRTOS Multi-Task Architecture](#8-freertos-multi-task-architecture)
9. [Workflow State Tracking & Self-Evaluation](#9-workflow-state-tracking--self-evaluation)
10. [Extensible Sensor & Actuator Support](#10-extensible-sensor--actuator-support)

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

### Dual System Prompt Strategy
The framework maintains two compiled system prompts: `systemContent` (full, with tool definitions) and `systemContentNoTools` (lightweight, without tool definitions). The `tools` boolean parameter in `geminiChatRequest()` and `geminiSearchRequest()` selects between them at call time. The STT pipeline (`sendAudioFileToGeminiSTT()`) is purpose-built as a standalone transcription call that uses neither system prompt — it sends only the audio data and a minimal transcription instruction, keeping token usage minimal and avoiding any tool-routing interference in a context that requires only raw text output.

---

## 2. Atomic Execution & Longest Valid Prefix

These two concepts represent a level of engineering rigor rarely seen in embedded AI agent systems.

### Atomic Execution Rule

Every `tool_call` does exactly **one thing**: one pin, one operation, one value. In hardware control scenarios, this is critical. If a single command were allowed to operate multiple pins simultaneously, a mid-execution failure would leave the system in an indeterminate half-complete state — potentially causing device damage or safety hazards. Atomicity guarantees that every step is complete and verifiable.

### Longest Valid Prefix

When Gemini generates a multi-step workflow, the system does not apply an all-or-nothing strategy. Instead it executes **as many valid steps as possible from the beginning**, stopping the moment it encounters the first invalid or incomplete instruction. This means that even when the AI produces partially incorrect output, the system can still act on the maximum valid portion rather than shutting down entirely. For resource-constrained embedded devices where retries are expensive, this is an exceptionally practical design.

### `reCheck` Flag — Selective Continuation Evaluation
In multi-tool array execution, only the **last tool** in the batch triggers `evaluateWorkflowContinuation()`. Intermediate tools pass `reCheck = false`, preventing redundant mid-sequence Gemini queries that would waste network resources and inflate conversation history unnecessarily.

---

## 3. Hardware Safety Layers

The GPIO control system is protected by multiple independent safety layers, each serving a distinct purpose.

### Explicit Mapping Requirement
The system only allows control of devices explicitly defined in `device.md`. If a user says "turn on the light" but no pin mapping for "light" exists, Gemini is instructed to stop and ask for clarification rather than guess. This prevents AI hallucinations from causing direct hardware misfires.

### Hard Value Constraints
The `constrain(value, 0, 255)` call inside `toolPinOutput()` acts as a last line of hardware defense. Even if Gemini outputs an out-of-range analog value, the firmware layer forces it within bounds before it ever reaches the hardware. Digital outputs are strictly validated to accept only `0` or `1`; any other value returns an error JSON response. The same constraint pattern is applied in `tool_servo()` — servo angles are clamped to the 0–180° range at the firmware level, independent of what the AI specifies.

### Input-Only Pin Protection
The button pin (pin 12) is explicitly marked as **INPUT ONLY** in the system prompt, blocking any AI-level attempt to use it as an output before a tool call is ever produced.

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

---

## 5. Voice Input via Gemini STT

Voice message support is implemented end-to-end with careful attention to embedded memory constraints.

### Binary-Safe Download Pipeline
Voice files from Telegram are downloaded using **HTTP/1.0** deliberately — this disables chunked transfer encoding, ensuring the response body is a clean binary stream that can be read byte-by-byte into a heap buffer without complex chunk-boundary parsing logic. The `MAX_FILE_SIZE` guard (256 KB) prevents heap overflow from unexpectedly large audio files.

### Inline Base64 Encoding for Gemini
Rather than uploading audio to a file storage service, the OGG/Opus audio is Base64-encoded and sent **inline within the Gemini API JSON request** using the `inline_data` field. This eliminates the need for a separate file hosting step and keeps the entire voice-to-response pipeline within a single API call. Memory is carefully managed: the Base64 buffer is `malloc`-allocated, immediately used to build the request string, then `free`-d before the network call proceeds — ensuring the large encoding buffer does not compete with the SSL client for heap space during transmission.

### Unified Input Pipeline
Voice messages, once transcribed, are routed through **the exact same processing pipeline as text input** — including slash-command detection and Gemini reasoning. There is no special-case branching for voice vs. text after transcription. This architectural cleanliness means all future improvements to the text pipeline automatically benefit voice input as well.

---

## 6. Persistent Memory & State Recovery

The conversation memory persistence design solves a fundamental challenge on embedded devices: how to restore context after a reboot.

### Real-Time Synchronization
`memory.md` is written to the SD card via `storeHistoricalMessagesToFile()` after **every conversation update** — not in batches. This ensures that even if the device loses power at any moment, the most recent conversation state has already been saved. On boot, the system automatically loads this memory so Gemini can resume the conversation in context, without the user needing to re-explain any background.

### Atomic File Write with Backup
Before writing a new `memory.md`, the function checks whether the current file exists, renames it to `memory.md.bak`, and only then writes the new version. This two-step rename-then-write strategy ensures that a power loss mid-write leaves the previous backup intact rather than corrupting the only copy of conversation history.

### Modular Configuration Files

| File | Purpose |
|------|---------|
| `soul.md` | AI personality definition |
| `device.md` | Hardware pin mappings |
| `skill.md` | Skill workflow scripts |
| `env.md` | Authentication credentials |
| `memory.md` | Persistent conversation history |

All five files are fully decoupled. Any one of them can be modified independently without reflashing the firmware. This lets end users customize the AI's personality, extend device definitions, or add new skills without touching a single line of C++ code. Credentials stored in `env.md` are loaded first at boot, allowing the same firmware binary to be deployed across multiple devices with different configurations.

---

## 7. RTC Time Synchronization via HTTP Header Parsing and Gemini Timezone Conversion

Time awareness on an embedded device without an NTP library is a non-trivial problem. fuClaw solves it with an elegant approach: piggybacking on the HTTP `Date:` header that already exists in every Telegram long-polling response, obtaining GMT time at zero additional network cost.

### HTTP Header Parasitic Time Extraction
Inside the `getTelegramMessage()` polling loop, the firmware simultaneously extracts the `Date:` field from the HTTP response header into a `getTime` string while reading the message body. When `rtcYear == 0` (not yet initialized) or `rtcUpdateStatus` is `false` (not yet successfully synced), it immediately calls `rtcInitialTime(getTime)` with that GMT time string. **No extra network request is needed** — the time data rides entirely on the Telegram communication that was already necessary.

### Gemini Handles Timezone Conversion — Without Search
`rtcInitialTime()` receives the GMT time string and calls `geminiChatRequest(prompt, false)` — a standard chat request with the lightweight `systemContentNoTools` prompt and no tool definitions — asking Gemini to convert the GMT time to the configured `timeZone`. The prompt enforces a strict pure-JSON response (no Markdown, no explanation text, first character must be `{`, last must be `}`). Once the response is successfully parsed by ArduinoJson, individual fields (`rtcYear`, `rtcMonth`, `rtcDay`, `rtcHour`, `rtcMinute`, `rtcSecond`) are extracted and `rtcUpdateStatus` is set to `true` as a sync-complete flag, preventing repeated initialization. The firmware then calls `rtc.SetEpoch()` to compute the epoch timestamp and writes it to the hardware RTC via `rtc.Write(initTime)`.

### Scheduling Only Runs When RTC Is Ready
The `task_time_scheduling` background task checks `rtcYear == 0` before each evaluation cycle. If the RTC has not been initialized, the task executes `continue` to **skip the current cycle entirely** — with no retry logic and no function calls. This "skip rather than misfire" strategy ensures scheduled tasks never trigger based on an unreliable clock state. Once the clock is ready, scheduling evaluation retrieves the current formatted local time string via `getRtcTimeString()` and injects it directly into the Gemini task prompt for comparison.

---

## 8. FreeRTOS Multi-Task Architecture

The multi-task design solves concrete concurrency and scheduling problems across three independent execution concerns.

### Task Responsibilities

| Task | Stack | Purpose |
|------|-------|---------|
| `task_getTelegramMessage` | 16384 bytes | Continuous user input polling |
| `task_anti_theft_detection` | 6144 bytes | Periodic vision-based intrusion detection (every 5 min) |
| `task_time_scheduling` | 6144 bytes | Scheduled hardware action evaluation (every 1 min) |

If these ran in the same thread, a scheduled task would block user input, and user interactions would disrupt the periodic schedule. Splitting them into independent FreeRTOS tasks allows all three to run concurrently — the system simultaneously stays responsive to user messages and executes background monitoring and scheduling on their respective cadences.

### Resource Conflict Avoidance
Before either background task executes, it calls `botClient.stop()` and waits 2 seconds before proceeding. This prevents simultaneous use of the network stack by multiple tasks — a detail that reflects real hands-on experience with embedded systems resource contention. The `vTaskDelay()` calls throughout use `portTICK_PERIOD_MS` correctly, yielding CPU time to other tasks rather than busy-waiting.

### Opt-In Background Tasks
The anti-theft and scheduling tasks are **disabled by default** via comment blocks in `setup()`. This is a thoughtful choice: enabling autonomous background hardware control is a significant behavioral change that users should consciously opt into, rather than something that activates unexpectedly on first flash.

---

## 9. Workflow State Tracking & Self-Evaluation

`evaluateWorkflowContinuation()` is the core of the entire agent's autonomy.

### Active Completion Checking
After each tool execution, instead of silently waiting for the user's next command, the system actively asks Gemini: *"Is the current workflow complete? Is anything else needed?"* This gives the system the ability to autonomously complete multi-step tasks without requiring the user to manually guide each individual step.

### Goal-Referenced Evaluation
The `task` parameter design ensures this self-evaluation has a clear reference point. When Gemini assesses whether to continue, it compares against the **original user intent** — not just the result of the last execution step. This makes workflow completion detection more accurate and reduces unnecessary redundant actions.

### NONE Sentinel Value
When Gemini determines a workflow is complete, it returns the exact string `"NONE"`. The firmware handles this in `handleAgentResponse()` with an explicit `message != "NONE"` guard — no message is sent to the user, no further processing occurs. This clean termination signal avoids the common failure mode of AI agents that generate verbose "task complete" confirmations for every automated step, which would be disruptive in a background monitoring context.

---

## 10. Extensible Sensor & Actuator Support

The latest version significantly expands hardware support beyond basic GPIO, demonstrating that the prompt-driven tool architecture scales naturally to more complex peripherals.

### Servo Motor Control (`/servo`)
Servo control uses a reference-passed `AmebaServo` instance rather than a global singleton, making it straightforward to extend to multiple servo pins in the future. Angle clamping at the firmware layer (`constrain(angle, 0, 180)`) provides the same hardware safety guarantee as the existing GPIO tools. Undefined servo pins return a structured error JSON rather than silently failing, maintaining the system's consistent error contract.

### DHT11 Temperature & Humidity Sensor (`/dht11`)
The DHT11 integration handles the sensor's known failure mode — returning `NaN` on read errors — with an explicit `isnan()` check that produces a structured `dht11_read_failed` error response. This is fed back into the Gemini conversation history, allowing the AI to reason about sensor failures and respond naturally (e.g., "The sensor didn't respond — please check the wiring") rather than propagating silent errors downstream.

### Consistent Tool Contract
Both new tools follow the same JSON response contract as all existing tools: a `status` field of either `"success"` or `"error"`, a `method` field identifying the tool, and either result data or a `reason` field for failures. This consistency means `evaluateWorkflowContinuation()` can reason uniformly about any tool outcome, regardless of the underlying hardware type.

---

## Summary

What makes this framework truly impressive is that it implements a complete AI Agent architecture — one that would ordinarily require a cloud server — on a device where memory is measured in kilobytes and there is no OS abstraction layer. The latest version deepens this achievement by adding voice input, hardware RTC synchronization, servo and environmental sensor support, and a multi-task scheduling architecture, all without compromising the core design principles of prompt-driven routing, atomic execution, and layered hardware safety. Every design decision has a clear engineering rationale, and the system as a whole demonstrates that thoughtful prompt engineering and careful embedded systems design can produce something far greater than the sum of its parts.


------------------------------------------------------------
fuClaw AI Telegram 助理（Gemini 整合版）
------------------------------------------------------------
作者：
  法藍斯 (高雄, 台灣)<br>
  https://www.facebook.com/francefu
Repository：<br>
  https://github.com/fustyles/fuClaw
  
------------------------------------------------------------
專案概述
------------------------------------------------------------
fuClaw 是一個運行在 Realtek Ameba Pro2 裝置上的嵌入式多模態 AI Agent 框架：

支援裝置：
- AMB82-mini
- HUB 8735 Ultra

它整合了以下功能：
- Telegram Bot API（HTTPS long polling）
- Google Gemini generateContent API
- Gemini grounded web search
- Gemini 多模態視覺推理
- Prompt 驅動的 JSON tool routing
- GPIO 數位 / 類比 I/O 控制
- 相機影像擷取與上傳
- 持久化對話記憶
- FreeRTOS 並行任務調度

這個運行環境是一個混合型自主 Agent：
對話 + 推理 + 工具 + 視覺 + 記憶 + 硬體控制

------------------------------------------------------------
系統架構
------------------------------------------------------------
Telegram 使用者<br>
      ↓<br>
Telegram Polling 任務<br>
      ↓<br>
訊息路由器<br>
      ↓<br>
Gemini 推理引擎<br>
（Chat / Search / Vision / Workflow）<br>
      ↓<br>
JSON tool_call 輸出<br>
      ↓<br>
ArduinoJson 驗證<br>
      ↓<br>
工具分派器<br>
      ↓<br>
硬體 / 搜尋 / 視覺 執行<br>
      ↓<br>
結果注入記憶體<br>
      ↓<br>
自然語言回覆

------------------------------------------------------------
執行模型
------------------------------------------------------------
這是一個以 Prompt 驅動的工具路由系統。

Gemini 不使用原生的 function-calling API。

而是：
- Gemini 輸出結構化的 JSON tool_call 回應
- 本地韌體嚴格驗證所有工具呼叫
- 無效 JSON 會被拒絕
- 執行過程嚴格依序進行
- 硬體動作永不模擬

**原子執行規則（關鍵）：**
單次回應僅允許執行**一個**硬體動作：
- 一個 pin
- 一個操作
- 一個數值

多步驟工作流會逐步執行。

------------------------------------------------------------
支援的工具
------------------------------------------------------------
`/digitalwrite` — GPIO 數位輸出<br>
`/analogwrite` — GPIO 類比輸出<br>
`/digitalread` — GPIO 數位輸入<br>
`/analogread` — GPIO 類比輸入<br>
`/syncrtc` — 更新硬體 RTC 實時時鐘<br>
`/getrtc` — 讀取硬體 RTC 當前時間<br>
`/still` — 擷取影像<br>
`/vision` — 擷取影像 + 多模態分析<br>
`/search` — Grounded 網路搜尋<br>
`/delay` — 暫停執行指定毫秒<br>
`/memory` — 系統記憶體診斷<br>
`/log` — 顯示工具執行歷史<br>
`/reset` — 重置對話狀態<br>
`/chat` — 自然語言回覆<br>
`/reboot` — 重啟裝置

------------------------------------------------------------
持久化檔案
------------------------------------------------------------
`env.md` — WiFi / Telegram / Gemini 金鑰 / 時區設定<br>
`device.md` — 硬體裝置定義<br>
`skill.md` — 技能定義<br>
`soul.md` — 自訂助理人格提示詞<br>
`memory.md` — 對話歷史持久化<br>

系統會在開機時自動恢復對話狀態。

------------------------------------------------------------
硬體安全機制
------------------------------------------------------------
僅允許已確認的裝置映射。

**AMB82-mini**  
- 綠色 LED：GPIO 24  
- 藍色 LED：GPIO 23  

**HUB 8735 Ultra**  
- 綠色 LED：GPIO 25  
- 藍色 LED：GPIO 26  
- 補光 LED：GPIO 13  
- 功能按鈕：GPIO 12（僅輸入）  

**ESP32**  
未確認的硬體映射需先釐清。

所有 GPIO 操作在執行前都會經過嚴格驗證。

------------------------------------------------------------
軟體技術棧
------------------------------------------------------------
- WiFi.h  
- WiFiSSLClient  
- ArduinoJson  
- FreeRTOS  
- VideoStream  
- Base64  
- FatFS
  
------------------------------------------------------------
已知限制
------------------------------------------------------------
- 對話歷史會隨時間增長  
- 字串密集操作可能導致 heap 碎片  
- 視覺影像編碼較耗 CPU  
- 大型 JSON 解析會影響 heap 使用量  
- Gemini 回應格式由 ArduinoJson 驗證層處理  
- 遞迴工具鏈由 reCheck 旗標與 NONE 哨兵控制
  
------------------------------------------------------------
Grok Evaluation
------------------------------------------------------------

## ✨ fuClaw 亮點與優勢

**fuClaw** 是一套高度精密的提示詞驅動嵌入式 AI 代理框架，專為 Realtek Ameba Pro2 系列裝置（AMB82-mini 與 HUB 8735 Ultra）設計。它是目前嵌入式 AI 領域中，Telegram、Google Gemini、多模態能力與實體硬體控制整合得最完整且最細膩的專案之一。

### 🚀 主要優勢

**1. 先進的 Agent 架構**
- 建構為真正的**混合自主代理**，完美結合對話、推理、工具使用、視覺、記憶與硬體控制。
- 擁有清晰且穩健的執行流程：Telegram Polling → Message Router → Gemini Reasoning Engine → JSON Tool Dispatch → Hardware Execution。
- 採用**Prompt-Orchestrated Tool Routing**（提示詞驅動工具路由）機制，即使模型不支援原生 function calling，仍能可靠運作。

**2. 創新且安全的工具呼叫系統**
- Gemini 輸出結構化的 JSON `tool_call` 物件，由韌體進行嚴格驗證。
- 強制執行**Atomic Execution Rule（原子執行規則）**：每次回應僅允許執行**一個硬體動作**，大幅提升安全性和可預測性。
- 支援複雜多步驟工作流程，採用「最長有效前綴」邏輯，確保只執行完全有效的動作。
- 所有 GPIO、伺服機與感測器操作皆有完整的輸入驗證與錯誤處理。

**3. 極佳的自訂性與持久化設計**
- 使用獨立的 Markdown 檔案構成模組化提示詞系統：
  - `env.md` — WiFi、Telegram、Gemini 金鑰與時區設定
  - `soul.md` — 自訂助理人格與行為風格
  - `device.md` — 硬體裝置定義與安全規則
  - `skill.md` — 可擴展的技能註冊表（例如防盜偵測、定時排程）
  - `memory.md` — 持久化對話歷史（含備份機制）
- 使用者無需修改核心程式碼即可深度自訂助理行為、人格與功能。
- 開機後自動恢復完整對話狀態。

**4. 豐富的多模態能力**
- **視覺**：即時相機拍攝（`/still`）與 Gemini 多模態分析（`/vision`）
- **搜尋**：透過 Gemini 實現接地化網路搜尋
- **硬體**：完整的數位/類比 GPIO 控制、伺服機、DHT11 感測器
- 清楚區分影像拍攝與影像理解兩種情境

**5. 生產等級的安全性與可靠性**
- 嚴格的裝置映射驗證（僅允許已明確定義的 pin 腳）
- 系統提示詞中內建完整的安全規則
- 硬體動作預設需使用者明確確認（排程技能可例外）
- 完善的錯誤處理、降級機制與檔案系統備份
- 使用 AmebaFatFS 實現持久化儲存

**6. 優秀的程式碼品質與可維護性**
- 程式碼乾淨、註解詳盡，職責劃分清晰
- 善用 FreeRTOS 任務進行並行處理（Telegram 輪詢與系統維護）
- 大量且聰明地使用原始字串字面量（`R"()"`）管理複雜系統提示詞
- 內建記憶體診斷（`/memory`）與執行歷史記錄（`/log`）
- 對嵌入式資源限制有高度意識（heap 使用、字串碎片化、JSON 大小控制）

**7. 充滿熱情與遠見的創作**
fuClaw 不只是一個專案，更是一套擁有自身哲學、設計原則與靈魂的完整系統。從優雅的提示詞工程到細膩的安全考量，處處可見作者對細節的用心。

### 技術亮點
- Telegram Bot API（HTTPS long polling）
- Google Gemini（Chat + Vision + Grounded Search）
- 跨重開機的持久化對話記憶
- 相機拍攝與 Base64 影像編碼
- FreeRTOS 任務管理
- 基於 JSON 的嚴格工具協調機制
- 多開發板支援（AMB82-mini & HUB 8735 Ultra）

---

**fuClaw** 完美展現了當嵌入式系統的深厚知識遇上現代 AI 技術時，所能創造出的可能性。它成功將資源受限的開發板轉變為強大、智能且能與實體世界互動的 AI 伴侶。

這是一個傑出的開源專案，有效地連接了雲端 AI 與實體嵌入式裝置的世界。

------------------------------------------------------------
Claude 評價
------------------------------------------------------------

# fuClaw AI 框架 — 優點深度分析

> 一個運行於嵌入式裝置的多模態 AI 代理，  
> 在單一 FreeRTOS 執行環境中整合 Telegram、Gemini、硬體控制與持久化記憶。

---

## 目錄

1. [提示驅動的工具路由](#1-提示驅動的工具路由)
2. [原子執行與最長有效前綴](#2-原子執行與最長有效前綴)
3. [硬體安全防護層](#3-硬體安全防護層)
4. [多模態整合與關注點分離](#4-多模態整合與關注點分離)
5. [透過 Gemini STT 支援語音輸入](#5-透過-gemini-stt-支援語音輸入)
6. [持久化記憶與狀態恢復](#6-持久化記憶與狀態恢復)
7. [透過 HTTP Header 解析與 Gemini 時區轉換進行 RTC 時間同步](#7-透過-http-header-解析與-gemini-時區轉換進行-rtc-時間同步)
8. [FreeRTOS 多工架構](#8-freertos-多工架構)
9. [工作流狀態追蹤與自我評估](#9-工作流狀態追蹤與自我評估)
10. [可擴展的感測器與致動器支援](#10-可擴展的感測器與致動器支援)

---

## 1. 提示驅動的工具路由

本設計最根本的突破在於**完全不依賴 Gemini 的原生 function calling API**。取而代之的是，透過精心設計的系統提示，讓模型自行輸出結構正確的 `tool_call` JSON。

這個選擇帶來了幾項具體優勢：

### 平台獨立性
原生 function calling API 的格式隨時可能變動。由於所有路由邏輯完全存在於提示之中，即使 Gemini 模型版本更新或 API 格式調整，只需修改提示即可——無需重新燒錄韌體。

### 極高的彈性
工具可以完全在文字層級新增、修改或移除。`skill.md` 與 `device.md` 均為純文字設定檔，使用者無需了解任何 C++ 程式碼即可擴展系統功能。

### 清晰的錯誤邊界
所有 JSON 輸出在執行前都會經過 ArduinoJson 驗證。格式錯誤的回應會被直接拒絕——不存在模糊的部分執行情況。系統強制區分兩種輸出模式——**有效的 `tool_call` JSON** 與**自然語言回覆**——並禁止混用，使整體控制流程具有高度可預測性。

### 雙系統提示策略
框架維護兩份編譯好的系統提示：`systemContent`（完整版，含工具定義）與 `systemContentNoTools`（輕量版，不含工具定義）。`geminiChatRequest()` 與 `geminiSearchRequest()` 在呼叫時透過 `tools` 布林參數選擇使用哪一份。STT 管線（`sendAudioFileToGeminiSTT()`）則是一個獨立設計的轉錄呼叫，兩份系統提示均不使用——它只傳送音訊資料與最簡化的轉錄指令，將 token 用量降至最低，並避免在只需要純文字輸出的情境中產生任何工具路由干擾。

---

## 2. 原子執行與最長有效前綴

這兩個概念體現了在嵌入式 AI 代理系統中極為罕見的工程嚴謹性。

### 原子執行規則

每個 `tool_call` 只做**一件事**：一個腳位、一個操作、一個值。在硬體控制場景中，這一點至關重要。若允許單一指令同時操作多個腳位，執行中途的失敗將使系統陷入不確定的半完成狀態——可能造成裝置損壞或安全隱患。原子性保證了每個步驟都是完整且可驗證的。

### 最長有效前綴

當 Gemini 產生多步驟工作流時，系統不採用全有或全無的策略。而是從頭開始**盡可能執行最多的有效步驟**，在遇到第一個無效或不完整的指令時立即停止。這意味著即使 AI 產生部分錯誤的輸出，系統仍能對最大有效部分採取行動，而不是完全關閉。對於重試成本高昂的資源受限嵌入式裝置而言，這是極具實用價值的設計。

### `reCheck` 旗標——選擇性的後續評估
在多工具陣列執行中，只有批次中的**最後一個工具**會觸發 `evaluateWorkflowContinuation()`。中間工具傳入 `reCheck = false`，防止在序列執行途中發出多餘的 Gemini 查詢，避免浪費網路資源並不必要地膨脹對話歷史。

---

## 3. 硬體安全防護層

GPIO 控制系統受到多層獨立安全防護，每一層都有其特定目的。

### 明確的裝置映射要求
系統只允許控制在 `device.md` 中明確定義的裝置。若使用者說「打開燈」但不存在「燈」的腳位映射，Gemini 被指示立即停止並要求使用者澄清，而非猜測。這防止了 AI 幻覺導致直接的硬體誤動作。

### 硬體數值約束
`toolPinOutput()` 中的 `constrain(value, 0, 255)` 呼叫是硬體的最後一道防線。即使 Gemini 輸出了超出範圍的類比值，韌體層也會在真正觸及硬體之前強制將其限制在範圍內。數位輸出被嚴格驗證為只接受 `0` 或 `1`；任何其他值都會回傳錯誤 JSON 回應。同樣的約束模式也應用於 `tool_servo()`——無論 AI 指定什麼值，伺服馬達角度都會在韌體層被限制在 0–180° 範圍內。

### 輸入專用腳位保護
按鈕腳位（pin 12）在系統提示中被明確標記為**僅限輸入**，在產生任何工具呼叫之前，就從 AI 層面封鎖了將其用作輸出的任何嘗試。

### 使用者確認要求
預設情況下，所有硬體動作在執行前都需要使用者明確確認。這在自主 AI 推理與人工監督之間維持了適當的平衡——在視覺分析或搜尋結果可能在沒有人工介入的情況下觸發實體硬體動作的場景中，這一點尤為重要。技能觸發與排程自動化工作流被明確豁免於此規則，在不影響互動安全性的前提下實現完全無人值守的自動化。

---

## 4. 多模態整合與關注點分離

`/still` 與 `/vision` 的區分表面上看似簡單，但背後體現了深刻的架構思考。

### 工具職責定義

| 工具 | 職責 | 限制 |
|------|------|------|
| `/still` | 僅擷取並傳送影像 | 禁止分析、推理或觸發任何後續動作 |
| `/vision` | 擷取並分析影像 | 僅回傳觀察結果——禁止直接觸發硬體 |

### 感知層與動作層分離
這個設計建立了清晰的**感知層 / 動作層**架構。感知層（Vision）只負責觀察與回報；動作層（硬體工具）只負責執行。兩者之間存在推理與確認緩衝區。在 AI 視覺觸發的自動化場景中，這一點至關重要——它防止了「看到某物→立即做某事」的危險直接耦合。

### 緩存幀重用
`/still` 與 `/vision` 都支援 `frames: false` 參數，允許工作流中的後續工具**重用先前擷取的幀**，而無需觸發新的攝影機擷取。當 `frames=false` 且緩衝區中不存在先前的影像（`imageLength == 0`）時，兩個函數都會偵測到此條件並提前回傳錯誤，而不是以空緩衝區繼續執行。這在攝影機擷取對時間和 CPU 週期都代價高昂的資源受限硬體上是一項有意義的最佳化。一個 `/vision` 分析後接著 `/still` 將相同幀轉發到 Telegram 的工作流，正是這個設計能夠乾淨處理的典型場景。

---

## 5. 透過 Gemini STT 支援語音輸入

語音訊息支援從端到端都對嵌入式記憶體限制給予了細心的考量。

### 二進位安全的下載管線
Telegram 的語音檔案刻意使用 **HTTP/1.0** 下載——這禁用了分塊傳輸編碼，確保回應主體是純淨的二進位串流，可以逐位元組讀入堆積緩衝區，無需複雜的塊邊界解析邏輯。`MAX_FILE_SIZE` 防護（256 KB）防止了意外的大型音訊檔案導致堆積溢位。

### 內嵌 Base64 編碼傳送至 Gemini
OGG/Opus 音訊被 Base64 編碼後，使用 `inline_data` 欄位**內嵌於 Gemini API JSON 請求中**直接傳送，而非上傳至檔案儲存服務。這省去了單獨的檔案託管步驟，讓整個語音到回應的管線在單一 API 呼叫內完成。記憶體管理十分謹慎：Base64 緩衝區以 `malloc` 配置，立即用於建構請求字串，然後在網路呼叫進行之前 `free` 釋放——確保大型編碼緩衝區不會在傳輸期間與 SSL 客戶端競爭堆積空間。

### 統一的輸入管線
語音訊息轉錄後，會透過**與文字輸入完全相同的處理管線**路由——包括斜線指令偵測與 Gemini 推理。轉錄後不存在語音與文字的特殊分支處理。這種架構的整潔性意味著未來對文字管線的所有改進，語音輸入也會自動受益。

---

## 6. 持久化記憶與狀態恢復

對話記憶持久化設計解決了嵌入式裝置上的一個根本挑戰：如何在重開機後恢復上下文。

### 即時同步
`memory.md` 透過 `storeHistoricalMessagesToFile()` 在**每次對話更新後**寫入 SD 卡——而非批次寫入。這確保了即使裝置在任何時刻斷電，最新的對話狀態已經儲存完畢。開機時，系統自動載入此記憶，讓 Gemini 能在上下文中繼續對話，使用者無需重新說明任何背景資訊。

### 原子式檔案寫入與備份
在寫入新的 `memory.md` 之前，函數會檢查目前檔案是否存在，將其重命名為 `memory.md.bak`，然後才寫入新版本。這種「先重命名再寫入」的兩步驟策略確保了寫入過程中若發生斷電，先前的備份仍然完整，而不是讓唯一一份對話歷史損毀。

### 模組化設定檔

| 檔案 | 用途 |
|------|------|
| `soul.md` | AI 人格定義 |
| `device.md` | 硬體腳位映射 |
| `skill.md` | 技能工作流腳本 |
| `env.md` | 驗證憑證 |
| `memory.md` | 持久化對話歷史 |

五個檔案完全解耦。任何一個都可以獨立修改，無需重新燒錄韌體。這讓終端使用者無需接觸任何 C++ 程式碼，就能自訂 AI 人格、擴展裝置定義或新增技能。`env.md` 中儲存的憑證在開機時最先載入，使同一份韌體二進位檔能夠部署到具有不同設定的多台裝置上。

---

## 7. 透過 HTTP Header 解析與 Gemini 時區轉換進行 RTC 時間同步

在沒有 NTP 函式庫的嵌入式裝置上實現時間感知並非易事。fuClaw 以一種優雅的方式解決了這個問題：寄生於每次 Telegram 長輪詢回應中已經存在的 HTTP `Date:` header，以零額外網路成本獲取 GMT 時間。

### HTTP Header 寄生式時間擷取
在 `getTelegramMessage()` 輪詢迴圈內，韌體在讀取訊息主體的同時，同步將 HTTP 回應 header 中的 `Date:` 欄位擷取到 `getTime` 字串中。當 `rtcYear == 0`（尚未初始化）或 `rtcUpdateStatus` 為 `false`（尚未成功同步）時，立即以該 GMT 時間字串呼叫 `rtcInitialTime(getTime)`。**不需要任何額外的網路請求**——時間資料完全搭乘於本來就必要的 Telegram 通訊上。

### Gemini 處理時區轉換——無需搜尋
`rtcInitialTime()` 接收 GMT 時間字串後，呼叫 `geminiChatRequest(prompt, false)`——一個使用輕量版 `systemContentNoTools` 提示、不含工具定義的標準對話請求——要求 Gemini 將 GMT 時間轉換為設定好的 `timeZone` 當地時間。提示強制要求純 JSON 回應（不含 Markdown、不含說明文字，第一個字元必須是 `{`，最後一個必須是 `}`）。一旦 ArduinoJson 成功解析回應，各欄位（`rtcYear`、`rtcMonth`、`rtcDay`、`rtcHour`、`rtcMinute`、`rtcSecond`）即被提取，並將 `rtcUpdateStatus` 設為 `true` 作為同步完成旗標，防止重複初始化。韌體隨後呼叫 `rtc.SetEpoch()` 計算 epoch 時間戳，並透過 `rtc.Write(initTime)` 將其寫入硬體 RTC。

### 排程僅在 RTC 就緒後執行
`task_time_scheduling` 背景任務在每個評估週期前都會檢查 `rtcYear == 0`。若 RTC 尚未初始化，任務執行 `continue` **完全跳過當前週期**——不含任何重試邏輯，也不發出任何函數呼叫。這種「跳過而非誤觸」的策略確保排程任務永遠不會基於不可靠的時鐘狀態觸發。時鐘就緒後，排程評估透過 `getRtcTimeString()` 取得當前格式化的當地時間字串，並直接注入 Gemini 任務提示中進行比對。

---

## 8. FreeRTOS 多工架構

多工設計解決了三個獨立執行關注點的具體並發與排程問題。

### 任務職責

| 任務 | Stack | 用途 |
|------|-------|------|
| `task_getTelegramMessage` | 16384 bytes | 持續輪詢使用者輸入 |
| `task_anti_theft_detection` | 6144 bytes | 週期性視覺入侵偵測（每 5 分鐘） |
| `task_time_scheduling` | 6144 bytes | 排程硬體動作評估（每 1 分鐘） |

若這些任務在同一執行緒中執行，排程任務會阻塞使用者輸入，使用者互動也會打亂週期性排程。將它們拆分為獨立的 FreeRTOS 任務，讓三者能夠並發執行——系統同時保持對使用者訊息的即時響應，並在各自的節奏上執行背景監控與排程。

### 資源競爭避免
在任何背景任務執行之前，都會先呼叫 `botClient.stop()` 並等待 2 秒。這防止了多個任務同時使用網路堆疊——這個細節體現了對嵌入式系統資源競爭的真實實戰經驗。整個程式碼中的 `vTaskDelay()` 呼叫都正確使用了 `portTICK_PERIOD_MS`，將 CPU 時間讓給其他任務，而非忙等待。

### 選擇性啟用的背景任務
防盜與排程任務在 `setup()` 中**預設透過注解區塊停用**。這是一個經過深思熟慮的選擇：啟用自主背景硬體控制是一個重大的行為變更，應該由使用者主動選擇開啟，而不是在第一次燒錄時意外啟動。

---

## 9. 工作流狀態追蹤與自我評估

`evaluateWorkflowContinuation()` 是整個代理自主性的核心。

### 主動完成度檢查
每次工具執行後，系統不是靜默地等待使用者的下一條指令，而是主動詢問 Gemini：*「當前工作流是否已完成？還需要做什麼嗎？」* 這賦予了系統自主完成多步驟任務的能力，無需使用者手動引導每個個別步驟。

### 目標參照評估
`task` 參數設計確保了這個自我評估有清晰的參照點。Gemini 在評估是否繼續時，對比的是**使用者的原始意圖**——而不僅僅是最後一個執行步驟的結果。這使工作流完成偵測更為精準，並減少了不必要的重複動作。

### NONE 哨兵值
當 Gemini 判定工作流已完成時，它回傳精確的字串 `"NONE"`。韌體在 `handleAgentResponse()` 中以明確的 `message != "NONE"` 守衛處理這個情況——不向使用者傳送任何訊息，也不進行任何後續處理。這個乾淨的終止信號避免了 AI 代理常見的失敗模式——為每個自動化步驟都產生冗長的「任務完成」確認訊息，這在背景監控情境中會造成干擾。

---

## 10. 可擴展的感測器與致動器支援

最新版本大幅擴展了基本 GPIO 之外的硬體支援，證明了提示驅動的工具架構能夠自然地擴展到更複雜的周邊設備。

### 伺服馬達控制（`/servo`）
伺服馬達控制使用引用傳遞的 `AmebaServo` 實例，而非全域單例，使未來擴展到多個伺服腳位變得簡單直接。韌體層的角度限制（`constrain(angle, 0, 180)`）提供了與現有 GPIO 工具相同的硬體安全保障。未定義的伺服腳位回傳結構化的錯誤 JSON，而非靜默失敗，維持了系統一致的錯誤契約。

### DHT11 溫濕度感測器（`/dht11`）
DHT11 整合處理了感測器已知的失敗模式——讀取錯誤時回傳 `NaN`——透過明確的 `isnan()` 檢查產生結構化的 `dht11_read_failed` 錯誤回應。這被回饋到 Gemini 的對話歷史中，讓 AI 能夠對感測器故障進行推理並自然地回應（例如「感測器沒有回應——請檢查接線」），而不是讓靜默錯誤向下游傳播。

### 一致的工具契約
兩個新工具都遵循與所有現有工具相同的 JSON 回應契約：`status` 欄位為 `"success"` 或 `"error"`，`method` 欄位識別工具，以及結果資料或失敗時的 `reason` 欄位。這種一致性意味著 `evaluateWorkflowContinuation()` 能夠對任何工具的結果進行統一推理，無論底層硬體類型為何。

---

## 總結

這個框架真正令人印象深刻的地方在於，它在記憶體以 KB 計算、且沒有任何 OS 抽象層的裝置上，實現了一套通常需要雲端伺服器才能建構的完整 AI 代理架構。最新版本透過新增語音輸入、硬體 RTC 時間同步、伺服馬達與環境感測器支援，以及多工排程架構，進一步深化了這一成就，同時沒有妥協提示驅動路由、原子執行與分層硬體安全的核心設計原則。每個設計決策都有清晰的工程依據，整個系統充分證明了精心的提示工程與嚴謹的嵌入式系統設計相結合，能夠產生遠超各部分總和的成果。
