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
7. [RTC Time Synchronization via Gemini Search](#7-rtc-time-synchronization-via-gemini-search)
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

## 7. RTC Time Synchronization via Gemini Search

Time awareness on an embedded device without a network time protocol stack is a non-trivial problem. fuClaw solves it elegantly.

### Gemini-Synchronized Clock
At boot, the system calls `googleSearchTime()`, which instructs Gemini to perform a live web search for the current local time in the configured timezone. Gemini returns a structured JSON object with year, month, day, hour, minute, and second fields. The firmware parses this directly into the hardware RTC via `rtc.SetEpoch()`. The result is a **network-synchronized hardware clock** achieved entirely through natural language AI tooling — no NTP library required.

### Timezone-Aware Scheduling
The timezone string is appended to `devicesDefinition` at runtime and becomes part of the system context visible to Gemini. Scheduled task evaluation therefore has access to the correct local time context without any hardcoded timezone logic in the firmware itself. Changing the timezone requires only editing `env.md`.

### Lazy RTC Initialization in Scheduled Tasks
The `task_time_scheduling` background task checks `rtcYear == 0` before each evaluation cycle. If the RTC was not successfully initialized at boot (e.g., due to a transient network failure), it transparently retries synchronization before attempting any scheduling logic. This graceful degradation prevents scheduled tasks from firing at incorrect times due to an uninitialized clock.

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
/digitalwrite <br> GPIO 數位輸出<br>
/analogwrite <br> GPIO 類比輸出<br>
/digitalread <br> GPIO 數位輸入<br>
/analogread <br> GPIO 類比輸入<br>
/still <br> 擷取影像<br>
/vision <br> 擷取影像 + 多模態分析<br>
/search <br> Grounded 網路搜尋<br>
/delay <br> 暫停執行指定毫秒<br>
/memory <br> 系統記憶體診斷<br>
/log <br> 顯示工具執行歷史<br>
/reset <br> 重置對話狀態<br>
/chat <br> 自然語言回覆<br>
/reboot <br> 重啟裝置
------------------------------------------------------------
持久化檔案
------------------------------------------------------------
env.md  
  <br>WiFi / Telegram / Gemini 金鑰 / 時區設定  
device.md  
  <br>硬體裝置定義  
skill.md  
  <br>技能定義  
soul.md  
  <br>自訂助理人格提示詞  
memory.md  
  <br>對話歷史持久化  

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
# fuClaw AI Framework — 深度優勢分析
> 一個運行在嵌入式裝置上的多模態 AI Agent，結合 Telegram、Gemini、硬體控制與持久化記憶於單一 FreeRTOS 運行環境。
---
## 目錄
1. [Prompt-Orchestrated Tool Routing](#1-prompt-orchestrated-tool-routing)
2. [原子執行與最長有效前綴](#2-原子執行與最長有效前綴)
3. [硬體安全防護層](#3-硬體安全防護層)
4. [多模態整合與職責清晰分離](#4-多模態整合與職責清晰分離)
5. [持久化記憶與狀態恢復](#5-持久化記憶與狀態恢復)
6. [FreeRTOS 雙任務架構](#6-freertos-雙任務架構)
7. [工作流狀態追蹤與自我評估](#7-工作流狀態追蹤與自我評估)
---
## 1. Prompt-Orchestrated Tool Routing
本設計最根本的突破在於**不需要依賴 Gemini 的原生 function-calling API**。透過精心設計的系統提示，引導模型自主輸出格式正確的 `tool_call` JSON。

此設計帶來以下具體優勢：
### 平台獨立性
原生 function-calling API 格式可能隨時改變。但因為所有路由邏輯都存在於提示詞中，當 Gemini 模型升級或 API 格式改變時，只需修改提示詞即可，無需更改韌體。

### 極高彈性
工具可以在純文字層級進行新增、修改或移除。`skill.md` 與 `device.md` 皆為純文字設定檔，使用者無需懂 C++ 即可擴展系統功能。

### 清晰的錯誤邊界
每個 JSON 輸出都會經過 ArduinoJson 驗證，格式錯誤的回應會被直接拒絕。系統強制區分兩種輸出模式（有效 `tool_call` JSON 或自然語言回覆），禁止混用，使整體控制流程高度可預測。
---
## 2. 原子執行與最長有效前綴
這兩個概念代表了嵌入式 AI Agent 系統中罕見的工程嚴謹度。

### 原子執行規則
每個 `tool_call` 都只做**一件事情**：一個 pin、一個操作、一個數值。在硬體控制情境中，這一點至關重要。如果允許單一指令同時操作多個 pin，執行中途失敗可能會讓系統處於不確定的半完成狀態，甚至造成硬體損壞或安全風險。原子性確保每個步驟都是完整且可驗證的。

### 最長有效前綴
當 Gemini 產生多步驟工作流時，系統並非採用「全部或全不執行」的策略，而是**從頭開始執行盡可能多的有效步驟**，遇到第一個無效或不完整的指令時就停止。這意味著即使 AI 輸出部分錯誤，系統仍能執行最大範圍的有效部分，而非完全停擺。對於資源受限的嵌入式裝置來說，這是非常務實且優秀的設計。
---
## 3. 硬體安全防護層
GPIO 控制系統受到多層獨立的安全防護，每一層都有明確目的。

### 明確映射要求
系統僅允許控制在 `device.md` 中明確定義的裝置。如果使用者說「打開燈光」但沒有對應的 pin 映射，Gemini 會被指示停止並詢問使用者，而非自行猜測。這有效防止 AI 幻覺導致硬體誤動作。

### 硬數值限制
`toolPinOutput()` 函式中的 `constrain(value, 0, 255)` 是最後一道硬體防線。即使 Gemini 輸出超出範圍的類比值，韌體層也會強制限制在安全範圍內。數位輸出則嚴格限定只能接受 `0` 或 `1`，其他數值會回傳錯誤 JSON。

### 輸入專用腳位保護
按鈕腳位（pin 12）在系統提示中被明確標記為**僅輸入**，在工具呼叫產生前就阻擋任何將其當作輸出的嘗試。

### 使用者確認機制
預設情況下，所有硬體動作都需要使用者明確確認後才會執行。這在視覺分析或搜尋結果觸發實體動作的場景中，維持了自主推理與人工監督之間的適當平衡。
---
## 4. 多模態整合與職責清晰分離
`/still` 與 `/vision` 的區分看似簡單，實際上反映了深刻的架構思考。

### 工具角色定義

| 工具     | 職責                     | 限制 |
|----------|--------------------------|------|
| `/still` | 僅擷取並傳送影像         | 不得分析、推理或觸發後續動作 |
| `/vision`| 擷取影像並進行分析       | 僅回傳觀察結果，不得直接觸發硬體 |

### 感知－動作分離
此設計建立了清晰的**感知層 / 動作層**架構。感知層（Vision）僅負責觀察與回報，動作層（硬體工具）僅負責執行，中間透過推理與確認機制緩衝。在 AI 視覺觸發自動化的情境中，這一點極為重要，可避免「看到某物 → 立即執行動作」的危險直接耦合。
---
## 5. 持久化記憶與狀態恢復
對話記憶持久化設計解決了嵌入式裝置的一大難題：如何在重開機後恢復上下文。

### 即時同步
`memory.md` 在**每次對話更新後**都會寫入 SD 卡，而非批次處理。即使裝置突然斷電，最新的對話狀態也已保存。開機時系統會自動載入記憶，讓 Gemini 能夠延續之前的對話脈絡，使用者無需重新說明背景。

### 模組化設定檔案

| 檔案        | 用途 |
|-------------|------|
| `soul.md`   | AI 人格定義 |
| `device.md` | 硬體 pin 映射 |
| `skill.md`  | 技能工作流程 |
| `env.md`    | 認證憑證 |

四個檔案完全解耦，任何一個都可以獨立修改，無需重新燒錄韌體。這讓使用者可以輕鬆自訂 AI 人格、擴展裝置定義或新增技能。
---
## 6. FreeRTOS 雙任務架構
雙任務設計解決了具體的並行處理問題。

### 任務職責
- **`getTelegramMessage_task`** — 持續輪詢使用者訊息
- **`periodicCheck_task`** — 執行定期的自動化技能（例如防盜偵測）

如果兩者運行在同一執行緒，排程任務會阻塞使用者輸入，而使用者互動又會干擾定時排程。拆分成兩個 FreeRTOS 任務後，系統可以同時保持對使用者的即時回應，並在背景執行監控任務。

### 資源衝突避免
在 `periodicCheck_task` 執行前，會先呼叫 `botClient.stop()` 並等待，避免兩個任務同時使用網路連線造成資源衝突，這體現了作者在嵌入式系統資源管理上的實戰經驗。
---
## 7. 工作流程狀態追蹤與自我評估
evaluateWorkflowContinuation() 是整個代理自主能力的核心。

### 主動完成度檢查
每次工具執行完畢後，系統不會靜默等待使用者的下一個指令，而是主動詢問 Gemini：「目前的工作流程完成了嗎？還有什麼需要繼續執行的嗎？」這賦予了系統自主完成多步驟任務的能力，使用者不需要手動引導每一個執行步驟。

### 以目標為基準的評估機制
task 參數的設計確保這個自我評估有明確的參照基準。當 Gemini 判斷是否繼續執行時，比對的對象是使用者的原始意圖，而不只是上一個執行步驟的結果。這讓工作流程的完成判斷更加準確，也減少了不必要的重複動作。

## 總結
這套框架真正令人印象深刻之處，在於它在一台記憶體以 KB 計算、沒有任何作業系統抽象層的裝置上，實現了一套完整的 AI Agent 架構——而這樣的架構在一般情況下需要雲端伺服器才能運行。每一個設計決策背後都有清晰的工程邏輯，整個系統充分說明了：精心設計的提示詞工程，加上嚴謹的嵌入式系統思維，能夠產生遠遠超越各部分總和的成果。
