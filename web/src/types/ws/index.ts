import EventEmitter from "events";

export class ws extends EventEmitter {
  ws: WebSocket;
  constructor(private url: string) {
    super();
    this.ws = new WebSocket(url);
    this.addEvent();
  }
  get src() {
    return this.url;
  }
  set src(url: string) {
    this.src = url;
    this.upWs();
    this.addEvent();
  }
  private upWs() {
    this.ws = new WebSocket(this.url);
    this.addEvent();
  }
  private addEvent() {
    this.ws.onclose = () => {
      this.emit("close");
      this.upWs();
    };
    this.ws.onopen = () => {
      this.emit("open");
    };
    this.ws.onmessage = (data) => {
      let json = null;
      try {
        json = JSON.parse(data.data || "{}");
      } catch {
        return this.emit("error", "格式錯誤");
      }
      this.emit("message", { json, data });
    };
    this.ws.onerror = (error) => {
      this.emit("error", error);
    };
  }
}
