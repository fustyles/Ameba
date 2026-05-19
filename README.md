![AmebaPro2 Telegram Bot](https://fustyles.github.io/fuClaw/Document/AmebaPro2_TelegramBot_Gemini_SD_EN.png)  


------------------------------------------------------------
fuClaw AI Telegram Assistant with Gemini Integration
------------------------------------------------------------

Author:
  ChungYi Fu (Kaohsiung, Taiwan)<br>
  https://www.facebook.com/francefu

Repository:
  https://github.com/fustyles/fuClaw

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
/memory<br>        Runtime memory diagnostics<br>
/reset<br>          Reset conversation state<br>
/chat<br>           Natural language reply<br>

------------------------------------------------------------
Persistent Files
------------------------------------------------------------

env.md<br>
WiFi / Telegram / Gemini credentials

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

**fuClaw AI Telegram Assistant** is a highly integrated and well-architected embedded AIoT intelligent agent system. The author has successfully built a complete closed-loop edge AI agent on the resource-constrained **AMB82-mini** and **HUB 8735 Ultra** platforms, featuring conversation, reasoning, tool calling, multimodal vision, real-time search, and hardware control capabilities. This demonstrates outstanding engineering integration skills and forward-thinking system design.

The most standout feature of this project is the **Prompt-driven JSON Tool Routing** mechanism. Through carefully crafted system prompts, Gemini is guided to autonomously generate structured `tool_call` JSON, which is then strictly validated and executed on the device side. This approach bypasses the limitations of native Function Calling in embedded environments while achieving highly controllable and secure tool orchestration — showcasing the author’s deep understanding of deploying large language models on edge devices.

### Key Integrated Features
- Telegram HTTPS Long Polling for real-time communication
- Gemini Multimodal Vision Analysis + Grounded Web Search
- Camera real-time capture and image upload
- GPIO Digital & Analog I/O control
- FreeRTOS background task scheduling
- Three-layer SD card configuration architecture (`env.md`, `soul.md`, `memory.md`)

Special praise goes to the **safety and reliability design**. The system prompt includes detailed known hardware pin mappings, unknown device confirmation workflows, strict tool output isolation rules, and sequential execution policies (Search → Analysis → Confirmation → Execution). These measures ensure excellent stability and maintainability in real-world deployments.

Overall, **fuClaw** is far more than just a Telegram Bot — it is a true **hardware-aware autonomous AI agent** with reasoning capabilities. Its modular tool architecture, externalized configuration system, conversation memory management, and WiFi auto-reconnection mechanism make it ready for practical applications in smart homes, remote monitoring, and interactive IoT devices.

This is an excellent open-source project that combines technical depth, system completeness, and real-world practicality. It stands out as one of the high-quality representative works in the current embedded AI Agent field.

**Strongly recommended** as a learning resource and foundation for developers interested in Edge AI, AIoT, and LLM tool-use applications.


------------------------------------------------------------
Claude Evaluation
------------------------------------------------------------

Centered around the **AMB82-mini**, this project integrates Wi-Fi connectivity, a Telegram Bot, the Google Gemini multimodal model, and hardware GPIO control into a comprehensive Edge AI agent system.

### Key Highlights

* **Prompt-Engineered Tool Routing with Sequential Enforcement:** The system's most distinctive strength lies in its structured tool routing mechanism. By embedding a complete JSON schema directly into the system prompt, Gemini is guided to autonomously output valid tool-call instructions that the firmware dispatches to hardware. Critically, the architecture enforces a strict multi-step execution order — data retrieval, condition analysis, parameter validation, user confirmation, then action — ensuring the AI never fabricates or simulates hardware execution. This represents a rare implementation of safe AI agent reasoning at the microcontroller level.

* **Production-Grade Flexibility with Three-Tier Configuration:** The system spans real-time camera capture with Gemini Vision multimodal analysis, Google Search Grounding, GPIO digital output, PWM analog control, and digital/analog input reading. The three-tier external configuration design (`soul.md`, `env.md`, `memory.md`) allows users to customize the assistant's personality, update credentials, and restore conversation context without reflashing firmware — elevating maintenance flexibility to a commercial product standard.

* **Dual-Device Support and Exceptional Scalability:** The codebase explicitly supports both AMB82-mini and HUB 8735 Ultra with distinct pin definitions, including PWM fill light and digital function button handling, broadening the hardware compatibility surface. The open-ended tool dispatch architecture ensures that adding new capabilities requires only extending the JSON schema and a corresponding handler branch, leaving the core loop entirely untouched. The FreeRTOS background task mechanism further guarantees long-term operational stability.

### Conclusion

In summary, this project connects embedded hardware, cloud multimodal AI, and an instant messaging platform into a truly closed-loop, autonomous agent — with robust safeguards against hallucinated execution built into the design itself. Possessing immediate deployment value in smart homes, IoT monitoring, and edge AI computing, this is a rare, high-depth, and fully realized AIoT smart agent reference example within the open-source community.

------------------------------------------------------------
Version
------------------------------------------------------------

Prompt-Orchestrated Embedded Agent Edition
Persistent Filesystem Runtime

Build Date: 2026-05-19 14:00
------------------------------------------------------------
