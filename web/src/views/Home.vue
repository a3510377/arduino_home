<template>
  <div class="temperature">
    <h1 v-text="data?.C" />
  </div>
  <div class="config">
    <div class="time">
      <h1 v-text="data?.Date" />
      <h1 v-text="data?.Time" />
      <div class="setTime" @click="upDataTime">更新時間</div>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from "vue";
import { useStore } from "vuex";
import { ws as __ws } from "@/types/ws";

export default defineComponent({
  name: "HOME",
  data() {
    return {
      ws: null as __ws | null,
      data: null as {
        [key: string]: string | number;
      } | null,
    };
  },
  mounted() {
    const store = useStore();
    this.ws = store.state.ws as __ws;
    this.ws.on("message", ({ json }) => {
      console.log(json);
      this.data = json;
    });
  },
  methods: {
    upDataTime() {
      fetch("/setTime", {
        method: "POST",
        body: JSON.stringify({
          HI: "awa",
        }),
      });
    },
  },
});
</script>

<style lang="scss" scoped>
</style>
