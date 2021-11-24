<template>
  <Alert />
  <router-view class="wrapper" />
</template>

<script lang="ts">
import { defineComponent } from "vue"
import { useStore } from "vuex"
import Alert from "@/components/alert.vue"

export default defineComponent({
  name: "APP",
  data() {
    return {
      $store: useStore(),
    }
  },
  components: {
    Alert,
  },
})
</script>
<style lang="scss">
@import url(@/assets/scss/main.scss);
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
}
</style>