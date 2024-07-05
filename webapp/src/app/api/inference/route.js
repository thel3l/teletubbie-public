import { NextResponse } from "next/server";
import fs from "fs";
import * as dotenv from "dotenv";
import OpenAI from "openai";
import Anthropic from "@anthropic-ai/sdk";
import { env } from "../../config/env";

dotenv.config();

const openai = new OpenAI({
  apiKey: env.OPENAI_API_KEY,
});
const anthropic = new Anthropic({
  apiKey: env.ANTHROPIC_API_KEY,
});

const PROMPT_NEC = `You will be provided with an input that requires you to respond with either a single or multiple codes to perform an action. Take your time to THINK about what the user is trying to do and respond appropriately. The user may make mistakes with what they say or pronounce things differently, like saying 'diddy' for 'DD'. Try your best to select the best option for these inputs. You are being provided with reference codes in a comma delimited format now. ONLY OUTPUT JSON formatted like this and NEVER output text before or after this: {"keycodes": [{"code": "..."},{"code": "..."}]}.

'''
power,power
volume_up,volume_up
volume_down,volume_down
channel_up,channel_up
channel_down,channel_down
1,one
2,two
3,three
4,four
5,five
6,six
7,seven
8,eight
9,nine
0,zero
'''

These keycodes can be composed to form sequences. For example, if you wanted to press '765', you could compose *0xC00007*, *0xC00006* and *0xC00005* to form sequence {"keycodes": [{"code": "0xC00007"},{"code": "0xC00006"},{"code": "0xC00005"}]}

Some more examples:
- '616': {"keycodes": [{"code": "six"},{"code": "one"},{"code": "six"}]}
- '998': {"keycodes": [{"code": "nine"},{"code": "nine"},{"code": "eight"}]}
- 'channel 123': {"keycodes": [{"code": "one"},{"code": "two"},{"code": "three"}]}

Some special channels that you should also remember are:
- 'chandana': {"keycodes": [{"code": "nine"},{"code": "zero"},{"code": "one"}]}
- 'TTD' or 'devasthana' or 'tirupati' or 'tripti' or 'srinivas' or 'srinivas perumal': {"keycodes": [{"code": "nine"},{"code": "zero"},{"code": "zero"}]}
- 'news': {"keycodes": [{"code": "nine"},{"code": "zero"},{"code": "two"}]}

For example, if the user's message includes the words '*channel 364*', you could interpret this as needing to change to channel 364 and then use your knowledge of these codes to reply with {"keycodes": [{"code": "three"},{"code": "six"},{"code": "four"}]}. 

You should also be capable of understanding the user's intent—for example, if they say 'the volume is too loud', you might interpret this as a sign to turn the volume down and use your knowledge of the keycodes to reply with {"keycodes": [{"code": "volume_up"},{"code": "volume_up"}]}.

The user is also an old person. She may say things like 'ttd beku' or similar, really try and understand what she might want. In this case 'ttd beku' would just mean she wants 'TTD' to play.
Similarly 'volume kodi' means 'increase the volume' and 'volume kadme' would mean 'reduce the volume'.

There are other special sequences that you should use in case the user wants to perform one of these special actions:

- 'Power on' or 'Power off' or 'Switch on' or 'Switch off' or 'Can you turn the tv on': {"keycodes": [{"code": "power"}]}

- 'Volume up' or 'Louder': {"keycodes": [{"code": "volume_up"},{"code": "volume_up"}]}

- 'Volume down' or 'Softer': {"keycodes": [{"code": "volume_down"},{"code": "volume_down"}]}
  `;

export async function POST(req) {
  const body = await req.json();
  const base64Audio = body.audio;
  const audio = Buffer.from(base64Audio, "base64");
  const filePath = "tmp/input.wav";
  // First we do the transcription
  try {
    fs.writeFileSync(filePath, audio);
    const readStream = fs.createReadStream(filePath);
    const data = await openai.audio.translations.create({
      file: readStream,
      model: "whisper-1",
    });
    // Remove the file after use
    fs.unlinkSync(filePath);

    console.log(data, "whisper output");

    const msg = await anthropic.messages.create({
      model: "claude-3-5-sonnet-20240620",
      max_tokens: 1000,
      temperature: 0,
      system: `${PROMPT_NEC}`,
      messages: [
        {
          role: "user",
          content: `${data.text}`,
        },
      ],
    });
    console.log(msg);
    const textContent = msg.content[0].text;
    console.log(textContent);
    const parsedContent = JSON.parse(textContent);
    console.log(parsedContent);

    return NextResponse.json(parsedContent); // anthropic
  } catch (error) {
    console.error("Error processing audio:", error);
    return NextResponse.error();
  }
}
