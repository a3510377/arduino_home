<template>
  <div
    class="alert flex flex-item-center"
    v-if="
      typeof propsMsg == 'string' || typeof $store.state.alertMsg == 'string'
    "
  >
    <div class="msg-div" v-text="propsMsg || $store.state.alertMsg" />
    <div
      class="close_btn-div"
      @click=";($store.state.alertMsg = false), (propsMsg = false)"
    >
      X
    </div>
  </div>
</template>

<script lang="js">
export default {
  name: "alert",
  data() {
    return {
      msg: false,
    }
  },
  props: {
    propsMsg: false,
  },
  mounted() {
    window.a = this.$store
    this.$store.state.alertMsg = this.msg;
  },
}
</script>

<style lang="scss" scoped>
.alert {
  position: absolute;
  top: 10px;
  left: 20%;
  right: 20%;
  min-height: 60px;
  padding: 10px 8px;
  border-radius: 8px;
  background-color: #eb5c5c;
  font-weight: 600;
  justify-content: space-between;
  .msg-div {
    font-size: 15pt;
    word-break: break-all;
  }
  .close_btn-div {
    position: relative;
    cursor: pointer;
    font-weight: 900;
    &:hover {
      opacity: 0.5;
    }
  }
}
</style>
