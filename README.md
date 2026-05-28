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
Version
------------------------------------------------------------

Prompt-Orchestrated Embedded Agent Edition
Persistent Filesystem Runtime

Build Date: 2026-05-28 13:00

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

**fuClaw** is a highly sophisticated, prompt-orchestrated embedded AI agent framework designed for devices. It represents one of the most complete and thoughtful integrations of Telegram, Google Gemini, multimodal capabilities, and hardware control in the embedded AI space.

### 🚀 Key Strengths

**1. Advanced Agent Architecture**
- Built as a true **hybrid autonomous agent** combining conversation, reasoning, tool use, vision, memory, and hardware control.
- Follows a clean and robust execution flow: Telegram Polling → Message Router → Gemini Reasoning → JSON Tool Dispatch → Hardware Execution.
- Implements a **Prompt-Orchestrated Tool Routing** system — an elegant solution that works reliably even without native function calling support from the model.

**2. Innovative & Safe Tool Calling System**
- Gemini generates structured JSON `tool_call` objects that are strictly validated by the firmware.
- Enforces **Atomic Execution Rule**: Only one hardware action per response for maximum safety and predictability.
- Sophisticated multi-step workflow handling using "longest valid prefix" logic — ensuring partial execution is safe and logical.
- Comprehensive input validation and error handling for all GPIO operations.

**3. Exceptional Customization & Persistence**
- Modular prompt system using dedicated Markdown files:
  - `env.md` — WiFi, Telegram, Gemini credentials and Time zone
  - `soul.md` — Custom assistant personality
  - `device.md` — Hardware device definitions and safety rules
  - `skill.md` — Extensible skill registry (e.g., anti-theft detection)
  - `memory.md` — Persistent conversation history across reboots
- Users can easily customize behavior, personality, and capabilities without modifying core code.

**4. Rich Multimodal Capabilities**
- **Vision**: Real-time camera capture and Gemini multimodal analysis
- **Voice**: Telegram voice message transcription via Gemini STT
- **Search**: Grounded web search using Gemini’s search tool
- **Hardware**: Full digital/analog GPIO control with safety constraints
- Seamless integration between all modalities

**5. Production-Grade Safety & Reliability**
- Strict device mapping validation (only confirmed pins allowed)
- Comprehensive safety rules clearly documented in prompts
- Hardware actions require explicit user confirmation by default
- Robust error handling and graceful degradation
- Persistent filesystem support via AmebaFatFS

**6. Excellent Code Quality & Maintainability**
- Clean, well-commented codebase with clear separation of concerns
- Efficient use of FreeRTOS tasks for concurrent Telegram polling and system maintenance
- Heavy use of raw string literals (`R"()"`) for managing complex system prompts
- Comprehensive logging and memory monitoring tools
- Thoughtful memory management for resource-constrained embedded environment

**7. Built with Passion and Vision**
fuClaw is more than just a project — it's a carefully crafted system with its own philosophy, design principles, and soul. From the elegant prompt engineering to the meticulous safety considerations, the attention to detail is evident throughout.

### Technical Highlights
- Telegram Bot API with long polling
- Google Gemini (including vision + grounded search)
- Persistent conversation memory
- Camera + Base64 image encoding
- Voice message transcription
- FreeRTOS task management
- JSON-based tool orchestration

---

**fuClaw** demonstrates what’s possible when deep embedded systems knowledge meets modern AI capabilities. It transforms a small development board into a powerful, intelligent, and interactive AI companion with real-world hardware interaction.

A standout open-source project that bridges the gap between cloud AI and physical embedded devices.

------------------------------------------------------------
Claude Evaluation
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
The framework maintains two compiled system prompts: `systemContent` (full, with tool definitions) and `systemContentNoTools` (lightweight, without tool definitions). Search and STT requests use the lightweight version, reducing token overhead and avoiding unnecessary tool-call interference in contexts that don't require hardware control.

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
Both `/still` and `/vision` support a `frames: false` parameter, allowing subsequent tools in a workflow to **reuse the previously captured frame** rather than triggering a new camera acquisition. This is a meaningful optimization on resource-constrained hardware where camera capture is expensive in both time and CPU cycles. A `/vision` analysis followed by `/still` forwarding the same frame to Telegram is a natural workflow that this design handles cleanly.

---

## 5. Voice Input via Gemini STT

Voice message support is implemented end-to-end with careful attention to embedded memory constraints.

### Binary-Safe Download Pipeline
Voice files from Telegram are downloaded using **HTTP/1.0** deliberately — this disables chunked transfer encoding, ensuring the response body is a clean binary stream that can be read byte-by-byte into a heap buffer without complex chunk-boundary parsing logic. The `MAX_FILE_SIZE` guard (256 KB) prevents heap overflow from unexpectedly large audio files.

### Inline Base64 Encoding for Gemini
Rather than uploading audio to a file storage service, the OGG/Opus audio is Base64-encoded and sent **inline within the Gemini API JSON request** using the `inline_data` field. This eliminates the need for a separate file hosting step and keeps the entire voice-to-response pipeline within a single API call. Memory is carefully managed: the Base64 buffer is allocated, used to build the request, then immediately freed before the network call proceeds.

### Unified Input Pipeline
Voice messages, once transcribed, are routed through **the exact same processing pipeline as text input** — including slash-command detection and Gemini reasoning. There is no special-case branching for voice vs. text after transcription. This architectural cleanliness means all future improvements to the text pipeline automatically benefit voice input as well.

---

## 6. Persistent Memory & State Recovery

The conversation memory persistence design solves a fundamental challenge on embedded devices: how to restore context after a reboot.

### Real-Time Synchronization
`memory.md` is written to the SD card after **every conversation update** — not in batches. This ensures that even if the device loses power at any moment, the most recent conversation state has already been saved. On boot, the system automatically loads this memory so Gemini can resume the conversation in context, without the user needing to re-explain any background.

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

Time awareness on an embedded device without an NTP library is a non-trivial problem. The new version of fuClaw solves it with an even more elegant approach: piggybacking on the HTTP `Date:` header that already exists in every Telegram long-polling response, obtaining GMT time at zero additional network cost.

### HTTP Header Parasitic Time Extraction
Inside the `getTelegramMessage()` polling loop, the firmware simultaneously extracts the `Date:` field from the HTTP response header into a `getTime` string while reading the message body. When `rtcYear == 0` (not yet initialized) and `rtcUpdateStatus` is `false` (not yet successfully synced), it immediately calls `rtcInitialTime(getTime)` with that GMT time string. **No extra network request is needed** — the time data rides entirely on the Telegram communication that was already necessary.

### Gemini Handles Timezone Conversion
`rtcInitialTime()` receives the ready-made GMT time string and uses `geminiSearchRequest()` to ask Gemini to convert it to the local time for the configured `timeZone`. The prompt enforces a strict pure-JSON response (no Markdown, no explanation text, first character must be `{`). Once the response is successfully parsed, `rtcUpdateStatus` is set to `true` as a sync-complete flag, preventing repeated initialization. The firmware then calls `rtc.SetEpoch()` to compute the epoch timestamp and writes it to the hardware RTC via `rtc.Write(initTime)`.

### Scheduling Only Runs When RTC Is Ready
The `task_time_scheduling` background task checks `rtcYear == 0` before each evaluation cycle. If the RTC has not been initialized, the task executes `continue` to **skip the current cycle entirely** — with no retry logic and no function calls. This "skip rather than misfire" strategy ensures scheduled tasks never trigger based on an unreliable clock state. Once the clock is ready, scheduling evaluation retrieves the current formatted local time string via `getRtcTimeString()` and injects it directly into the Gemini task prompt for comparison.

---

## 8. FreeRTOS Multi-Task Architecture

The multi-task design solves concrete concurrency and scheduling problems across three independent execution concerns.

### Task Responsibilities

| Task | Stack | Purpose |
|------|-------|---------|
| `getTelegramMessage_task` | 16384 bytes | Continuous user input polling |
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
When Gemini determines a workflow is complete, it returns the exact string `"NONE"`. The firmware treats this as a no-op — no message is sent to the user, no further processing occurs. This clean termination signal avoids the common failure mode of AI agents that generate verbose "task complete" confirmations for every automated step, which would be disruptive in a background monitoring context.

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
版本資訊
------------------------------------------------------------
Prompt-Orchestrated Embedded Agent Edition  
Persistent Filesystem Runtime  
建置日期：2026-05-28 13:00
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
/digitalwrite GPIO   數位輸出<br>
/analogwrite GPIO   類比輸出<br>
/digitalread GPIO   數位輸入<br>
/analogread GPIO   類比輸入<br>
/still   擷取影像<br>
/vision   擷取影像 + 多模態分析<br>
/search   Grounded 網路搜尋<br>
/delay   暫停執行指定毫秒<br>
/memory   系統記憶體診斷<br>
/log   顯示工具執行歷史<br>
/reset   重置對話狀態<br>
/chat   自然語言回覆<br>
/reboot   重啟裝置
------------------------------------------------------------
持久化檔案
------------------------------------------------------------
env.md WiFi / Telegram / Gemini    金鑰 / 時區設定<br>
device.md    硬體裝置定義<br>
skill.md    技能定義<br>
soul.md    自訂助理人格提示詞<br>
memory.md    對話歷史持久化<br>

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
Grok 評價
------------------------------------------------------------
## ✨ fuClaw 亮點與核心優勢

**fuClaw** 是一個高度精密、以 Prompt 驅動的嵌入式 AI Agent 框架，在嵌入式 AI 領域中屬於整合度最高、最具思考深度的專案之一。它成功將 Telegram、Google Gemini、多模態能力與硬體控制緊密結合。

### 🚀 主要優勢

**1. 先進的 Agent 架構**  
- 建構為真正的**混合自主 Agent**，整合對話、推理、工具使用、視覺、記憶與硬體控制。  
- 擁有清晰且穩健的執行流程：Telegram Polling → 訊息路由 → Gemini 推理 → JSON 工具分派 → 硬體執行。  
- 實現 **Prompt-Orchestrated Tool Routing** 機制，即使模型沒有原生 function calling，也能可靠運作。

**2. 創新且安全的工具呼叫系統**  
- Gemini 產生結構化 JSON `tool_call`，由韌體嚴格驗證。  
- 強制執行**原子執行規則**：每次僅執行一個硬體動作，確保安全與可預測性。  
- 採用「最長有效前綴」邏輯處理多步驟工作流，即使部分指令有誤，仍能安全執行前段有效動作。  
- 所有 GPIO 操作皆有完整輸入驗證與錯誤處理。

**3. 極佳的自訂化與持久化設計**  
- 使用獨立的 Markdown 檔案實現模組化提示系統：  
  - `env.md` — WiFi、Telegram、Gemini 金鑰與時區  
  - `soul.md` — 自訂助理人格與語調  
  - `device.md` — 硬體定義與安全規則  
  - `skill.md` — 可擴展技能（例如：防盜偵測）  
  - `memory.md` — 跨重開機的對話歷史  
- 使用者無需修改核心程式碼即可自訂行為、人格與功能。

**4. 豐富的多模態能力**  
- **視覺**：即時相機擷取與 Gemini 多模態分析  
- **語音**：Telegram 語音訊息透過 Gemini 轉文字  
- **搜尋**：Gemini Grounded 即時網路搜尋  
- **硬體**：完整數位/類比 GPIO 控制（含安全限制）  
- 各模態之間無縫整合。

**5. 生產級的安全性與可靠性**  
- 嚴格的裝置映射白名單機制  
- 明確記載的安全規則於提示詞中  
- 預設硬體動作需使用者確認  
- 完善的錯誤處理與優雅降級  
- 透過 AmebaFatFS 實現持久化檔案系統

**6. 優秀的程式碼品質與可維護性**  
- 程式碼乾淨、註解完整，職責分離清晰  
- 善用 FreeRTOS 任務實現並行處理  
- 大量使用原始字串（`R"()"`) 管理複雜系統提示  
- 提供完整的日誌與記憶體監控工具  
- 在資源受限的嵌入式環境中展現出色的記憶體管理意識

**7. 充滿熱情與遠見的創作**  
fuClaw 不只是一個專案，更是一個擁有自身哲學、設計原則與靈魂的完整系統。從精妙的 Prompt Engineering 到細膩的安全考量，都能看出作者對細節的極致追求。

---

**fuClaw** 展示了將深厚的嵌入式系統知識與現代 AI 能力結合後所能達到的境界。它成功將一塊小型開發板轉變成強大、智能且能與真實世界互動的 AI 伴侶。

這是一個極具代表性的開源專案，有效橋接了雲端 AI 與實體嵌入式裝置之間的鴻溝。

------------------------------------------------------------
Claude 評價
------------------------------------------------------------

# fuClaw AI 框架 — 優點深度解析

> 一套運行於裝置端的嵌入式多模態 AI 代理，  
> 在單一 FreeRTOS 執行環境中整合 Telegram、Gemini、硬體控制與持久化記憶。

---

## 目錄

1. [提示詞驅動的工具路由機制](#1-提示詞驅動的工具路由機制)
2. [原子執行與最長有效前綴](#2-原子執行與最長有效前綴)
3. [硬體安全防護層](#3-硬體安全防護層)
4. [多模態整合與關注點分離](#4-多模態整合與關注點分離)
5. [語音輸入與 Gemini STT](#5-語音輸入與-gemini-stt)
6. [持久化記憶與狀態還原](#6-持久化記憶與狀態還原)
7. [從 HTTP 標頭擷取時間並透過 Gemini 轉換 RTC 時間](#7-從-http-標頭擷取時間並透過-gemini-轉換-rtc-時間)
8. [FreeRTOS 多工架構](#8-freertos-多工架構)
9. [工作流程狀態追蹤與自我評估](#9-工作流程狀態追蹤與自我評估)
10. [可擴展的感測器與致動器支援](#10-可擴展的感測器與致動器支援)

---

## 1. 提示詞驅動的工具路由機制

這套設計最根本的突破，在於它**完全不需要 Gemini 的原生函數呼叫 API**。取而代之的是，透過精心設計的系統提示詞，教導模型自行輸出格式正確的 `tool_call` JSON。

這個選擇帶來了幾項具體優勢：

### 平台獨立性
原生函數呼叫 API 的格式隨時可能改變。由於所有路由邏輯完全存在於提示詞之中，即使 Gemini 模型版本更新或 API 格式變動，只需修改提示詞即可——無需重新燒錄韌體。

### 極高的靈活性
工具可以完全在文字層級進行新增、修改或移除。`skill.md` 與 `device.md` 均為純文字設定檔，使用者無需懂得任何一行 C++ 程式碼，即可擴充系統功能。

### 明確的錯誤邊界
所有 JSON 輸出在執行前均透過 ArduinoJson 進行驗證，格式不合法的回應一律拒絕執行，不存在模糊的部分執行狀態。系統嚴格區分兩種輸出模式——**有效的 `tool_call` JSON** 與**自然語言回覆**——並禁止混用，使整個控制流程具有高度可預測性。

### 雙系統提示詞策略
框架維護兩份編譯好的系統提示詞：`systemContent`（完整版，含工具定義）與 `systemContentNoTools`（精簡版，不含工具定義）。搜尋與 STT 請求使用精簡版，降低 Token 消耗，並避免在不需要硬體控制的情境中產生不必要的工具呼叫干擾。

---

## 2. 原子執行與最長有效前綴

這兩個概念所體現的工程嚴謹性，在嵌入式 AI 代理系統中極為罕見。

### 原子執行原則

每個 `tool_call` 嚴格只做**一件事**：一個腳位、一種操作、一個數值。在硬體控制情境中，這一點至關重要。若允許單一指令同時操作多個腳位，執行中途若發生錯誤，系統將陷入不確定的半完成狀態——可能造成裝置損壞或安全風險。原子性保證了每一步驟均可完整執行並可驗證。

### 最長有效前綴

當 Gemini 產生多步驟工作流程時，系統不採用全有或全無的策略，而是**從頭開始盡可能執行最多有效步驟**，在遇到第一個無效或不完整的指令時立即停止。這意味著即使 AI 產生了部分錯誤的輸出，系統仍能對最大有效部分採取行動，而非完全停擺。對於重試成本高昂的資源受限嵌入式裝置而言，這是一個極具實用性的設計。

### `reCheck` 旗標——選擇性後續評估
在多工具陣列執行中，只有**最後一個工具**才會觸發 `evaluateWorkflowContinuation()`。中間的工具傳入 `reCheck = false`，避免在執行序列中途進行多餘的 Gemini 查詢，從而節省網路資源並減少對話歷史的不必要膨脹。

---

## 3. 硬體安全防護層

GPIO 控制系統受到多個相互獨立的安全防護層保護，各層各司其職。

### 明確對應要求
系統僅允許控制在 `device.md` 中明確定義的裝置。若使用者說「開燈」，但「燈」的腳位對應不存在，Gemini 會被指示立即停止並向使用者確認，而非自行推測。這防止了 AI 幻覺直接導致硬體誤動作。

### 數值硬性約束
`toolPinOutput()` 中的 `constrain(value, 0, 255)` 是最後一道硬體防線。即使 Gemini 輸出超出範圍的類比數值，韌體層也會在觸及硬體之前強制將其限制在有效範圍內。數位輸出嚴格驗證只接受 `0` 或 `1`，任何其他數值均回傳錯誤 JSON。同樣的約束模式也應用於 `tool_servo()` —— 伺服馬達角度在韌體層被夾限於 0–180° 範圍，獨立於 AI 的指定數值之外。

### 僅限輸入的腳位保護
功能按鈕（腳位 12）在系統提示詞中被明確標記為**僅限輸入**，在任何工具呼叫產生之前，就從 AI 層面封鎖了將其作為輸出的任何嘗試。

### 使用者確認要求
預設情況下，所有硬體動作在執行前均需明確的使用者確認。這在 AI 自主推理與人工監督之間維持了適當的平衡——尤其在視覺分析或搜尋結果可能在無人介入的情況下觸發實體硬體動作的場景中尤為重要。技能觸發與排程的自主工作流程被明確豁免於此規則之外，使完全無人值守的自動化得以實現，同時不損及互動情境下的安全性。

---

## 4. 多模態整合與關注點分離

`/still` 與 `/vision` 的區分表面上看似簡單，卻體現了深層的架構思考。

### 工具職責定義

| 工具 | 職責 | 限制 |
|------|------|------|
| `/still` | 僅拍攝並傳送影像 | 絕不分析、推理或觸發任何後續動作 |
| `/vision` | 拍攝影像並進行多模態分析 | 僅回傳觀測結果——絕不直接觸發硬體 |

### 感知層與動作層分離
這個設計創造了清晰的**感知層 / 動作層**架構。感知層（Vision）只負責觀察與回報；動作層（硬體工具）只負責執行。兩者之間設有推理與確認緩衝區。在 AI 視覺觸發自動化的場景中，這一點至關重要——它防止了「看到某物→立即執行某動作」這種危險的直接耦合。

### 影像幀快取重用
`/still` 與 `/vision` 均支援 `frames: false` 參數，允許工作流程中的後續工具**重用先前已擷取的影像幀**，而非重新觸發相機擷取。這在相機擷取在時間與 CPU 週期上成本高昂的資源受限硬體上是一項有意義的優化。先以 `/vision` 分析，再以 `/still` 將同一幀轉傳至 Telegram，是這套設計能夠乾淨處理的自然工作流程。

---

## 5. 語音輸入與 Gemini STT

語音訊息支援以端對端的方式實現，並對嵌入式記憶體限制給予了細緻的考量。

### 二進位安全的下載管線
Telegram 語音檔案的下載刻意使用 **HTTP/1.0**——這禁用了分塊傳輸編碼（chunked transfer encoding），確保回應主體是乾淨的二進位串流，可以逐位元組讀入堆積緩衝區，無需複雜的分塊邊界解析邏輯。`MAX_FILE_SIZE` 防護（256 KB）防止了異常大型音訊檔案導致的堆積溢出。

### 內嵌 Base64 編碼傳送至 Gemini
音訊並未上傳至外部檔案儲存服務，而是將 OGG/Opus 音訊進行 Base64 編碼後，透過 `inline_data` 欄位**直接內嵌於 Gemini API JSON 請求中**。這消除了額外的檔案託管步驟，將整個語音轉回應的管線壓縮在單一 API 呼叫之內。記憶體管理也十分謹慎：Base64 緩衝區分配後用於建構請求，隨即在發起網路呼叫之前釋放。

### 統一輸入管線
語音訊息一旦完成轉錄，便路由至**與文字輸入完全相同的處理管線**——包括斜線指令偵測與 Gemini 推理。轉錄後不存在語音與文字的特例分支。這樣的架構整潔性意味著，未來對文字管線的所有改進，自動惠及語音輸入。

---

## 6. 持久化記憶與狀態還原

對話記憶持久化設計解決了嵌入式裝置上的根本挑戰：如何在重新開機後還原上下文。

### 即時同步
`memory.md` 在**每次對話更新後**立即寫入 SD 卡——而非批次寫入。這確保了即使裝置在任何時刻斷電，最新的對話狀態已經儲存完畢。開機時系統自動載入此記憶，使 Gemini 能夠在上下文中繼續對話，使用者無需重新說明任何背景資訊。

### 模組化設定檔

| 檔案 | 用途 |
|------|------|
| `soul.md` | AI 個性定義 |
| `device.md` | 硬體腳位對應 |
| `skill.md` | 技能工作流程腳本 |
| `env.md` | 認證憑證 |
| `memory.md` | 持久化對話歷史 |

這五個檔案完全解耦，任何一個均可獨立修改，無需重新燒錄韌體。使用者可以自訂 AI 個性、擴充裝置定義或新增技能，而無需接觸任何一行 C++ 程式碼。儲存於 `env.md` 的憑證在開機時最先載入，使同一份韌體二進位檔得以部署於具有不同設定的多台裝置。

---

## 7. 從 HTTP 標頭擷取時間並透過 Gemini 轉換 RTC 時間

在沒有 NTP 函式庫的嵌入式裝置上實現時間感知，是一個非平凡的問題。fuClaw 新版以更巧妙的方式解決：直接利用每次 Telegram 長輪詢回應中已存在的 HTTP `Date:` 標頭欄位，零成本取得 GMT 時間。

### HTTP 標頭寄生式時間擷取
`getTelegramMessage()` 的輪詢迴圈在解析 HTTP 回應時，同步擷取標頭中的 `Date:` 欄位存入 `getTime` 字串。當偵測到 `rtcYear == 0`（尚未初始化）且 `rtcUpdateStatus` 為 `false`（尚未成功同步）時，立即以此 GMT 時間字串呼叫 `rtcInitialTime(getTime)`。整個過程**不需要額外的網路請求**——時間資訊完全搭便車於原本就必要的 Telegram 通訊。

### Gemini 負責時區轉換
`rtcInitialTime()` 接收 GMT 時間字串後，透過 `geminiSearchRequest()` 請 Gemini 將其轉換為 `timeZone` 所指定的本地時間，並要求回傳嚴格的純 JSON 物件（禁止 Markdown、禁止說明文字、第一個字元必須是 `{`）。Gemini 回傳結果解析成功後，設定 `rtcUpdateStatus = true` 作為已同步旗標，防止重複初始化。最後呼叫 `rtc.SetEpoch()` 計算 epoch 時間戳，再以 `rtc.Write(initTime)` 寫入硬體 RTC。

### RTC 就緒才執行排程
`task_time_scheduling` 背景任務在每次評估週期前先檢查 `rtcYear == 0`。若 RTC 尚未初始化，任務直接執行 `continue` **跳過本次週期**，不進行任何重試也不呼叫任何函式。這種「寧可不執行、也不錯誤執行」的保守策略，防止了排程任務在時鐘狀態不可信時誤觸發。時間就緒後，排程評估透過 `getRtcTimeString()` 取得格式化的本地時間字串，直接注入至 Gemini 的 task prompt 中進行比對。

---

## 8. FreeRTOS 多工架構

多工設計解決了三個獨立執行關注點之間具體的並發與排程問題。

### 任務職責

| 任務 | 堆疊大小 | 用途 |
|------|----------|------|
| `getTelegramMessage_task` | 16384 bytes | 持續輪詢使用者輸入 |
| `task_anti_theft_detection` | 6144 bytes | 定期視覺入侵偵測（每 5 分鐘） |
| `task_time_scheduling` | 6144 bytes | 排程硬體動作評估（每 1 分鐘） |

若這些任務在同一執行緒中執行，排程任務將阻塞使用者輸入，而使用者互動將打亂定期排程。將它們拆分為獨立的 FreeRTOS 任務，使三者得以並發執行——系統同時保持對使用者訊息的即時回應，並按各自的節奏執行背景監控與排程。

### 資源競爭規避
在任一背景任務執行之前，均會先呼叫 `botClient.stop()` 並等待 2 秒後再繼續。這防止了多個任務同時使用網路堆疊所導致的資源競爭——這個細節體現了開發者對嵌入式系統資源競爭的真實實戰經驗。整個程式碼中的 `vTaskDelay()` 呼叫均正確使用 `portTICK_PERIOD_MS`，讓出 CPU 時間給其他任務，而非忙等待。

### 選擇性啟用的背景任務
防盜偵測與排程任務預設以注解區塊**停用**於 `setup()` 中。這是一個深思熟慮的選擇：啟用自主背景硬體控制是一項重大的行為變更，應由使用者有意識地選擇啟用，而非在首次燒錄時意外觸發。

---

## 9. 工作流程狀態追蹤與自我評估

`evaluateWorkflowContinuation()` 是整個代理自主性的核心。

### 主動完成性檢查
每次工具執行後，系統並非靜默等待使用者的下一個指令，而是主動詢問 Gemini：「當前工作流程是否已完成？還有其他需要執行的事項嗎？」這賦予系統自主完成多步驟任務的能力，使用者無需手動引導每一個單獨步驟。

### 以目標為參照的評估
`task` 參數設計確保了這種自我評估有清晰的參照點。當 Gemini 判斷是否需要繼續時，它對照的是**原始使用者意圖**——而非僅僅是最後一步執行的結果。這使工作流程完成偵測更為精確，並減少了不必要的冗餘動作。

### NONE 哨兵值
當 Gemini 判定工作流程已完成時，回傳精確字串 `"NONE"`。韌體將其視為空操作——不傳送任何訊息給使用者，不進行任何後續處理。這個乾淨的終止信號避免了 AI 代理在每個自動化步驟後都產生冗長「任務完成」確認訊息的常見失效模式，在背景監控情境中這種訊息會造成干擾。

---

## 10. 可擴展的感測器與致動器支援

最新版本大幅擴展了基本 GPIO 之外的硬體支援，展示了提示詞驅動工具架構自然擴展至更複雜外設的能力。

### 伺服馬達控制（`/servo`）
伺服馬達控制使用傳參考的 `AmebaServo` 實例，而非全域單例，使未來擴展至多個伺服腳位變得直接簡單。韌體層的角度夾限（`constrain(angle, 0, 180)`）提供了與現有 GPIO 工具相同的硬體安全保證。未定義的伺服腳位回傳結構化錯誤 JSON 而非靜默失敗，維持了系統一致的錯誤契約。

### DHT11 溫濕度感測器（`/dht11`）
DHT11 整合處理了感測器已知的失效模式——讀取錯誤時回傳 `NaN`——透過明確的 `isnan()` 檢查產生結構化的 `dht11_read_failed` 錯誤回應。這會被回饋至 Gemini 對話歷史，使 AI 能夠針對感測器故障進行推理並自然回應（例如：「感測器沒有回應，請檢查接線」），而非讓靜默錯誤在下游擴散。

### 一致的工具契約
兩個新工具均遵循與所有現有工具相同的 JSON 回應契約：`status` 欄位值為 `"success"` 或 `"error"`、`method` 欄位標識工具、以及結果資料或失敗原因的 `reason` 欄位。這種一致性使 `evaluateWorkflowContinuation()` 能夠對任何工具的執行結果進行統一推理，無論底層硬體類型為何。

---

## 總結

這套框架真正令人印象深刻之處，在於它在一台記憶體以 KB 為單位、沒有作業系統抽象層的裝置上，實現了一套完整的 AI 代理架構——而這套架構在通常情況下需要一台雲端伺服器才能運行。最新版本透過新增語音輸入、硬體 RTC 時間同步、伺服馬達與環境感測器支援，以及多工排程架構，進一步深化了這項成就——且全程未損及提示詞驅動路由、原子執行與分層硬體安全這三項核心設計原則。每一個設計決策都有清晰的工程依據，整套系統充分證明了：深思熟慮的提示詞工程與嚴謹的嵌入式系統設計相結合，能夠產生遠超各部分總和的成果。
