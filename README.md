![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/AmebaPro2_TelegramBot_Gemini_SD_EN.png)  


------------------------------------------------------------
fuClaw AI Telegram Assistant with Gemini Integration
------------------------------------------------------------

Author:
  ChungYi Fu (Kaohsiung, Taiwan)
  https://www.facebook.com/francefu

Repository:
  https://github.com/fustyles/fuClaw

------------------------------------------------------------
Version
------------------------------------------------------------

Prompt-Orchestrated Embedded Agent Edition
Persistent Filesystem Runtime

Build Date: 2026-05-24 18:00

------------------------------------------------------------
Overview
------------------------------------------------------------

fuClaw is an embedded multimodal AI agent framework running
on Realtek Ameba Pro2 devices:

- AMB82-mini
- HUB 8735 Ultra

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

Telegram User
      ↓
Telegram Polling Task
      ↓
Message Router
      ↓
Gemini Reasoning Engine
(Chat / Search / Vision / Workflow)
      ↓
JSON tool_call output
      ↓
ArduinoJson validation
      ↓
Tool Dispatcher
      ↓
Hardware / Search / Vision Execution
      ↓
Result injection into memory
      ↓
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

/digitalwrite   GPIO digital output
/analogwrite    GPIO analog output
/digitalread    GPIO digital input
/analogread     GPIO analog input
/still          Capture image
/vision         Capture + multimodal analysis
/search         Grounded web search
/delay          Pause execution for specified milliseconds
/memory         Runtime memory diagnostics
/log            Show tool execution history
/reset          Reset conversation state
/chat           Natural language reply
/reboot         Reboot the device

------------------------------------------------------------
Persistent Files
------------------------------------------------------------

env.md
  WiFi / Telegram / Gemini credentials

device.md
  Devices definition

skill.md
  Skills definition

soul.md
  Custom assistant personality prompt

memory.md
  Conversation history persistence

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
- AmebaFatFS

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

------------------------------------------------------------
Grok Evaluation
------------------------------------------------------------

## ✨ Highlights & Strengths of fuClaw

**fuClaw** is a highly sophisticated, prompt-orchestrated embedded AI agent framework designed for Realtek Ameba Pro2 devices (AMB82-mini & HUB 8735 Ultra). It represents one of the most complete and thoughtful integrations of Telegram, Google Gemini, multimodal capabilities, and hardware control in the embedded AI space.

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
  - `env.md` — WiFi, Telegram, and Gemini credentials
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

> An embedded multimodal AI agent running on Realtek Ameba Pro2 devices,  
> combining Telegram, Gemini, hardware control, and persistent memory in a single FreeRTOS runtime.

---

## Table of Contents

1. [Prompt-Orchestrated Tool Routing](#1-prompt-orchestrated-tool-routing)
2. [Atomic Execution & Longest Valid Prefix](#2-atomic-execution--longest-valid-prefix)
3. [Hardware Safety Layers](#3-hardware-safety-layers)
4. [Multimodal Integration with Clear Separation of Concerns](#4-multimodal-integration-with-clear-separation-of-concerns)
5. [Persistent Memory & State Recovery](#5-persistent-memory--state-recovery)
6. [FreeRTOS Dual-Task Architecture](#6-freertos-dual-task-architecture)
7. [Workflow State Tracking & Self-Evaluation](#7-workflow-state-tracking--self-evaluation)

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

---

## 2. Atomic Execution & Longest Valid Prefix

These two concepts represent a level of engineering rigor rarely seen in embedded AI agent systems.

### Atomic Execution Rule

Every `tool_call` does exactly **one thing**: one pin, one operation, one value. In hardware control scenarios, this is critical. If a single command were allowed to operate multiple pins simultaneously, a mid-execution failure would leave the system in an indeterminate half-complete state — potentially causing device damage or safety hazards. Atomicity guarantees that every step is complete and verifiable.

### Longest Valid Prefix

When Gemini generates a multi-step workflow, the system does not apply an all-or-nothing strategy. Instead it executes **as many valid steps as possible from the beginning**, stopping the moment it encounters the first invalid or incomplete instruction. This means that even when the AI produces partially incorrect output, the system can still act on the maximum valid portion rather than shutting down entirely. For resource-constrained embedded devices where retries are expensive, this is an exceptionally practical design.

---

## 3. Hardware Safety Layers

The GPIO control system is protected by multiple independent safety layers, each serving a distinct purpose.

### Explicit Mapping Requirement
The system only allows control of devices explicitly defined in `device.md`. If a user says "turn on the light" but no pin mapping for "light" exists, Gemini is instructed to stop and ask for clarification rather than guess. This prevents AI hallucinations from causing direct hardware misfires.

### Hard Value Constraints
The `constrain(value, 0, 255)` call inside `toolPinOutput()` acts as a last line of hardware defense. Even if Gemini outputs an out-of-range analog value, the firmware layer forces it within bounds before it ever reaches the hardware. Digital outputs are strictly validated to accept only `0` or `1`; any other value returns an error JSON response.

### Input-Only Pin Protection
The button pin (pin 12) is explicitly marked as **INPUT ONLY** in the system prompt, blocking any AI-level attempt to use it as an output before a tool call is ever produced.

### User Confirmation Requirement
By default, all hardware actions require explicit user confirmation before execution. This maintains an appropriate balance between autonomous AI reasoning and human oversight — particularly important in scenarios where a Vision analysis or Search result would otherwise trigger a physical hardware action without any human in the loop.

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

### Voice Input Integration
Voice messages from Telegram are transcribed via Gemini STT and fed directly into the same processing pipeline as text input — no extra branching logic required. All multimodal inputs converge into a single unified reasoning and tool-routing flow, keeping the architecture remarkably clean.

---

## 5. Persistent Memory & State Recovery

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

All four files are fully decoupled. Any one of them can be modified independently without reflashing the firmware. This lets end users customize the AI's personality, extend device definitions, or add new skills without touching a single line of C++ code.

---

## 6. FreeRTOS Dual-Task Architecture

The two-task design solves a concrete concurrency problem.

### Task Responsibilities

- **`getTelegramMessage_task`** — Continuously polls for user input
- **`periodicCheck_task`** — Executes scheduled automated skills (e.g., anti-theft detection)

If these ran in the same thread, a scheduled task would block user input, and user interactions would disrupt the periodic schedule. Splitting them into two FreeRTOS tasks allows both to run in parallel — the system simultaneously stays responsive to user messages and executes background monitoring on schedule.

### Resource Conflict Avoidance
Before `periodicCheck_task` executes, it calls `botClient.stop()` and waits before proceeding. This prevents the two tasks from simultaneously using the network connection and causing resource conflicts — a detail that reflects real hands-on experience with embedded systems resource contention.

---

## 7. Workflow State Tracking & Self-Evaluation

`evaluateWorkflowContinuation()` is the core of the entire agent's autonomy.

### Active Completion Checking
After each tool execution, instead of silently waiting for the user's next command, the system actively asks Gemini: *"Is the current workflow complete? Is anything else needed?"* This gives the system the ability to autonomously complete multi-step tasks without requiring the user to manually guide each individual step.

### Goal-Referenced Evaluation
The `task` parameter design ensures this self-evaluation has a clear reference point. When Gemini assesses whether to continue, it compares against the **original user intent** — not just the result of the last execution step. This makes workflow completion detection more accurate and reduces unnecessary redundant actions.

---

## Summary

What makes this framework truly impressive is that it implements a complete AI Agent architecture — one that would ordinarily require a cloud server — on a device where memory is measured in kilobytes and there is no OS abstraction layer. Every design decision has a clear engineering rationale, and the system as a whole demonstrates that thoughtful prompt engineering and careful embedded systems design can produce something far greater than the sum of its parts.

------------------------------------------------------------
Version
------------------------------------------------------------

Prompt-Orchestrated Embedded Agent Edition
Persistent Filesystem Runtime

Build Date: 2026-05-24
------------------------------------------------------------
