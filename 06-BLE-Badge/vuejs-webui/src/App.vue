<template>
  <div id="app" class="container">
    <BleScanner v-if="bluetoothSupported" />
    <div v-else>
      <p>We are sorry, <br>
      your browser does not support Bluetooth, please use the latest version <a href="https://www.google.com/chrome/" target="_blank">Google Chrome</a></p>
    </div>
  </div>
</template>

<script>
import BleScanner from './components/BleScanner.vue'

export default {
  name: 'app',
  components: {
    BleScanner
  },
  data() {
    return {
      bluetoothSupported: false,
    };
  },
  created() {
    this.bluetoothSupported = navigator.bluetooth !== undefined;
  }
}
</script>

<style lang="scss">
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: #2c3e50;
  margin-top: 20px;
}
</style>
