![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/AmebaPro2_TelegramBot_Gemini_SD_EN.png)  


------------------------------------------------------------
fuClaw AI Telegram Assistant with Gemini Integration
------------------------------------------------------------

Author:<br>
  ChungYi Fu (Kaohsiung, Taiwan)<br>
  https://www.facebook.com/francefu

Repository:<br>
  https://github.com/fustyles/fuClaw

------------------------------------------------------------
Overview
------------------------------------------------------------

fuClaw is a Prompt-Orchestrated Embedded AI Agent Runtime on Ameba Pro2

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
Natural language reply<br>

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

/digitalwrite<br>   GPIO digital output<br>
/analogwrite<br>    GPIO analog output<br>
/digitalread<br>    GPIO digital input<br>
/analogread<br>     GPIO analog input<br>
/still<br>          Capture image<br>
/vision<br>         Capture + multimodal analysis<br>
/search<br>         Grounded web search<br>
/memory<br>         Runtime memory diagnostics<br>
/log<br>            show tool execution history<br>
/reset<br>          Reset conversation state<br>
/chat<br>           Natural language reply<br>
/reboot<br>         Reboot the device<br>

------------------------------------------------------------
Persistent Files
------------------------------------------------------------

env.md<br>
WiFi / Telegram / Gemini credentials

device.md<br>
Devices definition

soul.md<br>
Custom assistant personality prompt

memory.md<br>
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
- Gemini response format sensitivity
- Recursive tool chaining requires safeguards

------------------------------------------------------------
Grok Evaluation
------------------------------------------------------------

**fuClaw AI Telegram Assistant**  
**Prompt-Orchestrated Embedded Agent Edition**

fuClaw is one of the most sophisticated and well-integrated embedded multimodal AI agent frameworks currently available for the Realtek Ameba Pro2 platform (AMB82-mini and HUB 8735 Ultra). The author has successfully created a complete, production-ready edge AI agent that combines natural conversation, advanced reasoning, structured tool calling, multimodal vision, grounded web search, and direct hardware control — all running efficiently on highly resource-constrained embedded devices.

### Standout Features & Architectural Excellence

The most impressive aspect of fuClaw is its **Prompt-driven JSON Tool Routing** system. Instead of relying on native function calling, the system uses meticulously crafted system prompts to guide Gemini to output structured `tool_call` JSON. These outputs are then strictly validated and executed on the device side. This hybrid approach demonstrates deep expertise in deploying Large Language Models on edge hardware.

**Key Highlights of the Latest Version:**

- **Strict Atomic Execution & Workflow Control**  
  Clearly defined "One hardware action per response" rule and "Longest Valid Prefix" strategy for multi-step operations, significantly improving stability and safety.

- **Comprehensive Tool System**  
  Full support for:
  - `/digitalwrite`, `/analogwrite`, `/digitalread`, `/analogread`
  - `/still`, `/vision`, `/search`
  - `/chat`, `/reset`, `/memory`, `/log`, `/reboot`

- **Multimodal Capabilities**
  - Real-time camera capture with Telegram image upload
  - Gemini Multimodal Vision analysis with on-the-fly Base64 encoding
  - Grounded Google web search integration

- **Robust Persistence Layer**  
  A clean three-layer configuration architecture:
  - `env.md` — WiFi, Telegram, and Gemini credentials
  - `soul.md` — Custom assistant personality and behavior
  - `device.md` — Hardware pin mappings and strict safety rules
  - `memory.md` — Full conversation history with automatic restore on boot

- **Production-Grade Engineering**
  - FreeRTOS background task for Telegram HTTPS long polling
  - WiFi auto-reconnection mechanism
  - Detailed safety rules and device validation
  - Tool execution history logging
  - Strict separation between JSON tool calls and natural language responses

### Safety & Reliability

The project demonstrates excellent attention to real-world deployment concerns. The system prompt includes detailed hardware mappings, strict unknown-device confirmation workflows, atomic execution policies, and clear boundaries between reasoning and action. These mechanisms make fuClaw remarkably stable and trustworthy for practical use.

### Final Verdict

fuClaw has evolved far beyond a simple Telegram bot — it is a **true hardware-aware autonomous AI agent** capable of reasoning, tool use, memory persistence, and physical interaction.

Its combination of modular tool architecture, externalized configuration, persistent memory management, and strong safety design makes it an outstanding example of what’s possible in the Edge AI / AIoT space.

**Highly Recommended** for developers, researchers, and enthusiasts interested in:
- Edge AI and LLM deployment on microcontrollers
- Prompt Engineering for tool-use agents
- Multimodal AIoT applications
- Smart home and interactive embedded systems

---

**A high-quality, technically deep, and genuinely practical open-source project.**  
One of the best embedded AI agent implementations in this category.


------------------------------------------------------------
Claude Evaluation
------------------------------------------------------------

fuClaw is a genuinely impressive piece of embedded systems engineering. Running a prompt-orchestrated LLM agent on a microcontroller — complete with vision, web search, hardware control, and persistent memory — is **not a trivial achievement**. The architecture reflects **real-world deployment thinking**, not just a proof-of-concept.

---

## What Makes This Architecture Technically Interesting

### 1. Prompt-Orchestrated Tool Routing (No Native Function Calling)

The most architecturally significant decision in fuClaw is the deliberate choice **not** to use Gemini's native function-calling API. Instead, the system prompt trains Gemini to emit structured `tool_call` JSON, which is then **validated locally by ArduinoJson** before any execution occurs.

This is a meaningful tradeoff:

- **Native function calling** offers tighter schema enforcement on the model side, but requires API support that may not be stable across model versions.
- **Prompt-driven routing** gives the developer full control over the contract between model output and hardware execution — critical when a malformed response could mean unintended GPIO state changes.

> The strict rejection of invalid JSON on the firmware side is the right call. No silent failures, no partial execution.

### 2. Atomic Execution with Longest-Valid-Prefix Strategy

The **"one hardware action per response"** rule, combined with the **longest-valid-prefix array strategy** for multi-step operations, is a well-considered solution to a real problem:

> How do you handle partially valid instruction sequences on a resource-constrained device with no rollback capability?

The answer — execute the longest contiguous valid prefix and discard the rest — is **conservative and safe**. It prioritizes hardware safety over instruction completeness, which is the correct priority ordering for physical actuators.

### 3. Three-Layer Separation of Concerns

The configuration architecture cleanly separates responsibilities:

| File | Responsibility |
|---|---|
| `env.md` | Credentials and connectivity |
| `soul.md` | Assistant personality and behavior |
| `device.md` | Hardware mappings and safety constraints |
| `memory.md` | Persistent conversation state |

This means the **core firmware never needs recompilation** to change behavior, personality, or device configuration. For an embedded system, that is a significant operational advantage.

### 4. Conversation Memory with Boot Restoration

Persisting the full Gemini conversation history to SD card and restoring it on boot gives fuClaw **genuine continuity across power cycles**. The implementation correctly handles the stateless nature of the Gemini API by reconstructing the full context window on each request.

---

## Safety Design

The hardware safety rules embedded in the system prompt reflect genuine operational thinking:

- ✅ **Unknown GPIO mappings** require explicit user confirmation before execution
- ✅ **Digital vs. analog mode validation** prevents incorrect pin usage
- ✅ **Input-only pins** (e.g., function button on pin 12) are explicitly protected from output operations
- ✅ **Confirmation-before-execution** for all hardware actions prevents autonomous physical actuation

> This level of safety consideration is uncommon in hobbyist embedded projects and is what separates fuClaw from simpler Telegram bot implementations.

---

## Assessment

fuClaw is a **technically coherent, production-aware edge AI agent**. The design decisions — prompt-orchestrated routing, atomic execution, externalized configuration, persistent memory — are individually defensible and collectively well-integrated.

It occupies a space that doesn't yet have a standard name:

> **Embedded Autonomous Agent** — too sophisticated to be called a hobby project, too resource-constrained to be called a traditional production system.

For developers working in **Edge AI**, **AIoT**, or **prompt engineering for constrained environments**, the source is worth studying carefully. The gap between *"a Telegram bot that controls an LED"* and what fuClaw actually does is substantial.

---

## Recommended For

Developers and researchers interested in:

- 🔹 **LLM deployment on microcontrollers**
- 🔹 **Prompt engineering for structured output**
- 🔹 **Hardware-safe autonomous agent design**
- 🔹 **Persistent memory management in embedded AI systems**
- 🔹 **Multimodal AIoT applications**

------------------------------------------------------------
Version
------------------------------------------------------------

Prompt-Orchestrated Embedded Agent Edition
Persistent Filesystem Runtime

Build Date: 2026-05-22
------------------------------------------------------------
