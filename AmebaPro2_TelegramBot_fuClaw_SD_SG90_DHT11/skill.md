
==================================================
BUILT-IN SKILLS REGISTRY
==================================================

==================================================
SKILL: anti_theft_detection
==================================================

Goal:
Detect human presence and trigger alert workflow.

--------------------------------------------------
SKILL EXECUTION
--------------------------------------------------

MUST OUTPUT EXACT JSON ARRAY ONLY:

Step 1: Analyze image for human presence

{
  "type": "tool_call",
  "method": "/vision",
  "params": {
    "query": "Determine whether a person is visible in the image.",
    "frames": true,
    "task": "If a person is detected, continue workflow. If no person is detected, return NONE."
  }
}

Step 2: If person detected → trigger alert sequence

[
  {
    "type": "tool_call",
    "method": "/still",
    "params": {
      "frames": false,
      "task": "NONE"
    }
  },
  {
    "type": "tool_call",
    "method": "/digitalwrite",
    "params": {
      "pin": <blue_led_pin>,
      "pinmode": "digitalwrite",
      "value": 1
    }
  },
  {
    "type": "tool_call",
    "method": "/delay",
    "params": {
      "milliseconds": 500
    }
  },
  {
    "type": "tool_call",
    "method": "/digitalwrite",
    "params": {
      "pin": <blue_led_pin>,
      "pinmode": "digitalwrite",
      "value": 0
    }
  },
  {
    "type": "tool_call",
    "method": "/delay",
    "params": {
      "milliseconds": 500
    }
  }
]

--------------------------------------------------
FALLBACK
--------------------------------------------------

If uncertain → return natural conversational response.

==================================================
SKILL: time_scheduling_task
==================================================

Goal:
Execute scheduled hardware actions at correct time using timezone-aware validation.

--------------------------------------------------
SKILL EXECUTION
--------------------------------------------------

MUST OUTPUT EXACT JSON ARRAY ONLY:

--------------------------------------------------
Step 0: Parse scheduled task
--------------------------------------------------

Extract from conversation:

- execution time
- hardware action

If no valid time → treat as normal conversation

--------------------------------------------------
Step 1: Verify timezone
--------------------------------------------------

{
  "type": "tool_call",
  "method": "/chat",
  "params": {
    "reply": "Check whether timezone is known from conversation context."
  }
}

IF timezone is UNKNOWN:
→ ask user for city / region / timezone
→ STOP (no tool calls)

--------------------------------------------------
Step 2: Get current time
--------------------------------------------------

{
  "type": "tool_call",
  "method": "/search",
  "params": {
    "query": "current local time in user timezone",
    "task": "Compare current time with scheduled tasks in conversation history."
  }
}

--------------------------------------------------
Step 3: Decision logic
--------------------------------------------------

IF current_time < scheduled_time:
RETURN EXACTLY:
NONE

IF current_time >= scheduled_time AND task not executed:
RETURN ONLY valid tool_call JSON

--------------------------------------------------
CRITICAL RULES
--------------------------------------------------

1. Scheduled tasks override normal confirmation rules
2. Do NOT ask user for current time
3. Do NOT execute before scheduled time
4. Do NOT simulate execution success
5. Execution success only valid after tool response
6. Time check MUST always include task context
7. /search is ONLY for time retrieval, not decision making

--------------------------------------------------
TASK REGISTRATION RULE
--------------------------------------------------

When user gives schedule (e.g. "10:56 turn on green LED"):

1. Store task in memory
2. Confirm task recorded
3. Inform scheduler must be enabled
4. Do NOT execute immediately

Example:
"I've recorded your scheduled task. It will execute when system scheduler is active."

--------------------------------------------------
FALLBACK
--------------------------------------------------

If no scheduled task exists:
Return natural conversational response only.
