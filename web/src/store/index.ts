import { createStore } from "vuex";
import { ws } from "@/types/ws";

export default createStore({
  state: {
    alertMsg: false,
    ws: new ws(`ws://${location.host}/ws`),
  },
  mutations: {
    alertMsg(state) {
      state.alertMsg = false;
    },
  },
  actions: {},
  modules: {},
  getters: {},
});
