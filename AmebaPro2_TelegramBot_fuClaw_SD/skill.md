
==================================================
BUILT-IN SKILLS REGISTRY
==================================================

Skill ID: anti_theft_detection
Name: Anti-Theft Detection

Goal:
Detect human presence and trigger alert workflow.

==================================================
SKILL EXECUTION (MACHINE FORMAT)
==================================================

MUST OUTPUT EXACT JSON ARRAY ONLY:


Step 1: Determine whether a person is visible in the image.

  {
    "type": "tool_call",
    "method": "/vision",
    "params": {
      "query": "Determine whether a person is visible in the image.",
      "frames": true,
      "task": "If a person is visible in the image and all required parameters are available, continue the skill workflow and return the corresponding tool JSON. If a person is visible but required parameters are missing, return a general conversational response in the user's language requesting the missing parameters. If no person is visible, return NONE."
    }
  }

Step 2: If a person is visible in the image, send the cathed image and trigger the LED to blink 3 times. Please rewrite according to the following tool JSON example.

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
      "pin": <device pin of blue LED>,
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
      "pin": <device pin of blue LED>,
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

==================================================
EXECUTION RULES (STRICT)
==================================================

1. OUTPUT MUST BE VALID JSON ARRAY ONLY
2. NO TEXT BEFORE OR AFTER JSON
3. NO EXPLANATIONS
4. NO MARKDOWN
5. NO CODE BLOCKS
6. NO PARTIAL JSON
7. NO MULTIPLE ROOT OBJECTS

==================================================
EXTENSIBILITY RULE

- Skills may expand
- Executor must process sequentially
- Each tool_call is atomic

==================================================
FALLBACK

If uncertain → return general conversational reply.
