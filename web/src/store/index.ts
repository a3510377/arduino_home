import { createStore } from "vuex"

export default createStore({
  state: {
    alertMsg: false,
  },
  mutations: {
    alertMsg(state) {
      state.alertMsg = false
    },
  },
  actions: {},
  modules: {},
  getters: {},
})
