import { NextApiRequest, NextApiResponse } from "next";
import * as dotenv from "dotenv";

import { env } from "../../config/env";
import { NextResponse } from "next/server";

dotenv.config();
// const esp = env.ESP_URL;
const esp = "http://192.168.0.125/post";

export async function POST(req) {
  const body = await req.json();
  console.log("received body", body);

  try {
    const keycodes = body.keycodes;

    if (keycodes && Array.isArray(keycodes)) {
      const messages = keycodes.map(
        (keycode) => `plain=${encodeURIComponent(keycode.code)}`
      );
      const promises = messages.map((message) =>
        fetch(esp, {
          method: "POST",
          headers: {
            "Content-Type": "application/x-www-form-urlencoded",
          },
          body: message,
        })
      );

      const responses = await Promise.all(promises);
      const data = await Promise.all(
        responses.map((response) => response.text())
      );

      console.log(data);
      return NextResponse.json({ data });
    } else {
      return NextResponse.json({ error: "Invalid request body" });
    }
  } catch (error) {
    return NextResponse.json({
      error: "Failed to get a response from the ESP",
    });
  }
}
