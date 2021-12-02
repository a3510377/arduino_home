let ws = new WebSocket(`ws://${location.host}/ws`);
ws.onopen = () => {
  console.log("is link");
};

ws.onclose = () => {
  console.log("close");
};

ws.onmessage = (data) => {
  try {
    console.log(JSON.parse(data.data) || "{}");
  } catch {
    console.error("格式錯誤");
  }
};
